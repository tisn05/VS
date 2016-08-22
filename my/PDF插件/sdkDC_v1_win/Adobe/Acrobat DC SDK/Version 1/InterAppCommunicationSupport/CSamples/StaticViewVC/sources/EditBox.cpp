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

 EditBox.cpp

*********************************************************************/

#include "stdafx.h"
#include "editbox.h"
#include "mainfrm.h"
#include "staticviewdoc.h"
#include "staticviewvw.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditBox

CEditBox::CEditBox()
{
}

CEditBox::~CEditBox()
{
}


BEGIN_MESSAGE_MAP(CEditBox, CWnd)
	//{{AFX_MSG_MAP(CEditBox)
	ON_WM_CHAR()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditBox message handlers

void CEditBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN) {
        CString EnteredText;
		GetWindowText(EnteredText);
		unsigned long entered_val = atol((const char *) EnteredText);
        ((CStaticViewDoc *)((CStaticViewVw *)((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetLastView())->GetDocument())->GotoPage(entered_val-1);
		((CMainFrame *)AfxGetApp()->m_pMainWnd)->SetFocus();
	}
	else
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

UINT CEditBox::OnGetDlgCode()
{
	return CEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
}


