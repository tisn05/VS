using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Linq;
using Corel.Interop.VGCore;

namespace cdrTest
{
    public partial class frmToJPG : BaseForm
    {

        private bool doing = false;

        private String outFileName = "";

        public frmToJPG()
        {
            InitializeComponent();
            this.cmdName = "导出图片";
            CheckForIllegalCrossThreadCalls = false;
        }

        void app_OnApplicationEvent(string EventName, ref object[] Parameters)
        {
            if (doing)
                return;
            //Console.WriteLine(EventName);
            if (EventName == "ActiveViewChange")
            {
                chklbDocuments.Items.Clear();
                foreach (Document d in app.Documents)
                {
                    chklbDocuments.Items.Add(d.Name);
                }
                setName();
                rbDocument_CheckedChanged(null, null);
            }
            else if (EventName == "PageCountChange" || EventName == "ActivePageChange")
            {
                rbDocument_CheckedChanged(null, null);
            }
            else if (EventName == "SelectionChange")
            {
                rbSelection.Enabled = app.ActiveDocument.Selection().Shapes.Count != 0;
                if (!rbSelection.Enabled && rbSelection.Checked)
                {
                    rbCurDocument.Checked = true;
                }
            }
        }
        private void btBrw_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.SelectedPath = tbPath.Text;
            folderBrowserDialog1.ShowNewFolderButton = true;
            if (folderBrowserDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                tbPath.Text = folderBrowserDialog1.SelectedPath;
            }
        }

        private void frmToJPG_Load(object sender, EventArgs e)
        {
            cbCM.SelectedIndex = Settings1.Default.ColorMode;
            cbSmooth.Checked = Settings1.Default.Smooth;
            tbPath.Text = Settings1.Default.Path;
            cboFbl.Text = Settings1.Default.Resol.ToString();
            cbYSSF.SelectedIndex = Settings1.Default.Ari;
            cbHalfOp.SelectedIndex = 0;
            setName();
            rbSelection.Enabled = app.ActiveDocument.Selection().Shapes.Count != 0;
            chklbDocuments.Items.Clear();
            foreach (Document d in app.Documents)
            {
                chklbDocuments.Items.Add(d.Name);
            }
            if (app.ActiveDocument.Pages.Count > 1)
            {
                tbPageNum.Text = String.Format("1-{0:d}", app.ActiveDocument.Pages.Count);
            }
            else
            {
                tbPageNum.Text = "1";
            }
            app.OnApplicationEvent += app_OnApplicationEvent;
            progressBar1.Style = ProgressBarStyle.Blocks;

            cbIncludePageName_CheckedChanged(null, null);
            comboBox_Unit.SelectedIndex = 1;
        }

        private void setName()
        {
            if (doing)
                return;
            String n = app.ActiveDocument.Name;
            if (n.EndsWith(".cdr"))
            {
                n = n.Replace(".cdr", "");
            }
            tbName.Text = n;
        }

        private void btStart_Click(object sender, EventArgs e)
        {
            backgroundWorker1.RunWorkerAsync();
        }

        /// <summary>
        /// 分析页码
        /// </summary>
        /// <returns></returns>
        private int[] AnalysisPageNum()
        {
            List<int> list = new List<int>();
            String nu = tbPageNum.Text;
            if (nu == "")
            {

            }
            else if (nu.IndexOf(",") == -1)
            {
                if (nu.IndexOf("-") == -1)
                {
                    int p = Convert.ToInt32(nu);
                    if (p > app.ActiveDocument.Pages.Count)
                    {
                        MessageBox.Show("页码超过了文档的最大页，请重新设置！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        tbPageNum.Text = "";
                        tbPageNum.Focus();
                    }
                }
                else
                {
                    string[] strSubPages = nu.Split('-');
                    int intStart = Convert.ToInt32(strSubPages[0].ToString());
                    int intEnd = Convert.ToInt32(strSubPages[1].ToString());
                    for (int i = intStart; i <= intEnd; i++)
                    {
                        list.Add(i);
                    }
                }
            }
            else
            {
                string[] strPages = nu.Split(',');
                for (int i = 0; i < strPages.Length; i++)
                {
                    if (strPages[i].IndexOf("-") == -1)
                        list.Add(Convert.ToInt32(strPages[i]));
                    else
                    {
                        string[] strSubPages = strPages[i].Split('-');
                        int intStart = Convert.ToInt32(strSubPages[0].ToString());
                        int intEnd = Convert.ToInt32(strSubPages[1].ToString());
                        for (int j = intStart; j <= intEnd; j++)
                            list.Add(j);
                    }
                }
            }
            list.Sort();
            return list.Distinct().ToArray();
        }
        /// <summary>
        /// 获取已选中文档
        /// </summary>
        /// <returns></returns>
        private Document[] GetSelctionDocuments()
        {
            List<Document> list = new List<Document>();
            for (int i = 0; i < chklbDocuments.Items.Count; i++)
            {
                if (chklbDocuments.GetItemChecked(i))
                {
                    String name = (String)chklbDocuments.Items[i];
                    foreach (Document d in app.Documents)
                    {
                        if (name == d.Name)
                        {
                            list.Add(d);
                        }
                    }
                }
            }
            return list.ToArray();
        }

        /// <summary>
        /// 导出文档
        /// </summary>
        /// <param name="doc">欲导出的文档</param>
        private void ExportDocument(Document doc)
        {
            addMessage("开始处理文件：" + doc.Name);
            doc.Activate();
            object[] a = new object[] { };
            setName();
            foreach (Page p in doc.Pages)
            {
                addMessage("    处理页面：" + p.Name);
                ExportJPG(p);
            }
            addMessage("文件处理结束：" + doc.Name);
        }

        private String getIndex()
        {
            if (checkBox_autoNum.Checked)
            {
                return "-" + index++;
            }
            return "";
        }

        private String getPageName(Page page)
        {
            if (cbIncludePageName.Checked)
            {
                return "-" + page.Name;
            }
            return "";
        }

        private String getUnit(double width, double height)
        {
            if (cbUnit.Checked)
            {
                return String.Format("-{0,f}×{1,f}", width, height);
            }
            return "";
        }

        private void ExportJPG(Page page)
        {
            if (page.Shapes.Count == 0)
            {
                return;
            }
            String namen;
            if (rbDocument.Checked)
            {
                namen = app.ActiveDocument.Name;
            }
            else
            {
                namen = outFileName;
            }
            String DestDir, FileName;
            ExportFilter ef;
            StructExportOptions seo = new StructExportOptions();
            seo.AntiAliasingType = cbSmooth.Checked ? cdrAntiAliasingType.cdrNormalAntiAliasing : cdrAntiAliasingType.cdrNoAntiAliasing;
            seo.ImageType = (cdrImageType)cbCM.SelectedIndex;
            seo.Compression = (cdrCompressionType)cbYSSF.SelectedIndex;
            seo.UseColorProfile = cbUseColorProfile.Checked;
            int s = 72;
            if (cboFbl.Text != "")
            {
                int.TryParse(cboFbl.Text, out s);
            }
            seo.ResolutionX = s;
            seo.ResolutionY = s;
            DestDir = tbPath.Text;
            if (!DestDir.EndsWith("\\"))
            {
                DestDir += "\\";
            }
            if (rbSelection.Checked)
            {
                FileName = DestDir + namen + getPageName(page) + getIndex() + getUnit(0, 0) + ".jpg";
                page.Activate();
                ef = app.ActiveDocument.ExportEx(FileName, cdrFilter.cdrJPEG, cdrExportRange.cdrSelection, seo);
                ef.Finish();
            }
            else if (cbPageScope.Checked)
            {
                Rect rct = new Rect();
                if (cbPageHalf.Checked)
                {
                    bool hz = cbHalfOp.SelectedIndex == 0 ? true : false;
                    rct.x = page.LeftX;
                    rct.y = page.BottomY;
                    if (hz)
                    {
                        rct.Width = page.SizeWidth;
                        rct.Height = page.SizeHeight / 2;

                        rct.y = page.SizeHeight / 2;
                    }
                    else
                    {
                        rct.Width = page.SizeWidth / 2;
                        rct.Height = page.SizeHeight;
                    }
                    seo.set_ExportArea(rct);
                    FileName = DestDir + namen + getPageName(page) + "-1前" + getIndex() + getUnit(0, 0) + ".jpg";
                    page.Activate();
                    ef = app.ActiveDocument.ExportEx(FileName, cdrFilter.cdrJPEG, cdrExportRange.cdrCurrentPage, seo);
                    ef.Finish();
                    if (hz)
                    {
                        rct.y = page.BottomY;
                    }
                    else
                    {
                        rct.x = page.SizeWidth / 2;
                    }
                    seo.set_ExportArea(rct);
                    FileName = DestDir + tbName.Text + getPageName(page) + "-2后" + getIndex() + getUnit(0, 0) + ".jpg";
                    page.Activate();
                    ef = app.ActiveDocument.ExportEx(FileName, cdrFilter.cdrJPEG, cdrExportRange.cdrCurrentPage, seo);
                    ef.Finish();
                    return;
                }
                else
                {
                    rct.x = page.LeftX;
                    rct.y = page.BottomY;
                    rct.Width = page.SizeWidth;
                    rct.Height = page.SizeHeight;
                    seo.set_ExportArea(rct);
                    //MessageBox.Show("x:" + rct.x + ",y:" + rct.y + ",width:" + rct.Width + ",height" + rct.Height);
                }
            }
            FileName = DestDir + tbName.Text + getPageName(page) + getIndex() + getUnit(0, 0) + ".jpg";
            page.Activate();
            ef = app.ActiveDocument.ExportEx(FileName, cdrFilter.cdrJPEG, cdrExportRange.cdrCurrentPage, seo);
            ef.Finish();
        }

        private void brReset_Click(object sender, EventArgs e)
        {
            setName();
        }

        private void frmToJPG_FormClosing(object sender, FormClosingEventArgs e)
        {
            Settings1.Default.Path = tbPath.Text;
            Settings1.Default.ColorMode = cbCM.SelectedIndex;
            int f = 300;
            int.TryParse(cboFbl.Text, out f);
            Settings1.Default.Resol = f;
            Settings1.Default.Smooth = cbSmooth.Checked;
            Settings1.Default.Ari = cbYSSF.SelectedIndex;
            Settings1.Default.Save();
        }

        private void btExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void btAdvance_Click(object sender, EventArgs e)
        {
            ExportFilter ef;
            if (app.ActiveDocument.ActivePage.SelectableShapes.Count > 0)
            {
                ef = app.ActiveDocument.ExportEx("C:\\tmp.jpg", cdrFilter.cdrJPEG);
                if (ef.ShowDialog())
                {
                }
            }
        }
        private int index = 0;
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            backgroundWorker1.ReportProgress(0);
            //this.StartEvent();
            addMessage("开始处理");
            doing = true;
            index = 1;
            if (cbUnit.Checked)
            {
                switch (comboBox_Unit.SelectedIndex)
                {
                    case 0:
                        app.ActiveDocument.Unit = cdrUnit.cdrMillimeter;
                        break;
                    case 1:
                        app.ActiveDocument.Unit = cdrUnit.cdrCentimeter;
                        break;
                    case 2:
                        app.ActiveDocument.Unit = cdrUnit.cdrMeter;
                        break;
                }
            }
            outFileName = tbName.Text;
            if (rbCurDocument.Checked)
            {
                addMessage("导出当前页面");
                ExportDocument(app.ActiveDocument);
            }
            else if (rbDocument.Checked)
            {
                foreach (Document d in GetSelctionDocuments())
                {
                    ExportDocument(d);
                }
            }
            else if (rbCurPage.Checked)
            {
                ExportJPG(app.ActiveDocument.ActivePage);
            }
            else if (rbPage.Checked)
            {
                addMessage("导出页码范围");
                int[] ps = AnalysisPageNum();
                foreach (int n in ps)
                {
                    ExportJPG(app.ActiveDocument.Pages[n]);
                }
            }
            else if (rbSelection.Checked)
            {
                ExportJPG(app.ActiveDocument.ActivePage);
            }
            doing = false;
            backgroundWorker1.ReportProgress(100);
            //this.EndEvent();
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            //progressBar1.Value = e.ProgressPercentage;
            //progressBar1.Update();
            if (e.ProgressPercentage == 0)
            {
                //progressBar1.Style = ProgressBarStyle.Marquee;
                progressBar1.MarqueeAnimationSpeed = 100;
                label6.Text = "正在处理";
                groupBox3.Enabled = false;
                label6.Update();
            }
            else if (e.ProgressPercentage == 100)
            {
                progressBar1.Value = 100;
                //progressBar1.Style = ProgressBarStyle.Blocks;
                addMessage("所有任务完成");
                label6.Text = "处理完成";
                groupBox3.Enabled = true;
                label6.Update();
                flashTaskBar(this.Handle, falshType.FLASHW_TIMERNOFG);
                //MessageBox.Show("处理完成！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else if (e.ProgressPercentage > 0 && e.ProgressPercentage < 100)
            {
                progressBar1.Value = e.ProgressPercentage;
                label6.Text = (String)e.UserState;
                label6.Update();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            label6.Text = "待处理";
            progressBar1.Value = 0;
            timer1.Enabled = false;
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            timer1.Enabled = true;
        }

        private void cbPageScope_CheckedChanged(object sender, EventArgs e)
        {
            cbPageHalf.Enabled = cbPageScope.Checked;
            if (cbPageHalf.Checked && cbPageScope.Checked)
                cbHalfOp.Enabled = true;
            else
                cbHalfOp.Enabled = false;
        }

        private void cbPageHalf_CheckedChanged(object sender, EventArgs e)
        {
            cbHalfOp.Enabled = cbPageHalf.Checked;
        }

        private void rbDocument_CheckedChanged(object sender, EventArgs e)
        {
            chklbDocuments.Visible = rbDocument.Checked;
            if (rbCurDocument.Checked)
            {
                if (app.ActiveDocument.Pages.Count > 1)
                {
                    tbPageNum.Text = String.Format("1-{0:d}", app.ActiveDocument.Pages.Count);
                }
                else
                {
                    tbPageNum.Text = "1";
                }
            }
            if (rbCurPage.Checked)
            {
                tbPageNum.Text = String.Format("{0:d}", app.ActiveDocument.ActivePage.Index);
            }
            groupBox2.Enabled = !rbSelection.Checked;
            tbName.Enabled = !rbDocument.Checked;
        }

        private void addMessage(String msg)
        {
            listBox1.Items.Add(msg);
        }

        private void tbPageNum_TextChanged(object sender, EventArgs e)
        {
            // rbPage.Checked = true;
        }

        private void cbUseColorProfile_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void cbYSSF_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void tbPath_TextChanged(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void cbCM_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void cbSmooth_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void cboFbl_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void cbIncludePageName_CheckedChanged(object sender, EventArgs e)
        {
            checkBox_autoNum.Enabled = cbIncludePageName.Checked;
            if (!cbIncludePageName.Checked)
                checkBox_autoNum.Checked = true;
        }
    }
}
