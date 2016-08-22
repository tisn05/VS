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

 EmitPostScriptSnip.cpp

 - Registers for notification of a number of print events and emits a
   pre-selected PostScript file found in the SnippetRunner/Examples folder
   into the print stream at the start of each page (under all other content).

 - It's easy to modify the sample to print at the end of the page (on
   top of all other content) instead of the beginning. Simply set
   BACKEND to 1. The PostScript file draft.ps causes the word DRAFT
   to be printed at 45 degrees in the middle of the page.

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

//ASPathName gPrintPathName = NULL;
ASBool gEmitEnabled = false;
static ASFile gPrintFile = NULL;
static const ASInt32 BUF_SIZE = 512;
static const char * INPUT_POSTSCRIPT = "EmitPostScript.ps";
static ASCallback cbOpenPrintFile = NULL;
static ASCallback cbClosePrintFile = NULL;
static ASCallback cbEmitPostScript = NULL;

/*-------------------------------------------------------
	Notification Callbacks
-------------------------------------------------------*/

/* ClosePrintFile
** ------------------------------------------------------
**
** Undo OpenPrintFile.
*/
static ACCB1 void ACCB2 ClosePrintFile (PDDoc doc, ASInt32 fromPage, ASInt32 toPage,
										ASInt32 error, void *clientData)
{
	if (gPrintFile) {
		ASFileClose (gPrintFile);
		gPrintFile = NULL;
	}
}

/* OpenPrintFile
** ------------------------------------------------------
**
** Open the file previously chosen by the user.
*/
static ACCB1 void ACCB2 OpenPrintFile (PDDoc doc, ASInt32 fromPage, ASInt32 toPage,
								ASInt32 psLevel, ASBool binaryOK, void *clientData)
{
	// The ASStm parameter of the PDDocWill/DidPrintPage callbacks
	// is only non-NULL when printing to a PostScript printer.
	// If the user isn't printing to a PostScript printer, we don't
	// emit anything.

	if (psLevel != 0) {

		// Open the file.
		gPrintFile = SnippetRunnerUtils::openSnippetExampleASFile(INPUT_POSTSCRIPT);

		if (gPrintFile == NULL)
			AVAlertNote ("Failed to open the PostScript file.");
	}
}

/* EmitPostScriptToStream
** ------------------------------------------------------
**
** Transfer the contents of gPrintFile to the print stream.
*/
static ACCB1 void ACCB2 EmitFileToStream (PDDoc doc, Int32 page, ASStm stm, void *clientData)
{
	ASInt32 numBytesRead;
	char buf[BUF_SIZE];

	if (gPrintFile) {

		DURING
			// Write the contents of the file to the print stream.
			ASFileSetPos(gPrintFile, 0);
			while ((numBytesRead = ASFileRead(gPrintFile, buf, BUF_SIZE)) != 0)
				ASStmWrite(buf, numBytesRead, 1, stm);
		HANDLER
			// Fail quietly.
		END_HANDLER
	}
}

/*-------------------------------------------------------
	UI Callbacks
-------------------------------------------------------*/

/** 
	@see ASTextNew
	@see ASTextSetEncoded
	@see AVAppOpenDialog
	@see ASTextDestroy
	@see ASFileClose
	@see ASFileSysOpenFile
	@see ASFileSetPos
	@see ASFileRead
	@see ASStmWrite
	@see ASCallbackCreateNotification
	@see AVAppUnregisterNotification
	@see AVAppRegisterNotification
	@see ASFileSysReleasePath
*/
void EmitPostScriptSnip()
{
	// Create our notification callbacks if we haven't done so already.
	if (!cbOpenPrintFile) {
		cbOpenPrintFile = (void*)ASCallbackCreateNotification (PDDocWillPrintPages, &OpenPrintFile);
		cbClosePrintFile = (void*)ASCallbackCreateNotification (PDDocDidPrintPages, &ClosePrintFile);
#if BACKEND
		cbEmitPostScript = (void*)ASCallbackCreateNotification (PDDocDidPrintPage, &EmitFileToStream);
#else
		cbEmitPostScript = (void*)ASCallbackCreateNotification (PDDocWillPrintPage, &EmitFileToStream);
#endif
	}

	// If we're enabled, unregister our notification callbacks.
	if (gEmitEnabled) {

		AVAppUnregisterNotification (PDDocWillPrintPagesNSEL, gExtensionID, cbOpenPrintFile, NULL);
		AVAppUnregisterNotification (PDDocDidPrintPagesNSEL, gExtensionID, cbClosePrintFile, NULL);
#if BACKEND
		AVAppUnregisterNotification (PDDocDidPrintPageNSEL, gExtensionID, cbEmitPostScript, NULL);
#else
		AVAppUnregisterNotification (PDDocWillPrintPageNSEL, gExtensionID, cbEmitPostScript, NULL);
#endif

		gEmitEnabled = false;

	} else {

		// We're disabled, get the input file from the user. If they confirm
		// their selection, we register our notification callbacks.
		AVAppRegisterNotification (PDDocWillPrintPagesNSEL, gExtensionID, cbOpenPrintFile, NULL);
		AVAppRegisterNotification (PDDocDidPrintPagesNSEL, gExtensionID, cbClosePrintFile, NULL);
#if BACKEND
		AVAppRegisterNotification (PDDocDidPrintPageNSEL, gExtensionID, cbEmitPostScript, NULL);
#else
		AVAppRegisterNotification (PDDocWillPrintPageNSEL, gExtensionID, cbEmitPostScript, NULL);
#endif

		gEmitEnabled = true;
	}
}


SNIPRUN_REGISTER(EmitPostScriptSnip, "PD Layer:Emit PostScript", "Toggles whether PostScript is emitted during printing.")


