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

 PDPageNotifyContentsChangeSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** Forces a redraw of the page specified in the SnippetRunner's parameter dialog. Use this snippet if you suspect 
	you are writing code that has some sort of UI update problem.
	@see PDPageNotifyContentsDidChange
	@see PDPageRelease
*/
void PDPageNotifyContentsChangeSnip(ParamManager * thePM){
	ASInt32 pageNumber;
	thePM->getNextParamAsInt(pageNumber);
	CDocument document;
	PDDoc theDoc = (PDDoc)document;
	if (theDoc == NULL){
		return;
	}
	ASInt32 numPages =  PDDocGetNumPages(theDoc);
	if ((pageNumber<1)||(pageNumber>numPages)){
		AVAlertNote("Required page number not in front document");
		return;
	}
	PDPage thePage = PDDocAcquirePage(theDoc, pageNumber-1);
	if (thePage == NULL){
		AVAlertNote("Cannot obtain page");
		return;
	}
	PDPageNotifyContentsDidChange(thePage);
	PDPageRelease(thePage);
}

SNIPRUN_REGISTER_WITH_DIALOG(PDPageNotifyContentsChangeSnip, "Document page refresh", "Forces a redraw of a specified page in the document - default is the first page", "1")


