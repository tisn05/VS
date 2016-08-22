/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PdfEventSink.cpp : Implementation file.

*********************************************************************/

#include "stdafx.h"
#include "Resource.h"
#include "PdfEventSink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdfEventSink

IMPLEMENT_DYNCREATE(CPdfEventSink, CCmdTarget)

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction

CPdfEventSink::CPdfEventSink ()
	: m_dwCookie(0),
	  m_pSrcConnection(NULL),
	  m_pParent(0),
	  m_bWorking(FALSE)
{
	EnableAutomation();
}

CPdfEventSink::~CPdfEventSink ()
{
}


void CPdfEventSink::OnFinalRelease ()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BOOL CPdfEventSink::RegisterConnection (struct IUnknown *pSrcUnk)
{
	HRESULT hRes;
	IConnectionPointContainer *pContainer;

	// Register our sink interface with Distiller.
	hRes = pSrcUnk->QueryInterface (IID_IConnectionPointContainer, reinterpret_cast<void **>(&pContainer));
	if (SUCCEEDED(hRes)) {

		hRes = pContainer->FindConnectionPoint (__uuidof(_PdfEvents), &m_pSrcConnection);
		if (SUCCEEDED(hRes))
			m_pSrcConnection->Advise (this->GetIDispatch(FALSE), &m_dwCookie);

		ULONG ref = pContainer->Release();
	}

	return (0 != m_dwCookie);
}

void CPdfEventSink::UnregisterConnection ()
{
	if (0 != m_dwCookie) {

		// Detach sink from source
		m_pSrcConnection->Unadvise (m_dwCookie);
		m_dwCookie = 0;
	}

	if (NULL != m_pSrcConnection) {

		// Release connection point
		ULONG ref = m_pSrcConnection->Release();
		m_pSrcConnection = NULL;
	}

	delete this;
}

BEGIN_MESSAGE_MAP(CPdfEventSink, CCmdTarget)
	//{{AFX_MSG_MAP(CPdfEventSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPdfEventSink, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CPdfEventSink)
	DISP_FUNCTION(CPdfEventSink, "OnLogMessage", OnLogMessage, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CPdfEventSink, "OnJobStart", OnJobStart, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CPdfEventSink, "OnJobDone", OnJobDone, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CPdfEventSink, "OnJobFail", OnJobFail, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CPdfEventSink, "OnPercentDone", OnPercentDone, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CPdfEventSink, "OnPageNumber", OnPageNumber, VT_EMPTY, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CPdfEventSink, CCmdTarget)
	INTERFACE_PART(CPdfEventSink, __uuidof(_PdfEvents), Dispatch)
END_INTERFACE_MAP()

//IID_IPdfEventSink

/////////////////////////////////////////////////////////////////////////////
// CPdfEventSink message handlers


void CPdfEventSink::OnLogMessage(LPCTSTR strMessage)
{
}

void CPdfEventSink::OnJobStart(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF)
{
}

void CPdfEventSink::OnJobDone(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF)
{
}

void CPdfEventSink::OnJobFail(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF)
{
}

void CPdfEventSink::OnPercentDone(long nPercentDone)
{
	if (NULL != m_pParent) {

		if (nPercentDone > 0)
			m_bWorking = true;

		if (m_bWorking) {

			CString label;
			CStatic *dlgLabel;

			// Update the progress bar.
			CProgressCtrl *progBar = (CProgressCtrl *)m_pParent->GetDlgItem (IDC_STATUS_PROGBAR);
			progBar->SetPos (nPercentDone);

			if (nPercentDone == 0) {

				// Reset the UI labels.
				dlgLabel = (CStatic *)m_pParent->GetDlgItem (IDC_STATUS_PAGECOUNT);
				dlgLabel->SetWindowText (label);

				dlgLabel = (CStatic *)m_pParent->GetDlgItem (IDC_STATUS_PERCENT);
				dlgLabel->SetWindowText (label);

				label = "Idle";
				dlgLabel = (CStatic *)m_pParent->GetDlgItem (IDC_STATUS_LABEL);
				dlgLabel->SetWindowText (label);

				m_bWorking = false;

			} else {

				// Update the UI label.
				label.Format ("%d%%", nPercentDone);

				dlgLabel = (CStatic *)m_pParent->GetDlgItem (IDC_STATUS_PERCENT);
				dlgLabel->SetWindowText (label);
			}
		}
	}
}

void CPdfEventSink::OnPageNumber(long nPageNumber)
{
	if (NULL != m_pParent) {

		if (m_bWorking) {

			CString label;
			CStatic *dlgLabel = (CStatic *)m_pParent->GetDlgItem (IDC_STATUS_PAGECOUNT);

			// Update UI label.
			label.Format ("page %d", nPageNumber);
			dlgLabel->SetWindowText (label);
		}
	}
}

