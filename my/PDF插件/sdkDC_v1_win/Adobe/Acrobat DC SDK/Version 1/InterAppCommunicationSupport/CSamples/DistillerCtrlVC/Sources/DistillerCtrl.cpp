/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 DistillerCtrl.cpp : Defines the class behaviors for the application.

*********************************************************************/

#include "stdafx.h"
#include "DistillerCtrl.h"
#include "DistillerCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDistillerCtrlApp

BEGIN_MESSAGE_MAP(CDistillerCtrlApp, CWinApp)
	//{{AFX_MSG_MAP(CDistillerCtrlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDistillerCtrlApp construction

CDistillerCtrlApp::CDistillerCtrlApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDistillerCtrlApp object

CDistillerCtrlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDistillerCtrlApp initialization

BOOL CDistillerCtrlApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Initialize the COM libraries
	HRESULT hRes = CoInitialize(NULL);
	if (!SUCCEEDED(hRes))
		return FALSE;

	CDistillerCtrlDlg *dlg = new CDistillerCtrlDlg();
	m_pMainWnd = dlg;
	dlg->DoModal();
	delete dlg;

	// Uninitialize the COM libraries
	CoUninitialize();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
