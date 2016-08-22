/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------
 
   FontInfoSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static const int kMaxInfoLen = 50;
static int gTextRunNumber = 0;
static ACCB1 ASBool ACCB2 PDTextSelectEnumTextProcCB (void* procObj, PDFont font, ASFixed size, PDColorValue color, char* text, ASInt32 textLen);

/* 
	Called for each run in the text selection. Dumps out font information to debug window.
*/
static ACCB1 ASBool ACCB2 PDTextSelectEnumTextProcCB(void* procObj, PDFont font, ASFixed size, 
											   PDColorValue color, char* text, ASInt32 textLen)
{
	string strOutput;
	char buff[kMaxInfoLen];

	char fontName[kMaxInfoLen];
	ASInt32 fontSize = 0;	

	memset(buff, 0, sizeof (buff));
	memset(fontName, 0, sizeof (fontName));

	sprintf(buff,"Text run number %d",gTextRunNumber++);
	strOutput = buff;
	strOutput += "\n";
	// Get strings of fontname, colorspace name, and font size
	ASInt32 bufsize = PDFontGetName(font, fontName, sizeof(fontName));
	fontSize = ASFixedRoundToInt32(size);
	sprintf(buff,"Font name = %s, Size = %d",fontName,fontSize);
	strOutput += buff;
	strOutput += "\n";
	sprintf(buff,"Encoding name = %s,Subtype = %s",(char *)PDFontGetEncodingName(font), ASAtomGetString(PDFontGetSubtype(font)));
	strOutput += buff;
	strOutput += "\n";
	switch (color->space)
	{
	case PDDeviceGray:
		sprintf(buff,"ColorSpace = DeviceGray\n");
		break;
	case PDDeviceRGB:
		sprintf(buff,"ColorSpace = DeviceRGB\n");
		break;
	case PDDeviceCMYK:
		sprintf(buff,"ColorSpace = DeviceCMYK\n");
		break;
	default:
		sprintf(buff,"Unknown color space");
		break;
	}
	strOutput += buff;
	Console::displayString(strOutput.c_str());
	return true;
}

/** Ennumerates the text selection extracting the font information for each of the text runs.
	@testfile TwoColumnTaggedDoc.pdf
	@see PDTextSelectEnumText
	@see ASCallbackCreateNotification
	@see PDFontGetName
	@see ASFixedRoundToInt32
	@see ASCallbackDestroy
*/
void TextInfoSnip()
{
	gTextRunNumber = 0;
	CDocument document;
	AVDoc avDoc = (AVDoc)document;
	if (avDoc == NULL){
		return;
	}

	ASAtom theSelectionType = AVDocGetSelectionType(avDoc);
	if (theSelectionType == ASAtomFromString("Text")){
		PDTextSelect ts = static_cast<PDTextSelect>(AVDocGetSelection(avDoc));

		ASInt32 error = 0;
		// Create the PDTextSelectEnumTextProc callback
		PDTextSelectEnumTextProc cbPDTextSelectEnumTextProc = ASCallbackCreateProto(PDTextSelectEnumTextProc, &PDTextSelectEnumTextProcCB);
		
		// Enumerate the text runs in PDText
		PDTextSelectEnumText(ts, cbPDTextSelectEnumTextProc, NULL);
		ASCallbackDestroy((void*)cbPDTextSelectEnumTextProc);
	}
	else {
		AVAlertNote("Not a valid text selection"); 
	}
}

SNIPRUN_REGISTER(TextInfoSnip, "AV Layer:PageView:Get Text Info",
				 "Usage: Select text and execute this snippet. The results are displayed in the console window.")


