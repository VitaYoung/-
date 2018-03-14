using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace Todos
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
            var viewTitleBar = Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().TitleBar;
            viewTitleBar.BackgroundColor = Windows.UI.Colors.CornflowerBlue;
            viewTitleBar.ButtonBackgroundColor = Windows.UI.Colors.CornflowerBlue;
        }

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(NewPage), "");
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.NavigationMode == NavigationMode.New)
            {
                ApplicationData.Current.LocalSettings.Values.Remove("TheWorkInProgress");
            }
            else
            {
                if (ApplicationData.Current.LocalSettings.Values.ContainsKey("TheWorkInProgress"))
                {
                    var composite = ApplicationData.Current.LocalSettings.Values["TheWorkInProgress"] as ApplicationDataCompositeValue;
                    box1.IsChecked = (bool)composite["box1"];
                    box2.IsChecked = (bool)composite["box2"];
                    line1.Opacity = (double)composite["line1"];
                    line2.Opacity = (double)composite["line2"];
                    ApplicationData.Current.LocalSettings.Values.Remove("TheWorkInProgress");
                }
            }
        }

        private void Check(object sender, RoutedEventArgs e)
        {
            if (box1.IsChecked == true)
            {
                line1.Opacity = 1;
            }
            if (box1.IsChecked == false)
            {
                line1.Opacity = 0;
            }
            if (box2.IsChecked == true)
            {
                line2.Opacity = 1;
            }
            if (box2.IsChecked == false)
            {
                line2.Opacity = 0;
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {

            bool suspending = ((App)App.Current).IsSuspending;
            if (suspending)
            {
                var composite = new ApplicationDataCompositeValue();
                composite["box1"] = box1.IsChecked;
                composite["box2"] = box2.IsChecked;
                composite["line1"] = line1.Opacity;
                composite["line2"] = line2.Opacity;
                ApplicationData.Current.LocalSettings.Values["TheWorkInProgress"] = composite;
            }
        }
    }
}
