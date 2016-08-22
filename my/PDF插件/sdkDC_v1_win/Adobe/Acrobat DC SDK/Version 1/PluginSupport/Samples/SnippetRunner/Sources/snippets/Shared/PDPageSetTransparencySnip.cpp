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

 PDPageSetTransparencySnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/
// function prototype
bool SetPageElementTransparency( PDPage pdPage,			// [in] PDPage
						   PDEElement pdeElement,		// [in] NULL for first call
						   ASFixed fixedOpac);			// [in] opacity, fixed number from float [0 - 1]


/** The snippet will change or try to create transparency for the elements of 
  the first page content in the current document.
	If there is any PDE element for which we cannot set transparency, we 
  ignore the setting and show a message in the debug window. 
	The snippet can be used to set transparency of page text content in most cases.
	Input opacity value of zero will hide the elements.

	@testfile threepies.pdf
	@requires pdf with page content
	@see PDDocAcquirePage
	@see PDPageNotifyContentsDidChange
	@see ASAtomFromString
	@see PDPageAcquirePDEContent
	@see PDEContainerGetContent
	@see PDEContentGetNumElems
	@see PDEContentGetElem
	@see PDEElementHasGState
	@see PDEExtGStateSetOpacityFill
	@see PDEExtGStateSetOpacityStroke
	@see PDEObjectGetType
	@see PDEContainerGetContent
	@see PDPageSetPDEContent
	@see PDPageReleasePDEContent
	@see PDPageReleasePDEContent
*/
void PDPageSetTransparencySnip(ParamManager * thePM)
{
	// Get the front doc 
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc==NULL) return;

	// get user input of opacity 
	ASFixed fixedOpacity;
	if (ASBoolToBool(thePM->getNextParamAsFixed(fixedOpacity)) != true)
		return;
	if (ASFixedToFloat(fixedOpacity) < 0.0 || ASFixedToFloat(fixedOpacity) > 1.0){
		AVAlertNote ("Enter a valid value from 0 to 1.");
		return;
	}

DURING

	// acquire the first page
	PDPage gPdPage = PDDocAcquirePage(pdDoc, 0);
			
	// go through all elements to set text transparency
	int iElemNumCount = 0;
	SetPageElementTransparency(gPdPage,	NULL, fixedOpacity);

	// notify changes
	PDPageNotifyContentsDidChange (gPdPage);
	PDPageRelease(gPdPage);

HANDLER

	char buf[256];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, 255));

END_HANDLER
}


/* SetPageElementTransparency
** ---------------------------------------------------------------------
** internal function to set Page Element Transparency
**
*/
bool SetPageElementTransparency(PDPage pdPage,			// [in] PDPage
						   PDEElement pdeElement,		// [in] NULL for first call
						   ASFixed fixedOpac)			// [in] opacity, fixed number from float [0 - 1]
{

	// find the page content
	PDEElement pdeElement1;
	PDEContent pdeContent, pdeContent1;
	PDEGraphicState gstate;
	ASFixed  opacityFill, opacityStroke;
	PDDoc pdDoc = PDPageGetDoc (pdPage);

	// set opacity values to use
	opacityFill = opacityStroke = fixedOpac;

	if(!pdeElement) {
		// get page content
		pdeContent = PDPageAcquirePDEContent(pdPage, gExtensionID);
	}
	else {
		// get element content
		pdeContent = PDEContainerGetContent ((PDEContainer) pdeElement);
	}

	// get unmber of elements
	ASInt32 numElems = PDEContentGetNumElems(pdeContent);
	int NumElemsFound = 0;

	// loop through elements from the first to a maximum number.
	for (int i = 0; i < numElems; i++) {

DURING
		// get element
		pdeElement1 = PDEContentGetElem(pdeContent, i);

		// if text element, we need to work on its text runs
		if (PDEObjectGetType((PDEObject) pdeElement1)==kPDEText){
			PDEText textObject = (PDEText)pdeElement1;
			ASInt32 numTextRuns = PDETextGetNumRuns(textObject);

			// for each text run...
			for (int t = 0;t<numTextRuns;t++){

				// get its current graphics state
				PDETextGetGState(textObject, kPDETextRun,t,&gstate, sizeof(PDEGraphicState));	
			
				// if there is extGState, we set its transparency.
				if (gstate.extGState) {
					// set opacity
					PDEExtGStateSetOpacityFill(gstate.extGState, opacityFill);
					PDEExtGStateSetOpacityStroke(gstate.extGState, opacityStroke);
				}
				// if there is no extGState, we create new one and set its transparency.
				else {
					PDEExtGState pdeExtGState = PDEExtGStateCreateNew (PDDocGetCosDoc(pdDoc));
					PDEExtGStateSetOpacityFill(gstate.extGState, opacityFill);
					PDEExtGStateSetOpacityStroke(gstate.extGState, opacityStroke);

					// Wrap the ExtGState object in a GState object and apply it to the element.
					gstate.extGState = pdeExtGState;
					gstate.wasSetFlags = kPDEExtGStateWasSet;
					PDEElementSetGState((PDEElement)pdeElement1,  &gstate, sizeof(PDEGraphicState));
				}
			}
		}

		// besides text element, we only work on the element which we can get the Graphic State for
		else if(PDEElementHasGState (pdeElement1, &gstate, sizeof(PDEGraphicState))) {

			// if there is extGState, we set its transparency.
			if (gstate.extGState) {
				// set opacity
				PDEExtGStateSetOpacityFill(gstate.extGState, opacityFill);
				PDEExtGStateSetOpacityStroke(gstate.extGState, opacityStroke);
			}
			// if there is no extGState, we create new one and set its transparency.
			else {
				PDEExtGState pdeExtGState = PDEExtGStateCreateNew (PDDocGetCosDoc(pdDoc));
				PDEExtGStateSetOpacityFill(gstate.extGState, opacityFill);
				PDEExtGStateSetOpacityStroke(gstate.extGState, opacityStroke);

				// Wrap the ExtGState object in a GState object and apply it to the element.
				gstate.extGState = pdeExtGState;
				gstate.wasSetFlags = kPDEExtGStateWasSet;
				PDEElementSetGState((PDEElement)pdeElement1,  &gstate, sizeof(PDEGraphicState));
			}
	
		}

HANDLER

	char buf[256];
	ASGetErrorString(ERRORCODE, buf, 255);
	AVAlertNote(buf);
	AVAlertNote("Cannot set transparency for this element.");

END_HANDLER

		//  since a container is a group of elements,
		//  go down one level to travel through elements under containers.
		if (PDEObjectGetType((PDEObject) pdeElement1) == kPDEContainer){

			// get element
			pdeContent1 = PDEContainerGetContent ((PDEContainer) pdeElement1);

			// recuring
			SetPageElementTransparency(pdPage, pdeElement1, fixedOpac);
		}
	}

DURING

	// set page content back
	PDPageSetPDEContent(pdPage, gExtensionID);

	// release page content
	PDPageReleasePDEContent(pdPage, gExtensionID);

	E_RETURN(true);

HANDLER

	char buf[256];
	ASGetErrorString(ERRORCODE, buf, 255);
	AVAlertNote(buf);
	
	// release page content
	PDPageReleasePDEContent(pdPage, gExtensionID);

	return(false);

END_HANDLER

return true;

}

SNIPRUN_REGISTER_WITH_DIALOG(PDPageSetTransparencySnip, "PDE Layer:Element:Set transparency",
		"Set transparency of the page content in the front document.  Input an opacity value from 0 <invisible> to 1<opaque>.","0.3")

