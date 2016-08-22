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

 StaticViewVw.cpp : Implementation of the CStaticViewVw class.

*********************************************************************/

#include "stdafx.h"
#include "staticview.h"

#include "staticviewdoc.h"
#include "staticviewvw.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticViewVw

IMPLEMENT_DYNCREATE(CStaticViewVw, CScrollView)

BEGIN_MESSAGE_MAP(CStaticViewVw, CScrollView)
	//{{AFX_MSG_MAP(CStaticViewVw)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticViewVw construction/destruction

CStaticViewVw::CStaticViewVw()
{
	// Unhide extra toolbar items (Page # box, Zoom Button, Page # label)
	((CMainFrame *)AfxGetApp()->m_pMainWnd)->ShowToolbarAddOns(TRUE);

	// Increment # of views counter stored by the main frame
	((CMainFrame *)AfxGetApp()->m_pMainWnd)->IncNumViews();
}

CStaticViewVw::~CStaticViewVw()
{

	// Decrement # of views counter stored by the main frame
	((CMainFrame *)AfxGetApp()->m_pMainWnd)->DecNumViews();

    // If there are no more views, hide extra toolbar items
	if (((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetNumViews() == 0) {
		((CMainFrame *)AfxGetApp()->m_pMainWnd)->ShowToolbarAddOns(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CStaticViewVw Drawing

void CStaticViewVw::OnDraw(CDC* pDC)
{
	CStaticViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// Call document Draw command with origin at the window's scroll pos
	pDoc->Draw(pDC, m_hWnd, GetScrollPosition());
}

/////////////////////////////////////////////////////////////////////////////
// CStaticViewVw printing

BOOL CStaticViewVw::OnPreparePrinting(CPrintInfo* pInfo)
{
	// TODO: implement printing
	AfxMessageBox ("This functionality has not been implemented.");
	return FALSE;
}

void CStaticViewVw::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CStaticViewVw::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CStaticViewVw diagnostics

#ifdef _DEBUG
void CStaticViewVw::AssertValid() const
{
	CScrollView::AssertValid();
}

void CStaticViewVw::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CStaticViewDoc* CStaticViewVw::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStaticViewDoc)));
	return (CStaticViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStaticViewVw message handlers


// called whenever view is focused on
void CStaticViewVw::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// Setup last view variable in main frame to point to this view
	((CMainFrame *)AfxGetApp()->m_pMainWnd)->SetLastView(this);

	// Call Document to setup toolbar status boxes for this view
	GetDocument()->SetToolbarStatusBoxes();
}


// called when view first attatched to document
void CStaticViewVw::OnInitialUpdate()
{
	// setup document size for scroll bars
	RECT ViewRect;
	GetClientRect((LPRECT) &ViewRect);
	SetScrollSizes(MM_TEXT, GetDocument()->GetScaledDocSize((ViewRect.right-ViewRect.left),(ViewRect.bottom-ViewRect.top)));
}


// called when window is resized
void CStaticViewVw::OnSize(UINT nType, int cx, int cy)
{
	SetScrollSizes(MM_TEXT, GetDocument()->GetScaledDocSize(cx,cy));
    GetDocument()->SetToolbarStatusBoxes();
	CScrollView::OnSize(nType, cx, cy);
	GetDocument()->UpdateAllViews(NULL, (LPARAM) TRUE, NULL);
}


// called when view has been modified
void CStaticViewVw::OnUpdate(CView *, LPARAM UpdateBars, CObject *)
{
	// if necessary to update scroll bars:
	if ((BOOL) UpdateBars) {
		RECT ViewRect;
		GetClientRect((LPRECT) &ViewRect);
		SetScrollSizes(MM_TEXT, GetDocument()->GetScaledDocSize((ViewRect.right-ViewRect.left),(ViewRect.bottom-ViewRect.top)));
	}
	CScrollView::OnUpdate(NULL,(LPARAM) 0, NULL);
}

