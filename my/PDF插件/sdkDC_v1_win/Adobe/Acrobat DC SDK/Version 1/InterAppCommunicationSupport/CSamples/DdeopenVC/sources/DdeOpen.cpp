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

 DdeOpen.cpp

 - Implements a very simple DDE client that communicates with the
   viewer to open a file called "simple.pdf" located at the root level
   of the C drive.

 - The viewer must be running in order for the DDE transaction to
   take place.

*********************************************************************/

#include <windows.h>
#include <stdio.h>

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

const char *REG_INSTALL_KEY = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Acrobat.exe";
const char *DDE_CMD_STRING = "[FileOpenEx(\"c:\\simple.pdf\")]";
const char *ACRO_DDESERVER = "acroviewA12";
const char *ACRO_DDETOPIC = "control";
const DWORD MAX_TIMEOUT = 3000, STEP_SIZE = 200;

HDDEDATA CALLBACK DDE_ProcessMessage (UINT uType, UINT uFmt, HCONV hconv, HSZ hsz1, HSZ hsz2,
									  HDDEDATA hdata, DWORD dwData1, DWORD dwData2);

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/* WinMain
** ------------------------------------------------------
**
** Main entrypoint for application.
*/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	long lRetCode;
	HKEY hkey;
	DWORD pathBuf[MAX_PATH + 1];
	DWORD size = MAX_PATH + 1;

	// Determine if a PDF viewer is installed.
	lRetCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_INSTALL_KEY, 0, KEY_READ, &hkey);
	if (lRetCode == ERROR_SUCCESS) {

		// Get the path to the viewer executable and launch it.
		lRetCode = RegQueryValueEx (hkey, "", 0, 0, (unsigned char *)pathBuf, &size);
		RegCloseKey (hkey);

		STARTUPINFO startupInfo;
		memset (&startupInfo, 0, sizeof(STARTUPINFO));
		startupInfo.cb = sizeof(STARTUPINFO);

		PROCESS_INFORMATION processInfo;
		memset (&processInfo, 0, sizeof(PROCESS_INFORMATION));

		// Launch the viewer.
		CreateProcess ((const char *)pathBuf, NULL, NULL, NULL, false, NORMAL_PRIORITY_CLASS,
						NULL, NULL, &startupInfo, &processInfo);

	} else {

		// Advertise the problem and bail.
		MessageBox (NULL,"No PDF Viewer installed. Aborting...", "DdeOpen - Error",MB_OK);
		return -1;
	}

	char ddeCmdBuf[64];
	UINT retVal;
	DWORD id = 0;
	DWORD dwSleep = 0;

	// Start the DDE work now that the viewer is launched.
	memset (ddeCmdBuf, 0, sizeof(ddeCmdBuf));
	strcpy (ddeCmdBuf, DDE_CMD_STRING);

	retVal = DdeInitialize (&id, &DDE_ProcessMessage, APPCMD_CLIENTONLY, 0);
	if (DMLERR_NO_ERROR == retVal) {

		DWORD dwResult;
		HCONV hConversation = NULL;
		HSZ hszServerName, hszTopicName;

		// Initialize DDE conversation with server.
		hszServerName = DdeCreateStringHandle (id, ACRO_DDESERVER, 0);
		hszTopicName = DdeCreateStringHandle (id, ACRO_DDETOPIC, 0);

		// Acrobat can take a while to launch. We repeatedly attempt
		// to connect to the server until MAX_TIMEOUT expires.
		do {

			hConversation = DdeConnect (id, hszServerName, hszTopicName, NULL);
			if (hConversation || (dwSleep > MAX_TIMEOUT))
				break;

			// Give Acrobat some more time to launch....
			Sleep (dwSleep += STEP_SIZE);

		} while (true);

		if (!hConversation)
			MessageBox (NULL, "Could not connect to server.", "DdeOpen - Error", MB_OK);
		else {
			// Execute the DDE Command.
			DdeClientTransaction ((unsigned char *)ddeCmdBuf, (DWORD)strlen(ddeCmdBuf), (HCONV)hConversation,
									NULL, (UINT)CF_TEXT, (UINT)XTYP_EXECUTE, (DWORD)1000, &dwResult);
			DdeDisconnect (hConversation);
		}

		// Release resources.
		DdeFreeStringHandle (id, hszServerName);
		DdeFreeStringHandle (id, hszTopicName);
		DdeUninitialize (id);

	} else
		MessageBox (NULL, "Failed to initialize DDE.", "DdeOpen - Error", MB_OK);

	return TRUE;
}

HDDEDATA CALLBACK DDE_ProcessMessage (UINT uType, UINT uFmt, HCONV hconv, HSZ hsz1, HSZ hsz2,
									  HDDEDATA hdata, DWORD dwData1, DWORD dwData2)
{
	return NULL;
}

