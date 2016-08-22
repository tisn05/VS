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

 StaticViewDoc.h : Interface of the CStaticViewvDoc class.

*********************************************************************/

#ifndef	_STATICVIEWDOC_H
#define _STATICVIEWDOC_H

class CAcroApp;
class CAcroPDDoc;
class CAcroAVDoc;
class CAcroPDPage;

class CStaticViewDoc : public CDocument
{
protected: // create from serialization only
	CStaticViewDoc();
	DECLARE_DYNCREATE(CStaticViewDoc)

// Attributes
public:
protected:
	unsigned long 	NumPages;		// stores # of pages in this document
	unsigned long 	CurPage;        // keeps track of current page #
	int				ViewMode;		// keeps track of view sizing mode
	short			nScale;			// current scale percentage for doc
	int				OldWidth; 		// keeps last width of window
	int 			OldHeight;		// keeps last height of window

// Operations
public:
	unsigned long GetCurPage() { return(CurPage); }
	CAcroApp* 		m_pAcroApp;
	CAcroPDPage* 	m_pAcroPDPage;
	CAcroPDDoc* 	m_pAcroPDDoc;
	CAcroAVDoc* 	m_pAcroAVDoc;

	void Draw(CDC *pDC, HWND m_hWnd, CPoint Origin);
	void GotoPage(unsigned long NewPage);
	void SetScale(int NewScale);
	int GetScale(void);
	void SetToolbarStatusBoxes();
	CSize GetScaledDocSize(int width, int height);
    void CalculateScale(int width, int height);

// Implementation
public:
	virtual ~CStaticViewDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnOpenDocument(const char* pszPathName);
	virtual BOOL OnSaveDocument(const char* pszPathName);

// Generated message map functions
protected:
	//{{AFX_MSG(CStaticViewDoc)
	afx_msg void OnUpdateViewFirstpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewLastpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewNextpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPrevpage(CCmdUI* pCmdUI);
	afx_msg void OnViewFirstpage();
	afx_msg void OnViewLastpage();
	afx_msg void OnViewNextpage();
	afx_msg void OnViewPrevpage();
	afx_msg void OnViewActualsize();
	afx_msg void OnViewFitpage();
	afx_msg void OnViewFitwidth();
	afx_msg void OnUpdateViewActualsize(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewFitpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewFitwidth(CCmdUI* pCmdUI);
	afx_msg void OnZoomButton();
	afx_msg void OnOtherCopytoclipboard();
	afx_msg void OnFilePrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CStaticViewDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};



#endif
/////////////////////////////////////////////////////////////////////////////
