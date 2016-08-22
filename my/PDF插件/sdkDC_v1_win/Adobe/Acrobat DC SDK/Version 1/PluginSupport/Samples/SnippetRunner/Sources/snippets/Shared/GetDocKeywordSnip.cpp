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

 GetDocKeywordSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** example of how to extract the keywords from a document's info record
	@testfile TwoColumnTaggedDoc.pdf
	@requires open pdf, likes one with keywords
	@see PDDocGetInfo
	@see AVDocGetPDDoc
	@see AVAppGetActiveDoc
*/
void GetDocKeywordSnip(ParamManager * thePM){
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}
	string infoRequired;
	if (ASBoolToBool(thePM->getNextParamAsString(infoRequired))!= true){
		AVAlertNote("Enter a string key for the information directory");
	}
	else {
		ASInt32 buffSize = PDDocGetInfo(pdDoc, infoRequired.c_str(), NULL,0);
		if (buffSize == 0) {
			AVAlertNote("No information with that tag with this document.");
		}
		else {
			char * buffer = static_cast<char *>(ASmalloc(buffSize+1));
			buffer[0]='\0';
			PDDocGetInfo(pdDoc, infoRequired.c_str(), buffer, buffSize+1);
			Console::displayString(buffer);
			ASfree(buffer);
		}
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(GetDocKeywordSnip, "PD Layer:Document:Doc Keyword Info","Extracts document information for the frontmost \ndocument. \
Enter a single dictionary key from \nthe document's information dictionary. See the \nCommon SnippetRunner Interface for the \
content \nof the dictionary (for the specified key).","Title")

