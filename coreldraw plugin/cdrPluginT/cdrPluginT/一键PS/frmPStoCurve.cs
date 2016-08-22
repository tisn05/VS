using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;
using Microsoft.Win32;

using Bitmap = Corel.Interop.VGCore.Bitmap;
using cdrPluginT.Properties;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;

namespace cdrPluginT
{
    public partial class frmPStoCurve : BaseForm
    {

        private DateTime dtB, dtA;
        private String CPtemp;

        private double d11, d12, d21, d22;
        private double tx, ty;

        private ShapeRange BitmapA;

        [DllImport("user32.dll")]
        private static extern bool SetForegroundWindow(IntPtr hWnd);

        public frmPStoCurve()
        {
            InitializeComponent();
        }

        public frmPStoCurve(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.cmdName = "tisnplugin_一键PS";
            this.application = app;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (application.ActiveDocument.Selection().Shapes.Count == 0)
            {
                MessageBox.Show("请选择一个对象");
                return;
            }
            if (application.ActiveDocument.Selection().Shapes.Count > 1)
            {
                MessageBox.Show("只能选择一个对象");
                return;
            }
            if (application.ActiveShape.Type != cdrShapeType.cdrBitmapShape)
            {
                MessageBox.Show("请选择一个位图");
                return;
            }

            String PSPath = Settings.Default.PSPath;
            if (PSPath == "")
            {
                PSPath = getRegPSPath();
            }
            if (PSPath == "")
            {
                MessageBox.Show("未找到PS主程序photoshop.exe，请手动设置");
                frmSetPSPath f = new frmSetPSPath();
                f.Show(this);
                return;
            }

            StartEvent(true);
            BitmapA = application.ActiveSelectionRange;
            application.ActiveShape.GetMatrix(out d11, out d12, out d21, out d22, out tx, out ty);
            application.ActiveShape.ClearTransformations();

            Bitmap b = application.ActiveShape.Bitmap;

            CPtemp = application.CorelScriptTools().GetTempFolder() + "tisn2015.psd";
            try
            {
                ExportFilter expflt = application.ActiveDocument.ExportBitmap(CPtemp, cdrFilter.cdrPSD, cdrExportRange.cdrSelection, b.Mode, b.SizeWidth, b.SizeHeight, b.ResolutionX, b.ResolutionY, b.Mode == cdrImageType.cdr16ColorsImage ? cdrAntiAliasingType.cdrNoAntiAliasing : cdrAntiAliasingType.cdrNormalAntiAliasing, false, b.Mode == cdrImageType.cdr16ColorsImage ? false : true, true, false, cdrCompressionType.cdrCompressionNone);
                expflt.Finish();
            }
            catch (Exception ex)
            {
                MessageBox.Show("图片不在页面范围，请放入页面范围后重试！");
                goto end;
            }
            application.ActiveShape.SetMatrix(d11, d12, d21, d22, tx, ty);

            dtB = File.GetLastWriteTime(CPtemp);
            Process.Start(PSPath, CPtemp);
            if (MessageBox.Show("编辑完成单击确认，放弃编辑单击取消", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Information) == System.Windows.Forms.DialogResult.OK)
            {
                dtA = File.GetLastWriteTime(CPtemp);
                if (dtA > dtB)
                {
                    EditEnd();
                }
            }
            end:
            EndEvent();
            SetForegroundWindow(new IntPtr(application.AppWindow.Handle));
            //application.AppWindow.WindowState = cdrWindowState.cdrWindowRestore;
            //timer1.Start();

        }

        private String getRegPSPath()
        {
            String PSPath = "";
            RegistryKey rk = Registry.LocalMachine;
            RegistryKey ps;
            ps = rk.OpenSubKey("SOFTWARE\\Adobe\\Photoshop");
            if (ps == null)
            {
                rk = Registry.CurrentUser;
                ps = rk.OpenSubKey("SOFTWARE\\Adobe\\Photoshop");
                if (ps == null)
                    return "";
            }
            try
            {
                String[] ks = ps.GetSubKeyNames();
                RegistryKey ps1 = ps.OpenSubKey(ks[0]);
                PSPath = (String)ps1.GetValue("ApplicationPath", "");
                Settings.Default.PSPath = PSPath;
                Settings.Default.Save();
            }
            catch (Exception ex)
            {
                return "";
            }
            return PSPath + "Photoshop.exe";
        }

        private void EditEnd()
        {
            ImportFilter impflt;
            StructImportOptions impopt;
            impopt = application.CreateStructImportOptions();
            impopt.Mode = cdrImportMode.cdrImportFull;
            impopt.MaintainLayers = true;
            impflt = application.ActiveLayer.ImportEx(CPtemp, cdrFilter.cdrPSD, impopt);
            impflt.Finish();
            application.ActiveShape.SetMatrix(d11, d12, d21, d22, tx, ty);
            application.ActiveShape.OrderToFront();
            BitmapA.Delete();
            File.Delete(CPtemp);
            //EndEvent();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            dtA = File.GetLastWriteTime(CPtemp);
            if (dtA > dtB)
            {
                if (!FileIsUsed(CPtemp))
                {
                    timer1.Stop();
                    EditEnd();
                }
            }
        }

        /// <summary>
        /// 返回指示文件是否已被其它程序使用的布尔值
        /// </summary>
        /// <param name="fileFullName">文件的完全限定名，例如：“C:\MyFile.txt”。</param>
        /// <returns>如果文件已被其它程序使用，则为 true；否则为 false。</returns>
        public static Boolean FileIsUsed(String fileFullName)
        {
            Boolean result = false;
            //判断文件是否存在，如果不存在，直接返回 false
            if (!System.IO.File.Exists(fileFullName))
            {
                result = false;
            }//end: 如果文件不存在的处理逻辑
            else
            {//如果文件存在，则继续判断文件是否已被其它程序使用
                //逻辑：尝试执行打开文件的操作，如果文件已经被其它程序使用，则打开失败，抛出异常，根据此类异常可以判断文件是否已被其它程序使用。
                System.IO.FileStream fileStream = null;
                try
                {
                    fileStream = System.IO.File.Open(fileFullName, System.IO.FileMode.Open, System.IO.FileAccess.ReadWrite, System.IO.FileShare.None);
                    result = false;
                }
                catch (System.IO.IOException ioEx)
                {
                    result = true;
                }
                catch (System.Exception ex)
                {
                    result = true;
                }
                finally
                {
                    if (fileStream != null)
                    {
                        fileStream.Close();
                    }
                }
            }//end: 如果文件存在的处理逻辑
            //返回指示文件是否已被其它程序使用的值
            return result;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            frmSetPSPath f = new frmSetPSPath();
            f.Show(this);
        }

        private void frmPStoCurve_Load(object sender, EventArgs e)
        {
            int x = Screen.PrimaryScreen.WorkingArea.Size.Width - this.Width;
            int y = Screen.PrimaryScreen.WorkingArea.Size.Height - this.Height;
            this.SetDesktopLocation(x, y);
        }
    }
}
