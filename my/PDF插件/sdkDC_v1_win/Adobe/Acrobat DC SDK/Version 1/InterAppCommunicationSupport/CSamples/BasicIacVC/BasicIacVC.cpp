/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 BasicIacVc.cpp

 - This was a machine generated file, and the code to use Acrobat IAC 
 has been added to it after the generation.
**********************************************************************/

/** BasicIacVc: A simple sample provides the minimum code to use Acrobat IAC in a VC application.

 - The project was created in the following steps:
	.Create a new project in VC:
	 File->New->Projects->PFC wizard (exe)->dialog based project.
	.Connect type library
	 View->ClassWizard->add class->from type library->(choose acrobat.tlb from SDK)
	 then file Acrobat.h and Acrobat.cpp were generated in the project.
	 An alternative way is to use file Acrobat.h and Acrobat.cpp delivered in SDK.
	.Build the project.
	.Add the Acrobat code to this file, as marked. 
 - This IAC sample first creates an Acrobat IAC PDDoc object, then open a 
 sample file "C:\simple.pdf". If it's opened successfully, gets the number of pages
 and show a message. Otherwise, shows an error message. 
 - The project only provides the basic code needed to make Acrobat IAC work. The user
 can further add code to make it as a working program. 

  @see CreateDispatch
  @see PDDoc->Open
  @see PDDoc->GetNumPages
  
*/

// BasicIacVc.cpp : Defines the class behaviors for the application.

// headers
#include "stdafx.h"
#include "BasicIacVc.h"
#include "Acrobat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// a sample PDF to open
const char* SamplePdfPath = "C:\\simple.pdf";

// global variable
CAcroPDDoc *gAcroPdDoc = NULL;

/////////////////////////////////////////////////////////////////////////////
// CBasicIacVcApp

BEGIN_MESSAGE_MAP(CBasicIacVcApp, CWinApp)
	//{{AFX_MSG_MAP(CBasicIacVcApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicIacVcApp construction

CBasicIacVcApp::CBasicIacVcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBasicIacVcApp object

CBasicIacVcApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBasicIacVcApp initialization

BOOL CBasicIacVcApp::InitInstance()
{
	// === Code added by Acrobat Developer... 
	
	// === Initialize OLE libraries. 
	if (!AfxOleInit())	{
		AfxMessageBox("OLE initialization failed in BasicIacVc sample.");
		return FALSE;
	}

	// === Create an Acrobat IAC PDDoc object  
	COleException e;
	gAcroPdDoc = new CAcroPDDoc;
	gAcroPdDoc->CreateDispatch("AcroExch.PDDoc", &e);
	if(!gAcroPdDoc) {
		AfxMessageBox("Creating Acrobat IAC object failed in BasicIacVc sample.");
		return FALSE;
	}
	
	// === Open a pdf file.  
	if(!gAcroPdDoc->Open(SamplePdfPath)) {
		CString message = "Opening the PDF file \"";
		message += SamplePdfPath;
		message += "\" failed in the BasicIacVc sample.";
		AfxMessageBox(message);
		return FALSE;
	}

	// === Get the number of pages, and compose a massage.  
	char str[256];
	sprintf(str,"PDF document %s is loaded in Acrobat through IAC program.\n",SamplePdfPath);
	long n = gAcroPdDoc->GetNumPages();
	if(n!=-1) 
		sprintf(str, "%sThe PDF file has %d pages.", str, n);
	else
		sprintf(str, "Error in getting page number.");
	
	// message
	AfxEnableControlContainer();
	AfxMessageBox(str);

	// === Code to finish program.
	gAcroPdDoc->Close();
	delete gAcroPdDoc;
	gAcroPdDoc = NULL;

	// === end of Initialize OLE and Acrobat code.

	// Since the program has finished, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
