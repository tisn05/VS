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
#include "staticview.h"
#include "mainfrm.h"
#include "resource.h"

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
	ID_VIEW_LASTPAGE
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
	NumViews = 0;
	LastView = NULL;

	MainAcroApp = new CAcroApp;
	COleException e;
	if (!MainAcroApp->CreateDispatch("AcroExch.App",&e))
		AfxMessageBox("Failed to create object :<");
}

CMainFrame::~CMainFrame()
{
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

    // Create label next to page # box
	CRect rect;
	rect.top = 3;
    rect.left = 220;
	rect.right = rect.left + 50;
	rect.bottom = rect.top + 23;

	if (!m_wndStaticLabel.Create("Page: ",WS_CHILD,
		rect, &m_wndToolBar, IDC_EDITSTATIC))
	{
		TRACE("Failed to create edit box label inside toolbar\n");
		return -1;      // fail to create
	}

    // Create Page # box
	rect.top = 1;   // 1 pixel down from top of statusbar
    rect.left = 260;
	rect.right = rect.left + 50;
	rect.bottom = rect.top + 21;

	if (!m_wndEditBox.Create(
		WS_CHILD|WS_BORDER|ES_AUTOHSCROLL,
		rect, &m_wndToolBar, IDC_EDITBOX))
	{
		TRACE("Failed to create edit box inside toolbar\n");
		return -1;      // fail to create
	}

  // Create Zoom % button
	rect.top = 1;   // 1 pixel down from top of statusbar
    rect.left = 320;
	rect.right = rect.left + 50;
	rect.bottom = rect.top + 21;

	if (!m_wndZoomButton.Create("",
		WS_CHILD|BS_PUSHBUTTON,
		rect, &m_wndToolBar, IDC_ZOOMBUTTON))
	{
		TRACE("Failed to create edit box inside toolbar\n");
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

void CMainFrame::ShowToolbarAddOns(BOOL ShowOrHide)
{
	m_wndEditBox.ShowWindow(ShowOrHide);
    m_wndStaticLabel.ShowWindow(ShowOrHide);
    m_wndZoomButton.ShowWindow(ShowOrHide);
}


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

