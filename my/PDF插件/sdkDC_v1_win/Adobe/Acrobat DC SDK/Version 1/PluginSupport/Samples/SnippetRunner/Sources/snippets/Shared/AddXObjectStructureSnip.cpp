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

 AddXObjectStructureSnip.cpp

 - Illustrates working with logical structure within a PDF document.

 - Operations performed include:

 	1. Tagging image XObjects within the displayed document.

 - NOTE: the code to scan the page PDEContent for PDEText objects is
   		 limited such the sample only tags PDEText objects located at
   		 the root level. The code does NOT scan content associated
   		 with PDEContainers. This limitation becomes apparent when
   		 tagging a page whose content is already tagged.

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

static const char *XOBJECT_LIST_ID = "ADBE:ImgXObjectListID";
static const char *XOBJECT_LIST = "Image XObject list";

bool addedStructure = false;


/*-------------------------------------------------------
	XObject Structure Implementation
-------------------------------------------------------*/

/* XObjectEnumProc
** ------------------------------------------------------
**
** Called for each XObject resource found on in a page's
** resource dictionary. Re-raises all errors up
** to the calling function.
**
** If an image XObject is found, we:
**
**  - Add a structure tree root if not already present.
**  - Add an image XObject list element to the tree root
**	  if not already present.
**  - Add a structure element child to the list element
**	  for the XObject.
**	- Finally, add an OBJR to the XObject structure element.
**
** Return true to continue enumeration, false to halt enumeration.
*/
static ACCB1 ASBool ACCB2 XObjectEnumProc (CosObj key, CosObj value, void *clientData)
{
	PDPage pdPage = (PDPage)clientData;
	PDDoc pdDoc = PDPageGetDoc (pdPage);

	DURING

		// key should be a stream. That attribute dictionary associated
		// with that stream should have a /Subtype key. If either of
		// those conditions is not true, we skip this resource.

		if ((CosObjGetType(key) != CosStream) || (!CosDictKnownKeyString (CosStreamDict(key), "Subtype")))
			E_RETURN(true);

		// If we haven't been passed an image XObject, skip this resource.
		CosObj oSubType = CosDictGetKeyString (CosStreamDict(key), "Subtype");
		if ((CosObjGetType(oSubType) != CosName) || (CosNameValue(oSubType) != ASAtomFromString("Image")))
			E_RETURN(true);
	HANDLER
		return false;
	END_HANDLER

	PDSTreeRoot treeRoot;
	ASAtom xObjName = ASAtomNull;
	PDSElement pdsListElement = CosNewNull();
	PDSElement xObjElement;
	ASUns8 * volatile elemIdBuf = NULL;

	DURING
		// Get the structure tree root, creating it if necessary.
		if (!PDDocGetStructTreeRoot (pdDoc, &treeRoot))
			PDDocCreateStructTreeRoot(pdDoc, &treeRoot);

		ASInt32 numKids = PDSTreeRootGetNumKids (treeRoot);

		if (numKids > 0) {

			// Try to find the XObject list structure element.
			for (ASInt32 i = 0; i < numKids; i++) {

				PDSElement element;
				PDSTreeRootGetKid (treeRoot, i, &element);
				ASInt32 length = PDSElementGetID (element, NULL);

				if (length > 0) {

					elemIdBuf = (ASUns8*)ASmalloc (sizeof(char) * length);
					PDSElementGetID (element, (ASUns8*)elemIdBuf);
					elemIdBuf[length] = 0;

					if (strcmp ((char*)elemIdBuf, XOBJECT_LIST_ID) == 0)
						pdsListElement = element;

					ASfree (elemIdBuf);
					elemIdBuf = NULL;
				}

				if (!CosObjEqual(pdsListElement, CosNewNull()))
					break;
			}
		}

		if (CosObjEqual(pdsListElement, CosNewNull())) {

			// We didn't find our XObject list structure, so create it now.
			PDSElementCreate (pdDoc, &pdsListElement);
			PDSElementSetType (pdsListElement, ASAtomFromString("L"));
			PDSElementSetTitle (pdsListElement, (const ASUns8*)XOBJECT_LIST, strlen(XOBJECT_LIST));
			PDSElementSetID (pdsListElement, (const ASUns8*)XOBJECT_LIST_ID, strlen(XOBJECT_LIST_ID));
			PDSTreeRootInsertKid (treeRoot, pdsListElement, PDSTreeRootGetNumKids(treeRoot));
		}

		// Get the name of the XObject, remembering that it's not a
		// required dictionary key.

		CosObj oXObjName = CosDictGetKeyString (CosStreamDict(key), "Name");
		if (!CosObjEqual(oXObjName, CosNewNull()))
			xObjName = CosNameValue (oXObjName);
	HANDLER
		if (elemIdBuf)
			ASfree (elemIdBuf);
		return false;
	END_HANDLER

	DURING

		// Create a structure element for the XObject we've been passed
		// and add it to the image XObject list element.

		PDSElementCreate (pdDoc, &xObjElement);
		PDSElementSetType (xObjElement, ASAtomFromString("LI"));
		if (xObjName != ASAtomNull) {
			const char *name = ASAtomGetString(xObjName);
			PDSElementSetTitle (xObjElement, (const ASUns8*)name, strlen(name));
		}
		PDSElementInsertKid (pdsListElement, xObjElement, kPDSAfterLast);
	HANDLER
		return false;
	END_HANDLER

	DURING
		// Add an OBJR for the XObject to the structure element.
		CosObj oPage = PDPageGetCosObj (pdPage);
		PDSElementInsertOBJAsKid (xObjElement, oPage, key, 0);
		addedStructure = true;
	HANDLER

		// We may receive the "There is already a table entry with the same name"
		// error from PDSElementInsertOBJAsKid. That happens if this XObject is
		// already referenced on another page. In that case, we remove the XObject
		// structure element we just added and continue with the enumeration.

		ASInt32 err = ASGetExceptionErrorCode();
		if (err == PDSEditError(pdsErrAlreadyExists)) {
			PDSElementRemoveKid (pdsListElement, xObjElement);
			return true;
		}

		return false;
	END_HANDLER
	return true;
}

/** Creates a structure element for each image XObject
	resource found in the document.

	Enumerates the resource dictionary of each page.
	
	@testfile image.pdf
	@requires a pdf with at least one image xobject
	
	@see PDPageGetDoc
	@see CosObjGetType
	@see CosDictKnownKeyString
	@see CosDictGetKeyString
	@see CosStreamDict
	@see CosNameValue
	@see CosNewNull
	@see PDDocGetStructTreeRoot
	@see PDDocCreateStructTreeRoot
	@see PDSTreeRootGetNumKids
	@see PDSTreeRootGetKid
	@see PDSElementGetID
	@see ASfree
	@see CosObjEqual
	@see PDSElementCreate
	@see PDSElementSetType
	@see PDSElementSetTitle
	@see PDSElementSetID
	@see PDSTreeRootInsertKid 
	@see PDPageGetCosObj
	@see PDSElementInsertOBJAsKid
	@see PDSEditError
	@see PDSElementRemoveKid
	@see PDDocGetNumPages
	@see AVSysGetCursor
	@see PDDocAcquirePage
	@see PDDocEnumResources
	@see AVSysGetCursor
	@see AVSysSetCursor
	@see AVSysGetStandardCursor
	@see PDPageRelease
*/
void AddXObjectStructureSnip()
{
	ASInt32 numPages;
	PDPage volatile pdPage = NULL;

	// Get the active document.
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if(pdDoc == NULL)
		return;

	numPages = PDDocGetNumPages (pdDoc);
	CosObjEnumProc cbProc = ASCallbackCreateProto(CosObjEnumProc, XObjectEnumProc);

#ifndef ACROBAT_LIBRARY
	// Show wait cursor since this could take a while...
	AVCursor currentCursor = AVSysGetCursor();
	AVSysSetCursor (AVSysGetStandardCursor(WAIT_CURSOR));
#endif

	DURING
		// Iterate through each page in the document, enumerating
		// its resource dictionary.
		for (ASInt32 i = 0; i < numPages; i++) {

			// Acquire the page.
			pdPage = PDDocAcquirePage (pdDoc, i);

			// Our enumProc raises on error.
			PDDocEnumResources (pdDoc, i, i, ASAtomFromString("XObject"), cbProc, pdPage);

			PDPageRelease (pdPage);
			pdPage = NULL;
		}

		if(addedStructure)
			Console::displayString("Added structure to XObject");
		else
			AVAlertNote("no structure added");
	HANDLER
		char errorMsg[256];
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);
		AVAlertNote (errorMsg);
	END_HANDLER

	// Reset the cursor and release the pdPage if it's still acquired.
#ifndef ACROBAT_LIBRARY
	AVSysSetCursor (currentCursor);
#endif
	if (pdPage)
		PDPageRelease (pdPage);
	ASCallbackDestroy( (void*) cbProc );
}


SNIPRUN_REGISTER(AddXObjectStructureSnip, "PD Layer:Structure:Add XObject Structure", "Adds structure based on the XObjects.")
