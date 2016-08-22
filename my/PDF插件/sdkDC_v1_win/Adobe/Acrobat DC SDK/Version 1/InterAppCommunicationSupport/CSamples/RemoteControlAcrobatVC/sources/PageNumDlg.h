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

 PageNumDlg.h : MFC Dialog header file

*********************************************************************/
#if !defined(AFX_PageNumDlg_H__61740213_365B_4AEE_977E_E74E17A6F147__INCLUDED_)
#define AFX_PageNumDlg_H__61740213_365B_4AEE_977E_E74E17A6F147__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageNumDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PageNumDlg dialog

class PageNumDlg : public CDialog
{
// Construction
public:
	PageNumDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PageNumDlg)
	enum { IDD = IDD_DIALOG2 };
	CString	m_szFileName;
	int	m_szPageNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PageNumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PageNumDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDTEXTDLG_H__61740213_365B_4AEE_977E_E74E17A6F147__INCLUDED_)
