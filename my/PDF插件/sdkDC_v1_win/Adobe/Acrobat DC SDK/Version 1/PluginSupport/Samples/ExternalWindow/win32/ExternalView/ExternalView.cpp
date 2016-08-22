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

 ExternalView.cpp

 - Simple application that exercises the IAC interface exposed by the
    ExternalWindow plug-in.

 - The application communicates with the plug-in through Windows Messaging
   to have the plug-in open a PDF document in the main window of our
   application.

*********************************************************************/

#include "stdafx.h"
#include "resource.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

static const int MAX_LOADSTRING = 100;
static bool bWindowIsOpen = false;
static HINSTANCE hInst;
static HWND msgWnd = NULL;
static HWND mainWnd;
static HANDLE hProcess = NULL;
static TCHAR szTitle[MAX_LOADSTRING];
static TCHAR szWindowClass[MAX_LOADSTRING];

/*-------------------------------------------------------
	Utility Methods
-------------------------------------------------------*/

void GetAcrobatPath (char *strPath, DWORD bufSize)
{
	HKEY hKey = NULL;
	LONG lResult;
	DWORD dwSize = 0;
	DWORD dwType;

	strPath[0] = 0;

	// Grab path to Acrobat.exe form the registry.
	lResult = RegOpenKeyEx (HKEY_CLASSES_ROOT, "Software\\Adobe\\Acrobat\\Exe", 0, KEY_READ, &hKey);
	if (lResult == ERROR_SUCCESS) {

		lResult = ::RegQueryValueEx (hKey, NULL, NULL, &dwType, NULL, &dwSize);
		if (lResult != ERROR_SUCCESS)
			return;

		if (dwSize > bufSize)
			return;

		lResult = ::RegQueryValueEx (hKey, NULL, NULL, &dwType, (LPBYTE)strPath, &dwSize);
 	}

	// To suppress the open file dialog when launching
	// Acrobat, we use the /o option. We also suppress
	// the splash screen.

	if (lResult == ERROR_SUCCESS && strlen(strPath))
		sprintf (strPath, "%s /o /s", strPath);
}

void ReleaseExternalPDF ()
{
	if (msgWnd) {

		HWND childWnd = ::GetWindow (mainWnd, GW_CHILD);
		if (childWnd)
			::SendMessage (childWnd, WM_CLOSE, NULL, NULL);
	}
}

void OpenPDFInExternalWindow ()
{
	OPENFILENAME params;
	TCHAR szFile[MAX_PATH] = "\0";

	// Fill in the OPENFILENAME structure.
	memset (&params, 0, sizeof(OPENFILENAME));
	params.lStructSize = sizeof(OPENFILENAME);
    params.hwndOwner = mainWnd;
    params.hInstance = hInst;
    params.lpstrFilter = "Adobe PDF Files\0*.pdf\0\0";
    params.lpstrFile = szFile;
    params.nMaxFile = MAX_PATH;
    params.lpstrTitle = "Select a PDF File";
    params.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;

	// Allow user to select a PDF document.
	if (!GetOpenFileName(&params))
		return;

	// Find the invisible message window created by the
	// ExternalWindow plug-in.

	if (!msgWnd)
		msgWnd = ::FindWindow("IACMessageWindow", NULL);

	if (msgWnd && strlen(szFile)) {

		// We're definitely going to try and open this PDF, so if
		// we have another one open, close it.

		if (bWindowIsOpen)
			ReleaseExternalPDF();

		COPYDATASTRUCT copyData;

		copyData.dwData = NULL;
		copyData.cbData = strlen(szFile);
		copyData.lpData = (PVOID)szFile;

		::SendMessage (msgWnd, WM_COPYDATA, (WPARAM)mainWnd, (LPARAM)&copyData);
	}
}


/*-------------------------------------------------------
	Entry Point/Message Handler
-------------------------------------------------------*/

/* WndProc
** ------------------------------------------------------
**
** Processes messages for the main window.
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;

	switch (message)
	{
		case WM_COMMAND:

			wmId    = LOWORD(wParam);

			// Parse the menu selections:
			switch (wmId)
			{
				case ID_FILE_OPEN:

					// Open a PDF document in our window.
					OpenPDFInExternalWindow();
					bWindowIsOpen = true;
					break;

				case ID_FILE_CLOSE:

					// If we have an open PDF, close it.
					if (bWindowIsOpen)
						ReleaseExternalPDF();

					bWindowIsOpen = false;
					break;

				case IDM_EXIT:

					// If we have an open PDF, close it.
					if (bWindowIsOpen)
						ReleaseExternalPDF();
					DestroyWindow(hWnd);
					break;

				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_SIZE:
			{
				RECT clientRect;
				GetClientRect (mainWnd, &clientRect);

				HWND childWnd = ::GetWindow (mainWnd, GW_CHILD);
				if (childWnd)
					SetWindowPos (childWnd, 0, 0, 0, clientRect.right, clientRect.bottom,
									SWP_NOZORDER | SWP_NOMOVE);
			}
			break;

		case WM_DESTROY:

			PostQuitMessage(0);
			break;
   }

   return DefWindowProc(hWnd, message, wParam, lParam);
}

/* WinMain
** ------------------------------------------------------
**
** Initializes the app and enters the message loop.
*/
int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndClass;

	// Initialize global variables.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EXTERNALVIEW, szWindowClass, MAX_LOADSTRING);
	hInst = hInstance;

	// Create and register our window class.
	wndClass.style			= CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	= (WNDPROC)WndProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= hInstance;
	wndClass.hIcon			= LoadIcon (hInstance, (LPCTSTR)IDI_EXTERNALVIEW);
	wndClass.hCursor		= LoadCursor (NULL, IDC_ARROW);
	wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wndClass.lpszMenuName	= (LPCSTR)IDC_EXTERNALVIEW;
	wndClass.lpszClassName	= szWindowClass;
	RegisterClass (&wndClass);

	// Perform application initialization.
	mainWnd = CreateWindow (szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
							CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!mainWnd)
      return FALSE;

	ShowWindow (mainWnd, nCmdShow);
	UpdateWindow (mainWnd);

	// We need to launch Acrobat to allow us to communicate with
	// the window created by the ExternalWindow plug-in

	char strAcrobatPath [1024];
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;

	GetAcrobatPath (strAcrobatPath, 1024);

	// Initialize CreateProcess input/output params
	memset (&startupInfo, 0, sizeof(STARTUPINFO));
	memset (&processInfo, 0, sizeof(PROCESS_INFORMATION));

	startupInfo.cb = sizeof(STARTUPINFO);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW;
	startupInfo.wShowWindow = SW_HIDE;

	CreateProcess (NULL, strAcrobatPath, NULL, NULL, FALSE, NULL, NULL, NULL, &startupInfo, &processInfo);
	hProcess = processInfo.hProcess;

	if (!hProcess) {
		MessageBox (mainWnd, "Failed to launch Acrobat. Aborting...", "ExternalView - Error", MB_OK);
		return -1;
	}

	// Main message loop.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Terminate Acrobat.
	TerminateProcess (hProcess, 0);

	return msg.wParam;
}
