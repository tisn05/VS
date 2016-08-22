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

 PDOCGChangeLockedStateSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* prototype for the actual callback function */
static ACCB1 ASBool ACCB2 OCGChangeLockedStateCallback(PDOCG theOCG, void * clientData);


/** registers the optional content group callback proc with the application. The callback 
	toggles the locked state of each OCG. 

	@testfile OCGSample1.pdf
	@requires pdf with OCGs
	@see PDOCGGetLocked
	@see PDOCGSetLocked
*/
void PDOCGChangeLockedStateSnip(){
	CDocument document;
	PDDoc theDoc = (PDDoc)document;
	if (theDoc!=NULL){
		// ennumerate the OCGs
		PDOCGEnumProc cbOCGs = ASCallbackCreateProto(PDOCGEnumProc, OCGChangeLockedStateCallback);
		PDDocEnumOCGs(theDoc, cbOCGs, NULL);
		ASCallbackDestroy( (void*) cbOCGs );
	}
}

SNIPRUN_REGISTER(PDOCGChangeLockedStateSnip, 
				 "PD Layer:Optional Content:Manipulate:Change OCG Locked State",
				 "Toggles the locked state of each optional content group within the \
document")

/* called once for each OCG (in this case in the document, although it could also be 
   page based) */
ACCB1 ASBool ACCB2 OCGChangeLockedStateCallback(PDOCG theOCG, void *)
{
	// make sure the document is still valid
	CDocument document;
	PDDoc frontDoc = (PDDoc)document;
	if (frontDoc==NULL){
		return false;
	}
	
	/* Toggles the locked state of the OCG in the built-in default optional-content configuration 
       for the document */
	PDOCConfig defaultDocConfig = PDDocGetOCConfig(frontDoc);
	
	ASBool bOrgLockedState = PDOCGGetLocked(theOCG, defaultDocConfig);
	PDOCGSetLocked(theOCG, defaultDocConfig, !bOrgLockedState);
	
	Console::displayString("OCG locked state toggled");
	
	return true;
}


