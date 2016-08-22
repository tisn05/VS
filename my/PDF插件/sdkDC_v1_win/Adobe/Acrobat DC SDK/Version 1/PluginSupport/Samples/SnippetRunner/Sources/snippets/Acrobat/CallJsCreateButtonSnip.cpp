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

 CallJsCreateButtonSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

const int MAX_JSSCRIPT_LENGTH = 1024;

/** Example of how to create a button by calling JavaScript in C code.
	See comments in CallJsTextFieldSnip for more information.
	
	@testfile blank.pdf
	
	@see AFExecuteThisScript
	@see PDDocCreate
	@see PDDocCreatePage
	@see PDPageRelease
*/
void CallJsCreateButtonSnip(){

	const int inch = 72;

	// create a new target pdf file with one empty page
	PDDoc aPdDoc = PDDocCreate();
	ASFixedRect mediaBox = {fixedZero,fixedZero,ASInt32ToFixed(612),ASInt32ToFixed(792)};
	PDPage aPage = PDDocCreatePage (aPdDoc, PDBeforeFirstPage, mediaBox);
	AVDoc avDoc = AVDocOpenFromPDDoc (aPdDoc, NULL);

	// create one button.  
	char jsScript[MAX_JSSCRIPT_LENGTH+1];
	char *strFieldName = "btn_MyButton";
	char *strButtonCaption = "My Button";
	char *strMessage = "This button is created through AFExecuteThisScript function.";
	sprintf(jsScript, "var f = this.addField('%s','button', 0, [%d,%d,%d,%d]); \
					if(f!=null) { f.setAction('MouseUp', 'app.alert(\"%s\");');} \
					else { event.rc = false;} ", 
					strFieldName, inch, 10*inch, 3*inch, 10*inch-inch/2, strMessage);
					
	ASBool bRc = AFExecuteThisScript (aPdDoc, jsScript, NULL);

	// if failed, return.
	if(bRc == false) {
		AVAlertNote("Error in creating button.");
		return;
	}

	// set the button's properties
	sprintf(jsScript, "var f = this.getField('%s'); \
					f.buttonSetCaption(\'%s\');f.strokeColor = color.black; \
					f.fillColor = color.ltGray; f.borderStyle = border.b;", \
					strFieldName, strButtonCaption); 
	AFExecuteThisScript (aPdDoc, jsScript, NULL);
	
	Console::displayString("Button Created");

	PDPageRelease (aPage);
}

SNIPRUN_REGISTER(CallJsCreateButtonSnip, "Form:Execute JavaScript:Create Button",
"Example of how to create a button by calling JavaScript in C code.")
