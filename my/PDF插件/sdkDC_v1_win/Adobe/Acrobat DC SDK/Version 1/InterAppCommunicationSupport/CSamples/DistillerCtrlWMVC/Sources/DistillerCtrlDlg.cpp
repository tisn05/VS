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

 DistillerCtrlDlg.cpp : Implementation file.

*********************************************************************/

#include "stdafx.h"
#include "DistillerCtrl.h"
#include "DistillerCtrlDlg.h"
#include "DistCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Macro to help clean up the registry access code
#define THROW_ON_ERROR(X) if (ERROR_SUCCESS != X) throw X

/////////////////////////////////////////////////////////////////////////////
// CDistillerCtrlDlg dialog

CDistillerCtrlDlg::CDistillerCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDistillerCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDistillerCtrlDlg)
	m_inputFile = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hProcess = m_pDistillerWnd = NULL;
	m_enqueueOption = 0;
}

void CDistillerCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDistillerCtrlDlg)
	DDX_Text(pDX, IDC_INPUTFILELABEL, m_inputFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDistillerCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CDistillerCtrlDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTINPUT, OnSelectInput)
	ON_BN_CLICKED(IDC_PROCESSFILE, OnProcessFile)
	ON_BN_CLICKED(IDC_ENQUEUE1, OnNoInteraction)
	ON_BN_CLICKED(IDC_ENQUEUE2, OnSourceLocation)
	ON_BN_CLICKED(IDC_ENQUEUE3, OnMostRecentLocation)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDistillerCtrlDlg message handlers

BOOL CDistillerCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize the input file string
	m_inputFile = "File not yet selected.";
	UpdateData (FALSE);

	// Initialize the radio button set
	CButton *pBtn = reinterpret_cast<CButton *>(this->GetDlgItem (IDC_ENQUEUE1));
	pBtn->SetCheck (1);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDistillerCtrlDlg::OnPaint()
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
HCURSOR CDistillerCtrlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDistillerCtrlDlg::OnSelectInput()
{
	DWORD flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_LONGNAMES;
	char filter[] = "PostScript Files (*.ps)|*.ps|EPS Files (*.eps)|*.eps||";

	// Display a standard file open dialog to allow user to select input file
	CFileDialog fd(TRUE, "*.ps", "*.ps", flags, filter, this);

	if (fd.DoModal() == IDOK) {

		// Enable "Process File" button if need be
		GetDlgItem(IDC_PROCESSFILE)->EnableWindow();

		// Save path and update dialog
		m_inputFile = fd.GetPathName();
		UpdateData (FALSE);
	}
}

void CDistillerCtrlDlg::OnProcessFile()
{
	WORD res=0;

	long lRetCode;
	char regKey[] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\AcroDist.exe";
	HKEY hkey;
	BYTE *buf = NULL;
	DWORD bufSize = 0;
	DWORD type;

	// Update UI now that we're processing
	SetWindowText("Distiller Control - Processing");
	GetDlgItem(IDC_PROCESSFILE)->EnableWindow(FALSE);

	if (NULL == m_hProcess) {

		try {

			// Retrieve the path to the Distiller executable from the registry
			lRetCode = RegOpenKeyEx (HKEY_LOCAL_MACHINE, regKey, 0, KEY_READ, &hkey);
			THROW_ON_ERROR(lRetCode);

			// Query for the length of the path
			lRetCode = RegQueryValueEx (hkey, "", 0, &type, NULL, &bufSize);
			THROW_ON_ERROR(lRetCode);

			// Allocate memory for path and grab it
			buf = new BYTE[bufSize];
			lRetCode = RegQueryValueEx (hkey, "", 0, &type, buf, &bufSize);
			THROW_ON_ERROR(lRetCode);

			// Initialize CreateProcess input/output params
			STARTUPINFO startupInfo;
			PROCESS_INFORMATION processInfo;

			memset (&startupInfo, 0, sizeof(STARTUPINFO));
			memset (&processInfo, 0, sizeof(PROCESS_INFORMATION));

			startupInfo.cb = sizeof(STARTUPINFO);
			startupInfo.dwFlags = STARTF_USESHOWWINDOW;
			startupInfo.wShowWindow = SW_HIDE;

			// Launch Distiller
			CreateProcess (NULL, (char *)buf, NULL, NULL, FALSE, CREATE_NEW_PROCESS_GROUP,
							NULL, NULL, &startupInfo, &processInfo);

			// Store process info
			m_hProcess = processInfo.hProcess;

			// Free memory for path
			delete[] buf;

		} catch (long errCode) {

			// Free memory for path if we allocated it
			if (NULL != buf)
				delete[] buf;

			void *lpMsgBuf;
			FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
							NULL, errCode ? lRetCode : GetLastError(),
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							(LPTSTR)&lpMsgBuf, 128, NULL);

			// Display error then free the buffer
			MessageBox ((LPCTSTR)lpMsgBuf, "Error : Registry Access", MB_OK | MB_ICONEXCLAMATION);
			LocalFree (lpMsgBuf);
		}

		if (NULL == m_hProcess) {

			// We can't continue from here....so we die
			MessageBox ("Failed to launch the Distiller application. Aborting...", "Fatal Error");
			ExitProcess (-1);
		}
	}

	DISTILLRECORD dr; 					/* from distctrl.h */
	COPYDATASTRUCT cds;

	if (NULL == m_pDistillerWnd) {

		// Allow the Distiller process some startup time
		const DWORD MAX_WAIT_PERIOD = 2000;
		const DWORD WAIT_INCREMENT = 200;
		DWORD dwWaiting = 0;

		do {
			// Get handle on the Distiller window
			m_pDistillerWnd = FindWindow("Distiller", NULL);
			if ((NULL != m_pDistillerWnd) || (dwWaiting > MAX_WAIT_PERIOD))
				break;

			Sleep (dwWaiting += WAIT_INCREMENT);
		}
		while (true);
	}

	if (NULL != m_pDistillerWnd) {

		// Sync class members with the dialog items
		UpdateData();

		// Fill in the DISTILLRECORD structure and place it in the COPYDATASTRUCT.
		// If we're doing silent conversion, we default the output file to the
		// folder containing AcroDist.exe
		memset (&dr, 0, sizeof(DISTILLRECORD));
		strcpy(dr.fileList, m_inputFile);
		if (EQ_NO_SAVE_DIALOG == (dr.param = m_enqueueOption))
			strcpy(dr.outputFile, "Output.pdf");

		cds.dwData = DM_DISTILL;
		cds.cbData = sizeof(DISTILLRECORD);
		cds.lpData = &dr;

		if (m_pDistillerWnd->SendMessage (WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)&cds))
			m_pDistillerWnd->PostMessage (WM_TIMER, ID_TIMER, 0L); // Wake up Distiller

	} else {

		// We can't continue from here....so we die
		MessageBox ("Failed to acquire handle to the Distiller window. Aborting...", "Fatal Error");
		ExitProcess (-1);
	}
}

void CDistillerCtrlDlg::OnNoInteraction()
{
	m_enqueueOption = EQ_NO_SAVE_DIALOG;
}

void CDistillerCtrlDlg::OnSourceLocation()
{
	m_enqueueOption = EQ_DEFAULT_SOURCE;
}

void CDistillerCtrlDlg::OnMostRecentLocation()
{
	m_enqueueOption = EQ_DEFAULT_OLD_DEST;
}

BOOL CDistillerCtrlDlg::DestroyWindow()
{
	// Kill the Distiller process if we started one
	if (NULL != m_hProcess)
		TerminateProcess (m_hProcess, 0);

	return CDialog::DestroyWindow();
}

BOOL CDistillerCtrlDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// Handle the job done message
	if (DM_DONE == pCopyDataStruct->dwData) {

		char buf[64];
		PDISTILLRECORD pDistRec = (DISTILLRECORD *)pCopyDataStruct->lpData;

		// Pop ourselves to the front
		this->SetForegroundWindow();

		if (0 == pDistRec->param) {

			// File distilled successfully
			MessageBox ("Successfully processed file.", "Status");

		} else {

			// Error occurred.
			sprintf(buf, "Error (%d) occurred during processing.", pDistRec->param);
			MessageBox (buf, "Status");
		}

		// Distiller may have shown itself. Hide its window.
		m_pDistillerWnd->ShowWindow(SW_HIDE);

		// Reset Update UI now that we've finished processing
		m_inputFile = "";
		SetWindowText ("Distiller Control - Done");
		GetDlgItem(IDC_PROCESSFILE)->EnableWindow();
		return TRUE;
	}

	// Delegate to the base class
	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}
