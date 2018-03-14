using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace Homework8
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        void OnMouseDownPlayMedia(object sender, RoutedEventArgs e)
        {
            myMediaElement.Play();
            InitializePropertyValues();
        }

        void OnMouseDownPauseMedia(object sender, RoutedEventArgs e)
        {
            myMediaElement.Pause();
        }

        void OnMouseDownStopMedia(object sender, RoutedEventArgs e)
        {
            myMediaElement.Stop();
        }

        void ChangeScreen(object sender, RoutedEventArgs e)
        {
            if (myMediaElement.IsFullWindow == false)
            {
                myMediaElement.IsFullWindow = true;
            } else
            {
                myMediaElement.IsFullWindow = false;
            }
        }
        private void ChangeMediaVolume(object sender, RangeBaseValueChangedEventArgs e)
        {
            myMediaElement.Volume = (double)volumeSlider.Value;
        }

        DispatcherTimer timer = null;
        private void Element_MediaOpened(object sender, RoutedEventArgs e)
        {
            timelineSlider.Maximum = myMediaElement.NaturalDuration.TimeSpan.TotalMilliseconds;
            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Tick += timer_tick;
            timer.Start();
        }

        private void timer_tick(object sender, object e)
        {
            timelineSlider.Value = myMediaElement.Position.TotalMilliseconds;
        }
        private void Element_MediaEnded(object sender, RoutedEventArgs e)
        {
            myMediaElement.Stop();
        }

        private void SeekToMediaPosition(object sender, RangeBaseValueChangedEventArgs e)
        {
            int SliderValue = (int)timelineSlider.Value;
            TimeSpan ts = new TimeSpan(0, 0, 0, 0, SliderValue);
            myMediaElement.Position = ts;
        }

        void InitializePropertyValues()
        {
            myMediaElement.Volume = (double)volumeSlider.Value;
        }
        private void myMediaElement_MediaEnded(object sender, RoutedEventArgs e)
        {

        }

        private async void PickFile(object sender, RoutedEventArgs e)
        {
            FileOpenPicker picker = new FileOpenPicker();
            picker.ViewMode = PickerViewMode.List;
            picker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;
            picker.FileTypeFilter.Add(".mp3");
            picker.FileTypeFilter.Add(".mp4");
            StorageFile file = await picker.PickSingleFileAsync();
            var filepath = await file.OpenAsync(Windows.Storage.FileAccessMode.Read);
            if (file.FileType == ".mp4")
            {
                pic.Visibility = Visibility.Collapsed;
                myMediaElement.SetSource(filepath, ".mp4");
            } else
            {
                myMediaElement.SetSource(filepath, ".mp3");
                if (pic.Visibility == Visibility.Collapsed)
                {
                    pic.Visibility = Visibility.Visible;
                }
            }
        }
    }
}
