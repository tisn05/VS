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

 SetDocBaseURLSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

const ASInt32 metaStrLen = 1024;

static ASBool SetMetaProperty(PDDoc pdd, char *key, char *value);
static ACCB1 ASText ACCB2 PDDocGetBaseURI(PDDoc pdd, char *key);

/** example of how to set and get a PDF document metadata property.
	@testfile blank.pdf
	@requires open pdf
	@see PDDocGetXAPMetadata
	@see PDDocSetXAPMetadata
*/
void SetDocBaseURLSnip(ParamManager * thePM)
{
	// must have an active pdf.
	CDocument document;
	PDDoc activePDDoc = (PDDoc)document;
	if(activePDDoc == NULL)
		return; 

	char str[metaStrLen+50];
	string url;

	// get user input that is the new Base URL.
	if (thePM->getNextParamAsString(url)==false){
		Console::displayString("There is no input for Base URL");
		return;
	}
	char NewBaseURL[metaStrLen+1];
	strcpy(NewBaseURL,url.c_str());

DURING

	// check existing base URL
	ASText BaseURLASText = PDDocGetBaseURI(activePDDoc, "BaseURL"); 
 
	// Get the existing base URL of PDF
	if (ASBoolToBool( ASTextIsEmpty (BaseURLASText) ) == true)
		strcpy (str, "There was no base URL found for this PDF.");
	else {
		sprintf (str, "The existing base URL for this PDF was\n\t%s\n",
						ASTextGetScriptText(BaseURLASText, kASRomanScript));
	}
	Console::displayString(str);

	// set new base URL 
	if(ASBoolToBool(SetMetaProperty (activePDDoc,"BaseURL" ,NewBaseURL)) == true) {
		sprintf (str, "The base URL for this PDF is set as \n\t%s\n",NewBaseURL);
		PDDocSetFlags(activePDDoc, PDDocNeedsSave);
	}
	else
		sprintf (str, "Error in setting base URL.");
	
	Console::displayString(str);

HANDLER
	char buf[metaStrLen+1];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, metaStrLen));
	return;

END_HANDLER
}

/** Register snippet */
SNIPRUN_REGISTER_WITH_DIALOG(SetDocBaseURLSnip, "PD Layer:Metadata:Set Document Base URL","Set PDF document Base URL metadata. Parameters [new Base URL]", "http://www.adobe.com")



/** PDDocGetProperty
** ----------------------------------------------------------------------
** returns a Doc metadata property in ASText. 
*/
static ACCB1 ASText ACCB2 PDDocGetBaseURI(PDDoc pdd, char *key)
{
	ASText namespaceName = ASTextFromPDText("http://ns.adobe.com/pdfx/1.3/");
	ASText path = ASTextFromPDText(key);

	ASText MetaPropText = PDDocGetXAPMetadataProperty (pdd, namespaceName, path);
	
	return MetaPropText;
}


/** SetMetaProperty
** ----------------------------------------------------------------------
** Set metadata properties with Acrobat 6.0 APIs, PDDocSetXAPMetadataProperty.
** Returns true if successful, or false.
*/
static ASBool SetMetaProperty(PDDoc pdd, char *key, char *value)
{
	if(pdd == NULL)	return false;

	ASText namespaceName = ASTextFromPDText("http://ns.adobe.com/pdfx/1.3/");
	ASText namespacePrefix = ASTextFromPDText("pdfx");
	ASText path = ASTextFromPDText(key);
	ASText newValue = ASTextFromPDText(value);
	
DURING
	PDDocSetXAPMetadataProperty (pdd,
                                    namespaceName,
                                    namespacePrefix,
                                    path, 
                                    newValue);

HANDLER
	char buf[256];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, 255));
	return false;
END_HANDLER
	return true;
}

