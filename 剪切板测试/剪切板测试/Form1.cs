using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace 剪切板测试
{
    public partial class Form1 : Form
    {
        public enum ClipboardFormats : uint
        {
            CF_TEXT = 1,
            CF_BITMAP = 2,
            CF_METAFILEPICT = 3,
            CF_SYLK = 4,
            CF_DIF = 5,
            CF_TIFF = 6,
            CF_OEMTEXT = 7,
            CF_DIB = 8,
            CF_PALETTE = 9,
            CF_PENDATA = 10,
            CF_RIFF = 11,
            CF_WAVE = 12,
            CF_UNICODETEXT = 13,
            CF_ENHMETAFILE = 14
        }

        [DllImport("user32.dll")]
        public static extern bool AddClipboardFormatListener(IntPtr hwnd);

        [DllImport("user32.dll")]
        public static extern bool RemoveClipboardFormatListener(IntPtr hwnd);


        [DllImport("user32.dll")]
        private static extern bool OpenClipboard(IntPtr hWndNewOwner);

        [DllImport("user32.dll")]
        private static extern bool EmptyClipboard();

        [DllImport("user32.dll")]
        private static extern IntPtr GetClipboardData(uint uFormat);

        [DllImport("user32.dll")]
        private static extern IntPtr SetClipboardData(uint uFormat, IntPtr hMem);

        [DllImport("user32.dll")]
        private static extern bool CloseClipboard();

        [DllImport("user32.dll")]
        private static extern uint RegisterClipboardFormat(string lpszFormat);


        private static int WM_CLIPBOARDUPDATE = 0x031D;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            AddClipboardFormatListener(this.Handle);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            RemoveClipboardFormatListener(this.Handle);
        }

        protected override void DefWndProc(ref Message m)
        {
            if (m.Msg == WM_CLIPBOARDUPDATE)
            {
                IDataObject ido = Clipboard.GetDataObject();
                String[] fmt = ido.GetFormats();
                foreach (String s in fmt)
                {
                    Object o = ido.GetData(s);
                    if (o is MemoryStream)
                    {
                        writeToFile((MemoryStream)o, s);
                    }
                }
            }
            else
            {
                base.DefWndProc(ref m);
            }
        }

        public void writeToFile(MemoryStream ms, String name)
        {
            FileStream fs = new FileStream(@"C:\Users\tisn0\Desktop\test\" + name, FileMode.Create);
            byte[] b = new byte[ms.Length];
            ms.Read(b, 0, b.Length);
            fs.Write(b, 0, b.Length);
            ms.Close();
            fs.Close();
        }

        private static String AIClipFormat = "ADOBE AI3";
        private static String PDFClipFormat = "Portable Document Format";
        private static String EPSClipFormat = "Encapsulated Postscript";

        private void button1_Click(object sender, EventArgs e)
        {
            if (OpenClipboard(this.Handle))
            {
                if (EmptyClipboard())
                {
                    FileStream fs = new FileStream(@"C:\Users\tisn0\Desktop\temp.ai", FileMode.Open);
                    byte[] b = new byte[fs.Length];
                    fs.Read(b, 0, b.Length);
                    fs.Close();
                    IntPtr hMem = Marshal.AllocHGlobal(b.Length);
                    Marshal.Copy(b, 0, hMem, b.Length);
                    Marshal.FreeHGlobal(hMem);
                    uint t = RegisterClipboardFormat(AIClipFormat);
                    SetClipboardData(t, hMem);
                }
                CloseClipboard();
            }
        }
    }
}
