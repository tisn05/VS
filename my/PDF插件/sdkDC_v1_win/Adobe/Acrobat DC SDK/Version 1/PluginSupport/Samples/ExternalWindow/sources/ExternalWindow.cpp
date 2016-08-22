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

 ExternalWindow.cpp

 - Platform dependent implementation to display PDF documents in
   windows that are external to the Acrobat application.

 - External documents can be opened by two means - through the
   menuitem in the Acrobat SDK menu and through IAC with an external
   application.

   The sample uses an invisible window to receive communication from
   the external application. The IAC handler supports two "commands":

   Open File : to open a file, send the WM_COPYDATA message with the
   			   path of the PDF to open and the HWND into which the
   			   PDF is to be drawn.

   Close File : to close a file, send the WM_CLOSE message.

*********************************************************************/

// Acrobat Headers.
#ifdef WIN_PLATFORM
#include "PIHeaders.h"
#endif


/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

extern "C" HINSTANCE gHINSTANCE;

struct _t_ExternDocInfo
{
	AVDoc doc;
	ASFile file;
	ASBool bWindowIsOpen;
} gDocInfo;

static HWND msgHWnd = NULL;
static HWND externHWnd = NULL;
static ASCallback cbAppCanQuitProc;
static CrossDocLinkWithDestProc cbCrossDocLink;

extern ASPathName OpenPDFFile (void);

/*-------------------------------------------------------
	UI Callbacks
-------------------------------------------------------*/

/**
	Opens a document in an external window.
	@param clientData IN/OUT client data used in this callback. Not used in this function.
	@see AVDocOpenParamsRec
	@see ExternalDocServerCreationDataRec
	@see AVDocOpenFromASFileWithParams
	@see ASFileSysOpenFile
	@see ASGetDefaultFileSys
*/
ACCB1 void ACCB2 OpenExternalWindow (void* clientData)
{
	ASPathName volatile pathName = NULL;
	ASInt32 retVal;
	AVDocOpenParamsRec params;
	ExternalDocServerCreationDataRec creationData;
	bool bWindowIsOpen = false;
	externHWnd = CreateWindow ("ExternalWindow", NULL, WS_OVERLAPPEDWINDOW,
								50, 50, 500, 500, 0, 0, gHINSTANCE, NULL);
	if (externHWnd) {

		memset(&params, 0, sizeof(AVDocOpenParamsRec));
		params.size = sizeof(AVDocOpenParamsRec);
		params.useFrame = false;
		params.useVisible = true;
		params.visible = true;
		params.useServerType = true;
		params.serverType = "EXTERNAL";
		params.serverCreationData = (void*) &creationData;
		params.useViewType = true;
		params.viewType = "AVDocView";
		params.useReadOnly = true;
		params.readOnly = true;
		params.useSourceDoc = false;
		params.useViewDef = false;

		memset(&creationData, 0, sizeof(ExternalDocServerCreationDataRec));
		creationData.size = sizeof(ExternalDocServerCreationDataRec);
		creationData.platformWindow = (ExternalDocWindowData)externHWnd;
		creationData.acrobatProc = NULL;
		creationData.crossDocLinkWithDestProc = cbCrossDocLink;
		creationData.crossDocLinkWithDestData = (void *)externHWnd;

		DURING
			// Initiate the file open interaction with the user.
			pathName = OpenPDFFile();
			if (pathName) {

				// The user selected a file, so let's try and open
				// it and display it if we can.

				retVal = ASFileSysOpenFile (ASGetDefaultUnicodeFileSys(), pathName, ASFILE_READ, &gDocInfo.file);
				if (!retVal) {
					ASText title = ASTextFromScriptText ("External Window",kASRomanScript);
					gDocInfo.doc = AVDocOpenFromASFileWithParams (gDocInfo.file, title, &params);
					ASTextDestroy(title);
					bWindowIsOpen = true;
				} else
					AVAlertNote ("Failed to open document. Aborting...");
			}
		HANDLER

		END_HANDLER

		if (!bWindowIsOpen) {
			DestroyWindow (externHWnd);
			externHWnd = NULL;
		}

		if (pathName)
			ASFileSysReleasePath (NULL, pathName);
	}
}

/**
	Enables the menuitem if we have no document already
	open.

	return true to enable the menuitem.
*/
ACCB1 ASBool ACCB2 ExternalWindowIsEnabled (void *clientData)
{
	return !gDocInfo.bWindowIsOpen;
}

/*-------------------------------------------------------
	Acrobat Callbacks
-------------------------------------------------------*/

/** 
	Called by Acrobat to determine if it can quit. We
	use this hook to destroy any windows that we have
	created. Otherwise, we defer to the default
	behavior.

	@return true to allow quit.
*/
static ACCB1 ASBool ACCB2 AppCanQuitProc (void)
{
	// Destroy the windows that we have created.
	if (externHWnd) {
		DestroyWindow (externHWnd);
		externHWnd = NULL;
	}

	if (msgHWnd) {
		DestroyWindow (msgHWnd);
		msgHWnd = NULL;
	}

	return CALL_REPLACED_PROC(gAcroViewHFT, AVAppCanQuitSEL, cbAppCanQuitProc)();
}

/**
	Callback supplied with the AVDocOpenParamsRec. This
	callback manages cross-document links within the
	context of our external window.

	We close the current document and open the target
	document in the existing external window.

	@param path IN the path of the document being linked to
	@param fielSys IN the filesys being used
	@param viewDef IN defining the view of the document
	@param destInfo IN representation of the destination of the document
	@param srcDoc IN the document containing the source of the link
	@param data IN client data passed in when the callback was defined
	@returns the new AVDoc.
	@see ASFileSysOpenFile
	@see ASGetDefaultFileSys
	@see AVDocGetAVWindow
	@see AVWindowShow
	@see AVDocOpenParamsRec
	@see AVPageView
	@see AVDocOpenParamsRec
	@see AVWindow
*/
static ACCB1 AVDoc ACCB2 CrossDocLinkWithDest (ASPathName path, ASFileSys fileSys, AVDocViewDef viewDef,
												AVDestInfo destInfo, AVDoc srcDoc, void* data)
{
	AVWindow avWindow;
	ASInt32 retVal;
	ASFile targetFile = NULL;
	AVDoc volatile targetDoc = NULL;
	AVPageView pageView;
	AVDocOpenParamsRec params;
	ExternalDocServerCreationDataRec creationData;


	retVal = ASFileSysOpenFile (ASGetDefaultUnicodeFileSys(), path, ASFILE_READ, &targetFile);
	if (!retVal) {

		memset(&params, 0, sizeof(AVDocOpenParamsRec));
		params.size = sizeof(AVDocOpenParamsRec);
		params.useFrame = false;
		params.useVisible = true;
		params.visible = true;
		params.useServerType = true;
		params.serverType = "EXTERNAL";
		params.serverCreationData = (void*) &creationData;
		params.useViewType = true;
		params.viewType = "AVDocView";
		params.useReadOnly = true;
		params.readOnly = true;
		params.useSourceDoc = true;	// Set this so that we close the original doc and
		params.sourceDoc = srcDoc;	// replace it with the new one.
		params.useViewDef = true;
		params.viewDef = viewDef;

		memset(&creationData, 0, sizeof(ExternalDocServerCreationDataRec));
		creationData.size = sizeof(ExternalDocServerCreationDataRec);
		creationData.platformWindow = (ExternalDocWindowData)data;
		creationData.crossDocLinkWithDestProc = cbCrossDocLink;
		creationData.crossDocLinkWithDestData = (void *)data;

		DURING
			// Open the new document in the existing window.
			ASText title = ASTextFromScriptText ("External Window",kASRomanScript);
			targetDoc = AVDocOpenFromASFileWithParams (targetFile, title, &params);
			ASTextDestroy(title);
			pageView = AVDocGetPageView (targetDoc);
			AVPageViewUseDestInfo (pageView, destInfo);
		HANDLER
			if (targetDoc)
				AVDocClose (targetDoc, false);
			else
				ASFileClose (targetFile);
			return NULL;
		END_HANDLER

		avWindow = AVDocGetAVWindow (targetDoc);
		AVWindowShow (avWindow);

		gDocInfo.file = targetFile;
		gDocInfo.doc = targetDoc;

		return targetDoc;
	}
	else {
		AVAlertNote("Unable to open cross doc link");
		return NULL;
	}
}

/*-------------------------------------------------------
	WndProc Message Handlers
-------------------------------------------------------*/

/**
	WndProc for our message target. External applications
	can send us requests by sending the appropriate
	windows message.

	The communication is strictly one-way at the moment.
	If something goes awry, we fail quietly.
	@return the default window handling procedure
*/
LRESULT CALLBACK IACWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CLOSE) {

		if (gDocInfo.doc) {

			// Close document without saving.
			AVDocClose (gDocInfo.doc, false);

			gDocInfo.doc = NULL;
			gDocInfo.file = NULL;
			gDocInfo.bWindowIsOpen = false;
		}

    } else if (msg == WM_COPYDATA) {

		HWND parentHWnd;
		COPYDATASTRUCT *copyData;
		ASInt32 pathLen;
		char path[255];
		RECT clientRect;

		// We can only track one open document at a time.
		if (gDocInfo.bWindowIsOpen)
			return FALSE;

		parentHWnd = (HWND)wParam;
		copyData = (COPYDATASTRUCT*)lParam;

		pathLen = (ASInt32)copyData->cbData;
		strncpy (path, (char *)copyData->lpData, pathLen);
		path[pathLen] = 0;

		// Assuming that we were passed the correct parameters, we
		// proceed to open the PDF document in the applications window.
		// We create a child window that sits inside the parent.

		GetClientRect (parentHWnd, &clientRect);
		externHWnd = CreateWindow ("ExternalWindow", NULL, WS_CHILDWINDOW, 0, 0,
						(clientRect.right - clientRect.left), (clientRect.bottom - clientRect.top),
						parentHWnd, 0, gHINSTANCE, NULL);

		if (externHWnd) {

			ASPathName volatile pathName = NULL;
			ASInt32 retVal;
			AVDocOpenParamsRec params;
			ExternalDocServerCreationDataRec creationData;

			memset(&params, 0, sizeof(AVDocOpenParamsRec));
			params.size = sizeof(AVDocOpenParamsRec);
			params.useFrame = false;
			params.useVisible = true;
			params.visible = true;
			params.useServerType = true;
			params.serverType = "EXTERNAL";
			params.serverCreationData = (void*) &creationData;
			params.useViewType = true;
			params.viewType = "AVDocView";
			params.useReadOnly = true;
			params.readOnly = true;
			params.useSourceDoc = false;
			params.useViewDef = false;

			memset(&creationData, 0, sizeof(ExternalDocServerCreationDataRec));
			creationData.size = sizeof(ExternalDocServerCreationDataRec);
			creationData.platformWindow = (ExternalDocWindowData) externHWnd;
			creationData.crossDocLinkWithDestProc = (CrossDocLinkWithDestProc)cbCrossDocLink;
			creationData.crossDocLinkWithDestData = (void *)externHWnd;

			DURING
				// Attempt to open the PDF.
				pathName = ASPathFromPlatformPath (path);
				if (pathName) {

					retVal = ASFileSysOpenFile (ASGetDefaultUnicodeFileSys(), pathName, ASFILE_READ, &gDocInfo.file);
					if (!retVal) {
						ASText title = ASTextFromScriptText ("External Window",kASRomanScript);
						gDocInfo.doc = AVDocOpenFromASFileWithParams (gDocInfo.file, title, &params);
						ASTextDestroy(title);
						gDocInfo.bWindowIsOpen = true;
					}
				}
			HANDLER

			END_HANDLER

			if (!gDocInfo.bWindowIsOpen) {
				DestroyWindow (externHWnd);
				externHWnd = NULL;
			}

			if (pathName)
				ASFileSysReleasePath (NULL, pathName);
		}
	}
    return DefWindowProc (hwnd, msg, wParam, lParam);
}

/**
	WndProc for our external window. Defers to the default
	procedure.
*/
LRESULT CALLBACK AVWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY)
		memset (&gDocInfo, 0, sizeof(gDocInfo));

   return DefWindowProc (hwnd, msg, wParam, lParam);
}

/*-------------------------------------------------------
	Initialization
-------------------------------------------------------*/

/**
	Registers the two window classes that we use and
	creates the HWND that is used to capture messages
	from external applications.
	
	@return true if initialized successfully.
*/
ASBool InitializeWindowHandler (void)
{
	WNDCLASS wndClass, iacWndClass;

	// This is the window class used to display the PDF.
    wndClass.cbClsExtra    = 0;
    wndClass.hInstance     = gHINSTANCE;
    wndClass.style	     = CS_DBLCLKS;
    wndClass.hCursor	     = (HCURSOR)NULL;
    wndClass.hbrBackground = (HBRUSH)NULL;
    wndClass.lpfnWndProc   = AVWndProc;
    wndClass.hIcon	     = (HICON)NULL;
    wndClass.lpszMenuName  = NULL;
    wndClass.cbWndExtra    = 0;
    wndClass.lpszClassName = "ExternalWindow";

   	if (!RegisterClass(&wndClass))
		return false;

	// This is the window class used for IAC with an external app to display
	// a PDF file in that apps window.

    iacWndClass.cbClsExtra    = 0;
    iacWndClass.hInstance     = gHINSTANCE;
    iacWndClass.style	     = CS_DBLCLKS;
    iacWndClass.hCursor	     = (HCURSOR)NULL;
    iacWndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    iacWndClass.lpfnWndProc   = IACWndProc;
    iacWndClass.hIcon	     = (HICON)NULL;
    iacWndClass.lpszMenuName  = NULL;
    iacWndClass.cbWndExtra    = 0;
    iacWndClass.lpszClassName = "IACMessageWindow";

   	if (!RegisterClass(&iacWndClass))
		return false;

	// Create our hidden window to capture messages from the external app.
	msgHWnd = CreateWindow ("IACMessageWindow", "External Window Message Target", WS_OVERLAPPEDWINDOW,
								50, 50, 100, 100, 0, 0, gHINSTANCE, NULL);
	if (msgHWnd)
		ShowWindow (msgHWnd, SW_HIDE);
	else
		return false;

	memset (&gDocInfo, 0, sizeof(gDocInfo));

	// Create our cross-document link callback.
	cbCrossDocLink = ASCallbackCreateProto (CrossDocLinkWithDestProc, &CrossDocLinkWithDest);

	cbAppCanQuitProc = ASCallbackCreateReplacement (AVAppCanQuitSEL, &AppCanQuitProc);
	REPLACE(gAcroViewHFT, AVAppCanQuitSEL, cbAppCanQuitProc);
	return true;
}
