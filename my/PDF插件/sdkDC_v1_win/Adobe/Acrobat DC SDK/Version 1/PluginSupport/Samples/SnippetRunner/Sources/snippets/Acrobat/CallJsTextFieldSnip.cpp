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

 CallJsTextFieldSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

const int MAX_JSSCRIPT_LENGTH = 1024;


/* JSCreateTextField
** ----------------------------------------------------------------------
** Convenient function calling JavaScript to create text field.
** in: all the parameters
** return: always true if pdDoc is available.
*/
ASBool JSCreateTextField(PDDoc pdDoc, int nPage, char* sFieldName, char* sText, char* sColor, 
						 int l, int t, int r, int b, ASBool bReadonly)
{
	if(!pdDoc) return false;

	char jsScript[MAX_JSSCRIPT_LENGTH+1];
	sprintf(jsScript, "var f = this.addField('%s','text', %d, [%d,%d,%d,%d]); ",sFieldName, nPage, l, t, r, b); 
	sprintf(jsScript, "%s f.value = '%s'; f.textFont = font.Helv; f.textSize = 14; f.textColor = color.%s; ",
							jsScript, sText, sColor); 
	if(bReadonly) 
		strcat(jsScript, "f.readonly = true; ");
	else
		strcat(jsScript, "f.readonly = false; f.strokeColor = color.black;");

	return AFExecuteThisScript (pdDoc, jsScript, NULL);
}


/** example of how to call JavaScript in C code to create a text field.

 This Execute JavaScript code snippet set provides sample code to show how to 
 call JavaScript code from C/C++ plug-in code. All routines are tested to be working, 
 but they are only simple sample code, you may need to further improve when use them.

 The basic function used is AcroForm Method: 
	ASBool AFExecuteThisScript (PDDoc pdd, const char* cScript, char** pRetValue);
  
 Tips:
  - PDDoc pdd:
	You must have a PDDoc in order to use call JavaScript in C code.
  - const char* cScript:
	You need to make up your own C string cScript, pay attention to using '' and "" pairs, 
	and escape symbol \ when needed.
  - char** pRetValue
	Use pRetValue to get a return value from the execution of the script  
	Pass a non-NULL value for this parameter, and assign it to event.value inside your script. 
	If on return *pRetVal is non-NULL, the caller should dispose of the string by calling ASFree.  
  - Return value:
	The method return value is the JavaScript value of event.rc in the JAvaScript code.
	This function pre-initializes it to true; You may set it to false in your script if desired.
  - avoid errors:
	To avoid errors, improve your code by adding check ( null ? typeof ? undefined?) 
	and using the try/catch/finally. JavaScript Console and debugger are good tools. 
  
	Check Acrobat SDK documents for detail.
	
	@testfile blank.pdf
	@requires open pdf file
	@see AFExecuteThisScript
	@see PDDocCreate
	@see PDDocCreatePage
	@see PDPageRelease

*/
void CallJsTextFieldSnip(){

	const int inch = 72;

	// (1) create a new target pdf file with one empty page
		CDocument doc;
	PDDoc pdDoc = (PDDoc)doc;
	if (pdDoc == NULL){
		return;
	}

	// (2) create form fields
	int npage = 0;
	int x_position = 1*inch;
	int y_position = 10*inch;

	// create a read only text field as label 
	int rc = JSCreateTextField(pdDoc, npage, "myLabel", "Please input a text string :", "black", 
						x_position, y_position, x_position+3*inch, y_position-inch/4, true);
	
	// create a text fieled for input
	x_position += 3*inch;
	rc = JSCreateTextField(pdDoc, npage, "userInput", "Change me.", "blue", 
						x_position, y_position, x_position+3*inch, y_position-inch/4, false);

	Console::displayString("Text Field created");
}

SNIPRUN_REGISTER(CallJsTextFieldSnip, "Form:Execute JavaScript:Create Text Field using JavaScript",
		"Example of how to call JavaScript in C code to create text fields.")

