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

 ZoomSeto.h

*********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// ZoomSetOther dialog

class ZoomSetOther : public CDialog
{
// Construction
public:
	ZoomSetOther(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(ZoomSetOther)
	enum { IDD = IDD_ZOOMSETOTHER };
	int		m_NewValue;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(ZoomSetOther)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
