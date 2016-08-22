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

 JPXPaletteExplorerSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"


union Bytes {
	unsigned integer;
	char bytes[4];
} test, result;

static enum Endianness { big, little} endian;

static Endianness CheckEndianness();
static ASInt32 BytesToInt(char*, int);
static void DisplayJPXPaletteInfo(JPXPalette);


/** 
	This snippet enumerates image XObjects on a page. For each JPX encoded
	image XObject, displays palette information.

	@testfile JPXPalette.pdf
	@requires pdf with jpx encoded image
	@see PDEImageGetType
	@see PDEImageAcquireImageJPX
	@see PDEImageJPXHasPalette
	@see PDEImageJPXAcquirePalette
	@see JPXPaletteGetNumComponents
	@see JPXPaletteGetBitDepths
	@see JPXPaletteGetNumEntries
	@see JPXPaletteGetTable
*/
static void JPXPaletteExplorerSnip()
{
	CDocument doc;
	PDPage pdPage = (PDPage) doc;
	if (!pdPage) return;

	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, gExtensionID);
	int numElems = PDEContentGetNumElems(pdeContent);

	//
	// Enumerates PDEContent elements on the first page to find 
	// available JPX encoded image XObjects
	//
	int count = 0;	// count of JPX image XObjects on the first page

DURING

	PDEElement elem;
	for (int i=0; i<numElems; i++)
	{
		elem = PDEContentGetElem(pdeContent, i);
		if (PDEObjectGetType((PDEObject)elem) == kPDEImage)
		{
			if (PDEImageGetType((PDEImage)elem) == ASAtomFromString("JPXDecode"))
			{
				char msg[64];
				sprintf(msg, "* JPX image #%d\n", count++);
				Console::displayString(msg);

				PDEImageJPX imgJPX;
				JPXPalette jpxPalette;

				// Acquires the PDEImageJPX resource of the PDEImage content element

				imgJPX = PDEImageAcquireImageJPX((PDEImage)elem);

				// Check whether the JPX image has a palette entry

				if (PDEImageJPXHasPalette(imgJPX))
				{
					// Acquires the JPXPalette from the JPX image XObject

					jpxPalette = PDEImageJPXAcquirePalette(imgJPX);

					// Display palette info with a helper function

					DisplayJPXPaletteInfo(jpxPalette);

					// Clean up

					PDERelease((PDEObject)jpxPalette);
				} else {
					Console::displayString("This JPX image object does not have palette entry.\n");
				}

				PDERelease((PDEObject)imgJPX);
			}
		}
	}
HANDLER
	ASInt32 error = ERRORCODE;
	char err[256];
	ASGetErrorString(error, err, sizeof(err));
	AVAlertNote(err);
END_HANDLER

	// Clean up

	PDPageReleasePDEContent(pdPage, gExtensionID);

	if (count <= 0)
	{
		Console::displayString(
			"No JPX encoded image XObjects found on the first page of the open document.\n");
		return;
	}
}

/**
	Helper function that extracts JPX palette information
*/
static void DisplayJPXPaletteInfo(JPXPalette jpxPalette)
{
DURING
	// Gets the number of color components in the palette

	ASInt32 numComps = JPXPaletteGetNumComponents(jpxPalette);

	// Gets an array of bit depths for each color component

	ASInt32* aBitDepths = (ASInt32*) ASmalloc(sizeof(ASInt32)*numComps);
	JPXPaletteGetBitDepths(jpxPalette, aBitDepths);

	char msg[128] = "";
	sprintf(msg, "Palette has %d color components.\n", numComps);
	Console::displayString(msg);
	
	sprintf(msg, "Bit depths [comp1..comp%d]: [", numComps);
	
	char buf[64];
	memset(buf, 0, sizeof(buf));
	for (int i=0; i<numComps; i++)
	{
		sprintf(buf, "%i", aBitDepths[i]); 
		strcat(msg, buf);
		strcat(msg, " ");
	}
	strcat(msg, "]\n");

	Console::displayString(msg);

  ASfree(aBitDepths);

	//
	// We Handle the color table from here on
	//

	ASStm stmPalette;
	ASInt32 lenPalette;
	ASInt32 numEntries;

  numEntries = JPXPaletteGetNumEntries(jpxPalette);
	 
	// Gets the color look-up table as an ASStm
	
	stmPalette = JPXPaletteGetTable(jpxPalette, &lenPalette);

	char msg2[128];
	sprintf(msg2, "The palette has %d entries and a length of %d bytes.\n", 
		numEntries, lenPalette);
	Console::displayString(msg2);
	Console::displayString("Color table entries of the palette:\n");

	//
	// Each component entry in the palette is represented by the number
	// of bytes needed to contain the bit depth for that component.
	//

	int bytesPerComp = lenPalette/numEntries/numComps;
	int bytesPerEntry = lenPalette/numEntries;
  char* bufEntry = (char*) ASmalloc(sizeof(char)*lenPalette);

	ASStmRead(bufEntry, lenPalette/numEntries, numEntries, stmPalette);
	ASStmClose(stmPalette);

	char* bufComp;
	bufComp = (char*) ASmalloc(sizeof(char)*bytesPerComp);

	// Check platform byte ordering to determine color table entry representation

	endian = CheckEndianness();

	// Interpret and display content of color table entries

	char msgTbl[128], temp[64];
	char* curPos = bufEntry;
	for (int i=lenPalette; i>0; i-=bytesPerEntry)
	{
		memset(msgTbl, 0, sizeof(msgTbl));

		strcpy(msgTbl, "[");
		for (int j=0; j<numComps; j++)
		{
			memset(temp, 0, sizeof(temp));

			strncpy(bufComp, curPos, bytesPerComp);
			sprintf(temp, "%i", BytesToInt(bufComp, bytesPerComp));
			strcat(msgTbl, temp);
			strcat(msgTbl, " ");
			curPos += bytesPerComp;
		}	
		strcat(msgTbl, "]\n");
		Console::displayString(msgTbl);
	}
	
	ASfree(bufComp);
  ASfree(bufEntry);

HANDLER
	ASInt32 error = ERRORCODE;
	char err[256];
	ASGetErrorString(error, err, sizeof(err));
	AVAlertNote(err);
END_HANDLER

	Console::displayString("-------------------------\n");
}

/**
	Helper function that facilitats interpreting color table entry contents
*/
static ASInt32 BytesToInt(char* inBytes, int lenBytes)
{
	result.integer = 0x00000000;

	// Re-ordering bytes if necessary, depending on byte ordering of platform

	if (endian == little)
		for (int i=0; i<lenBytes; i++)
			result.bytes[i] = inBytes[i];
	else
		for (int i=0; i<lenBytes; i++)
			result.bytes[3-i] = inBytes[i];

	return (ASInt32) result.integer;
}

/**
	Helper function to determine byte ordering of color table entry components
*/
static Endianness CheckEndianness()
{
	test.integer = 0x0c0d0e0f;

	if (test.bytes[0] == 0x0c)
		return big;
	else
		return little;
}

SNIPRUN_REGISTER(JPXPaletteExplorerSnip, "PDE Layer:Image:JPX Palette Exploration", \
"This snippet displays palette information of JPX image XObjects. \
This operation may take a while ...")

