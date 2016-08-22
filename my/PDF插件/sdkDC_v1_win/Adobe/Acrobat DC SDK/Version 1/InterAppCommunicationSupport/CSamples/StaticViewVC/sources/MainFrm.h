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

 MainFrm.h : Interface of the CMainFrame class.

*********************************************************************/

#ifndef	_MAINFRM_H
#define	_MAINFRM_H

#include "editbox.h"
#include "staticviewvw.h"
#include "acrobat.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
 	CEditBox		*GetEditBox() 	{ return(&m_wndEditBox); }
    CStatic			*GetEditLabel() { return(&m_wndStaticLabel); }
    CButton			*GetZoomButton(){ return(&m_wndZoomButton); }
	CStaticViewVw 	*GetLastView() 	{ return(LastView); }
	void			ShowToolbarAddOns(BOOL ShowOrHide);
	void			SetLastView(CStaticViewVw *LV) { LastView = LV; }
	int				GetNumViews() 	{ return(NumViews); }
	void			IncNumViews() 	{ NumViews++; }
	void			DecNumViews() 	{ NumViews--; }

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CEditBox    m_wndEditBox;     // edit box in the toolbar
	CStatic		m_wndStaticLabel; // label in the toolbar
	CButton		m_wndZoomButton;

	CAcroApp	*MainAcroApp;	 // AcroApp kept open as long as Drawer
								 // running to prevent having to be opened again

	int 		NumViews;        // number of open views
	CStaticViewVw *LastView;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
/////////////////////////////////////////////////////////////////////////////
