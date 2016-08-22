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

 CreateAnnotOCSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** an example of how to create optional content applied to annotations.
	An optional content group is created using any link annotations found in the first
	page of the document. If there are no link annotations on the first page, an OCG with no
	associated content is produced. This snippet associates an optional content membership dictionary
	with the OCG, then applies the membership dictionary to the annotation. In this respect, the content
	is indirectly controlled by the OCG through the OCMD.
	Steps to creating OCGs associated with Links annotations: <ul>
	<li> Create the OCG </li>
	<li> Set the intent to "view" </li>
	<li> Set initial visibility (on) </li>
	<li> Add the OCG to the order array in the document's OCProperties dictionary </li>
	<li> Create an OC membership directory associated with the OCG </li>
	<li> Iterate through annotations, adding link annotations to the OCMD </li> </ul>
	You may experience some UI redraw issues when using this snippet, in this 
	situation you can update the page view either manually or programmatically.
	
	@testfile blank.pdf
	@requires any pdf doc
	@see PDOCGCreate
	@see PDOCGSetIntent
	@see PDDocGetOCConfig
	@see PDOCGSetInitialState
	@see PDOCConfigGetOCGOrder
	@see PDOCGGetCosObj
	@see PDOCConfigSetOCGOrder
	@see PDOCMDCreate
	@see PDPageGetNumAnnots
	@see PDPageGetAnnot
	@see PDAnnotGetSubtype
	@see PDAnnotSetOCMD
	@see PDPageRelease
	@see ASTextDestroy
*/
void CreateAnnotOCSnip(void){
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	// create OCG named "SDK Link Annotation"
	ASText OCGtitle =ASTextFromScriptText("SDK Link Annotation",kASRomanScript);
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

	// now create the membership directory, this maintains the relationship between annotation and OCG
	// need to add the PDOCG to an array
	PDOCG PDOCGarray[2];
	PDOCGarray[0] = ocg;
	PDOCGarray[1] = NULL;
	PDOCMD theMD = PDOCMDCreate(pdDoc, PDOCGarray, kOCMDVisibility_AllOn);

	// grab the front page of the document
	PDPage theFirstPage = PDDocAcquirePage(pdDoc, 0);
	ASInt32 numOfAnnots = PDPageGetNumAnnots(theFirstPage);
	for (ASInt32 t = 0; t < numOfAnnots;t++){
		PDAnnot existingAnnot = PDPageGetAnnot(theFirstPage, t);

		if (PDAnnotGetSubtype(existingAnnot) == ASAtomFromString("Link")){
			// we have a link annotation, add it to the membership dictionary
			PDAnnotSetOCMD(existingAnnot,theMD);
		}
	}
	PDPageRelease(theFirstPage);
	if (OCGtitle!= NULL){
		ASTextDestroy(OCGtitle);
	}
	
	Console::displayString("OC Annot created");
}

SNIPRUN_REGISTER(CreateAnnotOCSnip, "PD Layer:Optional Content:Create:LinkAnnot OCG","Creates\
 an optional content group and associates it to an optional content menbership dictionary. The membership\
 dictionary is applied to existing link annotations on the first page of the document. If there are no link annotations on\
 the first page, the OCG is still created with no applicable content")

