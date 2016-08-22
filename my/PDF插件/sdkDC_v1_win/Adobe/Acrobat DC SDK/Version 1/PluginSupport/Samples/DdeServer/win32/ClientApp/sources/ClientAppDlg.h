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

 ClientAppDlg.h

 - Dialog header file.

*********************************************************************/

#if !defined(AFX_CLIENTAPPDLG_H__11DE0515_F0FD_41EC_BDE1_7D7172971E2E__INCLUDED_)
#define AFX_CLIENTAPPDLG_H__11DE0515_F0FD_41EC_BDE1_7D7172971E2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientAppDlg dialog

class CClientAppDlg : public CDialog
{
// Construction
public:
	CClientAppDlg(CWnd* pParent = NULL);	// standard constructor

	// Need a DDE callback, even if there's nothing to do.
	static HDDEDATA CALLBACK DdeCallback (UINT type, UINT format, HCONV hconv, HSZ hString1, HSZ hString2, HDDEDATA hData, DWORD dwData1, DWORD dwData2);

// Dialog Data
	//{{AFX_DATA(CClientAppDlg)
	enum { IDD = IDD_CLIENTAPP_DIALOG };
	CString	m_annotContent;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAddNote();
	afx_msg void OnGetPageNum();
	afx_msg void OnTextChange();
	afx_msg void OnPerformOp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTAPPDLG_H__11DE0515_F0FD_41EC_BDE1_7D7172971E2E__INCLUDED_)
