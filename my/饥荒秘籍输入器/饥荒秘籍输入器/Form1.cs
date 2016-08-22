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
        //注册热键的api
        [DllImport("user32")]
        public static extern bool RegisterHotKey(IntPtr hWnd, int id, uint control, Keys vk);
        //解除注册热键的api
        [DllImport("user32")]
        public static extern bool UnregisterHotKey(IntPtr hWnd, int id);


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

        [DllImport("user32.dll")]
        static extern void keybd_event(byte bVk, byte bScan, uint dwFlags, int dwExtraInfo);

        public Form1()
        {
            InitializeComponent();
        }

        List<String> list_材料类 = new List<String>();
        List<String> list_工具武器 = new List<String>();
        List<String> list_穿戴 = new List<String>();
        List<String> list_建筑 = new List<String>();
        List<String> list_食物 = new List<String>();
        List<String> list_植物 = new List<String>();
        List<String> list_动物 = new List<String>();

        private void Form1_Load(object sender, EventArgs e)
        {
            //RegisterHotKey(this.Handle, 53171, 2, Keys.F1);


            String[] tmpList = Properties.Resources.材料类.Split(new String[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            foreach (String l in tmpList)
            {
                String[] t1 = l.Split(new String[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                if (t1.Length != 2)
                    continue;
                list_材料类.Add(t1[1]);
                comboBox_材料类.Items.Add(t1[0] + "(" + t1[1] + ")");
            }
            comboBox_材料类.SelectedIndex = 0;


            tmpList = Properties.Resources.工具武器.Split(new String[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            foreach (String l in tmpList)
            {
                String[] t1 = l.Split(new String[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                if (t1.Length != 2)
                    continue;
                list_工具武器.Add(t1[1]);
                comboBox_工具武器.Items.Add(t1[0] + "(" + t1[1] + ")");
            }
            comboBox_工具武器.SelectedIndex = 0;


            tmpList = Properties.Resources.穿戴.Split(new String[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            foreach (String l in tmpList)
            {
                String[] t1 = l.Split(new String[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                if (t1.Length != 2)
                    continue;
                list_穿戴.Add(t1[1]);
                comboBox_穿戴.Items.Add(t1[0] + "(" + t1[1] + ")");
            }
            comboBox_穿戴.SelectedIndex = 0;


            tmpList = Properties.Resources.建筑.Split(new String[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            foreach (String l in tmpList)
            {
                String[] t1 = l.Split(new String[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                if (t1.Length != 2)
                    continue;
                list_建筑.Add(t1[1]);
                comboBox_建筑.Items.Add(t1[0] + "(" + t1[1] + ")");
            }
            comboBox_建筑.SelectedIndex = 0;


            tmpList = Properties.Resources.食物.Split(new String[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            foreach (String l in tmpList)
            {
                String[] t1 = l.Split(new String[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                if (t1.Length != 2)
                    continue;
                list_食物.Add(t1[1]);
                comboBox_食物.Items.Add(t1[0] + "(" + t1[1] + ")");
            }
            comboBox_食物.SelectedIndex = 0;


            tmpList = Properties.Resources.植物.Split(new String[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            foreach (String l in tmpList)
            {
                String[] t1 = l.Split(new String[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                if (t1.Length != 2)
                    continue;
                list_植物.Add(t1[1]);
                comboBox_植物.Items.Add(t1[0] + "(" + t1[1] + ")");
            }
            comboBox_植物.SelectedIndex = 0;


            tmpList = Properties.Resources.动物.Split(new String[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            foreach (String l in tmpList)
            {
                String[] t1 = l.Split(new String[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                if (t1.Length != 2)
                    continue;
                list_动物.Add(t1[1]);
                comboBox_动物.Items.Add(t1[0] + "(" + t1[1] + ")");
            }
            comboBox_动物.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SK(textBox1.Text);
        }

        public void SK(String k, bool setForegroud = true)
        {
            IntPtr maindHwnd = FindWindow("opengles2.0", "饥荒    3DM汉化版");
            if (maindHwnd == IntPtr.Zero)
                return;

            foreach (InputLanguage iL in InputLanguage.InstalledInputLanguages)
            {
                if (iL.LayoutName == "美式键盘")
                {
                    InputLanguage.CurrentInputLanguage = iL;
                    break;
                }
            }

            if (!setForegroud)
            {
                this.Activate();
            }
            ShowWindow(maindHwnd, SW_SHOWNORMAL);
            SetForegroundWindow(maindHwnd);

            k = k.Replace("(", "{(}");
            k = k.Replace(")", "{)}");

            Thread.Sleep(1000);

            keybd_event(192, 0, 0, 0);

            SendKeys.Send(k);
            SendKeys.Send("{ENTER}");
        }

        public void InputStr(IntPtr myIntPtr, string Input)
        {
            byte[] ch = (ASCIIEncoding.ASCII.GetBytes(Input));
            for (int i = 0; i < ch.Length; i++)
            {
                SendMessage(myIntPtr, WM_CHAR, ch[i], 0);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            SK("GetPlayer().components.builder:GiveAllRecipes()");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            SK("GetPlayer().components.hungerause(true)");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            SK("GetPlayer().components.health:SetInvincible(true)");
        }

        private void button5_Click(object sender, EventArgs e)
        {
            SK("TheSim:FindFirstEntityWithTag(\"minimap\").MiniMap:ShowArea(0,0,0, 10000)");
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            /*if (radioButton2.Checked)
            {
                button_获得物品.Text = "必须使用快捷键！";
                button_获得物品.Enabled = false;
            }
            else
            {
                button_获得物品.Text = "获取";
                button_获得物品.Enabled = true;
            }*/
        }

        private void button_获得物品_Click(object sender, EventArgs e)
        {
            String code = "";
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    code = list_材料类[comboBox_材料类.SelectedIndex];
                    break;
                case 1:
                    code = list_工具武器[comboBox_工具武器.SelectedIndex];
                    break;
                case 2:
                    code = list_穿戴[comboBox_穿戴.SelectedIndex];
                    break;
                case 3:
                    code = list_建筑[comboBox_建筑.SelectedIndex];
                    break;
                case 4:
                    code = list_食物[comboBox_食物.SelectedIndex];
                    break;
                case 5:
                    code = list_植物[comboBox_植物.SelectedIndex];
                    break;
                case 6:
                    code = list_动物[comboBox_动物.SelectedIndex];
                    break;
            }
            if (code == "")
                return;
            bool sf = true;
            if (radioButton1.Checked)
            {
                int ct = 1;
                int.TryParse(textBox_数量.Text, out ct);
                code = String.Format("c_give(\"{0:s}\",{1:d})", code, ct);
            }
            else
            {
                code = "DebugSpawn \"" + code + "\"";
                sf = false;
            }

            SK(code, sf);
        }

        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case 0x0312:  //这个是window消息定义的注册的热键消息  
                    if (m.WParam.ToString() == "53171")   // 按下CTRL+Q隐藏  
                    {
                        button_获得物品_Click(null, null);
                    }
                    break;
            }
            base.WndProc(ref m);
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            //UnregisterHotKey(this.Handle, 53171);
        }
    }
}
