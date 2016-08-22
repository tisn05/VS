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

 ObjShiftSnip.cpp
 
*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static void ShiftObjects(PDEElement, ASFixedMatrix);

/* 
** ObjShiftSnip
**
** This snippet demonstrates translating page object positions with PDFEdit APIs.
**
** Objects on the first page of the current front-most open document are translated 
** in number of points (in User Space coordinates) according to the input parameters
** retrieved upon snippet execution. 
** 
** Default translation is 1" (72 points) upwards and 1" (72 points) to the right. 
** Use negative numbers to translate objects downwards and/or to the left.
**
	@testfile cmykText.pdf
	@requires pdf with objects on first page
** @see PDPageAcquirePDEContent
** @see PDEContentGetNumElems
** @see PDEContentGetElem
** @see PDEClipGetNumElems
** @see PDEClipGetElem
** @see PDEObjectGetType
** @see PDEFormGetContent
** @see PDEGroupGetContent
** @see PDEContainerGetContent
** @see PDETextGetNumRuns
** @see PDETextGetMatrix
** @see PDEElementGetMatrix
** @see PDETextRunSetMatrix
** @see PDEElementSetMatrix
** @see ASFixedMatrixConcat
** @see PDPageSetPDEContent
**
*/

void ObjShiftSnip(ParamManager* thePM)
{
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (!pdDoc) return;

	ASInt32	transX;
	ASInt32	transY;
	// Get the target translations along the X, Y axis
	thePM->getNextParamAsInt(transX);
	thePM->getNextParamAsInt(transY);

	ASFixedMatrix transMatrix; 		// The target translation matrix
	// Initialize the target transformation matrix
	transMatrix.a = fixedOne;
	transMatrix.b = fixedZero;
	transMatrix.c = fixedZero;
	transMatrix.d = fixedOne;
	transMatrix.h = ASInt32ToFixed(transX);
	transMatrix.v = ASInt32ToFixed(transY);

	PDPage pdPage = PDDocAcquirePage(pdDoc, 0); 
	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, 0);

	PDEElement pdeElement;
	ASInt32 numElems = PDEContentGetNumElems(pdeContent);

	for (ASInt32 i = 0; i < numElems; i++)
	{
		// Iterate through elements on the page
		pdeElement = PDEContentGetElem(pdeContent, i); 
		// Relocate the page objects
		ShiftObjects(pdeElement, transMatrix);
	}

	// Set the modified content back to the page
	PDPageSetPDEContent(pdPage, gExtensionID);
	PDPageNotifyContentsDidChange (pdPage);
	// Clean up
	PDPageReleasePDEContent(pdPage, gExtensionID);
	PDPageRelease (pdPage);
}

/*
** Helper function to perform PDEElement iteration, matrix concatenation, and
** setting the new matrix to objects
*/

static void ShiftObjects(PDEElement pdeElement, ASFixedMatrix xMatrix)
{
	ASFixedMatrix elemMatrix;
	ASFixedMatrix newMatrix;

	ASInt32 objType = PDEObjectGetType((PDEObject)pdeElement);
	switch (objType) 
	{
	/*
	 * Recursively call this helper function to relocate objects contained in 
	 * PDEForm, PDEClip, PDEGroup, and PDEContainer objects
	 */
	case kPDEForm:	
		{
			PDEContent theContent = PDEFormGetContent((PDEForm)pdeElement);
			ASInt32 numElems = PDEContentGetNumElems(theContent);
			PDEElement formElem;

			for (ASInt32 j=0; j<numElems; j++)
			{
				formElem = PDEContentGetElem(theContent, j);
				ShiftObjects(formElem, xMatrix);
			}
		}
		break;
	case kPDEClip:
		{
			ASInt32 numClips;
			PDEElement clipElem;

			numClips = PDEClipGetNumElems((PDEClip)pdeElement);
			for (ASInt32 j=0; j<numClips; j++)
			{
				clipElem = PDEClipGetElem((PDEClip)pdeElement, j);
				ShiftObjects(clipElem, xMatrix);
			}
		}
		break;
	case kPDEGroup:
		{
			PDEContent groupContent = PDEGroupGetContent((PDEGroup)pdeElement);
			ASInt32 numElems = PDEContentGetNumElems(groupContent);
			PDEElement theElem;

			for (int j=0; j<numElems; j++)
			{
				theElem = PDEContentGetElem(groupContent, j);
				ShiftObjects(theElem, xMatrix);
			}
		}
		break;
	case kPDEContainer:
		{
			PDEContent containerContent = PDEContainerGetContent((PDEContainer)pdeElement);
			ASInt32 numElems = PDEContentGetNumElems(containerContent);
			PDEElement theElem;

			for (ASInt32 j=0; j<numElems; j++)
			{
				theElem = PDEContentGetElem(containerContent, j);
				ShiftObjects(theElem, xMatrix);
			}
		}
		break;
	case kPDEText:
		{
			ASUns32 numRuns = PDETextGetNumRuns ((PDEText)pdeElement);

			for (ASUns32 j=0; j<numRuns; j++)
			{
				PDETextGetMatrix((PDEText)pdeElement, kPDETextRun, j, &elemMatrix);
				// Perform transformation to translate object position
				ASFixedMatrixConcat( &newMatrix, &xMatrix, &elemMatrix );
				// and we apply it
				PDETextRunSetMatrix((PDEText)pdeElement, j, &newMatrix);
			}
		}
		break;
	case kPDEPath:	// Perform the same matrix manipulation for both path and image objects
	case kPDEImage:
		PDEElementGetMatrix(pdeElement, &elemMatrix);
		ASFixedMatrixConcat( &newMatrix, &xMatrix, &elemMatrix );
		// Set new matrix
		PDEElementSetMatrix(pdeElement, &newMatrix);
		break;
	default:
		break;
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(
	ObjShiftSnip, 
	"PDE Layer:Translate Object Positions", 
	"Demonstrates how to relocate page objects with PDFEdit APIs. \
Input parameters: X-shift Y-shift (default: 72 72)\n\n\
Units are 1/72 of an inch in user-space coordinates.", "72 72")
