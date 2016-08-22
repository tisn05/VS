using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;
using System.Runtime.InteropServices;

namespace cdrPluginT
{
    public class BaseForm : Form
    {
        public struct FLASHWINFO
        {
            public UInt32 cbSize;
            public IntPtr hwnd;
            public UInt32 dwFlags;
            public UInt32 uCount;
            public UInt32 dwTimeout;
        }
        private bool bSettingSaved, bOptimization, bEventsEnabled;
        protected Corel.Interop.VGCore.Application application;
        protected String cmdName = "Command";

        [DllImport("user32.dll")]
        public static extern bool FlashWindowEx(ref FLASHWINFO pwfi);

        private bool EventHide = false;
        private bool WithCommand = true;

        public enum falshType : uint
        {
            FLASHW_STOP = 0,    //停止闪烁
            FALSHW_CAPTION = 1,  //只闪烁标题
            FLASHW_TRAY = 2,   //只闪烁任务栏
            FLASHW_ALL = 3,     //标题和任务栏同时闪烁
            FLASHW_PARAM1 = 4,
            FLASHW_PARAM2 = 12,
            FLASHW_TIMER = FLASHW_TRAY | FLASHW_PARAM1,   //无条件闪烁任务栏直到发送停止标志，停止后高亮
            FLASHW_TIMERNOFG = FLASHW_TRAY | FLASHW_PARAM2  //未激活时闪烁任务栏直到发送停止标志或者窗体被激活，停止后高亮
        }

        public static bool flashTaskBar(IntPtr hWnd, falshType type)
        {
            FLASHWINFO fInfo = new FLASHWINFO();
            fInfo.cbSize = Convert.ToUInt32(Marshal.SizeOf(fInfo));
            fInfo.hwnd = hWnd;//要闪烁的窗口的句柄，该窗口可以是打开的或最小化的
            fInfo.dwFlags = (uint)type;//闪烁的类型
            fInfo.uCount = UInt32.MaxValue;//闪烁窗口的次数
            fInfo.dwTimeout = 0; //窗口闪烁的频度，毫秒为单位；若该值为0，则为默认图标的闪烁频度
            return FlashWindowEx(ref fInfo);
        }

        public BaseForm() { }

        public BaseForm(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
            this.application.DocumentClose += application_DocumentClose;
        }

        void application_DocumentClose(Document Doc)
        {
            if (application.Documents.Count == 1)
            {
                this.application.DocumentClose -= application_DocumentClose;
                Close();
            }
        }

        protected void StartEvent()
        {
            StartEvent(false);
        }

        protected void StartEvent(bool hide, bool withCommand = true)
        {
            EventHide = hide;
            if (withCommand)
                application.ActiveDocument.BeginCommandGroup(cmdName);
            bSettingSaved = false;
            bOptimization = false;
            bEventsEnabled = true;
            application.ActiveDocument.SaveSettings();
            bSettingSaved = true;
            if (EventHide)
            {
                bOptimization = application.Optimization;
                bEventsEnabled = application.EventsEnabled;
                application.Optimization = true;
                application.EventsEnabled = false;
            }
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // BaseForm
            // 
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Name = "BaseForm";
            this.Load += new System.EventHandler(this.BaseForm_Load);
            this.ResumeLayout(false);
        }

        private void BaseForm_Load(object sender, EventArgs e)
        {
        }

        protected void EndEvent()
        {
            if (bSettingSaved)
            {
                if (WithCommand)
                    application.ActiveDocument.EndCommandGroup();
                application.ActiveDocument.RestoreSettings();
            }
            if (EventHide)
            {
                application.Optimization = false;
                application.EventsEnabled = bEventsEnabled;
            }
            application.Refresh();
        }
    }
}
