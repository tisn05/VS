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

 CloseFrontDocSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** Closes the front document. If there is no document open, an alert is displayed. If the document is not closed
	(the user hits cancel when asked if they want to save the modified document), an alert is displayed.\
	
	@testfile blank.pdf
	@requires an open doc
	@see AVDocClose
	@see AVAppGetActiveDoc
*/
void CloseFrontDocSnip(void){
	CDocument document;
	AVDoc avDoc = (AVDoc)document;
	if (avDoc == NULL){
		return;
	}
	else
	{
		// attempt to close the document, prompt the user if a save is required
		if(AVDocClose(avDoc, false)==false) {
			AVAlertNote("Document close cancelled");
		}
	}
	Console::displayString("Document closed");
}

SNIPRUN_REGISTER(CloseFrontDocSnip, "AV Layer:Document:Close Front Document", "Closes the front document, prompting for a save if required")



