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

 RegisterFileConverterSnip.cpp

 - Implementation of the AVConversionToPDFHandler.

 - The "ToPDF" conversion handler converts a proprietary file format
   to PDF. The file format appears as an option in the
   "Open As Adobe PDF..." dialog. Conversion handlers that support
   synchronous conversion also appear as supported file types in the
   batch framework.

*********************************************************************/

#include "SnippetRunner.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

static AVConversionToPDFHandlerRec gConversionHandler;

// ADT file format header info.
static const char *kPIUniqueID = "com.adobe.acrobatsdk.adt";
static const char *kPIFileTag = "_ADT";
static const char kPIHeaderTag = '%';
static const char kPIHeaderSep = '#';
static const ASInt16 kPITagLen = 4;
static const ASInt16 kPIMaxHeaderLen = 32;

// Plug-in defined error conditions.
static ASInt32 piErrUnknownFileType;
static ASInt32 piErrInvalidHeader;
static ASInt32 piErrIncorrectAmountOfData;

static const char *kPIUnknownFile = "Invalid file: does not begin with '_ADT'.";
static const char *kPIInvalidHeader = "Invalide file: corrupt header information.";
static const char *kPIIncorrectAmountOfData = "Invalid file: file size does not match image size.";

bool gFileTypeRegistered = false;

/*-------------------------------------------------------
	Helper Functions
-------------------------------------------------------*/

/* PIPDPageAddRGBImageRaw
** ------------------------------------------------------
**
** Adds an RBG image to the specified page. The current
** implementation requires that the image data provided
** in srcFile must be interlaced and cannot be compressed
** or encoded.
**
** Re-raises errors to client.
*/
static void PIPDPageAddRGBImageRaw (PDPage destPage, ASFile srcFile, ASInt32 dataOffset,
										ASInt32 imgWidth, ASInt32 imgHeight)
{
	char buf[kPIMaxHeaderLen];
	ASFixedMatrix imgMatrix;
	ASInt32 errorCode = 0;
	ASStm asStm = NULL;
	PDEImageAttrs pdeImageAttrs;
	PDEFilterSpec filterSpec;
	PDEFilterArray filterArray;

	// Configure the image attributes.
	memset (&pdeImageAttrs, 0, sizeof(PDEImageAttrs));
	pdeImageAttrs.flags = kPDEImageExternal;
	pdeImageAttrs.width = imgWidth;
	pdeImageAttrs.height = imgHeight;
	pdeImageAttrs.bitsPerComponent = 8;

	// Configure the image transformation matrix.
	// Renders image at the bottom left of the page
	// at a scale of 100%.

	imgMatrix.b = imgMatrix.c = imgMatrix.h = imgMatrix.v = fixedZero;
	imgMatrix.a = ASInt32ToFixed (imgWidth);
	imgMatrix.d = ASInt32ToFixed (imgHeight);

	// Specify that the FlateDecode compression filter should
	// be applied to the data stream.

	memset (&filterSpec, 0, sizeof(PDEFilterSpec));
	filterSpec.name = ASAtomFromString ("FlateDecode");
	filterArray.numFilters = 1;
	filterArray.spec[0] = filterSpec;

	PDEImage volatile pdeImage = NULL;
	PDEColorSpace volatile pdeColorSpace = NULL;
	PDEContent volatile pdeContent = NULL;

	DURING
		// Open a data stream and skip past the file header.
		asStm = ASFileStmRdOpen (srcFile, 0);
		ASStmRead (buf, 1, dataOffset, asStm);

		// Create the image XObject.
		pdeColorSpace = PDEColorSpaceCreateFromName (ASAtomFromString("DeviceRGB"));
		pdeImage = PDEImageCreate (&pdeImageAttrs, sizeof(pdeImageAttrs), &imgMatrix,
										0, pdeColorSpace, NULL, &filterArray, asStm, NULL, 0 );

		// Insert image into page content.
		pdeContent = PDPageAcquirePDEContent (destPage, gExtensionID);
		PDEContentAddElem (pdeContent, kPDEAfterLast, (PDEElement)pdeImage);

		// Commit our changes to the page content.
		PDPageSetPDEContent (destPage, gExtensionID);
	HANDLER
		errorCode = ASGetExceptionErrorCode();
	END_HANDLER

	// remember to release all objects that were created .
	if (NULL != pdeImage)
		PDERelease ((PDEObject)pdeImage);
	if (NULL != pdeColorSpace)
		PDERelease ((PDEObject)pdeColorSpace);
	if (NULL != pdeContent)
		PDPageReleasePDEContent (destPage, gExtensionID);

	// Close the stream.
	if (asStm)
		ASStmClose(asStm);

	// If an exception occurred, re-raise it.
	if (0 != errorCode)
		ASRaise (errorCode);
}

/* PIOpenAndValidateFile
** ------------------------------------------------------
**
** Opens the specfied file and validate that it conforms
** to the following specification:
**
** _ADT%width#height%...image data...EOF
**
** Returns offset from beginning of file to image data.
** Re-raises errors to client.
*/
static ASInt32 PIOpenAndValidateFile (ASFileSys fileSys, ASPathName path,
										ASFile *file, ASInt32 *imgWidth, ASInt32 *imgHeight)
{
	ASInt32 iDataOffset;
	char *marker, *ptr;

	*imgWidth = 0;
	*imgHeight = 0;

	DURING

		if (0 != ASFileSysOpenFile(fileSys, path, ASFILE_READ, file) )
			ASRaise (ASFileError(fileErrOpenFailed));

		char rdBuf[kPIMaxHeaderLen];
		memset(rdBuf, 0, kPIMaxHeaderLen);

		// Validate the file. Does it contain the tag?.
		if (kPITagLen != ASFileRead(*file, rdBuf, kPITagLen) || 0 != strcmp(kPIFileTag, rdBuf))
			ASRaise (piErrUnknownFileType);

		// Read header information from file.
		ASFileRead (*file, rdBuf, kPIMaxHeaderLen);
		if (kPIHeaderTag != rdBuf[0])
			ASRaise (piErrInvalidHeader);

		// Calculate size of header to pass back to client.
		char *headerEnd = strchr ((rdBuf + 1), kPIHeaderTag);
		if (NULL == headerEnd)
			ASRaise (piErrInvalidHeader);

		iDataOffset = ((headerEnd - rdBuf) + kPITagLen + 1);

		// Extract width and height from header string.
		if (NULL == (marker = strchr(rdBuf, kPIHeaderSep)))
			ASRaise (piErrInvalidHeader);

		ptr = (marker + 1);
		*marker = 0;

		*imgWidth = atoi (rdBuf + 1);
		*imgHeight = atoi (ptr);

		// Perform some high-level validation.
		if ((*imgWidth <= 0) || (*imgHeight <= 0))
			ASRaise (piErrInvalidHeader);
		else if (((*imgWidth * *imgHeight * 3) + iDataOffset) != ASFileGetEOF(*file))
			ASRaise (piErrIncorrectAmountOfData);

	HANDLER
		if (NULL != *file) {
			ASFileClose (*file);
			*file = NULL;
		}
		RERAISE();
	END_HANDLER

	return iDataOffset;
}

/*-------------------------------------------------------
	AVConversionToPDFHandler Callbacks
-------------------------------------------------------*/

/* ADTDefaultSettingsProc [optional]
** ------------------------------------------------------
**
** Called to obtain an ASCab descibing the settings that
** are used to populate settings dialog.
*/
ACCB1 ASCab ACCB2 ADTDefaultSettingsProc (const char *filterDescription, AVConversionClientData clientData)
{
	return NULL;
}

/* ADTParamDescProc [optional]
** ------------------------------------------------------
**
** Called to obtain the conversion parameter description.
*/
ACCB1 void ACCB2 ADTParamDescProc (const ASCab settings, ASCab paramDesc, AVConversionClientData clientData)
{

}

/* ADTSettingsDialogProc [optional]
** ------------------------------------------------------
**
** Called to display settings dialog. We don't have one.
*/
ACCB1 ASBool ACCB2 ADTSettingsDialogProc (ASCab settings, AVConversionClientData clientData)
{
	return true;
}

/* ADTConvertToPDFProc
** ------------------------------------------------------
**
** Called to convert a file to PDF.
*/
ACCB1 AVConversionStatus ACCB2 ADTConvertToPDFProc (ASCab inSettings, AVConversionFlags flags,
													ASPathName inPath, ASFileSys inFileSys, PDDoc *outPDDoc,
													AVStatusMonitorProcs statusMonitor,
													AVConversionClientData clientData)
{
	AVConversionStatus retVal = kAVConversionSuccess;
	PDDoc volatile destDoc = NULL;
	PDPage volatile destPage = NULL;
	ASFile volatile srcFile = NULL;
	ASInt32 iImgWidth = 0, iImgHeight = 0, dataOffset;

	DURING
		// Open and validate the source file.
		dataOffset = PIOpenAndValidateFile (inFileSys, inPath, (ASFile*)&srcFile, &iImgWidth, &iImgHeight);

		// Create the new PDF document.
		destDoc = PDDocCreate();

		// Create the page that will hold the image.
		ASFixedRect mediaBox;
		mediaBox.bottom = mediaBox.left = 0;
		mediaBox.right = ASInt32ToFixed (iImgWidth);
		mediaBox.top = ASInt32ToFixed (iImgHeight);
		destPage = PDDocCreatePage (destDoc, PDBeforeFirstPage, mediaBox);

		// Add the image to the page.
		PIPDPageAddRGBImageRaw (destPage, srcFile, dataOffset, iImgWidth, iImgHeight);

	HANDLER
		// Try to report error through statusMonitor. If that fails,
		// use traditional AVAlert if flags allow.
		char errBuf[256];

		if (NULL != statusMonitor->reportProc)
			statusMonitor->reportProc (kASReportError, ERRORCODE, NULL, NULL, NULL,
										statusMonitor->reportProcClientData);
		else if (kAVConversionInteractive == (flags & kAVConversionInteractive)) {
			ASGetErrorString (ERRORCODE, errBuf, 256);
			AVAlertNote (errBuf);
		}

		retVal = kAVConversionFailed;
	END_HANDLER

	// Release all resources we may have opened or acquired.
	if (NULL != srcFile) ASFileClose (srcFile);
	if (NULL != destPage) PDPageRelease (destPage);

	// Return PDDoc to conversion framework if it's valid.
	if (kAVConversionFailed == retVal)
		PDDocClose (destDoc);
	else
		*outPDDoc = destDoc;

	return retVal;
}

/* Implementation of the AVConversionToPDFHandler.
	The "ToPDF" conversion handler converts a proprietary file format
	to PDF. The file format appears as an option in the
	"Open As Adobe PDF..." dialog. Conversion handlers that support
	synchronous conversion also appear as supported file types in the
	batch framework.
	@testfile RegisterFileConverter.adt
	@requires adt file to test if conversion handler got installed correctly
	@see ASFileStmRdOpen
	@see ASStmRead
	@see PDEColorSpaceCreateFromName
	@see PDEImageCreate
	@see PDPageAcquirePDEContent
	@see PDEContentAddElem
	@see PDPageSetPDEContent
	@see PDERelease
	@see PDPageReleasePDEContent
	@see ASStmClose
	@see ASFileSysOpenFile
	@see ASFileRead
	@see ASFileGetEOF
	@see ASFileClose
	@see PDDocCreate
	@see PDDocCreatePage
	@see PDPageRelease
	@see PDDocClose
	@see ASTextNew
	@see ASTextSetEncoded
	@see ASScriptToHostEncoding
	@see AVAppRegisterToPDFHandler
	@see ASRegisterErrorString
*/
void RegisterFileConverterSnip()
{

	if(gFileTypeRegistered == true)
	{
		AVAlertNote("File type ADT already registered.");
		return;
	}
	const int kPINumExtensions = 1;
	const char *kPIFilterDesc = "ADT files";
	const char *extArray[] = {"adt"};

	memset (&gConversionHandler, 0, sizeof(AVConversionToPDFHandlerRec));
	gConversionHandler.size = sizeof(AVConversionToPDFHandlerRec);

	// AVConversionHandler properties.
	gConversionHandler.canDoSync = true;
	gConversionHandler.priority = 1;

	// Create descriptors for the file types we support.
	AVFileDescRec *fileDesc;
	fileDesc = (AVFileDescRec*)ASmalloc (sizeof(AVFileDescRec) * kPINumExtensions);
	if (NULL == fileDesc)
		return;

	memset (fileDesc, 0, (sizeof(AVFileDescRec) * kPINumExtensions));
	for (ASUns16 i = 0; i < kPINumExtensions; i++)
		strcpy (fileDesc[i].extension, extArray[i]);

	DURING

		// Store the descriptors in the conversion handler.
		gConversionHandler.convFilter.fileDescs = fileDesc;
		gConversionHandler.convFilter.numFileDescs = kPINumExtensions;
		gConversionHandler.convFilter.filterDescription = ASTextNew();
		ASTextSetEncoded (gConversionHandler.convFilter.filterDescription,
							kPIFilterDesc, ASScriptToHostEncoding(kASRomanScript));

		/* Initialize the callbacks that we implement.
		gConversionHandler.defaultSettings = ASCallbackCreateProto (AVConversionDefaultSettingsProc, ADTDefaultSettingsProc);
		gConversionHandler.parameterDescription = ASCallbackCreateProto (AVConversionParamDescProc, ADTParamDescProc);
		gConversionHandler.settingsDialog = ASCallbackCreateProto (AVConversionSettingsDialogProc, ADTSettingsDialogProc);*/
		gConversionHandler.convert = ASCallbackCreateProto (AVConversionConvertToPDFProc, ADTConvertToPDFProc);

		// Every conversion handler has a unique identifier of the
		// form com.companyname.productname.type.
		strcpy (gConversionHandler.uniqueID, kPIUniqueID);

		// Register the AVCommandHandler.
		AVAppRegisterToPDFHandler (&gConversionHandler);

		// Register the error codes used by the AVCommandHandler.
		piErrUnknownFileType = ASRegisterErrorString (ErrSuppressable, kPIUnknownFile);
		piErrInvalidHeader = ASRegisterErrorString (ErrSuppressable, kPIInvalidHeader);
		piErrIncorrectAmountOfData = ASRegisterErrorString (ErrSuppressable, kPIIncorrectAmountOfData);

	HANDLER
		AVAlertNote("Registration failed!");
		return;
	END_HANDLER
	
		Console::displayString("ADT file type successfully registered.");
		gFileTypeRegistered = true;
	return;
}

SNIPRUN_REGISTER(RegisterFileConverterSnip, "AV Layer:Register File Converter", "Registers a conversion handler that produces PDF from an ADT file.")
