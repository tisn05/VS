using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Linq;
using Corel.Interop.VGCore;
using cdrPluginT;
using cdrPluginT.Properties;
using System.IO;

namespace cdrPluginT
{
    public partial class frmToJPG : BaseForm
    {

        private bool doing = false;

        private String outFileName = "";
        private frmTJAsk frm_TJAsk;

        public frmToJPG()
        {
            InitializeComponent();
            this.cmdName = "导出图片";
            CheckForIllegalCrossThreadCalls = false;
        }

        public frmToJPG(Corel.Interop.VGCore.Application app)
            : base(app)
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
                foreach (Document d in application.Documents)
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
                rbSelection.Enabled = application.ActiveDocument.Selection().Shapes.Count != 0;
                rbSelRge.Enabled = rbSelection.Enabled;
                if (!rbSelection.Enabled && rbSelection.Checked)
                {
                    rbCurDocument.Checked = true;
                }
                if (!rbSelRge.Enabled && rbSelRge.Checked)
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
            this.TopMost = checkBox_TopMost.Checked;

            LoadSettings();

            cbHalfOp.SelectedIndex = 0;
            setName();
            rbSelection.Enabled = application.ActiveDocument.Selection().Shapes.Count != 0;
            chklbDocuments.Items.Clear();
            foreach (Document d in application.Documents)
            {
                chklbDocuments.Items.Add(d.Name);
            }
            if (application.ActiveDocument.Pages.Count > 1)
            {
                tbPageNum.Text = String.Format("1-{0:d}", application.ActiveDocument.Pages.Count);
            }
            else
            {
                tbPageNum.Text = "1";
            }
            application.OnApplicationEvent += app_OnApplicationEvent;
            progressBar1.Style = ProgressBarStyle.Blocks;

            cbIncludePageName_CheckedChanged(null, null);
            comboBox_Unit.SelectedIndex = 1;
            comboBox_Type.SelectedIndex = 0;
            comboBox_Type_SelectedIndexChanged(null, null);
            rbDocument_CheckedChanged(null, null);
            frm_TJAsk = new frmTJAsk();
        }

        private void LoadSettings()
        {
            cbCM.SelectedIndex = Settings.Default.ColorMode;
            //cbSmooth.Checked = Settings.Default.Smooth;
            //tbPath.Text = Settings.Default.Path;
            //cboFbl.Text = Settings.Default.Resol.ToString();
            cbYSSF.SelectedIndex = Settings.Default.Ari;
            //checkBox_trans.Checked = Settings.Default.Trans;
            //checkBox_OverPrintBlack.Checked = Settings.Default.OverPrintBlack;
        }

        private void setName()
        {
            if (doing)
                return;
            String n = application.ActiveDocument.Name;
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
                    if (p > application.ActiveDocument.Pages.Count)
                    {
                        addMessage("页码超过了文档的最大页，请重新设置！");
                        tbPageNum.Text = "";
                        tbPageNum.Focus();
                        return new int[0];
                    }
                    list.Add(p);
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
                    foreach (Document d in application.Documents)
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

        private cdrFilter getFilter()
        {
            switch (comboBox_Type.SelectedIndex)
            {
                case 0:
                    return cdrFilter.cdrJPEG;
                case 1:
                    return cdrFilter.cdrGIF;
                case 2:
                    return cdrFilter.cdrPNG;
                case 3:
                    return cdrFilter.cdrBMP;
                case 4:
                    return cdrFilter.cdrPSD;
                default:
                    return cdrFilter.cdrJPEG;
            }
        }

        private String getExt()
        {

            switch (comboBox_Type.SelectedIndex)
            {
                case 0:
                    return ".jpg";
                case 1:
                    return ".gif";
                case 2:
                    return ".png";
                case 3:
                    return ".bmp";
                case 4:
                    return ".psd";
                default:
                    return ".jpg";
            }
        }

        private double[] getSize(Page page)
        {
            double w = 0;
            double h = 0;
            ShapeRange sr;
            if (rbSelection.Checked)
            {
                w = page.SelectableShapes.All().SizeWidth;
                h = page.SelectableShapes.All().SizeHeight;
            }
            else if (cbPageScope.Checked)
            {
                if (cbPageHalf.Checked)
                {
                    bool hz = cbHalfOp.SelectedIndex == 0 ? true : false;
                    if (hz)
                    {
                        w = page.SizeWidth;
                        h = page.SizeHeight / 2;
                    }
                    else
                    {
                        w = page.SizeWidth / 2;
                        h = page.SizeHeight;
                    }
                }
                else
                {
                    w = page.SizeWidth;
                    h = page.SizeHeight;
                }
            }
            else
            {
                w = page.Shapes.All().SizeWidth;
                h = page.Shapes.All().SizeHeight;
            }
            return new double[] { w, h };
        }

        private void ExportJPG(Page page)
        {
            if (page.Shapes.Count == 0)
            {
                addMessage("页面无对象");
                return;
            }
            page.Activate();
            String namen;
            if (rbDocument.Checked)
            {
                namen = application.ActiveDocument.Name;
            }
            else
            {
                namen = outFileName;
            }
            application.ActiveDocument.Unit = cdrUnit.cdrPixel;
            cdrFilter filter = getFilter();
            String ext = getExt();
            String DestDir, FileName;
            ExportFilter ef;
            StructExportOptions seo = application.CreateStructExportOptions();
            seo.AntiAliasingType = cbSmooth.Checked ? cdrAntiAliasingType.cdrSupersampling : cdrAntiAliasingType.cdrNoAntiAliasing;
            seo.ImageType = (cdrImageType)cbCM.SelectedIndex;
            seo.Compression = (cdrCompressionType)cbYSSF.SelectedIndex;
            seo.UseColorProfile = cbUseColorProfile.Checked;

            //特别注意，透明度在jpg等无透明的格式的时候不能为True，不然光滑处理将会失效
            seo.Transparent = checkBox_trans.Checked;
            seo.AlwaysOverprintBlack = checkBox_OverPrintBlack.Checked;
            seo.MaintainLayers = checkBox_MaintainLayers.Checked;


            int s = 72;
            if (cboFbl.Text != "")
            {
                int.TryParse(cboFbl.Text, out s);
            }
            seo.ResolutionX = s;
            seo.ResolutionY = s;

            double sc = 1;
            if (comboBox_SC.Text != "")
            {
                double.TryParse(comboBox_SC.Text, out sc);
            }
            if (sc == 0)
            {
                addMessage("放大部数不能为0，取消。。");
                return;
            }
            if (sc != 1)
            {
                double[] dt = getSize(page);

                seo.SizeX = (int)(dt[0] * sc * (s / 300.0));
                seo.SizeY = (int)(dt[1] * sc * (s / 300.0));
            }

            DestDir = tbPath.Text;
            if (!DestDir.EndsWith("\\"))
            {
                DestDir += "\\";
            }
            if (rbSelection.Checked)
            {
                FileName = DestDir + namen + getPageName(page) + getIndex() + getUnit(0, 0) + ext;
                if (CheckFilExists(FileName))
                {
                    addMessage("文件已存在，取消");
                    return;
                }
                page.Activate();
                ef = application.ActiveDocument.ExportEx(FileName, filter, cdrExportRange.cdrSelection, seo);
                ef.Finish();
                return;
            }
            else if (rbSelRge.Checked)
            {
                Rect rct = application.CreateRect();
                ShapeRange sr = application.ActiveSelectionRange;
                rct.x = sr.LeftX;
                rct.y = sr.BottomY;
                rct.Width = sr.SizeWidth;
                rct.Height = sr.SizeHeight;
                seo.set_ExportArea(ref rct);
                FileName = DestDir + namen + getPageName(page) + getIndex() + getUnit(0, 0) + ext;
                if (CheckFilExists(FileName))
                {
                    addMessage("文件已存在，取消");
                    return;
                }
                page.Activate();
                ef = application.ActiveDocument.ExportEx(FileName, filter, cdrExportRange.cdrCurrentPage, seo);
                ef.Finish();
                return;
            }
            else if (cbPageScope.Checked)
            {
                Rect rct = application.CreateRect();
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
                    FileName = DestDir + namen + getPageName(page) + "-1前" + getIndex() + getUnit(0, 0) + ext;
                    if (CheckFilExists(FileName))
                    {
                        addMessage("文件已存在，取消");
                        return;
                    }
                    ef = application.ActiveDocument.ExportEx(FileName, filter, cdrExportRange.cdrCurrentPage, seo);
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
                    FileName = DestDir + namen + getPageName(page) + "-2后" + getIndex() + getUnit(0, 0) + ext;
                    if (CheckFilExists(FileName))
                    {
                        addMessage("文件已存在，取消");
                        return;
                    }
                    ef = application.ActiveDocument.ExportEx(FileName, filter, cdrExportRange.cdrCurrentPage, seo);
                    ef.Finish();
                    return;
                }
                else
                {
                    rct = application.CreateRect(page.LeftX, page.BottomY, page.SizeWidth, page.SizeHeight);
                    //rct.x = page.LeftX;
                    //rct.y = page.BottomY;
                    //rct.Width = page.SizeWidth;
                    //rct.Height = page.SizeHeight;
                    seo.set_ExportArea(rct);
                    //MessageBox.Show("x:" + rct.x + ",y:" + rct.y + ",width:" + rct.Width + ",height" + rct.Height);
                }
            }
            FileName = DestDir + namen + getPageName(page) + getIndex() + getUnit(0, 0) + ext;
            if (CheckFilExists(FileName))
            {
                addMessage("文件已存在，取消");
                return;
            }
            ef = application.ActiveDocument.ExportEx(FileName, filter, cdrExportRange.cdrCurrentPage, seo);
            ef.Finish();
        }

        private bool CheckFilExists(String fileName)
        {
            if (File.Exists(fileName))
            {
                frm_TJAsk.FileName = fileName;
                if (frm_TJAsk.ShowDialog(this) == DialogResult.OK)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        private void brReset_Click(object sender, EventArgs e)
        {
            setName();
        }

        private void frmToJPG_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveSettings();
            application.OnApplicationEvent -= app_OnApplicationEvent;
        }

        private void SaveSettings()
        {
            //Settings.Default.Path = tbPath.Text;
            Settings.Default.ColorMode = cbCM.SelectedIndex;
            int f = 300;
            int.TryParse(cboFbl.Text, out f);
            Settings.Default.Resol = f;
            //Settings.Default.Smooth = cbSmooth.Checked;
            Settings.Default.Ari = cbYSSF.SelectedIndex;
            //Settings.Default.Trans = checkBox_trans.Checked;
            //Settings.Default.OverPrintBlack = checkBox_OverPrintBlack.Checked;
            Settings.Default.Save();
        }

        private void btExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void btAdvance_Click(object sender, EventArgs e)
        {
            ExportFilter ef;
            if (application.ActiveDocument.ActivePage.SelectableShapes.Count > 0)
            {
                ef = application.ActiveDocument.ExportEx("C:\\tmp.jpg", cdrFilter.cdrJPEG);
                if (ef.ShowDialog())
                {
                }
            }
        }
        private int index = 0;
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                backgroundWorker1.ReportProgress(0);
                addMessage("开始处理");
                doing = true;
                index = 1;
                if (cbUnit.Checked)
                {
                    switch (comboBox_Unit.SelectedIndex)
                    {
                        case 0:
                            application.ActiveDocument.Unit = cdrUnit.cdrMillimeter;
                            break;
                        case 1:
                            application.ActiveDocument.Unit = cdrUnit.cdrCentimeter;
                            break;
                        case 2:
                            application.ActiveDocument.Unit = cdrUnit.cdrMeter;
                            break;
                    }
                }
                outFileName = tbName.Text;
                if (rbCurDocument.Checked)
                {
                    addMessage("导出当前文档");
                    ExportDocument(application.ActiveDocument);
                }
                else if (rbDocument.Checked)
                {
                    addMessage("导出文档");
                    foreach (Document d in GetSelctionDocuments())
                    {
                        addMessage("  导出文档：" + d.Name);
                        ExportDocument(d);
                    }
                }
                else if (rbCurPage.Checked)
                {
                    addMessage("导出当前页面");
                    ExportJPG(application.ActiveDocument.ActivePage);
                }
                else if (rbPage.Checked)
                {
                    addMessage("导出页码范围");
                    int[] ps = AnalysisPageNum();
                    if (ps.Length == 0)
                    {
                        addMessage("无效页面范围，请重新输入");
                        goto done;
                    }
                    foreach (int n in ps)
                    {
                        ExportJPG(application.ActiveDocument.Pages[n]);
                    }
                }
                else if (rbSelection.Checked)
                {
                    addMessage("导出选中对象");
                    ExportJPG(application.ActiveDocument.ActivePage);
                }
                else if (rbSelRge.Checked)
                {
                    addMessage("导出选中范围");
                    ExportJPG(application.ActiveDocument.ActivePage);
                }
                else
                {
                    addMessage("错误错误错误！！！");
                    MessageBox.Show("错误错误错误！！！");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        done:
            doing = false;
            backgroundWorker1.ReportProgress(100);
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
                if (application.ActiveDocument.Pages.Count > 1)
                {
                    tbPageNum.Text = String.Format("1-{0:d}", application.ActiveDocument.Pages.Count);
                }
                else
                {
                    tbPageNum.Text = "1";
                }
            }
            if (rbCurPage.Checked)
            {
                tbPageNum.Text = String.Format("{0:d}", application.ActiveDocument.ActivePage.Index);
            }
            groupBox2.Enabled = !rbSelection.Checked;
            if (groupBox2.Enabled)
                groupBox2.Enabled = !rbSelRge.Checked;
            tbName.Enabled = !rbDocument.Checked;
            tbPageNum.Enabled = rbPage.Checked;
        }

        private void addMessage(String msg)
        {
            listBox1.Items.Add(msg);
            listBox1.SelectedIndex = listBox1.Items.Count - 1;
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

        private void checkBox_TopMost_CheckedChanged(object sender, EventArgs e)
        {
            this.TopMost = checkBox_TopMost.Checked;
        }

        private void groupBox3_Enter(object sender, EventArgs e)
        {

        }

        private void comboBox_Type_SelectedIndexChanged(object sender, EventArgs e)
        {
            bool trans = (comboBox_Type.SelectedIndex != 0 && comboBox_Type.SelectedIndex != 3);
            checkBox_trans.Enabled = trans;
            if (!trans)
                checkBox_trans.Checked = false;
            bool ml = comboBox_Type.SelectedIndex == 4;
            checkBox_MaintainLayers.Enabled = ml;
            if (!ml)
                checkBox_MaintainLayers.Checked = false;
        }

        private void checkBox_trans_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox_OverPrintBlack_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void skinButton1_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void rbSelRge_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
