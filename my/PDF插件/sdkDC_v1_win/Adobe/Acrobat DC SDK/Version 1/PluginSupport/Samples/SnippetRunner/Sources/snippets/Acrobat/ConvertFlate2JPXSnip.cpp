/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

ConvertFlate2JPXSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** The ConvertFlate2JPXSnip snippet demonstrate how to reencode pdf embedded 
    Flate image XObjects with the JPX (JPEG2000) encoding standard using PDFEdit
	and Cos layer APIs.

	@testfile cottage.pdf
	@requires pdf with flate encoded xobject
	@see PDEImageGetType
	@see PDEImageAcquireImageFlate
	@see PDEImageFlateAcquireColorSpace
	@see PDEImageFlateGetAttrs
	@see PDEImageFlateGetCosObj
	@see PDEImageFlateGetDataStm
	@see PDDocOpenFromASFile
	@see PDDocAcquirePage
	@see PDPageAcquirePDEContent
	@see PDEContentGetNumElems
	@see PDEContentGetElem
	@see PDEObjectGetType
	@see PDPageReleasePDEContent
	@see PDPageRelease
	@see PDDocClose
	@see ASFileClose
	@see PDDocCreatePage
*/

/*------------------------------------------------------------------
	Constants
-------------------------------------------------------------------*/
static const char * STR_OUT_FILE_NAME = "JPX_out.pdf";
static const char * STR_SRC_FILE_NAME = "cottage.pdf";
static const char * STR_FLATE_DECODE = "FlateDecode";

PDEImage GetJPXEncodedImage(PDDoc dstDoc);

/*------------------------------------------------------------------
	Implementation
-------------------------------------------------------------------*/
void ConvertFlate2JPXSnip()
{
	DURING
		// Create new document
		PDDoc pdDstFile = PDDocCreate();
		
		// Set the media box of the new document pages
		ASFixedRect mediaBox;
		mediaBox.left = fixedZero;
		mediaBox.top = Int16ToFixed(11 * 72);
		mediaBox.right = Int16ToFixed(8.5 * 72);
		mediaBox.bottom = fixedZero;

		// Create page with the mediabox and insert as first page
		PDPage dstPage = PDDocCreatePage(pdDstFile, PDBeforeFirstPage, mediaBox);

		// Acquire PDEContent container for page
		PDEContent dstContent = PDPageAcquirePDEContent(dstPage, NULL);

		PDEImage pdeImage = GetJPXEncodedImage(pdDstFile);
		if(!pdeImage) {
			// Clean up
			PDPageReleasePDEContent(dstPage, NULL);
			PDPageRelease(dstPage);
			PDDocClose(pdDstFile);
			E_RTRN_VOID;
		}

		// Insert image into page content
		PDEContentAddElem(dstContent, kPDEAfterLast, (PDEElement)pdeImage);

		// Set the PDEContent for the page
		PDPageSetPDEContent(dstPage, NULL);

		// Save document to a file
		ASPathName asPathName = SnippetRunnerUtils::getOutputPath(STR_OUT_FILE_NAME);

		if(asPathName != NULL)	// Got OutputPath info
			PDDocSave(pdDstFile, PDSaveFull|PDSaveCollectGarbage|PDSaveLinearized, asPathName, NULL, NULL, NULL);

		// Clean up
		PDERelease((PDEObject)pdeImage);
		PDPageReleasePDEContent(dstPage, NULL);
		PDPageRelease(dstPage);
		PDDocClose(pdDstFile);
		if(asPathName != NULL) {
			ASFileSysReleasePath(NULL, asPathName);
			Console::displayString("Output document saved.\n");
		} else {
			AVAlertNote("Output path is not set. Output not saved.\n");
		}
	HANDLER	
		ASInt32 iErrCode = ASGetExceptionErrorCode();
		const ASInt32 BUF_SIZE = 256;
		char errBuf[BUF_SIZE];
		ASGetErrorString(iErrCode, errBuf, BUF_SIZE);
		strcat(errBuf, "\n");
		AVAlertNote(errBuf);
	END_HANDLER
}

PDEImage GetJPXEncodedImage(PDDoc dstDoc)
{
	ASFile volatile asSrcFile = NULL;
	PDDoc volatile pdSrcFile = NULL;
	PDEImage dstImage; // PDEImage object to be created
	char strMsg[256];

	DURING
		asSrcFile = SnippetRunnerUtils::openSnippetExampleASFile(STR_SRC_FILE_NAME);
		if(!asSrcFile) {
			sprintf(strMsg, "Failed to locate sample file %s\n", STR_SRC_FILE_NAME);
			AVAlertNote(strMsg);
			E_RETURN(NULL); 
		}
			
		pdSrcFile = PDDocOpenFromASFile(asSrcFile, NULL, true);
		if(!pdSrcFile){
			sprintf(strMsg, "Failed to open sample file %s\n", STR_SRC_FILE_NAME);
			AVAlertNote(strMsg);

			// Close the file we opened and return
			ASFileClose(asSrcFile);
			E_RETURN(NULL);
		}

		// We only deal with the first page of the document
		PDPage srcPage = PDDocAcquirePage(pdSrcFile, 0);
		PDEContent srcContent = PDPageAcquirePDEContent(srcPage, 0);
		ASInt32 iNumElems = PDEContentGetNumElems(srcContent);

		ASBool bFound = false;
		PDEElement srcElem = NULL;
		PDEImage srcImage = NULL;

		// Enumerate the content elelments to find an Flate image XObject.
		// For demonstration purpose, we only deal with the first found image XObject
		for(int i = 0; i < iNumElems; i++) {
			srcElem = PDEContentGetElem(srcContent, i);
			
			// Make sure we are getting an JPX image XObject
			if(PDEObjectGetType((PDEObject)srcElem) == kPDEImage
				&& PDEImageIsCosObj((PDEImage)srcElem)) {
					ASAtom imgType = PDEImageGetType((PDEImage)srcElem);
					if(!strcmp(STR_FLATE_DECODE, ASAtomGetString(imgType))) {
						srcImage = (PDEImage)srcElem;
						bFound = true;
						break;
					}
				}
		}

		// No JPX image XObject found
		if(!bFound) {
			// Release resources
			if(srcContent)
				PDPageReleasePDEContent(srcPage, gExtensionID);
			if(srcPage)
				PDPageRelease(srcPage);
			// Close the file
			PDDocClose(pdSrcFile);
			ASFileClose(asSrcFile);
			
			sprintf(strMsg, "No Flate image XObject found on the first page of %s\n", STR_SRC_FILE_NAME);
			Console::displayString(strMsg);

			E_RETURN(NULL);
		}

		// Acquire the PDEImageFlate resource of the Flate image.
		PDEImageFlate imageFlate = PDEImageAcquireImageFlate(srcImage);

		// Get the decoded data stream from the Flate image XObject
		ASStm srcStm = PDEImageFlateGetDataStm(imageFlate, 0);

		// Get the Flate image attributes
		PDEImageAttrs srcImageAttrs;
		PDEImageFlateGetAttrs(imageFlate, &srcImageAttrs, sizeof(srcImageAttrs));

		// Acquire the PDEColorSpace accociated with the Flate encoded image
		PDEColorSpace srcColorSpace = PDEImageFlateAcquireColorSpace(imageFlate);

		// Get the Flate image transformation matrix
		ASFixedMatrix srcMatrix;
		PDEElementGetMatrix((PDEElement)srcImage, &srcMatrix);

		PDEFilterArray filterArray; // Structure to specify filter information for data streams
		PDEFilterSpec filterSpec; // Structure to store filter elements in a filter array

		CosDoc cosDoc = PDDocGetCosDoc(dstDoc);

		// Dictionary of parameters passes to the JPX encoder
		CosObj jpxParams = CosNewDict(cosDoc, false, 15);

		// width and height of image in pixels
		CosDictPutKeyString(jpxParams, "Width", CosNewInteger(cosDoc, false, srcImageAttrs.width));
		CosDictPutKeyString(jpxParams, "Height", CosNewInteger(cosDoc, false, srcImageAttrs.height));

		// with and height of tiles to use in compression; 256 X 256 is typical
		CosDictPutKeyString(jpxParams, "TileWidth", CosNewInteger(cosDoc, false, 256));
		CosDictPutKeyString(jpxParams, "TileHeight", CosNewInteger(cosDoc, false, 256));

		// Should the decorrelation transform be used or not. For best results this is set to true for
		// RGB, for LAB false, and for CMYK true may yield better compression.
		CosDictPutKeyString(jpxParams, "ColorTransform", CosNewBoolean(cosDoc, false, false));

		// Quality is a value from 1 to 100.
		CosDictPutKeyString(jpxParams, "Quality", CosNewInteger(cosDoc, false, 75));

		// If lossless comopression is specified, the Quality number is ignored.
		CosDictPutKeyString(jpxParams, "EncodeLossLessly", CosNewBoolean(cosDoc, false, false));

		// The number of compression resolutions to place in the data.
		// 5 gives good compression, and good decompression performance.
		CosDictPutKeyString(jpxParams, "ResProgressionSize", CosNewInteger(cosDoc, false, 5));

		// The image samples may be indices into a color look-up table
		// rather than direct color components. This is signified by a Palette 
		// entry in the jpxParams. 
		//
		if (PDEColorSpaceGetName(srcColorSpace) == ASAtomFromString("Indexed")) {
			// Here we deal with DeviceRGB base color space
			if (PDEColorSpaceGetBase(srcColorSpace) == ASAtomFromString("DeviceRGB")) {
				// Get the number of entries in the color look-up table
				ASInt32 sizeLUT = PDEColorSpaceGetHiVal(srcColorSpace) + 1;
				ASInt32 baseNumComps = PDEColorSpaceGetBaseNumComps(srcColorSpace);
			
				// Obtain the color look-up table
				ASUns8* bufLUT = (ASUns8*)ASmalloc(sizeLUT * baseNumComps);
				PDEColorSpaceGetCTable (srcColorSpace, bufLUT);

				// The Palette entry is an array. The first entry is an integer giving 
				// the size of the look-up table in terms of the number of entries. 
				// The second Palette entry is also an integer and gives the bit depth 
				// of the entries generated by the table. The remaining entries in 
				// Palette are strings giving the bytes making up the look-up table. 
				// There is one string for each component in the color space on which 
				// The palette is based. This may be any of the color spaces described next.
				//

				// Decomposing the color table entries (in the pattern of
				// RGB...RGB...RGB...) into RRR..., GGG..., and BBB... strings 
				ASUns8* rCompStr = (ASUns8*)ASmalloc(sizeLUT);
				ASUns8* gCompStr = (ASUns8*)ASmalloc(sizeLUT);
				ASUns8* bCompStr = (ASUns8*)ASmalloc(sizeLUT);

				for (int i = 0, j = 0; i < sizeLUT; i++, j += baseNumComps) {
					rCompStr[i] = bufLUT[j];
					gCompStr[i] = bufLUT[j+1];
					bCompStr[i] = bufLUT[j+2];
				}
				ASfree(bufLUT);

				// Composing the Palette array
				CosObj palette = CosNewArray(cosDoc, false, baseNumComps + 2);

				CosArrayPut(palette, 0, CosNewInteger( cosDoc, false, sizeLUT));
				CosArrayPut(palette, 1, CosNewInteger( cosDoc, false,  srcImageAttrs.bitsPerComponent));
				
				CosObj rComp = CosNewString(cosDoc, false, (char*)rCompStr, sizeLUT);
				CosObj gComp = CosNewString(cosDoc, false, (char*)gCompStr, sizeLUT);
				CosObj bComp = CosNewString(cosDoc, false, (char*)bCompStr, sizeLUT);
				CosArrayPut(palette, 2, rComp);
				CosArrayPut(palette, 3, gComp);
				CosArrayPut(palette, 4, bComp);
				
				ASfree(rCompStr);
				ASfree(gCompStr);
				ASfree(bCompStr);

				CosDictPutKeyString(jpxParams, "Palette", palette);

				// These two entries, BitsPerComponent and Colors, describe the image samples 
				// to the JPX compressor. Colors is the number of color components.

				// In the case of indexed color space, Colors has a value of 1 indicating that 
				// there is only one channel of data (the indices). 
				// BitsPerComponent gives the number of components in the indices 
				// - either 8 or 16. The look-up table may contain up to 1024 entries.
				CosDictPutKeyString(jpxParams, "BitsPerComponent", CosNewInteger(cosDoc, false, srcImageAttrs.bitsPerComponent));
				CosDictPutKeyString(jpxParams, "Colors", CosNewInteger(cosDoc, false, 1));
	
				// The supported JPEG2000 enumerated color space values:
				//
				// 12 - CMYK
				// 14 - LAB
				// 16 - sRGB
				// 17 - grayscale
				// 18 - sYCC
				// 20 - e-sRGB
				// 21 - ROMMRGB
				// 24 - e-sYCC
				//

				// only one color spec being provided here
				CosObj colorSpecs = CosNewArray(cosDoc, false, 1); 
				CosObj aColorSpec = CosNewDict(cosDoc, false, 1);
				CosArrayPut(colorSpecs, 0, aColorSpec);

				// The Method key specifies the base color space of the Indexed color space
				CosDictPutKeyString(aColorSpec, "Method", CosNewInteger(cosDoc, false, 16)); 

				CosDictPutKeyString(jpxParams, "ColorSpace", colorSpecs);
			} else {
				Console::displayString("We don't deal with Indexed color spaces with base color space other than /DeviceRGB here.\n");
				// clean up
				PDERelease((PDEObject) srcColorSpace);
				ASStmClose(srcStm);
				PDDocClose(pdSrcFile);
				E_RETURN(NULL);
			}
		} else {
			// We handle non-indexed color spaces here

			// These two entries, BitsPerComponent and Colors, describe the image samples 
			// to the JPX compressor. Colors is the number of color components.

			CosDictPutKeyString(jpxParams, "BitsPerComponent", CosNewInteger(cosDoc, false, srcImageAttrs.bitsPerComponent));
			CosDictPutKeyString(jpxParams, "Colors", CosNewInteger(cosDoc, false, PDEColorSpaceGetNumComps(srcColorSpace)));

			CosObj colorSpecs = CosNewArray(cosDoc, false, 1); 
			CosObj aColorSpec = CosNewDict(cosDoc, false, 1);
			CosArrayPut(colorSpecs, 0, aColorSpec);
			
			// We specify 16 for the /Method key for DeviceRGB source image color space.
			// For other source color space, refer to the color space values listed above.
			// For instance, for source image in CMYK color space, specify 12 instead.

			CosDictPutKeyString(aColorSpec, "Method", CosNewInteger(cosDoc, false, 16)); 
			CosDictPutKeyString(jpxParams, "ColorSpace", colorSpecs);
		}

		// Fill in values for the PDEFilterSpec object
		memset(&filterSpec, 0, sizeof (PDEFilterSpec));
		filterSpec.padding = 0;
		filterSpec.encodeParms = jpxParams;
		filterSpec.decodeParms = CosNewNull();
		filterSpec.name = ASAtomFromString ("JPXDecode");
	
		// Fill in values for the PDEFilterArray object
		filterArray.numFilters = 1;
		filterArray.spec[0] = filterSpec;

		// Create the destination image XObject by passing in source image data stream,
		// image attributes, and JPXDecode filter information
		dstImage = PDEImageCreate(&srcImageAttrs, sizeof(srcImageAttrs),
			&srcMatrix, 0, srcColorSpace, NULL, &filterArray, srcStm, NULL, 0);

		CosObj    obj;
		PDEImageGetCosObj(dstImage, &obj);

		// For JPX images this dictionary entries is optional; 
		// Remove so that the color space specified in the JPEG2000 
		// data is used
		CosDictRemoveKeyString(obj, "ColorSpace");         
		// For JPX images this is never consulted; remove it
		CosDictRemoveKeyString(obj, "BitsPerComponent"); 

		// Do some clean-up before returning the PDEImage object
		PDERelease((PDEObject) srcColorSpace);
		ASStmClose(srcStm);
		PDDocClose(pdSrcFile);
		Console::displayString("Finished converting image from flate encoding to JPEG2000 encoding.\n");

	HANDLER	
		ASInt32 iErrCode = ASGetExceptionErrorCode();
		const ASInt32 BUF_SIZE = 256;
		char errBuf[BUF_SIZE];
		ASGetErrorString(iErrCode, errBuf, BUF_SIZE);
		strcat(errBuf, "\n");
		AVAlertNote(errBuf);
	END_HANDLER

	return dstImage;
}

SNIPRUN_REGISTER(ConvertFlate2JPXSnip, "PDE Layer:Image:ConvertFlate2JPXSnip", 
				 "Demonstrates how to reencode Flate encoded image XOjects \
into JPX (JPEG2000) image XObjects. The source file with embedded Flate \
image XObject is called \"cottage.pdf\" located in the \"ExampleFiles\" \
folder. The output file with reencoded JPEG image XObject will be put in \
the \"OutputFiles\" folder with the name \"JPX_out.pdf\".")
