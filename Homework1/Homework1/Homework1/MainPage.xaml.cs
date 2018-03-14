using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace Homework1
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private delegate string AnimalSaying(object sender);//声明一个委托
        private event AnimalSaying Say;//委托声明一个事件
        public MainPage()
        {
            this.InitializeComponent();
        }
        interface Animal
        {
            string saying(object sender);
        }

        class cat : Animal
        {
            TextBlock word;

            public cat(TextBlock w)
            {
                    this.word = w;
            }
            public string saying(object sender)
            {
                this.word.Text += "Cat: I am a cat\n";
                return "";
            }
        }

        class dog : Animal
        {
            TextBlock word;
            private int a;

            public dog(TextBlock w)
            {
                    this.word = w;
            }
            public string saying(object sender)
            {
                this.word.Text += "Dog: I am a dog\n";
                return "";
            }
        }

        class pig : Animal
        {
            TextBlock word;

            public pig(TextBlock w)
            {
                    this.word = w;
            }
            public string saying(object sender)
            {
                this.word.Text += "Pig: I am a pig\n";
                return "";
            }
        }

        private cat c;
        private dog d;
        private pig p;

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Random rd = new Random();
            int i = rd.Next(1, 4);
            c = new cat(words);
            d = new dog(words);
            p = new pig(words);
            words.Text = "";
            if (i == 3)
            {
                Say += new AnimalSaying(c.saying);
            }
            if (i == 1)
            {
                Say += new AnimalSaying(d.saying);
            }
            if (i == 2)
            {
                Say += new AnimalSaying(p.saying);
            }
            Say(this); 
        }

        private void OK(object sender, RoutedEventArgs e)
        {
            if (input.Text == "cat")
            {
                c = new cat(words);
                words.Text = "";
                Say += new AnimalSaying(c.saying);
                Say(this);
            }
            if (input.Text == "dog")
            {
                d = new dog(words);
                words.Text = "";
                Say += new AnimalSaying(d.saying);
                Say(this);
            }
            if (input.Text == "pig")
            {
                p = new pig(words);
                words.Text = "";
                Say += new AnimalSaying(p.saying);
                Say(this);
            }
            input.Text = "";
        }
    }
}
