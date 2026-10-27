using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.IO;
using System.Windows.Threading;
internal static class Program {
 [STAThread] static int Main(){
  var app=new Lyn.Loader.App();app.InitializeComponent();
  var assembly=typeof(Lyn.Loader.MainWindow).Assembly;
  var settings=Activator.CreateInstance(assembly.GetType("Lyn.Loader.LoaderSettings")!,true)!;
  var window=(Window)Activator.CreateInstance(typeof(Lyn.Loader.MainWindow),BindingFlags.Instance|BindingFlags.NonPublic,null,[settings],null)!;
  var root=(FrameworkElement)window.Content;
  window.Content=null;root.DataContext=window.DataContext;root.Resources.MergedDictionaries.Add(window.Resources);TextElement.SetForeground(root,window.Foreground);
  TextElement.SetFontWeight(root,window.FontWeight);
  ((TextBox)window.FindName("UsernameInput")).Text="usuario_demo";
  ((PasswordBox)window.FindName("PasswordInput")).Password="vista_previa";
  root.Measure(new Size(816,616));root.Arrange(new Rect(0,0,816,616));root.UpdateLayout();
  var backdrop=(FrameworkElement)window.FindName("Backdrop");
  var motion=(Storyboard)backdrop.Resources["BackdropMotion"];
  motion.Begin(backdrop,true);
  Directory.CreateDirectory("build/loader-preview");
  foreach(string mode in new[]{"login","dashboard"}){
   if(mode=="dashboard") {
    var client=typeof(Lyn.Loader.MainWindow).GetField("license",BindingFlags.NonPublic|BindingFlags.Instance)!.GetValue(window)!;
    client.GetType().GetField("token",BindingFlags.NonPublic|BindingFlags.Instance)!.SetValue(client,"preview");
    client.GetType().GetProperty("Username",BindingFlags.NonPublic|BindingFlags.Instance)!.SetValue(client,"Usuario");
    var license=Activator.CreateInstance(assembly.GetType("Lyn.Loader.LicenseInfo")!,true)!;
    license.GetType().GetProperty("ExpiresAt")!.SetValue(license,DateTimeOffset.UtcNow.AddDays(30).ToUnixTimeSeconds());
    client.GetType().GetProperty("License",BindingFlags.NonPublic|BindingFlags.Instance)!.SetValue(client,license);
    var target=Activator.CreateInstance(assembly.GetType("Lyn.Loader.GameTarget")!,[1234,1L,""])!;
    var state=Activator.CreateInstance(assembly.GetType("Lyn.Loader.GameState")!,[target,true,"Listo para cargar","Vista previa",null])!;
    typeof(Lyn.Loader.MainWindow).GetField("game",BindingFlags.NonPublic|BindingFlags.Instance)!.SetValue(window,state);
    typeof(Lyn.Loader.MainWindow).GetField("status",BindingFlags.NonPublic|BindingFlags.Instance)!.SetValue(window,"Listo para cargar.");
    typeof(Lyn.Loader.MainWindow).GetMethod("Notify",BindingFlags.NonPublic|BindingFlags.Instance)!.Invoke(window,null);
    root.Measure(new Size(816,616));root.Arrange(new Rect(0,0,816,616));root.UpdateLayout();
   }
   foreach(int seconds in new[]{1,4,7,10}){
    motion.SeekAlignedToLastTick(backdrop,TimeSpan.FromSeconds(seconds),TimeSeekOrigin.BeginTime);
    root.UpdateLayout();
    var renderedWindow=new RenderTargetBitmap(816,616,96,96,PixelFormats.Pbgra32);renderedWindow.Render(root);
    var encoder=new PngBitmapEncoder();encoder.Frames.Add(BitmapFrame.Create(renderedWindow));using var file=File.Create($"build/loader-preview/{mode}-{seconds}s.png");encoder.Save(file);
   }
  }
  Console.WriteLine("Rendered both screens at four facet lighting positions without opening a window.");
  SynchronizationContext.SetSynchronizationContext(new DispatcherSynchronizationContext());
  bool closes=CheckClose(assembly,false)&&CheckClose(assembly,true);
  return closes?0:1;
 }

 static bool CheckClose(Assembly assembly,bool busy){
  var settings=Activator.CreateInstance(assembly.GetType("Lyn.Loader.LoaderSettings")!,true)!;
  var window=(Window)Activator.CreateInstance(typeof(Lyn.Loader.MainWindow),BindingFlags.Instance|BindingFlags.NonPublic,null,[settings],null)!;
  var type=window.GetType();const BindingFlags flags=BindingFlags.Instance|BindingFlags.NonPublic;
  if(busy)type.GetField("busy",flags)!.SetValue(window,true);
  bool closed=false;
  var frame=new DispatcherFrame();
  window.Closed+=(_,_)=>{closed=true;frame.Continue=false;};
  window.Close();
  bool deferred=!closed;
  if(busy){
   deferred&=(bool)type.GetField("closeRequested",flags)!.GetValue(window)!;
   type.GetMethod("FinishOperation",flags)!.Invoke(window,null);
  }
  var timeout=new DispatcherTimer{Interval=TimeSpan.FromSeconds(5)};
  timeout.Tick+=(_,_)=>frame.Continue=false;
  timeout.Start();if(!closed)Dispatcher.PushFrame(frame);timeout.Stop();
  bool passed=deferred&&closed&&(bool)type.GetField("exitAnimationComplete",flags)!.GetValue(window)!;
  Console.WriteLine($"{(passed?"PASS":"FAIL")}: {(busy?"busy operation honors the pending close":"manual close before login prepares cleanup and closes")}");
  return passed;
 }
}
