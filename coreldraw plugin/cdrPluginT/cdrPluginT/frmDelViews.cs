using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace cdrPluginT
{
    public partial class frmDelViews : BaseForm
    {
        public frmDelViews()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            backgroundWorker1.RunWorkerAsync();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            this.StartEvent();
            progressBar1.Maximum = application.ActiveDocument.Views.Count;
            backgroundWorker1.ReportProgress(0);
            int i = 1;
            label1.Text = String.Format("共{0:d}个视图", progressBar1.Maximum);
            foreach (Corel.Interop.VGCore.View v in application.ActiveDocument.Views)
            {
                v.Delete();
                i++;
                //backgroundWorker1.ReportProgress(i / progressBar1.Maximum * 100);
                //progressBar1.Value = i;
                //label1.Text = String.Format("正在删除第{0:d}个视图，共{1:d}个视图", i, progressBar1.Maximum);
            }
            backgroundWorker1.ReportProgress(100);
            this.EndEvent();
            MessageBox.Show("已经删除所有视图！");
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (e.ProgressPercentage == 0)
            {
            }
            else if (e.ProgressPercentage == 100)
            {

            }
            else
            {

            }
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {

        }
    }
}
