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

 DistillerCtrlDlg.cpp : Implementation file.

*********************************************************************/

#include "stdafx.h"
#include "DistillerCtrl.h"
#include "DistillerCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	m_pPdfEventSink = NULL;
}

void CDistillerCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDistillerCtrlDlg)
	DDX_Text(pDX, IDC_INPUTFILE, m_inputFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDistillerCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CDistillerCtrlDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTINPUT, OnSelectInput)
	ON_BN_CLICKED(IDC_PROCESSFILE, OnProcessFile)
	ON_BN_CLICKED(IDOK, OnQuit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDistillerCtrlDlg message handlers

BOOL CDistillerCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
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

	// Display a standard file open dialog to allow user to select input file.
	CFileDialog fd(TRUE, "*.ps", "*.ps", flags, filter, this);

	if (fd.DoModal() == IDOK) {

		// Enable "Process File" button.
		GetDlgItem(IDC_PROCESSFILE)->EnableWindow();

		// Save path and update dialog.
		m_inputFile = fd.GetPathName();
		UpdateData (FALSE);
	}
}

void CDistillerCtrlDlg::OnProcessFile()
{
	// Create the instance of the Distiller app, if necessary
	if ((NULL == m_pDistApp) && (!InitializeAutomation())) {

		AfxMessageBox ("Failed to initialize connection with Distiller. Aborting...");
		this->OnQuit();

	} else {

		// Update the UI labels.
		CStatic *dlgLabel = (CStatic *)GetDlgItem (IDC_STATUS_LABEL);
		dlgLabel->SetWindowText ("Processing");

		dlgLabel = (CStatic *)GetDlgItem (IDC_STATUS_PAGECOUNT);
		dlgLabel->SetWindowText ("page 0");

		// Process the file.
		if (m_pDistApp->FileToPDF ((LPCTSTR)m_inputFile, "", "Standard.joboptions")) {

			SetForegroundWindow();
			AfxMessageBox ("Successfully processed file.");

		} else {

			// Reset the UI labels.
			dlgLabel->SetWindowText ("");
			dlgLabel = (CStatic *)GetDlgItem (IDC_STATUS_LABEL);
			dlgLabel->SetWindowText ("Idle");

			SetForegroundWindow();
			AfxMessageBox ("Error processing file.");
		}
	}
}

BOOL CDistillerCtrlDlg::InitializeAutomation()
{
	HRESULT hRetVal;

	// Instantiate the automation server
	hRetVal = CoCreateInstance (__uuidof(PdfDistiller), NULL, CLSCTX_SERVER,
								__uuidof(IPdfDistiller), reinterpret_cast<void **>(&m_pDistApp));

	if (!SUCCEEDED(hRetVal))
		return FALSE;

	// Create and register the event sink
	if (NULL == m_pPdfEventSink)
		m_pPdfEventSink = new CPdfEventSink();

	m_pPdfEventSink->SetParentDialog (this);
	return m_pPdfEventSink->RegisterConnection (m_pDistApp);
}


void CDistillerCtrlDlg::OnQuit()
{
	// Allow the event sink to clean up
	if (NULL != m_pPdfEventSink)
		m_pPdfEventSink->UnregisterConnection();

	CDialog::OnOK();
}
