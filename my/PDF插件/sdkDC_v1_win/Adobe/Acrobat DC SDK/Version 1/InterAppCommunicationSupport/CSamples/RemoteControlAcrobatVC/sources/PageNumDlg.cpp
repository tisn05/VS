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

 PageNumDlg.cpp : MFC Dialog implementation file

*********************************************************************/

#include "stdafx.h"
#include "RemoteControlAcrobat.h"
#include "PageNumDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PageNumDlg dialog


PageNumDlg::PageNumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PageNumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PageNumDlg)
	m_szFileName = _T("");
	m_szPageNum = 1;
	//}}AFX_DATA_INIT
}


void PageNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PageNumDlg)
	DDX_Text(pDX, IDC_STATIC_FILENAME, m_szFileName);
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_szPageNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PageNumDlg, CDialog)
	//{{AFX_MSG_MAP(PageNumDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PageNumDlg message handlers
