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

 MainFrm.cpp : Implementation of the CMainFrame class.

*********************************************************************/

#include "stdafx.h"
#include "activeview.h"
#include "mainfrm.h"
#include <afxole.h>
#include <stdio.h>
#include "iac.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_QUERYENDSESSION()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
	    ID_SEPARATOR,
    ID_VIEW_FIRSTPAGE,
    ID_VIEW_PREVPAGE,
    ID_VIEW_NEXTPAGE,
    ID_VIEW_LASTPAGE,
		ID_SEPARATOR,
	ID_TOOLS_HAND,
	ID_EDIT_SELECTTEXT,
	ID_EDIT_SELECTGRAPHIC,
};


static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	LastView = NULL;

	CWinApp* pWinApp = AfxGetApp();
/*
** Returns CWinApp for currently running application
*/
	if (pWinApp->m_pMessageFilter != NULL) {
		pWinApp->m_pMessageFilter->EnableBusyDialog(FALSE);
/*
** EnableBusyDialog - Specifies whether the "busy" dialog is enabled or disabled.
*/
		pWinApp->m_pMessageFilter->SetRetryReply(10000);
/*
** SetRetryReply - Determines the calling application's action when it
** receives a busy response from a called application.
** Number of milliseconds between retries.
*/
		pWinApp->m_pMessageFilter->SetMessagePendingDelay(20000);
/*
** SetMessagePendingDelay - Determines how long the calling application waits
** for a response from the called application before taking further action.
** Number of milliseconds for the message-pending delay.
*/
	}

	MainAcroApp = new CAcroApp;
	COleException e;
	if (!MainAcroApp->CreateDispatch("AcroExch.App",&e))
		AfxMessageBox("Failed to create object :<");

	/* Set the preference for the viewer to one column display */
	BOOL b = MainAcroApp->SetPreference(avpPageViewLayoutMode, PDLayoutOneColumn);

}

CMainFrame::~CMainFrame()
{
	MainAcroApp->Unlock();
	MainAcroApp->Exit();
	delete MainAcroApp;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame public functions



/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnQueryEndSession()
{
	if (!CMDIFrameWnd::OnQueryEndSession())
		return FALSE;

	if (!MDIGetActive(NULL))
		return TRUE;
	else {
		AfxMessageBox("Some open docs - attempting to close them.");
		SendMessageToDescendants(WM_SYSCOMMAND,SC_CLOSE,NULL,TRUE,FALSE);
		return TRUE;
	}
}

