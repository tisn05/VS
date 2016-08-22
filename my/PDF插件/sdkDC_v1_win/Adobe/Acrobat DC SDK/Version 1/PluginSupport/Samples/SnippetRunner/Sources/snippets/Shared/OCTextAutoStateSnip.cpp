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

 OCTextAutoStateSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static void AddTextToOCG(PDOCG ocg, PDEContent content);

/** We create an OCG named "SDK Autostate Content" and associate it with the text on the first page of the document. We set the
	usage information for the OCG to indicate it should have a maximum visibility of 150% (1.5 zoom factor) and a minimum visibility
	of 100% ((1.0 zoom factor). The autostate dictionary of the document default configuration is updated to inidcate the (zoom) 
	usage information of this OCG should be used to set the visibility of content controlled by the OCG. 
	@testfile TwoColumnTaggedDoc.pdf
	@requires open pdf with text
	@see CreateTextOCSnip
	@see CosNewDict
	@see FloatToFixed
	@see CosNewFixed
	@see CosDictPutKeyString
	@see PDOCConfigGetCosObj
	@see CosNewNameFromString
	@see ASAtomFromString
	@see CosNewArray
	@see PDOCGGetCosObj
	@see CosArrayPut
	@see CosNewArray
*/
void OCTextAutoStateSnip(void){
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	// create OCG named "SDK Autostate Content"
	ASText OCGtitle =ASTextFromScriptText("SDK Autostate Content",kASRomanScript);
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

	// Add the text from the pdecontent to the ocg
	AddTextToOCG(ocg,pdeContent);

	// Change the ocg usage dictionary to indicate a specific zoom usage.
	// get the cos document 
	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);

	// set up the OCG's usage dictionary to specify the max/min zoom factors
	CosObj zoomDict = CosNewDict(cosDoc,false,2);
	CosDictPutKeyString(zoomDict,"max",CosNewFixed(cosDoc,true,FloatToASFixed(1.5))); 
	CosDictPutKeyString(zoomDict,"min",CosNewFixed(cosDoc,true,FloatToASFixed(1))); 
	PDOCGSetUsageDictEntry(ocg, ASAtomFromString("Zoom"),zoomDict);
	
	// this sets up the OCG with particular usage. We need to indicate that this is not incidental,
	// interactive applications should use this to determine visibility. To do this, we specify such
	// in the default configuration's autostate array.
	CosObj cosDocConfig = PDOCConfigGetCosObj(docConfig);
	
	// the event is view, the category is zoom for the autostate control dictionary
	CosObj viewName= CosNewNameFromString(cosDoc, true, "View");
	CosObj zoomName= CosNewNameFromString(cosDoc, true, "Zoom");
	// create the autostate control dictionary
	CosObj usageAppDict = CosNewDict(cosDoc,false,2);

	// we have a list of categories for each entry in the autostate array.
	CosObj usageArray = CosNewArray(cosDoc, false, 1);	
	// our list just has the single "Zoom" entry
	CosArrayPut(usageArray,0,zoomName);
	
	// populate the autostate entry dictionary
	CosDictPutKeyString(usageAppDict,"Event",viewName);
	CosDictPutKeyString(usageAppDict,"Category",usageArray);

	// create the array for OCGs
	CosObj ocgArray = CosNewArray(cosDoc, true,1);
	// add the OCG
	CosArrayInsert(ocgArray,0,PDOCGGetCosObj(ocg));

	// add the OCG array to the AS entry we created above
	CosDictPutKeyString(usageAppDict,"OCGs",ocgArray);

	// the AS entry for the properties dictionary contains a list of control entries, create the array
	CosObj usageAppArray = CosNewArray(cosDoc, false, 1);	
	// add the entry created above to the array
	CosArrayPut(usageAppArray,0,usageAppDict);
	
	// I need to now add the usageAppDict to the properties dictionary.
	CosDictPutKeyString(cosDocConfig,"AS",usageAppArray);

	// Commit the changes that we made to the pdeContent.
	PDPageSetPDEContent(theFirstPage, gExtensionID);
	PDPageNotifyContentsDidChange(theFirstPage);

	PDPageReleasePDEContent(theFirstPage, gExtensionID);
	PDPageRelease(theFirstPage);
	if (OCGtitle!= NULL){
		ASTextDestroy(OCGtitle);
	}
}

SNIPRUN_REGISTER(OCTextAutoStateSnip, "PD Layer:Optional Content:Create:Text AutoState OCG","Creates\
 an optional content group and associated menbership dictionary that includes any\
 text on the first page of the document. This text is only visible with a zoom factor between 100% and 150%")

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

				PDEContentAddElem (pdeTextContent, kPDEBeforeFirst, pdeElement);
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

