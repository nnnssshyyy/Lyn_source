using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Threading;

namespace Lyn.Loader;

public partial class MainWindow : Window, INotifyPropertyChanged
{
    private readonly LoaderSettings settings;
    private readonly LicenseClient license = new();
    private LicenseLeaseHost? leaseHost;
    private readonly DispatcherTimer timer = new() { Interval = TimeSpan.FromSeconds(2) };
    private GameState game = new(null, false, "Buscando el juego", "Comprobando si Left 4 Dead 2 está abierto...");
    private DllInfo? dll;
    private string dllError = "Selecciona lyn.dll para continuar.";
    private bool busy, refreshing, closing, closeRequested, closeScheduled;
    private bool lastSignedIn, exitAnimationStarted, exitAnimationComplete;
    private bool loginAttempted;
    private string pendingHardwareCode = "";
    private CancellationTokenSource? cancellation;
    private string status = "";
    public event PropertyChangedEventHandler? PropertyChanged;
    public string DllName => dll?.Name ?? "Ninguna DLL seleccionada";
    public string DllDescription => dll?.Description ?? dllError;
    public string DllPath => dll?.FullPath ?? "";
    private bool Pending => game.Target is { } target && settings.PendingPid == target.Pid && settings.PendingStarted == target.Started;
    public string GameTitle => Pending && game.LoadedPath == null ? "Carga pendiente" : game.Title;
    public string GameDetail => Pending && game.LoadedPath == null ? "Esperando a Windows. Reinicia el juego si la carga no termina." : game.Detail;
    public string StatusText => status;
    public Visibility LoginStatusVisibility => loginAttempted ? Visibility.Visible : Visibility.Collapsed;
    public Visibility PendingHardwareVisibility => pendingHardwareCode.Length > 0 ? Visibility.Visible : Visibility.Collapsed;
    public string HardwareRequestText => "Solicitud de equipo: " + pendingHardwareCode;
    public bool CanChoose => !busy;
    private bool CanResume => game.LoadedPath != null && leaseHost == null;
    public bool CanLoad => !busy && license.Licensed && game.Target != null && game.Ready && (game.LoadedPath!=null ? leaseHost==null : !Pending);
    public bool CanLoadOrLaunch => !busy && license.Licensed && (game.LoadedPath != null ? leaseHost == null : !Pending);
    public bool IsSignedIn => license.SignedIn;
    public GridLength LeftColumnWidth => new(license.SignedIn?390:300);
    public bool IsSignedOut => !license.SignedIn;
    public bool HasLicense => license.Licensed;
    public bool NeedsActivation => license.SignedIn && !license.Licensed;
    public bool CanAuthenticate => !busy;
    public string LoginLabel => busy ? "Validando..." : "Iniciar sesión";
    public string WelcomeText => license.SignedIn ? "Bienvenido, " + license.Username : "Bienvenido a VS 3";
    public string SubscriptionText => license.Licensed ? $"Hasta {DateTimeOffset.FromUnixTimeSeconds(license.License!.ExpiresAt).ToLocalTime():dd/MM/yyyy}" : "Sin activar";
    public string LicenseStatus => license.Licensed ? "Licencia validada" : "Esperando activación";
    public bool CanLaunch => !busy && game.Target == null;
    public string LoadLabel => "Load";
    public Visibility DevVisibility => App.Dev ? Visibility.Visible : Visibility.Collapsed;

    internal MainWindow(LoaderSettings settings)
    {
        this.settings = settings;
        InitializeComponent();
        DataContext = this;
        UsernameInput.Text = settings.RememberUsername ? settings.Username : "";
        RememberUsername.IsChecked = settings.RememberCredentials;
        if (settings.RememberCredentials)
        {
            try
            {
                var saved = CredentialVault.Read();
                if (saved != null) { UsernameInput.Text = saved.Username; PasswordInput.Password = saved.Password; }
                else { settings.RememberCredentials = false; RememberUsername.IsChecked = false; }
            }
            catch (Win32Exception) { settings.RememberCredentials = false; RememberUsername.IsChecked = false; }
        }
        SpinnerRotation.BeginAnimation(RotateTransform.AngleProperty, new DoubleAnimation(0, 360, TimeSpan.FromSeconds(0.9)) { RepeatBehavior = RepeatBehavior.Forever });
        timer.Tick += async (_, _) => await RefreshGame();
        Closed += (_, _) => { closing = true; timer.Stop(); leaseHost?.Dispose(); license.Dispose(); cancellation?.Dispose(); };
    }

    private async void OnLoaded(object sender, RoutedEventArgs e)
    {
        if(UiMotion.Enabled)BeginAnimation(OpacityProperty,UiMotion.Tween(0,1,260));
        UiMotion.Reveal(WindowSurface);
        if (App.Dev) PrepareDevMode();
        await RefreshGame();
        if (!closing) timer.Start();
    }

    private void Notify()
    {
        bool changed = lastSignedIn != IsSignedIn;
        double previousWidth = BackdropColumn.ActualWidth;
        lastSignedIn = IsSignedIn;
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(null));
        if(changed && IsLoaded && UiMotion.Enabled)
        {
            UiMotion.Reveal(IsSignedIn ? SessionView : LoginView);
            BackdropColumn.BeginAnimation(ColumnDefinition.WidthProperty,new GridWidthAnimation
            {From=previousWidth,To=LeftColumnWidth.Value,Duration=TimeSpan.FromMilliseconds(360),FillBehavior=FillBehavior.Stop});
        }
    }
    private void ButtonHovered(object sender, MouseEventArgs e) {if(sender is Button button)UiMotion.Hover(button,true);}
    private void ButtonLeft(object sender, MouseEventArgs e) {if(sender is Button button)UiMotion.Hover(button,false);}
    private void AddEntry(string message)
    {
        status = message;
        LoaderLog.Write(message);
        Notify();
    }

    private void SaveSettings()
    {
        try { settings.Save(); }
        catch (Exception e) when (e is IOException or UnauthorizedAccessException)
        { AddEntry("No se pudieron guardar las preferencias: " + e.Message); }
    }

    private void SelectDll(string path)
    {
        try
        {
            dll = PeImage.Inspect(path);
            settings.DllPath = dll.FullPath;
            AddEntry($"Seleccionado {dll.Name} · x86 · {dll.Length / 1048576d:0.00} MB.");
            SaveSettings();
        }
        catch (Exception e) when (e is IOException or UnauthorizedAccessException or BadImageFormatException or ArgumentException)
        {
            dll = null;
            dllError = "DLL no compatible";
            AddEntry("No se pudo usar la DLL: " + e.Message);
        }
        game = game with { LoadedPath = null };
        Notify();
    }

    private void PrepareDevMode()
    {
        AddEntry("MODO DEV · El módulo del servicio no se usará; se cargará una compilación local.");
        string path = App.DevDllPath;
        if (path.Length == 0) path = LoaderSettings.FindDevDll(settings.DllPath);
        if (path.Length == 0)
        {
            AddEntry("MODO DEV · No se encontró una lyn.dll local. Usa --dev-dll.");
            return;
        }
        AddEntry("MODO DEV · Compilación local: " + path + ".");
        SelectDll(path);
    }

    private async Task RefreshGame(bool force = false)
    {
        if (refreshing || closing || (busy && !force)) return;
        refreshing = true;
        string selected = DllPath;
        Notify();
        try
        {
            GameState result = await Task.Run(() => GameSession.Discover(selected));
            if (closing || (busy && !force)) return;
            // The file picker can change the selection while discovery is in flight.
            if (!string.Equals(selected, DllPath, StringComparison.OrdinalIgnoreCase)) return;
            bool changed = game != result;
            game = result;
            if (leaseHost != null && !leaseHost.IsFor(game.Target)) { leaseHost.Dispose(); leaseHost = null; }
            if (settings.PendingPid != 0 && (game.LoadedPath != null ||
                (game.Target is { } pendingTarget && (pendingTarget.Pid != settings.PendingPid || pendingTarget.Started != settings.PendingStarted))))
            {
                settings.PendingPid = 0; settings.PendingStarted = 0; SaveSettings();
            }
            // La presencia del módulo no se anuncia como actividad: el flujo de carga ya la confirma.
            if (changed && game.LoadedPath == null) AddEntry(game.Title + ". " + game.Detail);
            if (!busy && !closeScheduled && leaseHost is { Armed: true } && game.LoadedPath != null) ScheduleClose();
        }
        catch (Exception e) when (e is Win32Exception or InvalidOperationException or IOException)
        { game = new(null, false, "Estado no disponible", e.Message); AddEntry(e.Message); }
        finally { refreshing = false; Notify(); }
    }

    private async void LoadClicked(object sender, RoutedEventArgs e)
    {
        if (busy || !license.Licensed) return;
        if (game.LoadedPath != null && leaseHost != null)
        { AddEntry("La sesión ya está activa: el módulo está cargado y autorizado."); return; }
        if (game.Title == "Varias instancias abiertas")
        { AddEntry(game.Detail); return; }
        busy = true;
        cancellation = new CancellationTokenSource();
        BusySpinner.Visibility = Visibility.Visible;
        Notify();
        try
        {
            // Un solo botón: si el juego está cerrado se abre por Steam y se
            // espera al menú principal; después se carga el módulo y el loader
            // se cierra solo cuando el módulo confirma su plan firmado.
            if (game.Target == null && !LaunchGame()) return;
            if (!game.Ready && !await WaitForGame(cancellation.Token))
            { AddEntry("L4D2 no llegó al menú principal a tiempo. Vuelve a intentarlo cuando esté abierto."); return; }
            GameTarget target = game.Target ?? throw new InvalidOperationException("No se detectó Left 4 Dead 2.");
            string path;
            if (App.Dev)
            {
                if (dll == null) throw new InvalidOperationException("Modo dev: selecciona una lyn.dll local antes de cargar.");
                AddEntry("MODO DEV · Preparando " + dll.FullPath + "…");
                path = DevModule.Prepare(dll.FullPath);
            }
            else
            {
                AddEntry("Preparando el módulo firmado desde el servidor…");
                path = await license.PrepareModule(target, cancellation.Token);
            }
            dll=PeImage.Inspect(path);settings.DllPath=path;SaveSettings();
            var current=await Task.Run(()=>GameSession.Discover(path),cancellation.Token);
            if(current.Target?.Pid!=target.Pid || current.Target.Started!=target.Started || !current.Ready)throw new InvalidOperationException("El juego cambió durante la preparación. Vuelve a intentarlo.");
            if(current.LoadedPath!=null && !string.Equals(current.LoadedPath,path,StringComparison.OrdinalIgnoreCase))throw new InvalidOperationException("Hay otra versión cargada. Reinicia L4D2 para usar la actualización.");
            game=current;
            var ticket = await license.Lease(target, cancellation.Token);
            leaseHost?.Dispose();
            leaseHost = new LicenseLeaseHost(license, target, ticket, message =>
            {
                if (!closing) Dispatcher.BeginInvoke(() => AddEntry(message));
            });
            leaseHost.Start();
            if (game.LoadedPath != null)
            {
                if (await WaitForModuleReady(cancellation.Token)) {
                    AddEntry("Sesión renovada y módulo confirmado. Cerrando el loader…");
                    await Task.Delay(1500);
                    closeRequested = true;
                } else AddEntry("Esperando la confirmación del módulo antes de cerrar el loader.");
                return;
            }
            // Persist the attempt before dispatch. A closed/crashed loader must
            // not allow a second remote load while the first may still run.
            settings.PendingPid = target.Pid; settings.PendingStarted = target.Started;
            settings.Save();
            var progress = new Progress<string>(AddEntry);
            var result = await LibraryLoader.LoadAsync(target, path, progress, cancellation.Token);
            AddEntry(result.Message);
            if (result.Outcome != LoadOutcome.Pending)
            { settings.PendingPid = 0; settings.PendingStarted = 0; SaveSettings(); }
            if (result.Outcome == LoadOutcome.Loaded)
            {
                if (await WaitForModuleReady(cancellation.Token))
                {
                    AddEntry("Todo listo: el módulo quedó autorizado para toda la partida. Cerrando el loader…");
                    await Task.Delay(1600);
                    closeRequested = true;
                }
                else AddEntry("El módulo aún no confirmó su activación firmada; el loader seguirá abierto y cerrará solo al confirmarla.");
            }
            else if (result.Outcome == LoadOutcome.AlreadyLoaded)
            {
                if (await WaitForModuleReady(cancellation.Token)) {
                    await Task.Delay(1000);
                    closeRequested = true;
                } else AddEntry("Esperando la confirmación del módulo antes de cerrar el loader.");
            }
        }
        catch (OperationCanceledException)
        {
            leaseHost?.Dispose(); leaseHost = null;
            settings.PendingPid = 0; settings.PendingStarted = 0; SaveSettings();
            AddEntry("Carga cancelada antes de ejecutarla.");
        }
        catch (Exception exception)
        {
            if (exception is not LoadOperationException { MayStillBeRunning: true })
            { leaseHost?.Dispose(); leaseHost = null; settings.PendingPid = 0; settings.PendingStarted = 0; SaveSettings(); }
            AddEntry("No se completó la carga: " + exception.Message);
        }
        finally
        {
            BusySpinner.Visibility = Visibility.Collapsed;
            cancellation.Dispose(); cancellation = null;
            FinishOperation();
            if (!closeRequested) await RefreshGame();
        }
    }

    private async Task<bool> WaitForGame(CancellationToken token)
    {
        for (int attempt = 0; attempt < 180 && !token.IsCancellationRequested; ++attempt)
        {
            await RefreshGame(true);
            if (game.Ready && game.Target != null) return true;
            await Task.Delay(1000, token);
        }
        return false;
    }

    private async Task<bool> WaitForModuleReady(CancellationToken token)
    {
        var host = leaseHost;
        if (host == null) return false;
        try
        {
            for (int attempt = 0; attempt < 200; ++attempt)
            {
                if (host.Armed) return true;
                await Task.Delay(100, token);
            }
        }
        catch (OperationCanceledException) { }
        return host.Armed;
    }

    private async void ScheduleClose()
    {
        if (closeScheduled || closing) return;
        closeScheduled = true;
        AddEntry("Todo listo: el módulo quedó autorizado para toda la partida. Cerrando el loader…");
        await Task.Delay(1600);
        if (closing) return;
        if (busy) { closeRequested = true; return; }
        Close();
    }

    private Task<bool> PrepareCleanup() => LoaderCleanup.Schedule(true, App.Dev, LoaderLog.Write);

    private void FinishOperation()
    {
        busy = false;
        Notify();
        if (closeRequested) Close();
    }

    private async void LoginClicked(object sender, RoutedEventArgs e)
    {
        if (busy) return;
        busy = true; loginAttempted = true; pendingHardwareCode = ""; Notify();
        try
        {
            await license.Login(UsernameInput.Text, PasswordInput.Password, CancellationToken.None);
            settings.RememberUsername = RememberUsername.IsChecked == true;
            settings.Username = settings.RememberUsername ? license.Username : ""; SaveSettings();
            bool saved = true;
            try
            {
                if (RememberUsername.IsChecked == true) CredentialVault.Save(license.Username, PasswordInput.Password);
                else CredentialVault.Delete();
                settings.RememberCredentials = RememberUsername.IsChecked == true;
            }
            catch (Win32Exception) { saved = false; settings.RememberCredentials = false; }
            SaveSettings();
            AddEntry(license.Licensed ? "Licencia validada. Listo para preparar L4D2." : "Cuenta validada. Introduce tu clave de activación.");
            if (!saved) AddEntry("Sesión iniciada. Windows no pudo guardar las credenciales.");
            string recovery = license.TakeRecoveryCode();
            if (recovery.Length > 0) RecoveryCodeWindow.ShowCode(this, recovery);
        }
        catch (HardwareApprovalRequired error) { pendingHardwareCode = error.RequestCode; AddEntry("Equipo bloqueado. Autoriza esta solicitud desde la página web y vuelve a iniciar sesión."); }
        catch (Exception error) when (error is System.Net.Http.HttpRequestException or InvalidOperationException or TaskCanceledException or System.Text.Json.JsonException)
        { AddEntry(error is TaskCanceledException ? "El servidor no respondió a tiempo. Vuelve a intentarlo." : error.Message); }
        finally { PasswordInput.Clear(); FinishOperation(); }
    }
    private void ForgetCredentials(object sender, RoutedEventArgs e)
    {
        settings.RememberCredentials = false; settings.RememberUsername = false; settings.Username = "";
        try { CredentialVault.Delete(); }
        catch (Win32Exception) { AddEntry("Windows no pudo borrar las credenciales guardadas. Puedes quitarlas en el Administrador de credenciales."); }
        SaveSettings();
    }
    private async void ManageHardwareClicked(object sender, RoutedEventArgs e)
    {
        if (busy || !license.SignedIn) return;
        busy = true; Notify();
        try
        {
            var link = await license.HardwareManagementLink(CancellationToken.None);
            Process.Start(new ProcessStartInfo(link.AbsoluteUri) { UseShellExecute = true });
            AddEntry("Gestión de equipos abierta en la web. El enlace autoriza este navegador una sola vez.");
        }
        catch (Exception error) when (error is System.Net.Http.HttpRequestException or InvalidOperationException or TaskCanceledException or System.Text.Json.JsonException or Win32Exception)
        { AddEntry(error.Message); }
        finally { FinishOperation(); }
    }
    private void OpenAccountHardware(object sender, RoutedEventArgs e)
    {
        try { Process.Start(new ProcessStartInfo("https://vs3-accounts.vercel.app/#equipos") { UseShellExecute = true }); }
        catch (Win32Exception error) { AddEntry(error.Message); }
    }
    private async void ActivateClicked(object sender, RoutedEventArgs e)
    {
        if (busy || !license.SignedIn) return;
        busy = true; Notify();
        try { await license.Activate(ActivationInput.Text, CancellationToken.None); ActivationInput.Clear(); AddEntry("Licencia activada. Ya puedes cargar VS 3."); }
        catch (Exception error) when (error is System.Net.Http.HttpRequestException or InvalidOperationException or TaskCanceledException or System.Text.Json.JsonException) { AddEntry(error.Message); }
        finally { FinishOperation(); }
    }
    private bool LaunchGame()
    {
        try
        {
            Process.Start(new ProcessStartInfo("steam://rungameid/550") { UseShellExecute = true })?.Dispose();
            AddEntry("Solicitud enviada a Steam. Esperando a que abra Left 4 Dead 2...");
            return true;
        }
        catch (Exception exception) when (exception is Win32Exception or InvalidOperationException)
        { AddEntry("No se pudo abrir Steam: " + exception.Message); return false; }
    }

    private void DragTitle(object sender, MouseButtonEventArgs e)
    {
        if (e.ChangedButton == MouseButton.Left && e.ButtonState == MouseButtonState.Pressed)
            DragMove();
    }
    private void CloseWindow(object sender, RoutedEventArgs e) => Close();
    private async void OnClosing(object? sender, CancelEventArgs e)
    {
        if(!busy)
        {
            if(exitAnimationComplete)return;
            e.Cancel=true;
            if(exitAnimationStarted)return;
            exitAnimationStarted=true;closing=true;timer.Stop();IsEnabled=false;
            // Prepare the helper for every accepted close, including X/Alt+F4
            // before login. App.OnExit remains a synchronous fallback.
            await Task.Yield(); // Return from the canceled Closing event first.
            await PrepareCleanup();
            if(IsLoaded && UiMotion.Enabled)
            {
                var fade=UiMotion.Tween(Opacity,0,150);fade.FillBehavior=FillBehavior.HoldEnd;
                BeginAnimation(OpacityProperty,fade);
                await Task.Delay(150);
            }
            exitAnimationComplete=true;Close();return;
        }
        e.Cancel = true;
        closeRequested = true;
        cancellation?.Cancel();
        AddEntry("Terminando la operación antes de cerrar; puede tardar hasta 30 segundos.");
    }
}
