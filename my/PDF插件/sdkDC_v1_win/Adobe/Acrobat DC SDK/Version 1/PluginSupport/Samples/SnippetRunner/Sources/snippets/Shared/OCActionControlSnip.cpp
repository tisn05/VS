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

 OCActionControlSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* Associates the OCG with any text found in the pdeContent passed in */
static void AddTextToOCG(PDOCG ocg, PDEContent pdeContent);


/** This snippet creates an OCG and associates it with the text found on the first page of the document. The snippet then creates two
	bookmarks, one with an action that turns makes the OCG visible, and one that makes the OCG invisible. We create goto-actions and 
	link them to the OC actions (through the dictionaries "Next" field). This way, clicking on the bookmark performs two funtions,
	the goto-page and the set OCG visibility.
	@testfile MultipageDoc.pdf
	@requires pdf with text. Having multiple pages helps test if bookmarks work correctly
	@see CreateTextContentOCSnip for information on how the text OCG is created (the AddTextToOCG function is replicated).
	@see PDOCGCreate
	@see PDOCGSetIntent
	@see PDDocGetOCConfig
	@see PDOCGSetInitialState
	@see PDOCConfigGetOCGOrder
	@see CosObjGetType
	@see CosArrayLength
	@see CosArrayInsert
	@see PDOCConfigSetOCGOrder
	@see PDDocAcquirePage
	@see PDPageAcquirePDEContent
	@see PDPageSetPDEContent
	@see PDPageReleasePDEContent
	@see PDPageRelease
	@see PDDocGetCosDoc
	@see PDActionNew
	@see CosNewArray
	@see CosNewNameFromString
	@see CosArrayInsert
	@see PDActionGetCosObj
	@see CosNewNull
	@see PDOCGGetCosObj
	@see ASAtomFromString
	@see CosDictPutKeyString
	@see PDDocGetBookmarkRoot
	@see PDBookmarkAddNewChild
	@see PDBookmarkSetAction
	@see PDPageGetMediaBox
	@see PDViewDestCreate
	@see PDActionNewFromDest
*/
void OCActionControlSnip(void){
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	// create OCG named "SDK Text Content"
	ASText OCGtitle =ASTextFromScriptText("SDK Text Content",kASRomanScript);
	PDOCG ocg = PDOCGCreate(pdDoc, OCGtitle);
	
	// Set its intent to "view" to indicate it is to be used in a viewer application
	ASAtom intentArray[] = {ASAtomFromString ("View"), ASAtomNull};
	PDOCGSetIntent(ocg, intentArray);

	// Indicate it should initially be turned on (visible)
	PDOCConfig docConfig = PDDocGetOCConfig(pdDoc);
	PDOCGSetInitialState(ocg, docConfig, true);


	// we want it to be in the layers UI in acrobat, so we need to indicate where the label should
	// be placed (by specifying the order in the document's OCProperties dictionary)
	
	// get existing OCG ui ordering for the document
	CosObj existingOCOrder;
	PDOCConfigGetOCGOrder(docConfig,&existingOCOrder);
	if (CosObjGetType(existingOCOrder) != CosArray){
		AVAlertNote("Error in PDF document, OCProperties has invalid Order array");
		return;
	}

	// if there are pre-existing OCGroups, just add the new one onto the end
	ASInt32 numberOfObjs = CosArrayLength(existingOCOrder);
	CosArrayInsert(existingOCOrder,numberOfObjs,PDOCGGetCosObj(ocg));
	// save back to the OCProperties for the document
	PDOCConfigSetOCGOrder(docConfig,existingOCOrder);

	// grab the front page of the document
	PDPage theFirstPage = PDDocAcquirePage(pdDoc, 0);
	PDEContent pdeContent = PDPageAcquirePDEContent(theFirstPage,0);

	// Add the text from the first page to the ocg
	AddTextToOCG(ocg, pdeContent);

	// Commit the changes that we made to the pdeContent.
	PDPageSetPDEContent(theFirstPage, gExtensionID);

	PDPageReleasePDEContent(theFirstPage, gExtensionID);
	PDPageRelease(theFirstPage);
	if (OCGtitle!= NULL){
		ASTextDestroy(OCGtitle);
	}
	
	// get the cos document 
	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);
	// we want a new action, one that sets the OCG state on
	PDAction myAct1 = PDActionNew(pdDoc, ASAtomFromString("SetOCGState"));
	// and another for setting the OCG state off.
	PDAction myAct2 = PDActionNew(pdDoc, ASAtomFromString("SetOCGState"));

	// Create and populate the CosArray for the On Action
	CosObj newOnArray = CosNewArray(cosDoc, false,3);
	CosObj onName = CosNewNameFromString(cosDoc, true, "ON");
	CosArrayInsert(newOnArray, 0, onName);
	CosArrayInsert(newOnArray, 1, PDOCGGetCosObj(ocg));
	CosArrayInsert(newOnArray, 2, CosNewNull());

	// Create and populate the CosArray for the Off Action
	CosObj newOffArray = CosNewArray(cosDoc, false,3);
	CosObj offName = CosNewNameFromString(cosDoc, true, "OFF");
	CosArrayInsert(newOffArray, 0, offName);
	CosArrayInsert(newOffArray, 1, PDOCGGetCosObj(ocg));
	CosArrayInsert(newOffArray, 2, CosNewNull());

	// Assign the newly created CosArrays to the Action's dictionary
	CosObj onObj = PDActionGetCosObj(myAct1);
	CosObj offObj = PDActionGetCosObj(myAct2);
	CosDictPutKeyString(offObj,"State",newOffArray);
	CosDictPutKeyString(onObj,"State" ,newOnArray);

	// We now have the two PDActions that set the OC visibility, we also
	// create set view destination actions for goto-behaviour

	// create a destination to the page and set it as action of the boomark
	// In general, the user should adjust to get a proper page view when the document is open,
	// so the link would inherit the zoom factor to display pages.
 	ASFixedRect mediaBox, rect;
    PDPageGetMediaBox(theFirstPage, &mediaBox );
    rect.left   = 0;
    rect.top    = mediaBox.top;
    rect.right  = 0;
    rect.bottom = 0;
    
    PDViewDestination pdvDest = PDViewDestCreate(pdDoc,
                                theFirstPage,
                                ASAtomFromString("XYZ"),
                                &rect,
                                PDViewDestNULL,
                                0);
	PDAction gotoAction = PDActionNewFromDest(pdDoc,pdvDest, pdDoc);
	// now we need to get the CosObj for the OCG actions and set the "Next" dictionary value to the goto action
	CosObj cosGoToAction = PDActionGetCosObj(gotoAction);
	CosDictPutKeyString(onObj, "Next", cosGoToAction);
	CosDictPutKeyString(offObj, "Next", cosGoToAction);
	

	// get the document's root bookmark
	PDBookmark docBookmark = PDDocGetBookmarkRoot(pdDoc);

	// Add the two new bookmarks	
	PDBookmark firstBookmark = PDBookmarkAddNewChild(docBookmark,"Page with text");
	PDBookmark secondBookmark = PDBookmarkAddNewChild(docBookmark,"Page without text");
	PDBookmarkSetAction(firstBookmark, myAct1);
	PDBookmarkSetAction(secondBookmark, myAct2);
	
#ifndef ACROBAT_LIBRARY
	/* force a refresh of the layers panel... */
	AVMenubar theMB = AVAppGetMenubar();
	AVMenuItem viewMenu = AVMenubarAcquireMenuItemByName(theMB,"ShowHideOptCont");
	if (viewMenu != NULL){
		// we toggle the layers panel to force the redraw.
		AVMenuItemExecute(viewMenu);
		AVMenuItemExecute(viewMenu);
		AVMenuItemRelease(viewMenu);
	}
#endif
}

SNIPRUN_REGISTER(OCActionControlSnip, "PD Layer:Optional Content:Create:Bookmark Action OCG","Creates\
 an optional content group and associates it with the first page of text. It also creates two bookmarks, one that\
 turns the content on, and the other that turns the content off.")

void AddTextToOCG(PDOCG ocg, PDEContent pdeContent){
	volatile PDEContainer pdeContainer = NULL;
	volatile PDEContent pdeTextContent = NULL;
	DURING
		ASInt32 numElems = PDEContentGetNumElems(pdeContent);
		for (ASInt32 t = 0; t < numElems; t++){
			PDEElement pdeElement = PDEContentGetElem(pdeContent,t);
			if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEText){

				// candidate PDEElements for optional content are PDEForms, PDEImages and PDEContainers
				// We need to shroud our text object in a container, passing in the ocg we want it associated with
				CosObj ocgObj = PDOCGGetCosObj(ocg);

				pdeContainer = PDEContainerCreate(ASAtomFromString("OC"), &ocgObj, false);
				pdeTextContent = PDEContentCreate();

				PDEContentAddElem(pdeTextContent, kPDEBeforeFirst, pdeElement);
				PDEContainerSetContent(pdeContainer, pdeTextContent);

				// Replace the text element in the page with the container
				// holding the text element.

				PDEContentRemoveElem(pdeContent, t);
				PDEContentAddElem(pdeContent, t - 1, reinterpret_cast<PDEElement>(pdeContainer));
				
				// Release the objects we created.
				PDERelease(reinterpret_cast<PDEObject>(pdeTextContent));
				PDERelease(reinterpret_cast<PDEObject>(pdeContainer));
				pdeContainer = NULL;
				pdeTextContent = NULL;
			}
			if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEContainer){
				// if we have a container, we just recurse
				AddTextToOCG(ocg,PDEContainerGetContent(reinterpret_cast<PDEContainer>(pdeElement)));
			}
			if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEGroup){
				// if we have a container, we just recurse
				AddTextToOCG(ocg,PDEGroupGetContent(reinterpret_cast<PDEGroup>(pdeElement)));
			}
		}
	HANDLER
		if (pdeTextContent!=NULL){
			PDERelease(reinterpret_cast<PDEObject>(pdeTextContent));
			pdeTextContent = NULL;
		}
		if (pdeContainer != NULL){
			PDERelease(reinterpret_cast<PDEObject>(pdeContainer));
			pdeContainer = NULL;		
		}
		Console::displayString("Exception when adding text to OCG - incomplete results follow");
	END_HANDLER
}

