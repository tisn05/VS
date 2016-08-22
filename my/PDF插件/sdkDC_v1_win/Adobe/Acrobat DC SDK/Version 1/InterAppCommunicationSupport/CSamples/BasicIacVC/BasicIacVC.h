// BasicIacVc.h : main header file for the BasicIacVc application
//

#if !defined(AFX_BasicIacVc_H__0DA12561_1E32_4AEC_9E42_D2192420C6FE__INCLUDED_)
#define AFX_BasicIacVc_H__0DA12561_1E32_4AEC_9E42_D2192420C6FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBasicIacVcApp:
// See BasicIacVc.cpp for the implementation of this class
//

class CBasicIacVcApp : public CWinApp
{
public:
	CBasicIacVcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasicIacVcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBasicIacVcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BasicIacVc_H__0DA12561_1E32_4AEC_9E42_D2192420C6FE__INCLUDED_)
