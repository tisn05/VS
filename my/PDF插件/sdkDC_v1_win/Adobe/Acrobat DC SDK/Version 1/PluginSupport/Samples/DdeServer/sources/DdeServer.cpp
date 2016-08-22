/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/

/**

 \file DdeServer.cpp

 - DDEServer exposes functionality to external Windows applications
   through DDE.

 - The plug-in interprets XTYP_EXECUTE transactions as commands. The
   only command that has been implemented is:

 	"AddNote" - Adds a text annotation upper left corner of page.

 - A XTYP_REQUEST transaction with an item of "PageNumber" will
   retrieve the page number of the currently displayed page.

*********************************************************************/

// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

extern HINSTANCE gHINSTANCE;

// DDE Management.
static DWORD ddeInst;
static HSZ hszServiceName, hszPageNumber;

/*-------------------------------------------------------
	Utility Methods
-------------------------------------------------------*/

/**
	Fills the buffer with the page number associated with
	the active document view.
	@param buffer OUT holds the page number as a char *
	@return the number of bytes written to buffer, not
	including the terminating NULL character.
	@see AVAppGetActiveDoc
	@see AVPageViewGetPageNum
	@see AVDocGetPageView
*/
static ASInt32 GetDisplayedPageNumberAsString (char *buffer)
{
	AVDoc avDoc;
	ASInt32 pageNum;

	if (!buffer)
		return 0;

	DURING
		avDoc = AVAppGetActiveDoc();
		if (!avDoc)
			E_RETURN(0);

		pageNum = AVPageViewGetPageNum (AVDocGetPageView(avDoc));
	HANDLER
		return 0;
	END_HANDLER

	sprintf (buffer, "%d", pageNum + 1);
	return strlen (buffer);
}

/**
	Validates the command string. The required format is
	AddNote(StringToBeDisplayedInTheAnnotation).
	@param cmdString IN the text representation of the command
	@return the annotation contents or NULL if the
	format was not recognized.
	@see PDXlateToPDFDocEnd
*/
static char *ValidateCommandString (char *cmdString)
{
	ASInt32 strLength;
	char *noteContent = NULL;

	if (!strstr(cmdString, "AddNote"))
		return noteContent;

	// Detect the parenthesis.
	char *strBegin = strchr (cmdString, '(');
	char *strEnd = strrchr (cmdString, ')');
	if (!strBegin || !strEnd)
		return noteContent;

	// Allocate memory for the string and take a copy.
	strLength = (strEnd - strBegin) - 1;
	noteContent = (char*)ASmalloc (sizeof(char) * (strLength + 1));
	strncpy (noteContent, (strBegin + 1), strLength);
	noteContent[strLength] = 0;

	// Convert to PDFDocEncoding
	PDXlateToPDFDocEnc (noteContent, noteContent, strLength);
	return noteContent;
}

/**
	Adds a text annotation to the displayed page.
	@param buffer IN the contents of the text annotation
	@return DDE_FACK on success, or some DDE error code.
	@see AVAppGetActiveDoc
	@see AVDocGetPageView
	@see AVPageViewGetPageNum
	@see PDDocAcquirePage
	@see PDPageGetCropBox
	@see PDPageAddNewAnnot
	@see PDAnnotSetTitle
	@see PDTextAnnotSetContents
	@see PDTextAnnotSetOpen
	@see AVPageViewDrawNow
*/
static ASInt32 AddTextAnnotationToDisplayedPage (char *buffer)
{
	AVDoc avDoc;
	AVPageView pageView;
	PDDoc pdDoc;
	PDPage volatile pdPage = NULL;
	PDAnnot pdAnnot;
	ASInt32 pageNum, retVal = DDE_FACK;
	ASFixedRect annotBounds, cropBox;

	avDoc = AVAppGetActiveDoc();
	if (!avDoc)
		retVal = DDE_FNOTPROCESSED;
	else {

		DURING
			// Acquire the displayed page.
			pdDoc = AVDocGetPDDoc (avDoc);
			pageView = AVDocGetPageView (avDoc);
			pageNum = AVPageViewGetPageNum (pageView);
			pdPage = PDDocAcquirePage (pdDoc, pageNum);

			// The annotation is placed one inch from
			// the top left of the crop box.

			PDPageGetCropBox (pdPage, &cropBox);

			annotBounds.left = cropBox.left + fixedSeventyTwo;
			annotBounds.top = cropBox.top - fixedSeventyTwo;
			annotBounds.right = cropBox.left + ASInt32ToFixed(18);
			annotBounds.bottom = cropBox.top - ASInt32ToFixed(22);

			// Create the annotation.
			pdAnnot = PDPageAddNewAnnot (pdPage, -2, ASAtomFromString("Text"), &annotBounds);
			PDAnnotSetTitle (pdAnnot, "AddNote", strlen("AddNote"));
			PDTextAnnotSetContents (pdAnnot, buffer, strlen(buffer));
			PDTextAnnotSetOpen (pdAnnot, true);

			// Force redraw of page.
			AVPageViewDrawNow (pageView);
		HANDLER
			retVal = DMLERR_SYS_ERROR;
		END_HANDLER

		if (pdPage)
			PDPageRelease (pdPage);
	}
	return retVal;
}

/*-------------------------------------------------------
	DDE Implementation
-------------------------------------------------------*/

/**
	Message handler. Processes three transaction types:
	XTYP_CONNECT, XTYP_EXECUTE, and XTYP_REQUEST.
*/
HDDEDATA CALLBACK DdeMsgCallBack (UINT type, UINT format, HCONV conv, HSZ hsz1,
							  HSZ hsz2, HDDEDATA hData, DWORD data1, DWORD data2)
{
	HDDEDATA retVal = NULL;

	switch(type)
	{

	case XTYP_CONNECT:

		// Accept connection.
		retVal = (HDDEDATA)TRUE;
		break;

	case XTYP_EXECUTE:

		{
			DWORD lng;
			char *cmdString = (char*)DdeAccessData (hData, &lng);
			char *noteContent = NULL;

			cmdString[lng] = 0;
			// Execute "data" as a command against the active document.
			noteContent = ValidateCommandString (cmdString);
			if (noteContent) {

				retVal = (HDDEDATA)AddTextAnnotationToDisplayedPage (noteContent);
				ASfree (noteContent);

			} else
				retVal = DDE_FNOTPROCESSED;

			DdeUnaccessData (hData);
		}
		break;

	case XTYP_REQUEST:

		// If the transaction item name (hsz2) is
		// "PageNumber", return the current page number.

		if ((hsz2 == hszPageNumber) && (format == CF_TEXT)) {

			char pageNum[32];
			ASInt32 strLength;

			strLength = GetDisplayedPageNumberAsString (pageNum);

			if (0 == strLength)
				retVal = (HDDEDATA)NULL;
			else {
				retVal = DdeCreateDataHandle (ddeInst, (LPBYTE)pageNum, (strLength + 1),
												0, hsz2, CF_TEXT, 0);
			}
		} else
			retVal = NULL;

		break;
	}

	return retVal;
}

/**
	Initialises the dde server.
	@return false if the initialisation fails, true otherwise.
*/
ASBool InitializeDDEServer()
{
	// Initialize DDEML.
	if (DdeInitialize(&ddeInst, DdeMsgCallBack, APPCLASS_STANDARD, 0)) {

		AVAlertNote("DdeServer: DDE Initialization failed.");
		return false;
	}

	// Create DDE string handles.
	hszServiceName = DdeCreateStringHandle (ddeInst, "SdkDdeService", CP_WINANSI);
	hszPageNumber = DdeCreateStringHandle (ddeInst, "PageNumber", CP_WINANSI);

   	// Register service name.
	if (DdeNameService(ddeInst,	hszServiceName, 0, DNS_REGISTER) == 0) {

		AVAlertNote("DdeServer: DdeNameService failed.");
		return false;
	}

	return true;
}

/**
	terminates the dde service
*/
void TerminateDDEServer()
{
	// Unregister our service.
	DdeNameService (ddeInst, hszServiceName, 0, DNS_UNREGISTER);

	// Free our DDE handles.
	DdeFreeStringHandle (ddeInst, hszServiceName);
	DdeFreeStringHandle (ddeInst, hszPageNumber);

	// Release DDEML resources.
	DdeUninitialize (ddeInst);
}
