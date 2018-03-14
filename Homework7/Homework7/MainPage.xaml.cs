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
using System.Net.Http;
using Windows.Data.Xml.Dom;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace Homework7
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

        private void Search(object sender, RoutedEventArgs e)
        {
            placeresult.Text = "";
            birthdayresult.Text = "";
            sexresult.Text = "";
            queryAsync(input.Text);
        }

        async void queryAsync(string id)
        {
            string url = "http://api.k780.com:88/?app=idcard.get&idcard=" + id + "&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=xml";
            HttpClient client = new HttpClient();
            string result = await client.GetStringAsync(url);
            XmlDocument document = new XmlDocument();
            document.LoadXml(result);
            XmlNodeList list = document.GetElementsByTagName("att");
            IXmlNode node = list.Item(0);
            placeresult.Text = node.InnerText;
            list = document.GetElementsByTagName("born");
            node = list.Item(0);
            birthdayresult.Text = node.InnerText;
            list = document.GetElementsByTagName("sex");
            node = list.Item(0);
            sexresult.Text = node.InnerText;
        }
    }
}
