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

 PDOCGToggleIntentSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* prototype for the actual callback function */
static ACCB1 ASBool ACCB2 OCGLayerLockCallback(PDOCG theOCG, void * clientData);


/** Uses the optional content group enum function to enummerate all OCGs in the document toggling their state between
	"Design" and "View". In Acrobat, this has the effect of locking and un-locking the layers. This "locking" does 
	not alter the editability of the content a particular "layer" has, it only "locks" the visibility of the layer.

	With Acrobat 6 (and Adobe Reader 6), the layers panel does not automatically update to reflect the "locked" state
	of a layer, you need to save the document and re-load it before the ui is updated.
	@testfile OCGSample1.pdf
	@requires pdf with OCGs
	@see PDDocEnumOCGs
	@see PDOCGEnumProc
	@see PDOCGGetIntent
	@see PDOCGSetIntent
*/
void PDOCGToggleIntentSnip(){
	CDocument document;
	PDDoc theDoc = (PDDoc)document;
	if (theDoc!=NULL){
		// ennumerate the OCGs
		PDOCGEnumProc cbOCGs = ASCallbackCreateProto(PDOCGEnumProc, OCGLayerLockCallback);
		PDDocEnumOCGs(theDoc, cbOCGs, NULL);
		ASCallbackDestroy( (void*) cbOCGs );
		
		Console::displayString("OCGs Intent toggled");
	}
}

SNIPRUN_REGISTER(PDOCGToggleIntentSnip, "PD Layer:Optional Content:Manipulate:Lock layers","toggles the locking of layers")

/* called once for each OCG (in this case in the document, although it could also be page based) */
ACCB1 ASBool ACCB2 OCGLayerLockCallback(PDOCG theOCG, void *)
{

	ASAtom* thisIntent = PDOCGGetIntent(theOCG);

	const char* intentCstr = ASAtomGetString(*thisIntent);
	if (strcmp(intentCstr,"Design")==0) {
		ASAtom viewIntentArray[] = { ASAtomFromString("View"), ASAtomNull };
		PDOCGSetIntent(theOCG, viewIntentArray);
		Console::displayString("OCG toggled to View");
	}
	else {
		ASAtom designIntentArray[] = { ASAtomFromString("Design"), ASAtomNull };
		PDOCGSetIntent(theOCG, designIntentArray);
		Console::displayString("OCG toggled to Design");
	}
	return true;
}


