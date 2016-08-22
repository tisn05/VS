/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

SmartPDPageSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"
#include "SmartPDPage.h"

/** The SmartPDPageSnip snippet demonstrates how to use the CSmartPDPage
    Class.
	@testfile comments.pdf
	@requires open pdf
	@see CSmartPage
*/

/*-----------------------------------------------------------------------
	Implementation
-----------------------------------------------------------------------*/
void SmartPDPageSnip()
{
	DURING
		CDocument document;
		PDDoc frontDoc = (PDDoc)document;
		if(frontDoc == NULL)
			E_RTRN_VOID;

		// Get a CSmartPDPage object
		CSmartPDPage page1(frontDoc, 0);
		ASInt32 number = PDPageGetNumber(page1);

		char strBuf[256];
		sprintf(strBuf, "Demonstrating CSmartPDPage(pddoc, nPageNum)...\n\
The first page of the front document is page %d (zero-based).\n\n", number);
		Console::displayString(strBuf);

		// Get the second CSmartPDPage object
		CSmartPDPage page2;
		page2.AcquirePage(frontDoc, PDDocGetNumPages(frontDoc) - 1);
		number = PDPageGetNumber(page2);

		sprintf(strBuf, "Demonstrating CSmartPDPage method \"AcquirePage\"...\n\
The last page of the front document is page %d (zero-based).\n\n", number);
		Console::displayString(strBuf);

		// Get the third CSmartPDPage object
		AVPageView avPageView = (AVPageView)document;
		PDPage pdPage = AVPageViewGetPage(avPageView);
		CSmartPDPage page3(pdPage);
		number = PDPageGetNumber(page3);

		sprintf(strBuf, "Demonstrating CSmartPDPage(pdPage)...\n\
The currently displayed page of the front document is page %d (zero-based).\n\n", number);
		Console::displayString(strBuf);

		// Get the fourth CSmartPDPage object using the copy constructor
		CSmartPDPage page4(page1);
		number = PDPageGetNumber(page4);

		sprintf(strBuf, "Demonstrating the CSmartPDPage copy constructor...\n\
The first page of the front document is page %d (zero-based).\n\n", number);
		Console::displayString(strBuf);

		// Demonstrating the CSmartPDPage assignment operator
		page4 = page2;
		number = PDPageGetNumber(page4);

		sprintf(strBuf, "Demonstrating the CSmartPDPage assignment operator...\n\
The last page of the front document is page %d (zero-based).\n\n", number);
		Console::displayString(strBuf);

		// Get the fifth CSmartPDPage object
		CSmartPDPage page5;
		// Add a blank page to the end of the document
		// Default mediaBox 8.5 X 11 page size
		ASFixedRect mediaBox = {0, 0, ASInt32ToFixed(612), ASInt32ToFixed(792)};
		page5.AssignAndTakeOwnership(PDDocCreatePage(frontDoc, 
			PDDocGetNumPages(frontDoc) - 1, mediaBox));
		number = PDPageGetNumber(page5);

		sprintf(strBuf, "Demonstrating the CSmartPDPage method \"AssignAndTakeOwnership\"...\n\
The newly create page is page %d (zero-based).\n\n", number);
		Console::displayString(strBuf);

	HANDLER
		ASInt32 iErrCode = ASGetExceptionErrorCode();
		const ASInt32 BUF_SIZE = 256;
		char errBuf[BUF_SIZE];
		AVAlertNote(ASGetErrorString(iErrCode, errBuf, BUF_SIZE));
	END_HANDLER
}

SNIPRUN_REGISTER(SmartPDPageSnip, "PD Layer:SmartPDPageSnip", 
				 "Demonstrates how to use the CSmartPDPage class.")