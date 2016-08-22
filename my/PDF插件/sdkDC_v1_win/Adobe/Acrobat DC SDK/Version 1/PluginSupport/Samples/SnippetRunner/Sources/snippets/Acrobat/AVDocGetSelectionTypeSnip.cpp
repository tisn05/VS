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

 AVDocGetSelectionTypeSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** Obtains and displays the type of the current selection. Pops an alert if the selection type is not known, or if there is
	no active selection.
	
	@testfile TwoColumnTaggedDoc.pdf
	@requires pdf file with selectable object (text, image, ...)
	@see AVDocGetSelectionType
	@see AVAppGetActiveDoc
*/
void AVDocGetSelectionTypeSnip(void){
	CDocument document;
	AVDoc avDoc = (AVDoc)document;
	if (avDoc == NULL){
		return;
	}
	else
	{
		ASAtom theSelectionType = AVDocGetSelectionType(avDoc);
		if (theSelectionType == ASAtomNull){
			AVAlertNote("No active selection");
		}
		else {
			const char * buff = ASAtomGetString(theSelectionType);
			if (buff == NULL){
				Console::displayString("Selection type not defined\n");
			}
			else {
				Console::displayString("Selection type is: ");
				Console::displayString(buff);
				Console::displayString("\n");
			}
		}
	}
}

SNIPRUN_REGISTER(AVDocGetSelectionTypeSnip, "AV Layer:Document:Get Selection Type", "Reports the type of the active selection")


