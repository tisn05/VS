using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Corel.Interop.VGCore;
using Microsoft.Win32.SafeHandles;
using Clipboard = System.Windows.Forms.Clipboard;

namespace cdrPluginT
{
    public partial class frmAIClipBoard : BaseForm
    {
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern bool OpenClipboard(IntPtr hWndNewOwner);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern bool CloseClipboard();
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern bool EmptyClipboard();
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static public extern IntPtr SetClipboardData(uint Format, IntPtr hData);
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
        static extern unsafe IntPtr GlobalAlloc(int flag, int l);
        [DllImport("kernel32")]
        static extern unsafe IntPtr GlobalLock(IntPtr hMem);
        [DllImport("kernel32")]
        static extern unsafe IntPtr GlobalUnlock(IntPtr hMem);
        [DllImport("kernel32")]
        static extern unsafe IntPtr GlobalFree(IntPtr hMem);

        private static String AIClipFormat = "ADOBE AI3";
        private static String PDFClipFormat = "Portable Document Format";
        private static String EPSClipFormat = "Encapsulated Postscript";

        public frmAIClipBoard()
        {
            InitializeComponent();
        }

        public frmAIClipBoard(Corel.Interop.VGCore.Application app)
            : base(app)
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            CopyToClipboard(true, false);
        }

        private void CopyToClipboard(bool bCopyAI = true, bool bCopyPDF = true, bool bCopyEPS = true)
        {
            if (application.ActiveDocument == null || application.ActiveShape == null)
            {
                MessageBox.Show("没有选定对象");
                return;
            }
            String sTempFileAI = null;
            String sTempFilePDF = null;
            String sTempFileEPS = null;
            uint nClipFmtAI = 0, nClipFmtPDF = 0, nClipFmtEPS = 0;
            ExportFilter ef;
            StructExportOptions seo = application.CreateStructExportOptions();
            if (bCopyAI)
            {
                nClipFmtAI = RegisterClipboardFormat(AIClipFormat);
                sTempFileAI = GetTempFile("ai");
                ef = application.ActiveDocument.ExportEx(sTempFileAI, cdrFilter.cdrAI, cdrExportRange.cdrSelection);
                ef.ShowDialog();
                ef.Finish();
            }
            if (bCopyPDF)
            {
                nClipFmtPDF = RegisterClipboardFormat(PDFClipFormat);
                sTempFilePDF = GetTempFile("pdf");
                ef = application.ActiveDocument.ExportEx(sTempFilePDF, cdrFilter.cdrPDF, cdrExportRange.cdrSelection);
                ef.Finish();
            }
            if (bCopyEPS)
            {
                nClipFmtEPS = RegisterClipboardFormat(EPSClipFormat);
                sTempFileEPS = GetTempFile("eps");
                ef = application.ActiveDocument.ExportEx(sTempFileEPS, cdrFilter.cdrEPS, cdrExportRange.cdrSelection);
                ef.Finish();
            }
            if (bCopyAI || bCopyPDF || bCopyEPS)
            {
                if (OpenClipboard((IntPtr)application.AppWindow.Handle))
                {
                    if (EmptyClipboard())
                    {
                        //Clipboard.Clear();
                        if (bCopyAI)
                        {
                            IntPtr hMem = LoadFile(sTempFileAI);
                            SetClipboardData(nClipFmtAI, hMem);
                        }
                        if (bCopyPDF)
                        {
                            IntPtr hMem = LoadFile(sTempFilePDF);
                            SetClipboardData(nClipFmtPDF, hMem);
                        }
                        if (bCopyEPS)
                        {
                            IntPtr hMem = LoadFile(sTempFileEPS);
                            SetClipboardData(nClipFmtEPS, hMem);
                        }
                    }
                    CloseClipboard();
                }
                if (sTempFileAI != null)
                    File.Delete(sTempFileAI);
                if (sTempFilePDF != null)
                    File.Delete(sTempFilePDF);
                if (sTempFileEPS != null)
                    File.Delete(sTempFileEPS);
            }
        }

        private String GetTempFile(String name)
        {
            //return application.CorelScriptTools().GetTempFolder() + "temp." + name;
            return @"D:\" + "temp." + name;
        }

        private IntPtr LoadFile(String fileName)
        {
            IntPtr hMem = IntPtr.Zero;
            IntPtr hFile, lpMemPtr;
            hFile = CreateFile(fileName, 0x80000000, 0, 0, 3, 0x80, 0);
            if (hFile.ToInt64() != -1)
            {
                int nFileSize = GetFileSize(hFile, 0);
                hMem = GlobalAlloc(0, nFileSize);
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
    }
}
