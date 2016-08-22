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

\file ExternalWindowUtils.cpp

 - Implements a platform independent method to allow the user to open
   a PDF document.

*********************************************************************/

// Acrobat Headers.
#ifdef WIN_PLATFORM
#include "PIHeaders.h"
#endif

/*-------------------------------------------------------
	Utility Methods
-------------------------------------------------------*/

/** 
	Displays a standard open file dialog box to allow
	user to open a PDF document.

	@return the ASPathName if the user confirmed the
	selection, NULL if they cancelled.
	@see AVOpenSaveDialogParamsRec
	@see AVFileFilterRec
	@see ASTextSetEncoded
	@see ASScriptToHostEncoding
	@see AVAppOpenDialog
	@see ASGetErrorString
	@see ASTextDestroy
*/
ASPathName OpenPDFFile (void)
{
	AVOpenSaveDialogParamsRec dialogParams;
	AVFileFilterRec filterRec, *filterRecP;
	AVFileDescRec descRec;
	ASPathName * volatile pathName = NULL;
	ASBool bSelected = false;
	char errorBuf[256];

	// Set up the PostScript file filter description.
	strcpy (descRec.extension, "pdf");
	descRec.macFileType = 'PDF ';
	descRec.macFileCreator = 'CARO';

	memset (&filterRec, 0, sizeof(AVFileFilterRec));
	filterRec.fileDescs = &descRec;
	filterRec.numFileDescs = 1;
	filterRecP = &filterRec;

	// Configure the dialog box parameters.
	memset (&dialogParams, 0, sizeof (AVOpenSaveDialogParamsRec));
	dialogParams.size = sizeof(AVOpenSaveDialogParamsRec);
	dialogParams.fileFilters = &filterRecP;
	dialogParams.numFileFilters = 1;

	DURING

		// The following methods can raise so we wrap them within
		// the exception handling macros.

		filterRec.filterDescription = ASTextNew();
		ASTextSetEncoded (filterRec.filterDescription, "Adobe PDF Files",
						ASScriptToHostEncoding(kASRomanScript));

		dialogParams.windowTitle = ASTextNew();
		ASTextSetEncoded (dialogParams.windowTitle, "Select A PDF Document To Open",
						ASScriptToHostEncoding(kASRomanScript));

		dialogParams.flags |= kAVOpenSaveAllowForeignFileSystems;
		dialogParams.initialFileSys = ASGetDefaultUnicodeFileSys();

		// Pop the dialog.
		ASFileSys outFileSys;
		bSelected = AVAppOpenDialog(&dialogParams, &outFileSys, (ASPathName**)&pathName, NULL, NULL);

	HANDLER
		// Display error message to user.
		ASGetErrorString (ASGetExceptionErrorCode(), errorBuf, 256);
		AVAlertNote (errorBuf);
	END_HANDLER

	// Destroy the ASText object then return.
	ASTextDestroy (filterRec.filterDescription);
	ASTextDestroy (dialogParams.windowTitle);

	return bSelected ? *pathName : NULL;
}