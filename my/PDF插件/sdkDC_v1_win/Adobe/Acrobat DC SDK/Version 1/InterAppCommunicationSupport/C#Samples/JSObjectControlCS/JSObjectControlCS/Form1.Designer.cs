namespace JSObjectControlCS
{
    partial class JSObjectControlForm
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
            this.browseButton = new System.Windows.Forms.Button();
            this.filenameTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.addWatermarkButton = new System.Windows.Forms.Button();
            this.processFormDataButton = new System.Windows.Forms.Button();
            this.searchButton = new System.Windows.Forms.Button();
            this.searchTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tasksPanel = new System.Windows.Forms.Panel();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.searchPanel = new System.Windows.Forms.Panel();
            this.currentStatus = new System.Windows.Forms.Label();
            this.tasksPanel.SuspendLayout();
            this.searchPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // browseButton
            // 
            this.browseButton.Location = new System.Drawing.Point(235, 25);
            this.browseButton.Name = "browseButton";
            this.browseButton.Size = new System.Drawing.Size(75, 23);
            this.browseButton.TabIndex = 0;
            this.browseButton.Text = "Browse...";
            this.browseButton.UseVisualStyleBackColor = true;
            this.browseButton.Click += new System.EventHandler(this.browseButton_Click);
            // 
            // filenameTextBox
            // 
            this.filenameTextBox.AllowDrop = true;
            this.filenameTextBox.Location = new System.Drawing.Point(10, 25);
            this.filenameTextBox.Name = "filenameTextBox";
            this.filenameTextBox.Size = new System.Drawing.Size(215, 20);
            this.filenameTextBox.TabIndex = 1;
            this.filenameTextBox.Text = "Choose File...";
            this.filenameTextBox.LostFocus += new System.EventHandler(this.filenameTextBox_LostFocus);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Open File";
            // 
            // addWatermarkButton
            // 
            this.addWatermarkButton.Location = new System.Drawing.Point(5, 23);
            this.addWatermarkButton.Name = "addWatermarkButton";
            this.addWatermarkButton.Size = new System.Drawing.Size(123, 23);
            this.addWatermarkButton.TabIndex = 3;
            this.addWatermarkButton.Text = "Choose Watermark...";
            this.addWatermarkButton.UseVisualStyleBackColor = true;
            this.addWatermarkButton.Click += new System.EventHandler(this.addWatermarkButton_Click);
            // 
            // processFormDataButton
            // 
            this.processFormDataButton.Location = new System.Drawing.Point(5, 73);
            this.processFormDataButton.Name = "processFormDataButton";
            this.processFormDataButton.Size = new System.Drawing.Size(123, 23);
            this.processFormDataButton.TabIndex = 4;
            this.processFormDataButton.Text = "Choose Data Set...";
            this.processFormDataButton.UseVisualStyleBackColor = true;
            this.processFormDataButton.Click += new System.EventHandler(this.processFormDataButton_Click);
            // 
            // searchButton
            // 
            this.searchButton.Enabled = false;
            this.searchButton.Location = new System.Drawing.Point(80, 53);
            this.searchButton.Name = "searchButton";
            this.searchButton.Size = new System.Drawing.Size(75, 23);
            this.searchButton.TabIndex = 5;
            this.searchButton.Text = "Search";
            this.searchButton.UseVisualStyleBackColor = true;
            this.searchButton.Click += new System.EventHandler(this.searchButton_Click);
            // 
            // searchTextBox
            // 
            this.searchTextBox.Location = new System.Drawing.Point(10, 23);
            this.searchTextBox.Name = "searchTextBox";
            this.searchTextBox.Size = new System.Drawing.Size(145, 20);
            this.searchTextBox.TabIndex = 6;
            this.searchTextBox.TextChanged += new System.EventHandler(this.searchTextBox_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(5, 5);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(62, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Search For:";
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Location = new System.Drawing.Point(12, 58);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(302, 2);
            this.panel1.TabIndex = 8;
            // 
            // panel3
            // 
            this.panel3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel3.Location = new System.Drawing.Point(146, 67);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(2, 109);
            this.panel3.TabIndex = 10;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(5, 5);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(81, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "Add Watermark";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 53);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(97, 13);
            this.label4.TabIndex = 12;
            this.label4.Text = "Process Form Data";
            // 
            // tasksPanel
            // 
            this.tasksPanel.Controls.Add(this.processFormDataButton);
            this.tasksPanel.Controls.Add(this.addWatermarkButton);
            this.tasksPanel.Controls.Add(this.label4);
            this.tasksPanel.Controls.Add(this.label3);
            this.tasksPanel.Enabled = false;
            this.tasksPanel.Location = new System.Drawing.Point(10, 62);
            this.tasksPanel.Name = "tasksPanel";
            this.tasksPanel.Size = new System.Drawing.Size(134, 110);
            this.tasksPanel.TabIndex = 13;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(0, 0);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 0;
            // 
            // searchPanel
            // 
            this.searchPanel.Controls.Add(this.label2);
            this.searchPanel.Controls.Add(this.searchTextBox);
            this.searchPanel.Controls.Add(this.searchButton);
            this.searchPanel.Enabled = false;
            this.searchPanel.Location = new System.Drawing.Point(155, 62);
            this.searchPanel.Name = "searchPanel";
            this.searchPanel.Size = new System.Drawing.Size(159, 110);
            this.searchPanel.TabIndex = 14;
            // 
            // currentStatus
            // 
            this.currentStatus.Location = new System.Drawing.Point(10, 179);
            this.currentStatus.Name = "currentStatus";
            this.currentStatus.Size = new System.Drawing.Size(300, 13);
            this.currentStatus.TabIndex = 15;
            this.currentStatus.Text = "No Document Open";
            // 
            // JSObjectControlForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(322, 198);
            this.Controls.Add(this.currentStatus);
            this.Controls.Add(this.searchPanel);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.filenameTextBox);
            this.Controls.Add(this.browseButton);
            this.Controls.Add(this.tasksPanel);
            this.Name = "JSObjectControlForm";
            this.Text = "JSObject Controller";
            this.tasksPanel.ResumeLayout(false);
            this.tasksPanel.PerformLayout();
            this.searchPanel.ResumeLayout(false);
            this.searchPanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button browseButton;
        private System.Windows.Forms.TextBox filenameTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button addWatermarkButton;
        private System.Windows.Forms.Button processFormDataButton;
        private System.Windows.Forms.Button searchButton;
        private System.Windows.Forms.TextBox searchTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Panel tasksPanel;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Panel searchPanel;
        private System.Windows.Forms.Label currentStatus;
    }
}

