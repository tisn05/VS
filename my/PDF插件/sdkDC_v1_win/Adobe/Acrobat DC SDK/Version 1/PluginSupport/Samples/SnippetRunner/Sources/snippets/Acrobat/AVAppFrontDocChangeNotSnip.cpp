/********************************************************************
 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.
 ---------------------------------------------------------------------
 AVAppFrontDocChangeNotSnip.cpp
*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* prototype for function invoked when notified */

ACCB1 void ACCB2 frontDocChangeProc(AVDoc doc, void* clientData);

/** example of how to register for front document change notifications. Changing the application document
	focus (for example, moving between two different document's windows, or closing the foremost window) 
	causes a AVAppFrontDocDidChange notification.
	
	@testfile none
	@see AVAppRegisterNotification
	@see AVAppUnregisterNotification
	@see AVAppFrontDocDidChange
	@see ASTextCat
	@see ASTextNew
	@see ASTextFromScriptText
	@see AVDocGetAVWindow
	@see ASTextDestroy
	@see AVWindowGetTitle
*/

void AVAppFrontDocChangeNotSnip(void){
	if (SnippetRunnerUtils::toggleSnippetCheck()==false){
		AVAppRegisterNotification(AVAppFrontDocDidChangeNSEL, gExtensionID, (void*)frontDocChangeProc, NULL);	
		AVAlertNote("Registered front doc change notification snippet");
	}

	else {
		AVAppUnregisterNotification(AVAppFrontDocDidChangeNSEL, gExtensionID, (void*)frontDocChangeProc, NULL);	
		AVAlertNote("Un-registered front doc change notification snippet");
	}
}


SNIPRUN_REGISTER(AVAppFrontDocChangeNotSnip, "Notifications:Document:Front Doc Change","Reports every time a front document change notification is received")


ACCB1 void ACCB2 frontDocChangeProc(AVDoc doc, void* clientData){

	// get the window for the document
	// if we kill the last front window, doc will be nill.

	ASText windowText = ASTextNew();

	if (doc==NULL){
		ASText noWinMess = ASTextFromScriptText("No associated window title (no document open?)",kASRomanScript);
		ASTextCat(windowText,noWinMess);
		ASTextDestroy(noWinMess);	
	}
	else {
		AVWindow docWin  = AVDocGetAVWindow(doc);

		// belt and braces check...
		if (docWin!=NULL){
			AVWindowGetTitle(docWin, windowText);
		}
	}

	ASText astMsg = ASTextNew();
	
	ASTextCatMany(
		astMsg, 
		ASTextFromScriptText("[Notification] Trapped front document changed notification Associated title - ", kASRomanScript),
		windowText, 
		ASTextFromScriptText("\n", kASRomanScript),
		NULL);
	
	Console::displayStringNow(astMsg);
	
	ASTextDestroy(astMsg);
	ASTextDestroy(windowText);
}





