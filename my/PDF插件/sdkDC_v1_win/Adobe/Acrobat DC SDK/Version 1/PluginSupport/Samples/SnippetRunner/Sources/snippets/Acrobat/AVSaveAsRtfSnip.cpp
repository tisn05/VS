/**************************************************************************
 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------
 AVSaveAsRtfSnip.cpp

This snippet converts a PDF to a RTF ( Rich Text Format ) file using Acrobat File Conversion Handlers.
 
**********************************************************************************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static ASInt32 gNumToPDFExt;
static ASInt32 gNumFromPDFExt;
static AVConversionFromPDFHandler RightHandler;

const char* RTF_FILE = "SaveAsRtf.rtf";


/**
	User defined callback: AVConversionFromPDFEnumProc
*/
static ACCB1 ASBool ACCB2 myAVConversionFromPDFEnumProc(AVConversionFromPDFHandler handler,AVConversionEnumProcData data)
{
	AVFileFilterRec filter = handler->convFilter;
	ASUns16 numFileExt = filter.numFileDescs;
	
	// go through the conversion handlers to find a handler for rtf extention files. 
	for (ASInt32 i = 0; i < numFileExt; i++)
	{
		if (strlen(handler->convFilter.fileDescs[i].extension)>0)
		{
			// found it, fill in the handler and return false to stop going on.
			if(!strcmp(handler->convFilter.fileDescs[i].extension,"rtf")) {
				RightHandler = handler;
				return false;
			}
		}
	}
	return true;
}


/** This snippet converts a PDF to a RTF ( Rich Text Format ) file using Acrobat File Conversion Handlers.

	The SaveAsRtf functionality is enhanced in Acrobat 7 to preserve multi-column layout as well as 
   page size and margins.

Implementation:
<ul>
<li>	Enumerate all "From PDF" conversion handlers to find the one handling rtf extention files. 
<li>	Convert the PDF to RTF and save it to a local file.
</ul>

	@testfile TwoColumnDoc.pdf
	@requires non-empty pdf to generate rtf data
	@see AVConversionEnumFromPDFConverters
	@see AVConversionConvertFromPDFWithHandler
*/  
static ACCB1 void ACCB2 AVSaveAsRtfSnip()
{
	// get a PDF open in front.
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if(pdDoc == NULL){
		return;
	}
		
	// enumerate from PDF conversion handlers to find the "rtf" handler.
	AVConversionEnumFromPDFConverters(myAVConversionFromPDFEnumProc, NULL);
	SnippetRunnerUtils::ShowDebugWindow();

	// save the rtf file to the snippetRunner's output files folder. 
	ASPathName OutPath = SnippetRunnerUtils::getOutputPath(RTF_FILE);
	if(OutPath==NULL) 
		OutPath = ASFileSysCreatePathName (NULL, ASAtomFromString("Cstring"), RTF_FILE, 0);
	if(OutPath==NULL) {
		AVAlertNote("Cannot open an output file.");
		return;
	}
	
	// do conversion
	AVConversionStatus status=AVConversionConvertFromPDFWithHandler(RightHandler, 
					NULL,kAVConversionNoFlags, pdDoc, OutPath, ASGetDefaultFileSys(),NULL);

	// check the returned status and show message 
	if(status == kAVConversionSuccess)
		AVAlertNote("The Rtf file was saved in SnippetRunner output folder.");
	else if(status == kAVConversionFailed)
		AVAlertNote("The Rtf file conversion failed.");
	else if(status == kAVConversionSuccessAsync)
		AVAlertNote("The conversion will continue asynchronously.");
	else if(status == kAVConversionCancelled)
		AVAlertNote("The conversion was cancelled.");
}

// Register the snippet
SNIPRUN_REGISTER(AVSaveAsRtfSnip, "AV Layer:Document:Save As Rich Text File", 
	"This snippet saves the PDF to a Rich Text Format file using the Acrobat File Conversion Handler. ")
