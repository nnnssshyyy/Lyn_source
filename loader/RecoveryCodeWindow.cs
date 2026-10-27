using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Lyn.Loader;

internal static class RecoveryCodeWindow
{
    internal static void ShowCode(Window owner, string code)
    {
        var window = new Window { Title = "VS 3 · Recuperación de equipo", Owner = owner, Width = 520, Height = 330,
            ResizeMode = ResizeMode.NoResize, WindowStartupLocation = WindowStartupLocation.CenterOwner,
            Background = new SolidColorBrush(Color.FromRgb(17, 24, 19)), Foreground = Brushes.White, FontFamily = new FontFamily("Segoe UI") };
        var content = new StackPanel { Margin = new Thickness(26) };
        content.Children.Add(new TextBlock { Text = "Tu equipo quedó vinculado.", FontSize = 22, FontWeight = FontWeights.SemiBold });
        content.Children.Add(new TextBlock { Text = "Guarda este código en un lugar seguro. Permite autorizar un nuevo PC desde la web si pierdes el anterior. Solo se muestra ahora.",
            TextWrapping = TextWrapping.Wrap, FontSize = 12, Foreground = Brushes.LightGray, Margin = new Thickness(0, 14, 0, 15) });
        var box = new TextBox { Text = code, IsReadOnly = true, TextWrapping = TextWrapping.Wrap, Padding = new Thickness(10), FontSize = 13 };
        content.Children.Add(box);
        var copy = new Button { Content = "Copiar código", Margin = new Thickness(0, 15, 0, 0), Padding = new Thickness(12, 8, 12, 8) };
        copy.Click += (_, _) => { try { Clipboard.SetText(code); copy.Content = "Código copiado"; } catch (System.Runtime.InteropServices.ExternalException) { box.SelectAll(); box.Focus(); } };
        content.Children.Add(copy);
        var done = new Button { Content = "Continuar", Margin = new Thickness(0, 10, 0, 0), Padding = new Thickness(12, 8, 12, 8) };
        done.Click += (_, _) => window.Close(); content.Children.Add(done); window.Content = content; window.ShowDialog(); box.Clear();
    }
}
