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

 ZoomSeto.cpp : Implementation file.

*********************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "zoomset.h"
#include "mainfrm.h"
#include "staticview.h"
#include "staticviewvw.h"
#include "zoomseto.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ZoomSetOther dialog

ZoomSetOther::ZoomSetOther(CWnd* pParent /*=NULL*/)
	: CDialog(ZoomSetOther::IDD, pParent)
{
	//{{AFX_DATA_INIT(ZoomSetOther)
	m_NewValue = ((CStaticViewDoc *)((CStaticViewVw *)((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetLastView())->GetDocument())->GetScale();
	//}}AFX_DATA_INIT
}

void ZoomSetOther::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ZoomSetOther)
	DDX_Text(pDX, IDC_EDIT1, m_NewValue);
	DDV_MinMaxInt(pDX, m_NewValue, 9, 1600);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ZoomSetOther, CDialog)
	//{{AFX_MSG_MAP(ZoomSetOther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ZoomSetOther message handlers

void ZoomSetOther::OnOK()
{
	UpdateData(TRUE);
	EndDialog(m_NewValue);
}
