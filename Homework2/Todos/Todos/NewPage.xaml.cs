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
using Windows.UI.Popups;


namespace Todos
{
    public sealed partial class NewPage : Page
    {
        public NewPage()
        {
            this.InitializeComponent();
            var viewTitleBar = Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().TitleBar;
            viewTitleBar.BackgroundColor = Windows.UI.Colors.CornflowerBlue;
            viewTitleBar.ButtonBackgroundColor = Windows.UI.Colors.CornflowerBlue;
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            Frame rootFrame = Window.Current.Content as Frame;

            if (rootFrame.CanGoBack)
            {
                SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility =
                    AppViewBackButtonVisibility.Visible;
            }
            else
            {
                SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility =
                    AppViewBackButtonVisibility.Collapsed;
            }
            var i = new MessageDialog("Welcome!").ShowAsync();
        }
        public void Checkvalid(object sender, RoutedEventArgs e)
        {
            if (title.Text == "")
            {
                var warning1 = new MessageDialog("Title不能为空！").ShowAsync();
            }
            if (details.Text == "")
            {
                var warning2 = new MessageDialog("Details不能为空！").ShowAsync();
            }
            if (date.Date < DateTime.Now.Date)
            {
                var warning3 = new MessageDialog("日期不能小于当前日期！").ShowAsync();
            }
        }
        public void Cancel(object sender, RoutedEventArgs e)
        {
            title.Text = "";
            details.Text = "";
            date.Date = DateTime.Now;
        }
    }
}
