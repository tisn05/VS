namespace cdrPluginT
{
    partial class frmQrCode
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
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBox_ErrorCorrect = new System.Windows.Forms.ComboBox();
            this.button1 = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.label10 = new System.Windows.Forms.Label();
            this.textBox_cardReal = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textBox_cardAdress = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.textBox_cardCompany = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textBox_cardEMail = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox_cardQQ = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox_cardTel = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_cardHttp = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox_cardPosition = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_cardName = new System.Windows.Forms.TextBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.button2 = new System.Windows.Forms.Button();
            this.textBox_selfText = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button_toVector = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.comboBox_version = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(326, 172);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 21);
            this.textBox1.TabIndex = 7;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(222, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 8;
            this.label1.Text = "纠错级别";
            // 
            // comboBox_ErrorCorrect
            // 
            this.comboBox_ErrorCorrect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_ErrorCorrect.FormattingEnabled = true;
            this.comboBox_ErrorCorrect.Items.AddRange(new object[] {
            "L(7%)",
            "M(15%)",
            "Q(25%)",
            "H(30%)"});
            this.comboBox_ErrorCorrect.Location = new System.Drawing.Point(222, 27);
            this.comboBox_ErrorCorrect.Name = "comboBox_ErrorCorrect";
            this.comboBox_ErrorCorrect.Size = new System.Drawing.Size(121, 20);
            this.comboBox_ErrorCorrect.TabIndex = 9;
            this.comboBox_ErrorCorrect.SelectedIndexChanged += new System.EventHandler(this.comboBox_ErrorCorrect_SelectedIndexChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(408, 77);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 10;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new System.Drawing.Point(12, 229);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(648, 266);
            this.tabControl1.TabIndex = 11;
            this.tabControl1.SelectedIndexChanged += new System.EventHandler(this.tabControl1_SelectedIndexChanged);
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.label10);
            this.tabPage1.Controls.Add(this.textBox_cardReal);
            this.tabPage1.Controls.Add(this.label8);
            this.tabPage1.Controls.Add(this.textBox_cardAdress);
            this.tabPage1.Controls.Add(this.label9);
            this.tabPage1.Controls.Add(this.textBox_cardCompany);
            this.tabPage1.Controls.Add(this.label7);
            this.tabPage1.Controls.Add(this.textBox_cardEMail);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Controls.Add(this.textBox_cardQQ);
            this.tabPage1.Controls.Add(this.label6);
            this.tabPage1.Controls.Add(this.textBox_cardTel);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.textBox_cardHttp);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.textBox_cardPosition);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.textBox_cardName);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(640, 240);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "电子名片";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(11, 107);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(65, 12);
            this.label10.TabIndex = 17;
            this.label10.Text = "真实内容：";
            // 
            // textBox_cardReal
            // 
            this.textBox_cardReal.Location = new System.Drawing.Point(13, 122);
            this.textBox_cardReal.Multiline = true;
            this.textBox_cardReal.Name = "textBox_cardReal";
            this.textBox_cardReal.Size = new System.Drawing.Size(621, 112);
            this.textBox_cardReal.TabIndex = 16;
            this.textBox_cardReal.TextChanged += new System.EventHandler(this.textBox_cardReal_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(221, 80);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(41, 12);
            this.label8.TabIndex = 15;
            this.label8.Text = "地址：";
            // 
            // textBox_cardAdress
            // 
            this.textBox_cardAdress.Location = new System.Drawing.Point(268, 77);
            this.textBox_cardAdress.Name = "textBox_cardAdress";
            this.textBox_cardAdress.Size = new System.Drawing.Size(366, 21);
            this.textBox_cardAdress.TabIndex = 14;
            this.textBox_cardAdress.TextChanged += new System.EventHandler(this.textBox_cardAdress_TextChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(11, 80);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(41, 12);
            this.label9.TabIndex = 13;
            this.label9.Text = "单位：";
            // 
            // textBox_cardCompany
            // 
            this.textBox_cardCompany.Location = new System.Drawing.Point(58, 77);
            this.textBox_cardCompany.Name = "textBox_cardCompany";
            this.textBox_cardCompany.Size = new System.Drawing.Size(157, 21);
            this.textBox_cardCompany.TabIndex = 12;
            this.textBox_cardCompany.TextChanged += new System.EventHandler(this.textBox_cardCompany_TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(317, 53);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(41, 12);
            this.label7.TabIndex = 11;
            this.label7.Text = "邮箱：";
            // 
            // textBox_cardEMail
            // 
            this.textBox_cardEMail.Location = new System.Drawing.Point(364, 50);
            this.textBox_cardEMail.Name = "textBox_cardEMail";
            this.textBox_cardEMail.Size = new System.Drawing.Size(270, 21);
            this.textBox_cardEMail.TabIndex = 10;
            this.textBox_cardEMail.TextChanged += new System.EventHandler(this.textBox_cardEMail_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(164, 53);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 12);
            this.label5.TabIndex = 9;
            this.label5.Text = "QQ：";
            // 
            // textBox_cardQQ
            // 
            this.textBox_cardQQ.Location = new System.Drawing.Point(211, 50);
            this.textBox_cardQQ.Name = "textBox_cardQQ";
            this.textBox_cardQQ.Size = new System.Drawing.Size(100, 21);
            this.textBox_cardQQ.TabIndex = 8;
            this.textBox_cardQQ.TextChanged += new System.EventHandler(this.textBox_cardQQ_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(11, 53);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(41, 12);
            this.label6.TabIndex = 7;
            this.label6.Text = "电话：";
            // 
            // textBox_cardTel
            // 
            this.textBox_cardTel.Location = new System.Drawing.Point(58, 50);
            this.textBox_cardTel.Name = "textBox_cardTel";
            this.textBox_cardTel.Size = new System.Drawing.Size(100, 21);
            this.textBox_cardTel.TabIndex = 6;
            this.textBox_cardTel.TextChanged += new System.EventHandler(this.textBox_cardTel_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(317, 26);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 5;
            this.label4.Text = "网址：";
            // 
            // textBox_cardHttp
            // 
            this.textBox_cardHttp.Location = new System.Drawing.Point(364, 23);
            this.textBox_cardHttp.Name = "textBox_cardHttp";
            this.textBox_cardHttp.Size = new System.Drawing.Size(270, 21);
            this.textBox_cardHttp.TabIndex = 4;
            this.textBox_cardHttp.TextChanged += new System.EventHandler(this.textBox_cardHttp_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(164, 26);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "职务：";
            // 
            // textBox_cardPosition
            // 
            this.textBox_cardPosition.Location = new System.Drawing.Point(211, 23);
            this.textBox_cardPosition.Name = "textBox_cardPosition";
            this.textBox_cardPosition.Size = new System.Drawing.Size(100, 21);
            this.textBox_cardPosition.TabIndex = 2;
            this.textBox_cardPosition.TextChanged += new System.EventHandler(this.textBox_cardPosition_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 26);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "姓名：";
            // 
            // textBox_cardName
            // 
            this.textBox_cardName.Location = new System.Drawing.Point(58, 23);
            this.textBox_cardName.Name = "textBox_cardName";
            this.textBox_cardName.Size = new System.Drawing.Size(100, 21);
            this.textBox_cardName.TabIndex = 0;
            this.textBox_cardName.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(640, 240);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "WIFI";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.button2);
            this.tabPage3.Controls.Add(this.textBox_selfText);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(640, 240);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "自助文本";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(24, 20);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(95, 45);
            this.button2.TabIndex = 1;
            this.button2.Text = "截图识别文本";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // textBox_selfText
            // 
            this.textBox_selfText.Location = new System.Drawing.Point(24, 71);
            this.textBox_selfText.Multiline = true;
            this.textBox_selfText.Name = "textBox_selfText";
            this.textBox_selfText.Size = new System.Drawing.Size(592, 150);
            this.textBox_selfText.TabIndex = 0;
            this.textBox_selfText.TextChanged += new System.EventHandler(this.textBox_selfText_TextChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button_toVector);
            this.groupBox1.Location = new System.Drawing.Point(460, 123);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 100);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "操作";
            // 
            // button_toVector
            // 
            this.button_toVector.Location = new System.Drawing.Point(62, 71);
            this.button_toVector.Name = "button_toVector";
            this.button_toVector.Size = new System.Drawing.Size(75, 23);
            this.button_toVector.TabIndex = 0;
            this.button_toVector.Text = "生成矢量";
            this.button_toVector.UseVisualStyleBackColor = true;
            this.button_toVector.Click += new System.EventHandler(this.button_toVector_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(12, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(204, 205);
            this.pictureBox1.TabIndex = 15;
            this.pictureBox1.TabStop = false;
            // 
            // comboBox_version
            // 
            this.comboBox_version.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_version.FormattingEnabled = true;
            this.comboBox_version.Items.AddRange(new object[] {
            "自动",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "13",
            "14",
            "15",
            "16",
            "17",
            "18",
            "19",
            "20",
            "21",
            "22",
            "23",
            "24",
            "25",
            "26",
            "27",
            "28",
            "29",
            "30",
            "31",
            "32",
            "33",
            "34",
            "35",
            "36",
            "37",
            "38",
            "39",
            "40"});
            this.comboBox_version.Location = new System.Drawing.Point(222, 67);
            this.comboBox_version.Name = "comboBox_version";
            this.comboBox_version.Size = new System.Drawing.Size(121, 20);
            this.comboBox_version.TabIndex = 16;
            this.comboBox_version.SelectedIndexChanged += new System.EventHandler(this.comboBox_version_SelectedIndexChanged);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(222, 52);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(29, 12);
            this.label11.TabIndex = 17;
            this.label11.Text = "版本";
            // 
            // frmQrCode
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(686, 507);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.comboBox_version);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.comboBox_ErrorCorrect);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox1);
            this.Name = "frmQrCode";
            this.Text = "二维码-By Tisn";
            this.Load += new System.EventHandler(this.frmQrCode_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBox_ErrorCorrect;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox textBox_selfText;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button_toVector;
        private System.Windows.Forms.TextBox textBox_cardName;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBox_cardReal;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBox_cardAdress;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBox_cardCompany;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBox_cardEMail;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox_cardQQ;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox_cardTel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_cardHttp;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox_cardPosition;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.ComboBox comboBox_version;
        private System.Windows.Forms.Label label11;
    }
}