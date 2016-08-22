namespace 饥荒秘籍输入器
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.button1 = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.comboBox_材料类 = new System.Windows.Forms.ComboBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.comboBox_工具武器 = new System.Windows.Forms.ComboBox();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.comboBox_穿戴 = new System.Windows.Forms.ComboBox();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.comboBox_建筑 = new System.Windows.Forms.ComboBox();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.comboBox_食物 = new System.Windows.Forms.ComboBox();
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.comboBox_植物 = new System.Windows.Forms.ComboBox();
            this.tabPage7 = new System.Windows.Forms.TabPage();
            this.comboBox_动物 = new System.Windows.Forms.ComboBox();
            this.button_获得物品 = new System.Windows.Forms.Button();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_数量 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.tabPage5.SuspendLayout();
            this.tabPage6.SuspendLayout();
            this.tabPage7.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(391, 25);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(58, 29);
            this.button1.TabIndex = 0;
            this.button1.Text = "输入";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 30);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(373, 21);
            this.textBox1.TabIndex = 1;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(12, 57);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(53, 44);
            this.button2.TabIndex = 2;
            this.button2.Text = "全物品制造";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(71, 57);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(53, 44);
            this.button3.TabIndex = 3;
            this.button3.Text = "饥饿值不降低";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(130, 57);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(53, 44);
            this.button4.TabIndex = 4;
            this.button4.Text = "上帝\r\n模式";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(189, 57);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(53, 44);
            this.button5.TabIndex = 5;
            this.button5.Text = "地图\r\n全开";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 6;
            this.label1.Text = "自定义代码";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.textBox_数量);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.radioButton2);
            this.groupBox1.Controls.Add(this.radioButton1);
            this.groupBox1.Controls.Add(this.button_获得物品);
            this.groupBox1.Controls.Add(this.tabControl1);
            this.groupBox1.Location = new System.Drawing.Point(12, 107);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(384, 186);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "获得物品";
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Controls.Add(this.tabPage5);
            this.tabControl1.Controls.Add(this.tabPage6);
            this.tabControl1.Controls.Add(this.tabPage7);
            this.tabControl1.Location = new System.Drawing.Point(9, 20);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(367, 100);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.comboBox_材料类);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(359, 74);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "材料类";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // comboBox_材料类
            // 
            this.comboBox_材料类.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_材料类.FormattingEnabled = true;
            this.comboBox_材料类.Location = new System.Drawing.Point(18, 24);
            this.comboBox_材料类.Name = "comboBox_材料类";
            this.comboBox_材料类.Size = new System.Drawing.Size(319, 20);
            this.comboBox_材料类.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.comboBox_工具武器);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(359, 74);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "工具武器";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // comboBox_工具武器
            // 
            this.comboBox_工具武器.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_工具武器.FormattingEnabled = true;
            this.comboBox_工具武器.Location = new System.Drawing.Point(18, 24);
            this.comboBox_工具武器.Name = "comboBox_工具武器";
            this.comboBox_工具武器.Size = new System.Drawing.Size(319, 20);
            this.comboBox_工具武器.TabIndex = 1;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.comboBox_穿戴);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(359, 74);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "穿戴";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // comboBox_穿戴
            // 
            this.comboBox_穿戴.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_穿戴.FormattingEnabled = true;
            this.comboBox_穿戴.Location = new System.Drawing.Point(18, 24);
            this.comboBox_穿戴.Name = "comboBox_穿戴";
            this.comboBox_穿戴.Size = new System.Drawing.Size(319, 20);
            this.comboBox_穿戴.TabIndex = 2;
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.comboBox_建筑);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Size = new System.Drawing.Size(359, 74);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "建筑";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // comboBox_建筑
            // 
            this.comboBox_建筑.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_建筑.FormattingEnabled = true;
            this.comboBox_建筑.Location = new System.Drawing.Point(18, 24);
            this.comboBox_建筑.Name = "comboBox_建筑";
            this.comboBox_建筑.Size = new System.Drawing.Size(319, 20);
            this.comboBox_建筑.TabIndex = 2;
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.comboBox_食物);
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Size = new System.Drawing.Size(359, 74);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "食物";
            this.tabPage5.UseVisualStyleBackColor = true;
            // 
            // comboBox_食物
            // 
            this.comboBox_食物.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_食物.FormattingEnabled = true;
            this.comboBox_食物.Location = new System.Drawing.Point(18, 24);
            this.comboBox_食物.Name = "comboBox_食物";
            this.comboBox_食物.Size = new System.Drawing.Size(319, 20);
            this.comboBox_食物.TabIndex = 2;
            // 
            // tabPage6
            // 
            this.tabPage6.Controls.Add(this.comboBox_植物);
            this.tabPage6.Location = new System.Drawing.Point(4, 22);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Size = new System.Drawing.Size(359, 74);
            this.tabPage6.TabIndex = 5;
            this.tabPage6.Text = "植物";
            this.tabPage6.UseVisualStyleBackColor = true;
            // 
            // comboBox_植物
            // 
            this.comboBox_植物.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_植物.FormattingEnabled = true;
            this.comboBox_植物.Location = new System.Drawing.Point(18, 24);
            this.comboBox_植物.Name = "comboBox_植物";
            this.comboBox_植物.Size = new System.Drawing.Size(319, 20);
            this.comboBox_植物.TabIndex = 2;
            // 
            // tabPage7
            // 
            this.tabPage7.Controls.Add(this.comboBox_动物);
            this.tabPage7.Location = new System.Drawing.Point(4, 22);
            this.tabPage7.Name = "tabPage7";
            this.tabPage7.Size = new System.Drawing.Size(359, 74);
            this.tabPage7.TabIndex = 6;
            this.tabPage7.Text = "动物";
            this.tabPage7.UseVisualStyleBackColor = true;
            // 
            // comboBox_动物
            // 
            this.comboBox_动物.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_动物.FormattingEnabled = true;
            this.comboBox_动物.Location = new System.Drawing.Point(18, 24);
            this.comboBox_动物.Name = "comboBox_动物";
            this.comboBox_动物.Size = new System.Drawing.Size(319, 20);
            this.comboBox_动物.TabIndex = 2;
            // 
            // button_获得物品
            // 
            this.button_获得物品.Location = new System.Drawing.Point(169, 130);
            this.button_获得物品.Name = "button_获得物品";
            this.button_获得物品.Size = new System.Drawing.Size(113, 34);
            this.button_获得物品.TabIndex = 1;
            this.button_获得物品.Text = "获取";
            this.button_获得物品.UseVisualStyleBackColor = true;
            this.button_获得物品.Click += new System.EventHandler(this.button_获得物品_Click);
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Checked = true;
            this.radioButton1.Location = new System.Drawing.Point(13, 126);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(71, 16);
            this.radioButton1.TabIndex = 2;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "放入背包";
            this.radioButton1.UseVisualStyleBackColor = true;
            this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(13, 148);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(83, 16);
            this.radioButton2.TabIndex = 3;
            this.radioButton2.Text = "放在鼠标处";
            this.radioButton2.UseVisualStyleBackColor = true;
            this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.Red;
            this.label2.Location = new System.Drawing.Point(288, 141);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "快捷键:Ctrl+F1";
            // 
            // textBox_数量
            // 
            this.textBox_数量.Location = new System.Drawing.Point(125, 124);
            this.textBox_数量.Name = "textBox_数量";
            this.textBox_数量.Size = new System.Drawing.Size(34, 21);
            this.textBox_数量.TabIndex = 5;
            this.textBox_数量.Text = "1";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(90, 128);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "数量";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(453, 412);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.tabPage5.ResumeLayout(false);
            this.tabPage6.ResumeLayout(false);
            this.tabPage7.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.ComboBox comboBox_材料类;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.TabPage tabPage5;
        private System.Windows.Forms.TabPage tabPage6;
        private System.Windows.Forms.TabPage tabPage7;
        private System.Windows.Forms.ComboBox comboBox_工具武器;
        private System.Windows.Forms.ComboBox comboBox_穿戴;
        private System.Windows.Forms.ComboBox comboBox_建筑;
        private System.Windows.Forms.ComboBox comboBox_食物;
        private System.Windows.Forms.ComboBox comboBox_植物;
        private System.Windows.Forms.ComboBox comboBox_动物;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.Button button_获得物品;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox_数量;
    }
}

