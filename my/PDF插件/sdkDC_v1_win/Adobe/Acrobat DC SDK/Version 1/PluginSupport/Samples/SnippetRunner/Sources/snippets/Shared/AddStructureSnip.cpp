/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 AddStructureSnip.cpp

 - Illustrates working with logical structure within a PDF document.

 - Operations performed include:

 	1. Tagging PDEText elements within the displayed page.

 - NOTE: the code to scan the page PDEContent for PDEText objects is
   		 limited such the sample only tags PDEText objects located at
   		 the root level. The code does NOT scan content associated
   		 with PDEContainers. This limitation becomes apparent when
   		 tagging a page whose content is already tagged.

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

// Handy macro to detect if a PDSElement is NULL.
#define PDSElementIsNULL(X) CosObjEqual (X, CosNewNull())

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

static const char *PAGE_LIST = "Page List";
static const char *PAGE_LIST_ID = "ADBE:PgListID";
static const char *TEXT_LIST = "Text Element List";
static const char *TEXT_ELEMENT = "A text element";
static const char *XOBJECT_LIST_ID = "ADBE:ImgXObjectListID";
static const char *XOBJECT_LIST = "Image XObject list";


/*-------------------------------------------------------
	Text Structure Implementation
-------------------------------------------------------*/
#include "PDSReadCalls.h"

/* PDDocGetStructureTextListForPage
** ------------------------------------------------------
**
** Returns the text list structure element for a given
** page. If bCreate is true the method creates the
** structure hierarchy, if necessary, before returning
** the text list in pdsTextList.
*/
static ASBool PDDocGetStructureTextListForPage (PDDoc pdDoc, ASInt32 pageNum, PDSElement *pdsTextList, ASBool bCreate)
{
	ASBool success = true;
	ASBool bCreated = false;
	PDSTreeRoot pdsTreeRoot;
	PDSElement pdsPageList, pdsKid;
	char * volatile titleBuf = NULL, numBuf[32];

	// Let's be sure the client has passed in the necessary info
	if (bCreate && (NULL == pdsTextList))
		ASRaise (GenError(genErrBadParm));

	// Initialize our local variables and the return parameter.
	pdsTreeRoot = pdsPageList = pdsKid = CosNewNull();
	if (pdsTextList) *pdsTextList = CosNewNull();

	DURING

		// Get the structure tree root, creating it if necessary if we've been asked to.
		if (!PDDocGetStructTreeRoot (pdDoc, &pdsTreeRoot) && bCreate) {

			PDDocCreateStructTreeRoot (pdDoc, &pdsTreeRoot);
			bCreated = true;
		}

		if (PDSElementIsNULL(pdsTreeRoot))
			E_RETURN(false);

		// Find the page list element, creating it if necessary.
		PDSTreeRootGetElementFromID (pdsTreeRoot, PAGE_LIST_ID, strlen(PAGE_LIST_ID), &pdsPageList);
		if (PDSElementIsNULL(pdsPageList)) {

			if (bCreate) {

				PDSElementCreate (pdDoc, &pdsPageList);
				PDSElementSetType (pdsPageList, ASAtomFromString(PAGE_LIST));
				PDSElementSetID (pdsPageList, (const ASUns8*)PAGE_LIST_ID, strlen(PAGE_LIST_ID));
				PDSTreeRootInsertKid (pdsTreeRoot, pdsPageList, kPDSAfterLast);
				bCreated = true;

			} else
				E_RETURN(true);
		}

		// Now find the text list element for the desired page. If we created either the tree
		// root or the page list, we know that it doesn't exist yet so we can skip this.
		if (!bCreated) {

			ASAtom kidType;
			ASInt32 numKids = PDSElementGetNumKids (pdsPageList), titleLength;

			for (ASInt32 i = 0; i < numKids; i++) {

				// If any of the kids aren't structure elements, then we can't handle this tree.
				kidType = PDSElementGetKid (pdsPageList, i, &pdsKid, NULL, NULL);
				if (kidType != ASAtomFromString("StructElem"))
					ASRaise (PDSEditError(pdsErrBadPDF));

				titleLength = PDSElementGetTitle (pdsKid, NULL);
				if (0 == titleLength)
					ASRaise (PDSEditError(pdsErrBadPDF));

				titleBuf = (char*)ASmalloc (sizeof(char) * (titleLength + 1));
				if (!titleBuf)
					ASRaise(GenError(genErrNoMemory));

				PDSElementGetTitle (pdsKid, (ASUns8*)titleBuf);
				titleBuf[titleLength] = 0;

				// Okay, now compare the title to the requested page number.
				sprintf (numBuf, "%d", pageNum);
				if (0 != strcmp(titleBuf, numBuf)) {

					// No match, so reset and continue.
					pdsKid = CosNewNull();
				}

				// Release the allocated memory.
				ASfree (titleBuf);
				titleBuf = NULL;

				// Break out the loop if we found a match.
				if (!PDSElementIsNULL(pdsKid))
					break;
			}
		}

		if (!PDSElementIsNULL(pdsKid)) {

			if (pdsTextList) *pdsTextList = pdsKid;

		} else if (bCreate) {

			// We need to create the text list element.
			PDSElementCreate (pdDoc, &pdsKid);
			PDSElementSetType (pdsKid, ASAtomFromString(TEXT_LIST));

			// The title of the text list is the number of the page it represents.
			sprintf (numBuf, "%d", pageNum);
			PDSElementSetTitle (pdsKid, (const ASUns8*)numBuf, strlen(numBuf));

			// Add this element as a child of the page list.
			PDSElementInsertKid (pdsPageList, pdsKid, kPDSAfterLast);
			bCreated = true;

			// Assign the return value
			if (pdsTextList) *pdsTextList = pdsKid;
		}

	HANDLER

		if (NULL != titleBuf)
			ASfree (titleBuf);

		success = false;
	END_HANDLER

	return success;
}

/** Creates a structure element for every PDEText object on
	the visible page, that is, for the text aggregates as
	PDFEdit sees them.

	- Add a structure tree root if not already present.
	- Put each PDEText object in a PDEContainer.
	- Add a structure element for each text object,
	referencing the container as a marked content
	element.
	
	@testfile TwoColumnDoc.pdf
	@requires pdf with text
	
	@see CosNewNull
	@see PDDocGetStructTreeRoot
	@see PDDocCreateStructTreeRoot
	@see PDSTreeRootGetElementFromID
	@see PDSElementCreate
	@see PDSElementSetType
	@see PDSElementSetID
	@see PDSTreeRootInsertKid
	@see PDSElementGetNumKids
	@see PDSElementGetKid
	@see PDSElementGetTitle
	@see PDSElementSetTitle
	@see PDSElementInsertMCAsKid
	@see PDERelease
	@see PDPageSetPDEContent
	@see PDPageReleasePDEContent
	@see PDPageRelease
*/
void AddStructureSnip()
{
	PDPage pdPage;
	CosObj oPage;
	char errorMsg[256];

	// Get the displayed page for the active document.
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if(pdDoc == NULL)
		return;
	//page is owned by CDocument object. Don't release it.
	pdPage = (PDPage)document;
	oPage = PDPageGetCosObj (pdPage);

	PDSElement pdsTextList;

	DURING

		// Get the text list element for this page. We should have to create it or the
		// menuitem enabling logic is broken; we don't allow users to add text structure
		// for a page that is already mapped out.

		if(PDDocGetStructureTextListForPage (pdDoc, PDPageGetNumber(pdPage), &pdsTextList, true))
		{
			if (PDSElementIsNULL(pdsTextList))
				ASRaise (GenError(genErrGeneral));
		}

	HANDLER
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);
		AVAlertNote (errorMsg);
		return;
	END_HANDLER

	// Get the PDEContent for the currently displayed page.
	//
	// Our structure elements reference marked content
	// (BDC/BMC) objects. We create marked content objects
	// using PDEContainer elements. So, we have to put each
	// PDEText element inside a PDEContainer.

	PDEContainer volatile pdeContainer = NULL;
	PDEContent volatile pdeTextContent = NULL;
	PDEContent pdeContent = PDPageAcquirePDEContent (pdPage, gExtensionID);

	DURING
		ASInt32 numElems = PDEContentGetNumElems (pdeContent);

		for (ASInt32 i = 0; i < numElems; i++) {

			PDEElement pdeElement = PDEContentGetElem (pdeContent, i);
			if (PDEObjectGetType ((PDEObject)pdeElement) == kPDEText) {

				// Create a PDEContainer and add content to it.
				// Note: The tag of the container is "LI" to match
				// the subtype of the structure element that references
				// this marked content.

				ASInt32 numTextRuns = PDETextGetNumRuns ((PDEText)pdeElement);

				pdeContainer = PDEContainerCreate (ASAtomFromString("LI"), NULL, false);
				pdeTextContent = PDEContentCreate ();

				PDEContentAddElem (pdeTextContent, kPDEBeforeFirst, pdeElement);
				PDEContainerSetContent (pdeContainer, pdeTextContent);

				// Replace the text element in the page with the container
				// holding the text element.

				PDEContentRemoveElem (pdeContent, i);
				PDEContentAddElem (pdeContent, i - 1, (PDEElement)pdeContainer);

				// Create a structure element for this marked content and
				// insert it into listElement.

				PDSElement pdslistElement;
				PDSElementCreate (pdDoc, &pdslistElement);
				PDSElementSetTitle (pdslistElement, (const ASUns8*)TEXT_ELEMENT, strlen(TEXT_ELEMENT));
				PDSElementSetType (pdslistElement, ASAtomFromString("LI"));
				PDSElementInsertKid (pdsTextList, pdslistElement, kPDSAfterLast);

				// Put the marked content into this list element.
				PDSElementInsertMCAsKid (pdslistElement, oPage, pdeContainer, 0);

				// Release the objects we created.
				PDERelease ((PDEObject)pdeTextContent);
				PDERelease ((PDEObject)pdeContainer);
				pdeContainer = NULL;
				pdeTextContent = NULL;
			}
		}

		// Commit the changes that we made to the pdeContent.
		PDPageSetPDEContent (pdPage, gExtensionID);

	HANDLER
		if (pdeContainer)
			PDERelease((PDEObject)pdeContainer);
		if (pdeTextContent)
			PDERelease((PDEObject)pdeTextContent);
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);
		AVAlertNote (errorMsg);
	END_HANDLER

	// Perform the necessary clean-up.
	PDPageReleasePDEContent (pdPage, gExtensionID);

	Console::displayString ("Completed adding structure to text objects.");
}


SNIPRUN_REGISTER(AddStructureSnip, "PD Layer:Structure:Add Text Structure", "Adds structure based on the text elements")
