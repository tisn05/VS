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

 RemoteControlAcrobat.h : main header file for the REMOTECONTROLACROBAT application

*********************************************************************/

#if !defined(AFX_REMOTECONTROLACROBAT_H__AC926A6E_28B6_4DCC_8EB5_48FB0FCE834E__INCLUDED_)
#define AFX_REMOTECONTROLACROBAT_H__AC926A6E_28B6_4DCC_8EB5_48FB0FCE834E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "RemoteControlAcrobat_i.h"

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatApp:
// See RemoteControlAcrobat.cpp for the implementation of this class
//

class CRemoteControlAcrobatApp : public CWinApp
{
public:
	CRemoteControlAcrobatApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteControlAcrobatApp)
	public:
	virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRemoteControlAcrobatApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTECONTROLACROBAT_H__AC926A6E_28B6_4DCC_8EB5_48FB0FCE834E__INCLUDED_)
