/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 BasicIacOCXCS 
 
 This is a simple C# sample demonstrating how to use PDF ActiveX control
 in a C# application. To do that, you need first add it to your project's 
 references by selecting "Adobe Acrobat 7.0 Browswer Control Type Library 1.0" 
 from "Project - Add References... - COM". In order to be able to drag-drop 
 the ActiveX control to a window in your application, you need add the Acrobat 
 ActiveX control to the "Toolbox" by selecting "Adobe Acrobat 7.0 Browser 
 Control" from "Tools - Add/Remove Toolbox Items... - COM".

*********************************************************************/

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Data;
using AcroPDFLib;
using System.Text;

namespace BasicIacOCXCS
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class BasicIacOCXCS : System.Windows.Forms.Form
	{
        private System.Windows.Forms.Panel panelLeft;
		private System.Windows.Forms.Button goButtonLeft;
		private System.Windows.Forms.Button browseButtonLeft;
		private System.Windows.Forms.TextBox addressLeft;
		private System.Windows.Forms.Label AddressLabelLeft;
		private AxAcroPDFLib.AxAcroPDF pdfWindowLeft;
		private System.Windows.Forms.Panel panelRight;
		private System.Windows.Forms.Button goButtonRight;
		private System.Windows.Forms.Button browseButtonRight;
		private System.Windows.Forms.TextBox addressRight;
		private System.Windows.Forms.Label addressLabelRight;
		private AxAcroPDFLib.AxAcroPDF pdfWindowRight;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern uint GetShortPathName(
           [MarshalAs(UnmanagedType.LPTStr)] string lpszLongPath,
           [MarshalAs(UnmanagedType.LPTStr)] StringBuilder lpszShortPath,
           uint cchBuffer);

        public BasicIacOCXCS()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// Initialize address
			addressLeft.Text = "http://www.adobe.com/epaper/pdfs/elib_swedish_libraries.pdf";
			addressRight.Text = "http://www.adobe.com/misc/pdfs/chap4_pg1thru7.pdf";
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(BasicIacOCXCS));
			this.panelLeft = new System.Windows.Forms.Panel();
			this.goButtonLeft = new System.Windows.Forms.Button();
			this.browseButtonLeft = new System.Windows.Forms.Button();
			this.addressLeft = new System.Windows.Forms.TextBox();
			this.AddressLabelLeft = new System.Windows.Forms.Label();
			this.pdfWindowLeft = new AxAcroPDFLib.AxAcroPDF();
			this.panelRight = new System.Windows.Forms.Panel();
			this.goButtonRight = new System.Windows.Forms.Button();
			this.browseButtonRight = new System.Windows.Forms.Button();
			this.addressRight = new System.Windows.Forms.TextBox();
			this.addressLabelRight = new System.Windows.Forms.Label();
			this.pdfWindowRight = new AxAcroPDFLib.AxAcroPDF();
			this.panelLeft.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.pdfWindowLeft)).BeginInit();
			this.panelRight.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.pdfWindowRight)).BeginInit();
			this.SuspendLayout();
			// 
			// panelLeft
			// 
			this.panelLeft.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panelLeft.Controls.Add(this.goButtonLeft);
			this.panelLeft.Controls.Add(this.browseButtonLeft);
			this.panelLeft.Controls.Add(this.addressLeft);
			this.panelLeft.Controls.Add(this.AddressLabelLeft);
			this.panelLeft.Controls.Add(this.pdfWindowLeft);
			this.panelLeft.Location = new System.Drawing.Point(8, 8);
			this.panelLeft.Name = "panelLeft";
			this.panelLeft.Size = new System.Drawing.Size(416, 616);
			this.panelLeft.TabIndex = 0;
			// 
			// goButtonLeft
			// 
			this.goButtonLeft.Location = new System.Drawing.Point(264, 40);
			this.goButtonLeft.Name = "goButtonLeft";
			this.goButtonLeft.Size = new System.Drawing.Size(112, 24);
			this.goButtonLeft.TabIndex = 4;
			this.goButtonLeft.Text = "Go";
			this.goButtonLeft.Click += new System.EventHandler(this.goButtonLeft_Click);
			// 
			// browseButtonLeft
			// 
			this.browseButtonLeft.Location = new System.Drawing.Point(112, 40);
			this.browseButtonLeft.Name = "browseButtonLeft";
			this.browseButtonLeft.Size = new System.Drawing.Size(112, 24);
			this.browseButtonLeft.TabIndex = 3;
			this.browseButtonLeft.Text = "Browse";
			this.browseButtonLeft.Click += new System.EventHandler(this.browseButtonLeft_Click);
			// 
			// addressLeft
			// 
			this.addressLeft.Location = new System.Drawing.Point(56, 8);
			this.addressLeft.Name = "addressLeft";
			this.addressLeft.Size = new System.Drawing.Size(344, 20);
			this.addressLeft.TabIndex = 2;
			this.addressLeft.Text = "";
			// 
			// AddressLabelLeft
			// 
			this.AddressLabelLeft.Location = new System.Drawing.Point(8, 8);
			this.AddressLabelLeft.Name = "AddressLabelLeft";
			this.AddressLabelLeft.Size = new System.Drawing.Size(48, 23);
			this.AddressLabelLeft.TabIndex = 1;
			this.AddressLabelLeft.Text = "Address ";
			// 
			// pdfWindowLeft
			// 
			this.pdfWindowLeft.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.pdfWindowLeft.ContainingControl = this;
			this.pdfWindowLeft.Enabled = true;
			this.pdfWindowLeft.Location = new System.Drawing.Point(8, 72);
			this.pdfWindowLeft.Name = "pdfWindowLeft";
			this.pdfWindowLeft.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("pdfWindowLeft.OcxState")));
			this.pdfWindowLeft.Size = new System.Drawing.Size(392, 528);
			this.pdfWindowLeft.TabIndex = 0;
			// 
			// panelRight
			// 
			this.panelRight.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panelRight.Controls.Add(this.goButtonRight);
			this.panelRight.Controls.Add(this.browseButtonRight);
			this.panelRight.Controls.Add(this.addressRight);
			this.panelRight.Controls.Add(this.addressLabelRight);
			this.panelRight.Controls.Add(this.pdfWindowRight);
			this.panelRight.Location = new System.Drawing.Point(440, 8);
			this.panelRight.Name = "panelRight";
			this.panelRight.Size = new System.Drawing.Size(440, 616);
			this.panelRight.TabIndex = 1;
			// 
			// goButtonRight
			// 
			this.goButtonRight.Location = new System.Drawing.Point(272, 40);
			this.goButtonRight.Name = "goButtonRight";
			this.goButtonRight.Size = new System.Drawing.Size(120, 24);
			this.goButtonRight.TabIndex = 4;
			this.goButtonRight.Text = "Go";
			this.goButtonRight.Click += new System.EventHandler(this.goButtonRight_Click);
			// 
			// browseButtonRight
			// 
			this.browseButtonRight.Location = new System.Drawing.Point(112, 40);
			this.browseButtonRight.Name = "browseButtonRight";
			this.browseButtonRight.Size = new System.Drawing.Size(128, 24);
			this.browseButtonRight.TabIndex = 3;
			this.browseButtonRight.Text = "Browse";
			this.browseButtonRight.Click += new System.EventHandler(this.browseButtonRight_Click);
			// 
			// addressRight
			// 
			this.addressRight.Location = new System.Drawing.Point(56, 8);
			this.addressRight.Name = "addressRight";
			this.addressRight.Size = new System.Drawing.Size(376, 20);
			this.addressRight.TabIndex = 2;
			this.addressRight.Text = "";
			// 
			// addressLabelRight
			// 
			this.addressLabelRight.Location = new System.Drawing.Point(8, 8);
			this.addressLabelRight.Name = "addressLabelRight";
			this.addressLabelRight.Size = new System.Drawing.Size(48, 24);
			this.addressLabelRight.TabIndex = 1;
			this.addressLabelRight.Text = "Address ";
			// 
			// pdfWindowRight
			// 
			this.pdfWindowRight.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.pdfWindowRight.ContainingControl = this;
			this.pdfWindowRight.Enabled = true;
			this.pdfWindowRight.Location = new System.Drawing.Point(8, 72);
			this.pdfWindowRight.Name = "pdfWindowRight";
			this.pdfWindowRight.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("pdfWindowRight.OcxState")));
			this.pdfWindowRight.Size = new System.Drawing.Size(416, 528);
			this.pdfWindowRight.TabIndex = 0;
			// 
			// BasicIacOCXCS
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(888, 630);
			this.Controls.Add(this.panelRight);
			this.Controls.Add(this.panelLeft);
			this.Name = "BasicIacOCXCS";
			this.Text = "Basic IAC OCX C# Example";
			this.panelLeft.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.pdfWindowLeft)).EndInit();
			this.panelRight.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.pdfWindowRight)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new BasicIacOCXCS());
		}

		private void goButtonLeft_Click(object sender, System.EventArgs e)
		{
			if(0 == addressLeft.Text.Length)
				MessageBox.Show (this, "Please put a valid address in the left \"Address\" field!", "Need An Address",
					MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
            if (addressLeft.Text.StartsWith("http://"))
                pdfWindowLeft.src = addressLeft.Text;
            else {
                StringBuilder shortFileName = new StringBuilder(4096);
                uint shortFileNameSize = (uint)shortFileName.Capacity;
                if (GetShortPathName(addressLeft.Text, shortFileName, shortFileNameSize) != 0)
                    pdfWindowLeft.LoadFile(shortFileName.ToString());
                else
                    MessageBox.Show(this, "No such file.", "Need an existing file",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
            }
		}

		private void browseButtonLeft_Click(object sender, System.EventArgs e)
		{
			// Show the open file dialog
			OpenFileDialog openFile = new OpenFileDialog ();
			DialogResult result = openFile.ShowDialog();

			// Do nothing if the user hit the "Cancel" button of the open file dialog
			if(result  == DialogResult.Cancel)
				return;

			// Get the file name
			string strFileName = openFile.FileName;
			
			// Only accept a pdf file
			string strFileNameDup = strFileName;
			if(strFileNameDup.ToUpper().EndsWith(".PDF"))
			{
				addressLeft.Text = strFileName;
			}
			else
				MessageBox.Show(this, "Please select a pdf file!", "Wrong File Type", MessageBoxButtons.OK,
					MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1); 
		}

		private void goButtonRight_Click(object sender, System.EventArgs e)
		{
			if(0 == addressRight.Text.Length)
				MessageBox.Show (this, "Please put a valid address in the right \"Address\" field!", "Need An Address",
					MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
			if(addressRight.Text.StartsWith ("http://"))
				pdfWindowRight.src = addressRight.Text;
			else {
                StringBuilder shortFileName = new StringBuilder( 4096 );
                uint shortFileNameSize = (uint)shortFileName.Capacity;
                if (GetShortPathName(addressRight.Text, shortFileName, shortFileNameSize) != 0)
                    pdfWindowRight.LoadFile(shortFileName.ToString());
                else
                    MessageBox.Show(this, "No such file.", "Need an existing file",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
            }
		}

		private void browseButtonRight_Click(object sender, System.EventArgs e)
		{
			// Show the open file dialog
			OpenFileDialog openFile = new OpenFileDialog ();
			DialogResult result = openFile.ShowDialog();

			// Do nothing if the user hit the "Cancel" button of the open file dialog
			if(result  == DialogResult.Cancel)
				return;

			// Get the file name
			string strFileName = openFile.FileName;
			
			// Only accept a pdf file
			string strFileNameDup = strFileName;
			if(strFileNameDup.ToUpper().EndsWith(".PDF"))
			{
				addressRight.Text = strFileName;
			}
			else
				MessageBox.Show(this, "Please select a pdf file!", "Wrong File Type", MessageBoxButtons.OK,
					MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1); 
		}
	}
}
