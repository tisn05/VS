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

 ActiveViewVw.cpp : Implementation of the CActiveViewVw class.

*********************************************************************/

#include "stdafx.h"
#include "activeview.h"
#include "activeviewdoc.h"
#include "activeviewvw.h"
#include "mainfrm.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

CAcroAVDoc * tempAVDoc;

/////////////////////////////////////////////////////////////////////////////
// CActiveViewVw

IMPLEMENT_DYNCREATE(CActiveViewVw, CView)

BEGIN_MESSAGE_MAP(CActiveViewVw, CView)
	//{{AFX_MSG_MAP(CActiveViewVw)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActiveViewVw construction/destruction

CActiveViewVw::CActiveViewVw()
{
}

CActiveViewVw::~CActiveViewVw()
{
}

/////////////////////////////////////////////////////////////////////////////
// CActiveViewVw drawing

void CActiveViewVw::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CActiveViewVw printing

BOOL CActiveViewVw::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CActiveViewVw::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CActiveViewVw::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CActiveViewVw diagnostics

#ifdef _DEBUG
void CActiveViewVw::AssertValid() const
{
	CView::AssertValid();
}

void CActiveViewVw::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CActiveViewDoc* CActiveViewVw::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CActiveViewDoc)));
	return (CActiveViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CActiveViewVw message handlers


// called whenever view is focused on
void CActiveViewVw::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// Setup last view variable in main frame to point to this view
	((CMainFrame *)AfxGetApp()->m_pMainWnd)->SetLastView(this);
}

void CActiveViewVw::OnDestroy()
{
	GetDocument()->CloseAVDoc();
	CView::OnDestroy();
}

void CActiveViewVw::OnFilePrint()
{
	tempAVDoc=GetDocument()->GetAcroAVDoc();
	CCPrintDialog printDlg;
	printDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCPrintDialog dialog


CCPrintDialog::CCPrintDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCPrintDialog::IDD, pParent)
{
	long numDocs;

	numDocs = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->GetNumAVDocs();
	if (numDocs > 0) {
		CAcroPDDoc TempDoc;
		TempDoc.AttachDispatch(tempAVDoc->GetPDDoc(),TRUE);
		NumPages = TempDoc.GetNumPages();

		CAcroAVPageView TempPageView;
		TempPageView.AttachDispatch(tempAVDoc->GetAVPageView(),TRUE);
		CurrPage = TempPageView.GetPageNum();

		//{{AFX_DATA_INIT(CCPrintDialog)
		m_ShrinkToFit = FALSE;
		m_BinaryOK = FALSE;
		m_PSLevel = 0;
		m_FromPage = (UINT)(1L + CurrPage);
		m_ToPage = (UINT)NumPages;
		//}}AFX_DATA_INIT
	}
}

BEGIN_MESSAGE_MAP(CCPrintDialog, CDialog)
	//{{AFX_MSG_MAP(CCPrintDialog)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnKillfocusEdit2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCPrintDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPrintDialog)
	DDX_Check(pDX, IDC_CHECK2, m_ShrinkToFit);
	DDX_Check(pDX, IDC_CHECK1, m_BinaryOK);
	DDX_Radio(pDX, IDC_RADIO1, m_PSLevel);
	DDX_Text(pDX, IDC_EDIT1, m_FromPage);
	DDX_Text(pDX, IDC_EDIT2, m_ToPage);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCPrintDialog message handlers

void CCPrintDialog::OnOK()
{
	CDialog::OnOK();

	long int psl;
	if (!m_PSLevel)
		psl = 1L;
	else
		psl = 2L;

	long numDocs;
	numDocs = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetAcroApp()->GetNumAVDocs();
	if (numDocs > 0)
		BOOL ok = tempAVDoc->PrintPages((long)(m_FromPage-1), (long)(m_ToPage-1),
			psl, m_BinaryOK, m_ShrinkToFit);
}

void CCPrintDialog::OnKillfocusEdit1()
{
	UINT temp;
	UpdateData(TRUE);
	if (m_FromPage > m_ToPage) {
       temp = m_ToPage;
       m_ToPage = m_FromPage;
       m_FromPage = temp;
	}
	if (m_FromPage > (UINT)NumPages)
		m_FromPage = (UINT)NumPages;
	if (m_FromPage < 1)
		m_FromPage = 1;
	if (m_ToPage > (UINT)NumPages)
		m_ToPage = (UINT)NumPages;
	if (m_ToPage < 1)
		m_ToPage = 1;
	SetDlgItemInt(IDC_EDIT1,m_FromPage,FALSE);
	SetDlgItemInt(IDC_EDIT2,m_ToPage,FALSE);
}

void CCPrintDialog::OnKillfocusEdit2()
{
	UINT temp;
	UpdateData(TRUE);
	if (m_FromPage > m_ToPage) {
       temp = m_ToPage;
       m_ToPage = m_FromPage;
       m_FromPage = temp;
	}
	if (m_FromPage > (UINT)NumPages)
		m_FromPage = (UINT)NumPages;
	if (m_FromPage < 1)
		m_FromPage = 1;
	if (m_ToPage > (UINT)NumPages)
		m_ToPage = (UINT)NumPages;
	if (m_ToPage < 1)
		m_ToPage = 1;
	SetDlgItemInt(IDC_EDIT1,m_FromPage,FALSE);
	SetDlgItemInt(IDC_EDIT2,m_ToPage,FALSE);
}

void CCPrintDialog::OnCancel()
{
	CDialog::OnCancel();
}

BOOL CActiveViewVw::OnEraseBkgnd(CDC* pDC)
{
/* lets pretend we already erased the background */
	return 0;
}

void CActiveViewVw::OnSize(UINT nType, int cx, int cy)
{
	CWnd* pWndChild = GetWindow(GW_CHILD);
	if (!pWndChild)
		return;
	CRect rect;
	GetClientRect(&rect);
	pWndChild->SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),
	SWP_NOZORDER|SWP_NOMOVE);

	CView::OnSize(nType, cx, cy);
}

void CActiveViewVw::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd* pWndChild = GetWindow(GW_CHILD);
	if (!pWndChild)
		return;
	pWndChild->SendMessage(WM_KEYDOWN,nChar,MAKELONG((WORD)nRepCnt,(WORD)nFlags));

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CActiveViewVw::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd* pWndChild = GetWindow(GW_CHILD);
	if (!pWndChild)
		return;
	pWndChild->SendMessage(WM_KEYUP,nChar,MAKELONG((WORD)nRepCnt,(WORD)nFlags));

//	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
