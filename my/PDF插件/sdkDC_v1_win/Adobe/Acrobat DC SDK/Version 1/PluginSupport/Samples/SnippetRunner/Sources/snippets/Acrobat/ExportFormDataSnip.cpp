/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ExportFormDataSnip.cpp

- This snippet exports an FDF file from the frontmost document

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static char *FDF_HEADER = "%FDF-1.2";

// global variable to save output file name
static char gfdfPath[252];

/*-------------------------------------------------------
	Export Form Data
-------------------------------------------------------*/


	

/* SaveFDF
** ------------------------------------------------------
**
** Saves the CosDoc to the specified path.
*/
static void SaveFDF (CosDoc fdfDoc, ASFileSys fileSys, ASPathName path)
{
	ASInt32 errorCode;
	ASFile volatile fdfFile = NULL;
	CosDocSaveParamsRec params;

	// Initialize save params
	memset (&params, 0, sizeof(CosDocSaveParamsRec));
	params.size = sizeof(CosDocSaveParamsRec);
	params.header =	FDF_HEADER;

	DURING
		// Create/Open the file
		errorCode = ASFileSysOpenFile (fileSys, path, ASFILE_CREATE | ASFILE_WRITE,
										(ASFile*)&fdfFile);

		// If we succeeded, save the doc to the file
		if (0 == errorCode) {
			CosDocSaveToFile (fdfDoc, fdfFile, cosSaveFullSave, &params);
			CosDocClose (fdfDoc);
			ASFileClose (fdfFile);
			char msg[252] ="Form data were saved to file ";
			strcat(msg, gfdfPath); 
			Console::displayString(msg);
		} else {
			AVAlertNote("Error in saving the fdf file!");
			ASRaise (ASFileError(fileErrOpenFailed));
		}
	HANDLER
		if (fdfFile)
			ASFileClose (fdfFile);
		RERAISE();
	END_HANDLER
}


/** This snippet exports an FDF file from the frontmost document
	@testfile formfields.pdf
	@requires a form document
	@see ASFileSysOpenFile
	@see CosDocSaveToFile
	@see CosDocClose
	@see ASFileClose
	@see AVAlertNote
	@see ASRaise
	@see ASGetDefaultFileSys
	@see ASFileGetFileSys
	@see PDDocGetFile
	@see ASFileAcquirePathName
	@see ASFileSysDIPathFromPath
	@see ASFileSysReleasePath
	@see ASFileError
	@see ASmalloc
	@see ASfree
	@see ASFileSysCreatePathName
	@see AFPDDocLoadPDFields
	@see AVDocClearSelection
	@see ExportAsFDF
*/
void ExportFormDataSnip()
{
	AVDoc avDoc;
	PDDoc pdDoc;
	CosDoc cosDoc;
	ASFileSys fileSys = ASGetDefaultUnicodeFileSys();
	ASPathName volatile fdfPathName = NULL;
	char * volatile pdfDIPath = NULL;
	char * volatile fdfPath = NULL;
	char errorBuf[256];
	ASBool bInErrorState = false;

	// Generate a pathname to which we can save the FDF
	DURING
		// Get PDDoc for active AVDoc.
		CDocument document;
		avDoc = (AVDoc)document;
		if(avDoc == NULL) E_RTRN_VOID;

		pdDoc = (PDDoc)document;
		
		// Release PDF path and get the file system to provide us
		// with a new device-dependent path for the FDF.

		fdfPathName = SnippetRunnerUtils::getOutputPath("ExportFormData.fdf");
		if (!fdfPathName)
			ASRaise (ASFileError(fileErrOpenFailed));
	HANDLER
		// Display error.
		ASGetErrorString (ASGetExceptionErrorCode(), errorBuf, 256);
		AVAlertNote (errorBuf);
		bInErrorState = true;
	END_HANDLER

	if (!bInErrorState) {

		DURING
			// Ensure that all fields in form get created
			AFPDDocLoadPDFields (pdDoc);
			AVDocClearSelection (avDoc, true);

			// Export all fields to a CosDoc (FDF) and save it
			cosDoc = ExportAsFDF (pdDoc, CosNewNull(), true, true, false, true, NULL);
			SaveFDF (cosDoc, fileSys, fdfPathName);
		HANDLER
			ASGetErrorString (ASGetExceptionErrorCode(), errorBuf, 256);
			AVAlertNote (errorBuf);
		END_HANDLER
	}

	if (fdfPathName)
		ASFileSysReleasePath (fileSys, fdfPathName);
}


SNIPRUN_REGISTER(ExportFormDataSnip, "AV Layer:Forms:Export Form Data", "Exports an FDF file.")
