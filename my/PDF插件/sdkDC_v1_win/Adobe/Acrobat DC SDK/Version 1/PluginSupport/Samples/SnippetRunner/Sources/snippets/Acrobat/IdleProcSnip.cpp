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

 IdleProcSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* prototype for the actual idelProc function */
ACCB1 void ACCB2 idleProc (void* clientData);
/* some static data that allows me to keep track of time...*/
static int currenttime=0;

/** Each time this snippet is called it toggles between registering and deregistering the defined idle
	procedure. The idle procedure loosly tracks time. It dumps a counter to DebugWindow, the counter is updated every
	time the idle procedure is invoked. The period between invokations is passed in through the paramater dialog, specified
	in 1/60ths of a second.
	@see AVAppUnregisterIdleProc
*/
void IdleProcSnip(ParamManager * thePM){
	ASInt32 delay=0;

	if (SnippetRunnerUtils::toggleSnippetCheck()==false){
		if (ASBoolToBool(thePM->getNextParamAsInt(delay))!= true){
			AVAlertNote("Enter an integer to specify the delay between idle procedure calls.");
		}
		AVAppRegisterIdleProc (idleProc,NULL, delay);	
		SnippetRunnerUtils::turnParamDialogOff();
		AVAlertNote("Started idle procedure, see the console for output.");
	}
	else{
		AVAppUnregisterIdleProc (idleProc,NULL);
		SnippetRunnerUtils::turnParamDialogOn();
		AVAlertNote("Stopped idle procedure.");
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(IdleProcSnip, "Callback:Application:Example Idle Proc","Toggles registration of an idle proc which counts \nevery (n) 60ths of a second. 600 provides a delay \nof 10 seconds.\
 See console for output.","600")

ACCB1 void ACCB2 idleProc(void* clientData){
	char strMsg[256];
	sprintf(strMsg, "IdleProcSnip:: Clicks now: %d",  currenttime++);
	AVAlertNote(strMsg); 
}
