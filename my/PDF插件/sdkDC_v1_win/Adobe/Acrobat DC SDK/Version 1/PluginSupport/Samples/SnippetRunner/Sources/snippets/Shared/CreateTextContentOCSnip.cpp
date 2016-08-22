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

void AddTextToOCG(PDOCG ocg, PDEContent content);

/** an example of how to create optional content applicable to streams.
	An optional content group is created based on any text content found in the first
	page of the document. Any existing OCGs containing text become embedded within the 
	newly created OCG (this means the text will not be visible unless both OCGs are set
	to visible). If we find container or group objects, we recurse through their contents.
	<ul>
	<li> Create the OCG </li>
	<li> Set the intent to "view" </li>
	<li> Set initial visibility (on) </li>
	<li> Add the OCG to the order array in the document's OCProperties dictionary </li>
	<li> Get the firt page's PDEContent</li>
	<li> Iterate through PDEText objects within this content: </li> 
	<li><ul>
		<li> Create a new PDEContainer (and associated PDEContent) to house the PDEText object (it should have tag "OC") 
			and indirectly reference the ocg cosobj created for it</li>
		<li> Add the PDE(text)Element to the content </li>
		<li> Replace the original element with the newly created container in the Page's PDEContent </li>
		<li> Associate the PDEContainer with the ocg previously created </li>
		</ul>
	<li> Commit the changes made to the PDEContent </li>
	</ul>
	@testfile cmykText.pdf
	@requires pdf with text on first page
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
void CreateTextContentOCSnip(void){
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
	PDOCGSetIntent (ocg, intentArray);

	// Indicate it should initially be turned on (visible)
	PDOCConfig docConfig = PDDocGetOCConfig (pdDoc);
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
	// save back to the OCProperties for the document
	PDOCConfigSetOCGOrder(docConfig,existingOCOrder);

	// grab the front page of the document
	PDPage theFirstPage = PDDocAcquirePage(pdDoc, 0);
	PDEContent pdeContent = PDPageAcquirePDEContent(theFirstPage,0);

	AddTextToOCG(ocg,pdeContent);
	
	// Commit the changes that we made to the pdeContent.
	PDPageSetPDEContent(theFirstPage, gExtensionID);
	PDPageNotifyContentsDidChange(theFirstPage);

	PDPageReleasePDEContent(theFirstPage, gExtensionID);
	PDPageRelease(theFirstPage);
	if (OCGtitle!= NULL){
		ASTextDestroy(OCGtitle);
	}	
	
	Console::displayString("Text content OC created");
}

SNIPRUN_REGISTER(CreateTextContentOCSnip, "PD Layer:Optional Content:Create:Text Content OCG","Creates\
 an optional content group dictionary that includes any text on the first page of the document.")

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

