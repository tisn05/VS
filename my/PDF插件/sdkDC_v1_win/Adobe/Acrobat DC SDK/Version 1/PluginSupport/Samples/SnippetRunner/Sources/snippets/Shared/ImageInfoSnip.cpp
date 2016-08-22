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

 ImageInfoSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

#define Image_Str "Image"
#define Subtype_Str "Subtype"
#define Width_Str "Width"
#define Height_Str "Height"
#define Length_Str "Length"
#define Filter_Str "Filter"
#define BitsPerComponent_Str "BitsPerComponent"
#define Intent_Str "Intent"

static ACCB1 ASBool ACCB2 CosEnumProc (CosObj obj, CosObj value, void* clientData);
static void GetCosNamedArray(CosObj cosArrayObj, char * name);

/** This snippet is intended to show how to enumerate images (xobject images) on
    a page through the PDDoc resources enumeration API. It also shows on access image 
    information in a PDF document such as height, width, size,	compression filters, rendering intent etc.
	@testfile cottage.pdf
	@requires pdf with image image xobjects
	@see PDDocEnumResources
	@see CosArrayGet
	@see CosArrayLength
	@see CosDictGetKeyString
	@see CosNameValue
	@see CosCopyNameStringValue
	@see CosDictKnown
	@see CosIntegerValue
  	@see CosObjGetType
*/
void ImageInfoSnip(ParamManager * thePM)
{
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	ASInt32 pgNum;
	thePM->getNextParamAsInt(pgNum);

	ASInt32 numPages = PDDocGetNumPages(pdDoc);
	if (pgNum <= 0 || pgNum > numPages){
		AVAlertNote ("Invalid page number.\n");
		return;
	}

	CosObjEnumProc cosEnumProcCB = ASCallbackCreateProto(CosObjEnumProc, &CosEnumProc);
	PDDocEnumResources (pdDoc, pgNum-1, pgNum-1, ASAtomFromString("XObject"), cosEnumProcCB, pdDoc);
	ASCallbackDestroy((void*)cosEnumProcCB);
}

SNIPRUN_REGISTER_WITH_DIALOG(ImageInfoSnip, "PD Layer:Document:Image Info",
"Get image information (width, height, size, compression filters, rendering intent). \
Enter a valid page number.", "1")

/* GetCosNamedArray
** ----------------------------------------------------------------------
** Get the named value of a cos array. Return the result in a buffer "name"
*/
static void GetCosNamedArray(CosObj cosArray, char * name)
{
	CosType type = CosObjGetType (cosArray);
	
	// If the cos obj is a named value, get the value
	if (type == CosName){
		strcat(name, CosCopyNameStringValue (cosArray, NULL));
		strcat (name, " ");
		return;
	}

	// If the cos obj is a cos array, get the array of named values
	if (type == CosArray) {
		CosObj arrayEntry;
		ASInt32 len = CosArrayLength(cosArray);
		for(int i=0; i <=len-1; i++)
		{
			arrayEntry = CosArrayGet(cosArray, i);
			GetCosNamedArray (arrayEntry, name); 
		}
	}
}

/* CosEnumProc
** ----------------------------------------------------------------------
** Enumerate cos objects, if the cos obj is an image xobject, get the image info.
** If Acrobat throw exception, return false, otherwise return true
*/
static ACCB1 ASBool ACCB2 CosEnumProc (CosObj obj, CosObj value, void* clientData)
{
	PDDoc pdDoc = static_cast<PDDoc>(clientData);	
	//ASAtom typeAtom, intentAtom;
	char *typeString;
	CosObj subtypeObj, intentObj, lengthObj, filterObj, widthObj, heightObj, bitObj;
	ASInt32 width, height, length, bits;
	char filter[128], intent[64], output[1024];
	filter[0] = output[0] = intent[0] = '\0';
	width = height = length = bits = 0;

	subtypeObj = CosDictGetKeyString (obj, Subtype_Str);
	typeString = CosCopyNameStringValue(subtypeObj, NULL); 

DURING
	// If the XObject is image type, get image info 
	if (strcmp (typeString, Image_Str ) == 0 ) {
		// Get width, height and length
		widthObj = CosDictGetKeyString (obj, Width_Str);
		heightObj = CosDictGetKeyString (obj, Height_Str);
		lengthObj = CosDictGetKeyString (obj, Length_Str);

		width = CosIntegerValue (widthObj);
		height = CosIntegerValue (heightObj);
		length = CosIntegerValue (lengthObj);

		// Get the optional array of decode filters
		if (CosDictKnownKeyString (obj, Filter_Str)){
			filterObj = CosDictGetKeyString (obj, Filter_Str);
			GetCosNamedArray(filterObj, filter);
		}
		else
			strcpy (filter, "Not specified");
		
		// Get the optional image bits per components value
		if (CosDictKnownKeyString (obj, BitsPerComponent_Str)){
			bitObj = CosDictGetKeyString (obj, BitsPerComponent_Str);
			bits = CosIntegerValue (bitObj);
		}

		// Get the optional image rendering intent value
		if (CosDictKnownKeyString (obj, Intent_Str)){
			intentObj = CosDictGetKeyString (obj, Intent_Str);
			strcpy (intent, CosCopyNameStringValue(intentObj, NULL));
		}
		else
			strcpy (intent, "Not specified");

		sprintf(output, "Image Information:\n\
\nWidth: %d \nHeigth: %d \nSize: %d bytes \nFilter: %s \
\nBits per components: %d \nRendering intent: %s\n", 
width, height, length, filter, bits, intent);

		Console::displayString (output);
	}
HANDLER
	char buf[256];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, 255));
	return false;
	END_HANDLER
	return true;
}
