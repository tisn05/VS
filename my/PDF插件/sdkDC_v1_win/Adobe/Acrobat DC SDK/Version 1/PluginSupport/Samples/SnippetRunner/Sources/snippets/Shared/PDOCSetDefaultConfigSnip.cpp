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

PDOCSetDefaultConfigSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static ACCB1 ASBool ACCB2 myPDOCConfigEnumProc(PDOCConfig occonfig,void* clientData);

/** This snippet allows the user to specify a configuration that exists in the 
	document and have that configuration replace the document's default configuration. 
	It does not handle radio-groups. It uses a callback to search for the configuration. 
	If the configuration is not found, the snippet does nothing.

	@see PDOCConfigEnumProc
	@see PDDocEnumOCConfigs
	@see PDOCConfigGetName
	@see ASTextCmp
	@see PDOCConfigGetPDDoc
	@see PDDocGetOCConfig
	@see PDOCConfigSetName
	@see PDOCConfigGetName
	@see PDOCConfigSetIntent
	@see PDOCConfigGetIntent
	@see PDOCConfigSetCreator
	@see PDOCConfigGetCreator
	@see PDDocGetNumOCGs
	@see PDOCConfigGetInitState
	@see PDOCConfigSetInitState
	@see PDOCConfigGetOCGOrder
	@see PDOCConfigSetOCGOrder
	@see PDOCConfigGetLockedArray
	@see PDOCConfigSetLockedArray
*/
void PDOCSetDefaultConfigSnip(ParamManager * thePM){
	CDocument document;
	PDDoc frontDoc = (PDDoc)document;
	if (frontDoc == NULL){
		return;
	}
	string theName;
	if (ASBoolToBool(thePM->getNextParamAsString(theName))!= true){
		Console::displayString("Problem getting name parameter");
		return;
	}
	char * newChar = static_cast<char *>(ASmalloc(theName.length() + 1));
	strcpy(newChar,theName.c_str());
	PDDocEnumOCConfigs(frontDoc,myPDOCConfigEnumProc, static_cast<void *>(newChar));
	ASfree(newChar);
}

SNIPRUN_REGISTER_WITH_DIALOG(PDOCSetDefaultConfigSnip, 
							 "PD Layer:Optional Content:Manipulate:Set default config",
							 "Sets the document configuration to the one with the \
 \nname passed in from the parameter dialog. \nThis snippet really just copies the init \
 \nstate, name, creator, OCGOrder and list of locked OCGs...","\"New OCConfig\"")


static ACCB1 ASBool ACCB2 myPDOCConfigEnumProc(PDOCConfig occonfig,void* clientData){
	char * cname = static_cast<char *>(clientData);
	ASText desiredName = ASTextFromScriptText(cname,kASRomanScript);
	ASText thisName =  PDOCConfigGetName(occonfig);

	// if we have an unnamed configuration, we just return true - it isn't ours
	if (thisName == NULL){
		ASTextDestroy(desiredName);
		return true;
	}
	if (ASTextCmp(desiredName, thisName) == 0){
		//don't need these anymore
		ASTextDestroy(desiredName);
		ASTextDestroy(thisName);
		
		PDDoc frontDoc = PDOCConfigGetPDDoc(occonfig);
		PDOCConfig defaultConfig = PDDocGetOCConfig(frontDoc);
		// we have a match. Copy the attributes from occonfig to defaultConfig
		
		// name
		ASText nameText = PDOCConfigGetName(occonfig);
		if(nameText)
		{
			PDOCConfigSetName(defaultConfig, nameText);
			ASTextDestroy(nameText);
		}
		
		// Intent
		ASAtom* intentText = PDOCConfigGetIntent(occonfig);
		if(intentText)
		{
			PDOCConfigSetIntent(defaultConfig, intentText);
			ASfree(intentText);
		}
		
		// Creator
		ASText creatorText = PDOCConfigGetCreator(occonfig);
		if(creatorText)
		{
			PDOCConfigSetCreator(defaultConfig, creatorText);
			ASTextDestroy(creatorText);
		}
		
		ASInt32 numOCObjs = PDDocGetNumOCGs(frontDoc);

		// we now allocate space for base state and on/off arrays.
		PDOCG * ocgOnArray = NULL;
		PDOCG * ocgOffArray = NULL;
		PDOCConfigBaseState bs;
		
		// get the base state, on and off arrays and set them in the default configuration
		PDOCConfigGetInitState(occonfig,&bs,&ocgOnArray,&ocgOffArray);
		PDOCConfigSetInitState(defaultConfig,bs,ocgOnArray,ocgOffArray);
		
		// free above arrays
		ASfree(ocgOnArray);
		ASfree(ocgOffArray);
		
		// get the order of the incoming OCG order
		CosObj existingOCOrder;
		PDOCConfigGetOCGOrder(occonfig,&existingOCOrder);

		ASTArraySize sizeOfArray =  CosArrayLength(existingOCOrder);

		CosObj newConfigArray = CosNewArray(PDDocGetCosDoc(frontDoc), true,sizeOfArray);

		for (int t = 0; t<sizeOfArray;t++){
			// put the ocg into the new array
			CosArrayPut(newConfigArray, t,CosArrayGet(existingOCOrder, t)); 
		}
		
		// set the order
		PDOCConfigSetOCGOrder(defaultConfig,newConfigArray);

		// get the list of locked OCGs and set them in the default configuration
		PDOCG * ocgLockedArray = PDOCConfigGetLockedArray(occonfig);
		PDOCConfigSetLockedArray(defaultConfig, ocgLockedArray);

		// free the array of locked OCGs
		ASfree(ocgLockedArray);

		// finished processing
		return false;
	}
	// no match, continue processing
	return true;
}


