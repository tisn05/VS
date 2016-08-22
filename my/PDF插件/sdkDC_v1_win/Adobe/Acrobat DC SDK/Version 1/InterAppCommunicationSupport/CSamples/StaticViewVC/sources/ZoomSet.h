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

 ZoomSet.h

*********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CZoomSet dialog

#ifndef _ZOOMSET_H
#define _ZOOMSET_H


#define ZOOM_9			9
#define ZOOM_13		13
#define ZOOM_25		25
#define ZOOM_50		50
#define ZOOM_100		100
#define ZOOM_125		125
#define ZOOM_150		150
#define ZOOM_200		200
#define ZOOM_400		400
#define ZOOM_800		800
#define ZOOM_1600		1600
#define ZOOM_OTHER  	1

class CZoomSet : public CDialog
{
// Construction
public:
	CZoomSet(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CZoomSet)
	enum { IDD = IDD_ZOOMSET };
	CListBox	m_ZoomList;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL CZoomSet::OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CZoomSet)
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
