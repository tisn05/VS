namespace cdrPluginT
{
    partial class frmColorReplace
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
            this.listView2 = new System.Windows.Forms.ListView();
            this.label3 = new System.Windows.Forms.Label();
            this.cbR = new System.Windows.Forms.ComboBox();
            this.cb2 = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.rCurShape = new System.Windows.Forms.Button();
            this.rEyedropper = new System.Windows.Forms.Button();
            this.cmReplace = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.listView1 = new System.Windows.Forms.ListView();
            this.lblFind = new System.Windows.Forms.Label();
            this.cbF = new System.Windows.Forms.ComboBox();
            this.cb1 = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.fCurShape = new System.Windows.Forms.Button();
            this.cEyedropper = new System.Windows.Forms.Button();
            this.cmFind = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button1 = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // listView2
            // 
            this.listView2.Location = new System.Drawing.Point(66, 337);
            this.listView2.Name = "listView2";
            this.listView2.Size = new System.Drawing.Size(248, 156);
            this.listView2.TabIndex = 17;
            this.listView2.UseCompatibleStateImageBehavior = false;
            this.listView2.View = System.Windows.Forms.View.SmallIcon;
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(64, 257);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(250, 12);
            this.label3.TabIndex = 16;
            this.label3.Text = "黑(#0000FF)";
            // 
            // cbR
            // 
            this.cbR.FormattingEnabled = true;
            this.cbR.Location = new System.Drawing.Point(66, 311);
            this.cbR.Name = "cbR";
            this.cbR.Size = new System.Drawing.Size(248, 20);
            this.cbR.TabIndex = 15;
            // 
            // cb2
            // 
            this.cb2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb2.FormattingEnabled = true;
            this.cb2.Location = new System.Drawing.Point(193, 279);
            this.cb2.Name = "cb2";
            this.cb2.Size = new System.Drawing.Size(121, 20);
            this.cb2.TabIndex = 14;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(146, 282);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 13;
            this.label4.Text = "调色版";
            // 
            // rCurShape
            // 
            this.rCurShape.Location = new System.Drawing.Point(106, 272);
            this.rCurShape.Name = "rCurShape";
            this.rCurShape.Size = new System.Drawing.Size(34, 33);
            this.rCurShape.TabIndex = 12;
            this.rCurShape.Text = "<";
            this.rCurShape.UseVisualStyleBackColor = true;
            // 
            // rEyedropper
            // 
            this.rEyedropper.Location = new System.Drawing.Point(66, 277);
            this.rEyedropper.Name = "rEyedropper";
            this.rEyedropper.Size = new System.Drawing.Size(34, 23);
            this.rEyedropper.TabIndex = 11;
            this.rEyedropper.Text = "button2";
            this.rEyedropper.UseVisualStyleBackColor = true;
            // 
            // cmReplace
            // 
            this.cmReplace.BackColor = System.Drawing.Color.Red;
            this.cmReplace.Location = new System.Drawing.Point(14, 272);
            this.cmReplace.Name = "cmReplace";
            this.cmReplace.Size = new System.Drawing.Size(46, 33);
            this.cmReplace.TabIndex = 10;
            this.cmReplace.UseVisualStyleBackColor = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 257);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 12);
            this.label5.TabIndex = 9;
            this.label5.Text = "查找";
            // 
            // listView1
            // 
            this.listView1.Location = new System.Drawing.Point(66, 89);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(248, 156);
            this.listView1.TabIndex = 8;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.SmallIcon;
            // 
            // lblFind
            // 
            this.lblFind.Location = new System.Drawing.Point(64, 9);
            this.lblFind.Name = "lblFind";
            this.lblFind.Size = new System.Drawing.Size(250, 12);
            this.lblFind.TabIndex = 7;
            this.lblFind.Text = "黑(#0000FF)";
            // 
            // cbF
            // 
            this.cbF.FormattingEnabled = true;
            this.cbF.Location = new System.Drawing.Point(66, 63);
            this.cbF.Name = "cbF";
            this.cbF.Size = new System.Drawing.Size(248, 20);
            this.cbF.TabIndex = 6;
            // 
            // cb1
            // 
            this.cb1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb1.FormattingEnabled = true;
            this.cb1.Items.AddRange(new object[] {
            "选中对象",
            "文档调色板"});
            this.cb1.Location = new System.Drawing.Point(193, 31);
            this.cb1.Name = "cb1";
            this.cb1.Size = new System.Drawing.Size(121, 20);
            this.cb1.TabIndex = 5;
            this.cb1.SelectedIndexChanged += new System.EventHandler(this.cb1_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(146, 34);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "调色版";
            // 
            // fCurShape
            // 
            this.fCurShape.Location = new System.Drawing.Point(106, 29);
            this.fCurShape.Name = "fCurShape";
            this.fCurShape.Size = new System.Drawing.Size(18, 23);
            this.fCurShape.TabIndex = 3;
            this.fCurShape.Text = "<";
            this.fCurShape.UseVisualStyleBackColor = true;
            // 
            // cEyedropper
            // 
            this.cEyedropper.Location = new System.Drawing.Point(66, 29);
            this.cEyedropper.Name = "cEyedropper";
            this.cEyedropper.Size = new System.Drawing.Size(34, 23);
            this.cEyedropper.TabIndex = 2;
            this.cEyedropper.Text = "button2";
            this.cEyedropper.UseVisualStyleBackColor = true;
            this.cEyedropper.Click += new System.EventHandler(this.bt_Eyedropper_Click);
            // 
            // cmFind
            // 
            this.cmFind.BackColor = System.Drawing.Color.Red;
            this.cmFind.Location = new System.Drawing.Point(14, 24);
            this.cmFind.Name = "cmFind";
            this.cmFind.Size = new System.Drawing.Size(46, 33);
            this.cmFind.TabIndex = 1;
            this.cmFind.UseVisualStyleBackColor = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "查找";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.button1);
            this.panel1.Location = new System.Drawing.Point(320, 89);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(396, 427);
            this.panel1.TabIndex = 19;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(14, 30);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // frmColorReplace
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(728, 515);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.listView2);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cbR);
            this.Controls.Add(this.cb2);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.rCurShape);
            this.Controls.Add(this.rEyedropper);
            this.Controls.Add(this.cmReplace);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.lblFind);
            this.Controls.Add(this.cbF);
            this.Controls.Add(this.cb1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.fCurShape);
            this.Controls.Add(this.cEyedropper);
            this.Controls.Add(this.cmFind);
            this.Controls.Add(this.label1);
            this.Name = "frmColorReplace";
            this.Text = "颜色替换";
            this.Load += new System.EventHandler(this.frmColorReplace_Load);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button cmFind;
        private System.Windows.Forms.Button cEyedropper;
        private System.Windows.Forms.Button fCurShape;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cb1;
        private System.Windows.Forms.ComboBox cbF;
        private System.Windows.Forms.Label lblFind;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ListView listView2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cbR;
        private System.Windows.Forms.ComboBox cb2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button rCurShape;
        private System.Windows.Forms.Button rEyedropper;
        private System.Windows.Forms.Button cmReplace;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button button1;
    }
}