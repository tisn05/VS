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

 StaticViewVw.h : Interface of the CStaticViewVw class.

*********************************************************************/

#ifndef	_DrawVW_H
#define _DrawVW_H

#include "staticviewdoc.h"

class CStaticViewVw : public CScrollView
{
protected: // create from serialization only
	CStaticViewVw();
	DECLARE_DYNCREATE(CStaticViewVw)

// Attributes
public:
	CStaticViewDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CStaticViewVw();
	virtual void OnDraw(CDC* pDC);  // overridden to Draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void OnInitialUpdate();
	void OnUpdate(CView *, LPARAM, CObject *);
protected:
	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CStaticViewVw)
	afx_msg void OnUpdatePageEdit(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in StaticViewVw.cpp
inline CStaticViewDoc* CStaticViewVw::GetDocument()
   { return (CStaticViewDoc*)m_pDocument; }
#endif

#endif
/////////////////////////////////////////////////////////////////////////////
