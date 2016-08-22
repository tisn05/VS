/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ------------------------------------------------------------------------------------
AVPrintSnip.cpp

Demonstrates how to control printing to ps file programmatically with AVDocPrintPagesWithParams.

Implementation:
- check the doc security settings before printing the file
- print to a PS file with a full range of options
  
--------------------------------------------------------------------------------------------*/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** 
	This snippet checks the doc security settings before printing the file. 
	It then prints a document with a full range of options.
	The document is saved into the user's document folder (category kAVSFDocument, folder, kAVSFDocuments).

  @see AVAppGetActiveDoc
  @see AVDocPermRequest
  @see AVPageViewGetGrayRect
  @see AVDocGetPageView
  @see AVDocPrintPagesWithParams
  @see AVAlertNote
*/
ACCB1 void ACCB2 AVPrintSnip()
{
	CDocument document;
	PDDoc d = (PDDoc)document;
	if(d == NULL){
		AVAlertNote ("A document needs to be open...");
		return;
	}
	
	// If security has been set on a file so that it is not printable, the document won't
	// print, but no error is raised by the viewer. Check the security before printing the file.
	if (PDDocPermRequest(d, PDPermReqObjDoc, PDPermReqOprPrintHigh, 0) != 0 &&
		PDDocPermRequest(d, PDPermReqObjDoc, PDPermReqOprPrintLow, 0) != 0)
	{
		AVAlertNote ("Printing is not permitted");
			return;
	}

	AVDocPrintParamsRec r;
	memset (&r, 0, sizeof (AVDocPrintParamsRec));

	r.size = sizeof(AVDocPrintParamsRec);
	r.pageSpec = PDAllPages;
	
	// if true, displays dialog boxes; otherwise does not display dialog boxes.
	r.interactive = false;

	r.emitToPrinter = false;  //  only emitToPrinter or emitToFile must be true
	r.emitToFile = true;

#ifdef UNIX_PLATFORM
	r.filePathName = SnippetRunnerUtils::getOutputPath("test_print.ps");
	if(r.filePathName == NULL) {
		AVAlertNote("Could not find outputpath");
		return;
	}
#else
	ASFileSys theFileSys;
	ASInt32 retVal = AVAcquireSpecialFilePathName(kAVSCUser, kAVSFDocuments,"test_print.ps", &theFileSys, &(r.filePathName));
	if ((retVal != kAVSEOkay) && (retVal != kAVSEDoesntExist)){
		AVAlertNote("Could not find documents directory");
		return;
	}
#endif

	r.firstPage = 0;
	r.lastPage = PDDocGetNumPages(d) - 1;
	r.psLevel = 2;
	r.binaryOK = false;
	r.shrinkToFit = false;
	r.cancelDialog = false;
	r.doColorSeparations = false;

	/* file output options: PostScript or EPS, with or without a
	preview. Must be one of the following values:
	kAVEmitFilePS - PostScript file
	kAVEmitFileEPSNoPrev - EPS file with no preview
	kAVEmitFileEPSMacStdPrev - EPS file with standard preview
	kAVEmitFileEPSMacExtPrev - EPS file with extended preview
	*/
	r.emitFileOption = kAVEmitFilePS;
	r.emitFlags = 0;

	/* font option is one of the following:
	kAVEmitFontNoFonts (Obsolete after Acrobat 3.0) Embed no fonts.
	kAVEmitFontAllEmbType1 (Obsolete after Acrobat 3.0) Embed all Type 1 embedded fonts.
	kAVEmitFontAllType1 (Obsolete after Acrobat 3.0) Embed all Type 1 fonts.
	kAVEmitFontEmbeddedFonts Emit all embedded fonts.
	kAVEmitFontAllFonts Emit all fonts.
	*/
	r.emitFontOption = kAVEmitFontAllFonts;
	r.ranges = NULL;
	r.numRanges = 0;
	r.printAsImage = false;
	r.reverse = false;
	r.transparencyLevel = 3; //The transparency level range from 1 to 5

	strcpy(&r.destProfile[0], "PostScript printing");

	ASInt32 error = 0;

	DURING
		AVDocPrintPagesWithParams((AVDoc)document, &r);
#ifdef UNIX_PLATFORM
		ASFileSysReleasePath(NULL, r.filePathName);
		r.filePathName = NULL;
#else
		ASFileSysReleasePath(theFileSys, r.filePathName);
		theFileSys = NULL;
		r.filePathName = NULL;
#endif
	HANDLER
#ifdef UNIX_PLATFORM
		ASFileSysReleasePath(NULL, r.filePathName);
		r.filePathName = NULL;
#else
		if (theFileSys!=NULL){
			ASFileSysReleasePath(theFileSys, r.filePathName);
			theFileSys = NULL;
			r.filePathName = NULL;
		}
#endif
		error = ERRORCODE;
		ASRaise (error);
	END_HANDLER
	if (!error)
		AVAlertNote ("The file was printed successfully.");
}

// Register the snippet
SNIPRUN_REGISTER(AVPrintSnip, "AV Layer:Document:Print Document", "Control printing programmatically with AVDocPrintPagesWithParams. Output is to the Documents folder for the current user.")


