using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Zhangyue.iReader.EBK3;
using System.IO;
using Ikari;
using System.Collections;

namespace EBK3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                foreach (string f in openFileDialog1.FileNames)
                {
                    listView1.Items.Add(f);
                }
            }
        }

        private void ToTxt(string src, string dst)
        {
            if (!File.Exists(src))
            {
                return;
            }
            Ebk3Open ebk = new Ebk3Open();
            ebk.Open(src);
            FileStream fs = new FileStream(dst + "\\" + Path.GetFileNameWithoutExtension(src) + ".txt", FileMode.Create);
            for (int i = 0; i < ebk.GetChapterCount(); i++)
            {
                byte[] buff = Encoding.UTF8.GetBytes(ebk.EBK3GetChapter(i) + "\n");
                fs.Write(buff, 0, buff.Length);
            }
            fs.Close();
            ebk.EBK3Close();
        }

        private void ToUmd(string src, string dst)
        {
            if (!File.Exists(src))
            {
                return;
            }
            Ebk3Open ebk = new Ebk3Open();
            BookInfo info = ebk.Open(src);
            UMD_GENEGINE umd = new UMD_GENEGINE();
            ArrayList Chapters = new ArrayList();
            ArrayList ChapterStrs = new ArrayList();
            for (int i = 0; i < ebk.GetChapterCount(); i++)
            {
                Chapters.Add(ebk.EBK3GetChapterList()[i].chapter_name_data8);
                ChapterStrs.Add(ebk.EBK3GetChapter(i));
            }
            string aResult;
            umd.Initialize(info.BookName, info.Author == "" ? "未知" : info.Author, DateTime.Now.Year.ToString(), DateTime.Now.Month.ToString(), DateTime.Now.Day.ToString(), "", "", "tisn", "", 0, dst, ref Chapters, ref ChapterStrs, out aResult);
            ProgressBar pb = new ProgressBar();
            umd.Make(ref pb, out aResult);
            ebk.EBK3Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            comboBox1.SelectedIndex = 0;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                textBox1.Text = folderBrowserDialog1.SelectedPath;
            }
        }

        private void 全选AToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < listView1.Items.Count; i++)
            {
                listView1.Items[i].Checked = true;
            }
        }

        private void 反选UToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < listView1.Items.Count; i++)
            {
                listView1.Items[i].Checked = !listView1.Items[i].Checked;
            }
        }

        private void 取消选择CToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < listView1.Items.Count; i++)
            {
                listView1.Items[i].Checked = false;
            }
        }

        private void listView1_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.All;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        private void listView1_DragDrop(object sender, DragEventArgs e)
        {
            foreach (string f in (string[])e.Data.GetData(DataFormats.FileDrop, false))
            {
                if (f.ToLower().EndsWith(".ebk3"))
                {
                    listView1.Items.Add(f);
                }
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (!Directory.Exists(textBox1.Text))
            {
                MessageBox.Show("输出目录不存在，请检查后重试！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (listView1.CheckedItems.Count == 0)
            {
                MessageBox.Show("请至少选择一个文件！", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            for (int i = 0; i < listView1.Items.Count; i++)
            {
                ListViewItem item = listView1.Items[i];
                if (item.Checked)
                {
                    switch (comboBox1.SelectedIndex)
                    {
                        case 0://TXT
                            ToTxt(item.Text, textBox1.Text);
                            break;
                        case 1://UMD
                            ToUmd(item.Text, textBox1.Text);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}
