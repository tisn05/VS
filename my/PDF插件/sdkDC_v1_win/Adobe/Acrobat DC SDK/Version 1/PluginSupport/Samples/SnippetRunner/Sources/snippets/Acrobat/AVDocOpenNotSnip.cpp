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

 AVDocOpenNotSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* prototype for function called when document open notification sent */
ACCB1 void ACCB2 myAVDocDidOpen(AVDoc doc, ASInt32 error, void* clientData);

/** example of how to register for document open notifications. If there was an error opening a document, an error code is displayed.
	The name of the document window is also written out to trace.

	@testfile none
	@requires opening a document after notication is installed
	@see AVAppRegisterNotification
	@see AVDocGetAVWindow
	@see AVWindowGetTitle
*/

void AVDocOpenNotSnip(void){

	if (SnippetRunnerUtils::toggleSnippetCheck()==false){
		AVAppRegisterNotification(AVDocDidOpenNSEL, gExtensionID, (void*)myAVDocDidOpen, NULL);	
		AVAlertNote("Registered doc open notification snippet");
	} else {
		AVAppUnregisterNotification(AVDocDidOpenNSEL, gExtensionID, (void*)myAVDocDidOpen, NULL);	
		AVAlertNote("Un-registered doc open notification snippet");
	}
}

SNIPRUN_REGISTER(AVDocOpenNotSnip, "Notifications:Document:Doc open", "Reports every time a document open notificationis received")

ACCB1 void ACCB2 myAVDocDidOpen(AVDoc doc, ASInt32 error, void* clientData){

	// get the window for the document
	AVWindow docWin  = AVDocGetAVWindow(doc);

	ASText windowText = ASTextNew();
	if (docWin!=NULL){
		AVWindowGetTitle(docWin, windowText);
	} else {
		ASText noWinMess = ASTextFromScriptText("No associated window title",kASRomanScript);
		ASTextCat(windowText,noWinMess);
		ASTextDestroy(noWinMess);
	}

	char strMsg[256];

	sprintf(strMsg, "Trapped document open notification. Associated window title - %s", 
		ASTextGetEncodedCopy(windowText, (ASHostEncoding)PDGetHostEncoding()));

	ASTextDestroy(windowText);
	AVAlertNote(strMsg);

	if (error != 0){
		char buff[100];
		sprintf(buff, "Error opening document, error code is %d", error);
		AVAlertNote(buff);
	}
}
