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

 CallJsSignatureFieldSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

const int MAX_JSSCRIPT_LENGTH = 1024;

/* JSCreateFreeTextAnnot
** ----------------------------------------------------------------------
** Convenient function calling JavaScript to create a free text annotation.
** in: all parameters
** return: always true if pdDoc is available.
** Sample code only, protection is not added.
*/
ASBool JSCreateFreeTextAnnot(PDDoc pdDoc, int nPage, char* sFieldName, char* sText, char* sColor, 
						 int l, int t, int r, int b)
{
	if(!pdDoc) return false;

	char jsScript[MAX_JSSCRIPT_LENGTH+1];
	sprintf(jsScript, "var a = this.addAnnot({page:%d,type:'FreeText'}); a.name='%s';a.contents='%s';a.textFont=font.Helv;a.textSize=12;a.rect=[%d,%d,%d,%d]; a.fillColor=color.white; a.textColor=color.%s; a.width=0; a.alignment=0;",nPage,sFieldName,sText, l, t, r, b,sColor); 

	return AFExecuteThisScript (pdDoc, jsScript, NULL);
}

/* JSCreateSignatureField
** ----------------------------------------------------------------------
** Convenient function calling JavaScript to create a blank digital signature field.
** in: all parameters
** return: true if succeeded, false if failed.
*/
ASBool JSCreateSignatureField(PDDoc pdDoc, int nPage, char* sFieldName, char* sColor, 
						 int l, int t, int r, int b)
{
	if(!pdDoc) return false;

	char jsScript[MAX_JSSCRIPT_LENGTH+1];
	sprintf(jsScript, "var f = this.addField('%s','signature', %d, [%d,%d,%d,%d]); if(f == null) event.rc = false; else f.fillColor = color.%s;",
						sFieldName, nPage, l, t, r, b, sColor); 
	
	return AFExecuteThisScript (pdDoc, jsScript, NULL);
}


/** Example of how to create a blank digital signature field by calling JavaScript in C code. 
	See comments in CallJsTextFieldSnip for more information.
	
	@testfile blank.pdf
	@requires open pdf file
	@see AFExecuteThisScript
	@see PDDocCreate
	@see PDDocCreatePage
	@see PDPageRelease
*/
void CallJsSignatureFieldSnip(){

	const int inch = 72;

	// create a new target pdf file with one empty page
	CDocument doc;
	PDDoc pdDoc = (PDDoc)doc;
	if (pdDoc == NULL){
		return;
	}

	// create free text annotation as label
	int npage = 0;
	int x_position = 1*inch;
	int y_position = 10*inch;

	char *fieldValue = "Signature:";
	char *fieldName = "SignatureLabel";
	int rc = JSCreateFreeTextAnnot(pdDoc, npage, fieldName, fieldValue, "black", 
						x_position, y_position, x_position+int(0.85*inch), y_position-int(0.25*inch));

	// create a blank signature field with light gray background color
	x_position += inch;
	char *fieldName1 = "Signature";
	rc = JSCreateSignatureField(pdDoc, npage, fieldName1, "ltGray", 
						x_position, y_position, x_position+4*inch, y_position-int(0.7*inch));

	Console::displayString("Signature Field created");
}

SNIPRUN_REGISTER(CallJsSignatureFieldSnip, "Form:Execute JavaScript:Create signature field using JavaScript",
	"Example of how to create a blank digital signature field by calling JavaScript in C code.") 

