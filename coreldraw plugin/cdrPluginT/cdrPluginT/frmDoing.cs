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
    public partial class frmDoing : Form
    {

        public delegate void DoWorkEventHandler(frmDoing sender, DoWorkEventArgs e);
        public event DoWorkEventHandler DoWork;

        public frmDoing()
        {
            InitializeComponent();
        }
        public void setLabel(int all, int num, String wt)
        {
            label1.Text = String.Format("共{0:D}个{1:1}，正在删除第{0:D}个", all, wt, num);
        }

        public void doClose()
        {
            this.Close();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            if (DoWork != null)
                DoWork(this, e);
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (e.ProgressPercentage >= progressBar1.Minimum && e.ProgressPercentage <= progressBar1.Maximum)
            {
                progressBar1.Value = e.ProgressPercentage;
            }
            if (e.UserState != null)
            {
                label1.Text = e.UserState.ToString();
            }
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                DialogResult = DialogResult.Abort;
            }
            else if (e.Cancelled)
            {
                DialogResult = DialogResult.Cancel;
            }
            else
            {
                DialogResult = DialogResult.OK;
            }
            Close();
        }

        private void frmDoing_Load(object sender, EventArgs e)
        {
            CheckForIllegalCrossThreadCalls = false;
            backgroundWorker1.RunWorkerAsync();
        }

        public void SetProgress(int percent, String status)
        {
            backgroundWorker1.ReportProgress(percent, status);
        }
        public void SetMaxProgress(int p)
        {
            progressBar1.Maximum = p;
        }
    }
}
