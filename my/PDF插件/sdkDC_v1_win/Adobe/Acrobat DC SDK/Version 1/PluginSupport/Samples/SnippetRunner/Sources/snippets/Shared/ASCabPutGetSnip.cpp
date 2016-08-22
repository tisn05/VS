/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

ASCabPutGetSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* prototype for the actual callback function */
static ACCB1 ASBool ACCB2 myASCabEnumProc(ASCab theCab, const char * theKey, 
	ASCabValueType theType, void * clientData);

/** The ASCabPutGetSnip snippet demonstrates how to put arbitrary key-value
    pairs into an ASCab object and how to retrieve the corresponding value
	for a specified key from the ASCab object.    

	@see ASCabNew
	@see ASCabGetInt
	@see ASCabPutInt
	@see ASCabGetInt64
	@see ASCabPutInt64
	@see ASCabGetUns64
	@see ASCabPutUns64
	@see ASCabGetUns
	@see ASCabPutUns
	@see ASCabGetDouble
	@see ASCabPutDouble
	@see ASCabGetText
	@see ASCabPutText
	@see ASTextGetEncoded
	@see ASTextFromScriptText
	@see ASCabGetAtom
	@see ASCabPutAtom
	@see ASAtomFromString
	@see ASAtomGetString
	@see ASCabGetBool
	@see ASCabPutBool
	@see ASCabGetString
	@see ASCabPutString
	@see ASCabGetPointerType
	@see ASCabGetPointer
	@see ASCabPutPointer
	@see ASCabGetBinary
	@see ASCabPutBinary
	@see ASCabGetCab
	@see ASCabPutCab
	@see ASCabEnum
	@see ASCabNumEntries
	@see ASCabDestroy
	@see ASCallbackCreateProto
*/

/*-------------------------------------------------------------------
    Constants
____________________________________________________________________*/
const char * STR_PTR_TYPE = "char*";
const char * INT_PTR_TYPE = "int*";

/*------------------------------------------------------------------
	Implementation
-------------------------------------------------------------------*/
void ASCabPutGetSnip()
{
	char strBuf[256];
	DURING
		// Create an ASCab object
		ASCab myCab = ASCabNew();
		if(!myCab) {
			sprintf(strBuf, "Failed to create a new empty cabinet.");
			Console::displayString(strBuf);
			E_RTRN_VOID; 
		}

		// Put an ASInt32 value into the cabinet under the key "keyInt"
		ASCabPutInt(myCab, "keyInt", -1);

		// Put an AsUns value into the cabinet under the key "keyUns"
		ASCabPutUns(myCab, "keyUns", 1);

		// Put a double value into the cabinet under the key "keyDouble"
		ASCabPutDouble(myCab, "keyDouble", 1.12345);

		// Put an ASInt64 value into the cabinet under the key "keyInt64"
		ASCabPutInt64(myCab, "keyInt64", -123456789012345LL);

		// Put an ASUns64 value into the cabinet under the key "keyUns64"
		ASCabPutUns64(myCab, "keyUns64", 123456789012345ULL);

		// Put an ASText value into the cabinet under the key "keyASText"
		ASCabPutText(myCab, "keyASText", ASTextFromScriptText("ASText", kASRomanScript));

		// Put an ASAtom value into the cabinet under the key "keyAtom"
		ASCabPutAtom(myCab, "keyAtom", ASAtomFromString("ASAtom"));

		// Put an ASBool value into the cabinet under the key "keyBool"
		ASCabPutBool(myCab, "keyBool", true);

		// Put a string into the cabinet under the key "keyString"
		char strVal[] = "String";
		char * strPtr = (char *)ASmalloc(strlen(strVal) + 1);
		memset(strPtr, 0, sizeof(strPtr));
		strcpy(strPtr, strVal);
		ASCabPutString(myCab, "keyString", strPtr);

		// Put an ASInt32 pointer into the cabinet under the key "keyIntPtr"
		int anInt = 10;
		int * anIntPtr = &anInt;
		ASCabPutPointer(myCab, "keyIntPtr", int*, anIntPtr, NULL);

		// Put a string pointer into the cabinet under the key "keyStrPtr"
		char * anStrPtr = "a string pointer";
		ASCabPutPointer(myCab, "keyStrPtr", char*, anStrPtr, NULL);	

		// Put a binary object into the cabinet under the key "keyBinary"
		char strVal2[] = "Binary";
		char * strBin = (char *)ASmalloc(strlen(strVal2) + 1);
		memset(strBin, 0x00, sizeof(strBin));
		strcpy(strBin, strVal2);
		ASInt32 binSize = strlen(strVal2) + 1;
		ASCabPutBinary(myCab, "keyBinary", (void*)strBin, binSize);

		// Put a ASCab object into the cabinet under the key "keyASCab"
		ASCab elemCab = ASCabNew();
		if(elemCab) {
			ASCabPutInt(elemCab, "keyInt", 11);
			ASCabPutCab(myCab, "keyASCab", elemCab);
		}

		sprintf(strBuf, "Dumping the cabinet elements:\n");
		Console::displayString(strBuf);
		sprintf(strBuf, "Number of key-value pairs in the cabinet: %d\n", ASCabNumEntries(myCab));
		Console::displayString(strBuf);

		// Enumerate the newly creately ASCab object
		ASCabEnum(myCab, ASCallbackCreateProto(ASCabEnumProc, myASCabEnumProc), NULL);

		// Destroy the ASCab object
		ASCabDestroy(myCab);

	HANDLER
		ASInt32 iErrCode = ASGetExceptionErrorCode();
		const ASInt32 BUF_SIZE = 256;
		char errBuf[BUF_SIZE];
		AVAlertNote(ASGetErrorString(iErrCode, errBuf, BUF_SIZE));
	END_HANDLER
}

static ACCB1 ASBool ACCB2 myASCabEnumProc(ASCab theCab, const char * theKey, 
	ASCabValueType theType, void * clientData)
{
	ASText asTextVal;
	ASAtom asAtomVal;
	const char * strPtr;
	char strBuf[256];

	switch (theType) {
		case kASValueInteger:
			sprintf(strBuf, "Key = %s, Int32Val = %d\n", theKey, ASCabGetInt(theCab, theKey, 0));
			break;
		case kASValueUns:
			sprintf(strBuf, "Key = %s, Uns32Val = %u\n", theKey, ASCabGetUns(theCab, theKey, 0));
			break;
		case kASValueDouble:
			sprintf(strBuf, "Key = %s, doubleVal = %f\n", theKey, ASCabGetDouble(theCab, theKey, 0.0));
			break;
		case kASValueInt64:
#if WIN_PLATFORM
			sprintf(strBuf, "Key = %s, Int64Val = %I64d\n", theKey, ASCabGetInt64(theCab, theKey, 0));
#else
			sprintf(strBuf, "Key = %s, Int64Val = %lld\n", theKey, ASCabGetInt64(theCab, theKey, 0));		
#endif
			break;
		case kASValueUns64:
#if WIN_PLATFORM
			sprintf(strBuf, "Key = %s, Uns64Val = %I64u\n", theKey, ASCabGetUns64(theCab, theKey, 0));	
#else
			sprintf(strBuf, "Key = %s, Uns64Val = %llu\n", theKey, ASCabGetUns64(theCab, theKey, 0));
#endif
			break;
		case kASValueText:
			asTextVal = ASCabGetText(theCab, theKey);
			sprintf(strBuf, "Key = %s, ASText content = %s\n", theKey, ASTextGetEncoded(asTextVal, 0));
			break;
		case kASValueAtom:
			asAtomVal = ASCabGetAtom(theCab, theKey, ASAtomNull);
			sprintf(strBuf, "Key = %s, ASAtom content = %s\n", theKey, ASAtomGetString(asAtomVal));
			break;
		case kASValueBool:
			sprintf(strBuf, "Key = %s, boolVal = %s\n", theKey, ASCabGetBool(theCab, theKey, false) ? "true" : "false");
			break;
		case kASValueString:
			sprintf(strBuf, "Key = %s, stringVal = %s\n", theKey, ASCabGetString(theCab, theKey));
			break;
		case kASValuePointer:
			if(!strcmp(INT_PTR_TYPE, ASCabGetPointerType(theCab, theKey))) {
				sprintf(strBuf, "Key = %s, value pointed to by the pointer: %d\n", theKey, *(ASCabGetPointer(theCab, theKey, int*)));
			} else if (!strcmp(STR_PTR_TYPE, ASCabGetPointerType(theCab, theKey))) {
				sprintf(strBuf, "Key = %s, value pointed to by the pointer: %s\n", theKey, ASCabGetPointer(theCab, theKey, char*));
			}
			break;
		case kASValueBinary:
			ASInt32 iNumBytes;
			strPtr = (const char *)ASCabGetBinary(theCab, theKey, &iNumBytes);
			sprintf(strBuf, "Key = %s, value = %s\n", theKey, strPtr);
			break;
		case kASValueCabinet:
			sprintf(strBuf, "Key = %s, dump nested cabinet:\n", theKey);
			Console::displayString(strBuf);
			ASCabEnum(ASCabGetCab(theCab, theKey), ASCallbackCreateProto(ASCabEnumProc, myASCabEnumProc), NULL);
			sprintf(strBuf, "End of dumping the nested cabinet\n");
	}
	Console::displayString(strBuf);
	return true;
}

SNIPRUN_REGISTER(ASCabPutGetSnip, "AS Layer:ASCabPutGetSnip", "This snippet demonstrates how to put arbitrary \
key-value pairs into an ASCab object and how to retrieve the corresponding value for a specified key from the \
ASCab object.")
