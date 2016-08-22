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

 CallJsResponseSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

const int MAX_JSSCRIPT_LENGTH = 1024;
const int MAX_VALUE_LENGTH = 80;


/* JSResponse
** ----------------------------------------------------------------------
** Convenient function calling JavaScript response method
** in: PDDoc pdDoc, char* sQuestion, char* sAnswerDefault
** out: sAnswer, the string length is limited ( cut ) to MAX_VALUE_LENGTH = 80.
** return: true if succeeded, false if failed.
*/
ASBool JSResponse(PDDoc pdDoc, char* sTitle, char* sQuestion, char* sAnswer)
{
	if(!pdDoc) return false;

	char jsScript[MAX_JSSCRIPT_LENGTH+1];
	char ReturnValue[MAX_VALUE_LENGTH+1] = "";
	char* pReturnValue = ReturnValue;
	sprintf(jsScript, "var rcode = app.response( {cQuestion: '%s', cTitle: '%s',cDefault: '%s'} );  if(rcode!=null) event.value=rcode;", sQuestion, sTitle,sAnswer); 
	ASBool bRc = AFExecuteThisScript (pdDoc, jsScript, &pReturnValue);
	if(pReturnValue) {
		memcpy(sAnswer,(char*) pReturnValue,MAX_VALUE_LENGTH);
		ASfree(pReturnValue);
		return true;
	}
	else {
		strcpy(sAnswer,"");
		return false;
	}
}



/** Example of getting user input using JavaScript app.response method in C code.
	See comments in CallJsTextFieldSnip for more information.
	
	@testfile blank.pdf
	@requires nothing special, except an open document
	@see AFExecuteThisScript
*/
void CallJsResponseSnip(){

	// need a PDDoc for JavaScript execution.
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	// pop up the response dislog
	char str[MAX_VALUE_LENGTH+1]="change me";
	ASBool rc = JSResponse(pdDoc, "Snippet", "Input a text string", str);

	// echo
	if(rc) {
		char str1[MAX_VALUE_LENGTH+15]="Input is: ";
		strcat(str1,str);
		Console::displayString(str1);
	}
}

SNIPRUN_REGISTER(CallJsResponseSnip, "Form:Execute JavaScript:Response",
	"getting user input using JavaScript app.response method in C code")

