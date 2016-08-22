/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 CosStream64Snip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static const char * inFile = "AddImage.jpg";
static const char * outFile = "out.pdf";
static const int IMG_WIDTH = 297;
static const int IMG_HEIGHT = 445;

/**  
	This snippet Demonstrates 64-bit APIs for Cos Stream object construction and info access.
	We first open a JPEG image file as the input stream and create a Cos stream object
	from it. Then we create a PDEImage object out of the Cos stream representing the image
	data stream and add the PDEImage object to the page content. We then save the page to 
	the newly created document. 

	After the Cos stream is created, we also get the Cos stream length and data byte offset
	values with 64-bit Cos stream APIs.

	testfile AddImage.jpg
	@see ASFileSysCreatePathName
	@see ASFileSysOpenFile
	@see ASFileStmRdOpen
	@see CosNewNameFromString
	@see CosDictPutKeyString
	@see CosNewStream64
	@see CosStreamLength64
	@see CosStreamPos64
	@see PDDocCreatePage
	@see PDPageSetCropBox
	@see PDEImageCreateFromCosObj
*/
static void CosStream64Snip()
{
DURING
	
	//
	// Open the input file
	//

	ASFile asFile = SnippetRunnerUtils::openSnippetExampleASFile(inFile);

	//
	// Create a read-only, seek-able ASStm from the input file
	//

	ASStm inStm = ASFileStmRdOpen(asFile, 0);

	PDDoc pdDoc = PDDocCreate();
	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);

	//
	// Create the resource dictionary for the image XObject
	//

	CosObj imgDict = CosNewDict(cosDoc, false, 8);
	CosDictPutKeyString(imgDict, "Type", CosNewNameFromString(cosDoc, false, "XObject"));
	CosDictPutKeyString(imgDict, "Subtype", CosNewNameFromString(cosDoc, false, "Image"));
	CosDictPutKeyString(imgDict, "ColorSpace", CosNewNameFromString(cosDoc, false, "DeviceRGB"));
	CosDictPutKeyString(imgDict, "BitsPerComponent", CosNewInteger(cosDoc, false, 8)); 
	CosDictPutKeyString(imgDict, "Filter", CosNewNameFromString(cosDoc, false, "DCTDecode"));
	CosDictPutKeyString(imgDict, "Height", CosNewInteger(cosDoc, false, IMG_HEIGHT)); 
	CosDictPutKeyString(imgDict, "Width", CosNewInteger(cosDoc, false, IMG_WIDTH));

	//
	// Construct a Cos stream from the input image data stream
	//

	CosObj cosStm = CosNewStream64(
		cosDoc,				// cos document to use the newly created stream
		true,					// cos stream is indirect object?
		inStm,				// input stream
		0,						// start reading from stream position
		false,				// stream is decoded?
		imgDict,			// attributes dictionary
		CosNewNull(),	// encode params
		-1,						// decode length
		false					// allowing delayed reading of input stream
		);
	
	//
	// Get the length of the Cos stream and the byte offset 
	// of the Cos stream's data in the PDF document; 
	// Both shown in the debug window
	//

	ASInt64 cosStmLen = CosStreamLength64(cosStm);
	ASFilePos64 dataOffset = CosStreamPos64(cosStm);
	char msg[256];
#ifdef WIN_PLATFORM
	sprintf(msg, "Cos stream length = %I64d\nData byte offset = %I64d\n", cosStmLen, dataOffset);	
#else
	sprintf(msg, "Cos stream length = %lld\nData byte offset = %llu\n", cosStmLen, dataOffset);
#endif
	Console::displayString(msg);

	//
	// Set the page size and display region
	//

	ASFixedRect mediaBox, cropBox;
	mediaBox.left = fixedZero;
	mediaBox.top = FloatToASFixed(11*72);
	mediaBox.right = FloatToASFixed(8.5*72);
	mediaBox.bottom = fixedZero;
	cropBox.left = fixedZero;
	cropBox.top = ASInt32ToFixed(445);
	cropBox.right = ASInt32ToFixed(297);
	cropBox.bottom = fixedZero;
	
	//
	// Create a new PDPage object with letter-size dimension
	// Set the content display region
	//

	PDPage pdPage = PDDocCreatePage(pdDoc, PDBeforeFirstPage, mediaBox);
	PDPageSetCropBox(pdPage, cropBox);

	//
	// Set up the image transformation matrix;
	// Locate the image at the lower-left bottom of the crop box
	//

	ASFixedMatrix imgMatrix;
	imgMatrix.a = ASInt16ToFixed(IMG_WIDTH);
	imgMatrix.d = ASInt16ToFixed(IMG_HEIGHT);
	imgMatrix.b = imgMatrix.c = fixedZero;
	imgMatrix.h = fixedZero;
	imgMatrix.v = fixedZero;

	// 
	// Create the PDEImage object representing the cos stream object
	//

	PDEImage pdeImg = PDEImageCreateFromCosObj(&cosStm, &imgMatrix, NULL, NULL);

	//
	// Add the PDEImage object to the page content
	//

	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, gExtensionID);
	PDEContentAddElem(pdeContent, kPDEBeforeFirst, (PDEElement)pdeImg);
	if (PDPageSetPDEContent(pdPage, gExtensionID) == false)
	{
		AVAlertNote("Failed to set the PDEContent back into the PDPages's Cos object.\n");
		E_RTRN_VOID;
	}

	//
	// Save the newly created document with the embedded image
	//
	ASPathName outPath = SnippetRunnerUtils::getOutputPath(outFile);
	PDDocSave(pdDoc, PDSaveFull | PDSaveCollectGarbage | PDSaveLinearized, outPath, ASGetDefaultFileSys(), NULL, NULL);

	// 
	// Clean up
	//

	CosObjDestroy(cosStm);
	ASStmClose(inStm);
	ASFileClose(asFile);
	ASFileSysReleasePath(NULL, outPath);
	PDERelease((PDEObject)pdeImg);
	PDPageReleasePDEContent(pdPage, gExtensionID); 
	PDPageRelease(pdPage);
	PDDocClose(pdDoc);
	Console::displayString("Output document saved.");

HANDLER
	char buf[256];
	ASGetErrorString(ERRORCODE, buf, 256);
	AVAlertNote(buf);
END_HANDLER
}

SNIPRUN_REGISTER(CosStream64Snip, "COS Layer:64 bit Cos Stream", "Demonstrates 64-bit APIs for Cos \
Stream object construction and info access. The snippet creates a new PDF document with \
an embedded image taken from an input file represented as a Cos stream.")

