using System.Windows;
using System.Windows.Media;

namespace Lyn.Loader;

internal static class Theme
{
    // RGB values from menu::apply_theme_preset in src/velocity_ui/menu.core.cpp.
    private static readonly (string Accent, string Canvas, string Surface, string Text)[] Palettes =
    [
        ("#ADC0FF", "#111111", "#1F1F23", "#DDE5FF"),
        ("#BEA4FF", "#121117", "#221F2B", "#EBE5FF"),
        ("#7AE0B5", "#0E1413", "#192723", "#DEF6EE"),
        ("#FFAB70", "#16110F", "#2B201A", "#FFE8D6"),
        ("#CFCFCF", "#101010", "#202020", "#E8E8E8")
    ];

    public static void Apply(int index)
    {
        var p = Palettes[Math.Clamp(index, 0, Palettes.Length - 1)];
        Set("Accent", p.Accent); Set("Canvas", p.Canvas); Set("Surface", p.Surface); Set("Text", p.Text);
        var accent = (Color)ColorConverter.ConvertFromString(p.Accent);
        Set("Tint", Color.FromArgb(28, accent.R, accent.G, accent.B));
        Set("Line", Color.FromArgb(36, accent.R, accent.G, accent.B));
        var text = (Color)ColorConverter.ConvertFromString(p.Text);
        Set("Muted", Color.FromRgb((byte)(text.R * .65), (byte)(text.G * .65), (byte)(text.B * .65)));
    }

    private static void Set(string key, string value) => Set(key, (Color)ColorConverter.ConvertFromString(value));
    private static void Set(string key, Color color)
    {
        var brush = new SolidColorBrush(color);
        brush.Freeze();
        Application.Current.Resources[key] = brush;
    }
}
