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

 RemoteControlAcrobatView.h : interface of the CRemoteControlAcrobatView class

*********************************************************************/

#if !defined(AFX_REMOTECONTROLACROBATVIEW_H__A8DF2730_0A2C_420F_AB31_DCDA9E19CE2F__INCLUDED_)
#define AFX_REMOTECONTROLACROBATVIEW_H__A8DF2730_0A2C_420F_AB31_DCDA9E19CE2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// classes in Acrobat IAC
class CAcroApp;
class CAcroAVDoc;
class CAcroAVPageView;


class CRemoteControlAcrobatView : public CView
{
protected: // create from serialization only
	CRemoteControlAcrobatView();
	DECLARE_DYNCREATE(CRemoteControlAcrobatView)

// Attributes
public:
	CRemoteControlAcrobatDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteControlAcrobatView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRemoteControlAcrobatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Acrobat IAC objects
	CAcroApp *m_pAcroApp;
	CAcroAVDoc *m_pAvdoc;
	CAcroAVPageView *m_pPageView;	

	// utility function
	bool PDFDocIsActive();
	
// Generated message map functions
protected:
	//{{AFX_MSG(CRemoteControlAcrobatView)
	afx_msg void OnAcrobatAppHide();
	afx_msg void OnUpdateAcrobatAppHide(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatAppShow();
	afx_msg void OnUpdateAcrobatAppShow(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatLaunch();
	afx_msg void OnAcrobatExit();
	afx_msg void OnUpdateAcrobatExit(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatAvdocOpen();
	afx_msg void OnAcrobatAvdocPrint();
	afx_msg void OnUpdateAcrobatAvdocPrint(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatPddocClosepdf();
	afx_msg void OnUpdateAcrobatPddocClosepdf(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatPddocOpenpdf();
	afx_msg void OnAcrobatPdpageCrop();
	afx_msg void OnUpdateAcrobatPdpageCrop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAcrobatAvdocClose(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatAvdocCloseall();
	afx_msg void OnUpdateAcrobatAvdocCloseall(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatAvdocClose();
	afx_msg void OnUpdateAcrobatLaunch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAcrobatAvdocOpen(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatAvpageviewGoback();
	afx_msg void OnUpdateAcrobatAvpageviewGoback(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatAvpageviewGoforward();
	afx_msg void OnUpdateAcrobatAvpageviewGoforward(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatAvpageviewPagenumber();
	afx_msg void OnUpdateAcrobatAvpageviewPagenumber(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatAvpageviewGotopage();
	afx_msg void OnUpdateAcrobatAvpageviewGotopage(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatPdpageNum();
	afx_msg void OnUpdateAcrobatPdpageNum(CCmdUI* pCmdUI);
	afx_msg void OnAcrobatAvdocFindtext();
	afx_msg void OnUpdateAcrobatAvdocFindtext(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RemoteControlAcrobatView.cpp
inline CRemoteControlAcrobatDoc* CRemoteControlAcrobatView::GetDocument()
   { return (CRemoteControlAcrobatDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTECONTROLACROBATVIEW_H__A8DF2730_0A2C_420F_AB31_DCDA9E19CE2F__INCLUDED_)
