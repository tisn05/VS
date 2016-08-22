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

PDOCConfigCreateSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** Creates a new OCConfig based on the front document's existing OCG's 
	settings. This Snippet uses the visibility state of the existing 
	OCGs in the new configuration, any that are visible are set to be 
	visible by default. All others are set to be invisible. Further, only
	the visible OCGs are presented through the layers UI for the new 
	configuration. The list of locked OCGs in the new configuration is
	set to the the same as that of the default configuration.

	@testfile OCGSample1.pdf
	@requires pdf with OCGs
	@see PDDocGetOCConfig
	@see PDDocGetNumOCGs
	@see PDDocGetOCGs
	@see PDOCConfigCreate
	@see PDOCConfigGetIntent
	@see PDOCConfigSetIntent
	@see PDOCConfigSetName
	@see PDOCConfigSetCreator
	@see PDDocGetOCContext
	@see PDOCConfigGetOCGOrder
	@see PDOCGGetCurrentState
	@see PDOCGGetCosObj
	@see PDOCConfigSetInitState
	@see PDOCConfigSetOCGOrder
	@see PDOCSetDefaultConfigSnip to switch the document configuration
	@see PDOCConfigExplorerSnip to find out the names of the snippets
	@see PDOCConfigGetLockedArray
	@see PDOCConfigSetLockedArray
*/
void PDOCConfigCreateSnip(ParamManager * thePM){
	CDocument document;
	PDDoc frontDoc = (PDDoc)document;
	if (frontDoc == NULL){
		return;
	}
	string newName;
	if (ASBoolToBool(thePM->getNextParamAsString(newName))!=true){
		AVAlertNote("Problem getting name parameter");
		return;
	}
	PDOCConfig defaultConfig = PDDocGetOCConfig(frontDoc);

	ASInt32 numOCGObjs = PDDocGetNumOCGs(frontDoc);
	PDOCG* docOCGs = PDDocGetOCGs(frontDoc);
	
	PDOCConfig newConfig = PDOCConfigCreate(frontDoc);
	
	PDOCConfigSetIntent(newConfig,PDOCConfigGetIntent(defaultConfig));
	// set the name
	ASText nameText = ASTextFromScriptText(newName.c_str(),kASRomanScript);
	PDOCConfigSetName(newConfig, nameText);
	ASTextDestroy(nameText);
	// set the creator
	ASText creatorText = ASTextFromScriptText("DevTech SDK",kASRomanScript);
	PDOCConfigSetCreator(newConfig, creatorText);
	ASTextDestroy(creatorText);

	// Get the default context
	PDOCContext defaultContext = PDDocGetOCContext(frontDoc);

	CosObj OCOrderArray;
	// get the new order array
	PDOCConfigGetOCGOrder(newConfig,&OCOrderArray);
	
	CosObj newOrderArray = CosNewArray(PDDocGetCosDoc(frontDoc), true, numOCGObjs);
	PDOCG* onArray = static_cast<PDOCG *>(ASmalloc(sizeof(PDOCG*)*(numOCGObjs+1)));
	PDOCG* offArray = static_cast<PDOCG *>(ASmalloc(sizeof(PDOCG*)*(numOCGObjs+1)));

	ASInt32 onArrayElem = 0;
	ASInt32 offArrayElem = 0;
	for (int loop = 0;loop<numOCGObjs;loop++){
		// we look at the state of the ocg. If it is visible, we put it into the order array.
		// if it is invisible, we put it in the OFF array. Visible OCGs are therefore on, 
		// and present in the UI
		PDOCG currentOCG = docOCGs[loop];
	
		if (PDOCGGetCurrentState(currentOCG, defaultContext)==false){
			// if the OCG is off, add it to the OFF array
			offArray[offArrayElem++] = currentOCG;
		} else {
			// OCG is on, add it to the new order array and add it to the on array
			ASInt32 numberOfObjs = CosArrayLength(newOrderArray);
			CosArrayInsert(newOrderArray,numberOfObjs,PDOCGGetCosObj(currentOCG));
			onArray[onArrayElem++] = currentOCG;
		}
	}
	// null terminate the arrays
	onArray[onArrayElem] = NULL;
	offArray[offArrayElem] = NULL;
	// Set the initial state
	PDOCConfigSetInitState(newConfig, kPDOCBaseState_ON, onArray, offArray);
	ASfree(onArray);
	ASfree(offArray);
	
	// save back to the OCProperties for the document
	PDOCConfigSetOCGOrder(newConfig,newOrderArray);

	// get the list of locked OCGs from the default configuration and set them in 
	// the new configuration
	PDOCG * ocgLockedArray = PDOCConfigGetLockedArray(defaultConfig);
	PDOCConfigSetLockedArray(newConfig, ocgLockedArray);

	// free the array of locked OCGs
	ASfree(ocgLockedArray);
}

SNIPRUN_REGISTER_WITH_DIALOG(PDOCConfigCreateSnip, 
							 "PD Layer:Optional Content:Create:Create Configuration",
							 "Creates a configuration based on the default with the \
default state of OCGs dictated by their current states. \n\nSee PDOCConfigExplorerSnip \
for a way to examine all configurations in a document. \nSee PDOCSetDefaultConfigSnip \
for a way to switch between configurations. \nParameter: a name for the new configuration.","\"New OCConfig\"")


