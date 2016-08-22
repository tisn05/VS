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

 PDEPathToggleVisibilitySnip.cpp

*********************************************************************/

/* 
**
** This snippet toggles visibility of PDEPath objects on the first page 
** of current open document by modifying the PDEPath marking attributes.
**
** The snippet utilize a global boolean value 'switchOn' as a flag to
** "log" the logical operation to be performed next. The "dirty" attribute 
** of a document is also used in combination for this purpose.
**
**
	@testfile ShapeTest.pdf
	@requires open pdf with path content
** @see PDEPageAcquirePDEContent
** @see PDEContentGetElem
** @see PDEContentGetNumElems
** @see PDPageSetPDEContentCanRaise
** @see PDPageReleasePDEContent
** @see PDERelease
** @see PDPageRelease
** @see PDDocGetFlags
** @see PDDocClearFlags
** @see PDEPathGetPaintOp
** @see PDEPathSetPaintOp
** @see PDEElementGetGState
** @see PDEElementSetGState
** @see PDEObjectGetType
**
*/

#include <vector>
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static ASBool switchOn;
static void EnumeratePDEElems(PDEElement pdeElem);
static void ToggleVisibility(PDEElement);
static void ShowErrMsg(char*);
static void CleanUpMemory();

typedef struct {
	ASUns32 paintOp;
	PDEGraphicState gState;
	ASBool hasGState;
} GraphicsProp;

static vector <GraphicsProp> elemGProps;
static int elemIndex = 0, restoreIndex = 0;

void PDEPathToggleVisibilitySnip()
{
	CDocument document;
	PDPage pdPage = (PDPage)document; //CDocument releases page
	if (pdPage == NULL) return;
	
 	PDDoc pdDoc = PDPageGetDoc(pdPage);
	ASInt32 docFlags = PDDocGetFlags(pdDoc);
	
	if (!(docFlags & PDDocNeedsSave))
	{
		if (switchOn) {
			ShowErrMsg("You have switched off PDEPath objects visibility of another document. You should save the change(s) or close the document to revert the change(s) made before working on another document.\n Operation aborted.\n");
			switchOn = false;
			CleanUpMemory();
			return;
		}
	}
	
	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, 0);
	
	PDEElement pdeElem = NULL;

DURING
	ASInt32 numElems = PDEContentGetNumElems(pdeContent);

	for (int i=0; i<numElems; i++)
	{
		pdeElem = PDEContentGetElem(pdeContent, i);
		EnumeratePDEElems(pdeElem);
	}

	if (switchOn) {
		elemIndex = 0;
		CleanUpMemory();
		PDDocClearFlags(pdDoc, PDDocNeedsSave);
	}

	switchOn = !switchOn;
	PDPageSetPDEContentCanRaise(pdPage, 0);
	PDPageNotifyContentsDidChange(pdPage);
HANDLER
	char buf[256];
	ASInt32 err = ERRORCODE;
	ASGetErrorString(err, buf, 256);
	ShowErrMsg(buf);
	ShowErrMsg("\n");
END_HANDLER
	PDPageReleasePDEContent(pdPage, 0);
}

static void EnumeratePDEElems(PDEElement pdeElem)
{
	if (PDEObjectGetType((PDEObject)pdeElem) == kPDEPath)
	{
		ToggleVisibility(pdeElem);
	} else {
		if (PDEObjectGetType((PDEObject)pdeElem) == kPDEContainer) 
		{
			PDEContent pdeContent = PDEContainerGetContent((PDEContainer)pdeElem);
			PDEElement elem;

			ASInt32 numElems = PDEContentGetNumElems(pdeContent);
			for (int i=0; i<numElems; i++) 
			{
				// Recursively enumerates PDEPath object contained in PDEContainer objects.
				elem = PDEContentGetElem(pdeContent, i);
				EnumeratePDEElems(elem);
			}
		}
	} 
}

static void ToggleVisibility(PDEElement pdeElem)
{
	GraphicsProp gp;
	
	if (switchOn)
	{			
		gp = elemGProps.at(restoreIndex++);
		if(gp.hasGState) {
			if (gp.gState.extGState == NULL)
				PDEElementSetGState(pdeElem, &(gp.gState), sizeof(PDEGraphicState));
			else {				
				// For PDEGraphicState object that has the ExtGState subdictionary
				// (represented as a PDEExtGState object), turining off the
				// visibility invalidates the PDEExtGState object so we use
				// a default PDEGraphicState object instead to avoid an exception
				// upon the call to PDEElementSetGState
				PDEGraphicState gs;
				PDEDefaultGState(&gs, sizeof(PDEGraphicState));
				gs.extGState = NULL;
				PDEElementSetGState(pdeElem, &gs, sizeof(PDEGraphicState));
			}
		}
		PDEPathSetPaintOp((PDEPath)pdeElem, gp.paintOp);
		elemIndex--;
		if (elemIndex <= 0)
			restoreIndex = 0;
	} else {
		gp.paintOp = PDEPathGetPaintOp((PDEPath)pdeElem);

		if (PDEElementHasGState(pdeElem, &(gp.gState), sizeof(PDEGraphicState)))
			gp.hasGState = true;
				
		elemGProps.push_back(gp);
		elemIndex++;

		PDEPathSetPaintOp((PDEPath)pdeElem, kPDEInvisible);			
	}
}

static void CleanUpMemory()
{
	if (!elemGProps.empty())
		elemGProps.clear();
}

static void ShowErrMsg(char* errMsg)
{
	Console::displayString(errMsg);
}


SNIPRUN_REGISTER(PDEPathToggleVisibilitySnip, "PDE Layer:Path:Toggle PDEPath Visibility", 
				 "Switching on/off PDEPath objects visibility")
