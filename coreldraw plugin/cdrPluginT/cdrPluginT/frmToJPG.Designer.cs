namespace cdrPluginT
{
    partial class frmToJPG
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmToJPG));
            this.label6 = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.btStart = new System.Windows.Forms.Button();
            this.btExit = new System.Windows.Forms.Button();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.checkBox_MaintainLayers = new System.Windows.Forms.CheckBox();
            this.checkBox_OverPrintBlack = new System.Windows.Forms.CheckBox();
            this.cbSmooth = new System.Windows.Forms.CheckBox();
            this.cbUseColorProfile = new System.Windows.Forms.CheckBox();
            this.checkBox_trans = new System.Windows.Forms.CheckBox();
            this.label9 = new System.Windows.Forms.Label();
            this.comboBox_SC = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.comboBox_Type = new System.Windows.Forms.ComboBox();
            this.checkBox_TopMost = new System.Windows.Forms.CheckBox();
            this.cbIncludePageName = new System.Windows.Forms.CheckBox();
            this.comboBox_Unit = new System.Windows.Forms.ComboBox();
            this.cbUnit = new System.Windows.Forms.CheckBox();
            this.checkBox_autoNum = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.chklbDocuments = new System.Windows.Forms.CheckedListBox();
            this.rbSelRge = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.cbHalfOp = new System.Windows.Forms.ComboBox();
            this.cbPageHalf = new System.Windows.Forms.CheckBox();
            this.cbPageScope = new System.Windows.Forms.CheckBox();
            this.btAdvance = new System.Windows.Forms.Button();
            this.tbPageNum = new System.Windows.Forms.TextBox();
            this.rbPage = new System.Windows.Forms.RadioButton();
            this.rbSelection = new System.Windows.Forms.RadioButton();
            this.rbCurPage = new System.Windows.Forms.RadioButton();
            this.rbDocument = new System.Windows.Forms.RadioButton();
            this.rbCurDocument = new System.Windows.Forms.RadioButton();
            this.tbName = new System.Windows.Forms.TextBox();
            this.cbYSSF = new System.Windows.Forms.ComboBox();
            this.brReset = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.tbPath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btBrw = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.cbCM = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cboFbl = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label6
            // 
            this.label6.ForeColor = System.Drawing.Color.Red;
            this.label6.Location = new System.Drawing.Point(12, 359);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(342, 23);
            this.label6.TabIndex = 40;
            this.label6.Text = "待处理";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(12, 385);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(342, 23);
            this.progressBar1.Style = System.Windows.Forms.ProgressBarStyle.Marquee;
            this.progressBar1.TabIndex = 39;
            // 
            // btStart
            // 
            this.btStart.Location = new System.Drawing.Point(397, 385);
            this.btStart.Name = "btStart";
            this.btStart.Size = new System.Drawing.Size(75, 23);
            this.btStart.TabIndex = 37;
            this.btStart.Text = "导出";
            this.btStart.UseVisualStyleBackColor = true;
            this.btStart.Click += new System.EventHandler(this.btStart_Click);
            // 
            // btExit
            // 
            this.btExit.Location = new System.Drawing.Point(485, 385);
            this.btExit.Name = "btExit";
            this.btExit.Size = new System.Drawing.Size(75, 23);
            this.btExit.TabIndex = 36;
            this.btExit.Text = "退出";
            this.btExit.UseVisualStyleBackColor = true;
            this.btExit.Click += new System.EventHandler(this.btExit_Click);
            // 
            // folderBrowserDialog1
            // 
            this.folderBrowserDialog1.Description = "选择导出文件夹";
            // 
            // backgroundWorker1
            // 
            this.backgroundWorker1.WorkerReportsProgress = true;
            this.backgroundWorker1.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker1_DoWork);
            this.backgroundWorker1.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker1_ProgressChanged);
            this.backgroundWorker1.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorker1_RunWorkerCompleted);
            // 
            // timer1
            // 
            this.timer1.Interval = 2000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.groupBox4);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Controls.Add(this.comboBox_SC);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.comboBox_Type);
            this.groupBox3.Controls.Add(this.checkBox_TopMost);
            this.groupBox3.Controls.Add(this.cbIncludePageName);
            this.groupBox3.Controls.Add(this.comboBox_Unit);
            this.groupBox3.Controls.Add(this.cbUnit);
            this.groupBox3.Controls.Add(this.checkBox_autoNum);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.groupBox1);
            this.groupBox3.Controls.Add(this.tbName);
            this.groupBox3.Controls.Add(this.cbYSSF);
            this.groupBox3.Controls.Add(this.brReset);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.tbPath);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.btBrw);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.cbCM);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.cboFbl);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Location = new System.Drawing.Point(12, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(595, 344);
            this.groupBox3.TabIndex = 47;
            this.groupBox3.TabStop = false;
            this.groupBox3.Enter += new System.EventHandler(this.groupBox3_Enter);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.checkBox_MaintainLayers);
            this.groupBox4.Controls.Add(this.checkBox_OverPrintBlack);
            this.groupBox4.Controls.Add(this.cbSmooth);
            this.groupBox4.Controls.Add(this.cbUseColorProfile);
            this.groupBox4.Controls.Add(this.checkBox_trans);
            this.groupBox4.Location = new System.Drawing.Point(441, 194);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(137, 132);
            this.groupBox4.TabIndex = 59;
            this.groupBox4.TabStop = false;
            // 
            // checkBox_MaintainLayers
            // 
            this.checkBox_MaintainLayers.AutoSize = true;
            this.checkBox_MaintainLayers.Checked = true;
            this.checkBox_MaintainLayers.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_MaintainLayers.Location = new System.Drawing.Point(15, 109);
            this.checkBox_MaintainLayers.Name = "checkBox_MaintainLayers";
            this.checkBox_MaintainLayers.Size = new System.Drawing.Size(72, 16);
            this.checkBox_MaintainLayers.TabIndex = 59;
            this.checkBox_MaintainLayers.Text = "保持图层";
            this.checkBox_MaintainLayers.UseVisualStyleBackColor = true;
            // 
            // checkBox_OverPrintBlack
            // 
            this.checkBox_OverPrintBlack.AutoSize = true;
            this.checkBox_OverPrintBlack.Checked = global::cdrPluginT.Properties.Settings.Default.OverPrintBlack;
            this.checkBox_OverPrintBlack.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_OverPrintBlack.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::cdrPluginT.Properties.Settings.Default, "OverPrintBlack", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox_OverPrintBlack.Location = new System.Drawing.Point(15, 13);
            this.checkBox_OverPrintBlack.Name = "checkBox_OverPrintBlack";
            this.checkBox_OverPrintBlack.Size = new System.Drawing.Size(96, 16);
            this.checkBox_OverPrintBlack.TabIndex = 58;
            this.checkBox_OverPrintBlack.Text = "总是叠印黑色";
            this.checkBox_OverPrintBlack.UseVisualStyleBackColor = true;
            this.checkBox_OverPrintBlack.CheckedChanged += new System.EventHandler(this.checkBox_OverPrintBlack_CheckedChanged);
            // 
            // cbSmooth
            // 
            this.cbSmooth.AutoSize = true;
            this.cbSmooth.Checked = global::cdrPluginT.Properties.Settings.Default.Smooth;
            this.cbSmooth.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbSmooth.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::cdrPluginT.Properties.Settings.Default, "Smooth", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cbSmooth.Location = new System.Drawing.Point(15, 37);
            this.cbSmooth.Name = "cbSmooth";
            this.cbSmooth.Size = new System.Drawing.Size(72, 16);
            this.cbSmooth.TabIndex = 35;
            this.cbSmooth.Text = "光滑处理";
            this.cbSmooth.UseVisualStyleBackColor = true;
            this.cbSmooth.CheckedChanged += new System.EventHandler(this.cbSmooth_CheckedChanged);
            // 
            // cbUseColorProfile
            // 
            this.cbUseColorProfile.AutoSize = true;
            this.cbUseColorProfile.Checked = true;
            this.cbUseColorProfile.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbUseColorProfile.Location = new System.Drawing.Point(15, 61);
            this.cbUseColorProfile.Name = "cbUseColorProfile";
            this.cbUseColorProfile.Size = new System.Drawing.Size(96, 16);
            this.cbUseColorProfile.TabIndex = 50;
            this.cbUseColorProfile.Text = "使用用户配置";
            this.cbUseColorProfile.UseVisualStyleBackColor = true;
            this.cbUseColorProfile.CheckedChanged += new System.EventHandler(this.cbUseColorProfile_CheckedChanged);
            // 
            // checkBox_trans
            // 
            this.checkBox_trans.AutoSize = true;
            this.checkBox_trans.Checked = true;
            this.checkBox_trans.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_trans.Location = new System.Drawing.Point(15, 85);
            this.checkBox_trans.Name = "checkBox_trans";
            this.checkBox_trans.Size = new System.Drawing.Size(72, 16);
            this.checkBox_trans.TabIndex = 54;
            this.checkBox_trans.Text = "透明处理";
            this.checkBox_trans.UseVisualStyleBackColor = true;
            this.checkBox_trans.CheckedChanged += new System.EventHandler(this.checkBox_trans_CheckedChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(471, 148);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(59, 12);
            this.label9.TabIndex = 57;
            this.label9.Text = "放大部数:";
            // 
            // comboBox_SC
            // 
            this.comboBox_SC.FormattingEnabled = true;
            this.comboBox_SC.Items.AddRange(new object[] {
            "10",
            "5",
            "1",
            "0.1",
            "0.5"});
            this.comboBox_SC.Location = new System.Drawing.Point(473, 168);
            this.comboBox_SC.Name = "comboBox_SC";
            this.comboBox_SC.Size = new System.Drawing.Size(58, 20);
            this.comboBox_SC.TabIndex = 56;
            this.comboBox_SC.Text = "1";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(378, 148);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(35, 12);
            this.label8.TabIndex = 55;
            this.label8.Text = "格式:";
            // 
            // comboBox_Type
            // 
            this.comboBox_Type.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::cdrPluginT.Properties.Settings.Default, "导出图片_格式", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.comboBox_Type.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Type.FormattingEnabled = true;
            this.comboBox_Type.Items.AddRange(new object[] {
            "JPG",
            "GIF",
            "PNG",
            "BMP",
            "PSD"});
            this.comboBox_Type.Location = new System.Drawing.Point(380, 168);
            this.comboBox_Type.Name = "comboBox_Type";
            this.comboBox_Type.Size = new System.Drawing.Size(75, 20);
            this.comboBox_Type.TabIndex = 53;
            this.comboBox_Type.Text = global::cdrPluginT.Properties.Settings.Default.导出图片_格式;
            this.comboBox_Type.SelectedIndexChanged += new System.EventHandler(this.comboBox_Type_SelectedIndexChanged);
            // 
            // checkBox_TopMost
            // 
            this.checkBox_TopMost.AutoSize = true;
            this.checkBox_TopMost.Checked = global::cdrPluginT.Properties.Settings.Default.导出图片_窗口置顶;
            this.checkBox_TopMost.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_TopMost.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::cdrPluginT.Properties.Settings.Default, "导出图片_窗口置顶", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox_TopMost.Location = new System.Drawing.Point(468, 13);
            this.checkBox_TopMost.Name = "checkBox_TopMost";
            this.checkBox_TopMost.Size = new System.Drawing.Size(72, 16);
            this.checkBox_TopMost.TabIndex = 52;
            this.checkBox_TopMost.Text = "窗口置顶";
            this.checkBox_TopMost.UseVisualStyleBackColor = true;
            this.checkBox_TopMost.CheckedChanged += new System.EventHandler(this.checkBox_TopMost_CheckedChanged);
            // 
            // cbIncludePageName
            // 
            this.cbIncludePageName.AutoSize = true;
            this.cbIncludePageName.Checked = true;
            this.cbIncludePageName.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbIncludePageName.Location = new System.Drawing.Point(21, 64);
            this.cbIncludePageName.Name = "cbIncludePageName";
            this.cbIncludePageName.Size = new System.Drawing.Size(96, 16);
            this.cbIncludePageName.TabIndex = 51;
            this.cbIncludePageName.Text = "包含页面名称";
            this.cbIncludePageName.UseVisualStyleBackColor = true;
            this.cbIncludePageName.CheckedChanged += new System.EventHandler(this.cbIncludePageName_CheckedChanged);
            // 
            // comboBox_Unit
            // 
            this.comboBox_Unit.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Unit.FormattingEnabled = true;
            this.comboBox_Unit.Items.AddRange(new object[] {
            "毫米",
            "厘米",
            "米"});
            this.comboBox_Unit.Location = new System.Drawing.Point(429, 64);
            this.comboBox_Unit.Name = "comboBox_Unit";
            this.comboBox_Unit.Size = new System.Drawing.Size(96, 20);
            this.comboBox_Unit.TabIndex = 49;
            this.comboBox_Unit.Visible = false;
            // 
            // cbUnit
            // 
            this.cbUnit.AutoSize = true;
            this.cbUnit.Location = new System.Drawing.Point(279, 64);
            this.cbUnit.Name = "cbUnit";
            this.cbUnit.Size = new System.Drawing.Size(108, 16);
            this.cbUnit.TabIndex = 48;
            this.cbUnit.Text = "文件名后加尺寸";
            this.cbUnit.UseVisualStyleBackColor = true;
            this.cbUnit.Visible = false;
            // 
            // checkBox_autoNum
            // 
            this.checkBox_autoNum.AutoSize = true;
            this.checkBox_autoNum.Checked = true;
            this.checkBox_autoNum.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_autoNum.Location = new System.Drawing.Point(123, 64);
            this.checkBox_autoNum.Name = "checkBox_autoNum";
            this.checkBox_autoNum.Size = new System.Drawing.Size(150, 16);
            this.checkBox_autoNum.TabIndex = 47;
            this.checkBox_autoNum.Text = "文件名后面自动加编号 ";
            this.checkBox_autoNum.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 12);
            this.label1.TabIndex = 24;
            this.label1.Text = "文件名称:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.chklbDocuments);
            this.groupBox1.Controls.Add(this.rbSelRge);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.tbPageNum);
            this.groupBox1.Controls.Add(this.rbPage);
            this.groupBox1.Controls.Add(this.rbSelection);
            this.groupBox1.Controls.Add(this.rbCurPage);
            this.groupBox1.Controls.Add(this.rbDocument);
            this.groupBox1.Controls.Add(this.rbCurDocument);
            this.groupBox1.Location = new System.Drawing.Point(21, 210);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(414, 116);
            this.groupBox1.TabIndex = 46;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "范围选择";
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // chklbDocuments
            // 
            this.chklbDocuments.CheckOnClick = true;
            this.chklbDocuments.FormattingEnabled = true;
            this.chklbDocuments.HorizontalScrollbar = true;
            this.chklbDocuments.Location = new System.Drawing.Point(16, 42);
            this.chklbDocuments.Name = "chklbDocuments";
            this.chklbDocuments.Size = new System.Drawing.Size(184, 68);
            this.chklbDocuments.TabIndex = 49;
            this.chklbDocuments.Visible = false;
            // 
            // rbSelRge
            // 
            this.rbSelRge.AutoSize = true;
            this.rbSelRge.Location = new System.Drawing.Point(129, 80);
            this.rbSelRge.Name = "rbSelRge";
            this.rbSelRge.Size = new System.Drawing.Size(71, 16);
            this.rbSelRge.TabIndex = 51;
            this.rbSelRge.Text = "选定范围";
            this.rbSelRge.UseVisualStyleBackColor = true;
            this.rbSelRge.CheckedChanged += new System.EventHandler(this.rbSelRge_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.cbHalfOp);
            this.groupBox2.Controls.Add(this.cbPageHalf);
            this.groupBox2.Controls.Add(this.cbPageScope);
            this.groupBox2.Controls.Add(this.btAdvance);
            this.groupBox2.Location = new System.Drawing.Point(207, 16);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(200, 84);
            this.groupBox2.TabIndex = 50;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "页面范围";
            // 
            // cbHalfOp
            // 
            this.cbHalfOp.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbHalfOp.Enabled = false;
            this.cbHalfOp.FormattingEnabled = true;
            this.cbHalfOp.Items.AddRange(new object[] {
            "横向分割",
            "纵向分割"});
            this.cbHalfOp.Location = new System.Drawing.Point(105, 54);
            this.cbHalfOp.Name = "cbHalfOp";
            this.cbHalfOp.Size = new System.Drawing.Size(75, 20);
            this.cbHalfOp.TabIndex = 51;
            // 
            // cbPageHalf
            // 
            this.cbPageHalf.AutoSize = true;
            this.cbPageHalf.Enabled = false;
            this.cbPageHalf.Location = new System.Drawing.Point(15, 56);
            this.cbPageHalf.Name = "cbPageHalf";
            this.cbPageHalf.Size = new System.Drawing.Size(84, 16);
            this.cbPageHalf.TabIndex = 50;
            this.cbPageHalf.Text = "页面一分二";
            this.cbPageHalf.UseVisualStyleBackColor = true;
            this.cbPageHalf.CheckedChanged += new System.EventHandler(this.cbPageHalf_CheckedChanged);
            // 
            // cbPageScope
            // 
            this.cbPageScope.AutoSize = true;
            this.cbPageScope.Location = new System.Drawing.Point(15, 31);
            this.cbPageScope.Name = "cbPageScope";
            this.cbPageScope.Size = new System.Drawing.Size(72, 16);
            this.cbPageScope.TabIndex = 49;
            this.cbPageScope.Text = "页面范围";
            this.cbPageScope.UseVisualStyleBackColor = true;
            this.cbPageScope.CheckedChanged += new System.EventHandler(this.cbPageScope_CheckedChanged);
            // 
            // btAdvance
            // 
            this.btAdvance.Enabled = false;
            this.btAdvance.Location = new System.Drawing.Point(105, 20);
            this.btAdvance.Name = "btAdvance";
            this.btAdvance.Size = new System.Drawing.Size(75, 23);
            this.btAdvance.TabIndex = 38;
            this.btAdvance.Text = "高级";
            this.btAdvance.UseVisualStyleBackColor = true;
            this.btAdvance.Visible = false;
            this.btAdvance.Click += new System.EventHandler(this.btAdvance_Click);
            // 
            // tbPageNum
            // 
            this.tbPageNum.Enabled = false;
            this.tbPageNum.Location = new System.Drawing.Point(57, 77);
            this.tbPageNum.Name = "tbPageNum";
            this.tbPageNum.Size = new System.Drawing.Size(66, 21);
            this.tbPageNum.TabIndex = 5;
            this.tbPageNum.TextChanged += new System.EventHandler(this.tbPageNum_TextChanged);
            // 
            // rbPage
            // 
            this.rbPage.AutoSize = true;
            this.rbPage.Location = new System.Drawing.Point(16, 80);
            this.rbPage.Name = "rbPage";
            this.rbPage.Size = new System.Drawing.Size(35, 16);
            this.rbPage.TabIndex = 4;
            this.rbPage.Text = "页";
            this.rbPage.UseVisualStyleBackColor = true;
            this.rbPage.CheckedChanged += new System.EventHandler(this.rbDocument_CheckedChanged);
            // 
            // rbSelection
            // 
            this.rbSelection.AutoSize = true;
            this.rbSelection.Location = new System.Drawing.Point(129, 50);
            this.rbSelection.Name = "rbSelection";
            this.rbSelection.Size = new System.Drawing.Size(71, 16);
            this.rbSelection.TabIndex = 3;
            this.rbSelection.Text = "选定内容";
            this.rbSelection.UseVisualStyleBackColor = true;
            this.rbSelection.CheckedChanged += new System.EventHandler(this.rbDocument_CheckedChanged);
            // 
            // rbCurPage
            // 
            this.rbCurPage.AutoSize = true;
            this.rbCurPage.Location = new System.Drawing.Point(16, 50);
            this.rbCurPage.Name = "rbCurPage";
            this.rbCurPage.Size = new System.Drawing.Size(59, 16);
            this.rbCurPage.TabIndex = 2;
            this.rbCurPage.Text = "当前页";
            this.rbCurPage.UseVisualStyleBackColor = true;
            this.rbCurPage.CheckedChanged += new System.EventHandler(this.rbDocument_CheckedChanged);
            // 
            // rbDocument
            // 
            this.rbDocument.AutoSize = true;
            this.rbDocument.Location = new System.Drawing.Point(129, 20);
            this.rbDocument.Name = "rbDocument";
            this.rbDocument.Size = new System.Drawing.Size(47, 16);
            this.rbDocument.TabIndex = 1;
            this.rbDocument.Text = "文档";
            this.rbDocument.UseVisualStyleBackColor = true;
            this.rbDocument.CheckedChanged += new System.EventHandler(this.rbDocument_CheckedChanged);
            // 
            // rbCurDocument
            // 
            this.rbCurDocument.AutoSize = true;
            this.rbCurDocument.Checked = true;
            this.rbCurDocument.Location = new System.Drawing.Point(16, 20);
            this.rbCurDocument.Name = "rbCurDocument";
            this.rbCurDocument.Size = new System.Drawing.Size(71, 16);
            this.rbCurDocument.TabIndex = 0;
            this.rbCurDocument.TabStop = true;
            this.rbCurDocument.Text = "当前文档";
            this.rbCurDocument.UseVisualStyleBackColor = true;
            this.rbCurDocument.CheckedChanged += new System.EventHandler(this.rbDocument_CheckedChanged);
            // 
            // tbName
            // 
            this.tbName.Location = new System.Drawing.Point(21, 37);
            this.tbName.Name = "tbName";
            this.tbName.Size = new System.Drawing.Size(434, 21);
            this.tbName.TabIndex = 23;
            // 
            // cbYSSF
            // 
            this.cbYSSF.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbYSSF.FormattingEnabled = true;
            this.cbYSSF.Items.AddRange(new object[] {
            "无压缩",
            "LZW",
            "PackBits",
            "Huffman",
            "CCITT3_1d",
            "CCITT3_2d",
            "CCITT4",
            "RLE_LW",
            "ZIP",
            "JPEG"});
            this.cbYSSF.Location = new System.Drawing.Point(152, 168);
            this.cbYSSF.Name = "cbYSSF";
            this.cbYSSF.Size = new System.Drawing.Size(121, 20);
            this.cbYSSF.TabIndex = 42;
            this.cbYSSF.SelectedIndexChanged += new System.EventHandler(this.cbYSSF_SelectedIndexChanged);
            // 
            // brReset
            // 
            this.brReset.Location = new System.Drawing.Point(468, 35);
            this.brReset.Name = "brReset";
            this.brReset.Size = new System.Drawing.Size(75, 23);
            this.brReset.TabIndex = 25;
            this.brReset.Text = "重置";
            this.brReset.UseVisualStyleBackColor = true;
            this.brReset.Click += new System.EventHandler(this.brReset_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(150, 148);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(59, 12);
            this.label7.TabIndex = 41;
            this.label7.Text = "压缩算法:";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // tbPath
            // 
            this.tbPath.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::cdrPluginT.Properties.Settings.Default, "Path", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.tbPath.Location = new System.Drawing.Point(21, 109);
            this.tbPath.Name = "tbPath";
            this.tbPath.Size = new System.Drawing.Size(434, 21);
            this.tbPath.TabIndex = 26;
            this.tbPath.Text = global::cdrPluginT.Properties.Settings.Default.Path;
            this.tbPath.TextChanged += new System.EventHandler(this.tbPath_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(19, 89);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 12);
            this.label2.TabIndex = 27;
            this.label2.Text = "保存位置:";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // btBrw
            // 
            this.btBrw.Location = new System.Drawing.Point(468, 107);
            this.btBrw.Name = "btBrw";
            this.btBrw.Size = new System.Drawing.Size(75, 23);
            this.btBrw.TabIndex = 28;
            this.btBrw.Text = "浏览";
            this.btBrw.UseVisualStyleBackColor = true;
            this.btBrw.Click += new System.EventHandler(this.btBrw_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(19, 148);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 12);
            this.label3.TabIndex = 29;
            this.label3.Text = "颜色模式:";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // cbCM
            // 
            this.cbCM.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbCM.FormattingEnabled = true;
            this.cbCM.Items.AddRange(new object[] {
            "黑白",
            "灰度",
            "调色版",
            "16位",
            "RGB",
            "CMYK",
            "双色",
            "颜色模型",
            "CMYK多通道",
            "RGB多通道",
            "专色多通道"});
            this.cbCM.Location = new System.Drawing.Point(21, 168);
            this.cbCM.Name = "cbCM";
            this.cbCM.Size = new System.Drawing.Size(121, 20);
            this.cbCM.TabIndex = 30;
            this.cbCM.SelectedIndexChanged += new System.EventHandler(this.cbCM_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(282, 148);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 12);
            this.label4.TabIndex = 31;
            this.label4.Text = "分辨率:";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // cboFbl
            // 
            this.cboFbl.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::cdrPluginT.Properties.Settings.Default, "导出图片_分辨率", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.cboFbl.FormattingEnabled = true;
            this.cboFbl.Items.AddRange(new object[] {
            "72",
            "96",
            "150",
            "200",
            "300",
            "500",
            "600",
            "1200"});
            this.cboFbl.Location = new System.Drawing.Point(284, 168);
            this.cboFbl.Name = "cboFbl";
            this.cboFbl.Size = new System.Drawing.Size(58, 20);
            this.cboFbl.TabIndex = 32;
            this.cboFbl.Text = global::cdrPluginT.Properties.Settings.Default.导出图片_分辨率;
            this.cboFbl.SelectedIndexChanged += new System.EventHandler(this.cboFbl_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(348, 174);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(23, 12);
            this.label5.TabIndex = 33;
            this.label5.Text = "dpi";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 12;
            this.listBox1.Location = new System.Drawing.Point(12, 418);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(342, 112);
            this.listBox1.TabIndex = 48;
            // 
            // frmToJPG
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(623, 538);
            this.Controls.Add(this.listBox1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.btStart);
            this.Controls.Add(this.btExit);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmToJPG";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "导出图片";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmToJPG_FormClosing);
            this.Load += new System.EventHandler(this.frmToJPG_Load);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Button btStart;
        private System.Windows.Forms.Button btExit;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ComboBox cbHalfOp;
        private System.Windows.Forms.CheckBox cbPageHalf;
        private System.Windows.Forms.CheckBox cbPageScope;
        private System.Windows.Forms.CheckedListBox chklbDocuments;
        private System.Windows.Forms.TextBox tbPageNum;
        private System.Windows.Forms.RadioButton rbPage;
        private System.Windows.Forms.RadioButton rbSelection;
        private System.Windows.Forms.RadioButton rbCurPage;
        private System.Windows.Forms.RadioButton rbDocument;
        private System.Windows.Forms.RadioButton rbCurDocument;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.ComboBox cbYSSF;
        private System.Windows.Forms.Button brReset;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox tbPath;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btBrw;
        private System.Windows.Forms.Button btAdvance;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cbCM;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox cbSmooth;
        private System.Windows.Forms.ComboBox cboFbl;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox checkBox_autoNum;
        private System.Windows.Forms.ComboBox comboBox_Unit;
        private System.Windows.Forms.CheckBox cbUnit;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.CheckBox cbUseColorProfile;
        private System.Windows.Forms.CheckBox cbIncludePageName;
        private System.Windows.Forms.CheckBox checkBox_TopMost;
        private System.Windows.Forms.ComboBox comboBox_Type;
        private System.Windows.Forms.CheckBox checkBox_trans;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox comboBox_SC;
        private System.Windows.Forms.CheckBox checkBox_OverPrintBlack;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.CheckBox checkBox_MaintainLayers;
        private System.Windows.Forms.RadioButton rbSelRge;
    }
}