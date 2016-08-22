/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1999-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 TextExtractionSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"
#include "ParamManager.h"
#include "stdio.h"

FILE* pOutput;

ACCB1 ASBool ACCB2 WordEnumProc(PDWordFinder, PDWord, ASInt32, void*);
bool ExtractText(PDDoc, ASInt32, ASInt32, ASBool, PDWordFinderConfig, FILE*);

/**
** 
** TextExtractionSnip.cpp
**
** Snippet that demonstrates text extraction using the new WordFinder 
** creation API function based on various configurable options.
	@testfile MultipageDoc.pdf
	@requires open pdf with text to extract
** @see PDDocCreateWordFinderEx
** @see PDWordGetString
** @see PDWordGetLength
** @see PDWordGetAttrEx
** @see PDWordFinderDestroy
** @see PDWordFinderEnumWords
** @see ASFileSysDisplayStringFromPath
**
**/

void TextExtractionSnip(ParamManager* pm)
{
	// Get the active document.
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if(pdDoc == NULL)	return;
 
	if ( pm->getNumberParams() != 3) 
	{
		AVAlertNote("Parameters required: [StartPage EndPage ExtractToUnicode(1/0)] Example: [1 5 0]");
		return;
	}
	ASInt32 nStartPage, nEndPage, nToUnicode;
	ASBool bToUnicode;
	pm->getNextParamAsInt(nStartPage);
	pm->getNextParamAsInt(nEndPage);
	pm->getNextParamAsInt(nToUnicode);
	if (nToUnicode != 0 && nToUnicode != 1)
	{
		AVAlertNote("Incorrect value for third parameter: [ExtractToUnicode] should be 0 or 1");
		return;
	}
	bToUnicode = (nToUnicode==0)?(false):(true);

	// Set up WordFinder creation options record
	PDWordFinderConfigRec wfConfig;
	
	memset(&wfConfig, 0, sizeof(PDWordFinderConfigRec));
	
	wfConfig.recSize = sizeof(PDWordFinderConfigRec);
	wfConfig.ignoreCharGaps = true;
	wfConfig.ignoreLineGaps = false;
	wfConfig.noAnnots = true;
	wfConfig.noEncodingGuess = true;		// leave non-Roman single-byte font alone

	// Std Roman treatment for custom encoding; overrides the noEncodingGuess option
	wfConfig.unknownToStdEnc = false;		
		
	wfConfig.disableTaggedPDF = false;		// legacy mode WordFinder creation
	wfConfig.noXYSort = false;
	wfConfig.preserveSpaces = false;
	wfConfig.noLigatureExp = false;
	wfConfig.noHyphenDetection = false;
	wfConfig.trustNBSpace = false;
	wfConfig.noExtCharOffset = false;		// text extraction efficiency
	wfConfig.noStyleInfo = false;			// text extraction efficiency
	wfConfig.decomposeTbl = NULL;			// Unicode character replacement
	wfConfig.decomposeTblSize = 0;
	wfConfig.charTypeTbl = NULL;			// Custom char type table
	wfConfig.charTypeTblSize = 0;
		
	ASPathName pathName;
	pathName = SnippetRunnerUtils::getOutputPath("output.txt");

#ifndef MAC_PLATFORM
	//on the Mac ASFileSysDisplayStringFromPath returns an Mac OS 9 style path.
	pOutput = fopen(ASFileSysDisplayStringFromPath (NULL, pathName), "w+b");
#else
	ASPlatformPath platformPath;
	ASFileSysAcquirePlatformPath (NULL, pathName, ASAtomFromString("POSIXPath"), &platformPath);
	POSIXPath_Ptr path = ASPlatformPathGetPOSIXPathPtr (platformPath);
	pOutput = fopen(path, "w+b");
	ASFileSysReleasePlatformPath(NULL, platformPath);
#endif
	if (pOutput)
	{
		if(ExtractText(pdDoc, nStartPage - 1, nEndPage - 1, 
			bToUnicode, &wfConfig, pOutput))
				Console::displayString("Text extraction completed.");
	}
	else
		AVAlertNote("Text extraction aborted.");

	
	if (pathName) ASFileSysReleasePath(NULL, pathName);

	if (pOutput) fclose(pOutput);
}

/**
** Word Enumerator callback for text extraction 
**
** @param whObj IN The WordFinder object with which to enumerate words.
** @param pdWord OUT The PDWord enumerated.
** @param pgNum OUT The page number from which the PDWord is extracted.
** @param clientData IN Data passed in to the call. In this case the FILE stream pointer.
** @return true to continue word enumeration, flase otherwise.
*/
ACCB1 ASBool ACCB2 WordEnumProc(PDWordFinder wfObj, PDWord pdWord, ASInt32 pgNum, void* clientData)
{
	char str[128];

	PDWordGetString(pdWord, str, sizeof(str));

	for (int i = 0; i < PDWordGetLength(pdWord); i++)
		fputc(str[i], (FILE*) clientData);

	if (PDWordGetAttrEx(pdWord, 0) & WXE_LAST_WORD_ON_LINE)
	{	// taking care of line/page breaks
		if (PDWordGetAttrEx(pdWord, 1) & WXE_WORD_IS_UNICODE)
		{	
			for (int i=0; i<2; i++)
			{
				fputc(0x00, (FILE*) clientData); fputc(0x0d, (FILE*) clientData);
				fputc(0x00, (FILE*) clientData); fputc(0x0a, (FILE*) clientData);
			}
		}
		else
		{
			for (int i=0; i<2; i++)
			{
				fputc(0x0d, (FILE*) clientData); fputc(0x0a, (FILE*) clientData);
			}
		}
	}
	else
	{
		// taking care of word spacing
		if (PDWordGetAttrEx(pdWord, 0) & (WXE_ADJACENT_TO_SPACE|WXE_HAS_TRAILING_PUNC&!WXE_HAS_HYPHEN))
			if (PDWordGetAttrEx(pdWord, 1) & WXE_WORD_IS_UNICODE)
			{
				fputc(0x00, (FILE*) clientData);
				fputc(0x20, (FILE*) clientData);
			}
			else
				fputc(' ', (FILE*) clientData);
	}

	return true;
}

/**
** Called to perform WordFinder creation and text extraction on a PDF document
**
** @param pdDoc IN The PDDoc object on which to perform text extraction.
** @param startPg IN The page to start text extraction.
** @param endPg IN The page to end text extraction (inclusive).
** @param toUnicode IN Whether to extract text to Unicode encoding.
** @param pConfig IN Pointer to a WordFinder Configuration Record.
** @param pOutput IN/OUT Pointer to an output FILE stream to which the extracted 
**	text will be written.
** @return true to indicate text extraction operation a success, false otherwise.
*/
bool ExtractText(PDDoc pdDoc, ASInt32 startPg, ASInt32 endPg, 
				 ASBool toUnicode, PDWordFinderConfig pConfig, FILE* pOutput)
{
	if (startPg < 0 || endPg <0 || startPg > endPg || endPg > PDDocGetNumPages(pdDoc) - 1)
	{
		AVAlertNote("Exceeding starting or ending page number limit of current document."); 
		return false;
	}

	PDWordFinder pdWordFinder = NULL;

DURING	
	pdWordFinder = PDDocCreateWordFinderEx(pdDoc, WF_LATEST_VERSION, toUnicode, pConfig);
	
	if (toUnicode) fprintf(pOutput, "%c%c", 0xfe, 0xff);

	for (int i = startPg; i <= endPg; i++)
		PDWordFinderEnumWords(pdWordFinder, i, ASCallbackCreateProto(PDWordProc, &WordEnumProc), pOutput);

	PDWordFinderDestroy(pdWordFinder);
	E_RETURN(true);
HANDLER
	char buf[256], errmsg[256];
	sprintf(buf, "[ExtractText()]Error %d: %s",  ErrGetCode(ERRORCODE), ASGetErrorString(ERRORCODE, errmsg, sizeof(errmsg)));
	AVAlertNote(buf);
	if (pdWordFinder) PDWordFinderDestroy(pdWordFinder);
	if( pOutput) fclose(pOutput);
	return false;
END_HANDLER

return true;
}

SNIPRUN_REGISTER_WITH_DIALOG(TextExtractionSnip, "PD Layer:Text Extraction", 
	"Demonstrates the use of WordFinder APIs for text extraction. \n\
Parameters: StartPage EndPage ExtractToUnicode(1/0); \n\
Example: 1 5 0; Default: 1 1 0", "1 1 0")
