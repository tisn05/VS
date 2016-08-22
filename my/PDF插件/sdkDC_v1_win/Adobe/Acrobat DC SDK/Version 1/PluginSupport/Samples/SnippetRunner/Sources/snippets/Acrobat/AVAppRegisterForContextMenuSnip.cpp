/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 AVAppRegisterForContextMenuSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/


ACCB1 void ACCB2 myAVContentMenuAdditionProc(ASAtom menuName, AVMenu menu,
											void* menuData, void* clientData)
{
	char buffer[100];
	sprintf(buffer, "You can modify the %s context menu here.", (char*) clientData);
	AVAlertNote(buffer);
}

const char* pageContext = "Page";
const char*  selectionContext = "Select";
/**  
	Registers for either the Page or Selection context menus. There exist some
	context menus that are not controlled by either of these choices.

	@see AVAppRegisterForContextMenuAddition
*/
void AVAppRegisterForContextMenuSnip(ParamManager * thePM)
{
	ASInt32 type=0;
	thePM->getNextParamAsInt(type);

	if (type == 1)
		AVAppRegisterForContextMenuAddition( ASAtomFromString("Page"), myAVContentMenuAdditionProc, (void*)pageContext);
	else
		AVAppRegisterForContextMenuAddition( ASAtomFromString("Select"), myAVContentMenuAdditionProc, (void*)selectionContext);
}

SNIPRUN_REGISTER_WITH_DIALOG(AVAppRegisterForContextMenuSnip, "Callback:Application:Context Menu", "Register for either Page(1) or Selection(2) Context Menus. There is no unregistration.", "1")

