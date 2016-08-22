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

 PDCreateMasterOCGSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** This snippet manipulates the UI so all existing OCGs are maintained within 
	a "master" OCG. It achieves this by replacing the order array with a new 
	array that contains two elements. The first is the new "master" OCG, the 
	second is the original order array. The layers panel is updated to reflect 
	this, a new "folder" is created with all the content that was previously 
	there, and this "folder" has a visibility control. In this implementation 
	the visibility control only manipulates the ability to change the visibility 
	of the OCG elements within the folder, it does not change the child visibility. 
	This is more like a "master control switch", rather than "master visibility 
	switch". If you need to be able to control the visibility of a large number 
	of OCGs, you need to do the following:
	<ul>
		<li> Create a new "parent" OCG
		<li> Create an OCMD for each of the OCGs you want to control, the policy 
			should be an AllOn relationship. </li>
		<li> Associate the content previously controlled by the OCG with the new OCMD.</li>
		<li> For each of the new OCMDs, associate the relative OCG (the OCG the 
			OCMD has replaced) along with the "parent" OCG </li>
		<li> Represent the "parent" OCG as a node in the order array, followed by 
			the list of pre-existing OCGs (as shown in this snippet).</li>
	</ul>
	This has the effect of creating a parent group, and having the content visible only 
	if both the "parent" and original OCG is "on".
	@testfiles OGCSample1.pdf
	@requires pdf with OCGs
	@see OCGUIReorderSnip
	@see PDOCGCreate
	@see PDDocGetOCConfig
	@see PDOCGSetInitialState
	@see PDOCConfigGetOCGOrder
	@see PDOCConfigSetOCGOrder
	@see CosNewArray
	@see PDOCGGetCosObj
	@see CosArrayPut
*/
void PDCreateMasterOCGSnip(ParamManager * thePM){
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}
	string name;
	if (thePM->getNextParamAsString(name)==false){
		AVAlertNote("Error retrieving parameter");
		return;
	}

	// create OCG 
	ASText OCGtitle =ASTextFromScriptText(name.c_str(),kASRomanScript);
	PDOCG ocg = PDOCGCreate(pdDoc, OCGtitle);
	
	// Set its intent to "view" to indicate it is to be used in a viewer application
	ASAtom intentArray[] = {ASAtomFromString ("View"), ASAtomNull};
	PDOCGSetIntent (ocg, intentArray);

	// Indicate it should initially be turned on (visible)
	PDOCConfig docConfig = PDDocGetOCConfig(pdDoc);
	PDOCGSetInitialState(ocg, docConfig, true);

	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);
	CosObj existingOCOrder;
	// save back to the OCProperties for the document
	PDOCConfigGetOCGOrder(docConfig,&existingOCOrder);
	// reset the order to NULL, effectively removing a reference to the array.
	PDOCConfigSetOCGOrder(docConfig,CosNewNull());
	if (CosObjGetType(existingOCOrder) != CosArray){
		AVAlertNote("Error in PDF document, OCProperties has invalid Order array");
		return;
	}
	
	// create a new array and add the old order as a child to the new array. 
	// array[0] becomes the "master" ocg
	// array[1] is the original array.
	CosObj newArray = CosNewArray(cosDoc,false,2);
	CosArrayPut(newArray, 0,PDOCGGetCosObj(ocg));
	CosArrayPut(newArray, 1,existingOCOrder);	

	// save back to the OCProperties for the document
	PDOCConfigSetOCGOrder(docConfig,newArray);
	
	Console::displayString("OCGs created and saved");
}

SNIPRUN_REGISTER_WITH_DIALOG(PDCreateMasterOCGSnip, 
							 "PD Layer:Optional Content:Manipulate:Create master OCG",
							 "Creates a master OCG, that encloses all other \
OCGs in the UI. The name of the OCG is passed in as a parameter.","\"Master Switch\"")

