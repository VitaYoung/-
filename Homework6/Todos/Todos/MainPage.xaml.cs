using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Xml;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Notifications;
using Windows.UI.Popups;
using Windows.Data.Xml.Dom;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Xaml.Shapes;
using Windows.ApplicationModel.DataTransfer;
using Todos.Models;

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
            this.ViewModel = new ViewModels.TodoItemViewModel();
        }

        ViewModels.TodoItemViewModel ViewModel { get; set; }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.Parameter.GetType() == typeof(ViewModels.TodoItemViewModel))
            {
                this.ViewModel = (ViewModels.TodoItemViewModel)(e.Parameter);
            }
            load();
            DataTransferManager.GetForCurrentView().DataRequested += OnShareDataRequested;
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            DataTransferManager.GetForCurrentView().DataRequested -= OnShareDataRequested;
        }

        private void AppBarButton_Click(object sender, RoutedEventArgs e)
        {
            dynamic ori = e.OriginalSource;
            ViewModel.SelectedItem = (TodoItem)ori.DataContext;
            DataTransferManager.ShowShareUI();
        }

        private void DeleteButton_Click(object sender, RoutedEventArgs e)
        {
            var db = App.conn;
            dynamic ori = e.OriginalSource;
            ViewModel.SelectedItem = (TodoItem)ori.DataContext;
            using (var statement = db.Prepare("DELETE FROM Message WHERE Id = ?"))
            {
                statement.Bind(1, ViewModel.SelectedItem.id);
                statement.Step();
            } 
            ViewModel.RemoveTodoItem(ViewModel.SelectedItem.id);

        }

        private void Search_Click(object sender, RoutedEventArgs e)
        {
            if (searchinput.Text == "")
            {
                var i = new MessageDialog("搜索内容不能为空！").ShowAsync();
            } else
            {
                var target = "%" + searchinput.Text + "%";
                using (var statement = App.conn.Prepare("SELECT* FROM Message WHERE Title LIKE ? OR Description LIKE ? OR Time LIKE ?"))
                {
                    statement.Bind(1, target);
                    statement.Bind(2, target);
                    statement.Bind(3, target);
                    if (statement.ColumnCount != 0)
                    {
                        string result = "";
                        while (statement.Step() == SQLitePCL.SQLiteResult.ROW)
                        {
                            result += "Title: " + (string)statement[1] + " Description: " + (string)statement[2] + " Time: " + (string)statement[3] + "\n";
                        }
                        var i = new MessageDialog(result).ShowAsync();
                    }
                    else
                    {
                        var i = new MessageDialog("搜索的内容不存在！").ShowAsync();
                    }
                }
            }
        }

        async void OnShareDataRequested(DataTransferManager sender, DataRequestedEventArgs args)
        {
            var dp = args.Request.Data;
            var deferral = args.Request.GetDeferral();
            var photoFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///Assets/use2.jpg"));
            dp.Properties.Title = "Share from here";
            dp.Properties.Description = "Choose email";
            dp.SetStorageItems(new List<StorageFile> { photoFile });
            deferral.Complete();
        }
        private void Check(object sender, RoutedEventArgs e)
        {
            var parent = VisualTreeHelper.GetParent(sender as DependencyObject);
            Line line = VisualTreeHelper.GetChild(parent, 3) as Line;
            if (line.Opacity == 0)
            {
                line.Opacity = 1;
            } else
            {
                line.Opacity = 0;
            }
        }
        private void TodoItem_ItemClicked(object sender, ItemClickEventArgs e)
        {
            ViewModel.SelectedItem = (Models.TodoItem)(e.ClickedItem);
            if (ViewModel.SelectedItem != null)
            {
                createButton.Content = "Update";
            }
            if (InlineToDoItemViewGrid.Visibility != Visibility.Collapsed)
            {
                inputtitle1.Text = ViewModel.SelectedItem.title;
                inputdetail1.Text = ViewModel.SelectedItem.description;
                inputdate1.Date = ViewModel.SelectedItem.date;
            } else
            {
                Frame.Navigate(typeof(NewPage), ViewModel);
            }
        }

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            if (Window.Current.Bounds.Width < 800)
            {
                ViewModel.SelectedItem = null;
                Frame.Navigate(typeof(NewPage), ViewModel);
            }
            
        }
        private void CreateButton_Clicked(object sender, RoutedEventArgs e)
        {
            if (inputtitle1.Text == "")
            {
                var i = new MessageDialog("标题不能为空！").ShowAsync();
            }
            if (inputdetail1.Text == "")
            {
                var i = new MessageDialog("描述不能为空！").ShowAsync();
            }
            if (inputdate1.Date > System.DateTime.Now)
            {
                var i = new MessageDialog("输入的日期不合法！").ShowAsync();
            }
            if (inputtitle1.Text != "" && inputdetail1.Text != "" && inputdate1.Date <= DateTime.Now)
            {
                if (ViewModel.SelectedItem != null)
                {
                    var db = App.conn;
                    using (var updatedata = db.Prepare("UPDATE Message SET Title = ?, Description = ?, Time = ? WHERE Id = ?")) {
                        updatedata.Bind(1, inputtitle1.Text);
                        updatedata.Bind(2, inputdetail1.Text);
                        updatedata.Bind(3, Convert.ToString(inputdate1.Date.DateTime));
                        updatedata.Bind(4, ViewModel.SelectedItem.id);
                        updatedata.Step();
                    }
                    ViewModel.UpdateTodoItem(ViewModel.SelectedItem.id, inputtitle1.Text, inputdetail1.Text, inputdate1.Date.DateTime);
                    Frame.Navigate(typeof(MainPage), ViewModel);
                }
                if (createButton.Content.ToString() == "Create" && ViewModel.SelectedItem == null)
                {
                    var db = App.conn;
                    using (var insertdata = db.Prepare("INSERT INTO Message(Title, Description, Time) VALUES(?, ?, ?)"))
                    {
                        insertdata.Bind(1, inputtitle1.Text);
                        insertdata.Bind(2, inputdetail1.Text);
                        insertdata.Bind(3, Convert.ToString(inputdate1.Date.DateTime));
                        insertdata.Step();
                    }
                    ViewModel.AddTodoItem(ViewModel.AllItems.Count + 1, inputtitle1.Text, inputdetail1.Text, inputdate1.Date.DateTime);
                    Frame.Navigate(typeof(MainPage), ViewModel);
                }
            }
            load();
        }
        private void load()
        {
            string titleString = File.ReadAllText("title.xml");
            Windows.Data.Xml.Dom.XmlDocument doc = new Windows.Data.Xml.Dom.XmlDocument();
            doc.LoadXml(titleString);
            Windows.Data.Xml.Dom.XmlNodeList titles = doc.GetElementsByTagName("text");
            TileUpdateManager.CreateTileUpdaterForApplication().Clear();
            TileUpdateManager.CreateTileUpdaterForApplication().EnableNotificationQueue(true);
            for (int i = 0; i < ViewModel.AllItems.Count; i++)
            {
                titles[0].InnerText = titles[2].InnerText = titles[4].InnerText = ViewModel.AllItems[i].title;
                titles[1].InnerText = titles[3].InnerText = titles[5].InnerText = ViewModel.AllItems[i].description;
                var tileNotification = new TileNotification(doc);
                TileUpdater updater = TileUpdateManager.CreateTileUpdaterForApplication();
                updater.Update(tileNotification);
            }
        }
        private void Clear(object sender, RoutedEventArgs e)
        {
            inputtitle1.Text = "";
            inputdetail1.Text = "";
            inputdate1.Date = System.DateTime.Now;
        }

    }
}
