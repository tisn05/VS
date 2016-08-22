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

 StaticViewDoc.cpp : Implementation of the CStaticViewDoc class.

*********************************************************************/

#include <string.h>

#include "stdafx.h"
#include "staticview.h"

#include "staticviewdoc.h"
#include "mainfrm.h"
#include "zoomset.h"
#include "iac.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// #defines for View Mode
#define VIEWMODE_ACTUALSIZE	0
#define	VIEWMODE_FITPAGE 1
#define	VIEWMODE_FITWIDTH 2
#define VIEWMODE_NONAUTO 3

#define	NOT_YET_SET				0xABCD

/////////////////////////////////////////////////////////////////////////////
// CStaticViewDoc

IMPLEMENT_DYNCREATE(CStaticViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CStaticViewDoc, CDocument)
	//{{AFX_MSG_MAP(CStaticViewDoc)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FIRSTPAGE, OnUpdateViewFirstpage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LASTPAGE, OnUpdateViewLastpage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEXTPAGE, OnUpdateViewNextpage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREVPAGE, OnUpdateViewPrevpage)
	ON_COMMAND(ID_VIEW_FIRSTPAGE, OnViewFirstpage)
	ON_COMMAND(ID_VIEW_LASTPAGE, OnViewLastpage)
	ON_COMMAND(ID_VIEW_NEXTPAGE, OnViewNextpage)
	ON_COMMAND(ID_VIEW_PREVPAGE, OnViewPrevpage)
	ON_COMMAND(ID_VIEW_ACTUALSIZE, OnViewActualsize)
	ON_COMMAND(ID_VIEW_FITPAGE, OnViewFitpage)
	ON_COMMAND(ID_VIEW_FITWIDTH, OnViewFitwidth)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ACTUALSIZE, OnUpdateViewActualsize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FITPAGE, OnUpdateViewFitpage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FITWIDTH, OnUpdateViewFitwidth)
	ON_COMMAND(IDC_ZOOMBUTTON, OnZoomButton)
	ON_COMMAND(ID_OTHER_COPYTOCLIPBOARD, OnOtherCopytoclipboard)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CStaticViewDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CStaticViewDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticViewDoc construction/destruction

CStaticViewDoc::CStaticViewDoc()
{
	EnableAutomation();

    // initialize pointers to acrobat objects to NULL
 	m_pAcroApp = NULL;
	m_pAcroPDPage = NULL;
	m_pAcroPDDoc = NULL;
	m_pAcroAVDoc = NULL;

    // initialize viewmode to FITPAGE
	ViewMode = VIEWMODE_FITPAGE;

    // initialize scale to undetermined
	nScale = (short)NOT_YET_SET;

	AfxOleLockApp();
}

CStaticViewDoc::~CStaticViewDoc()
{
    // delete acrobat objects
	if (m_pAcroPDPage) delete m_pAcroPDPage;
	if (m_pAcroPDDoc) delete m_pAcroPDDoc;
	if (m_pAcroAVDoc) {
		m_pAcroAVDoc->Close(1);
		delete m_pAcroAVDoc;
	}
	if (m_pAcroApp) delete m_pAcroApp;

	AfxOleUnlockApp();
}



// Called when a document is opened
BOOL CStaticViewDoc::OnOpenDocument(const char* pszPathName)
{
	COleException e;

	// Create new CAcroPDDoc
	m_pAcroPDDoc = new CAcroPDDoc;
	if (!m_pAcroPDDoc->CreateDispatch("AcroExch.PDDoc",&e)) {
		AfxMessageBox("Failed to create PDDoc :<");
		return(FALSE);
	}

	// Open Specified .PDF file
	BOOL ok = m_pAcroPDDoc->Open(pszPathName);
	if (!ok) {
		AfxMessageBox("Error Opening File :<");
        return(FALSE);
	}

	// Get the first page (page 0)
	m_pAcroPDPage = new CAcroPDPage;
	LPDISPATCH TempDispatch = m_pAcroPDDoc->AcquirePage(0);
	if (TempDispatch == NULL) {
		AfxMessageBox("Failed to create PDPage :<");
		return(FALSE);
	}
	m_pAcroPDPage->AttachDispatch(TempDispatch);

	// Set Current page to 0
	CurPage = 0;

	// Get # of pages in file
	NumPages = m_pAcroPDDoc->GetNumPages();

	return(TRUE);
}


// Called by framework to save file
BOOL CStaticViewDoc::OnSaveDocument(const char* pszPathName)
{
	// Pass filename through to Acrobat's save function
	return(m_pAcroPDDoc->Save(1,pszPathName));
}


// calculates and sets new nScale according to view mode
// returns scale of Y value (needed to properly calculate y length
//    in GetScaledDocSize() when in FIT WIDTH mode.
void CStaticViewDoc::CalculateScale(int width, int height)
{
	// save values of width and height
	OldWidth = width;
	OldHeight = height;

	// continue only if m_pAcroPDPage has been initialized
	if (m_pAcroPDPage) {
		long xScale, yScale;

    	// Get Size of page (uses temporary CAcroPoint object)
		CAcroPoint* TempPoint = new CAcroPoint;
		LPDISPATCH TempDispatch =  m_pAcroPDPage->GetSize();
		TempPoint->AttachDispatch(TempDispatch);

        // Determine scale necessary to fit width of page into
        // view window
		xScale = ((long)(width) * 100) / TempPoint->GetX();

		// Determine overall scale according to ViewMode
		switch(ViewMode) {
				case VIEWMODE_ACTUALSIZE:      // Show actual size of page
					nScale = 100;
					break;
				case VIEWMODE_FITPAGE:         // Fit page to view window
	 				// Determine scale necessary to fit height of page
	 				// into view window
	 				yScale = ((long)(height) * 100) / TempPoint->GetY();

					// Overall scale = min(xScale, yScale);
 					if (xScale > yScale)
	 					nScale = (short) yScale;
 					else
	 					nScale = (short) xScale;
					break;
				case VIEWMODE_FITWIDTH:        // Fit page to width of window
				    // Overall scale = xScale
				    nScale = (short) xScale;
					break;
		}
		// Delete temporary CAcroPoint object
		delete(TempPoint);
	}
}


void CStaticViewDoc::Draw(CDC *pDC, HWND m_hWnd, CPoint Origin)
{
	if (m_pAcroPDPage) {
        // calculate scaling factor
		RECT ViewRect;
 		GetClientRect(m_hWnd,(LPRECT) &ViewRect);
 		CalculateScale((ViewRect.right - ViewRect.left),(ViewRect.bottom - ViewRect.top));

		BOOL b = m_pAcroPDPage->DrawEx((long)m_hWnd, NULL, NULL,
				(short)-Origin.x, (short)-Origin.y, nScale);
  }
}

void CStaticViewDoc::SetToolbarStatusBoxes()
{
		// update edit box to reflect new page number
		CEdit* pEdit = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetEditBox();
		char StrVal[20];
		_ltoa(CurPage+1,StrVal,10);
 		pEdit->SetWindowText(StrVal);

		// update Zoom % box if nScale has been initialized
		if (nScale != NOT_YET_SET) {
			_itoa(nScale,StrVal,10);
			strcat(StrVal,"%");
			((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetZoomButton()->SetWindowText(StrVal);
		}
}

void CStaticViewDoc::GotoPage(unsigned long NewPage)
{
	if (NewPage < NumPages) {

		CurPage = NewPage;

		// switch page
		LPDISPATCH TempDispatch = m_pAcroPDDoc->AcquirePage(CurPage);
		if (TempDispatch == NULL) {
			AfxMessageBox("Failed to create object :<");
		}
		m_pAcroPDPage->AttachDispatch(TempDispatch);

		// cause reDraw of page
		UpdateAllViews(NULL);

		// calculate new scale so it can be updated on toolbar
		CalculateScale(OldWidth, OldHeight);
        SetToolbarStatusBoxes();
	}
}


void CStaticViewDoc::SetScale(int NewScale)
{
	nScale = NewScale;

	// Viewmode no longer does auto-sizing
	ViewMode = VIEWMODE_NONAUTO;

	// Reset status boxes (for new zoom factor)
	SetToolbarStatusBoxes();

	// Force a reDraw (LPARAM) TRUE means status bars
	// will be recomputed
	UpdateAllViews(NULL, (LPARAM) TRUE, NULL);
}

int CStaticViewDoc::GetScale(void)
{
	return nScale;
}

CSize CStaticViewDoc::GetScaledDocSize(int width, int height)
{
	if (m_pAcroPDPage) {
		// Calculate scale factor
		CalculateScale(width, height);

		// Get actual page size using temporary CAcroPoint
		CAcroPoint* TempPoint = new CAcroPoint;
		LPDISPATCH TempDispatch =  m_pAcroPDPage->GetSize();
		TempPoint->AttachDispatch(TempDispatch);



		// Scale doc size
		CSize TempCSize = CSize((int)(((float)nScale / 100) * TempPoint->GetX()),
					 (int)(((float)nScale / 100) * TempPoint->GetY()));

		// Delete temporary CAcroPoint
		delete(TempPoint);

		return (TempCSize);
	}
	else
		return(CSize(0,0));
}

/////////////////////////////////////////////////////////////////////////////
// CStaticViewDoc serialization

void CStaticViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CStaticViewDoc diagnostics

#ifdef _DEBUG
void CStaticViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStaticViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStaticViewDoc commands

void CStaticViewDoc::OnUpdateViewFirstpage(CCmdUI* pCmdUI)
{
	// Only enable Go to First Page button if not on first page already
	pCmdUI->Enable((BOOL) CurPage > 0);
}

void CStaticViewDoc::OnUpdateViewLastpage(CCmdUI* pCmdUI)
{
	// Only enable Go to Last Page button if not on last page already
	pCmdUI->Enable((BOOL) (CurPage < (unsigned long) (NumPages-1)));
}

void CStaticViewDoc::OnUpdateViewNextpage(CCmdUI* pCmdUI)
{
	// Only enable Go to Next Page button if not on last page already
	pCmdUI->Enable((BOOL) (CurPage < (unsigned long) (NumPages-1)));
}

void CStaticViewDoc::OnUpdateViewPrevpage(CCmdUI* pCmdUI)
{
	// Only enable Go to Previous Page button if not on first page already
	pCmdUI->Enable((BOOL) CurPage > 0);
}

void CStaticViewDoc::OnViewFirstpage()
{
	// Go o first page
	GotoPage(0);
}

void CStaticViewDoc::OnViewLastpage()
{
	// Go to last page
	GotoPage(NumPages-1);
}

void CStaticViewDoc::OnViewNextpage()
{
	// Go to next page
	GotoPage(CurPage+1);
}

void CStaticViewDoc::OnViewPrevpage()
{
	// Go to previous page
	GotoPage(CurPage-1);
}


void CStaticViewDoc::OnViewActualsize()
{
	// Set viewmode to display actual size
	ViewMode = VIEWMODE_ACTUALSIZE;

	// Calculate scale factor
	CalculateScale(OldWidth, OldHeight);

	// Update toolbar status boxes (new scale)
	SetToolbarStatusBoxes();

	// Force reDraw, update scroll bars
	UpdateAllViews(NULL,(LPARAM) TRUE, NULL);
}

void CStaticViewDoc::OnViewFitpage()
{
	// Set viewmode to display fit page
	ViewMode = VIEWMODE_FITPAGE;

	// Calculate scale factor
	CalculateScale(OldWidth, OldHeight);

	// Update toolbar status boxes (new scale)
	SetToolbarStatusBoxes();

	// Force reDraw, update scroll bars
	UpdateAllViews(NULL,(LPARAM) TRUE, NULL);
}

void CStaticViewDoc::OnViewFitwidth()
{
	// Set viewmode to display fit width
	ViewMode = VIEWMODE_FITWIDTH;

	// Calculate scale factor
	CalculateScale(OldWidth, OldHeight);

	// Update toolbar status boxes (new scale)
	SetToolbarStatusBoxes();

	// Force reDraw, update scroll bars
	UpdateAllViews(NULL,(LPARAM) TRUE, NULL);
}

void CStaticViewDoc::OnUpdateViewActualsize(CCmdUI* pCmdUI)
{
	// Check actual size option if Viewmode is actual size
	pCmdUI->SetCheck((BOOL) ViewMode == VIEWMODE_ACTUALSIZE);
}

void CStaticViewDoc::OnUpdateViewFitpage(CCmdUI* pCmdUI)
{
	// Check fit page option if Viewmode is fit page
	pCmdUI->SetCheck((BOOL) ViewMode == VIEWMODE_FITPAGE);
}

void CStaticViewDoc::OnUpdateViewFitwidth(CCmdUI* pCmdUI)
{
	// Check fit width option if Viewmode is fit width
	pCmdUI->SetCheck((BOOL) ViewMode == VIEWMODE_FITWIDTH);
}


void CStaticViewDoc::OnZoomButton()
{
	// If zoom button clicked, do zoom dialog box
	CZoomSet dlg;
	dlg.DoModal();
}


void CStaticViewDoc::OnOtherCopytoclipboard()
{
	if (m_pAcroPDPage) {
		COleException e;
		CAcroRect* rect = new CAcroRect;

		if (rect) {
			rect->CreateDispatch("AcroExch.Rect", &e);
			rect->SetLeft(0);
			rect->SetTop(0);
			rect->SetRight(400);
			rect->SetBottom(400);

			BOOL b = m_pAcroPDPage->CopyToClipboard(rect->m_lpDispatch, 0, 0, 100);

			delete rect;
		}
	}
}
// called when print menu item is clicked
void CStaticViewDoc::OnFilePrint()
{
	// TODO: Add your command handler code here

	// get AVDoc
	if(m_pAcroAVDoc == NULL) {
		m_pAcroAVDoc = new CAcroAVDoc;
		m_pAcroAVDoc->AttachDispatch(m_pAcroPDDoc->OpenAVDoc("PDF Window"),true);
	}
	
	// print the currrent page with nPSLevel = 2, bBinaryOk and bShrinkToFit = 0.
	long n = m_pAcroPDPage->GetNumber();
	BOOL b = m_pAcroAVDoc->PrintPagesSilent(n, n, 2, 0, 0);
	if(b!=0)
		AfxMessageBox("The current page was printed.");

}
