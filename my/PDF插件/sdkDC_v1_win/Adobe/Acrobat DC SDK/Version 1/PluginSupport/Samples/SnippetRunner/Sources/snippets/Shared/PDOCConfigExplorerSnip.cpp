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

 PDOCConfigExplorerSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

ACCB1 ASBool ACCB2 myPDOCConfigEnumProc(PDOCConfig occonfig,void* clientData);

/** The snippet dumps diagnostic information about optional content 
	configurations found in the document to the DebugWindow. It uses 
	a enummeration procedure to iterate through the configurations in the document.

	@testfile OCGSample1.pdf
	@requires pdf with OCGs
	@see PDOCSetDefaultConfigSnip for a snippet that allows you to switch between configurations
	@see PDOCConfigCreateSnip for a snippet that creates aternative configurations
	@see PDOCConfigEnumProc
	@see PDDocEnumOCConfigs
	@see PDOCConfigGetName
	@see ASTextCat
	@see PDOCConfigGetIntent
	@see PDOCConfigGetCreator
	@see PDOCConfigGetOCGOrder
	@see ASTextFromInt32
	@see PDOCGGetFromCosObj
	@see PDDocGetNumOCGs
	@see PDOCConfigGetInitState
	@see ASTextCat
	@see PDOCConfigGetLockedArray
*/
void PDOCConfigExplorerSnip(){
	CDocument document;
	PDDoc frontDoc = (PDDoc)document;
	if (frontDoc == NULL){
		return;
	}
	// enummerate the configurations
	PDDocEnumOCConfigs(frontDoc,myPDOCConfigEnumProc, NULL);

}

SNIPRUN_REGISTER(PDOCConfigExplorerSnip, "PD Layer:Optional Content:Explore OC Configs",
				"Enumerates the optional content configurations that exist \
in the document. See the Common SnippetRunner Interface for output.")

/* the ennumeration procedure, called for each configuration */
ACCB1 ASBool ACCB2 myPDOCConfigEnumProc (PDOCConfig occonfig,void* clientData){

	// the name of the configuration 
	ASText configDetails = ASTextFromScriptText("OCConfig - Name == ",kASRomanScript);
	ASText name =  PDOCConfigGetName(occonfig);
	if (name == NULL || ASTextIsEmpty(name)){
		name = ASTextFromScriptText("No name information",kASRomanScript);
	}
	ASTextCat(configDetails,name);
	ASTextDestroy(name);

	// for formatting 
	ASText nltabText = ASTextFromScriptText("\n\t",kASRomanScript);
	ASText nlText = ASTextFromScriptText("\n", kASRomanScript);
	
	// the intent of the configuration
	ASAtom* myIntent = PDOCConfigGetIntent(occonfig);
	ASText intentText = ASTextFromScriptText(ASAtomGetString(*myIntent),kASRomanScript);
	ASTextCat(configDetails,nltabText);
	if (intentText == NULL || ASTextIsEmpty(intentText)){
		intentText = ASTextFromScriptText("No intent information",kASRomanScript);
	}
	ASText configIntent = ASTextFromScriptText("Config Intent == ",kASRomanScript);
	ASTextCat(configDetails,configIntent);
	ASTextCat(configDetails,intentText);
	ASTextDestroy(intentText);
	ASTextDestroy(configIntent);
	
	// the creator of the configuration
	ASTextCat(configDetails,nltabText);
	ASTextDestroy(nltabText);
	ASText creator = PDOCConfigGetCreator(occonfig);
	if (creator == NULL || ASTextIsEmpty(creator)){
		creator = ASTextFromScriptText("No creator information",kASRomanScript);
	}
	ASText configCreator = ASTextFromScriptText("Config creator == ",kASRomanScript);
	ASTextCat(configDetails, configCreator);
	ASTextCat(configDetails, creator);
	ASTextDestroy(creator);
	ASTextDestroy(configCreator);
	ASTextCat(configDetails, nlText);
	
	// output the string
	Console::displayString(configDetails);
	ASTextDestroy(configDetails);
	
	// now we dump the number of OCGs that are in the UI
	// get the order array
	CosObj existingOrder;
	PDOCConfigGetOCGOrder(occonfig,&existingOrder);
	// if the order is NULL, it means the configuration is empty
	if (CosObjEqual(existingOrder, CosNewNull())){
		Console::displayString("\tThis configuration is empty\n");
		return true;
	}
	ASInt32 numberOfObjs = CosArrayLength(existingOrder);
	configDetails = ASTextFromScriptText("\tNumber of OCGs in UI == ",kASRomanScript);
	ASText numObjs = ASTextFromInt32(numberOfObjs);
	ASTextCat(configDetails, numObjs);
	ASTextDestroy(numObjs);
	ASTextCat(configDetails, nlText);
	Console::displayString(configDetails);
	ASTextDestroy(configDetails);
	
	// dump information about each OCG that is in the order array
	for (int loopctr = 0; loopctr<numberOfObjs;loopctr++){
		CosObj ocgObj = CosArrayGet(existingOrder, loopctr);
		PDOCG ocg = PDOCGGetFromCosObj(ocgObj);
		configDetails = ASTextFromScriptText("\t\t",kASRomanScript);
		
		// get the OCG name and dump it
		ASText loopText = ASTextFromInt32(loopctr);
		ASText ocgNameText = ASTextFromScriptText(" OCG name ",kASRomanScript);
		ASTextCat(configDetails, loopText);
		ASTextCat(configDetails, ocgNameText);
		ASTextDestroy(loopText);
		ASTextDestroy(ocgNameText);
		ASText ocgName;
		if (ocg != NULL){
			ocgName = PDOCGGetName(ocg);
		} else {
			ocgName = ASTextFromScriptText("OCG is null?",kASRomanScript);
		}
		ASTextCat(configDetails,ocgName);
		ASTextDestroy(ocgName);
		ASTextCat(configDetails, nlText);
		Console::displayString(configDetails);
		ASTextDestroy(configDetails);
	}
	
	// total number of OCGs in the document
	ASInt32 numOCObjs = PDDocGetNumOCGs(PDOCConfigGetPDDoc(occonfig));

	// the base state, and the on/off exception arrays
	PDOCConfigBaseState bs;
	PDOCG * ocgOnArray;
	PDOCG * ocgOffArray;
		
	// get the initial state of the OCGs
	PDOCConfigGetInitState(occonfig,&bs,&ocgOnArray,&ocgOffArray);

	// deal with the base state
	configDetails = ASTextFromScriptText("\tBase state for configuration == ",kASRomanScript);
	switch (bs) {
		case kPDOCBaseState_OFF:
			{
				ASText offText = ASTextFromScriptText("OFF",kASRomanScript);
				ASTextCat(configDetails, offText);
				ASTextDestroy(offText);
			}
			break;
		case kPDOCBaseState_ON:
			{
				ASText onText = ASTextFromScriptText("ON",kASRomanScript);
				ASTextCat(configDetails, onText);
				ASTextDestroy(onText);
			}
			break;
		case kPDOCBaseState_Unchanged:
			{
				ASText unchangedText = ASTextFromScriptText("Unchanged",kASRomanScript);
				ASTextCat(configDetails, unchangedText);
				ASTextDestroy(unchangedText);
			}
			break;
		default:
			{
				ASText unknownText = ASTextFromScriptText("Unknown",kASRomanScript);
				ASTextCat(configDetails,unknownText);
				ASTextDestroy(unknownText);
			}
	}
	ASTextCat(configDetails, nlText);
	Console::displayString(configDetails);
	ASTextDestroy(configDetails);
	
	// iterate through the on array
	PDOCG* onArray = ocgOnArray;
	Console::displayString("\tOCGs on by default:\n");
	if (onArray != NULL){
		while (*onArray){
			configDetails = ASTextFromScriptText("\t\t Name - ",kASRomanScript);
			ASText nameText = PDOCGGetName(*onArray++);
			ASTextCat(configDetails, nameText);
			ASTextCat(configDetails, nlText);
			Console::displayString(configDetails);
			ASTextDestroy(nameText);
			ASTextDestroy(configDetails);
		}
	}

	// iterate through the off array
	PDOCG* offArray = ocgOffArray;
	Console::displayString("\tOCGs off by default:\n");
	if (offArray!=NULL){
		while (*offArray){
			configDetails = ASTextFromScriptText("\t\t Name - ",kASRomanScript);
			ASText nameText = PDOCGGetName(*offArray++);
			ASTextCat(configDetails, nameText);
			ASTextCat(configDetails, nlText);
			Console::displayString(configDetails);
			ASTextDestroy(nameText);
			ASTextDestroy(configDetails);
		}
	}

	// get the list of locked OCGs
	PDOCG* lockedArray = PDOCConfigGetLockedArray(occonfig);
	// iterate through the locked array
	PDOCG* iterator = lockedArray;
	Console::displayString("\tOCGs locked by default:\n");
	if(lockedArray!=NULL) {
		while (*iterator) {
			configDetails = ASTextFromScriptText("\t\t Name - ", kASRomanScript);
			ASText nameText = PDOCGGetName(*iterator++);
			ASTextCat(configDetails, nameText);
			ASTextCat(configDetails, nlText);
			Console::displayString(configDetails);
			ASTextDestroy(nameText);
			ASTextDestroy(configDetails);
		}
	}

	ASTextDestroy(nlText);

	// free the allocated memory
	ASfree(ocgOnArray);
	ASfree(ocgOffArray);
	ASfree(lockedArray);
	return true;
}

