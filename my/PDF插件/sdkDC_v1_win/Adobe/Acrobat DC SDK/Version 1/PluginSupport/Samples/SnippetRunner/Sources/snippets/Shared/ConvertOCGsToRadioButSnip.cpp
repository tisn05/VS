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

 ConvertOCGsToRadioButSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** an example of how to move all OCGs in a document to being a set of radio group OCGs.
	This is a pretty simple snippet, just taking all OCGs defined in the document's 
	default OC configuration (from the OCPProperties dictionary) and setting them in the RBGroups
	of the same configuration dictionary. This means only OCGs that are present in the UI are converted
	to the radio button relationship. This snippet does not respect any pre-existing RBrelationship between OCGs.
	
	@testfile OCGSample1.pdf
	@requires pdf with OCG's
	@see PDDocGetOCConfig
	@see PDOCConfigGetOCGOrder
	@see PDOCGGetFromCosObj
	@see PDOCConfigMakeRadioButtonGroup
*/
void ConvertOCGsToRadioButSnip(void){
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}

	// Indicate it should initially be turned on (visible)
	PDOCConfig docConfig = PDDocGetOCConfig (pdDoc);

	// we want it to be in the layers UI in acrobat, so we need to indicate where the label should
	// be placed (by specifying the order in the document's OCProperties dictionary)
	
	// get existing OCGs
	CosObj existingOCGs;

	// we use the ordering of the OCG to get the list of OCGs in the layer UI
	PDOCConfigGetOCGOrder(docConfig,&existingOCGs);
	if (CosObjGetType(existingOCGs) != CosArray){
		AVAlertNote("Error in PDF document, OCProperties has invalid Order array");
		return;
	}

	// count the number of OCGs in the cos array
	ASInt32 cnt = CosArrayLength(existingOCGs);
	PDOCG * theOCGArray = static_cast<PDOCG *>(ASmalloc((cnt + 1) * sizeof(PDOCG)));
	ASInt32 loop = 0;
	for ( ; loop<cnt; loop++){
		theOCGArray[loop] = PDOCGGetFromCosObj(CosArrayGet(existingOCGs, loop));
	}
	theOCGArray[loop] = NULL;
	PDOCConfigMakeRadioButtonGroup(docConfig,theOCGArray);
	ASfree(theOCGArray);

	// 
	Console::displayString("All OCGs in the document have been converted to a set of radio group OCGs. \
		Please use Layers Panel functions in Acrobat to test.");
}

SNIPRUN_REGISTER(ConvertOCGsToRadioButSnip, "PD Layer:Optional Content:Manipulate:Convert to Radio Buttons","Converts\
 the OCGs in the UI into a radio button group (exclusive-or relationship)")

