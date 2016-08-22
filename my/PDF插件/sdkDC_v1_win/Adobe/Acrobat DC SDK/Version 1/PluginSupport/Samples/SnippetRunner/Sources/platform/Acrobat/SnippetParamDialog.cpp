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

Implements the dialog used to get parameters passed to the snippet
from the user

*********************************************************************/

#include "SnippetParamDialog.h"
// Acrobat headers
#ifdef WIN_PLATFORM
#include "resource.h"
#else
#include "SnippetRunner.h"
#include <typeinfo>
#include "MacResource.h"
#endif

/*
	Factory methods. Single instance used.
*/
SnippetParamDialog* SnippetParamDialog::gSnippetParamDialog = NULL;

ASInt32 
SnippetParamDialog::DoDialog(const char * name, const char * desc, const char * defparams, const char * inparams, string &params)
{
	if (gSnippetParamDialog == NULL)
	{
		gSnippetParamDialog = new SnippetParamDialog();
	}
	else {
		AVAlertNote("More than a single instance of SnippetParamDialog?");
	}
	gSnippetParamDialog->fName = name;
	gSnippetParamDialog->fDescription = desc;
	gSnippetParamDialog->fDefaultParams = defparams;
	gSnippetParamDialog->fParams = inparams;

	ASInt32 result = gSnippetParamDialog->PromptForInfo();
	if (result == IDC_OK){
		result = IDOK;
		params = gSnippetParamDialog->fParams;
	}
	else {
		params = "";
		result = IDCANCEL;
	}
	delete gSnippetParamDialog;
	gSnippetParamDialog = NULL;
	return result;
}


void ASAPI 
SnippetParamDialog::PromptOnOK(ADMItemRef item, ADMNotifierRef inNotifier )
{
	char textStr[255];
	sADMItem->DefaultNotify(item, inNotifier);

	// get user input string
	ADMDialogRef dialog = sADMItem->GetDialog(item);
	ADMItemRef item1 = sADMDialog->GetItem(dialog, IDC_PARAMBOX);
	ADMUnicode unicodeText[256];
	sADMItem->GetTextW(item1, unicodeText, 255);
	ASText asText = ASTextFromUnicode(unicodeText,kUTF16HostEndian);	
	strcpy( textStr, ASTextGetScriptText(asText, kASRomanScript));
	gSnippetParamDialog->fParams = textStr;
	ASTextDestroy(asText);
}



void ASAPI 
SnippetParamDialog::PromptOnDefault(ADMItemRef item, ADMNotifierRef inNotifier )
{
	sADMItem->DefaultNotify(item, inNotifier);

	// get user input string
	ADMDialogRef dialog = sADMItem->GetDialog(item);
	ADMItemRef item1 = sADMDialog->GetItem(dialog, IDC_PARAMBOX);
	ASText text = ASTextFromScriptText(gSnippetParamDialog->fDefaultParams.c_str(), kASRomanScript);
	const ADMUnicode* unicodeText = ASTextGetUnicode(text);
	sADMItem->SetTextW(item1, unicodeText);
	ASTextDestroy(text);
}

ASErr ASAPI 
SnippetParamDialog::PromptDialogOnInit(ADMDialogRef dialogRef)
{
	//convert text to unicode, and set in dialog
	ASText text = ASTextFromScriptText(gSnippetParamDialog->fName, kASRomanScript);
	const ADMUnicode* unicodeText = ASTextGetUnicode(text);
	sADMDialog->SetTextW(dialogRef, unicodeText);
	ASTextDestroy(text);

	sADMDialog->SetDefaultItemID(dialogRef, IDC_OK);
	sADMDialog->SetCancelItemID(dialogRef, IDC_CANCEL);

	ADMItemRef itemRef;
	itemRef = sADMDialog->GetItem(dialogRef, IDC_OK);
	sADMItem->SetNotifyProc(itemRef, PromptOnOK);


	itemRef = sADMDialog->GetItem(dialogRef, IDC_DEFAULT);
	sADMItem->SetNotifyProc(itemRef, PromptOnDefault);


	itemRef = sADMDialog->GetItem(dialogRef, IDC_HELPTEXT);
	text = ASTextFromScriptText(gSnippetParamDialog->fDescription, kASRomanScript);
	const ADMUnicode* unicodeText2 = ASTextGetUnicode(text);
	sADMItem->SetTextW (itemRef,unicodeText2);
	ASTextDestroy(text);

	itemRef = sADMDialog->GetItem(dialogRef, IDC_PARAMBOX);
	text = ASTextFromScriptText(gSnippetParamDialog->fParams.c_str(), kASRomanScript);
	const ADMUnicode* unicodeText1 = ASTextGetUnicode(text);
	sADMItem->SetTextW (itemRef,unicodeText1);
	ASTextDestroy(text);

	return kSPNoError;
}

ASInt32 
SnippetParamDialog::PromptForInfo()
{

	// Initialize ADM.
	ADMUtils::InitializeADM();

	// Dispaly modal dialog to get user input
	ASInt32 rc = sADMDialog->Modal(sADMPluginRef, "ADBE:SnippetRunnerDlg", IDD_PARAMRSRC,
									kADMModalDialogStyle, PromptDialogOnInit, NULL, NULL);

	// Release ADM
	ADMUtils::ReleaseADM();

	return rc;
}
