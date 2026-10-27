using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;
using System.ComponentModel;

namespace Lyn.Loader.Controls;

public partial class PrismBackdrop : UserControl
{
    private Window? owner;
    private bool started;
    private Storyboard Motion => (Storyboard)Resources["BackdropMotion"];
    private void UpdateMotion()
    {
        if(!IsLoaded)return;
        if(!UiMotion.Enabled){Motion.Remove(this);started=false;return;}
        if(!started){Motion.Begin(this,true);started=true;}
        if(!IsVisible || owner?.WindowState==WindowState.Minimized)Motion.Pause(this);
        else Motion.Resume(this);
    }
    private void OwnerStateChanged(object? sender,EventArgs e)=>UpdateMotion();
    private void AnimationPreferenceChanged(object? sender,PropertyChangedEventArgs e)
    {if(e.PropertyName==nameof(SystemParameters.ClientAreaAnimation))Dispatcher.Invoke(UpdateMotion);}
    public PrismBackdrop()
    {
        InitializeComponent();
        Loaded += (_, _) =>
        {
            owner=Window.GetWindow(this);
            if(owner!=null)owner.StateChanged+=OwnerStateChanged;
            SystemParameters.StaticPropertyChanged+=AnimationPreferenceChanged;
            UpdateMotion();
        };
        Unloaded += (_, _) =>
        {
            Motion.Remove(this);started=false;
            if(owner!=null)owner.StateChanged-=OwnerStateChanged;
            owner=null;SystemParameters.StaticPropertyChanged-=AnimationPreferenceChanged;
        };
        IsVisibleChanged+=(_,_)=>UpdateMotion();
    }
}
