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

/** 
	This snippet demonstrates: 
	Alternatives to adding/removing dictionary entries, as well as checking
	dictionary membership for objects and retrieving values from dictionary
	entries, based on the given representation of the keys to the entries.

	@testfile none
	
	@see CosDictGetKey
	@see CosDictKnownKey
	@see CosDictPutKey
	@see CosDictRemoveKey
	@see CosDictKnownKeyString
	@see CosDictGetKeyString
	@see CosDictPutKeyString
	@see CosDictRemoveKeyString

	@see CosNewInteger
	@see CosNewBoolean
	@see CosNewFixed
	@see CosNewDict
	@see CosDictPut
	@see CosDictGet
	@see CosDictKnown
	@see CosDictRemove
*/
void CosDictKeyNameStringSnip(){
	PDDoc pdDoc = PDDocCreate();
	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);
	
	// Creates some Cos objects and a CosDict

	CosObj cosInt = CosNewInteger(cosDoc, true, 128);
	CosObj cosFixed = CosNewFixed(cosDoc, true, FloatToASFixed(1.414));
	CosObj cosBool = CosNewBoolean(cosDoc, true, 0);
	CosObj cosDict = CosNewDict(cosDoc, false, 3);

DURING
	// Adds Cos objects as dictionary entries with available alternatives

	CosObj intName = CosNewName(cosDoc, true, ASAtomFromString("Integer"));
	CosDictPutKey(cosDict,intName, cosInt);
	CosDictPutKeyString(cosDict, "Boolean", cosBool);
	CosDictPut(cosDict, ASAtomFromString("Fixed"), cosFixed);

	Console::displayString("Three entries added to the newly created CosDict object.\n");
	
	// Checks for object membership in the dictionary with available alternatives

	ASBool hasInt = CosDictKnownKey(cosDict, intName);
	ASBool hasBool = CosDictKnownKeyString(cosDict, "Boolean");
	ASBool hasFixed = CosDictKnown(cosDict, ASAtomFromString("Fixed"));

	// Confirm object types with available alternatives

	ASBool isInt, isBool, isFixed;
	isInt = isBool = isFixed = false;
	if (hasInt)
		if (CosInteger == CosObjGetType(CosDictGetKey(cosDict, intName)))
			isInt = true;
	if (hasBool)
		if (CosBoolean == CosObjGetType(CosDictGetKeyString(cosDict, "Boolean")))
			isBool = true;
	if (hasFixed)
		if (CosFixed == CosObjGetType(CosDictGet(cosDict, ASAtomFromString("Fixed"))))
			isFixed = true;

	char msg[256];
	memset(msg, 0, sizeof(msg));
	Console::displayString("Dictionary operations:\n");
	sprintf(msg, 
		"Adding a CosInteger object: %s\nAdding a CosBoolean object: %s\nAdding a CosFixed object: %s\n",
		(isInt)?"Successful":"Failed", (isBool)?"Successful":"Failed", (isFixed)?"Successful":"Failed");

	Console::displayString(msg);

	// Removes objects from dictionary with available alternatives then cleans up

	CosDictRemoveKey(cosDict, intName);
	CosDictRemoveKeyString(cosDict, "Boolean");
	CosDictRemove(cosDict, ASAtomFromString("Fixed"));
	
	CosObjDestroy(cosDict);

	Console::displayString("Objects removed from dictionary.\nDictionary destroyed.\n");

	PDDocClose(pdDoc);

HANDLER
	char errmsg[256];
	ASGetErrorString(ERRORCODE, errmsg, 255);
	AVAlertNote(errmsg);
END_HANDLER
}

SNIPRUN_REGISTER(CosDictKeyNameStringSnip, "COS Layer:CosDict Key",
	"This snippet demonstrates the new Cos layer APIs that takes CosName or character \
strings as the key to a CosDict. The execution of this snippet does *not* require an open document.")
