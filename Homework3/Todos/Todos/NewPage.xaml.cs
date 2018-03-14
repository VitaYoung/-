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

        private ViewModels.TodoItemViewModel ViewModel;

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            ViewModel = ((ViewModels.TodoItemViewModel)e.Parameter);
            if (ViewModel.SelectedItem == null)
            {
                createButton.Content = "Create";
                var i = new MessageDialog("Welcome!").ShowAsync();
            }
            else
            {
                createButton.Content = "Update";
                inputtitle2.Text = ViewModel.SelectedItem.title;
                inputdetail2.Text = ViewModel.SelectedItem.description;
                inputdate2.Date = ViewModel.SelectedItem.date;
            }
        }
        private void CreateButton_Clicked(object sender, RoutedEventArgs e)
        {
            if (inputtitle2.Text == "")
            {
                var i = new MessageDialog("标题不能为空！").ShowAsync();
            } 
            if (inputdetail2.Text == "")
            {
                var i = new MessageDialog("描述不能为空！").ShowAsync();
            }
            if (inputdate2.Date > System.DateTime.Now)
            {
                var i = new MessageDialog("输入的日期不合法！").ShowAsync();
            }
            if (inputtitle2.Text != "" && inputdetail2.Text != "" && inputdate2.Date <= DateTime.Now)
            {
                if (ViewModel.SelectedItem != null)
                {
                    ViewModel.UpdateTodoItem("", inputtitle2.Text, inputdetail2.Text, inputdate2.Date.DateTime);
                    Frame.Navigate(typeof(MainPage), ViewModel);
                }
                if (createButton.Content.ToString() == "Create" && ViewModel.SelectedItem == null)
                {
                    ViewModel.AddTodoItem(inputtitle2.Text, inputdetail2.Text, inputdate2.Date.DateTime);
                    Frame.Navigate(typeof(MainPage), ViewModel);
                }
            }
        }
        private  void DeleteButton_Clicked(object sender, RoutedEventArgs e)
        {
            if (ViewModel.SelectedItem != null)
            {
                ViewModel.RemoveTodoItem("");
                ViewModel.SelectedItem = null;
                Frame.Navigate(typeof(MainPage), ViewModel);
            }
        }
        private void Clear(object sender, RoutedEventArgs e)
        {
            inputtitle2.Text = "";
            inputdetail2.Text = "";
            inputdate2.Date = System.DateTime.Now;
        }
    }
}
