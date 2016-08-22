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
 
   RoleMapSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static void EnumerateRoleMapTypes(PDSRoleMap roleMap);
static void GetMap(PDSRoleMap roleMap, ASAtom stdType);
static void	UnmapSrcType(PDSRoleMap roleMap, ASAtom srcType);
static void	UnmapDestType(PDSRoleMap roleMap, ASAtom dstType);
		
static void AddMap(PDSRoleMap roleMap, ASAtom srcType, ASAtom stdType);
static ASBool ValidatePDSRoleMapObj(CosObj cosObj);
static ACCB1 ASBool ACCB2 GetKeyAtoms (CosObj keyObj, CosObj value, void* clientData);

/** This snippet is intended to demonstrate that an application using PDFL or plug-in extension that 
	processes logical structure can define its own structure type and mapping scheme to Adobe standard structure types. 
	By manipulating role map, the application (PDFL client or Acrobat plug-in) can make its owns layout decisions conforming 
	to its own layout model intent.

	This snippet does the following operations of class map in a Tagged PDF
	<ul>
	<li> enumerate type mapping in the role map
	<li> get direct map of a source type
	<li> unmap source type
	<li> unmap destination type
	<li> add a new type mapping
	</ul>

  It shows how to use PDSEdit APIs to add user defined structure types and 
  enumerate/edit the existing structure types.
	@testfile TwoColumnTaggedDoc.pdf
	@requires doc with structure
	@see PDDocGetStructTreeRoot
	@see PDSTreeRootGetRoleMap
	@see PDSTreeRootCreateRoleMap
	@see PDSRoleMapGetDirectMap
	@see PDSRoleMapUnMapSrc
	@see PDSRoleMapMap
	@see CosObjEnum 
	@see PDSRoleMapUnMapDst
  */
void RoleMapSnip(ParamManager * thePM)
{
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc==NULL){
		return;
	}

	ASBool isTaggedPDF = false;
	PDSTreeRoot pdsTreeRoot;
	PDSRoleMap roleMap;

	// Check if the PDF is a tagged PDF
	isTaggedPDF = SnippetRunnerUtils::PDDocIsMarkedPDF(pdDoc, false);

	if (ASBoolToBool(isTaggedPDF) == true){
		PDDocGetStructTreeRoot (pdDoc, &pdsTreeRoot);
		// Create an empty class map
		if (PDSTreeRootGetRoleMap (pdsTreeRoot, &roleMap) == false){
			PDSTreeRootCreateRoleMap (pdsTreeRoot, &roleMap);
			Console::displayString ("An empty role map has been created in this Tagged PDF.");
		}
	}
	else{
		AVAlertNote ("This document is not a tagged PDF.");
		return;
	}

	if (ValidatePDSRoleMapObj (roleMap) == false){
		AVAlertNote ("Invalid Role map.");
		return;
	}

	ASInt32 opType;	
	string MyType1, MyType2;
	// Get input parameters
	if (ASBoolToBool(thePM->getNextParamAsInt(opType)) == true && opType >= 1 && opType <= 5 ){
		if (opType > 1 && ASBoolToBool(thePM->getNextParamAsString(MyType1))!= true){
			AVAlertNote("A string type name is required.");
			return;
		}
		if (opType ==5 && ASBoolToBool(thePM->getNextParamAsString(MyType2))!= true){
			AVAlertNote("Two string type names are required.");
			return;
		}
	}
	else{
		AVAlertNote ("Enter a valid operation type (1-5).");
		return;
	}

	ASAtom MyType1_K = ASAtomFromString (MyType1.c_str());
	ASAtom MyType2_K = ASAtomFromString (MyType2.c_str());

	switch (opType) {
	case 1:
		EnumerateRoleMapTypes(roleMap);
		break;
	case 2:
		GetMap(roleMap, MyType1_K);
		break;
	case 3:
		UnmapSrcType(roleMap, MyType1_K);
		break;
	case 4:
		UnmapDestType(roleMap, MyType1_K);
		break;
	case 5:
		AddMap(roleMap, MyType1_K, MyType2_K);
		break;
	default:
		AVAlertNote ("Enter a correct operation type (1-5)\n");
		break;
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(RoleMapSnip, "PDS Layer:TaggedPDF:Role Map",
"Experiment with role maps. Choose the operation type (followed by a type string for operation type 2-4):\n \
1. Enumerate type mapping in the role map (1);\n \
2. Get the standard type for a given source type (2 SourceType);\n \
3. Unmap source type (3 SourceType);\n \
4. Unmap destination type (4 DestinationType);\n \
5. Add a new type mapping (5 SourceType DestinationType).", "1")

/* PDS role map object has to be a CosDict */
static ASBool ValidatePDSRoleMapObj(CosObj cosObj)
{
	CosType cosObjType = CosObjGetType(cosObj);
	return (cosObjType == CosDict);
}

/* UnmapSrcType
** ----------------------------------------------------------------------
** Makes the specified element type have no mapping.
**
*/
static void	UnmapSrcType(PDSRoleMap roleMap, ASAtom srcType)
{
	ASInt32 error = 0;
DURING	
	if (PDSRoleMapGetDirectMap (roleMap, srcType) != ASAtomNull){
		PDSRoleMapUnMapSrc (roleMap, srcType, false);
		Console::displayString ("The source type mapping was removed.");
	}
	else{
		AVAlertNote ("The type has no mapping in the role map.");
	}
HANDLER
	error = ERRORCODE;
END_HANDLER
	if (error != 0){
		char buffer[256];
		ASGetErrorString(ERRORCODE, buffer, 255);
		AVAlertNote(buffer);
	}
}


/* UnmapDestType
** ----------------------------------------------------------------------
** Makes the specified element type have no mapping.
**
*/
static void	UnmapDestType(PDSRoleMap roleMap, ASAtom dstType)
{
	ASInt32 error = 0;
DURING
	PDSRoleMapUnMapDst (roleMap, dstType); 		
	Console::displayString ("The destination standard type mapping was removed.");
HANDLER
	error = ERRORCODE;
END_HANDLER

	char buffer[256];
	if (error != 0){
		ASGetErrorString(ERRORCODE, buffer, 255);
		AVAlertNote(buffer);
	}
}

/* GetMap
** ----------------------------------------------------------------------
** Get the standard type mapped to a given source type
*/
static void GetMap(PDSRoleMap roleMap, ASAtom srcType)
{
	ASAtom stdType;
	ASInt32 error = 0;
DURING
	stdType = PDSRoleMapGetDirectMap (roleMap, srcType);
HANDLER
	error = ERRORCODE;
END_HANDLER
	char buffer[256];
	if (error != 0){
		ASGetErrorString(ERRORCODE, buffer, 255);
		AVAlertNote(buffer);
		return;
	}

	if (stdType == ASAtomNull){
		AVAlertNote ("The type has no mapping in the role map.");
	}
	else{
		char buf[128];
		sprintf (buf, "The source type \"%s\" is mapped to the standard type \"%s\"", 
			ASAtomGetString (srcType), ASAtomGetString(stdType));
		Console::displayString (buf);
	}
}


/* AddMap
** ----------------------------------------------------------------------
** Get the standard type mapped to a given source type
*/
static void AddMap(PDSRoleMap roleMap, ASAtom srcType, ASAtom stdType)
{
	ASInt32 error = 0;
DURING
	PDSRoleMapMap (roleMap, srcType, stdType);
HANDLER
	error = ERRORCODE;
END_HANDLER
	char buffer[256];
	if (error != 0){
		ASGetErrorString(ERRORCODE, buffer, 255);
		AVAlertNote(buffer);
		return;
	}
	else{
		sprintf (buffer, "A new mapping is created from the source type \"%s\" to the standard type \"%s\"", 
			ASAtomGetString (srcType), ASAtomGetString(stdType));
		Console::displayString (buffer);
	}
}
/* EnumerateRoleMapTypes
** ----------------------------------------------------------------------
** Enumerate all classes in the class map.
** And enumerate all attibutes in the given class
*/
static void EnumerateRoleMapTypes(PDSRoleMap roleMap)
{	
	// Enumerate all type mapping in the roleMap
	Console::displayString("Mapping list from source type to standard type:");
	CosObjEnumProc myCosObjEnumProcCB = ASCallbackCreateProto(CosObjEnumProc, &GetKeyAtoms);
	CosObjEnum (roleMap, myCosObjEnumProcCB, NULL);

	ASCallbackDestroy((void*)myCosObjEnumProcCB);
}

/* Get the type key names in the role map */
static ACCB1 ASBool ACCB2 GetKeyAtoms (CosObj keyObj, CosObj value, void* clientData)
{
	ASBool	processed	= false;
	//	Retrieve the key name
	if (CosObjGetType( keyObj ) == CosName){
		char buffer[128];
		char name[50], val[50];
		buffer[0] = name[0] = val[0] = '\0';

		strncpy ( name, CosCopyNameStringValue ( keyObj, NULL ), sizeof(name) - 1);
		strncpy ( val, CosCopyNameStringValue ( value, NULL ), sizeof(val) - 1);

		sprintf (buffer, "%s \t\t %s", name, val);
		Console::displayString(buffer);
		processed = true;
	}
	return processed;
}
