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

 AVEnumActionHandlerSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* the proc called for every registered action handler */
ACCB1 ASBool ACCB2 myAVActionEnumProc(ASAtom type,char* userName, void* clientData);

/** Obtains type/name information about all registered action handlers

	@testfile none
	@see AVAppEnumActionHandlers
	@see AVActionEnumProc
	@see AVActionHandlerProcs
	@see AVActionPerformProc
	@see AVActionDoPropertiesProc
	@see AVActionFillActionDictProc
	@see AVActionGetInstructionsProc
	@see AVActionGetbuttonTextProc
	@see AVActionGetStringOneProc
	@see AVActionGetStringTwoProc
	@see AVActionCopyProc
	@see AVActionPerformExProc
	@see AVActionDoPropertiesExProc
*/

int actionHandlerCount;
void AVEnumActionHandlerSnip(void){
	actionHandlerCount = 0;
	Console::displayString("Installed Action Handlers:");
	AVAppEnumActionHandlers(myAVActionEnumProc,NULL);
}

SNIPRUN_REGISTER(AVEnumActionHandlerSnip, "AV Layer:Action Handler:Enum Type/Name info", "Reports the types and (ui) names of all registered action handlers.\
This snippet can be made to expose the AVActionHandlerProcs structure. This is useful for any developer wanting to understand how to invoke or interact with a specific\
action handler - it shows which functions are valid for each action handler.")

ACCB1 ASBool ACCB2 myAVActionEnumProc(ASAtom type,char* userName, void* clientData){
	actionHandlerCount++;
	char buff[1000];
	sprintf(buff, "%d) %s Name: %s", actionHandlerCount,  ASAtomGetString(type), userName);
	Console::displayString(buff);

	return true;
}


