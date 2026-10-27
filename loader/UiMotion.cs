using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Animation;

namespace Lyn.Loader;
internal static class UiMotion
{
    internal static bool Enabled => SystemParameters.ClientAreaAnimation;
    internal static DoubleAnimation Tween(double from, double to, int milliseconds) => new(from, to, TimeSpan.FromMilliseconds(milliseconds))
    { EasingFunction = new CubicEase { EasingMode = EasingMode.EaseOut }, FillBehavior = FillBehavior.Stop };

    internal static void Reveal(FrameworkElement element)
    {
        element.Opacity = 1;
        if (!Enabled) return;
        element.BeginAnimation(UIElement.OpacityProperty, Tween(0, 1, 300), HandoffBehavior.SnapshotAndReplace);
        var shift = new TranslateTransform(); element.RenderTransform = shift;
        shift.BeginAnimation(TranslateTransform.YProperty, Tween(9, 0, 340), HandoffBehavior.SnapshotAndReplace);
    }
    internal static void Hover(Button button, bool entered)
    {
        if (!Enabled || (entered && !button.IsEnabled)) return;
        if (button.RenderTransform is not ScaleTransform scale)
        { scale = new ScaleTransform(1, 1); button.RenderTransform = scale; button.RenderTransformOrigin = new Point(.5, .5); }
        double target = entered ? 1.012 : 1;
        var animation = Tween(scale.ScaleX, target, entered ? 150 : 200);
        scale.ScaleX = scale.ScaleY = target;
        scale.BeginAnimation(ScaleTransform.ScaleXProperty, animation, HandoffBehavior.SnapshotAndReplace);
        scale.BeginAnimation(ScaleTransform.ScaleYProperty, animation, HandoffBehavior.SnapshotAndReplace);
    }
}
public sealed class GridWidthAnimation : AnimationTimeline
{
    public static readonly DependencyProperty FromProperty = DependencyProperty.Register(nameof(From), typeof(double), typeof(GridWidthAnimation));
    public static readonly DependencyProperty ToProperty = DependencyProperty.Register(nameof(To), typeof(double), typeof(GridWidthAnimation));
    public double From { get => (double)GetValue(FromProperty); set => SetValue(FromProperty, value); }
    public double To { get => (double)GetValue(ToProperty); set => SetValue(ToProperty, value); }
    public override Type TargetPropertyType => typeof(GridLength);
    protected override Freezable CreateInstanceCore() => new GridWidthAnimation();
    public override object GetCurrentValue(object defaultOriginValue, object defaultDestinationValue, AnimationClock clock)
    {
        double progress = clock.CurrentProgress ?? 0;
        double eased = 1 - Math.Pow(1 - progress, 3);
        return new GridLength(Math.Max(0, From + (To - From) * eased));
    }
}
