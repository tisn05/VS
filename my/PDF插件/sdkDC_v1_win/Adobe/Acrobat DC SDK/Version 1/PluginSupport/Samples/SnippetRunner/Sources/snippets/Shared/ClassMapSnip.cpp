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
 
   ClassMapSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

#define MyAttrOwner_K ASAtomFromString ("MyAttrOwner")
#define Contents_K ASAtomFromString("Contents")
#define AtomO_K ASAtomFromString("O")

#define PDFLClient_K ASAtomFromString ("PDFLClient")
#define AcrobatSdkPlugIn_K ASAtomFromString ("AcrobatSdkPlugIn")
const ASBool PDFL_SDK = false;

// this snippet uses ASCallbackDestroy, which macros out to nothing on PDFL.
// Ignore this warning
#pragma warning(disable: 4390)


static void RemoveClass(PDSClassMap classMap, ASAtom className);
static void	AddAttrObj(PDDoc pddoc, PDSClassMap classMap, ASAtom className);
static void	RemoveAttrObj(PDSClassMap classMap, ASAtom className);
static void EnumAttrObj(PDSClassMap classMap, ASAtom className);
static void EnumClass(PDSClassMap classMap);
static ASBool ValidatePDSClassMapObj(CosObj cosObj);
static ASBool ValidatePDSAttrObj(CosObj cosObj);

ACCB1 ASBool ACCB2 GetKeyAtoms (CosObj keyObj, CosObj value, void* clientData);

/** This snippet is intended to demonstrate that an application using PDFL or plug-in extension that 
	processes logical structure can attach additional information (attributes) to any structure element. 
	The owner of the attribute is hard-coded in the snippet as either "PDFLClient" or "AcrobatSdkPlugIn". 
	Adobe has defined a set of standard structure attributes, identified by specific standard owners. 
	But the mechanism allows 3rd party applications to facilitate the interchange of content among PDF applications.

	This snippet does the following operations to classmap of a tagged PDF
	<ul>
	<li> create a new class in the class map
	<li> remove a class
	<li> add an attribute to a class
	<li> remove an attribute from a class
	<li> enumerate calsses and attributes in a class
	</ul>
	
	@testfile TwoColumnTaggedDoc.pdf
	@requires tagged pdf file

	@see PDDocGetStructTreeRoot
	@see PDSTreeRootGetClassMap
	@see PDSTreeRootCreateClassMap
	@see PDSClassMapAddAttrObj
	@see PDSClassMapGetNumAttrObjs
	@see PDSClassMapRemoveAttrObj
	@see PDSClassMapRemoveClass

  */
void ClassMapSnip(ParamManager * thePM)
{
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc==NULL){
		return;
	}

	ASBool isTaggedPDF = false;
	PDSTreeRoot pdsTreeRoot;
	PDSClassMap classMap;

	// Check if the PDF is a tagged PDF
	isTaggedPDF = SnippetRunnerUtils::PDDocIsMarkedPDF(pdDoc, false);

	if (ASBoolToBool(isTaggedPDF) == true){
		PDDocGetStructTreeRoot (pdDoc, &pdsTreeRoot);
		// Create an empty class map
		if (PDSTreeRootGetClassMap (pdsTreeRoot, &classMap) == false){
			PDSTreeRootCreateClassMap (pdsTreeRoot, &classMap);
			Console::displayString ("An empty class map has been created in this Tagged PDF.");
		}
	}
	else{
		AVAlertNote ("This document is not a Tagged PDF.");
		return;
	}

	if (ValidatePDSClassMapObj (classMap) == false){
		AVAlertNote ("Invalid class map.");
		return;
	}

	ASInt32 opType;	
	string MyClass, MyAttrObj;
	// Get input parameters
	if (ASBoolToBool(thePM->getNextParamAsInt(opType)) == true || opType < 1 || opType > 5 ){
		if (ASBoolToBool(thePM->getNextParamAsString(MyClass))!=true && opType != 5){
			AVAlertNote("A string class name is required.");
			return;
		}
	}
	else{
		AVAlertNote ("Enter a valid operation type (1-5).");
		return;
	}

	ASAtom MyClass_K = ASAtomFromString (MyClass.c_str());

	switch (opType) {
	case 1:
		AddAttrObj(pdDoc, classMap, MyClass_K);
		break;
	case 2:
		EnumAttrObj(classMap, MyClass_K);
		break;
	case 3:
		RemoveAttrObj(classMap, MyClass_K);
		break;
	case 4:
		RemoveClass(classMap, MyClass_K);
		break;
	case 5:
		EnumClass(classMap);
		break;
	default:
		AVAlertNote ("Enter a correct operation type (1-5)\n");
		break;
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(ClassMapSnip, "PDS Layer:TaggedPDF:Class Map",
"Experiment class map in a tagged PDF, choose the operation type (1-5): \n\
 1. Add an attribute to a class (1 ClassName);\n 2. Enumerate attributes in a class (2 ClassName);\n \
3. Remove the first attribute in a class (3 ClassName);\n 4. Remove class (4 ClassName);\n \
5. Enumerate classes (5).", "1 MyClass")

/* Attribute objects have to be either CosDict or CosStream */
static ASBool ValidatePDSAttrObj(CosObj cosObj)
{
    CosType cosObjType = CosObjGetType(cosObj);
    return ((cosObjType == CosDict) || (cosObjType == CosStream));
}

/* PDS Class has to be a CosDict */
static ASBool ValidatePDSClassMapObj(CosObj cosObj)
{
	CosType cosObjType = CosObjGetType(cosObj);
	return (cosObjType == CosDict);
}

/* AddAttrObj
** ----------------------------------------------------------------------
** Add an attribute to a given class. Its onwer (the client application) is "myXML-1.xx".
**
*/
static void	AddAttrObj(PDDoc pdDoc, PDSClassMap classMap, ASAtom className)
{	
	PDSAttrObj attrObj = CosNewNull();
	ASBool success = false;
	ASInt32 error = 0;

	DURING
		if (PDFL_SDK == 1)
			// We create an dummy attribute owned by the client application 
			PDSAttrObjCreate (pdDoc, PDFLClient_K, true, &attrObj);
		else
			PDSAttrObjCreate (pdDoc, AcrobatSdkPlugIn_K, true, &attrObj);

		// Creates an attribute object with the specified owner from the specified Cos stream.
		// using API PDSAttrObjCreateFromStream
		if (ASBoolToBool( ValidatePDSAttrObj (attrObj)) == true){
			PDSClassMapAddAttrObj (classMap, className, attrObj);
			success = true;
		}
	HANDLER	
		error = ERRORCODE;
	END_HANDLER

	if (error != 0){
		char buffer[256];
		ASGetErrorString(ERRORCODE, buffer, 255);
		AVAlertNote(buffer);
	}

	if (ASBoolToBool(success) == true)
		Console::displayString ("\nThe attribute has been added to the class.");
}


/* RemoveAttrObj
** ----------------------------------------------------------------------
** Remove the first attribute from a class
*/
static void	RemoveAttrObj(PDSClassMap classMap, ASAtom className)
{	
	ASInt32 error = 0;
	PDSAttrObj attrObj;

DURING
	if (CosDictKnown(classMap, className)){
		PDSClassMapGetAttrObj (classMap, className, 0 /*index=0, the first attr*/, &attrObj);
		PDSClassMapRemoveAttrObj (classMap, className, attrObj);
		Console::displayString ("\nThe first attribute in the class has been removed.");
	}
	else{
		AVAlertNote("There is no such class in the class map.");
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

/* RemoveClass
** ----------------------------------------------------------------------
** Remove a class from the class map.
*/
static void RemoveClass(PDSClassMap classMap, ASAtom className)
{
	ASInt32 error = 0;
	ASBool ok = false;
DURING
	if (CosDictKnown(classMap, className)){
		PDSClassMapRemoveClass (classMap, className);
		ok = true;
	}
	else{
		AVAlertNote("There isn't such a class in the class map.");
	}
HANDLER	
	error = ERRORCODE;
END_HANDLER

	if (ASBoolToBool(ok) == true) {
		Console::displayString("\nThe class has been removed from the class map.");
	}
	if (error != 0){
		char buffer[256];
		ASGetErrorString(ERRORCODE, buffer, 255);
		AVAlertNote(buffer);
	}
}

/* EnumClass
** ----------------------------------------------------------------------
** Enumerate all classes in the class map.
*/
static void EnumClass(PDSClassMap classMap)
{	
	// Enumerate all classes in the classMap
	Console::displayString("\nAttribute classes in the class map:");
	CosObjEnumProc myCosObjEnumProcCB = ASCallbackCreateProto(CosObjEnumProc, &GetKeyAtoms);
	CosObjEnum (classMap, myCosObjEnumProcCB, NULL);

	if (myCosObjEnumProcCB){
		ASCallbackDestroy((void*)myCosObjEnumProcCB);
	}
}

/* EnumAttrObj
** ----------------------------------------------------------------------
** And enumerate all attibutes in the given class
*/
static void EnumAttrObj(PDSClassMap classMap, ASAtom className)
{	
	ASInt32 numOfAttr = 0;
	PDSAttrObj attrObj;

	CosObjEnumProc myCosObjEnumProcCB = ASCallbackCreateProto(CosObjEnumProc, &GetKeyAtoms);

	// Enumerate all attributes in the given class
	if (CosDictKnown(classMap, className)){
		numOfAttr = PDSClassMapGetNumAttrObjs (classMap, className);
		for (ASInt32 i = 0; i < numOfAttr; i++){
			PDSClassMapGetAttrObj (classMap, className, i, &attrObj);
			char buf[50];
			sprintf (buf, "\nAttribute %d of class %s: ", i+1, ASAtomGetString(className));
			Console::displayString(buf);
			CosObjEnum( attrObj,  GetKeyAtoms, NULL);
		}
	}
	else
		AVAlertNote("There is no such a class in the class map.");

	if (myCosObjEnumProcCB)
		ASCallbackDestroy((void*)myCosObjEnumProcCB);
}

/* Get the attribute key names in the class */
ACCB1 ASBool ACCB2 GetKeyAtoms (CosObj keyObj, CosObj value, void* clientData)
{
	ASBool	process	= false;
	//	Retrieve the key name
	if( CosObjGetType( keyObj ) == CosName)
	{
		if (CosNameValue(keyObj) == AtomO_K){
			char buf[50];
			sprintf (buf, "Owner - %s", CosCopyNameStringValue(value, NULL));
			Console::displayString(buf);
		}
		else
			Console::displayString(CosCopyNameStringValue(keyObj, NULL));
		
		process	= true;
	}
	return process;
}
