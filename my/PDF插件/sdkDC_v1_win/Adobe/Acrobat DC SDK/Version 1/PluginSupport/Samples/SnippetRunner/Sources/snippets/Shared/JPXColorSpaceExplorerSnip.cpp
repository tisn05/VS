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

 JPXColorSpaceExplorerSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static void InterpretEnumeratedColorSpace(JPXColorSpace);
static void InterpretICCColorProfile(JPXColorSpace);
static void DisplayJPXColorSpaceInfo(JPXColorSpace);

/** 
	This snippet enumerates image XObjects on a page. For each JPX encoded
	image XObject, displays its color space information.

	The JPX color space information is written to the debug window.

	<h3>Implementation</h3>
	<ul>
	<li> Enumerates elements on the first page of document</li>
	<li> For each image XObject that is JPX encoded
		<ul>
			<li> Acquires a linked list of JPXColorSpace objects 
			defined with the JPX encoded image	</li>
			<li> Displays the JPX color space information	</li>
			<li> Continue until we reach the end of the linked list</li>
		</ul>
	</li>
	<li> Continue until all JPX encoded image XObjects in 
	the page content are exhausted  </li>
	</ul>
 	@testfile JPEG2000.pdf
 	@requires pdf with jpx encoded object
	@see PDEImageGetType
	@see PDEImageAcquireImageJPX
	@see PDEImageJPXAcquireJPXColorSpace
	@see JPXColorSpaceAcquireNext
	@see JPXColorSpaceGetType
	@see JPXColorSpaceGetEnumAttrs
	@see JPXColorSpaceGetProfile
	@see ACMakeBufferProfile
	@see ACProfileDescription
	@see ACStringASCII
	@see ACUnReferenceProfile
*/
static void JPXColorSpaceExplorerSnip()
{
	// For brevity, we deal only with the first page of the document

	CDocument doc;
	PDPage pdPage = (PDPage) doc;
	if (!pdPage) return;

	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, gExtensionID);
	int numElems = PDEContentGetNumElems(pdeContent);

	//
	// Enumerates PDEContent elements on the first page to find 
	// available image XObjects encoded with JPXDecode filter 
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
				sprintf(msg, "* JPX Image XObject #%d\n", count++);
				Console::displayString(msg);
				
				PDEImageJPX imgJPX;
				JPXColorSpace csJPX;

				//
				// Acquires the PDEImageJPX resource of the PDEImage content element
				// when the image filter type is "JPXDecode" 
				//

				imgJPX = PDEImageAcquireImageJPX((PDEImage)elem);

				// Acquires a linked list of JPXColorSpace objects defined with the JPX encoded image.

				csJPX = PDEImageJPXAcquireJPXColorSpace(imgJPX);
				DisplayJPXColorSpaceInfo(csJPX);

				JPXColorSpace csJPXNxt = NULL, csTmp = csJPX;
				while ((csJPXNxt = JPXColorSpaceAcquireNext(csTmp)) != NULL)
				{
					DisplayJPXColorSpaceInfo(csJPXNxt);
					csTmp = csJPXNxt;
					PDERelease((PDEObject)csJPXNxt);
				}

				// Clean up

				PDERelease((PDEObject)imgJPX);
				PDERelease((PDEObject)csJPX);
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
			"No image XObjects encoded with JPXDecode filter found on the first page of the open document.\n");
		return;
	}
}

/**
	Helper function that displays color space information based on color space type
*/
static void DisplayJPXColorSpaceInfo(JPXColorSpace csJPX)
{

DURING

	// Gets info for display based on various color space types

	JPXColorSpaceType csJPXType = JPXColorSpaceGetType(csJPX);
	switch (csJPXType) 
	{
	case kJPXCSEnumerated:	// Enumerated color space
		Console::displayString("Enumerated color space\n");
		InterpretEnumeratedColorSpace(csJPX);
		break;
	case kJPXCSRestrictedICC:	// The only type of ICC profiles allowed for JP2 images.
		Console::displayString("Restricted ICC Based color space\n");
		InterpretICCColorProfile(csJPX);
		break;
	case kJPXCSAnyICC:	// ICC-based color space)
		Console::displayString("ICC based color space\n");
		InterpretICCColorProfile(csJPX);
		break;
	case kJPXCSVenderColor:	// Vender-defined color space; for color spaces of 2 or more than 4 color components
		Console::displayString("Vender-defined color space\n");
		break;
	default: // kJPXCSUnknown
		Console::displayString("Unknown color space\n");
		break;
	}

HANDLER
	ASInt32 error = ERRORCODE;
	char err[256];
	ASGetErrorString(error, err, sizeof(err));
	AVAlertNote(err);
END_HANDLER

	Console::displayString("-------------------------\n");
}

/**
	Helper function that extracts enumerated color space informarion
*/
static void InterpretEnumeratedColorSpace(JPXColorSpace csJPX)
{
	// Obtains the attributes of an enumerated color space.

	JPXCSEnumAttrs csJPXAttrs;
	JPXColorSpaceGetEnumAttrs(csJPX, &csJPXAttrs);
	
	//
	// Gets the color space name corresponding to the enumerated 
	// color space number as defined by the JPEG2000 spec
	//

	char strEnumCSType[64];
	switch (csJPXAttrs.enumNumber)
	{
	case 12: strcpy(strEnumCSType, "CMYK"); break;
	case 14: strcpy(strEnumCSType, "LAB"); break;
	case 16: strcpy(strEnumCSType, "sRGB"); break;
	case 17: strcpy(strEnumCSType, "grayscale"); break;
	case 18: strcpy(strEnumCSType, "sYCC"); break;
	case 20: strcpy(strEnumCSType, "e-sRGB"); break;
	case 21: strcpy(strEnumCSType, "ROMMRGB"); break;
	case 24: strcpy(strEnumCSType, "e-sYCC"); break;
	default: strcpy(strEnumCSType, "This enumerated color space is not supported by PDF"); break;
	}

	char msg[128];
	sprintf(msg, "Color space type: %s\nStd Illuminant: %c\nIlluminant Value: %u\n",
		strEnumCSType, (csJPXAttrs.stdIlluminant)?('Y'):('N'), csJPXAttrs.illuminant);
	Console::displayString(msg);

	if (csJPXAttrs.enumNumber == 14)	// LAB color space
	{
		char msg2[128];
		sprintf(msg2, "LAB Range: [%u %u %u]\nLAB Offset: [%u %u %u]\n",
			csJPXAttrs.labRange[0],csJPXAttrs.labRange[1], csJPXAttrs.labRange[2],
			csJPXAttrs.labOffset[0],csJPXAttrs.labOffset[1], csJPXAttrs.labOffset[2]);
		Console::displayString(msg2);
	}
}

/**
	Helper function that extracts color profile information for ICC-based color space
*/
static void InterpretICCColorProfile(JPXColorSpace csJPX)
{
  // Gets the color profile of an ICC based JPX color space.

  ASInt32 lenProfile;
	lenProfile = JPXColorSpaceGetProfile(csJPX, 0, 0);
  ASUns8* rawProfile;
	rawProfile = (ASUns8*) ASmalloc(sizeof(ASUns8)*lenProfile);
  JPXColorSpaceGetProfile(csJPX, rawProfile, lenProfile);

	// Costructs an AC_Profile object form the raw porilfe data
	
	AC_Profile iccProfile;
	if ( AC_Error_None != ACMakeBufferProfile(&iccProfile, rawProfile, lenProfile))
		Console::displayString("Something went wrong in trying to interpret the profile\n");
      
	ASfree(rawProfile);

	// Gets the profile description

	AC_String dscProfile;
	ACProfileDescription(iccProfile, &dscProfile);
	
	char dscBuf[128];
	ASUns32 countBuf;
	AC_Error err;

	// Gets the ASCII version of the profile description string

	err = ACStringASCII(dscProfile, dscBuf, &countBuf, sizeof(dscBuf));
	switch (err)
	{
	case AC_Error_NoASCII:
		Console::displayString("The profile does not contain ASCII description string.\n");
		break;
	case AC_Error_StringOverflow:
		Console::displayString("Profile description truncated.\n");
		break;
	case AC_Error_None:
		break;
	default:
		Console::displayString("Error in getting ASCII profile description string.\n");
		break;
	}

	char dsc[256];
	sprintf(dsc, "Profile: %s\nProfile size: %d bytes\n", dscBuf, lenProfile);
	Console::displayString(dsc);
	ACUnReferenceProfile(iccProfile);
}

SNIPRUN_REGISTER(JPXColorSpaceExplorerSnip, "PDE Layer:Image:JPX Color Space Exploration", \
		"This snippet displays color space characteristics of JPX image XObjects.")

