using System.IO;
using System.Text;

namespace Lyn.Loader;

internal enum LoadOutcome { Loaded, AlreadyLoaded, Pending }
internal sealed record LoadResult(LoadOutcome Outcome, string Message);
internal sealed class LoadOperationException(string message, bool mayStillBeRunning, Exception inner) : Exception(message, inner)
{
    public bool MayStillBeRunning { get; } = mayStillBeRunning;
}

internal static class LibraryLoader
{
    private static int active;

    public static async Task<LoadResult> LoadAsync(GameTarget target, string path, IProgress<string> progress, CancellationToken cancellation)
    {
        if (Interlocked.CompareExchange(ref active, 1, 0) != 0)
            throw new InvalidOperationException("Ya hay una carga en curso.");
        try { return await Task.Run(() => Load(target, path, progress, cancellation)); }
        finally { Volatile.Write(ref active, 0); }
    }

    private static LoadResult Load(GameTarget target, string path, IProgress<string> progress, CancellationToken cancellation)
    {
        if (IntPtr.Size != 4) throw new InvalidOperationException("Usa la compilación x86 del loader.");
        var dll = PeImage.Inspect(path);
        using var file = new FileStream(dll.FullPath, FileMode.Open, FileAccess.Read, FileShare.Read);
        PeImage.Inspect(file); // Keep this version of the file read-locked until dispatch completes.
        cancellation.ThrowIfCancellationRequested();
        progress.Report("Comprobando el proceso y la DLL...");
        var process = Native.OpenProcess(Native.LoadAccess, false, target.Pid);
        if (process.IsInvalid) { var error = Native.Error("Abrir L4D2 para cargar la DLL"); process.Dispose(); throw error; }
        KernelHandle? thread = null;
        IntPtr argument = IntPtr.Zero;
        bool deferred = false;
        bool mayStillBeRunning = false;
        try
        {
            // Validate through the handle that will be used for the operation,
            // not just the PID saved by the discovery timer.
            if (Native.Started(process) != target.Started ||
                !string.Equals(Native.ImagePath(process), target.Path, StringComparison.OrdinalIgnoreCase) ||
                !string.Equals(Path.GetFileName(target.Path), "left4dead2.exe", StringComparison.OrdinalIgnoreCase))
                throw new InvalidOperationException("La instancia de L4D2 cambió. Actualiza el estado antes de cargar.");
            Native.RequireX86(process);
            var modules = Native.Modules(target.Pid);
            var previous = modules.FirstOrDefault(module => string.Equals(module.Name, dll.Name, StringComparison.OrdinalIgnoreCase));
            if (previous.Base != IntPtr.Zero)
            {
                if (!SamePath(previous.Path, dll.FullPath))
                    throw new InvalidOperationException($"Ya hay otra {dll.Name} cargada desde {previous.Path}. Reinicia el juego para cambiar de versión.");
                return new(LoadOutcome.AlreadyLoaded, "La DLL ya está cargada en esta instancia.");
            }
            if (!GameSession.HasGameModules(modules)) throw new InvalidOperationException("Espera a que L4D2 termine de iniciar.");
            IntPtr loadLibrary = ResolveRemoteLoadLibrary(modules);
            byte[] bytes = Encoding.Unicode.GetBytes(dll.FullPath + '\0');
            argument = Native.VirtualAllocEx(process, IntPtr.Zero, (UIntPtr)bytes.Length, 0x3000, 0x04);
            if (argument == IntPtr.Zero) throw Native.Error("Reservar la ruta de la DLL");
            if (!Native.WriteProcessMemory(process, argument, bytes, (UIntPtr)bytes.Length, out var written))
                throw Native.Error("Enviar la ruta de la DLL");
            if (written.ToUInt64() != (ulong)bytes.Length) throw new IOException("Windows no pudo escribir la ruta completa de la DLL.");
            cancellation.ThrowIfCancellationRequested();
            progress.Report("Windows está cargando la DLL...");
            thread = Native.CreateRemoteThread(process, IntPtr.Zero, UIntPtr.Zero, loadLibrary, argument, 0, out _);
            if (thread.IsInvalid) throw Native.Error("Iniciar la carga de la DLL");
            mayStillBeRunning = true;

            // After dispatch, cancellation must not free an argument still in
            // use by Windows or terminate a thread inside the loader lock.
            uint wait = Native.WaitForSingleObject(thread, 30_000);
            if (wait != Native.Signaled)
            {
                deferred = true;
                _ = Task.Run(() => ReleaseAfterCompletion(process, thread, argument));
                LoaderLog.Write($"Carga pendiente: PID {target.Pid}; espera=0x{wait:X8}. Se conservó la ruta remota hasta finalizar el hilo.");
                return new(LoadOutcome.Pending, "La carga sigue pendiente. Se bloquearon los reintentos en esta instancia; el estado se actualizará si Windows termina de cargarla.");
            }
            mayStillBeRunning = false;
            if (!Native.GetExitCodeThread(thread, out uint moduleHandle)) throw Native.Error("Consultar el resultado de la carga");
            if (moduleHandle == 0)
                throw new InvalidOperationException("LoadLibraryW devolvió NULL. Revisa las dependencias x86 y la inicialización de la DLL. El código de error remoto no está disponible.");
            progress.Report("Confirmando el módulo en L4D2...");
            for (int attempt = 0; attempt < 4; ++attempt)
            {
                var loaded = Native.Modules(target.Pid).FirstOrDefault(module =>
                    unchecked((uint)module.Base.ToInt32()) == moduleHandle && SamePath(module.Path, dll.FullPath));
                if (loaded.Base != IntPtr.Zero)
                    return new(LoadOutcome.Loaded, "DLL cargada y confirmada en L4D2. Pulsa Insert dentro del juego para abrir el menú.");
                Thread.Sleep(100);
            }
            throw new InvalidOperationException($"El hilo terminó (0x{moduleHandle:X8}), pero no se confirmó el módulo. Revisa el registro de la DLL antes de reintentar.");
        }
        catch (OperationCanceledException) { throw; }
        catch (Exception exception)
        { throw new LoadOperationException(exception.Message, mayStillBeRunning, exception); }
        finally
        {
            if (!deferred)
            {
                if (argument != IntPtr.Zero && (thread == null || thread.IsInvalid || Native.WaitForSingleObject(thread, 0) == Native.Signaled))
                    FreeArgument(process, argument);
                thread?.Dispose();
                process.Dispose();
            }
        }
    }

    private static bool SamePath(string a, string b) => string.Equals(Path.GetFullPath(a), Path.GetFullPath(b), StringComparison.OrdinalIgnoreCase);

    private static IntPtr ResolveRemoteLoadLibrary(List<Native.ModuleEntry> modules)
    {
        IntPtr kernel = Native.GetModuleHandleW("kernel32.dll");
        if (kernel == IntPtr.Zero) throw Native.Error("Localizar kernel32.dll");
        IntPtr function = Native.GetProcAddress(kernel, "LoadLibraryW");
        if (function == IntPtr.Zero) throw Native.Error("Localizar LoadLibraryW");
        // The export can be forwarded to KernelBase. Find its actual owner,
        // then apply the relative address to that module in the target.
        if (!Native.GetModuleHandleExW(0x00000004 | 0x00000002, function, out IntPtr owner))
            throw Native.Error("Localizar el módulo de LoadLibraryW");
        var path = new StringBuilder(32768);
        if (Native.GetModuleFileNameW(owner, path, path.Capacity) == 0) throw Native.Error("Consultar el módulo de Windows");
        string ownerPath = path.ToString();
        var remote = modules.FirstOrDefault(module => SamePath(module.Path, ownerPath));
        if (remote.Base == IntPtr.Zero) throw new InvalidOperationException("No se encontró el mismo módulo de Windows en L4D2.");
        uint offset = unchecked((uint)function.ToInt32() - (uint)owner.ToInt32());
        if (offset >= remote.BaseSize) throw new InvalidOperationException("La dirección de LoadLibraryW no pertenece al módulo esperado.");
        return new IntPtr(unchecked((int)((uint)remote.Base.ToInt32() + offset)));
    }

    private static void ReleaseAfterCompletion(KernelHandle process, KernelHandle thread, IntPtr argument)
    {
        try
        {
            while (true)
            {
                uint wait = Native.WaitForSingleObject(thread, 1000);
                if (wait == Native.Signaled) { FreeArgument(process, argument); return; }
                if (Native.WaitForSingleObject(process, 0) == Native.Signaled) return;
                if (wait == Native.WaitFailed) return; // Never free memory that a running thread may read.
            }
        }
        finally { thread.Dispose(); process.Dispose(); }
    }

    private static void FreeArgument(KernelHandle process, IntPtr argument)
    {
        if (Native.WaitForSingleObject(process, 0) == Native.Signaled) return;
        if (!Native.VirtualFreeEx(process, argument, UIntPtr.Zero, 0x8000))
            LoaderLog.Write(Native.Error("Liberar la ruta temporal de la DLL").Message);
    }
}
