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

 CreateTextContentOCSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static void AddImageToOCG(PDOCG ocg, PDEContent content);

/** an example of how to create optional content applicable to image XObjects.
	An optional content group is created based on any images found in the first
	page of the document. Images with pre-existing associations with optional content 
	have this association replaced with the new optional content group. The old OCG 
	is not modified or deleted.
	<ul>
	<li> Create the OCG </li>
	<li> Set the intent to "view" </li>
	<li> Set initial visibility (on) </li>
	<li> Add the OCG to the order array in the document's OCProperties dictionary </li>
	<li> Get the firt page's PDEContent</li>
	<li> Iterate through objects within this content: </li> 
	<li> For any image objects, associate the ocg </li>
	<li> Commit the changes made to the PDEContent </li>
	</ul>
	@testfile cottage.pdf
	@requires pdf file with image xobject
	@see PDOCGCreate
	@see PDOCGSetIntent
	@see PDDocGetOCConfig
	@see PDOCGSetInitialState
	@see PDOCConfigGetOCGOrder
	@see PDOCGGetCosObj
	@see PDOCConfigSetOCGOrder
	@see PDOCMDCreate
	@see PDDocAcquirePage
	@see PDPageAcquirePDEContent
	@see PDEContentGetNumElems
	@see PDEContainerGetContent
	@see PDEContentGetElem
	@see PDEContainerCreate
	@see PDEContentCreate
	@see PDEContentAddElem
	@see PDEContainerSetContent
	@see PDEContentRemoveElem
	@see PDEContentAddElem
	@see PDERelease
	@see PDPageSetPDEContent
	@see PDPageReleasePDEContent
	@see PDPageRelease
	@see ASTextDestroy
*/
void CreateImageContentOCSnip(void){
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	// create OCG named "SDK Image Content"
	ASText OCGtitle =ASTextFromScriptText("SDK Image Content",kASRomanScript);
	PDOCG ocg = PDOCGCreate(pdDoc, OCGtitle);
	
	// Set its intent to "view" to indicate it is to be used in a viewer application
	ASAtom intentArray[] = {ASAtomFromString ("View"), ASAtomNull};
	PDOCGSetIntent (ocg, intentArray);

	PDOCConfig docConfig = PDDocGetOCConfig (pdDoc);

	// grab the front page of the document
	PDPage theFirstPage = PDDocAcquirePage(pdDoc, 0);
	PDEContent pdeContent = PDPageAcquirePDEContent(theFirstPage,0);

	// Add the images to the OCG
	AddImageToOCG(ocg,pdeContent);

	// Indicate it should initially be turned on (visible)
	PDOCGSetInitialState (ocg, docConfig, true);


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
	
	// Set the order in the default configuration
	PDOCConfigSetOCGOrder(docConfig,existingOCOrder);

	// Commit the changes that we made to the pdeContent.
	PDPageSetPDEContent(theFirstPage, gExtensionID);

	PDPageReleasePDEContent(theFirstPage, gExtensionID);
	PDPageRelease(theFirstPage);
	if (OCGtitle!= NULL){
		ASTextDestroy(OCGtitle);
	}
/*
	// force a refresh of the layers panel... 
	AVMenubar theMB = AVAppGetMenubar();
	AVMenuItem viewMenu = AVMenubarAcquireMenuItemByName(theMB,"ShowHideOptCont");
	if (viewMenu != NULL){
		// we toggle the layers panel to force the redraw.
		AVMenuItemExecute(viewMenu);
		AVMenuItemExecute(viewMenu);
		AVMenuItemRelease(viewMenu);
	}
*/
}

SNIPRUN_REGISTER(CreateImageContentOCSnip, "PD Layer:Optional Content:Create:Image content OCG","Creates\
 an optional content group and associates it with any images found on the first page of the front document")

/* Adds all images contained in the pdeContent to the optional content group ocg */
void AddImageToOCG(PDOCG ocg, PDEContent pdeContent){
	volatile PDEContainer pdeContainer = NULL;
	volatile PDEContent pdeTextContent = NULL;
	DURING
		ASInt32 numElems = PDEContentGetNumElems(pdeContent);
		for (ASInt32 t = 0; t < numElems; t++){
			PDEElement pdeElement = PDEContentGetElem(pdeContent,t);
			ASInt32 obType = PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement));
			if (obType == kPDEImage){
				Console::displayString("Adding image object to OCG");
				CosObj  t;
				PDEImage tmp = reinterpret_cast<PDEImage>(pdeElement);
				PDEImageGetCosObj (tmp, &t);
				CosObj ocgObj = PDOCGGetCosObj(ocg);
				CosDictPutKeyString(t, "OC", ocgObj);
			}
		}
	HANDLER
		Console::displayString("Exception when adding image to OCG - incomplete results follow");
	END_HANDLER
}

