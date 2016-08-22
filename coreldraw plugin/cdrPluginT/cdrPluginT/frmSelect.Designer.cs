namespace cdrPluginT
{
    partial class frmSelect
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBox_SFontSize = new System.Windows.Forms.CheckBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.checkBox_SLines = new System.Windows.Forms.CheckBox();
            this.checkBox_SFont = new System.Windows.Forms.CheckBox();
            this.checkBox_SType = new System.Windows.Forms.CheckBox();
            this.checkBox_SColor = new System.Windows.Forms.CheckBox();
            this.checkBox_SSize = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.checkBox_SFontSize);
            this.groupBox1.Controls.Add(this.checkBox_SLines);
            this.groupBox1.Controls.Add(this.checkBox_SFont);
            this.groupBox1.Controls.Add(this.checkBox_SType);
            this.groupBox1.Controls.Add(this.checkBox_SColor);
            this.groupBox1.Controls.Add(this.checkBox_SSize);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(189, 97);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            // 
            // checkBox_SFontSize
            // 
            this.checkBox_SFontSize.AutoSize = true;
            this.checkBox_SFontSize.Checked = global::cdrPluginT.Properties.Settings.Default.同类选择_相同字号;
            this.checkBox_SFontSize.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::cdrPluginT.Properties.Settings.Default, "同类选择_相同字号", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox_SFontSize.Location = new System.Drawing.Point(105, 64);
            this.checkBox_SFontSize.Name = "checkBox_SFontSize";
            this.checkBox_SFontSize.Size = new System.Drawing.Size(72, 16);
            this.checkBox_SFontSize.TabIndex = 5;
            this.checkBox_SFontSize.Text = "相同字号";
            this.checkBox_SFontSize.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 115);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 2;
            this.button1.Text = "同类选择";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label3
            // 
            this.label3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label3.Location = new System.Drawing.Point(102, 26);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(2, 70);
            this.label3.TabIndex = 3;
            this.label3.Text = "label3";
            // 
            // checkBox_SLines
            // 
            this.checkBox_SLines.AutoSize = true;
            this.checkBox_SLines.Checked = global::cdrPluginT.Properties.Settings.Default.同类选择_相同线粗;
            this.checkBox_SLines.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::cdrPluginT.Properties.Settings.Default, "同类选择_相同线粗", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox_SLines.Location = new System.Drawing.Point(6, 64);
            this.checkBox_SLines.Name = "checkBox_SLines";
            this.checkBox_SLines.Size = new System.Drawing.Size(72, 16);
            this.checkBox_SLines.TabIndex = 4;
            this.checkBox_SLines.Text = "相同线粗";
            this.checkBox_SLines.UseVisualStyleBackColor = true;
            // 
            // checkBox_SFont
            // 
            this.checkBox_SFont.AutoSize = true;
            this.checkBox_SFont.Checked = global::cdrPluginT.Properties.Settings.Default.同类选择_相同字体;
            this.checkBox_SFont.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::cdrPluginT.Properties.Settings.Default, "同类选择_相同字体", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox_SFont.Location = new System.Drawing.Point(105, 42);
            this.checkBox_SFont.Name = "checkBox_SFont";
            this.checkBox_SFont.Size = new System.Drawing.Size(72, 16);
            this.checkBox_SFont.TabIndex = 3;
            this.checkBox_SFont.Text = "相同字体";
            this.checkBox_SFont.UseVisualStyleBackColor = true;
            // 
            // checkBox_SType
            // 
            this.checkBox_SType.AutoSize = true;
            this.checkBox_SType.Checked = global::cdrPluginT.Properties.Settings.Default.同类选择_相同类型;
            this.checkBox_SType.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::cdrPluginT.Properties.Settings.Default, "同类选择_相同类型", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox_SType.Location = new System.Drawing.Point(6, 42);
            this.checkBox_SType.Name = "checkBox_SType";
            this.checkBox_SType.Size = new System.Drawing.Size(72, 16);
            this.checkBox_SType.TabIndex = 2;
            this.checkBox_SType.Text = "相同类型";
            this.checkBox_SType.UseVisualStyleBackColor = true;
            // 
            // checkBox_SColor
            // 
            this.checkBox_SColor.AutoSize = true;
            this.checkBox_SColor.Checked = global::cdrPluginT.Properties.Settings.Default.同类选择_相同颜色;
            this.checkBox_SColor.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::cdrPluginT.Properties.Settings.Default, "同类选择_相同颜色", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox_SColor.Location = new System.Drawing.Point(105, 20);
            this.checkBox_SColor.Name = "checkBox_SColor";
            this.checkBox_SColor.Size = new System.Drawing.Size(72, 16);
            this.checkBox_SColor.TabIndex = 1;
            this.checkBox_SColor.Text = "相同颜色";
            this.checkBox_SColor.UseVisualStyleBackColor = true;
            // 
            // checkBox_SSize
            // 
            this.checkBox_SSize.AutoSize = true;
            this.checkBox_SSize.Checked = global::cdrPluginT.Properties.Settings.Default.同类选择_相同尺寸;
            this.checkBox_SSize.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_SSize.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::cdrPluginT.Properties.Settings.Default, "同类选择_相同尺寸", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox_SSize.Location = new System.Drawing.Point(6, 20);
            this.checkBox_SSize.Name = "checkBox_SSize";
            this.checkBox_SSize.Size = new System.Drawing.Size(72, 16);
            this.checkBox_SSize.TabIndex = 0;
            this.checkBox_SSize.Text = "相同尺寸";
            this.checkBox_SSize.UseVisualStyleBackColor = true;
            // 
            // frmSelect
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(216, 148);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmSelect";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "同类选择";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.frmSelect_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.CheckBox checkBox_SSize;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBox_SFontSize;
        private System.Windows.Forms.CheckBox checkBox_SLines;
        private System.Windows.Forms.CheckBox checkBox_SFont;
        private System.Windows.Forms.CheckBox checkBox_SType;
        private System.Windows.Forms.CheckBox checkBox_SColor;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label3;
    }
}