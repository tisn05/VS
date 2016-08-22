/*ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.
------------------------------------------------------------

FillFormCS
- This is a simple Acrobat IAC C# sample(C# Console project). It 
  includes code to launch Acrobat Viewer, open a PDF file 
  (IAC\TestFiles\SampleForm.pdf), fill the form field "Name" with value 
  "John Doe". The sample leaves the document "SampleForm.pdf" open 
  after the application exits on purpose. To keep the pdf document 
  open after the application exits, you should utilize the CAcroAVDoc 
  class rather than the CAcroPDDoc class.
- The way to set up using Acrobat IAC in the project is from the menu
  Project -> Add References... -> COM to select Acrobat.
- The way to set up using Form IAC in the project is from the menu
  Project -> Add References... -> COM to select "AFormAut 1.0 Type
  Library".
------------------------------------------------------------*/
using System;
using System.Collections;
using System.Windows.Forms;
using System.ComponentModel;
using Acrobat;
using AFORMAUTLib;

namespace FillFormCS
{
	/// <summary>
	/// Summary description for FillFormCS.
	/// </summary>
	class FillFormCS
	{
		// Hard-coded file name, it can be changed when needed.
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
            String FORM_NAME = Application.StartupPath + "\\..\\..\\..\\..\\..\\TestFiles\\SampleForm.pdf";
			// Initialize Acrobat by cretaing App object.
			CAcroApp acroApp = new AcroAppClass();
			
			// Show Acrobat Viewer
			acroApp.Show();

			// Create an AVDoc object
			CAcroAVDoc avDoc = new AcroAVDocClass();

			// Open the pdf
			if(!avDoc.Open (FORM_NAME, "")) 
			{
				string szMsg = "Cannot open" + FORM_NAME + ".\n";
				Console.WriteLine(szMsg);
				return;
			}

			// Create a IAFormApp object, so that we can access the form fields in
			// the open document
			IAFormApp formApp = new AFormAppClass();

			// Get the IFields object associated with the form
			IFields myFields = (IFields)formApp.Fields;

			// Get the IEnumerator object for myFields
			IEnumerator myEnumerator = myFields.GetEnumerator();

			bool bFound = false;

			// Fill the "Name" field with value "John Doe"
			while(myEnumerator.MoveNext())
			{
				// Get the IField object
				IField myField = (IField)myEnumerator.Current;

				// If the field is "Name", set it's value to "John Doe"
				if(myField.Name == "Name")
				{
					bFound = true;
					myField.Value = "John Doe";
					break;
				}
			}

			if(bFound)
				Console.WriteLine("Sucessfully changed the \"Name\" field value to \"John Doe\".");
			else
				Console.WriteLine("Failed to locate the \"Name\" field in the form.");
		}
	}
}
