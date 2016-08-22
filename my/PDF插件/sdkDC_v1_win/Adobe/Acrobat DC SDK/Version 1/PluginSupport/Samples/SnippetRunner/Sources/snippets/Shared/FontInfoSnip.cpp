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

static ACCB1 ASBool ACCB2 myFontEnumProc(PDFont font, PDFontFlags* ecname, void* clientData);
static void ReportFontProtection(PDFont fontP);

/**
	This snippet enumerates the document font information (name, type,
	encoding, if embedded, and the font protection bits) and dump the info into debug window.
	
	@testfile cmykText.pdf
	@requires pdf with text
	@see PDDocGetNumPages
	@see ASCallbackCreateProto
	@see PDDocEnumFonts
	@see ASCallbackDestroy
	@see PDFontGetEncodingName
	@see PDFontGetSubtype
	@see PDFontGetName
  */
void FontInfoSnip()
{
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL)
		return;

	ASInt32  numOfPg = PDDocGetNumPages (pdDoc);

	PDFontEnumProc myFontEnumProcCB = ASCallbackCreateProto(PDFontEnumProc, &myFontEnumProc);
	PDDocEnumFonts (pdDoc, 0, numOfPg-1, myFontEnumProcCB, NULL, NULL, NULL);
	ASCallbackDestroy((void*)myFontEnumProcCB);
}

SNIPRUN_REGISTER(FontInfoSnip, "PD Layer:Fonts:Get Font Info",
"Get fonts information of a document.")

/* myFontEnumProc
** ----------------------------------------------------------------------
**
** Get font info with the user defined proc
**
*/
static ACCB1 ASBool ACCB2 myFontEnumProc(PDFont font, PDFontFlags* ecname, void* clientData)
{
	char fontinfo[256], fontname[128], embedded[20], type[50];
	
	ASBool isEmbd;
	isEmbd	= PDFontIsEmbedded(font);
	if (ASBoolToBool(isEmbd) == true) {
		strcpy (embedded, "embedded");
	}
	else {
		strcpy (embedded, "");
	}	
	const unsigned char * encoding = PDFontGetEncodingName(font);
	fontname[0] = '\0';
	PDFontGetName(font, fontname, sizeof(fontname));		
	strcpy(type, ASAtomGetString (PDFontGetSubtype (font)));

	if (encoding == NULL) {
		snprintf (fontinfo,sizeof(fontinfo), "%s  %s  %s  %s", fontname, type, "Custom-encoding", embedded);
	}
	else {
		snprintf (fontinfo,sizeof(fontinfo), "%s  %s  %s  %s", fontname, type, encoding, embedded);
	}
	Console::displayString (fontinfo);
	ReportFontProtection(font);

	return true;
}

/* Reports Embedding permissions */
static void ReportFontProtection(PDFont fontP)
{
	CosObj fontObj = PDFontGetCosObj(fontP);
	PDEFont pdeFont = PDEFontCreateFromCosObj (&fontObj);
	PDSysFont pdSysFont = NULL;
	PDEFontAttrs attrs;

	Console::displayString ("Corresponding system font protection:");

	// In this API, if there exists the system font, get the corresponding system font
	// if there is no such a systom font, and if the font is embedded, create a tempory system font with the embedded font
	// Otherwise, return NULL
	pdSysFont = PDEFontGetSysFont(pdeFont);
	if (pdSysFont)
	{
		PDSysFontGetAttrs(pdSysFont, &attrs, sizeof(attrs));
		
		if ((attrs.protection & kPDEFontNoEditableEmbedding) == 0)
			Console::displayString ("Editable Embedding Enabled");
		else if ((attrs.protection & kPDEFontNoEmbedding) == 0)
			Console::displayString("Print and Preview Embedding Enabled");
		else
			Console::displayString("Embedding Not Allowed!");

	}
	else
	{
		Console::displayString("System font not available and the font isn't embedded.");
	}
	Console::displayString("------");
}
