using Corel.Interop.VGCore;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace cdrPluginT
{
    public partial class frmScreen : BaseForm
    {
        const int DRIVERVERSION = 0;
        const int TECHNOLOGY = 2;
        const int HORZSIZE = 4;
        const int VERTSIZE = 6;
        const int HORZRES = 8;
        const int VERTRES = 10;
        const int BITSPIXEL = 12;
        const int PLANES = 14;
        const int NUMBRUSHES = 16;
        const int NUMPENS = 18;
        const int NUMMARKERS = 20;
        const int NUMFONTS = 22;
        const int NUMCOLORS = 24;
        const int PDEVICESIZE = 26;
        const int CURVECAPS = 28;
        const int LINECAPS = 30;
        const int POLYGONALCAPS = 32;
        const int TEXTCAPS = 34;
        const int CLIPCAPS = 36;
        const int RASTERCAPS = 38;
        const int ASPECTX = 40;
        const int ASPECTY = 42;
        const int ASPECTXY = 44;
        const int SHADEBLENDCAPS = 45;
        const int LOGPIXELSX = 88;
        const int LOGPIXELSY = 90;
        const int SIZEPALETTE = 104;
        const int NUMRESERVED = 106;
        const int COLORRES = 108;
        const int PHYSICALWIDTH = 110;
        const int PHYSICALHEIGHT = 111;
        const int PHYSICALOFFSETX = 112;
        const int PHYSICALOFFSETY = 113;
        const int SCALINGFACTORX = 114;
        const int SCALINGFACTORY = 115;
        const int VREFRESH = 116;
        const int DESKTOPVERTRES = 117;
        const int DESKTOPHORZRES = 118;
        const int BLTALIGNMENT = 119;

        [DllImport("gdi32.dll")]
        private static extern int GetDeviceCaps(IntPtr hdc, int Index);
        [DllImport("User32.dll")]
        static extern IntPtr GetDC(IntPtr Hwnd); //其在MSDN中原型为HDC GetDC(HWND hWnd),HDC和HWND都是驱动器句柄（长指针），在C#中只能用IntPtr代替了
        public frmScreen()
        {
            InitializeComponent();
        }

        public frmScreen(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
        }

        int px, py;
        private void frmScreen_Load(object sender, EventArgs e)
        {
            IntPtr hdc = GetDC(IntPtr.Zero);
            int w = GetDeviceCaps(hdc, HORZSIZE);
            int h = GetDeviceCaps(hdc, VERTSIZE);
            textBox1.Text = w.ToString();
            textBox2.Text = h.ToString();
            label1.Text = String.Format("屏幕尺寸为:{0:d}毫米×{1:d}毫米，{2:f}英寸", w, h, Math.Sqrt(w * w + h * h) * 0.03937);
            px = GetDeviceCaps(hdc, HORZRES);
            py = GetDeviceCaps(hdc, VERTRES);
            label2.Text = String.Format("屏幕分辨率为:{0:d}×{1:d}", px, py);
            button1_Click(null, null);

            textBox5.Text = application.GetApplicationPreferenceValue("Zoom Tool", "DisplayPixelsPerMeterX").ToString();
            textBox6.Text = application.GetApplicationPreferenceValue("Zoom Tool", "DisplayPixelsPerMeterY").ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int w, h;
            int.TryParse(textBox1.Text, out w);
            int.TryParse(textBox2.Text, out h);
            textBox3.Text = String.Format("{0:f8}", ((decimal)px) / ((decimal)w) * 1000);
            textBox4.Text = String.Format("{0:f8}", ((decimal)py) / ((decimal)h) * 1000);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            double w1,w2;
            double.TryParse(textBox3.Text, out w1);
            double.TryParse(textBox4.Text, out w2);
            application.SetApplicationPreferenceValue("Zoom Tool", "DisplayPixelsPerMeterX", w1);
            application.SetApplicationPreferenceValue("Zoom Tool", "DisplayPixelsPerMeterY", w2);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string strAppFileName = Process.GetCurrentProcess().MainModule.FileName;
            Process myNewProcess = new Process();
            myNewProcess.StartInfo.FileName = strAppFileName;
            myNewProcess.StartInfo.WorkingDirectory = System.Windows.Forms.Application.ExecutablePath;
            myNewProcess.Start();  
            application.Quit();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            application.ActiveDocument.Unit = cdrUnit.cdrMillimeter;
            application.ActiveLayer.CreateRectangle(0, 0, 155, 77);
            application.ActiveWindow.ActiveView.SetViewPoint(0, 0, 100);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            application.ActiveDocument.Unit = cdrUnit.cdrMillimeter;
            application.ActiveLayer.CreateRectangle(0, 0, 150, 70);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            application.ActiveDocument.Unit = cdrUnit.cdrMillimeter;
            application.ActiveLayer.CreateRectangle(0, 0, 140, 70);
        }

        private void button6_Click(object sender, EventArgs e)
        {

            application.ActiveDocument.Unit = cdrUnit.cdrMillimeter;
            application.ActiveLayer.CreateRectangle(0, 0, 130, 63);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
