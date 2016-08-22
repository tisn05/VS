/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

*/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static void CheckAndDisplayWeakReference(CosObj);

/** 
	This snippet demonstrates: 
	Assign/remove "weak" references to elements/entries of CosArray/CosDict objects,
	as well as checking the "weak" state of the references. 

	@testfile none
	@see CosArrayIsWeakReference
	@see CosArraySetWeakReference
	@see CosDictIsWeakReference
	@see CosDictSetWeakReference
	@see CosNewInteger
	@see CosNewBoolean
	@see CosNewArray
	@see CosArrayPut
	@see CosArrayInsert
	@see CosNewDict
	@see CosDictPut
	@see CosObjGetType
*/
void CosObjWeakReferenceSnip(){
	PDDoc pdDoc = PDDocCreate();
	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);
	
	// Creates some Cos objects and a CosArray

	CosObj cosInt = CosNewInteger(cosDoc, true, 128);
	CosObj cosFixed = CosNewFixed(cosDoc, true, FloatToASFixed(1.414));
	CosObj cosBool = CosNewBoolean(cosDoc, true, 0);
	CosObj cosArray = CosNewArray(cosDoc, false, 3);

	// Adds a couple of Cos objects to the CosArray object

	CosArrayPut(cosArray, 0, cosInt);
	CosArrayPut(cosArray, 1, cosFixed);

	// Sets weak reference to the second element of the CosArray - cosFixed

	CosArraySetWeakReference(cosArray, 1, true);

	// Inserts a CosBoolean object as the new second element, pushing cosFixed to the third

	CosArrayInsert(cosArray, 1, cosBool);

	Console::displayString("* Setting weak reference to the 3rd element of CosArray object...\n");
	CheckAndDisplayWeakReference(cosArray);	

	// Removes weak reference from the cosFixed object - now the 3rd element

	CosArraySetWeakReference(cosArray, 2, false);

	Console::displayString("* After removing weak reference from the 3rd element of CosArray object...\n");
	CheckAndDisplayWeakReference(cosArray);	

	CosObjDestroy(cosArray);

	//
	// Next we exercise APIs with the same function for a CosDict object
	// Creates a CosDict
	//
	
	CosObj cosDict = CosNewDict(cosDoc, false, 3);

	// Add Cos objects as dictionary entries

	CosDictPut(cosDict, ASAtomFromString("Integer"), cosInt);
	CosDictPut(cosDict, ASAtomFromString("Boolean"), cosBool);
	CosDictPut(cosDict, ASAtomFromString("Fixed"), cosFixed);

	// This time we set the weak refernce to the cosBool object

	Console::displayString("* Setting weak reference to the 2nd entry in the CosDict object...\n");
	CosDictSetWeakReference(cosDict, "Boolean", true);
	CheckAndDisplayWeakReference(cosDict);

	// We then remove the weak reference from the cosBool object

	Console::displayString("* After removing weak reference from the 2nd entry in the CosDict object...\n");
	CosDictSetWeakReference(cosDict, "Boolean", false);
	CheckAndDisplayWeakReference(cosDict);

	PDDocClose(pdDoc);
}


/**
	Helper function that takes care of checking and displaying 
	weak refernce states for element of a CosArray or entries 
	of a CosDict object
*/
static void CheckAndDisplayWeakReference(CosObj cosObj)
{
	char msg[256], name[16];
	ASBool isWeakRefFixed;
	ASBool isWeakRefBool;
	ASBool isWeakRefInt;

	memset(msg, 0, sizeof(msg));
	
	if (CosObjGetType(cosObj) == CosArray)
	{
		Console::displayString("[CosArray]\n");

		// Checks the state of a weak reference
		
		isWeakRefFixed = CosArrayIsWeakReference(cosObj, 2);
		isWeakRefBool = CosArrayIsWeakReference(cosObj, 1);
		isWeakRefInt = CosArrayIsWeakReference(cosObj, 0);
		strcpy(name, "element");
	} else {
		Console::displayString("[CosDict]\n");

		// Checks the state of a weak reference
		
		isWeakRefFixed = CosDictIsWeakReference(cosObj, "Fixed");
		isWeakRefBool = CosDictIsWeakReference(cosObj, "Boolean");
		isWeakRefInt = CosDictIsWeakReference(cosObj, "Integer");
		strcpy(name, "entry");
	}
	// We should observe that the "weak" reference state sticks with the CosObject cosFixed
	
	sprintf(msg, "1st %s (CosInteger): %s reference\n2nd %s (CosBoolean): %s reference\n3rd %s (CosFixed): %s reference\n\n", 
		name, (isWeakRefInt)?"WEAK":"normal", name, (isWeakRefBool)?"WEAK":"normal", name, (isWeakRefFixed)?"WEAK":"normal");
	Console::displayString(msg);
}

SNIPRUN_REGISTER(CosObjWeakReferenceSnip, "COS Layer:Weak Reference",
	"This snippet demonstrates the ability to set and check \"weak\" references to COS objects. \
It requires no input file. Output is written to the DebugWindow.")
