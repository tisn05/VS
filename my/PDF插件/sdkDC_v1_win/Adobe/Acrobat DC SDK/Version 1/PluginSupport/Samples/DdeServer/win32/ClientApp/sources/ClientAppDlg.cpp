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

 ClientAppDlg.cpp

 - Dialog implementation file.

*********************************************************************/

#include "stdafx.h"
#include "ClientApp.h"
#include "ClientAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientAppDlg dialog

CClientAppDlg::CClientAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientAppDlg)
	m_annotContent = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientAppDlg)
	DDX_Text(pDX, IDC_TEXTEDIT, m_annotContent);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientAppDlg, CDialog)
	//{{AFX_MSG_MAP(CClientAppDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADDNOTECMD, OnAddNote)
	ON_BN_CLICKED(IDC_PAGENUMCMD, OnGetPageNum)
	ON_EN_CHANGE(IDC_TEXTEDIT, OnTextChange)
	ON_BN_CLICKED(IDC_PERFORMOP, OnPerformOp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientAppDlg message handlers

BOOL CClientAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize the radio button set.
	CButton *pButton = (CButton*)GetDlgItem (IDC_PAGENUMCMD);
	pButton->SetCheck (1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CClientAppDlg::OnAddNote()
{
	// Enable the edit control.
	CWnd *pWnd = GetDlgItem (IDC_TEXTEDIT);
	pWnd->EnableWindow ();

	// If there's no text in the textedit control,
	// disable the perform operation button.

	UpdateData();
	if (m_annotContent.IsEmpty()) {
		pWnd = GetDlgItem (IDC_PERFORMOP);
		pWnd->EnableWindow (FALSE);
	}
}

void CClientAppDlg::OnGetPageNum()
{
	// Disable the edit control.
	CWnd *pWnd = GetDlgItem (IDC_TEXTEDIT);
	pWnd->EnableWindow (FALSE);

	// Enable the Perform Operation button
	pWnd = GetDlgItem (IDC_PERFORMOP);
	pWnd->EnableWindow();
}

void CClientAppDlg::OnTextChange()
{
	// Enable the Perform Operation button if
	// the user has entered some text.
	UpdateData();

	CWnd *pWnd = GetDlgItem (IDC_PERFORMOP);
	pWnd->EnableWindow (!m_annotContent.IsEmpty());
}

void CClientAppDlg::OnPerformOp()
{
	// Attempt to connect to server.
	DWORD ddeInst = 0, dwResult;
	HCONV convId;
	HSZ hszServiceName, hszPageNumber;
	HDDEDATA retVal;

	if (DdeInitialize(&ddeInst, DdeCallback, APPCMD_CLIENTONLY, 0)) {

		MessageBox ("DdeClient: DDE Initialization failed.");
		return;
	}

	// Create DDE string handles and connect to server.
	hszServiceName = DdeCreateStringHandle (ddeInst, "SdkDdeService", CP_WINANSI);
	hszPageNumber = DdeCreateStringHandle (ddeInst, "PageNumber", CP_WINANSI);
	convId = DdeConnect (ddeInst, hszServiceName, hszPageNumber, NULL);

	if (!convId) {
		MessageBox ("DdeClient: Failed to connect to server.");
		return;
	}

	// Perform operation requested by user.
	CButton *pButton = (CButton*)GetDlgItem (IDC_PAGENUMCMD);
	if (pButton->GetCheck()) {

		// Perform the GetPageNumber request.
		retVal = DdeClientTransaction (0, 0, convId, hszPageNumber, CF_TEXT, XTYP_REQUEST, 4000, 0);
		if (retVal) {

			// Unwrap the response data and fill in the dialog box control.
			char *pageNumString = (char *)DdeAccessData (retVal, 0);
			SetDlgItemText (IDC_PAGENUM, pageNumString);
			DdeFreeDataHandle (retVal);

		} else
			MessageBox ("DdeClient: Request failed.");

	} else {

		CString cmdString;

		// Format the contents of the edit control into
		// the DDE command string.

		UpdateData();
		cmdString.Format ("AddNote(%s)", m_annotContent);

		// Perform the AddNote command.
		retVal = DdeClientTransaction ((unsigned char*)(LPCTSTR)cmdString, cmdString.GetLength(),
										convId, 0, 0, XTYP_EXECUTE, 4000, &dwResult);

		if (!retVal || (LOWORD(dwResult) != DDE_FACK))
			MessageBox ("DdeClient: Execute failed.");

	}

	// Terminate conversation and release DDE resources.
	DdeDisconnect(convId);
	DdeFreeStringHandle (ddeInst, hszServiceName);
	DdeFreeStringHandle (ddeInst, hszPageNumber);
	DdeUninitialize (ddeInst);
}

/////////////////////////////////////////////////////////////////////////////
// CClientAppDlg static methods

HDDEDATA CALLBACK CClientAppDlg::DdeCallback(UINT type, UINT format, HCONV hconv, HSZ hString1, HSZ hString2, HDDEDATA hData, DWORD dwData1, DWORD dwData2)
{
	return 0;
}
