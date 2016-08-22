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

 MakeBookmarkSnip.cpp

 *********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** Makes a bookmark named 'Current Page' for the current 
	page at the top (visible) level of the bookmark tree.
	@testfile MultipageDoc.pdf
	@requires pdf, multiple pages helps testing of bookmark
	@see PDDocGetBookmarkRoot
	@see PDBookmarkAddNewChild
	@see PDBookmarkSetColor
	@see PDPageGetMediaBox
	@see PDViewDestCreate
	@see PDViewDestIsValid
	@see PDActionNewFromDest
	@see PDBookmarkSetAction
	@see PDPageRelease
*/
void MakeBookmarkSnip ()
{
	PDPage volatile pdPage = NULL;

	DURING
	
	// get the page
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if(pdDoc == NULL) E_RTRN_VOID;

	pdPage = (PDPage)document; //CDocument releases page
	
	// We make top-level bookmarks only, so add a new bookmark to the root
	PDBookmark bmRoot, bmNode;
	bmRoot = PDDocGetBookmarkRoot (pdDoc);
	bmNode = PDBookmarkAddNewChild (bmRoot, "Current Page"); 

	// set blue color for new bookmark
	PDColorValueRec blue;
	blue.space = PDDeviceRGB;
	blue.value[0] = fixedZero;
	blue.value[1] = fixedZero;
	blue.value[2] = ASInt32ToFixed(1);
	
	PDBookmarkSetColor (bmNode, &blue);

	// create a destination to the page and set it as action of the boomark
	// In general, the user should adjust to get a proper page view when the document is open,
	// so the link would inherit the zoom factor to display pages.
 	ASFixedRect mediaBox, rect;
    PDPageGetMediaBox(pdPage, &mediaBox );
    rect.left   = 0;
    rect.top    = mediaBox.top;
    rect.right  = 0;
    rect.bottom = 0;
    
    PDViewDestination pdvDest = PDViewDestCreate(pdDoc,
                                pdPage,
                                ASAtomFromString("XYZ"),
                                &rect,
                                PDViewDestNULL,
                                0);
   
	// The following is an alternative code for destination settings:  
	// ASFixedRect rect = {fixedZero,fixedZero,ASInt32ToFixed(100),ASInt32ToFixed(200)};
	//PDViewDestination pdvDest = PDViewDestCreate(pdDoc,pdPage, ASAtomFromString("FitBV"),&rect, PDViewDestNULL, 0);

	// create and set action
	if(PDViewDestIsValid(pdvDest)) {
		PDAction pdAction;
		pdAction = PDActionNewFromDest(pdDoc,pdvDest, pdDoc);
		PDBookmarkSetAction(bmNode,pdAction);
	}

	HANDLER

		// exception handling
		char errorMsg[256];
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);	
	END_HANDLER
}

SNIPRUN_REGISTER(MakeBookmarkSnip, "PD Layer:Document:Make Bookmark", "Creates a bookmark at the top level for the current page.")
