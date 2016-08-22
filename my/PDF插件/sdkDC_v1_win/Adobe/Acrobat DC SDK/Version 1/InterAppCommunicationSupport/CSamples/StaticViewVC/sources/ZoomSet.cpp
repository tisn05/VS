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

 ZoomSet.cpp

*********************************************************************/

#include "stdafx.h"
#include "staticview.h"
#include "zoomset.h"
#include "mainfrm.h"
#include "staticviewdoc.h"
#include "staticviewvw.h"
#include "zoomseto.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

struct ZOOM_TYPES
{
	char*   pType;
	int     nType;
};

static ZOOM_TYPES zoomNames[] =
{
	"  9%",					ZOOM_9,
	"  13%",				ZOOM_13,
	"  25%",				ZOOM_25,
	"  50%",				ZOOM_50,
	" 100%",				ZOOM_100,
	" 125%",				ZOOM_125,
	" 150%",				ZOOM_150,
	" 200%",				ZOOM_200,
	" 400%",				ZOOM_400,
	" 800%",				ZOOM_800,
	" 1600%",				ZOOM_1600,
	" Other..",				ZOOM_OTHER,
	NULL,					0
};

/////////////////////////////////////////////////////////////////////////////
// CZoomSet dialog

CZoomSet::CZoomSet(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZoomSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CZoomSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomSet)
	DDX_Control(pDX, IDC_LIST1, m_ZoomList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CZoomSet, CDialog)
	//{{AFX_MSG_MAP(CZoomSet)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomSet message handlers

BOOL CZoomSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	for (ZOOM_TYPES* pType = zoomNames; pType->pType != NULL; pType++)
	{
		int index = m_ZoomList.AddString(pType->pType);
		m_ZoomList.SetItemData(index, pType->nType);
	}
	m_ZoomList.SelectString(-1, zoomNames[0].pType);
	return(TRUE);
}

void CZoomSet::OnSelchangeList1()
{
    // close dialog box
	EndDialog(IDOK);

	// get new scale value (stored in list)
	int NewScale = (int) m_ZoomList.GetItemData(m_ZoomList.GetCurSel());

    // if Other.. was selected, ..
	if (NewScale == 1) {
		int val;
		ZoomSetOther other;
		val = other.DoModal();
		if (val == IDCANCEL)		// return if user hit Cancel
			return;
		else {
			if (val < 9)
				val = 9;
			else if (val > 1600)
				val = 1600;
			NewScale = val;
		}
	}

	// update the scale
	((CStaticViewDoc *)((CStaticViewVw *)((CMainFrame *)AfxGetApp()->m_pMainWnd)->GetLastView())->GetDocument())->SetScale(NewScale);
}
