using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;

namespace cdrTest
{
    public partial class frmConvertTo : BaseForm
    {
        public frmConvertTo()
        {
            InitializeComponent();
            this.cmdName = "文字转曲";
        }
        private void btStart_Click(object sender, EventArgs e)
        {
            backgroundWorker1.RunWorkerAsync();
        }
        private void Convert(Page p)
        {
            ShapeRange sr;
            sr = p.SelectableShapes.FindShapes(null, cdrShapeType.cdrTextShape);
            if (sr.Count > 0)
            {
                sr.ConvertToCurves();
            }
            foreach (Shape s in p.SelectableShapes.FindShapes(Query: "@com.PowerClip <> null"))
            {
                sr = s.PowerClip.Shapes.FindShapes(null, cdrShapeType.cdrTextShape);
                if (sr.Count > 0)
                {
                    sr.ConvertToCurves();
                }
            }

        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            backgroundWorker1.ReportProgress(0);

            this.StartEvent();
            if (rbCurPage.Checked)
            {
                backgroundWorker1.ReportProgress(50, "正在处理当前页");
                convertCurPage();
            }
            else
            {
                convertAllPages();
            }
            backgroundWorker1.ReportProgress(100);
            this.EndEvent();
        }

        private void convertCurPage()
        {
            if (app.ActiveDocument.ActivePage.SelectableShapes.Count > 0)
            {
                Convert(app.ActiveDocument.ActivePage);
            }
        }

        private void convertAllPages()
        {

            float n = 1;
            foreach (Page p in app.ActiveDocument.Pages)
            {
                if (p.SelectableShapes.Count == 0)
                    continue;
                Convert(p);
                n++;
            }
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar1.Value = e.ProgressPercentage;
            if (e.ProgressPercentage == 0)
            {
                label1.Text = "开始处理";
            }
            else if (e.ProgressPercentage == 100)
            {
                label1.Text = "处理完成";
            }
            else
            {
                label1.Text = e.UserState as String;
            }
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            timer1.Enabled = true;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            label1.Text = "待处理";
            progressBar1.Value = 0;
            timer1.Enabled = false;
        }

        private void frmConvertTo_Load(object sender, EventArgs e)
        {
            label2.Text = "当前文档：" + app.ActiveDocument.Name;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StartEvent();
            convertAllPages();
            frmInput f = new frmInput();
            f.value = app.ActiveDocument.Name;
            if (f.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                StructSaveAsOptions sso = app.CreateStructSaveAsOptions();
                sso.Version = cdrFileVersion.cdrVersion14;
                app.ActiveDocument.SaveAs(Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory) + "\\" + f.value, sso);
            }
            EndEvent();
        }
    }
}
