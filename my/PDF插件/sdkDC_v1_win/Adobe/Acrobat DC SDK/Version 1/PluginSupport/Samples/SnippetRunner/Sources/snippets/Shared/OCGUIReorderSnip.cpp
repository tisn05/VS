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

 OCGUIReorderSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

#include "ctype.h"

/* utility function to help insert objects into a cosarray */
static void insertCosElement(CosObj objToInsert, CosObj array, char * label, CosDoc inThisDoc);

/** Orders the OCGs in the UI into alphabetical groups. Creates "folders" for each letter of the alphabet (and Misc for non alpha chars).
	Rearranges the OCGOrder in the default configuration dictionary for the document. A "folder" of OCGs is represented by a sub-array 
	within the OCG order array. The first element of this sub array should be a cosstring that represents the title for the "folder". The 
	order of the (sub)elements in the array represents the order in the layers panel ui. This snippet assumes it is dealing with a flat OCG
	i.e. no previous grouping. If the snippet detects a previous grouping (i.e. the OCGOrder contains a CosObj that is not an OCG - we a
	assume it to be a string indicating a folder name), the operation is aborted.
	@testfile OCGSample1.pdf
 	@requires pdf with optional content groups
	@see PDDocGetCosDoc
	@see PDDocGetOCConfig
	@see PDOCConfigGetOCGOrder
	@see CosObjGetType
	@see CosArrayLength
	@see CosNewArray
	@see CosArrayGet
	@see PDOCGGetFromCosObj
	@see CosObjDestroy
	@see PDOCGGetName
	@see ASTextGetScriptText
	@see CosArrayRemoveNth
	@see CosObjDestroy
	@see PDOCConfigSetOCGOrder
	@see CosCopyStringValue
	@see CosArrayInsert
	@see CosNewString
	@see ASfree
*/
void OCGUIReorderSnip(void){
	Console::displayString("OCGUI reordering");
	
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);
	PDOCConfig docConfig = PDDocGetOCConfig(pdDoc);
	CosObj existingOCOrder;
	PDOCConfigGetOCGOrder(docConfig,&existingOCOrder);
	if (CosObjGetType(existingOCOrder) != CosArray){
		AVAlertNote("Error in PDF document, OCProperties has invalid Order array");
		return;
	}

	// if there are pre-existing OCGroups, just add the new one onto the end
	ASInt32 numberOfObjs = CosArrayLength(existingOCOrder);
	CosObj newArray = CosNewArray(cosDoc,false,2);
	// we iterate through the OCGs in the UI, creating a new array as we go...
	ASBool createdMisc = false;
	for (int loopcnt=0;loopcnt<numberOfObjs; loopcnt++){
		CosObj ocgAsCos = CosArrayGet(existingOCOrder, loopcnt);
		
		PDOCG ocg = PDOCGGetFromCosObj(ocgAsCos);
		// make sure the OCG is non-null. This could happen if it is an array within the array
		// i.e. we already have a grouping
		if (ocg == NULL){
			AVAlertNote("Problem getting ocg, do we already have grouping in this document?");
			CosObjDestroy(newArray);
			return;
		}
		ASText name = PDOCGGetName(ocg);
		const char*namestr = ASTextGetScriptText(name, kASRomanScript);
		char dstr[50];
		if (((namestr[0]<='z') && (namestr[0]>='a')) || ((namestr[0]<='Z') && (namestr[0]>='A'))) {	
			// note, this means that upper and lower case letters share the same entry
			sprintf(dstr,"Letter %c",toupper(namestr[0]));
			insertCosElement(ocgAsCos,newArray,dstr,cosDoc);
		}
		else {
			sprintf(dstr,"Misc");
			insertCosElement(ocgAsCos,newArray,dstr,cosDoc);
		}		
		ASTextDestroy(name);
	}
	for (int loop=0;loop<numberOfObjs; loop++){
		CosArrayRemoveNth(existingOCOrder, 0);
	}
	CosObjDestroy(existingOCOrder);
	// save back to the OCProperties for the document
	PDOCConfigSetOCGOrder(docConfig,newArray);
}

SNIPRUN_REGISTER(OCGUIReorderSnip, "PD Layer:Optional Content:Manipulate:Order in UI","Re-orders the OCGs in the UI. The OCG ui elements\
 are grouped depending on the first letter of the OCG. This snippet will only work for documents with no prior grouping of OCGs in the layers UI.")

/*
 a function that adds the CosObj to the CosArray with the following criteria:
 - the CosArray is iterated
 - Each element is either a CosObj (which we ignore) or a CosArray
 - if we find a CosArray, we get its first element
 - if its first element is a string equal to the one we pass in, we append the CosObj to this array
 - if its first element is a string greater to the one we pass in (or we come to the end of the list)
	we create a new array at the prior position and place the object on that
*/
void insertCosElement(CosObj objToInsert, CosObj array, char * label, CosDoc inThisDoc){
	ASInt32 arrayLength = CosArrayLength(array);
	ASBool inserted = false;
	for (int lp=0;lp<arrayLength;lp++){
		CosObj elem = CosArrayGet(array,lp);
		if (CosObjGetType(elem)==CosArray){
			CosObj nameObj = CosArrayGet(elem,0);
			if (CosObjGetType(nameObj)!=CosString){
				AVAlertNote("Bad string cos object?");
			}
			ASInt32 numberOfBytes;
			char* nameValue = CosCopyStringValue(nameObj, &numberOfBytes);
			if (strcmp(nameValue,label)==0) {
				// got a match, insert our object second
				CosArrayInsert(elem, 1, objToInsert);
				inserted = true;
				break;
			}
			else if (strcmp(nameValue,label)<0){
				// create a new array object
				// set its first and second elements
				CosObj newArray = CosNewArray(inThisDoc, false, 2);
				CosObj labelObj =  CosNewString(inThisDoc,false, label, strlen(label));
				CosArrayInsert(newArray,0,labelObj);
				CosArrayInsert(newArray,1,objToInsert);
				CosArrayInsert(array,lp,newArray);
				inserted = true;
				break;
			}
			ASfree(nameValue);
		}
	}
	if (inserted == false){
		// if we get here, we haven't yet inserted it, so we must have a new array to go on the end.
		// Console::displayString("New label, more than relationship");
		CosObj newArray =CosNewArray(inThisDoc, false, 2);
		CosObj labelObj =  CosNewString(inThisDoc,false, label, strlen(label));
		CosArrayInsert(newArray,0,labelObj);
		CosArrayInsert(newArray,1,objToInsert);
		CosArrayInsert(array,arrayLength,newArray);	
	}
}

