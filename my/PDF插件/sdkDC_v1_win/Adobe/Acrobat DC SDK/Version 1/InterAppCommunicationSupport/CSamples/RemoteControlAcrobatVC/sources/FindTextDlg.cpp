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

 FindTextDlg.cpp : MFC Dialog implementation file

*********************************************************************/

#include "stdafx.h"
#include "RemoteControlAcrobat.h"
#include "FindTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindTextDlg dialog


FindTextDlg::FindTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FindTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(FindTextDlg)
	m_szFindWord = _T("");
	m_szFileName = _T("");
	//}}AFX_DATA_INIT
}


void FindTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindTextDlg)
	DDX_Text(pDX, IDC_EDIT_WORD, m_szFindWord);
	DDX_Text(pDX, IDC_STATIC_FILENAME, m_szFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindTextDlg, CDialog)
	//{{AFX_MSG_MAP(FindTextDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindTextDlg message handlers
