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

 RemoteControlAcrobatDoc.cpp : implementation of the CRemoteControlAcrobatDoc class

*********************************************************************/

#include "stdafx.h"
#include "RemoteControlAcrobat.h"

#include "RemoteControlAcrobatDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatDoc

IMPLEMENT_DYNCREATE(CRemoteControlAcrobatDoc, CDocument)

BEGIN_MESSAGE_MAP(CRemoteControlAcrobatDoc, CDocument)
	//{{AFX_MSG_MAP(CRemoteControlAcrobatDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatDoc construction/destruction

CRemoteControlAcrobatDoc::CRemoteControlAcrobatDoc()
{
	// TODO: add one-time construction code here

}

CRemoteControlAcrobatDoc::~CRemoteControlAcrobatDoc()
{
}

BOOL CRemoteControlAcrobatDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatDoc serialization

void CRemoteControlAcrobatDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatDoc diagnostics

#ifdef _DEBUG
void CRemoteControlAcrobatDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRemoteControlAcrobatDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRemoteControlAcrobatDoc commands
