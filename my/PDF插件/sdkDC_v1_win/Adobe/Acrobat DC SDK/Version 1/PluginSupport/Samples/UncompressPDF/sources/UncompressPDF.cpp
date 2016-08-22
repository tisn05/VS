/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2001-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/ 
/** 
\file UncompressPDF.cpp

 - Implementation of the AVConversionFromPDFHandler.

 - The "FromPDF" conversion handler saves to PDF with all the page
   content and form XObject streams uncompressed so they can be 
   inspected in any standard file editor.
   
   The file format appears as an option in the "Save As..." dialog.

*********************************************************************/

#ifdef WIN_PLATFORM
#include "PIHeaders.h"
#endif

#include "UncompressPDF.h"
#include "UncompressPDFAgent.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

static AVConversionFromPDFHandlerRec gConversionHandler;
static AVStatusMonitorProcs gStatusMonitor = NULL;

static void PercentDoneCallback (ASReal fPercentDone);
static void UncompressStreamsInDocument (PDDoc pdDoc, AVStatusMonitorProcs statusMonitor);


/*-------------------------------------------------------
	AVConversionFromPDFHandler Callbacks
-------------------------------------------------------*/

/* ConvertFromPDFProc
** ----------------------------------------------------*/ 
/**
 ** Called to convert a PDF file to uncompressed PDF. A secured document 
 ** cannot be saved with unfiltered streams so if we
 ** are passed a document with an associated security
 ** handler, we attempt to remove that handler.
 **
 ** @return kAVConversionSuccess if the operation was
 ** completed without error.
 **
 ** @see AVCryptGetPassword
 ** @see PDDocPermRequest
 ** @see ASRaise
 ** @see PDDocSetNewCryptHandler
 ** @see PDDocSave
 ** @see ASGetErrorString
 ** @see AVAlertNote
 ** @see ASfree
 **/
ACCB1 AVConversionStatus ACCB2 ConvertFromPDFProc (ASCab inSettings, AVConversionFlags flags,
				PDDoc inPDDoc, ASPathName outPath, ASFileSys outFileSys, 
				AVStatusMonitorProcs statusMonitor,	AVConversionClientData clientDataASCab)
{
	const char *szPassPrompt = "Security settings must be cleared before the document data will be readable. Please enter the owner password for the document.";
	ASBool bInteractive = (0 != (flags & kAVConversionInteractive));
	AVConversionStatus retVal = kAVConversionSuccess;
	void * volatile authData = NULL;

	DURING

		if (PDDocGetNewCryptHandler(inPDDoc) != ASAtomNull) 
		{
			PDPerms permsRequired = pdPermSecure;
			PDPermReqStatus permsStatus = PDPermReqDenied;

			// If we don't have permission to remove the security settings then we either:
			//
			// - Report the error and fail.
			// - If in interactive mode, ask the user for the owner password.

			permsStatus = PDDocPermRequest (inPDDoc, PDPermReqObjDoc, PDPermReqOprSecure, NULL);
			if ((PDPermReqGranted != permsStatus) && bInteractive) 
			{
				AVAlertNote (szPassPrompt);
				if (AVCryptGetPassword(inPDDoc, permsRequired, (void**)&authData))
					permsStatus = PDDocPermRequest (inPDDoc, PDPermReqObjDoc, PDPermReqOprSecure, authData);

			}

			if (PDPermReqGranted != permsStatus)
				ASRaise(PDDocError(pdErrOpNotPermitted));

			// Remove the security handler.
			PDDocSetNewCryptHandler (inPDDoc, ASAtomNull);
		}

		// Remove the filters from the streams.
		UncompressStreamsInDocument (inPDDoc, statusMonitor);

		// Save out the document.
		PDDocSave (inPDDoc, PDSaveFull | PDSaveLinearized, outPath, outFileSys, 
							statusMonitor->progMon, statusMonitor->progMonClientData);

	HANDLER

		retVal = kAVConversionFailed;

		// Try to report error through statusMonitor. If that fails,
		// use traditional AVAlert if we're in interactive mode.

		if (NULL != statusMonitor->reportProc)
			statusMonitor->reportProc (kASReportError, ERRORCODE, NULL, NULL, NULL, statusMonitor->reportProcClientData);
		else if (kAVConversionInteractive == (flags & kAVConversionInteractive)) 
		{
			char errBuf[256];

			ASGetErrorString (ERRORCODE, errBuf, 256);
			AVAlertNote (errBuf);
		}

	END_HANDLER
	
	if (NULL != authData)
		ASfree (authData);

	return retVal;
}

/*-------------------------------------------------------
	Helper Methods
-------------------------------------------------------*/

/* PercentDoneCallback
** ----------------------------------------------------*/ 
/**
 ** Progress callback for the UncompressPDFAgent. Reports
 ** progress through the global AVStatusMonitor.
 **/
static void PercentDoneCallback (ASReal fPercentDone)
{
	if (NULL != gStatusMonitor->progMon) {
		if (NULL != gStatusMonitor->progMon->setCurrValue)
			gStatusMonitor->progMon->setCurrValue ((ASInt32)fPercentDone, gStatusMonitor->progMonClientData);
	}
}

/* UncompressStreamsInDocument
** ----------------------------------------------------*/ 
/**
 ** Uses the UncompressPDFAgent to visit each page content
 ** and form a XObject stream in the document and remove
 ** the filters from the stream.
 **
 ** @see ASTextNew
 ** @see ASTextSetEncoded
 ** @see ASScriptToHostEncoding
 ** @see CosDocGetRoot
 ** @see PDDocGetCosDoc
 ** @see ASGetExceptionErrorCode
 ** @see ASTextDestroy
 **/
static void UncompressStreamsInDocument (PDDoc pdDoc, AVStatusMonitorProcs statusMonitor)
{
	const PDFObjType kTargetObjs[] = {PT_CONTENTSSTREAM, PT_FORM};
	const ASUns32 kNumObjs = 2;
	
	ASInt32 errorCode = 0;
	ConsAgentPercentDoneCallback cb = NULL;
	Consultant volatile hConsultant = NULL;
	UncompressPDFAgent * volatile agent = NULL;
	ASText volatile opText = NULL;

	// No way to pass data to the progress callback so we'll use a global.
	gStatusMonitor = statusMonitor;

	DURING
	
		if (NULL != statusMonitor->progMon) {

			// Create the string describing the operation being performed.
			opText = ASTextNew ();
			ASTextSetEncoded (opText, "Uncompressing streams", ASScriptToHostEncoding(kASRomanScript));

			// Configure the progress monitor.
			if (NULL != statusMonitor->progMon->setText)
				statusMonitor->progMon->setText (opText, gStatusMonitor->progMonClientData);

			if (NULL != statusMonitor->progMon->setDuration)
				statusMonitor->progMon->setDuration (100, gStatusMonitor->progMonClientData);

			if (NULL != statusMonitor->progMon->setCurrValue)
				statusMonitor->progMon->setCurrValue (0, gStatusMonitor->progMonClientData);

			if (NULL != statusMonitor->progMon->beginOperation)
				statusMonitor->progMon->beginOperation (gStatusMonitor->progMonClientData);

			cb = &PercentDoneCallback;
		}

		// Create a Consultant object.
		hConsultant = ConsultantCreate (cb);
		if (NULL == hConsultant)
			ASRaise (GenError(genErrGeneral));

		try {
			// Create our Agent object.
			agent = new UncompressPDFAgent (kTargetObjs, kNumObjs);

		} catch (...) {
			
		}

		if (NULL == agent)
			ASRaise (GenError(genErrNoMemory));

		// Register the agent.
		ConsultantRegisterAgent (hConsultant, *agent, REG_REVISITNONE);

		// Start the processing.
		ConsultantTraverseFrom (hConsultant, CosDocGetRoot(PDDocGetCosDoc(pdDoc)), PT_CATALOG);

	HANDLER
		errorCode = ASGetExceptionErrorCode ();
	END_HANDLER

	if (NULL != agent)
		delete agent;

	if (NULL != hConsultant)
		ConsultantDestroy (hConsultant);

	if (NULL != statusMonitor->progMon)
	{
		if (NULL != opText)
			ASTextDestroy (opText);

		// End the operation.
		if (NULL != statusMonitor->progMon->endOperation)
			statusMonitor->progMon->endOperation (gStatusMonitor->progMonClientData);
	}

	if (0 != errorCode)
		ASRaise (errorCode);
}

/* PIInitializeConversionHandler
** ----------------------------------------------------*/ 
/**
 ** Initialize the AVConversionFromPDFHandler and
 ** register it with Acrobat.
 **
 ** @return true if initialization was successful.
 ** 
 ** @see ASTextSetEncoded
 ** @see ASScriptToHostEncoding
 ** @see AVAppRegisterFromPDFHandler
 **/
ASBool PIInitializeConversionHandler ()
{
	ASBool bInError = false;
	const int kPINumExtensions = 1;
	const char *kPIUniqueID = "com.adobe.acrobatsdk.uncomp";
	const char *kPIFilterDesc = "Uncompressed PDF Files";
	const char *kExtArray[] = {"pdf"};
	
	memset (&gConversionHandler, 0, sizeof(AVConversionFromPDFHandlerRec));
	gConversionHandler.size = sizeof (AVConversionFromPDFHandlerRec);

	// Configure the AVConversionHandler properties.
	gConversionHandler.canDoSync = true;
	gConversionHandler.priority = 1;
	strcpy (gConversionHandler.uniqueID, kPIUniqueID);

	// Create the descriptors for the file types we support.
	AVFileDescRec *fileDesc = (AVFileDescRec*)ASmalloc (sizeof(AVFileDescRec) * kPINumExtensions);
	if (NULL == fileDesc)
		return false;

	memset (fileDesc, 0, (sizeof(AVFileDescRec) * kPINumExtensions));
	for (ASUns16 i = 0; i < kPINumExtensions; i++)
		strcpy (fileDesc[i].extension, kExtArray[i]);

	// Store the descriptors in the conversion handler.
	gConversionHandler.convFilter.fileDescs = fileDesc;
	gConversionHandler.convFilter.numFileDescs = kPINumExtensions;

	// Initialize the callbacks that we implement.
	gConversionHandler.convert = ASCallbackCreateProto (AVConversionConvertFromPDFProc, ConvertFromPDFProc);

	DURING

		// Create the ASText object for the filter description.
		gConversionHandler.convFilter.filterDescription = ASTextNew();
		ASTextSetEncoded (gConversionHandler.convFilter.filterDescription,
							kPIFilterDesc, ASScriptToHostEncoding(kASRomanScript));

		// Register the AVCommandHandler.
		AVAppRegisterFromPDFHandler (&gConversionHandler);

	HANDLER
		bInError = true;
	END_HANDLER	

	return !bInError;
}

/* PITerminateConversionHandler
** ----------------------------------------------------*/ 
/**
 ** Release all resources associated with the handler.
 **/
void PITerminateConversionHandler ()
{
	if (NULL != gConversionHandler.convFilter.fileDescs)
		ASfree (gConversionHandler.convFilter.fileDescs);

	if (NULL != gConversionHandler.convFilter.filterDescription)
		ASTextDestroy (gConversionHandler.convFilter.filterDescription);
}