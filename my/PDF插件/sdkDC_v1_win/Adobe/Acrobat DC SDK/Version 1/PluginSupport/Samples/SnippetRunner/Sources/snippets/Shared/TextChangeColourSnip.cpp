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

 TextChangeColourSnip.cpp

*********************************************************************/


#include "SnippetRunner.h"
#include "ParamManager.h"
#include "SnippetRunnerUtils.h"

/** Will change the (PDE)text object of the first page of the front document to the RGB values specified through the paramater dialog.
	To change the colour of text, we iterate through all text runs for text objects found on the first page, setting their graphic state
	according to the r.g.b. values passed in. After manipulating the state, we commit the changes to the pdeContent.
	@testfile TextImageForm.pdf
	@requires pdf with text
	@see PDEContentGetElem
	@see PDEObjectGetType
	@see PDETextGetNumRuns
	@see PDETextGetGState
	@see PDEColorSpaceCreateFromName
	@see PDEColorSpaceGetNumComps
	@see PDETextRunSetGState
*/
void TextChangeColourSnip(ParamManager * thePM){

	// get the content from the first page
	CDocument document;
	PDPage theFirstPage = (PDPage)document; //CDocument releases page
	if (theFirstPage == NULL){
		return;
	}
	PDEContent pdeContent = PDPageAcquirePDEContent(theFirstPage,0);
	ASInt32 numElems = PDEContentGetNumElems(pdeContent);

	ASInt32 red,green,blue;
	thePM->getNextParamAsInt(red);
	thePM->getNextParamAsInt(green);
	thePM->getNextParamAsInt(blue);

	// for each element found on the first page...
	for (ASInt32 t = 0; t < numElems; t++){
		DURING
			PDEElement pdeElement = PDEContentGetElem(pdeContent,t);
			if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEText){
				// if text, get number of text runs
				PDEText textObject = (PDEText)pdeElement;

				ASInt32 numTextRuns = PDETextGetNumRuns(textObject);

				// for each text run...
				for (int t = 0;t<numTextRuns;t++){
						PDEGraphicState stateOfRun;
						
						// get its current graphics state
						PDETextGetGState(textObject, kPDETextRun,t,&stateOfRun, sizeof(PDEGraphicState));	
			
						// create a new colour space, and the colour value we require
						PDEColorSpace colourSpace=PDEColorSpaceCreateFromName(ASAtomFromString("DeviceRGB"));
				
						PDEColorValue colourValue;
						colourValue.color[0]=red;
						colourValue.color[1]=green;
						colourValue.color[2]=blue;
						
						colourValue.colorObj =NULL;
						colourValue.colorObj2 =NULL;

						// create a colour spec based on the colour space and values defined above
						PDEColorSpec colourSpec;
						colourSpec.space = colourSpace;
						colourSpec.value = colourValue;

						// assign the colour spec to the graphic state
						stateOfRun.strokeColorSpec = stateOfRun.fillColorSpec = colourSpec;

						// set the text run to the new graphic state
						PDETextRunSetGState(textObject, t,&stateOfRun, sizeof(stateOfRun));
				}
			}
		// there are a couple of calls that can raise an exception. 
		// I clear the transients and reraise to the backstop exception handler.
		HANDLER
		
			PDPageReleasePDEContent(theFirstPage, gExtensionID);
			ASRaise(ERRORCODE); // RERAISE();	
		END_HANDLER
	}

	// Commit the changes that we made to the pdeContent.
	PDPageSetPDEContent(theFirstPage, gExtensionID);
	PDPageNotifyContentsDidChange(theFirstPage);

	// delete transients
	PDPageReleasePDEContent(theFirstPage, gExtensionID);
}

SNIPRUN_REGISTER_WITH_DIALOG(TextChangeColourSnip, "PDE Layer:Text:Change Text Colour",
		"Shows how to change the colour of text in a document. \n\
Parameters: R G B (in ASInt32) \n\
Default: 0 0 65000","0 0 65000")
