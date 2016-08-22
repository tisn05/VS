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

 PDDocDidDeletePagesSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"
#include "NSelExpT.h"

/* prototype for the registered notification function */
ACCB1 void ACCB2 docDeletePageProc(PDDoc doc, ASInt32 fromPage, ASInt32 toPage, ASInt32 error, void* clientData);

/** Example of how to register a function to receive PDDocDidDeletePages notifications.
	This snippet toggles it baviour, execute it to enable, then execute it to disable.
	@see AVAppRegisterNotification
	@see PDDocDidDeletePages
*/
void PDDocDidDeletePagesNotSnip(void){
	if (SnippetRunnerUtils::toggleSnippetCheck()==false){
	    // for PDFL code, define AVAppRegisterNotification as AVExtensionMgrRegisterNotification in "SnippetRunner.h"
		AVAppRegisterNotification(PDDocDidDeletePagesNSEL, gExtensionID, (void *)docDeletePageProc, NULL);	
		AVAlertNote("Registered doc delete page notification snippet");
	}
	else {
	    // for PDFL code, define AVAppUnregisterNotification as AVExtensionMgrUnregisterNotification in "SnippetRunner.h"
		AVAppUnregisterNotification(PDDocDidDeletePagesNSEL, gExtensionID, (void *)docDeletePageProc, NULL);
		AVAlertNote("Un-registered doc delete page notification snippet");
	}
}

SNIPRUN_REGISTER(PDDocDidDeletePagesNotSnip, "Notifications:Document:Delete page notification","Reports every time pages are deleted using the PDDocDidDeletePages notifcation")

ACCB1 void ACCB2 docDeletePageProc(PDDoc doc, ASInt32 fromPage, ASInt32 toPage, ASInt32 error, void* clientData){
	string strMsg = "Delete pages notification - From page ";
	char strBuf[256];
	snprintf(strBuf, sizeof(strBuf), "%d", ++fromPage);
	strMsg += strBuf;
	strMsg += " to page ";
	snprintf(strBuf, sizeof(strBuf), "%d", ++toPage);
	strMsg += strBuf;
	strMsg += " Error code (0 == no error) ";
	snprintf(strBuf, sizeof(strBuf), "%d", error);
	strMsg += strBuf;
	AVAlertNote(strMsg.c_str());

}
 

