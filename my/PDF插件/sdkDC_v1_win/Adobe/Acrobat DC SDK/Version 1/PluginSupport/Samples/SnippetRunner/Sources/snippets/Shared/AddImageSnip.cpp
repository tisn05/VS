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

 AddImageSnip.cpp

 - The AddImage snippet creates an image XObject resource from a JPEG
   file and adds it to the displayed page. A JPEG sample file is in
   the sample file folder.

 - Performing this operation involves:

 	1. Configuring the image properties such width, height, etc.
 	2. Configuring the transformation matrix, which controls how the
 	   image is rendered.
 	3. Creating the image resource from the contents of the JPEG file
 	   then adding it to the contents of the page.

 - Uncomment the #define TRANSPARENT_IMAGE line to make the image
   semi-transparent.

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

const char *IMG_PATH = "AddImage.jpg";
const ASInt32 IMG_WIDTH = 297;
const ASInt32 IMG_HEIGHT = 445;

#define TRANSPARENT_IMAGE 1

// Utility function
static void SetElementOpacity (PDEElement pdeElement, PDDoc pdDoc);

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/* AddImage
** ------------------------------------------------------
**
** Inserts a JPEG file into the current page of the
** active document.
*/

/** Inserts a JPEG file into the current page of the active document. 

	@testfile blank.pdf, AddImage.jpg
	@requires pdf file and an image to add to it
	
	@see ASFileStmRdOpen
	@see PDEColorSpaceCreateFromName
	@see PDEImageCreate
	@see PDPageAcquirePDEContent
	@see SetElementOpacity
	@see PDEContentAddElem
	@see PDPageSetPDEContent
	@see PDPageReleasePDEContent
	@see PDPageNotifyContentsDidChangeEx
	@see PDPageRelease
	@see PDERelease
	@see ASStmClose
	@see ASFileClose
	@see PDEExtGStateCreateNew
	@see PDDocGetCosDoc
	@see PDEExtGStateSetOpacityFill
	@see PDEElementSetGState
*/
void AddImageSnip()
{
	ASInt32 iErrorCode = 0;

	ASFile volatile asFile = NULL;
	ASStm volatile asStm = NULL;

	DURING
		asFile = SnippetRunnerUtils::openSnippetExampleASFile(IMG_PATH);
		if(!asFile)
		{
			AVAlertNote("Can't find image");
			E_RTRN_VOID;
			
		}

		// Create a seekable read-only stream from the file.
		asStm = ASFileStmRdOpen (asFile, 0);
	HANDLER
		iErrorCode = ASGetExceptionErrorCode();
	END_HANDLER

	if (0 == iErrorCode) {

		PDEImage volatile pdeImage = NULL;
		PDEImageAttrs pdeImageAttrs;
		PDEColorSpace pdeColorSpace;
		ASFixedMatrix imageMatrix;

		// Configure the image attributes. These are specific to the sample file
		// provided. Ideally we would dynamically extract this information from
		// the file that is being processed.

		memset(&pdeImageAttrs, 0, sizeof(PDEImageAttrs));
		pdeImageAttrs.flags = kPDEImageExternal;
		pdeImageAttrs.width = IMG_WIDTH;
		pdeImageAttrs.height = IMG_HEIGHT;
		pdeImageAttrs.bitsPerComponent = 8;

		// The a, b, c, and d members of the matrix can be used to apply the
		// scaling, rotation, and skew to the image. The h and v members control
		// the positioning of the image on the page.
		//
		// Below we scale the image to 100% and render it at the bottom left of the page.

		imageMatrix.a = ASInt16ToFixed(IMG_WIDTH);
		imageMatrix.d = ASInt16ToFixed(IMG_HEIGHT);
		imageMatrix.b = imageMatrix.c = fixedZero;
		imageMatrix.h = fixedTwoSeventy;
		imageMatrix.v = fixedSeventyTwo;

		// Create one of the standard color spaces. We assume that the image has
		// three color components.
		pdeColorSpace = PDEColorSpaceCreateFromName(ASAtomFromString("DeviceRGB"));

		// We need to pass a PDEFilterArray to let the viewer know the filter that
		// was used to compress the image data. The JPEG format uses DCT encoding.
		//
		// See the PDF specification and the PostScript Reference manual for
		// details of the other supported filter types and the strucure of the
		// encode and decode parameters.

		PDEFilterArray filterArray;
		PDEFilterSpec filterSpec;

		memset (&filterSpec, 0, sizeof (PDEFilterSpec));
		filterSpec.name = ASAtomFromString ("DCTDecode");
		filterArray.numFilters = 1;
		filterArray.spec[0] = filterSpec;

		DURING
			// Create the image XObject.
			pdeImage = PDEImageCreate (&pdeImageAttrs, sizeof(pdeImageAttrs), &imageMatrix,
										kPDEImageEncodedData, pdeColorSpace, NULL, &filterArray,
										asStm, NULL, ASFileGetEOF(asFile));
		HANDLER
			iErrorCode = ASGetExceptionErrorCode();
		END_HANDLER

		if (0 == iErrorCode) {

			PDPage pdPage = NULL;
			PDEContent pdeContent = NULL;

			// Acquire the PDEContent of the page associated with the active view.
			CDocument document;
			pdPage = (PDPage)document;
			if(pdPage == NULL)
				return;

			pdeContent = PDPageAcquirePDEContent (pdPage, gExtensionID);

			DURING

#ifdef TRANSPARENT_IMAGE
				// Make the element semi-transparent.
				SetElementOpacity ((PDEElement)pdeImage, (PDDoc)document);
#endif

				// Add the PDEImage into the page content.
				PDEContentAddElem (pdeContent, kPDEAfterLast, (PDEElement)pdeImage);
			HANDLER
				iErrorCode = ASGetExceptionErrorCode();
			END_HANDLER

			// Commit the changes we made to the page contents.
			PDPageSetPDEContent (pdPage, gExtensionID);
			PDPageReleasePDEContent (pdPage, gExtensionID);

			// Request that the viewer update the page display then release
			// the resources we acquired.
			PDPageNotifyContentsDidChangeEx (pdPage, true);
			//page gets released by CDocument
			//PDPageRelease (pdPage);
			
			Console::displayString("Added image to document\n");
		}

		PDERelease ((PDEObject)pdeColorSpace);
		PDERelease ((PDEObject)pdeImage);
	}

	// Close the stream and file we opened.
	if (asStm)
		ASStmClose(asStm);
	if (asFile)
		ASFileClose(asFile);

	// If an error occurred along the way, we really should tell the user. They
	// probably won't be able to infer much from just the following message...

	if (0 != iErrorCode) {

		const ASInt32 BUF_SIZE = 256;
		char errorBuf[BUF_SIZE];

		AVAlertNote (ASGetErrorString(iErrorCode, errorBuf, BUF_SIZE));
	}
}


/* SetElementOpacity
** ------------------------------------------------------
**
** Configure the ExtGState associated with the image
** element to a pre-defined opacity value.
*/
static void SetElementOpacity (PDEElement pdeElement, PDDoc pdDoc)
{
	PDEGraphicState gState;
	PDEExtGState pdeExtGState = NULL;

	DURING
		// Opacity is a property of the extended graphics state.
		pdeExtGState = PDEExtGStateCreateNew (PDDocGetCosDoc(pdDoc));
		PDEExtGStateSetOpacityFill(pdeExtGState, FloatToASFixed(0.4));

		// Wrap the ExtGState object in a GState object and apply it to the element.
		memset (&gState, 0, sizeof(PDEGraphicState));
		gState.extGState = pdeExtGState;
		gState.wasSetFlags = kPDEExtGStateWasSet;

		PDEElementSetGState((PDEElement)pdeElement,  &gState, sizeof(PDEGraphicState));
	HANDLER
		if(pdeExtGState)
			PDERelease((PDEObject) pdeExtGState);
		return;
	END_HANDLER
}

SNIPRUN_REGISTER(AddImageSnip, "PD Layer:Document:Add Image", "Adds a JPEG image to the current page.")


