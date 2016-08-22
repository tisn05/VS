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

 ActiveViewDoc.h : Interface of the CActiveViewDoc class.

*********************************************************************/

#ifndef	_CVIEWDOC_H
#define _CVIEWDOC_H

class CAcroApp;
class CAcroAVDoc;
class CAcroPDPage;

class CActiveViewDoc : public CDocument
{
protected: // create from serialization only
	CActiveViewDoc();
	DECLARE_DYNCREATE(CActiveViewDoc)

// Attributes
public:
protected:
	unsigned long 	gNumPages;		// stores # of pages in this document
	CAcroAVDoc* 	m_pAcroAVDoc;

// Operations
public:
	void GotoPage(unsigned long NewPage);
    unsigned long GetCurrentPageNum();
	int GetCurrentZoomType();
	void SetCurrentZoomType(int NewType, int NewScale);
	void SetScale(int NewScale);
	void CloseAVDoc();
	inline CAcroAVDoc* CActiveViewDoc::GetAcroAVDoc()
		{ return (CAcroAVDoc*)m_pAcroAVDoc; }

// Implementation
public:
	virtual ~CActiveViewDoc();
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
	//{{AFX_MSG(CActiveViewDoc)
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
	afx_msg void OnUpdateViewFitpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewFitwidth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTools(CCmdUI* pCmdUI);
	afx_msg void OnToolsHand();
	afx_msg void OnToolsHilitewords();
	afx_msg void OnEditCopy();
	afx_msg void OnEditSelectall();
	afx_msg void OnEditSelectgraphic();
	afx_msg void OnEditSelecttext();
	afx_msg void OnToolsHilitebyrect();
	afx_msg void OnToolsAcquireBookmark();
	afx_msg void OnToolsAcquireAnnot();
	afx_msg void OnToolsAddTextAnnot();
	afx_msg void OnUpdateToolsHand(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectgraphic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelecttext(CCmdUI* pCmdUI);
	afx_msg void OnToolsDisplayaperture();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CActiveViewDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};

#endif
/////////////////////////////////////////////////////////////////////////////
