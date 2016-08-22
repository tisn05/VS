/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PdfEventSink.h : Header file.

*********************************************************************/

#if !defined(AFX_PDFEVENTSINK_H__0893A121_F23F_11D4_B82C_00C04F311E65__INCLUDED_)
#define AFX_PDFEVENTSINK_H__0893A121_F23F_11D4_B82C_00C04F311E65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "AcroDistX.tlb" no_namespace

/////////////////////////////////////////////////////////////////////////////
// CPdfEventSink command target

class CPdfEventSink : public CCmdTarget
{
	DECLARE_DYNCREATE(CPdfEventSink)

	CPdfEventSink ();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void SetParentDialog (CDialog *pParent)
	{ m_pParent = pParent; }

	BOOL RegisterConnection (struct IUnknown *pSrcUnk);
	void UnregisterConnection ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPdfEventSink)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bWorking;
	DWORD m_dwCookie;
	IConnectionPoint *m_pSrcConnection;
	CDialog *m_pParent;

	virtual ~CPdfEventSink ();

	// Generated message map functions
	//{{AFX_MSG(CPdfEventSink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CPdfEventSink)
	afx_msg void OnLogMessage(LPCTSTR strMessage);
	afx_msg void OnJobStart(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF);
	afx_msg void OnJobDone(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF);
	afx_msg void OnJobFail(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF);
	afx_msg void OnPercentDone(long nPercentDone);
	afx_msg void OnPageNumber(long nPageNumber);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDFEVENTSINK_H__0893A121_F23F_11D4_B82C_00C04F311E65__INCLUDED_)
