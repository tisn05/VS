using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Ikari;
using System.Data.SQLite;
using System.Collections;

namespace UC小说转换
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private SQLiteConnection mCon=null;
        private SQLiteCommand mCmd = null;
        private SQLiteDataReader mReader = null;
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.SelectedPath = tbFileName.Text;
            if (fbd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                tbFileName.Text = fbd.SelectedPath;
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            String mdbName = tbFileName.Text + @"\com.UCMobile_catalog";
            mCon = new SQLiteConnection("DataSource='" + mdbName + "';Version=3;New=False;Compress=True;");
            mCon.Open();
            mCmd = new SQLiteCommand(mCon);
            mCmd.CommandText = "SELECT * FROM `CATALOG_TABLE`";
            mReader = mCmd.ExecuteReader();
            while (mReader.Read())
            {
                lvM.Items.Add(mReader.GetString(1));
            }
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            int index = lvM.SelectedIndex;
            if (index == -1)
                return;
            String name = (String)lvM.Items.GetItemAt(index);
            mReader.Close();
            mCmd.CommandText = "SELECT * FROM `CATALOG_TABLE`";// WHERE `novel_name` =  `" + name + "`";
            mReader = mCmd.ExecuteReader();
            mReader.Read();
            String book_id = mReader.GetString(0);
            UCNovel ucn = new UCNovel(tbFileName.Text + @"\" + book_id + @"\" + book_id + ".ucnovel");
            UMD_GENEGINE umd = new UMD_GENEGINE();
            String title = mReader.GetString(1);
            String author = mReader.GetString(2);
            String tName = mReader.GetString(3);
            mReader.Close();
            mCmd.CommandText = "SELECT * FROM '" + tName + "'";
            mReader = mCmd.ExecuteReader();
            ArrayList Chapter = new ArrayList();
            ArrayList Chapstr = new ArrayList();
            while (mReader.Read())
            {
                Chapter.Add(mReader.GetString(2));
                Chapstr.Add(ucn.GetContent(mReader.GetString(9)));
            }
            String result;
            umd.Initialize(title, author, "2015", "01", "01", "", "", "", @"C:\Users\Administrator\Desktop\1", 0, "C:\\", ref Chapter, ref Chapstr, out result);
            umd.Make(ref progressBar1, out result);
            ucn.Close();
        }
    }
}
