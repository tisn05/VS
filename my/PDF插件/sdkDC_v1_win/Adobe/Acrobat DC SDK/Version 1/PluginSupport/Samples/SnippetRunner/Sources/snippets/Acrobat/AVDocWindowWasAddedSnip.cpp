/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

AVDocWindowWasAddedSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"


// Prototype for function called when document new window created notification sent
ACCB1 void ACCB2 myAVDocWindowWasAdded(AVDoc doc, AVWindow window, void* clientData);

/** Example of how to register for document window added notification.
	A new window was created for a document. The notification will be
	sent for all windows created for a document, including the initial 
	window. The name of the document window is written to the alert message.

	@testfile none
	@requires creating a new window for a document
	@see AVAppRegisterNotification
	@see AVAppUnregisterNotification
	@see AVDocWindowWasAdded
	@see AVWindowGetTitle
*/
void AVDocWindowWasAddedSnip()
{
	if(SnippetRunnerUtils::toggleSnippetCheck() == false) {
		AVAppRegisterNotification(AVDocWindowWasAddedNSEL, gExtensionID, (void*)myAVDocWindowWasAdded,
			NULL);
		AVAlertNote("Registered doc window added notification snippet.");
	} else {
		AVAppUnregisterNotification(AVDocWindowWasAddedNSEL, gExtensionID, (void*)myAVDocWindowWasAdded,
			NULL);
		AVAlertNote("Un-registered doc window added notification snippet.");
	}
}

ACCB1 void ACCB2 myAVDocWindowWasAdded(AVDoc doc, AVWindow window, void* clientData)
{
	ASText asText = ASTextNew();
	if(window)
		AVWindowGetTitle(window, asText);
	else {
		ASText noWinMsg = ASTextFromScriptText("No associated window title", kASRomanScript);
		ASTextCopy(asText, noWinMsg);
		ASTextDestroy(noWinMsg);
	}

	ASText message = ASTextFromScriptText( "Trapped document window added notification. Associated window title - ", kASRomanScript );
	ASTextCat( message, asText );

	// Use AVAlertWithParams to correctly deal with Unicode file names
	AVAlertParamsRec avapr;
	memset( &avapr, 0, sizeof( avapr ) );
	avapr.size = sizeof( avapr );
	avapr.iconType = ALERT_NOTE;
	avapr.message = message;
    avapr.alertType = kAVAlertTypeOk;
	AVAlertWithParams( &avapr );

}

SNIPRUN_REGISTER(AVDocWindowWasAddedSnip, 
				 "Notifications:Document:Doc window was added", 
				 "Reports every time a document window added notification is received. ")