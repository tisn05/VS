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

 ActiveViewDoc.cpp : Implementation of the CActiveViewDoc class.

*********************************************************************/

#include <string.h>

#include "stdafx.h"
#include "activeview.h"
#include "activeviewdoc.h"
#include "mainfrm.h"
#include "iac.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActiveViewDoc

IMPLEMENT_DYNCREATE(CActiveViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CActiveViewDoc, CDocument)
	//{{AFX_MSG_MAP(CActiveViewDoc)
	ON_COMMAND(ID_VIEW_FIRSTPAGE, OnViewFirstpage)
	ON_COMMAND(ID_VIEW_LASTPAGE, OnViewLastpage)
	ON_COMMAND(ID_VIEW_NEXTPAGE, OnViewNextpage)
	ON_COMMAND(ID_VIEW_PREVPAGE, OnViewPrevpage)
	ON_COMMAND(ID_VIEW_ACTUALSIZE, OnViewActualsize)
	ON_COMMAND(ID_VIEW_FITPAGE, OnViewFitpage)
	ON_COMMAND(ID_VIEW_FITWIDTH, OnViewFitwidth)
	ON_COMMAND(ID_TOOLS_HAND, OnToolsHand)
	ON_COMMAND(ID_TOOLS_HILITEWORDS, OnToolsHilitewords)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_COMMAND(ID_EDIT_SELECTGRAPHIC, OnEditSelectgraphic)
	ON_COMMAND(ID_EDIT_SELECTTEXT, OnEditSelecttext)
	ON_COMMAND(ID_TOOLS_HILITEBYRECT, OnToolsHilitebyrect)
	ON_COMMAND(ID_TOOLS_ACQUIREBOOKMARK, OnToolsAcquireBookmark)
	ON_COMMAND(ID_TOOLS_ACQUIREANNOT, OnToolsAcquireAnnot)
	ON_COMMAND(ID_TOOLS_ADDTEXTANNOT, OnToolsAddTextAnnot)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_HAND, OnUpdateToolsHand)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTGRAPHIC, OnUpdateEditSelectgraphic)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTTEXT, OnUpdateEditSelecttext)
	ON_COMMAND(ID_TOOLS_DISPLAYAPERTURE, OnToolsDisplayaperture)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CActiveViewDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CActiveViewDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActiveViewDoc construction/destruction

CActiveViewDoc::CActiveViewDoc()
{
	EnableAutomation();

    // initialize pointers to acrobat objects to NULL
	m_pAcroAVDoc = NULL;

	AfxOleLockApp();
}

CActiveViewDoc::~CActiveViewDoc()
{
    // delete acrobat objects
	if (m_pAcroAVDoc != NULL) {
		long l = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->GetNumAVDocs();
		BOOL B = m_pAcroAVDoc->Close(FALSE);
		l = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->GetNumAVDocs();
		delete m_pAcroAVDoc;
	}

	AfxOleUnlockApp();
}

// Called when a document is opened
BOOL CActiveViewDoc::OnOpenDocument(const char* pszPathName)
{
	// Create new CAcroAVDoc
	m_pAcroAVDoc = new CAcroAVDoc;
	if (!m_pAcroAVDoc->CreateDispatch ("AcroExch.AVDoc")) {
		AfxMessageBox ("Failed to create AVDoc");
		return FALSE;
	}

	// Open Specified .PDF file

	POSITION pos = GetFirstViewPosition();
	CView* pFirstView = GetNextView (pos);
	HWND OurHWND = pFirstView->m_hWnd;

	// Valid OpenFlags:
	//	AV_EXTERNAL_VIEW - Open the document with tool bar visible
	//	AV_DOC_VIEW - Draw the page pane and scrollbars
	//	AV_PAGE_VIEW - Draw only the page pane

	BOOL ok = m_pAcroAVDoc->OpenInWindowEx (pszPathName, (long)OurHWND, AV_DOC_VIEW /* openFlgs */,
					TRUE /* UseOpenParams */, 0 /* pgNum */, PDUseBookmarks /* pageMode */,
					AVZoomFitPage /* ZoomType */, 0 /* Zoom */, 0 /* Top */, 0 /* Left */);

	if (!ok) {
		AfxMessageBox ("Failed to opening file.");
        return FALSE;
	}

	// Get # of pages in file
	CAcroPDDoc tempDoc;
	tempDoc.AttachDispatch (m_pAcroAVDoc->GetPDDoc(),TRUE);

	gNumPages = tempDoc.GetNumPages();

	return(TRUE);
}
// Called by framework to save file
BOOL CActiveViewDoc::OnSaveDocument(const char* pszPathName)
{
	// Pass filename through to Acrobat's save function
	CAcroPDDoc TempDoc;
	TempDoc.AttachDispatch(m_pAcroAVDoc->GetPDDoc(), TRUE);

	return(TempDoc.Save(1,pszPathName));
}


void CActiveViewDoc::GotoPage(unsigned long NewPage)
{
	if ((NewPage < gNumPages) && (NewPage >= 0)) {

		// Get the AVPageView
		CAcroAVPageView pageView;
		pageView.AttachDispatch(m_pAcroAVDoc->GetAVPageView(), TRUE);

		// Switch page
        pageView.GoTo(NewPage);
	}
}

void CActiveViewDoc::SetScale(int NewScale)
{
	// Set new zoom type and scale in viewer
	SetCurrentZoomType (AVZoomNoVary, NewScale);
}

unsigned long CActiveViewDoc::GetCurrentPageNum()
{
	// Get the AVPageView
	CAcroAVPageView pageView;
	pageView.AttachDispatch(m_pAcroAVDoc->GetAVPageView(), TRUE);
	return pageView.GetPageNum();
}

int CActiveViewDoc::GetCurrentZoomType()
{
	// Get the AVPageView
	CAcroAVPageView pageView;
	pageView.AttachDispatch(m_pAcroAVDoc->GetAVPageView(), TRUE);
	return( pageView.GetZoomType());
}

void CActiveViewDoc::SetCurrentZoomType(int NewType, int NewScale)
{
	// Get the AVPageView
	CAcroAVPageView pageView;
	pageView.AttachDispatch(m_pAcroAVDoc->GetAVPageView(), TRUE);

	// Set the zoom type: parameter 2 is ignored by viewer except
	// in case of type being no auto-resizing
	pageView.ZoomTo(NewType, NewScale);
}

void CActiveViewDoc::CloseAVDoc()
{
	if (m_pAcroAVDoc) {
		long l = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->GetNumAVDocs();
		BOOL b = m_pAcroAVDoc->Close(FALSE);
		l = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->GetNumAVDocs();
		delete m_pAcroAVDoc;
		m_pAcroAVDoc = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CActiveViewDoc serialization

void CActiveViewDoc::Serialize(CArchive& ar)
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
// CActiveViewDoc diagnostics

#ifdef _DEBUG
void CActiveViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CActiveViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CActiveViewDoc commands

void CActiveViewDoc::OnUpdateViewFirstpage(CCmdUI* pCmdUI)
{
	// Only enable Go to First Page button if not on first page already
	pCmdUI->Enable((BOOL) GetCurrentPageNum() > 0);
}

void CActiveViewDoc::OnUpdateViewLastpage(CCmdUI* pCmdUI)
{
	// Only enable Go to Last Page button if not on last page already
	pCmdUI->Enable((BOOL) (GetCurrentPageNum() < (unsigned long) (gNumPages-1)));
}

void CActiveViewDoc::OnUpdateViewNextpage(CCmdUI* pCmdUI)
{
	// Only enable Go to Next Page button if not on last page already
	pCmdUI->Enable((BOOL) (GetCurrentPageNum() < (unsigned long) (gNumPages-1)));
}

void CActiveViewDoc::OnUpdateViewPrevpage(CCmdUI* pCmdUI)
{
	// Only enable Go to Previous Page button if not on first page already
	pCmdUI->Enable((BOOL) GetCurrentPageNum() > 0);
}

void CActiveViewDoc::OnViewFirstpage()
{
	// Go to first page
	GotoPage(0);
}

void CActiveViewDoc::OnViewLastpage()
{
	// Go to last page
	GotoPage(gNumPages-1);
}

void CActiveViewDoc::OnViewNextpage()
{
	// Go to next page
	GotoPage(GetCurrentPageNum()+1);
}

void CActiveViewDoc::OnViewPrevpage()
{
	// Go to previous page
	GotoPage(GetCurrentPageNum()-1);
}

void CActiveViewDoc::OnViewActualsize()
{
	// Set zoom type to display actual size
	SetCurrentZoomType(AVZoomNoVary,100);
}

void CActiveViewDoc::OnViewFitpage()
{
	// Set zoom type  to display fit page
	SetCurrentZoomType(AVZoomFitPage,0);
}

void CActiveViewDoc::OnViewFitwidth()
{
	// Set viewmode to display fit width
	SetCurrentZoomType(AVZoomFitWidth,0);
}

void CActiveViewDoc::OnUpdateViewFitpage(CCmdUI* pCmdUI)
{
	// Check fit page option if Viewmode is fit page
	pCmdUI->SetCheck((BOOL) GetCurrentZoomType() == AVZoomFitPage);
}

void CActiveViewDoc::OnUpdateViewFitwidth(CCmdUI* pCmdUI)
{
	// Check fit width option if Viewmode is fit width
	pCmdUI->SetCheck((BOOL) GetCurrentZoomType() == AVZoomFitWidth);
}

void CActiveViewDoc::OnUpdateTools(CCmdUI* pCmdUI)
{
	// Enable tools whenever a file is open
	pCmdUI->Enable((BOOL) TRUE);
}

void CActiveViewDoc::OnToolsHand()
{
	BOOL b;

	b = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->SetActiveTool( "Hand", TRUE );
}

void CActiveViewDoc::OnEditCopy()
{
	BOOL b;

	b = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->MenuItemIsEnabled("Copy");
	if (b)
		b = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->MenuItemExecute("Copy");
}

void CActiveViewDoc::OnEditSelectall()
{
	BOOL b;

	b = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->MenuItemIsEnabled("SelectAll");
	if (b)
		b = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->MenuItemExecute("SelectAll");
}

void CActiveViewDoc::OnEditSelectgraphic()
{
	BOOL b;

	b = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->SetActiveTool( "SelectGraphics", FALSE );
}

void CActiveViewDoc::OnEditSelecttext()
{
	BOOL b;

	b = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->SetActiveTool( "Select", FALSE );
}

// This code demonstrates how to hilite words with either a word or page hilite list
void CActiveViewDoc::OnToolsHilitewords()
{
	CAcroAVPageView pageView;
	CAcroPDPage page;
	CAcroPDTextSelect* textSelect = new CAcroPDTextSelect;
	CAcroHiliteList* hilite = new CAcroHiliteList;
	char buf[255];
	long selectionSize;

	if ((BOOL) GetCurrentPageNum() > PDBeforeFirstPage) {

		// Obtain the AVPageView
		pageView.AttachDispatch(m_pAcroAVDoc->GetAVPageView(),TRUE);

		// Create the Hilite list object
		hilite->CreateDispatch("AcroExch.HiliteList");
		if (hilite) {

			// Add the first 10 words or characters of that page to the hilite list
			hilite->Add(0,10);
			page.AttachDispatch(pageView.GetPage(), TRUE);

			// Create text selection for either page or word hilite list
			textSelect->AttachDispatch(page.CreateWordHilite(hilite->m_lpDispatch));
			m_pAcroAVDoc->SetTextSelection(textSelect->m_lpDispatch);
			m_pAcroAVDoc->ShowTextSelect();

			// Extract the number of words and the first word of text selection
			selectionSize = textSelect->GetNumText();
			if (selectionSize)
				sprintf (buf, "# of words in text selection: %ld\n1st word in text selection = '%s'",
							selectionSize, textSelect->GetText(0));
			else
				sprintf (buf, "Failed to create text selection.");

			AfxMessageBox(buf);
		}
	}

	delete textSelect;
	delete hilite;
}

// This code demonstrates how to hilite words with a rectangle
void CActiveViewDoc::OnToolsHilitebyrect()
{
	CAcroRect* rect = new CAcroRect;
	CAcroPDTextSelect* textSelect = new CAcroPDTextSelect;
	CAcroPDDoc pdDoc;
	char buf[255];
	long num, selectionSize;

	num = GetCurrentPageNum();
	if (num > PDBeforeFirstPage) {

		// Acquire the PDDoc
		pdDoc.AttachDispatch(m_pAcroAVDoc->GetPDDoc(),TRUE);

		// Create a dispatch for the rect
		rect->CreateDispatch("AcroExch.Rect");
		if (rect) {

			// Set values for rect - increases from right to left and bottom to top
			rect->SetLeft(100);
			rect->SetTop(400);
			rect->SetRight(400);
			rect->SetBottom(100);

			// Set the text selection to the rectangular region
			textSelect->AttachDispatch(pdDoc.CreateTextSelect(num, rect->m_lpDispatch));
			m_pAcroAVDoc->SetTextSelection(textSelect->m_lpDispatch);
			m_pAcroAVDoc->ShowTextSelect();

			// Extract the number of words and the first word of text selection
			selectionSize = textSelect->GetNumText();
			if (selectionSize)
				sprintf (buf, "# of words in text selection: %ld\n1st word in text selection = '%s'",
							selectionSize, textSelect->GetText(0));
			else
				sprintf (buf, "Failed to create text selection.");

			AfxMessageBox(buf);
		}
	}

	delete rect;
	delete textSelect;
}

// This code demonstrates how to acquire a bookmark by name
void CActiveViewDoc::OnToolsAcquireBookmark()
{
	char buf[255];
	CAcroPDBookmark* bkmk = new CAcroPDBookmark;

	// Create a dispatch for a PDBookmark
	bkmk->CreateDispatch("AcroExch.PDBookmark");

	// Acquire a bookmark by title
	bkmk->GetByTitle(m_pAcroAVDoc->GetPDDoc(), "Untitled");
	if (bkmk->IsValid()) {
		bkmk->SetTitle("Test Title");
		sprintf(buf, "Title of 'Untitled' Bookmark is now %s", bkmk->GetTitle());
		AfxMessageBox(buf);
	}
	else {

		// Create a new bookmark since no "Untitled" Bookmark is present. Note that
		// if there is a document open in Acrobat, the bookmark will be created in
		// that document, not the one open in this application.

		((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->MenuItemExecute("NewBookmark");
		bkmk->GetByTitle(m_pAcroAVDoc->GetPDDoc(), "Untitled");
		if (bkmk->IsValid()) {
			bkmk->SetTitle("Test Title");
			sprintf(buf, "Title of 'Untitled' Bookmark is now %s", bkmk->GetTitle());
			AfxMessageBox(buf);
		}
	}

	delete bkmk;
}

// This code demonstrates how to determine the number of annots and acquiring an annot
void CActiveViewDoc::OnToolsAcquireAnnot()
{
	CAcroAVPageView pageView;
	CAcroPDPage pdPage;
	CAcroPDAnnot* pdAnnot = new CAcroPDAnnot;
	char buf[255], str[20];
	long numAnnots;

	// Obtain the AVPageView and PDPage
	pageView.AttachDispatch(m_pAcroAVDoc->GetAVPageView(),TRUE);
	pdPage.AttachDispatch(pageView.GetPage(), TRUE);

	numAnnots = pdPage.GetNumAnnots();

	if (numAnnots > 0) {
		pdAnnot->AttachDispatch(pdPage.GetAnnot(0), TRUE);
		if (pdAnnot->IsValid())
			strcpy(str, "is valid");
		else
			strcpy(str, "is not valid");

		sprintf(buf, "Num Annots on Page = %ld\nAnnot 0 %s", numAnnots, str);
	}
	else
		strcpy(buf, "There are no annots on the page");
	AfxMessageBox(buf);

	delete pdAnnot;
}

// This code demonstrates how to create a new Text annot
void CActiveViewDoc::OnToolsAddTextAnnot()
{
	CAcroAVPageView pageView;
	CAcroPDPage pdPage;
	CAcroPDAnnot* pdAnnot = new CAcroPDAnnot;
	CAcroRect* rect = new CAcroRect;

	// Obtain the AVPageView and PDPage
	pageView.AttachDispatch(m_pAcroAVDoc->GetAVPageView(),TRUE);
	pdPage.AttachDispatch(pageView.GetPage(), TRUE);

	// Create a dispatch for the rect
	rect->CreateDispatch("AcroExch.Rect");
	if (rect) {

		// Set values for rect - increases from right to left and bottom to top
		rect->SetLeft(200);
		rect->SetRight(400);
		rect->SetBottom(200);
		rect->SetTop(400);

		// Add a text annotation (a note) before the first annotation on the page
		pdAnnot->AttachDispatch(pdPage.AddNewAnnot(-1, "Text", rect->m_lpDispatch), TRUE);
		if (pdAnnot->IsValid()) {

			// Set the color for the annot; R is 1st byte, G is 2nd byte, B is 3rd byte
			// 0 is minimum (black); FF is maximum (full Red, Green, or Blue)
			unsigned long int color;
			color = 0x000000FF;	// Red
			//color = 0x0000FF00;	// Green
			//color = 0x00FF0000;	// Blue
			pdAnnot->SetColor(color);

			pdAnnot->SetTitle("New Text Annotation");
			pdAnnot->SetContents("This is an annotation of type 'Text'");

			// Set the values for the time property of the text annot
			CAcroTime time;
			if (time.CreateDispatch("AcroExch.Time")) {
				time.SetYear(1996);
				time.SetMonth(12);
				time.SetDate(25);
				time.SetHour(12);
				time.SetMinute(01);
				time.SetSecond(59);
				time.SetMillisecond(02);
				time.SetDay(02);
				pdAnnot->SetDate(time.m_lpDispatch);
			}
		}
	}

	delete pdAnnot;
	delete rect;
}

void CActiveViewDoc::OnUpdateToolsHand(CCmdUI* pCmdUI)
{
	((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->ToolButtonIsEnabled("Hand");
}

void CActiveViewDoc::OnUpdateEditSelectgraphic(CCmdUI* pCmdUI)
{
	((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->ToolButtonIsEnabled("SelectGraphics");
}

void CActiveViewDoc::OnUpdateEditSelecttext(CCmdUI* pCmdUI)
{
	((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->ToolButtonIsEnabled("Select");
}

void CActiveViewDoc::OnToolsDisplayaperture()
{
	CAcroRect* rect = new CAcroRect;
	CAcroAVPageView pageView;
	short left, right, top, bottom;
	char buf[512], buf2[255];

	rect->CreateDispatch("AcroExch.Rect");
	if (rect) {
		pageView.AttachDispatch(m_pAcroAVDoc->GetAVPageView(),TRUE);
		rect->m_lpDispatch=pageView.GetAperture();
		left = rect->GetLeft();
		right = rect->GetRight();
		top = rect->GetTop();
		bottom = rect->GetBottom();
		sprintf(buf, "Aperture: L=%d, R=%d, T=%d, B=%d\r\n", left, right, top, bottom);
		sprintf(buf2, "Indicates Top-Left corner of Acrobat page is %d device pixels from left & %d device pixels from top",
			left, top);
		strcat(buf, buf2);
		MessageBox(NULL, buf, "Aperture Info", MB_OK);
	}

	delete rect;
}
