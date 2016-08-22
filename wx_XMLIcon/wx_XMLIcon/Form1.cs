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

namespace wx_XMLIcon
{
    public partial class Form1 : Form
    {

        public static String WCMN = "WCmnUI_UIItemBmp";
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                byte[] b = Convert.FromBase64String(textBox1.Text);
                ByteBuffer bb = new ByteBuffer(b);
                bb.Position = 6;
                String s = Encoding.ASCII.GetString(bb.GetByteArray(16));
                if (!s.Equals("WCmnUI_UIItemBmp"))
                {
                    MessageBox.Show("格式错误！！");
                    return;
                }
                bb.Position += 6;
                byte[] b1 = bb.GetByteArray(bb.Length - bb.Position - 1);
                ByteBuffer bb1 = new ByteBuffer(b1);
                bb1.Position = 18;
                int width = bb1.Get();
                bb1.Position = 22;
                int height = bb1.Get();
                bb1.Position = 28;
                int bpp = bb1.Get();
                bb1.Position = 34;
                int blen = bb1.Get() + 256 * bb1.Get();
                int tmp1;
                switch (bpp)
                {
                    case 1:
                        tmp1 = 2 * 4;
                        break;
                    case 4:
                        tmp1 = 16 * 4;
                        break;
                    default:
                        tmp1 = 256 * 4;
                        break;
                }
                int ofs = 0x36 + tmp1;
                blen += ofs;
                bb1.Position = 0;
                byte[] b2 = bb1.GetByteArray(blen);
                ByteBuffer bb2 = new ByteBuffer();
                bb2.Put(b2);
                bb2.Position = 0;
                bb2.Put(Encoding.ASCII.GetBytes("BM"));
                bb2.PutIntG(blen);
                bb2.Position = 10;
                bb2.PutIntG(ofs);
                if (saveFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    FileStream fs = new FileStream(saveFileDialog1.FileName, FileMode.Create);
                    byte[] b4 = bb2.ToByteArray();
                    fs.Write(b4, 0, b4.Length);
                    fs.Flush();
                    fs.Close();
                    MessageBox.Show("图片已保存");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != System.Windows.Forms.DialogResult.OK)
            {
                return;
            }
            FileStream fs = new FileStream(openFileDialog1.FileName, FileMode.Open);
            byte[] b = new byte[fs.Length];
            fs.Read(b, 0, (int)fs.Length);
            fs.Close();
            ByteBuffer bb = new ByteBuffer();
            bb.PutChar((char)0xffff);
            bb.PutInt(0x01001000);
            bb.Put(Encoding.ASCII.GetBytes(WCMN));
            bb.PutInt(0);
            bb.PutChar((char)0);
            int tp = bb.Position;
            bb.Put(b);
            bb.Position = tp;
            bb.PutInt(0x2800);
            bb.PutChar((char)0);
            bb.Position += 4;
            bb.PutInt(0x00010000);
            bb.Position = bb.Length;
            bb.PutInt(0xf0f0f000);
            bb.PutInt(0xa0a0a000);
            bb.PutInt(0xf0f0f000);
            bb.PutInt(0);
            byte[] b2 = bb.ToByteArray();
            String s = Convert.ToBase64String(b2);
            String ns = "";
            for (int i = 0; i <= s.Length / 72; i++)
            {
                ns += s.Substring(i * 72, (i + 1) * 72 > s.Length ? 72 - ((i + 1) * 72 - s.Length) : 72) + "\r\n";
            }
            textBox1.Text = ns;
        }
    }
}
