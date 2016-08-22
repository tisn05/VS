using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.IO;
using System.Windows.Forms;
using System.Data;
using tisn.cdr;
using System.Collections.Generic;

/// <copyright>
/// Giora Tamir (giora@gtamir.com), 2005
/// </copyright>

namespace RiffParserDemo2
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class RiffParserDemo2 : System.Windows.Forms.Form
	{
		// Constants
		private readonly string FILEFILTER = "CDR files(*.cdr)|*.cdr|AVI files (*.avi)|*.avi|WAV files (*.wav)|*.wav|MIDI files (*.rmi)|*.rmi|All files (*.*)|*.*";
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
		private System.Windows.Forms.TextBox txtFilename;
		private System.Windows.Forms.Label lblFilename;
		private System.Windows.Forms.Button BtnOpen;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox txtFileFormat;
		private System.Windows.Forms.TextBox txtFileType;
		private System.Windows.Forms.Panel pnlAVI;
		private System.Windows.Forms.Button btnBrowse;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.Label lblError;
		private System.Windows.Forms.Label lbl1a;
		private System.Windows.Forms.Label lbl2a;
		private System.Windows.Forms.Label lbl3a;
		private System.Windows.Forms.Label lbl4a;
		private System.Windows.Forms.Label lbl5a;
		private System.Windows.Forms.Label lbl6a;
		private System.Windows.Forms.Label lbl1b;
		private System.Windows.Forms.Label lbl2b;
		private System.Windows.Forms.Label lbl3b;
		private System.Windows.Forms.Label lbl4b;
		private System.Windows.Forms.Label lbl5b;
		private System.Windows.Forms.Label lbl6b;

		// Parser
		RiffParser rp;
		private System.Windows.Forms.Label lblInfo;
		DecodeHeader dh;
        CDRReader cr;

		public RiffParserDemo2()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// Initialize parser instance
			rp = new RiffParser();
			dh = new DecodeHeader(rp);
            cr = new CDRReader(rp);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.txtFilename = new System.Windows.Forms.TextBox();
            this.lblFilename = new System.Windows.Forms.Label();
            this.BtnOpen = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtFileFormat = new System.Windows.Forms.TextBox();
            this.txtFileType = new System.Windows.Forms.TextBox();
            this.pnlAVI = new System.Windows.Forms.Panel();
            this.lbl6b = new System.Windows.Forms.Label();
            this.lbl5b = new System.Windows.Forms.Label();
            this.lbl4b = new System.Windows.Forms.Label();
            this.lbl3b = new System.Windows.Forms.Label();
            this.lbl2b = new System.Windows.Forms.Label();
            this.lbl1b = new System.Windows.Forms.Label();
            this.lbl6a = new System.Windows.Forms.Label();
            this.lbl5a = new System.Windows.Forms.Label();
            this.lbl4a = new System.Windows.Forms.Label();
            this.lbl3a = new System.Windows.Forms.Label();
            this.lbl2a = new System.Windows.Forms.Label();
            this.lbl1a = new System.Windows.Forms.Label();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.lblError = new System.Windows.Forms.Label();
            this.lblInfo = new System.Windows.Forms.Label();
            this.pnlAVI.SuspendLayout();
            this.SuspendLayout();
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // txtFilename
            // 
            this.txtFilename.Location = new System.Drawing.Point(154, 26);
            this.txtFilename.Name = "txtFilename";
            this.txtFilename.Size = new System.Drawing.Size(316, 21);
            this.txtFilename.TabIndex = 0;
            // 
            // lblFilename
            // 
            this.lblFilename.Location = new System.Drawing.Point(38, 26);
            this.lblFilename.Name = "lblFilename";
            this.lblFilename.Size = new System.Drawing.Size(106, 26);
            this.lblFilename.TabIndex = 1;
            this.lblFilename.Text = "File to Examine";
            this.lblFilename.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // BtnOpen
            // 
            this.BtnOpen.Location = new System.Drawing.Point(480, 17);
            this.BtnOpen.Name = "BtnOpen";
            this.BtnOpen.Size = new System.Drawing.Size(96, 35);
            this.BtnOpen.TabIndex = 1;
            this.BtnOpen.Text = "Open";
            this.BtnOpen.Click += new System.EventHandler(this.BtnOpen_Click);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(38, 60);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(106, 26);
            this.label1.TabIndex = 3;
            this.label1.Text = "File Format";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(38, 95);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(106, 26);
            this.label2.TabIndex = 4;
            this.label2.Text = "File Type";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // txtFileFormat
            // 
            this.txtFileFormat.Location = new System.Drawing.Point(154, 60);
            this.txtFileFormat.Name = "txtFileFormat";
            this.txtFileFormat.Size = new System.Drawing.Size(76, 21);
            this.txtFileFormat.TabIndex = 3;
            // 
            // txtFileType
            // 
            this.txtFileType.Location = new System.Drawing.Point(154, 95);
            this.txtFileType.Name = "txtFileType";
            this.txtFileType.Size = new System.Drawing.Size(76, 21);
            this.txtFileType.TabIndex = 4;
            // 
            // pnlAVI
            // 
            this.pnlAVI.Controls.Add(this.lbl6b);
            this.pnlAVI.Controls.Add(this.lbl5b);
            this.pnlAVI.Controls.Add(this.lbl4b);
            this.pnlAVI.Controls.Add(this.lbl3b);
            this.pnlAVI.Controls.Add(this.lbl2b);
            this.pnlAVI.Controls.Add(this.lbl1b);
            this.pnlAVI.Controls.Add(this.lbl6a);
            this.pnlAVI.Controls.Add(this.lbl5a);
            this.pnlAVI.Controls.Add(this.lbl4a);
            this.pnlAVI.Controls.Add(this.lbl3a);
            this.pnlAVI.Controls.Add(this.lbl2a);
            this.pnlAVI.Controls.Add(this.lbl1a);
            this.pnlAVI.Location = new System.Drawing.Point(10, 155);
            this.pnlAVI.Name = "pnlAVI";
            this.pnlAVI.Size = new System.Drawing.Size(672, 233);
            this.pnlAVI.TabIndex = 8;
            // 
            // lbl6b
            // 
            this.lbl6b.Location = new System.Drawing.Point(365, 190);
            this.lbl6b.Name = "lbl6b";
            this.lbl6b.Size = new System.Drawing.Size(288, 25);
            this.lbl6b.TabIndex = 11;
            // 
            // lbl5b
            // 
            this.lbl5b.Location = new System.Drawing.Point(365, 155);
            this.lbl5b.Name = "lbl5b";
            this.lbl5b.Size = new System.Drawing.Size(288, 26);
            this.lbl5b.TabIndex = 10;
            // 
            // lbl4b
            // 
            this.lbl4b.Location = new System.Drawing.Point(365, 121);
            this.lbl4b.Name = "lbl4b";
            this.lbl4b.Size = new System.Drawing.Size(288, 25);
            this.lbl4b.TabIndex = 9;
            // 
            // lbl3b
            // 
            this.lbl3b.Location = new System.Drawing.Point(365, 86);
            this.lbl3b.Name = "lbl3b";
            this.lbl3b.Size = new System.Drawing.Size(288, 26);
            this.lbl3b.TabIndex = 8;
            // 
            // lbl2b
            // 
            this.lbl2b.Location = new System.Drawing.Point(365, 52);
            this.lbl2b.Name = "lbl2b";
            this.lbl2b.Size = new System.Drawing.Size(288, 26);
            this.lbl2b.TabIndex = 7;
            // 
            // lbl1b
            // 
            this.lbl1b.Location = new System.Drawing.Point(365, 17);
            this.lbl1b.Name = "lbl1b";
            this.lbl1b.Size = new System.Drawing.Size(288, 26);
            this.lbl1b.TabIndex = 6;
            // 
            // lbl6a
            // 
            this.lbl6a.Location = new System.Drawing.Point(29, 190);
            this.lbl6a.Name = "lbl6a";
            this.lbl6a.Size = new System.Drawing.Size(288, 25);
            this.lbl6a.TabIndex = 5;
            // 
            // lbl5a
            // 
            this.lbl5a.Location = new System.Drawing.Point(29, 155);
            this.lbl5a.Name = "lbl5a";
            this.lbl5a.Size = new System.Drawing.Size(288, 26);
            this.lbl5a.TabIndex = 4;
            // 
            // lbl4a
            // 
            this.lbl4a.Location = new System.Drawing.Point(29, 121);
            this.lbl4a.Name = "lbl4a";
            this.lbl4a.Size = new System.Drawing.Size(288, 25);
            this.lbl4a.TabIndex = 3;
            // 
            // lbl3a
            // 
            this.lbl3a.Location = new System.Drawing.Point(29, 86);
            this.lbl3a.Name = "lbl3a";
            this.lbl3a.Size = new System.Drawing.Size(288, 26);
            this.lbl3a.TabIndex = 2;
            // 
            // lbl2a
            // 
            this.lbl2a.Location = new System.Drawing.Point(29, 52);
            this.lbl2a.Name = "lbl2a";
            this.lbl2a.Size = new System.Drawing.Size(288, 26);
            this.lbl2a.TabIndex = 1;
            // 
            // lbl1a
            // 
            this.lbl1a.Location = new System.Drawing.Point(29, 17);
            this.lbl1a.Name = "lbl1a";
            this.lbl1a.Size = new System.Drawing.Size(288, 26);
            this.lbl1a.TabIndex = 0;
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(586, 17);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(96, 35);
            this.btnBrowse.TabIndex = 2;
            this.btnBrowse.Text = "Browse...";
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // lblError
            // 
            this.lblError.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblError.ForeColor = System.Drawing.Color.Red;
            this.lblError.Location = new System.Drawing.Point(259, 60);
            this.lblError.Name = "lblError";
            this.lblError.Size = new System.Drawing.Size(423, 61);
            this.lblError.TabIndex = 10;
            this.lblError.Visible = false;
            // 
            // lblInfo
            // 
            this.lblInfo.Location = new System.Drawing.Point(38, 129);
            this.lblInfo.Name = "lblInfo";
            this.lblInfo.Size = new System.Drawing.Size(106, 26);
            this.lblInfo.TabIndex = 11;
            this.lblInfo.Text = "Info:";
            // 
            // RiffParserDemo2
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
            this.ClientSize = new System.Drawing.Size(704, 454);
            this.Controls.Add(this.lblInfo);
            this.Controls.Add(this.lblError);
            this.Controls.Add(this.btnBrowse);
            this.Controls.Add(this.pnlAVI);
            this.Controls.Add(this.txtFileType);
            this.Controls.Add(this.txtFileFormat);
            this.Controls.Add(this.txtFilename);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.BtnOpen);
            this.Controls.Add(this.lblFilename);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "RiffParserDemo2";
            this.Text = "RiffParser";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.pnlAVI.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion


		private void Clear()
		{
			pnlAVI.Visible = false;
			lblError.Visible = false;
			txtFileFormat.Text = String.Empty;
			txtFileType.Text = String.Empty;
			lbl1a.Text = String.Empty;
			lbl2a.Text = String.Empty;
			lbl3a.Text = String.Empty;
			lbl4a.Text = String.Empty;
			lbl5a.Text = String.Empty;
			lbl6a.Text = String.Empty;
			lbl1b.Text = String.Empty;
			lbl2b.Text = String.Empty;
			lbl3b.Text = String.Empty;
			lbl4b.Text = String.Empty;
			lbl5b.Text = String.Empty;
			lbl6b.Text = String.Empty;
		}

		private void CheckFile(string filename)
		{
			rp.OpenFile(filename);
			txtFileFormat.Text = RiffParser.FromFourCC(rp.FileRIFF);
			txtFileType.Text = RiffParser.FromFourCC(rp.FileType);
            if (txtFileType.Text.StartsWith("CDR"))
            {
            }
			// Is this a type we are familiar with?
			if (RiffParser.ckidAVI == rp.FileType)
			{
				dh.ProcessMainAVI();
				lbl1a.Text = dh.FrameSize;
				lbl2a.Text = dh.FrameRate;
				lbl3a.Text = dh.TotalFrames;
				lbl4a.Text = dh.TotalTime;
				lbl5a.Text = dh.NumStreams;

				lbl1b.Text = dh.ISFT;
				lbl2b.Text = dh.VideoHandler;
				lbl3b.Text = dh.VideoDataRate;
				lbl4b.Text = dh.AudioHandler;
				lbl5b.Text = dh.AudioDataRate;

				pnlAVI.Visible = true;
			}
			else if (RiffParser.ckidWAV == rp.FileType)
			{
				dh.ProcessMainWAVE();
				lbl1a.Text = dh.NumChannels;
				lbl2a.Text = dh.BitsPerSample;
				lbl3a.Text = dh.BitsPerSec;
				lbl4a.Text = dh.SamplesPerSec;

				pnlAVI.Visible = true;
			}
			else if (RiffParser.ckidRMID == rp.FileType)
			{
			}
			
			rp.CloseFile();
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
		
		}

		private void openFileDialog1_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
		{
			Clear();
			txtFilename.Text = openFileDialog1.FileName;
			BtnOpen_Click(sender, e);
		}

		private void btnBrowse_Click(object sender, System.EventArgs e)
		{
			Clear();
			openFileDialog1.DefaultExt = "cdr";
			openFileDialog1.Filter = FILEFILTER;
			openFileDialog1.Multiselect = false;
			openFileDialog1.ReadOnlyChecked = true;
			openFileDialog1.RestoreDirectory = true;
			openFileDialog1.Title = "Choose a RIFF file to examine";
			openFileDialog1.ShowDialog();
		}

		private void BtnOpen_Click(object sender, System.EventArgs e)
		{
			try 
			{
				Clear();
				CheckFile(txtFilename.Text);
			}
			catch (FileNotFoundException)
			{
				lblError.Text = "File not found!";
				lblError.Visible = true;
			}
			catch (Exception ex) 
			{
				lblError.Text = ex.Message;
				lblError.Visible = true;
                throw ex;
			}
		
		}
	}
}
