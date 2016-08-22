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

 CallJsGetFieldValueSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

const int MAX_JSSCRIPT_LENGTH = 1024;
const int MAX_TEXT_LENGTH = 1024;
const int MAX_VALUE_LENGTH = 180;



/** Example of how to get field values by calling JavaScript in C code.
	See comments in CallJsTextFieldSnip for more information.
	
	@testfile TextImageFormForTest.pdf
	@requires pdf with field(s)
	@see AFExecuteThisScript
*/
void CallJsGetFieldValueSnip(){

	// need an open PDF.
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	char jsScript[MAX_JSSCRIPT_LENGTH+1];
	char ReturnValue[MAX_VALUE_LENGTH+1] = "";
	char* pReturnValue = ReturnValue;

	// get the number of fields.
	AFExecuteThisScript (pdDoc, "event.value = this.numFields;", &pReturnValue);
	
	if(!pReturnValue) {
		AVAlertNote("Error in JavaScript code execution.");
		return;
	}

	int numFields = atoi((char*) pReturnValue);

	// if there is no field, return.
	if(numFields<1) {
		ASfree(pReturnValue);
		AVAlertNote("No form fields in this PDF document.");
		return;
	}

	// echo
	char str[MAX_TEXT_LENGTH+1];
	sprintf(str,"\nTotal number of fields found : %d\n%-6s %-16s %-14s %s\n------------------------------------------\n",
		numFields, "Index", "Name", "Type", "Value");
	
	Console::displayString(str);

	// loop through fields
	int i;
	for(i=0;i<numFields;i++) {
		
		// get field name
		sprintf(jsScript,"event.value = this.getNthFieldName(%d);",i);
		AFExecuteThisScript (pdDoc, jsScript, &pReturnValue);

		char *fieldName = "";
		if(pReturnValue) 		
			fieldName = (char*) pReturnValue;
	
		// get field type
		sprintf(jsScript,"event.value = this.getField('%s').type;",fieldName);
		AFExecuteThisScript (pdDoc, jsScript, &pReturnValue);

		char *fieldType = "";
		if(pReturnValue) 
			fieldType = (char*) pReturnValue;
	
		// get field value
		sprintf(jsScript,"event.value = this.getField('%s').value;",fieldName);
		AFExecuteThisScript (pdDoc, jsScript, &pReturnValue);

		char *fieldValue = "";
		if(pReturnValue)
			fieldValue = (char*) pReturnValue;

		// scho
		sprintf(str,"%-6d %-16s %-10s %s", i+1, fieldName, fieldType, fieldValue);
		Console::displayString(str);
	}

	Console::displayString("\n-------------------------------------------");
}

SNIPRUN_REGISTER(CallJsGetFieldValueSnip, "Form:Execute JavaScript:Get all field values",
	"Example of how to get all field values by calling JavaScript in C code.")

