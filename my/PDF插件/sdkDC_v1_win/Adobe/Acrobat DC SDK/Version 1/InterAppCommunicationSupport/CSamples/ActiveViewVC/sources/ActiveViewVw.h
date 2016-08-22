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

 ActiveViewVw.h : Interface of the CActiveViewVw class.

*********************************************************************/

#ifndef	_CVIEWVW_H
#define _CVIEWVW_H

#include "activeviewdoc.h"

class CActiveViewVw : public CView
{
protected: // create from serialization only
	CActiveViewVw();
	DECLARE_DYNCREATE(CActiveViewVw)

// Attributes
public:
	CActiveViewDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CActiveViewVw();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	afx_msg void OnSysCommand( UINT nID, LPARAM lParam );

// Generated message map functions
protected:
	//{{AFX_MSG(CActiveViewVw)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnFilePrint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in cviewvw.cpp
inline CActiveViewDoc* CActiveViewVw::GetDocument()
   { return (CActiveViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCPrintDialog dialog

class CCPrintDialog : public CDialog
{
// Construction
public:
	CCPrintDialog(CWnd* pParent = NULL);	// standard constructor

// Attributes
public:
protected:
	long 	NumPages;		// stores # of pages in this document
	long	CurrPage;

// Dialog Data
	//{{AFX_DATA(CCPrintDialog)
	enum { IDD = IDD_PRINTDIALOG };
	BOOL	m_ShrinkToFit;
	BOOL	m_BinaryOK;
	int		m_PSLevel;
	UINT	m_FromPage;
	UINT	m_ToPage;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCPrintDialog)
	virtual void OnOK();
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnKillfocusEdit2();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
/////////////////////////////////////////////////////////////////////////////
