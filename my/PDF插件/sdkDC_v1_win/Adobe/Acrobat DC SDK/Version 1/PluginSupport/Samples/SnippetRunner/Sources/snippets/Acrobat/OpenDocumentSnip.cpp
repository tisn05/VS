/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2008 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------
Snippet that opens the about snippets document
*/

/* 
	Un-comment this pre-processor macro to exercise 
	Unicode File Syetem support on Windows platform.
*/
// #define UNICODE

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"
#include "CDocument.h"

/** This snippet demonstrates opening a document in the ExampleFiles folder.

	@testfile Acrobat_8_ProductLine_Overview.pdf
*/

void OpenDocumentSnip(void){
#ifdef UNICODE
	TCHAR * fileName = TEXT("ExampleFiles/[Unicode_Encoded_File_Name].pdf");
	CDocument::OpenDocument((const ASUTF16Val*)fileName);
#else
	CDocument::OpenDocument("ExampleFiles/acrobatpro_datasheet.pdf");
#endif
}

SNIPRUN_REGISTER(OpenDocumentSnip, "Open Document","Demonstrates opening a document in both Roman and Unicode encoded filenames.")
