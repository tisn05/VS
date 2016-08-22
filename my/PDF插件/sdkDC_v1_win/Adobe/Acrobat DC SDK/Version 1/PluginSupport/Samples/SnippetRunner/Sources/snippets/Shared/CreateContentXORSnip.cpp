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

 CreateContentXOROCSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* prototype for utility method to add some text to the content passed 
   in on the specified page. */
static PDEText AddText(PDPage pdPage, PDEContent pdeContent);
/* Utility method to take all the content on the first page and add it to a new OCG. */
static void AddContentToOCG(PDOCG ocg, PDEContent pdeContent);

/* Utility method to dump the Visibility Expression of a OCMD */
static void DumpVE(CosObj veObj);

/** an example of implementing an exclusive or (xor) relationship with content in a pdf.
	An optional content group is created based on any content found in the first
	page of the document. 
	
	The ocg created to control the page content is added to an optional content 
	membership dictionary (ocmd). This ocmd is used to relate this content to new 
	content, specifying the new content is to be visible only if the existing content 
	is invisible.
	
	The ocg created to control the page content is added to the UI (layers panel) to 
	allow the user to specify whether it should be visible or not. 
	
	Steps to creating OCGs associated with content: <ul>
	<li> Create the OCG </li>
	<li> Set the intent to "view" </li>
	<li> Set initial visibility (on) </li>
	<li> Add the OCG to the order array in the document's OCProperties dictionary </li>
	<li> Create an OC membership dictionary associated with the OCG - set the relationship to kOCMDVisibility_AllOff </li>
	<li> Get the first page's PDEContent</li>
	<li> Iterate through objects within this content: </li> 
	<li> Associate the content with the OCG
	<li> Create some new content ("this page intentionally blank" text)</li>
	<li> Associate the new content with the OCMD created above </li>
	<li> Commit the changes made to the PDEContent </li>
	</ul>

	To find out whether the OCMD has a visibility expression entry, use the API: <ul>
	<li> PDOCMDGetVisibilityExpression </li>
	</ul>
	@testfile blank.pdf
	@requires open pdf
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
	@see PDEContentGetElem
	@see PDEContainerCreate
	@see PDEContentCreate
	@see PDEContentAddElem
	@see PDEContainerSetContent
	@see PDEContentRemoveElem
	@see PDEContentAddElem
	@see PDEElementSetOCMD
	@see PDERelease
	@see PDPageSetPDEContent
	@see PDPageReleasePDEContent
	@see PDPageRelease
	@see ASTextDestroy
	@see PDOCMDFindOrCreateEx
	@see PDOCMDGetVisibilityExpression
*/
void CreateContentXORSnip(void){
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	// create OCG named "SDK XOR Text Content"
	ASText OCGtitle =ASTextFromScriptText("SDK XOR Text Content",kASRomanScript);
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

	// now acquire the membership dictionary, will be used to maintain the relationship
	// between the page OCG and our new content
	PDOCG PDOCGarray[2];
	PDOCGarray[0] = ocg;
	PDOCGarray[1] = NULL;

	CosDoc cd = PDDocGetCosDoc(pdDoc);
	CosObj veObj = CosNewArray(cd, false, 2);
	CosObj nameObj = CosNewName(cd, false, ASAtomFromString("Not"));
	CosArrayPut(veObj, 0, nameObj);
	CosObj ocgObj = PDOCGGetCosObj(ocg);
	CosArrayPut(veObj, 1, ocgObj);

	PDOCMD theMD = PDOCMDFindOrCreateEx(pdDoc, PDOCGarray, kOCMDVisibility_AllOff, veObj);

	// The following lines demonstrate how to retrieve Visibility Expression.
	// They are not required for creating OCMD
	// Start of retrieving Visibility Expression
	CosObj veArr;
	ASBool bHasVE = PDOCMDGetVisibilityExpression(theMD, &veArr);

	char strMsg[256];
	sprintf(strMsg, "The OCMD %s Visibility Expression.\n", bHasVE ? "has":"has no");
	Console::displayString(strMsg);

	if(bHasVE) {
		sprintf(strMsg, "The Visibility Expression is: \n");
		Console::displayString(strMsg);
		DumpVE(veArr);
		sprintf(strMsg, "\nEnd of dumping the Visibility Expression.\n\n");
		Console::displayString(strMsg);
	}
	// End of retrieving Visibility Expression

	// grab the front page of the document
	PDPage theFirstPage = PDDocAcquirePage(pdDoc, 0);
	PDEContent pdeContent = PDPageAcquirePDEContent(theFirstPage,0);
	AddContentToOCG(ocg, pdeContent);

	// we now have our new content with the text asisgned to an OCG.
	// We need to create the new alternative text and add the two objects to a parent object.
	
	// we need to create a new container for our "intentional blank text" text
	PDEContainer newChildContainer = PDEContainerCreate(ASAtomNull, NULL, false);
	// and the associated text content
	PDEContent childContent = PDEContentCreate();

	// populate the content with the errr, content
	PDEText element = AddText(theFirstPage, childContent);
	if (element == NULL){
		// there is some problem creating the text, free the objects and return
		// this should leave the document having an OCG, without the XOR text
		PDERelease(reinterpret_cast<PDEObject>(childContent));
		PDERelease(reinterpret_cast<PDEObject>(newChildContainer));
		childContent = NULL;
		newChildContainer = NULL;
		PDPageReleasePDEContent(theFirstPage, gExtensionID);
		PDPageRelease(theFirstPage);
		if (OCGtitle!= NULL){
			ASTextDestroy(OCGtitle);
		}
		AVAlertNote("There was an error creating the text content to associate with the OCG");
	}
	// bind the content to the container
	PDEContainerSetContent(newChildContainer, childContent);
	// add the content to the original page content as a child
	PDEContentAddElem(pdeContent, kPDEBeforeFirst, reinterpret_cast<PDEElement>(newChildContainer));

	// assign the content to the membership dictionary created above
	PDEElementSetOCMD(reinterpret_cast<PDEElement>(newChildContainer),theMD);
	// Release the objects we created for the "alternative" text.
	PDERelease(reinterpret_cast<PDEObject>(childContent));
	PDERelease(reinterpret_cast<PDEObject>(newChildContainer));
	newChildContainer = NULL;
	childContent = NULL;
	
	// Commit the changes that we made to the pdeContent.
	PDPageSetPDEContent(theFirstPage, gExtensionID);

	PDPageReleasePDEContent(theFirstPage, gExtensionID);
	PDPageRelease(theFirstPage);
	if (OCGtitle!= NULL){
		ASTextDestroy(OCGtitle);
	}
}

SNIPRUN_REGISTER(CreateContentXORSnip, "PD Layer:Optional Content:Create:Content XOR OCG","Creates\
 an optional content group and associated menbership dictionary that includes any \
 content on the first page of the document. It adds a new text element then sets an XOR relationship with\
 the existing text")


/* Adds text to the page passed in, returning the created PDEText object
*/
static PDEText AddText(PDPage pdPage, PDEContent pdeContent)
{
	ASFixedRect cropBox;
	char errorMsg[256];
	ASInt32 errorCode = 0;

	PDPageGetCropBox (pdPage, &cropBox);

	// Initialize the font descriptor then create the font reference.
	// Because we're using one of the base 14 Type 1 fonts, we only
	// need to pass a small amount of information to PDEFontCreate().

	PDEFont pdeFont = NULL;
	PDEFontAttrs pdeFontAttrs;

	memset(&pdeFontAttrs, 0, sizeof(pdeFontAttrs));
	pdeFontAttrs.name = ASAtomFromString("Times-Roman");
	pdeFontAttrs.type = ASAtomFromString("Type1");

	DURING
		// Create the font reference.
		pdeFont = PDEFontCreate(&pdeFontAttrs,sizeof(pdeFontAttrs), 0, 255, 0, 0, ASAtomNull, 0, 0, 0, 0);
	HANDLER
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);
		AVAlertNote (errorMsg);
		return NULL;
	END_HANDLER

	PDEColorSpace pdeColorSpace = PDEColorSpaceCreateFromName(ASAtomFromString("DeviceGray"));
	PDEGraphicState gState;
	ASFixedMatrix textMatrix;

	// The graphics state controls the various style properties of the text
	// including color, weight, and so forth.

	memset (&gState, 0, sizeof(PDEGraphicState));
	gState.strokeColorSpec.space = gState.fillColorSpec.space = pdeColorSpace;
	gState.miterLimit = fixedTen;
	gState.flatness = fixedOne;
	gState.lineWidth = fixedOne;

	// Fill out the text matrix, which determines the point
	// size of the text and where it will is drawn on the page.

	memset (&textMatrix, 0, sizeof(textMatrix));
	textMatrix.a = ASInt16ToFixed(12);
	textMatrix.d = ASInt16ToFixed(12);
	textMatrix.h = cropBox.left + (cropBox.right - cropBox.left)/2 - fixedSeventyTwo;
	textMatrix.v = cropBox.top - (cropBox.top - cropBox.bottom)/2 - fixedThirtyTwo;

	PDEText volatile pdeText = NULL;
	DURING

		// Create a new PDEText element and add a kPDETextRun object to it.
		pdeText = PDETextCreate();
		PDETextAdd (pdeText, kPDETextRun, 0, (Uns8 *)"This page intentionally blank", 30,
					pdeFont, &gState, sizeof(gState), NULL, 0, &textMatrix, NULL);

		// Insert text element into page content.
		PDEContentAddElem (pdeContent, kPDEAfterLast, (PDEElement)pdeText);

		// Commit the changes to the PDEContent.
		PDPageSetPDEContent(pdPage, gExtensionID);

		// Advertise that we changed the contents so the viewer redraws the
		// page and other clients can re-acquire the page contents if needed.
		PDPageNotifyContentsDidChange (pdPage);

	HANDLER
		// Store the error code.
		errorCode = ASGetExceptionErrorCode();
	END_HANDLER

	// Release any objects we may have created or acquired.
	// Note : PDERelease correctly handles NULL, so we don't
	// need to test for valid objects.

	PDERelease ((PDEObject) pdeColorSpace);
	PDERelease ((PDEObject) pdeFont);

	return pdeText;
}

static void AddContentToOCG(PDOCG ocg, PDEContent pdeContent){
	volatile PDEContainer pdeContainer = NULL;
	volatile PDEContent pdeNewContent = NULL;
	DURING
		ASInt32 numElems = PDEContentGetNumElems(pdeContent);
		for (ASInt32 t = 0; t < numElems; t++){
			PDEElement pdeElement = PDEContentGetElem(pdeContent,t);

			// We need to shroud our text object in a container, passing in the ocg we want it associated with
			CosObj ocgObj = PDOCGGetCosObj(ocg);

			pdeContainer = PDEContainerCreate(ASAtomFromString("OC"), &ocgObj, false);
			pdeNewContent = PDEContentCreate();

			PDEContentAddElem(pdeNewContent, kPDEBeforeFirst, pdeElement);
			PDEContainerSetContent(pdeContainer, pdeNewContent);

			// Replace the element in the page with the container
			// holding the element.

			PDEContentRemoveElem(pdeContent, t);
			PDEContentAddElem(pdeContent, t - 1, reinterpret_cast<PDEElement>(pdeContainer));
			
			// Release the objects we created.
			PDERelease(reinterpret_cast<PDEObject>(pdeNewContent));
			PDERelease(reinterpret_cast<PDEObject>(pdeContainer));
			pdeContainer = NULL;
			pdeNewContent = NULL;
		}
	HANDLER
		if (pdeNewContent!=NULL){
			PDERelease(reinterpret_cast<PDEObject>(pdeNewContent));
			pdeNewContent = NULL;
		}
		if (pdeContainer != NULL){
			PDERelease(reinterpret_cast<PDEObject>(pdeContainer));
			pdeContainer = NULL;		
		}
		Console::displayString("Exception when adding content to OCG - incomplete results follow\n");
	END_HANDLER
}

static void DumpVE(CosObj veObj)
{
	char strMsg[256];
	if (CosObjGetType(veObj) != CosArray){
		sprintf(strMsg, "The Visibility is not valid");
		Console::displayString(strMsg);
	} else {
		ASAtom atom;
		CosObj name;
		char strOCGName[256];
		ASInt32 ocgNameLen = 0;

		ASInt32 arrSize = CosArrayLength(veObj);

		for(int i = 0; i < arrSize; i++) {
			CosObj cosobj = CosArrayGet(veObj, i);
			CosType costype = CosObjGetType(cosobj);

			switch(costype) {
				case CosName: // The first element of Visibility Expression is a name
					atom = CosNameValue(cosobj);
					sprintf(strMsg, " %s[", ASAtomGetString(atom));
					break;
				case CosDict: // For a OCG
					name = CosDictGet(cosobj, ASAtomFromString("Name"));
					CosStringValueSafe(name, strOCGName, 256, &ocgNameLen);
					strOCGName[ocgNameLen] = '\0';
					sprintf(strMsg, " \"%s\"", strOCGName);
					break;
				case CosArray: // Complicated VE, including nested VE
					DumpVE(cosobj);
					break;
				default:
					sprintf(strMsg, "Unrecgonized Visibility Expression element.");	
			}
			Console::displayString(strMsg);
		}
		sprintf(strMsg, "]");
		Console::displayString(strMsg);
	}
}

