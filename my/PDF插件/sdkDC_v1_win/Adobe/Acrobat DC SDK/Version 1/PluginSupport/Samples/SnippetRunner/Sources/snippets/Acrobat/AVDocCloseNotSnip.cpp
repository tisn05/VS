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

 AVDocCloseNotSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* prototype for function called when document did close notification sent */
ACCB1 void ACCB2 docDidCloseProc(AVDoc doc, void* clientData);

/* prototype for function called when document will close notification sent */
ACCB1 void ACCB2 docWillCloseProc(AVDoc doc, void* clientData);

/** example of how to register for document did and will close notifications. The notification of impending closure can
	access information about the AVDoc passed to the notification function. The notification of a document having closed
	is passed a reference to the document. The document is closed at this point, but not freed. This gives the developer
	an opportunity to access and manipulate any private data contained within the PDF.

	@testfile none
	@requires opening document after notification is installed
	@see AVAppRegisterNotification
	@see AVDocDidClose
	@see AVDocWillClose
	@see AVDocGetAVWindow
	@see ASTextCat
	@see ASTextDestroy
	@see ASTextFromScriptText
	@see AVWindowGetTitle
*/

void AVDocCloseNotSnip(void){

	if (SnippetRunnerUtils::toggleSnippetCheck()==false){
		AVAppRegisterNotification(AVDocDidCloseNSEL, gExtensionID, (void*)docDidCloseProc, NULL);	
		AVAppRegisterNotification(AVDocWillCloseNSEL, gExtensionID, (void*)docWillCloseProc, NULL);	
		AVAlertNote("Registered doc close notification snippet");
	} else {
		AVAppUnregisterNotification(AVDocDidCloseNSEL, gExtensionID, (void*)docDidCloseProc, NULL);	
		AVAppUnregisterNotification(AVDocWillCloseNSEL, gExtensionID, (void*)docWillCloseProc, NULL);	
		AVAlertNote("Un-registered doc close notification snippet");
	}
}

SNIPRUN_REGISTER(AVDocCloseNotSnip, "Notifications:Document:Doc will or did close", "Reports every time a document will close or did close notification is received. ")


ACCB1 void ACCB2 docDidCloseProc(AVDoc doc, void* clientData){

	AVAlertNote("Trapped document did close notification");
}

ACCB1 void ACCB2 docWillCloseProc(AVDoc doc, void* clientData){

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

	sprintf(strMsg, "Trapped document will close notification. Associated window title - %s", 
		ASTextGetEncodedCopy(windowText, (ASHostEncoding)PDGetHostEncoding()));

	ASTextDestroy(windowText);
	AVAlertNote(strMsg);
}
