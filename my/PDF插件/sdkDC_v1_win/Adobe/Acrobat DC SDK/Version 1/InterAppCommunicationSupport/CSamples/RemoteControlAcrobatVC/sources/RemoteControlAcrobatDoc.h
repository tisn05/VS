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

 RemoteControlAcrobatDoc.h : interface of the CRemoteControlAcrobatDoc class

*********************************************************************/

#if !defined(AFX_REMOTECONTROLACROBATDOC_H__FE67B55D_DB61_4041_AE3C_A50AE22C6621__INCLUDED_)
#define AFX_REMOTECONTROLACROBATDOC_H__FE67B55D_DB61_4041_AE3C_A50AE22C6621__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRemoteControlAcrobatDoc : public CDocument
{
protected: // create from serialization only
	CRemoteControlAcrobatDoc();
	DECLARE_DYNCREATE(CRemoteControlAcrobatDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteControlAcrobatDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRemoteControlAcrobatDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRemoteControlAcrobatDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTECONTROLACROBATDOC_H__FE67B55D_DB61_4041_AE3C_A50AE22C6621__INCLUDED_)
