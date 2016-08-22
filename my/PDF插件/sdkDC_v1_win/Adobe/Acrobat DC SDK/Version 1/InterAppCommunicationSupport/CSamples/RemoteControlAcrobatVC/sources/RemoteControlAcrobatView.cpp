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

 RemoteControlAcrobatView.cpp : implementation of the CRemoteControlAcrobatView class

  Acrobat IAC code was added to the file.

*********************************************************************/

#include "stdafx.h"
#include "RemoteControlAcrobat.h"

#include "RemoteControlAcrobatDoc.h"
#include "RemoteControlAcrobatView.h"

#include "FindTextDlg.h"
#include "PageNumDlg.h"

// acrobat
#include "acrobat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// global flags for Acrobat IAC
bool gACROAPP_IS_HIDE;

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatView

IMPLEMENT_DYNCREATE(CRemoteControlAcrobatView, CView)

BEGIN_MESSAGE_MAP(CRemoteControlAcrobatView, CView)
	//{{AFX_MSG_MAP(CRemoteControlAcrobatView)
	ON_COMMAND(ID_ACROBAT_APP_HIDE, OnAcrobatAppHide)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_APP_HIDE, OnUpdateAcrobatAppHide)
	ON_COMMAND(ID_ACROBAT_APP_SHOW, OnAcrobatAppShow)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_APP_SHOW, OnUpdateAcrobatAppShow)
	ON_COMMAND(ID_ACROBAT_LAUNCH, OnAcrobatLaunch)
	ON_COMMAND(ID_ACROBAT_QUIT, OnAcrobatExit)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_QUIT, OnUpdateAcrobatExit)
	ON_COMMAND(ID_ACROBAT_AVDOC_OPEN, OnAcrobatAvdocOpen)
	ON_COMMAND(ID_ACROBAT_AVDOC_PRINT, OnAcrobatAvdocPrint)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_AVDOC_PRINT, OnUpdateAcrobatAvdocPrint)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_AVDOC_CLOSE, OnUpdateAcrobatAvdocClose)
	ON_COMMAND(ID_ACROBAT_AVDOC_CLOSE, OnAcrobatAvdocClose)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_LAUNCH, OnUpdateAcrobatLaunch)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_AVDOC_OPEN, OnUpdateAcrobatAvdocOpen)
	ON_COMMAND(ID_ACROBAT_AVPAGEVIEW_GOBACK, OnAcrobatAvpageviewGoback)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_AVPAGEVIEW_GOBACK, OnUpdateAcrobatAvpageviewGoback)
	ON_COMMAND(ID_ACROBAT_AVPAGEVIEW_GOFORWARD, OnAcrobatAvpageviewGoforward)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_AVPAGEVIEW_GOFORWARD, OnUpdateAcrobatAvpageviewGoforward)
	ON_COMMAND(ID_ACROBAT_AVPAGEVIEW_PAGENUMBER, OnAcrobatAvpageviewPagenumber)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_AVPAGEVIEW_PAGENUMBER, OnUpdateAcrobatAvpageviewPagenumber)
	ON_COMMAND(ID_ACROBAT_AVDOC_FINDTEXT, OnAcrobatAvdocFindtext)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_AVDOC_FINDTEXT, OnUpdateAcrobatAvdocFindtext)
	ON_COMMAND(ID_ACROBAT_AVPAGEVIEW_GOTOPAGE, OnAcrobatAvpageviewGotopage)
	ON_UPDATE_COMMAND_UI(ID_ACROBAT_AVPAGEVIEW_GOTOPAGE, OnUpdateAcrobatAvpageviewGotopage)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatView construction/destruction

CRemoteControlAcrobatView::CRemoteControlAcrobatView()
{
	// TODO: add construction code here
	// Initialize Acrobat IAC objects as NULL
	m_pAcroApp = NULL;
	m_pAvdoc = NULL;
	m_pPageView = NULL;

	gACROAPP_IS_HIDE = false;
}

CRemoteControlAcrobatView::~CRemoteControlAcrobatView()
{
	// finish
	OnAcrobatExit();
}

BOOL CRemoteControlAcrobatView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatView drawing

void CRemoteControlAcrobatView::OnDraw(CDC* pDC)
{
	CRemoteControlAcrobatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatView printing

BOOL CRemoteControlAcrobatView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRemoteControlAcrobatView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRemoteControlAcrobatView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatView diagnostics

#ifdef _DEBUG
void CRemoteControlAcrobatView::AssertValid() const
{
	CView::AssertValid();
}

void CRemoteControlAcrobatView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRemoteControlAcrobatDoc* CRemoteControlAcrobatView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRemoteControlAcrobatDoc)));
	return (CRemoteControlAcrobatDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatView message handlers

void CRemoteControlAcrobatView::OnAcrobatLaunch() 
{
	// TODO: Add your command handler code here
	
	// only one instance allowed
	if(m_pAcroApp) return;

	// start Acrobat, create or get existing object "AcroExch.App"
	COleException e;
	m_pAcroApp = new CAcroApp;
	m_pAcroApp->CreateDispatch("AcroExch.App", &e);
	if(!m_pAcroApp)
		AfxMessageBox("Can not get Acrobat IAC Object!");
	else
		m_pAcroApp->Show();	

	// if there is open document, attach object "AcroExch.AVDoc" now,
	// otherwise leave m_pAvdoc as NULL.
	if(m_pAcroApp->GetNumAVDocs()>0) {
		m_pAvdoc = new CAcroAVDoc;
		m_pAvdoc->AttachDispatch(m_pAcroApp->GetActiveDoc(), TRUE);
	}
}

void CRemoteControlAcrobatView::OnUpdateAcrobatLaunch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((BOOL) m_pAcroApp == NULL);
}

void CRemoteControlAcrobatView::OnAcrobatExit() 
{
	// TODO: Add your command handler code here
	if(!m_pAcroApp) return;
	
	// close all PDFs and exit
	m_pAcroApp->CloseAllDocs();
	m_pAcroApp->Hide();
	m_pAcroApp->Exit();

	// clean memory
	delete m_pAcroApp;
	m_pAcroApp = NULL;	
	delete m_pAvdoc;
	m_pAvdoc = NULL;
	delete m_pPageView;
	m_pPageView = NULL;

	gACROAPP_IS_HIDE = false;
}

void CRemoteControlAcrobatView::OnUpdateAcrobatExit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((BOOL) m_pAcroApp != NULL);
}

void CRemoteControlAcrobatView::OnAcrobatAppHide() 
{
	// TODO: Add your command handler code here
	if(!m_pAcroApp) return;

	// hide 
	m_pAcroApp->Hide();	
	gACROAPP_IS_HIDE = true;
}

void CRemoteControlAcrobatView::OnUpdateAcrobatAppHide(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((BOOL) m_pAcroApp != NULL && !gACROAPP_IS_HIDE);
}

void CRemoteControlAcrobatView::OnAcrobatAppShow() 
{
	// TODO: Add your command handler code here
	if(!m_pAcroApp) return;

	// show
	m_pAcroApp->Show();
	gACROAPP_IS_HIDE = false;
}

void CRemoteControlAcrobatView::OnUpdateAcrobatAppShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((BOOL) m_pAcroApp != NULL && gACROAPP_IS_HIDE);
}

void CRemoteControlAcrobatView::OnAcrobatAvdocOpen() 
{
	// TODO: Add your command handler code here

	// create a AVDoc object if there is not one.
	if(!m_pAvdoc) {
		m_pAvdoc = new CAcroAVDoc;
		COleException e;
		m_pAvdoc->CreateDispatch("AcroExch.AVDoc", &e);
	}

	// Display a standard file open dialog for user to select input file.
	CFileDialog fd(TRUE, "*.pdf", "*.pdf");
	if (fd.DoModal() == IDOK) {

		// open PDF 
		if(!m_pAvdoc->Open(fd.GetPathName(), NULL)) 

			// error message if cannot open the pdf
			AfxMessageBox("Error in opening a PDF file.");
		else
			OnAcrobatAppShow();
	}
}

void CRemoteControlAcrobatView::OnUpdateAcrobatAvdocOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		if(!m_pAcroApp) pCmdUI->Enable(false);
		else pCmdUI->Enable(!(PDFDocIsActive()));	

}


void CRemoteControlAcrobatView::OnAcrobatAvdocPrint() 
{
	// TODO: Add your command handler code here
	// get active avdoc
	m_pAvdoc->AttachDispatch(m_pAcroApp->GetActiveDoc(), TRUE);
	if(!m_pAvdoc) {
		AfxMessageBox("Cannot print. No PDF opened.");
		return;
	}
	
	//
	CAcroPDDoc pddoc;
	pddoc.AttachDispatch(m_pAvdoc->GetPDDoc(), TRUE);
	long num = pddoc.GetNumPages();
	// print it
	BOOL rc = m_pAvdoc->PrintPages((long)0, (long)(num-1), 2L, (long) 0, (long) 0); // true not working

	// message to inform result
	char msg[40];
	if(rc) sprintf(msg, "All pages were printed.");
	else sprintf(msg, "Error in print pages.");
	AfxMessageBox(msg);
}

void CRemoteControlAcrobatView::OnUpdateAcrobatAvdocPrint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(PDFDocIsActive());	
}



void CRemoteControlAcrobatView::OnAcrobatAvdocClose() 
{
	// TODO: Add your command handler code here
	// get active avdoc
	if(!m_pAcroApp) return;
	m_pAvdoc->AttachDispatch(m_pAcroApp->GetActiveDoc(), TRUE);
	
	// close it
	m_pAvdoc->Close(true);
}

void CRemoteControlAcrobatView::OnUpdateAcrobatAvdocClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(PDFDocIsActive());	
}



void CRemoteControlAcrobatView::OnAcrobatAvpageviewGoback() 
{
	// TODO: Add your command handler code here
	if(!m_pPageView) {
		m_pPageView = new CAcroAVPageView;	
		m_pPageView->AttachDispatch(m_pAvdoc->GetAVPageView(), TRUE);
	}
	m_pPageView->DoGoBack();
}

void CRemoteControlAcrobatView::OnUpdateAcrobatAvpageviewGoback(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(PDFDocIsActive());	
}

void CRemoteControlAcrobatView::OnAcrobatAvpageviewGoforward() 
{
	// TODO: Add your command handler code here
	if(!m_pPageView) {
		m_pPageView = new CAcroAVPageView;	
		m_pPageView->AttachDispatch(m_pAvdoc->GetAVPageView(), TRUE);
	}
	m_pPageView->DoGoForward();
}

void CRemoteControlAcrobatView::OnUpdateAcrobatAvpageviewGoforward(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(PDFDocIsActive());	
}

void CRemoteControlAcrobatView::OnAcrobatAvpageviewPagenumber() 
{
	// TODO: Add your command handler code here

	// make sure m_pAvdoc points to active AVDoc object
	if(!m_pAvdoc) return;
	m_pAvdoc->AttachDispatch(m_pAcroApp->GetActiveDoc(), TRUE);

	// get its pageview
	if(!m_pPageView) {
		m_pPageView = new CAcroAVPageView;	
	}
	m_pPageView->AttachDispatch(m_pAvdoc->GetAVPageView(), TRUE);

	// get page number and display
	if(m_pPageView) {
		int n = (int) m_pPageView->GetPageNum();
		char str[40];
		sprintf(str, "Current page number is %d.", n+1);
		AfxMessageBox(str);
	}
	else
		AfxMessageBox("Error in find the page number.");
}


void CRemoteControlAcrobatView::OnUpdateAcrobatAvpageviewPagenumber(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(PDFDocIsActive());	
}


void CRemoteControlAcrobatView::OnAcrobatAvpageviewGotopage() 
{
	// TODO: Add your command handler code here

	// make sure m_pAvdoc points to active AVDoc object
	m_pAvdoc->AttachDispatch(m_pAcroApp->GetActiveDoc(), TRUE);

	// get its pageview
	if(!m_pPageView) {
		m_pPageView = new CAcroAVPageView;	
	}
	m_pPageView->AttachDispatch(m_pAvdoc->GetAVPageView(), TRUE);

	// get PDDoc object 
	CAcroPDDoc pddoc;
	pddoc.AttachDispatch(m_pAvdoc->GetPDDoc(), TRUE);

	// pop up dialog
	PageNumDlg dlg;
	dlg.m_szFileName = pddoc.GetFileName();
	if(dlg.DoModal()==IDOK) {
		
		// get page number and display
		if(m_pPageView) 
			m_pPageView->GoTo(dlg.m_szPageNum-1);
		else
			AfxMessageBox("Error in goto page.");
		}
}


void CRemoteControlAcrobatView::OnUpdateAcrobatAvpageviewGotopage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(PDFDocIsActive());	
}

void CRemoteControlAcrobatView::OnAcrobatAvdocFindtext() 
{
	// TODO: Add your command handler code here

	// make sure m_pAvdoc points to active AVDoc object
	m_pAvdoc->AttachDispatch(m_pAcroApp->GetActiveDoc(), TRUE);

	// get PDDoc object in order to set filename shown in dilaog.
	CAcroPDDoc pddoc;
	pddoc.AttachDispatch(m_pAvdoc->GetPDDoc(), TRUE);

	// pop up dialog
	FindTextDlg dlg;
	dlg.m_szFileName = pddoc.GetFileName();
	if(dlg.DoModal()==IDOK) {

		// set parameters
		BOOL bCaseSensitive = false, bWholeWordsOnly=true, bReset= true;

		// copy the text user input
		char szText[24];
		strcpy(szText,(const char*) dlg.m_szFindWord);
		szText[23] = '\0';

		// find text and highlight it.
		BOOL rc = m_pAvdoc->FindText(szText, bCaseSensitive,
							 bWholeWordsOnly, bReset);
		if(!rc)
			AfxMessageBox("Cannot find the text in the document.");
		else
			OnAcrobatAppShow();
	}
}

void CRemoteControlAcrobatView::OnUpdateAcrobatAvdocFindtext(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		pCmdUI->Enable(PDFDocIsActive());	
}


// function used to enable menu item and operations. 
// return true if a PDF Doc Is Active in Acrobat, otherwise false.
bool CRemoteControlAcrobatView::PDFDocIsActive() 
{
	// TODO: Add your command update UI handler code here
	if(!m_pAcroApp) 
		return (false);	
	else if(m_pAcroApp->GetNumAVDocs()){

		LPDISPATCH activeDoc = m_pAcroApp->GetActiveDoc();
		if (!activeDoc)
			return false;

		if(!m_pAvdoc)
			m_pAvdoc = new CAcroAVDoc;
		m_pAvdoc->AttachDispatch(activeDoc, TRUE);

		if(m_pAvdoc)
			return (true);
		else 
			return (false);
	}
	else
		return (false);	
}

