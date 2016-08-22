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

 AVDocWindowWasRemovedSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

// Prototype for function called when document window removed notification sent
ACCB1 void ACCB2 myAVDocWindowWasRemoved(AVDoc doc, AVWindow window, void* clientData);

/** Example of how to register for document window removed notification. 
    A window was removed from a document. The notification is sent
	after the association between the window and document
	has been served but before the window is destroyed.
	The name of the document window is written to the alert message.

	@testfile none
	@requires document window to be closed after notification is registered
	@see AVAppRegisterNotification
	@see AVAppUnregisterNotification
	@see AVDocWindowWasRemoved
	@see AVWindowGetTitle
*/

void AVDocWindowWasRemovedSnip()
{
	if(SnippetRunnerUtils::toggleSnippetCheck() == false) {
		AVAppRegisterNotification(AVDocWindowWasRemovedNSEL, gExtensionID, (void*)myAVDocWindowWasRemoved,
			NULL);
		AVAlertNote("Registered doc window removed notification snippet.");
	} else {
		AVAppUnregisterNotification(AVDocWindowWasRemovedNSEL, gExtensionID, (void*)myAVDocWindowWasRemoved,
			NULL);
		AVAlertNote("Un-registered doc window removed notification snippet.");
	}
}

ACCB1 void ACCB2 myAVDocWindowWasRemoved(AVDoc doc, AVWindow window, void* clientData)
{
	ASText asText = ASTextNew();

	if(window)
		AVWindowGetTitle(window, asText);
	else {
		ASText noWinMsg = ASTextFromScriptText("No associated window title", kASRomanScript);
		ASTextCopy(asText, noWinMsg);
		ASTextDestroy(noWinMsg);
	}

	char strMsg[256];
	sprintf(strMsg, "Trapped document window removed notification. Associated window title - %s", 
		ASTextGetEncodedCopy(asText, (ASHostEncoding)PDGetHostEncoding()));
	ASTextDestroy(asText);
	Console::displayStringNow(strMsg);
}

SNIPRUN_REGISTER(AVDocWindowWasRemovedSnip, 
				 "Notifications:Document:Doc window was removed", 
				 "Reports every time a document window removed notification is received. ")