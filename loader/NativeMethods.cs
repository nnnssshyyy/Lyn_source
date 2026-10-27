using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Text;
using Microsoft.Win32.SafeHandles;

namespace Lyn.Loader;

internal sealed class KernelHandle : SafeHandleZeroOrMinusOneIsInvalid
{
    public KernelHandle() : base(true) { }
    protected override bool ReleaseHandle() => Native.CloseHandle(handle);
}

internal static class Native
{
    public const uint QueryLimited = 0x1000;
    public const uint LoadAccess = 0x0002 | 0x0400 | 0x0008 | 0x0010 | 0x0020 | 0x100000;
    public const uint Signaled = 0, Timeout = 258, WaitFailed = 0xffffffff;

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct ModuleEntry
    {
        public uint Size, ModuleId, ProcessId, GlobalUsage, ProcessUsage;
        public IntPtr Base;
        public uint BaseSize;
        public IntPtr Module;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)] public string Name;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)] public string Path;
    }

    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern KernelHandle OpenProcess(uint access, [MarshalAs(UnmanagedType.Bool)] bool inherit, int pid);
    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool CloseHandle(IntPtr handle);
    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool IsWow64Process2(KernelHandle process, out ushort machine, out ushort nativeMachine);
    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool GetProcessTimes(KernelHandle process, out long creation, out long exit, out long kernel, out long user);
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool QueryFullProcessImageNameW(KernelHandle process, uint flags, StringBuilder path, ref uint size);
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern KernelHandle CreateToolhelp32Snapshot(uint flags, int pid);
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool Module32FirstW(KernelHandle snapshot, ref ModuleEntry entry);
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool Module32NextW(KernelHandle snapshot, ref ModuleEntry entry);
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    public static extern IntPtr GetModuleHandleW(string name);
    [DllImport("kernel32.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
    public static extern IntPtr GetProcAddress(IntPtr module, string name);
    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool GetModuleHandleExW(uint flags, IntPtr address, out IntPtr module);
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    public static extern uint GetModuleFileNameW(IntPtr module, StringBuilder fileName, int size);
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr VirtualAllocEx(KernelHandle process, IntPtr address, UIntPtr size, uint type, uint protection);
    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool VirtualFreeEx(KernelHandle process, IntPtr address, UIntPtr size, uint type);
    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool WriteProcessMemory(KernelHandle process, IntPtr address, byte[] buffer, UIntPtr size, out UIntPtr written);
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern KernelHandle CreateRemoteThread(KernelHandle process, IntPtr attributes, UIntPtr stackSize, IntPtr start, IntPtr argument, uint flags, out uint threadId);
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern uint WaitForSingleObject(KernelHandle handle, uint milliseconds);
    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)] public static extern bool GetExitCodeThread(KernelHandle thread, out uint code);

    public static Win32Exception Error(string operation)
    {
        int code = Marshal.GetLastWin32Error();
        return new Win32Exception(code, $"{operation}: {new Win32Exception(code).Message} (Win32 {code}).");
    }

    public static long Started(KernelHandle process)
    {
        if (!GetProcessTimes(process, out long started, out _, out _, out _)) throw Error("Consultar el proceso");
        return started;
    }

    public static void RequireX86(KernelHandle process)
    {
        if (!IsWow64Process2(process, out ushort machine, out ushort nativeMachine)) throw Error("Consultar arquitectura");
        if (machine != 0x014c && !(machine == 0 && nativeMachine == 0x014c))
            throw new InvalidOperationException("El proceso no es L4D2 de 32 bits (x86).");
    }

    public static string ImagePath(KernelHandle process)
    {
        var buffer = new StringBuilder(32768);
        uint length = (uint)buffer.Capacity;
        if (!QueryFullProcessImageNameW(process, 0, buffer, ref length)) throw Error("Consultar ruta del juego");
        return buffer.ToString();
    }

    public static List<ModuleEntry> Modules(int pid)
    {
        for (int attempt = 0; attempt < 4; ++attempt)
        {
            using var snapshot = CreateToolhelp32Snapshot(0x00000008 | 0x00000010, pid);
            if (snapshot.IsInvalid)
            {
                var error = Error("Leer los módulos del juego");
                if (error.NativeErrorCode == 24 && attempt < 3) { Thread.Sleep(30); continue; }
                throw error;
            }
            var entry = new ModuleEntry { Size = (uint)Marshal.SizeOf<ModuleEntry>() };
            if (!Module32FirstW(snapshot, ref entry)) throw Error("Leer el primer módulo del juego");
            var modules = new List<ModuleEntry>();
            do { modules.Add(entry); } while (Module32NextW(snapshot, ref entry));
            int code = Marshal.GetLastWin32Error();
            if (code != 18) throw new Win32Exception(code, "No se pudo completar la lista de módulos.");
            return modules;
        }
        throw new InvalidOperationException("Los módulos del juego siguen cambiando. Vuelve a intentar cuando termine de iniciar.");
    }
}
