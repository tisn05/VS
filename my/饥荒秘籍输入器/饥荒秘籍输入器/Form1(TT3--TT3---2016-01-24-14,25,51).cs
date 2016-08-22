using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using tisn;

namespace 饥荒秘籍输入器
{
    public partial class Form1 : Form
    {
        [DllImport("user32.dll")]
        private static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
        [DllImport("user32.dll")]
        private static extern bool SetForegroundWindow(IntPtr hWnd);
        [DllImport("user32.dll")]
        private static extern IntPtr FindWindow(string lpClassName, string lpWindowName);
        [DllImport("user32.dll")]
        private static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);
        [DllImport("user32.dll")]
        private static extern bool SetCursorPos(int X, int Y);
        [DllImport("user32.dll")]
        private static extern void mouse_event(int dwFlags, int dx, int dy, int dwData, int dwExtraInfo);
        [DllImport("user32.dll")]
        private static extern void keybd_event(byte bVk, byte bScan, uint dwFlags, uint dwExtraInfo);
        [DllImport("user32.dll")]
        private static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndlnsertAfter, int X, int Y, int cx, int cy, uint Flags);
        //ShowWindow参数
        private const int SW_SHOWNORMAL = 1;
        private const int SW_RESTORE = 9;
        private const int SW_SHOWNOACTIVATE = 4;
        //SendMessage参数
        private const int WM_KEYDOWN = 0X100;
        private const int WM_KEYUP = 0X101;
        private const int WM_SYSCHAR = 0X106;
        private const int WM_SYSKEYUP = 0X105;
        private const int WM_SYSKEYDOWN = 0X104;
        private const int WM_CHAR = 0X102;

        public Form1()
        {
            InitializeComponent();
        }


        THotKey hotKey;
        private void Form1_Load(object sender, EventArgs e)
        {
            hotKey = new THotKey();

            //hotKey.Regist(this.Handle,1, Keys.Q, )
        }

        private void button1_Click(object sender, EventArgs e)
        {
            IntPtr maindHwnd = FindWindow("opengles2.0", "饥荒    3DM汉化版");
            if (maindHwnd == IntPtr.Zero)
                return;
            ShowWindow(maindHwnd, SW_SHOWNORMAL);
            SetForegroundWindow(maindHwnd);

            Thread.Sleep(1000);

            //SendMessage(maindHwnd, WM_SYSKEYDOWN, 192, 0);
            //SendMessage(maindHwnd, WM_SYSKEYUP, 192, 0);

            //InputStr(maindHwnd, "c_give(\"meat\",10)");

            SendKeys.Send("c_give");

        }

        public void InputStr(IntPtr myIntPtr, string Input)
        {
            byte[] ch = (ASCIIEncoding.ASCII.GetBytes(Input));
            for (int i = 0; i < ch.Length; i++)
            {
                SendMessage(myIntPtr, WM_CHAR, ch[i], 0);
            }
        }
    }
}
