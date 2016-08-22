using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace GMS破解
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
                textBox1.Text = openFileDialog1.FileName;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            String fileName = textBox1.Text;
            if (File.Exists(fileName))
            {
                FileStream fs = new FileStream(fileName, FileMode.Open);
                byte[] r = new byte[fs.Length];
                fs.Read(r, 0, (int)fs.Length);
                fs.Close();
                ByteBuffer bout = new ByteBuffer();
                int i = 0;
                bool xors = false;
                for (i = 0; i < r.Length; i++)
                {
                    if (r[i] == 0x2f && r[i + 1] == 0x30 && r[i + 2] == 0xee && r[i + 3] == 0x1f)
                    {
                        xors = true;
                        break;
                    }
                }
                if (xors)
                {
                    bout.Put(Encoding.ASCII.GetBytes("GMS"));
                    bout.Put(0x01);
                    bout.PutIntG(0x0a);
                    bout.PutIntG(0x02);
                    bout.PutChar((char)0x00);
                    bout.PutIntG(0x01);
                    for (int j = i; j < r.Length; j++)
                    {
                        bout.Put((byte)(r[j] ^ 0xff));
                    }
                    File.Move(fileName, fileName + ".bak");
                    FileStream fo = new FileStream(fileName, FileMode.Create);
                    byte[] t1 = bout.ToByteArray();
                    HexUtility.Replace(ref t1, Encoding.ASCII.GetBytes("CMG="), Encoding.ASCII.GetBytes("CMG."));
                    HexUtility.Replace(ref t1, Encoding.ASCII.GetBytes("DPB="), Encoding.ASCII.GetBytes("DPB."));
                    fo.Write(t1, 0, t1.Length);
                    fo.Flush();
                    fo.Close();
                }
                else
                {
                    if (MessageBox.Show("文件未加密！是否尝试移除密码？", "提示", MessageBoxButtons.YesNo) == System.Windows.Forms.DialogResult.Yes)
                    {
                        button3_Click(null, null);
                    }
                }
            }
        }

        private void Form1_DragDrop(object sender, DragEventArgs e)
        {
            textBox1.Text = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
        }

        private void Form1_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Link;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            String fileName = textBox1.Text;
            if (!File.Exists(fileName))
                return;
            FileStream fs = new FileStream(fileName, FileMode.Open);
            byte[] r = new byte[fs.Length];
            fs.Read(r, 0, (int)fs.Length);
            fs.Close();
            File.Move(fileName, fileName + ".bak");
            FileStream fo = new FileStream(fileName, FileMode.Create);
            HexUtility.Replace(ref r, Encoding.ASCII.GetBytes("CMG="), Encoding.ASCII.GetBytes("CMG."));
            HexUtility.Replace(ref r, Encoding.ASCII.GetBytes("DPB="), Encoding.ASCII.GetBytes("DPB."));
            fo.Write(r, 0, r.Length);
            fo.Flush();
            fo.Close();
            MessageBox.Show("密码已移除！");
        }
    }
}
