/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2004-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------
 
 ACETransPDETextColorSnip.cpp

***********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

#define CMYK_profile_K "U.S. Web Coated (SWOP) v2"
#define sRGB_profile_K "sRGB IEC61966-2.1"
#define DeviceRGB_K ASAtomFromString("DeviceRGB")
#define DeviceCMYK_K ASAtomFromString("DeviceCMYK")

static AC_Profile sRGBProfile;
static void TransformColor(PDEElement element);
void ACETransPDETextColorSnip();
void ACEconvertColorProfile (const char *orgColorProfile, const char *newColorProfile, void *streamData, void *streamOut, 
											ASInt32 size, AC_RenderIntent AC_intent, void *clientData);


ASBool gbConverted;


/** This snippet will take an input profile (DeviceCMYK) and create an output profile (sRGB), 
	make a color transform from them, and then apply the transform to a single color of PDF text objects.

	Implementation:
     * Enumerate all text runs of the front page
	 * If the text run's color space of its graphic state is DeviceCMYK, then get the CMYK color values, convert them to
	   ASUns8 types, transform the color values to RGB color values
	 * Reset the color space and color values of the text runs's graphic states
	 
	@testfile cmykTest.pdf
	@requires pdf file with cmyk text
  */
void ACETransPDETextColorSnip()
{
	ASInt32 j, numElems;
	PDEContent pdeContent = NULL;
	PDPage pg = NULL;
	PDEElement pdeElement;

	CDocument doc;
	pg = (PDPage)doc;
	if(pg == NULL) return;

	pdeContent = PDPageAcquirePDEContent(pg, 0);
	numElems = PDEContentGetNumElems(pdeContent);
	Console::displayString("PDEText color space conversion (C M Y K -> R G B):");

	gbConverted = false;
	for (j = 0; j < numElems; j++)
	{
		pdeElement = PDEContentGetElem(pdeContent, j); 
		TransformColor(pdeElement);
	}

	if( gbConverted )
	{
		// Commit the changes to the PDEContent.
		PDPageSetPDEContent(pg, gExtensionID);

		// Notify the change
		PDPageNotifyContentsDidChange (pg);
	}

	if (pdeContent != NULL){
		PDPageReleasePDEContent(pg, 0);
	}

	// inform the user if we have done a color conversion.
	if(ASBoolToBool(gbConverted) == true) 
		Console::displayString("Color transform has been done from CMYK to sRGB for text elements.");
	else 
		Console::displayString("No color transform was made.");
}

// Register the snippet
SNIPRUN_REGISTER(ACETransPDETextColorSnip, "ACE:Transform Color", 
				 "Transform color values of PDE text runs from CMYK to the corresponding sRGB")

static void TransformColor(PDEElement element)
{	
	PDEGraphicState gfx_state;

	ASInt32 pdeType = PDEObjectGetType(reinterpret_cast<PDEObject>(element));
	
	if(pdeType == kPDEText) {
		ASInt32 num_run, i;
		PDEText pde_text = (PDEText) element;
		num_run = 	PDETextGetNumRuns(pde_text);

		for (i = 0; i < num_run; i++){
			// Get graphic state of text run
			PDETextGetGState(pde_text,kPDETextRun,i,&gfx_state,sizeof(PDEGraphicState));
			// Get color space 
			ASAtom color_space = PDEColorSpaceGetName(gfx_state.fillColorSpec.space);

			char buf[128];
			if (color_space == ASAtomFromString("DeviceCMYK")){
				float colorVal[4];
				ASUns8 srcData[4], dstData[4];
			
				// Since AC_Packing_RGB8 is the destination ACE color packing code in the ACEconvertColorProfile call,
				// (where R, G, B = 0 is black. R, G, B = 255 is white), we multiply the color values by 255, and 
				// convert the values back to 0-1 scaled R G B values.
				colorVal[0] = ASFixedToFloat (gfx_state.fillColorSpec.value.color[0]);
				colorVal[1] = ASFixedToFloat (gfx_state.fillColorSpec.value.color[1]);
				colorVal[2] = ASFixedToFloat (gfx_state.fillColorSpec.value.color[2]);
				colorVal[3] = ASFixedToFloat (gfx_state.fillColorSpec.value.color[3]);
			       
				// Convert to unsigned char * (ASUns8 *) for ACE color transformation
				srcData[0] = (ASUns8) (colorVal[0] * 255.0);
				srcData[1] = (ASUns8) (colorVal[1] * 255.0);
				srcData[2] = (ASUns8) (colorVal[2] * 255.0);
				srcData[3] = (ASUns8) (colorVal[3] * 255.0);

				// Get rendering ACE intent type
				AC_RenderIntent AC_intent = SnippetRunnerUtils::GetRenderIntent(gfx_state.renderIntent);

				// Transform the color space values to the corresponding sRGB values
				ACEconvertColorProfile (CMYK_profile_K, sRGB_profile_K, (void *)srcData, (void *)dstData, 1, AC_intent, NULL);

				// Convert values back to 0-1 scales
				gfx_state.fillColorSpec.value.color[0] = FloatToASFixed (dstData[0] / 255.0);
				gfx_state.fillColorSpec.value.color[1] = FloatToASFixed (dstData[1] / 255.0);
				gfx_state.fillColorSpec.value.color[2] = FloatToASFixed (dstData[2] / 255.0);

				// We compare the color values differences here
				snprintf (buf,sizeof(buf), "(%5.3f %5.3f %5.3f %5.3f -> %5.3f %5.3f %5.3f)",
					colorVal[0], colorVal[1], colorVal[2], colorVal[3],
					dstData[0] / 255.0, dstData[1] / 255.0, dstData[2] / 255.0);
				strcat(buf, "\n");
				
				Console::displayString(buf);

				// Reset color space and graphic state
        		PDEColorSpace pdeColorSpace = PDEColorSpaceCreateFromName(DeviceRGB_K);
				gfx_state.fillColorSpec.space = pdeColorSpace;
				PDETextRunSetGState (pde_text, i, &gfx_state, sizeof(PDEGraphicState));
			
				// set global flag
				gbConverted = true;
			}
		}

		return;
	}
	// Recursively call the function
	else if (pdeType == kPDEContainer){
		PDEContent content = PDEContainerGetContent(reinterpret_cast<PDEContainer>(element));
		ASInt32 numElem = PDEContentGetNumElems(content);
		for (ASInt32 i = 0; i < numElem; i++){
			element = PDEContentGetElem(content, i);
			TransformColor(element);
		}
	}

	return;
}

/** Convert color profile using Adobe Color Engine
	@param orgColorProfile IN original color profile name
	@param newColorProfile IN new color profile name
	@param streamData IN input data stream to be transformed
	@param streamOut OUT output data stream after transformation
	@param colorCount IN the count of colors in the input data stream
	@param AC_intent IN rendering intent of input data stream
	@param clientData IN client data
*/
void 
ACEconvertColorProfile (const char *orgColorProfile, const char *newColorProfile, void *streamData, void *streamOut, 
											ASInt32 size, AC_RenderIntent AC_intent, void *clientData)
{
	AC_Transform xform = NULL;	
	AC_String rgbStr;
	AC_Profile profile;
	// Create the new Profile
	if(AC_Error_None == ACMakeString(&rgbStr, newColorProfile, NULL))
	{
		if (AC_Error_None != ACProfileFromDescription (&profile, rgbStr))
		{
			ACUnReferenceString(rgbStr);
			AVAlertNote("Failed to get the new profile description");
			return;
		} else
			ACUnReferenceString(rgbStr);
	}else{
		AVAlertNote ("Failed to make the new profile string");
		return;
	}	

	// Create a transform object and apply the transformation to the input data stream
	AC_String cmykStr;
    AC_Error err;
	if (AC_Error_None == ACMakeString(&cmykStr, orgColorProfile, NULL))
    {
		AC_Profile cmykIn;
		AC_Transform xform = NULL;     
		if (AC_Error_None != ACProfileFromDescription (&cmykIn, cmykStr))
		{
			ACUnReferenceString(cmykStr);
			AVAlertNote("Failed to get the old profile description.");
		}
		// Create a transformation object
		if (AC_Error_None != ACMakeColorTransform (&xform, cmykIn, profile, AC_intent))
		{
			ACUnReferenceProfile(cmykIn);
			ACUnReferenceString(cmykStr);
			AVAlertNote("Failed in creating transformation object.");
			return;
		}

		ACUnReferenceProfile(cmykIn);
        ACUnReferenceString(cmykStr);
		// Apply the transformation to the data stream
        err = ACApplyTransform (xform , (void *)streamData, (void *)streamOut, size, AC_Packing_CMYK8_White0,  AC_Packing_RGB8);
		ACUnReferenceTransform(xform);

		if (err != AC_Error_None){
			AVAlertNote("Failed in transformation.");
			return;
		}
	}
}
