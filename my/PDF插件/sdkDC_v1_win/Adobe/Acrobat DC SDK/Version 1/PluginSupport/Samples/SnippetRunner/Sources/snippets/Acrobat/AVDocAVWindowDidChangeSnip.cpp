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

AVDocAVWindowDidChangeSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"


// Prototype for function called when document new window created notification sent
ACCB1 void ACCB2 myAVDocAVWindowDidChange(AVDoc doc, AVWindow oldWindow, 
										  AVWindow newWindow, void* clientData);

/** Example of how to register for document window change notification. 
	The active window associated with a document has changed. The notification 
	is sent after the association between the window and document
	has been served but before the active window is changed.
    The names of the document windows are written to the alert message. 

	@testfile none
	@see AVAppRegisterNotification
	@see AVAppUnregisterNotification
	@see AVDocAVWindowDidChange
	@see AVWindowGetTitle
*/
void AVDocAVWindowDidChangeSnip()
{
	if(SnippetRunnerUtils::toggleSnippetCheck() == false) {
		AVAppRegisterNotification(AVDocAVWindowDidChangeNSEL, gExtensionID, (void*)myAVDocAVWindowDidChange,
			NULL);
		AVAlertNote("Registered doc AVWindow change notification snippet.");
	} else {
		AVAppUnregisterNotification(AVDocAVWindowDidChangeNSEL, gExtensionID, (void*)myAVDocAVWindowDidChange,
			NULL);
		AVAlertNote("Un-registered doc AVWindow change notification snippet.");
	}
}

ACCB1 void ACCB2 myAVDocAVWindowDidChange(AVDoc doc, AVWindow oldWindow, 
										  AVWindow newWindow, void* clientData)
{
	ASText asNewWindow = ASTextNew();
	if(newWindow)
		AVWindowGetTitle(newWindow, asNewWindow);
	else {
		ASText noNewWinMsg = ASTextFromScriptText("No associated new window title", kASRomanScript);
		ASTextCopy(asNewWindow, noNewWinMsg);
		ASTextDestroy(noNewWinMsg);
	}

	ASText asOldWindow = ASTextNew();
	if(oldWindow)
		AVWindowGetTitle(oldWindow, asOldWindow);
	else {
		ASText noOldWinMsg = ASTextFromScriptText("No associated old window title", kASRomanScript);
		ASTextCopy(asOldWindow, noOldWinMsg);
		ASTextDestroy(noOldWinMsg);
	}

	ASText astMsg = ASTextNew();

	ASTextCatMany(
		astMsg,
		ASTextFromScriptText("[Notification] Trapped document AVWindow changed notification. Associated new window title - ", kASRomanScript),
		asNewWindow,
		ASTextFromScriptText(", old window title - ", kASRomanScript),
		asOldWindow,
		ASTextFromScriptText("\n", kASRomanScript),
		NULL);

	Console::displayStringNow(astMsg);
	ASTextDestroy(asNewWindow);
	ASTextDestroy(asOldWindow);
}

SNIPRUN_REGISTER(AVDocAVWindowDidChangeSnip, 
				 "Notifications:Document:Doc AVWindow Change", 
				 "Reports every time a document AVWindow changed notification is received. Old Window only exists where there is a different view of the same document open.")