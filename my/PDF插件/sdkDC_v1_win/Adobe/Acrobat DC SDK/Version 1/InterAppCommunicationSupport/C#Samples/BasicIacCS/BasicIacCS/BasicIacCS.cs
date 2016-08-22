/*ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.
------------------------------------------------------------

BasicIacCS
- This is a simple Acrobat IAC C# sample. It includes the code
to launch Acrobat Viewer, open a PDF file ( IAC\TestFiles\TwoColumnTaggedDoc.pdf ), and get
simple information ( number of pages ). The purpose of the sample is
to provide a minimum code for C# IAC developers to get started.
It should be easy to add more code to improve and extend the capability.
- The way to set up using Acrobat IAC in the project is from the menu
 Project -> Add Rerences... -> COM to select Acrobat.
- To run the sample, Acrobat Viewer should not be launched, or it is launched but
not have any file is loaded. If Acrobat Viewer has launched with open files,
Acrobat is locked, and some IAC methods in the code won't work.
'------------------------------------------------------------*/
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Acrobat;

namespace BasicIacCS
{
	/// <summary>
	/// Summary description for BasicIac.
	/// </summary>
	public class BasicIac : System.Windows.Forms.Form
	{
		public System.Windows.Forms.Label label1;
		public System.Windows.Forms.Label label2;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		//Acrobat application as a private member variable of the class
		private CAcroApp mApp;

		public BasicIac()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
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
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.Font = new System.Drawing.Font("Arial", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label1.Location = new System.Drawing.Point(104, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(273, 33);
			this.label1.TabIndex = 0;
			this.label1.Text = "Sample : BasicIacCS";
			// 
			// label2
			// 
			this.label2.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label2.ForeColor = System.Drawing.Color.ForestGreen;
			this.label2.Location = new System.Drawing.Point(16, 48);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(441, 137);
			this.label2.TabIndex = 1;
			this.label2.Text = "Program is over.";
			// 
			// BasicIac
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(476, 202);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.Location = new System.Drawing.Point(4, 23);
			this.Name = "BasicIac";
			this.Text = "BasicIacC#";
			this.Load += new System.EventHandler(this.BasicIac_Load);
			this.Closed += new System.EventHandler(this.BasicIac_Closed);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new BasicIac());
		}

		private void StartAcrobatIac()
		{
			//IAC objects
			CAcroPDDoc pdDoc;
			CAcroAVDoc avDoc;

			//constant, hard coding for a pdf to open, it can be changed when needed.
            String szPdfPathConst = Application.StartupPath + "\\..\\..\\..\\..\\..\\TestFiles\\TwoColumnTaggedDoc.pdf";

			//variables
			string szStr;
			string szName;
			int iNum = 0;

			//Initialize Acrobat by creating App object
			mApp = new AcroAppClass();

			//Show Acrobat
			mApp.Show();

			//set AVDoc object
			avDoc = new AcroAVDocClass();

			//open the PDF
			if (avDoc.Open (szPdfPathConst, ""))
			{
				//set the pdDoc object and get some data
				pdDoc  = (CAcroPDDoc)avDoc.GetPDDoc ();
				iNum = pdDoc.GetNumPages ();
				szName = pdDoc.GetFileName();

				//compose a message
                szStr = szName + " has been loaded in Acrobat by the IAC application.\n\n";

                if(iNum >1)
					szStr += "The PDF document has " + iNum + " pages.\n";
                else
                    szStr += "The PDF document has " + iNum + " page.\n";

				label2.Text = szStr;
			} 
			else 
			{
				label2.Text = "Cannot open " + szPdfPathConst + "\n";
			}
		}

		private void BasicIac_Load(object sender, System.EventArgs e)
		{
			StartAcrobatIac();
		}

		private void BasicIac_Closed(object sender, System.EventArgs e)
		{
			if(mApp != null)
			{
				mApp.CloseAllDocs ();
				mApp.Exit ();
			}
		}
	}
}
