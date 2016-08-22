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

 EditBox.h : Interface of the CEditBox class.

*********************************************************************/

#ifndef   _EDITBOX_H
#define   _EDITBOX_H

#include "resource.h"

class CEditBox : public CEdit
{
// Construction
public:
	CEditBox();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CEditBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditBox)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
