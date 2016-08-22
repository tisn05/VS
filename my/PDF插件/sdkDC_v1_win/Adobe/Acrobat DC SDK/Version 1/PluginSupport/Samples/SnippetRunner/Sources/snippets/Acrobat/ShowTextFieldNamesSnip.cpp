/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ShowTextFieldNamesSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"



/*-------------------------------------------------------
	Enumerate Fields
-------------------------------------------------------*/

/* FieldEnumProc
** ------------------------------------------------------
**
** Function enumerated on PDFields.
** Perform various operations on the field.
**
** Returns true to continue enumeration
*/
ACCB1 ASBool ACCB2 FieldEnumProc (PDField fldP, void *clientData)
{
	CosDoc cosDoc;
	CosObj fieldCosObj, keyValueObj, stringObj;
	char *fieldName;
	ASInt32 fieldNameLength;

	DURING

		if (AFPDFieldIsValid(fldP)) {

			// Get the Cos object for the field.
			fieldCosObj = AFPDFieldGetCosObj (fldP);

			// If object is a text field, we'll set its value to its name.
			// A field may inherit the FT key from an ancestor, but we don't
			// check for that...
			if (!CosDictKnownKeyString(fieldCosObj, "FT"))
				E_RETURN(true);

			keyValueObj = CosDictGetKeyString (fieldCosObj, "FT");
			if (CosName != CosObjGetType(keyValueObj))
				E_RETURN(true)

			if ((CosNameValue(keyValueObj) == ASAtomFromString("Tx"))) {

				// We're set, change the value of the field to the field name.
				cosDoc = CosObjGetDoc (fieldCosObj);
				fieldName = AFPDFieldGetName (fldP);
				fieldNameLength = strlen (fieldName);
				stringObj = CosNewString (cosDoc, false, fieldName, fieldNameLength);
				AFPDFieldSetValue (fldP, stringObj);

				//if I wanted to reset the field, I could this way:
				//	AFPDFieldReset (fldP);
			}
		}
	HANDLER
		// Nothing too critical can cause us to land here, just continue enumeration
	END_HANDLER
	return true;
}


/** for all the text fields in the document, it will replace 
	their current value with the name of that field.
	@testfile formfields.pdf
	@requires pdf with text form fields
	@see AFPDFieldIsValid
	@see AFPDFieldGetCosObj
	@see CosDictKnownKeyString
	@see CosDictGetKeyString
	@see CosObjGetType
	@see CosNameValue
	@see ASAtomFromString
	@see CosObjGetDoc
	@see AFPDFieldGetName
	@see CosNewString
	@see AFPDFieldSetValue
	@see AFPDDocLoadPDFields
	@see AVDocClearSelection
	@see AFPDDocEnumPDFields
*/
void ShowTextFieldNamesSnip()
{
	AVDoc avDoc = NULL;
	PDDoc pdDoc;
	char errorBuf[256];
	AFPDFieldEnumProc cbEnumProc;

	CDocument document;
	avDoc = (AVDoc)document;
	if(!avDoc)
		return;
	DURING
		// Get the PDDoc for the active doc.

		pdDoc = AVDocGetPDDoc (avDoc);

		// Ensure that all fields in form get created.
		AFPDDocLoadPDFields (pdDoc);

		//need to clear selection so that field value change is recorded
		AVDocClearSelection (avDoc, true);

		// Perform enumeration
		cbEnumProc = ASCallbackCreateProto(AFPDFieldEnumProc, FieldEnumProc);
		AFPDDocEnumPDFields (pdDoc, false, false, cbEnumProc, NULL);

	HANDLER
		ASGetErrorString (ASGetExceptionErrorCode(), errorBuf, 256);
		AVAlertNote (errorBuf);
	END_HANDLER

		ASCallbackDestroy( (void*)cbEnumProc );
}



SNIPRUN_REGISTER(ShowTextFieldNamesSnip, "AV Layer:Forms:Show Text Field Names", "Displays text field names in the field.")

