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

namespace 剪切板测试
{
    public partial class Form1 : Form
    {

        [DllImport("user32.dll")]
        public static extern bool AddClipboardFormatListener(IntPtr hwnd);

        [DllImport("user32.dll")]
        public static extern bool RemoveClipboardFormatListener(IntPtr hwnd);

        private const int WM_CLIPBOARDUPDATE = 0x031D;

        #region Definitions
        //Constants for API Calls...
        private const int WM_DRAWCLIPBOARD = 0x308;
        private const int WM_CHANGECBCHAIN = 0x30D;

        //Handle for next clipboard viewer...
        private IntPtr mNextClipBoardViewerHWnd;

        //API declarations...
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern IntPtr SetClipboardViewer(IntPtr hWndNewViewer);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern bool ChangeClipboardChain(IntPtr HWnd, IntPtr HWndNext);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern int SendMessage(IntPtr hWnd, int msg, int wParam, int lParam);
        #endregion

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern bool OpenClipboard(IntPtr hWndNewOwner);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern bool CloseClipboard();
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern bool EmptyClipboard();
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern uint EnumClipboardFormats(uint uFormat);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern uint GetClipboardFormatName(uint uFormat, ref String szFormatName, int len);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern IntPtr SetClipboardData(uint Format, IntPtr hData);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern IntPtr GetClipboardData(uint Format);
        [DllImport("user32.dll", EntryPoint = "RegisterClipboardFormatA")]
        static public extern uint RegisterClipboardFormat(String Format);


        [System.Runtime.InteropServices.DllImport("kernel32", SetLastError = true, ThrowOnUnmappableChar = true, CharSet = System.Runtime.InteropServices.CharSet.Unicode)]
        static extern unsafe System.IntPtr CreateFile
        (
            string FileName,          // file name
            uint DesiredAccess,       // access mode
            uint ShareMode,           // share mode
            uint SecurityAttributes,  // Security Attributes
            uint CreationDisposition, // how to create
            uint FlagsAndAttributes,  // file attributes
            int hTemplateFile         // handle to template file
        );

        [System.Runtime.InteropServices.DllImport("kernel32", SetLastError = true)]
        static extern unsafe bool ReadFile
        (
            IntPtr hFile,      // handle to file
            IntPtr pBuffer,            // data buffer
            int NumberOfBytesToRead,  // number of bytes to read
            ref int pNumberOfBytesRead,  // number of bytes read
            int Overlapped            // overlapped buffer
        );

        [DllImport("kernel32.dll", SetLastError = true)]
        internal static extern int GetFileSize(IntPtr hFile, uint highSize);

        [DllImport("kernel32.dll")]
        internal static extern bool CloseHandle(IntPtr hObject);

        [DllImport("kernel32")]
        static extern unsafe IntPtr GlobalAlloc(int n, int l);
        [DllImport("kernel32")]
        static extern unsafe IntPtr GlobalLock(IntPtr hMem);
        [DllImport("kernel32")]
        static extern unsafe IntPtr GlobalUnlock(IntPtr hMem);
        [DllImport("kernel32")]
        static extern unsafe IntPtr GlobalFree(IntPtr hMem);
        [DllImport("kernel32")]
        static extern unsafe int GlobalSize(IntPtr hMem);


        private static String AIClipFormat = "ADOBE AI3";
        private static String PDFClipFormat = "Portable Document Format";
        private static String EPSClipFormat = "Encapsulated Postscript";
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            String fileName = @"D:\test\a.svg";
            if (OpenClipboard(this.Handle))
            {
                if (EmptyClipboard())
                {
                    String[] files = Directory.GetFiles(@"D:\test\clipboard\a\");
                    foreach (String f in files)
                    {
                        IntPtr hMem = LoadFile(f);
                        uint fmt = RegisterClipboardFormat(Path.GetFileName(f));
                        SetClipboardData(fmt, hMem);
                    }
                    //FileStream fs = new FileStream(@"D:\test\a.svg", FileMode.Open);
                    //byte[] b = new byte[fs.Length];
                    //fs.Read(b, 0, b.Length);
                    //fs.Close();
                    //IntPtr hMem = Marshal.AllocHGlobal(b.Length);
                    //Marshal.Copy(b, 0, hMem, b.Length);
                }
                CloseClipboard();
            }
        }

        #region Message Process
        //Override WndProc to get messages...
        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WM_CLIPBOARDUPDATE:
                    {
                        //The clipboard has changed...
                        //##########################################################################
                        // Process Clipboard Here :)........................
                        //##########################################################################
                        //SendMessage(mNextClipBoardViewerHWnd, m.Msg, m.WParam.ToInt32(), m.LParam.ToInt32());
                        if (!OpenClipboard(this.Handle))
                            return;
                        uint uFormat = EnumClipboardFormats(0);
                        while (uFormat != 0)
                        {
                            DataFormats.Format df = DataFormats.GetFormat((int)uFormat);
                            String lpFormatName = df.Name;
                            IntPtr hMem = GetClipboardData(uFormat);
                            IntPtr b = GlobalLock(hMem);
                            if (b.ToInt64() != 0)
                            {
                                int size = GlobalSize(hMem);
                                byte[] bt = new byte[size];
                                Marshal.Copy(b, bt, 0, size);
                                if (df.Name.Equals("UnicodeText"))
                                {
                                    textBox1.AppendText(Encoding.Unicode.GetString(bt) + Environment.NewLine);
                                }
                                FileStream fs = new FileStream(@"D:\test\clipboard\" + lpFormatName, FileMode.Create);
                                fs.Write(bt, 0, size);
                                fs.Close();
                                GlobalUnlock(hMem);
                            }
                            uFormat = EnumClipboardFormats(uFormat);
                        }
                        CloseClipboard();
                        /*IDataObject ido = Clipboard.GetDataObject();
                        String[] fmt = ido.GetFormats();
                        foreach (String f in fmt)
                        {
                            try
                            {
                                Object o = ido.GetData(f);
                                if (o is MemoryStream)
                                {
                                    MemoryStream ms = (MemoryStream)o;
                                    FileStream fs = new FileStream(@"D:\test\clipboard\" + f, FileMode.Create);
                                    byte[] b = new byte[ms.Length];
                                    ms.Read(b, 0, b.Length);
                                    fs.Write(b, 0, b.Length);
                                    ms.Close();
                                    fs.Close();
                                }
                            }
                            catch (Exception ex)
                            {

                            }
                        }*/
                        break;
                    }
            }
            base.WndProc(ref m);
        }
        #endregion

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
        }

        private IntPtr LoadFile(String fileName)
        {
            IntPtr hMem = IntPtr.Zero;
            IntPtr hFile, lpMemPtr;
            hFile = CreateFile(fileName, 0x80000000, 0, 0, 3, 0x80, 0);
            if (hFile.ToInt64() != -1)
            {
                int nFileSize = GetFileSize(hFile, 0);
                hMem = GlobalAlloc(2, nFileSize);
                if (hMem.ToInt64() != 0)
                {
                    lpMemPtr = GlobalLock(hMem);
                    if (lpMemPtr.ToInt64() != 0)
                    {
                        int nf = 0;
                        if (ReadFile(hFile, lpMemPtr, nFileSize, ref nf, 0) && nf == nFileSize)
                        {
                            GlobalUnlock(hMem);
                        }
                        else
                        {
                            MessageBox.Show("读取时发生临时文件错误");
                            GlobalUnlock(hMem);
                            GlobalFree(hMem);
                        }
                    }
                    else
                    {
                        GlobalFree(hMem);
                    }
                    CloseHandle(hFile);
                }

            }
            else
            {
                MessageBox.Show("无法打开一个临时文件");
            }
            return hMem;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            AddClipboardFormatListener(this.Handle);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            RemoveClipboardFormatListener(this.Handle);
        }
    }
}
