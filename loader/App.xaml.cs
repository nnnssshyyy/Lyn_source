using System.IO;
using System.Threading;
using System.Windows;

namespace Lyn.Loader;

public partial class App : Application
{
    private Mutex? instance;
    internal static bool Dev { get; private set; }
    internal static string DevDllPath { get; private set; } = "";
    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);
        ParseArguments(e.Args);
        LoaderCleanup.Capture(LoaderLog.Write);
        instance = new Mutex(true, @"Local\Lyn.Loader.x86", out bool created);
        if (!created)
        {
            MessageBox.Show("El loader ya está abierto.", "lyn", MessageBoxButton.OK, MessageBoxImage.Information);
            Shutdown();
            return;
        }
        DispatcherUnhandledException += (_, args) =>
        {
            LoaderLog.Write("Error inesperado: " + args.Exception);
            MessageBox.Show("El loader encontró un error. Consulta loader.log en la carpeta de registros.", "lyn", MessageBoxButton.OK, MessageBoxImage.Error);
            args.Handled = true;
            Shutdown(1);
        };
        var settings = LoaderSettings.Read();
        Theme.Apply(0);
        MainWindow = new MainWindow(settings);
        MainWindow.Show();
    }

    private static void ParseArguments(string[] arguments)
    {
        Dev = false;
        DevDllPath = "";
        for (int index = 0; index < arguments.Length; ++index)
        {
            string argument = arguments[index];
            if (argument.Equals("--dev", StringComparison.OrdinalIgnoreCase)) { Dev = true; continue; }
            string? candidate = null;
            if (argument.StartsWith("--dev-dll=", StringComparison.OrdinalIgnoreCase)) candidate = argument["--dev-dll=".Length..];
            else if (argument.Equals("--dev-dll", StringComparison.OrdinalIgnoreCase) &&
                     index + 1 < arguments.Length && !arguments[index + 1].StartsWith("--", StringComparison.Ordinal))
                candidate = arguments[++index];
            if (string.IsNullOrWhiteSpace(candidate)) continue;
            Dev = true;
            try { DevDllPath = Path.GetFullPath(candidate.Trim().Trim('"')); }
            catch (Exception error) when (error is ArgumentException or NotSupportedException or PathTooLongException)
            { LoaderLog.Write("Modo dev: ruta no válida: " + candidate); DevDllPath = ""; }
        }
        if (Dev) LoaderLog.Write(DevDllPath.Length == 0 ? "Modo dev activado; se buscará la DLL local." : "Modo dev activado con " + DevDllPath + ".");
    }

    protected override void OnExit(ExitEventArgs e)
    {
        LoaderCleanup.ScheduleExit(true, Dev, LoaderLog.Write);
        instance?.Dispose();
        base.OnExit(e);
    }
}
