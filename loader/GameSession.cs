using System.ComponentModel;
using System.Diagnostics;
using System.IO;

namespace Lyn.Loader;

internal sealed record GameTarget(int Pid, long Started, string Path);
internal sealed record GameState(GameTarget? Target, bool Ready, string Title, string Detail, string? LoadedPath = null);

internal static class GameSession
{
    private static readonly string[] Required = ["client.dll", "engine.dll", "vguimatsurface.dll"];

    public static bool HasGameModules(List<Native.ModuleEntry> modules) => Required.All(name =>
        modules.Any(module => string.Equals(module.Name, name, StringComparison.OrdinalIgnoreCase)));

    public static GameState Discover(string dllPath)
    {
        var processes = Process.GetProcessesByName("left4dead2");
        try
        {
            if (processes.Length == 0) return new(null, false, "Juego cerrado", "Abre Left 4 Dead 2 para continuar.");
            if (processes.Length > 1) return new(null, false, "Varias instancias abiertas", "Deja una sola instancia de Left 4 Dead 2 abierta.");
            int pid = processes[0].Id;
            using var process = Native.OpenProcess(Native.QueryLimited, false, pid);
            if (process.IsInvalid) throw Native.Error("Acceder a L4D2");
            Native.RequireX86(process);
            var target = new GameTarget(pid, Native.Started(process), Native.ImagePath(process));
            var modules = Native.Modules(pid);
            var loaded = modules.FirstOrDefault(module =>
                string.Equals(module.Name, string.IsNullOrEmpty(dllPath)?"lyn.dll":Path.GetFileName(dllPath), StringComparison.OrdinalIgnoreCase));
            if (loaded.Base != IntPtr.Zero)
                return new(target, true, "Módulo presente", $"PID {pid} · x86 · {loaded.Name}", loaded.Path);
            bool ready = HasGameModules(modules);
            return new(target, ready, ready ? "Listo para cargar" : "Iniciando el juego", ready
                ? $"PID {pid} · x86 · módulos del juego disponibles"
                : $"PID {pid} · esperando el menú principal...");
        }
        catch (Exception e) when (e is Win32Exception or InvalidOperationException or ArgumentException)
        { return new(null, false, "No se pudo acceder al juego", e.Message); }
        finally { foreach (var process in processes) process.Dispose(); }
    }
}
