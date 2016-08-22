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

 RemoveEmbeddedFontSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

//Keys as atoms
#define Roman_K ASAtomFromString("Roman")

//Keys as strings
#define FontDescriptor_KStr "FontDescriptor"
#define FontName_KStr "FontName"
#define BaseFont_KStr "BaseFont"
#define CharSet_KStr "CharSet"

static ASBool PDFontIsSubset(PDFont fontP);
static ASAtom PDEFontGetEncoding (PDEFont pdeFont);
static ASAtom PDEFontGetCharSet (PDEFont pdeFont);
static ACCB1 ASBool ACCB2 EnumDocPDFonts (PDFont font, PDFontFlags *fontFlags, void* clientData);
ACCB1 void ACCB2 RemoveEmbeddedFontsSnip(void *data);

const char gAlert1[200] = "The following embedded fonts have been permanently removed:";
const char gAlert2[200] = "If you don't have similar system fonts, this file may not be displayed properly. To avoid possible displaying problems, please save as a different file.";
/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/**  This snippet removes embedded Roman fonts from a PDF file.
 
	 An embedded font can be safely removed if and only if the font:
		-- is not multi-byte AND
		-- encoding is not "Identity" (avoid encoding mismatching) AND
		-- charset is Roman

	 Multi-byte fonts are not considered removable here because of the complication of the
	 fonts embedding restriction policy and the possible file rendering and printing problem.   
	 Type2 CIDFonts (atually TrueType fonts) are not considered removeable because of the possible
	 encoding mismatching.

	 Implementation:
		-- enumerate all PDFonts used in PDF
		-- if any removable embedded fonts exist, remove the font stream. If the embedded font is a subset, 
		-- strip off the subset tag from the BaseFont entry and FontName entry of the font descriptor.
		-- save as a different file
	 Usage:
		1. open a PDF file
		2. run the snippet from PD Layer > Fonts > Remove Embedded Fonts
		3. if there exist removable embedded fonts, a name list of removed fonts will be displayed 
		4. if you don't have similar system fonts, this file may not be displayed properly. 
		   To avoid possible document display problems, save as a different file. 
	@testfile TwoColumnTaggedDoc.pdf
	@requires pdf with embedded font
	@see PDDocGetNumPages
	@see PDDocEnumFonts
	@see PDFontIsEmbedded
	@see PDFontGetCosObj
	@see PDEFontCreateFromCosObj
	@see PDEFontGetEncoding
	@see PDEFontIsMultiByte
	@see CosObjGetDoc
	@see CosDictGetKeyString
	@see CosNameValue
	@see CosObjGetType
	@see CosDictRemoveKeyString
	@see PDEFontGetAttrs
	@see CosArrayGet
	@see CosDictRemoveKeyString
*/
void RemoveEmbeddedFontSnip()
{
	ASInt32 firstPage = 0, lastPage = 0;
	char buffer[5000], alertStr[5500];

	buffer[0] = '\0';
	memset (buffer, 0, sizeof(buffer));
	memset (alertStr, 0, sizeof(alertStr));

	
	CDocument document;
	PDDoc activePDDoc = (PDDoc)document;
	if(activePDDoc == NULL)
		return;
	ASInt32 pageNum = PDDocGetNumPages (activePDDoc); 
	
	lastPage = pageNum - 1;

	// Create a PDFont enumeration call back
	PDFontEnumProc PDFontEnumProcCB = ASCallbackCreateProto(PDFontEnumProc, &EnumDocPDFonts);
	// Enumerate all embedded fonts from the first page to the last
	PDDocEnumFonts (activePDDoc, firstPage, lastPage, PDFontEnumProcCB, (void*) buffer, NULL, NULL);
	// Destroy the PDFont enumeration call back
	ASCallbackDestroy((void*)PDFontEnumProcCB);

	if (buffer[0] != '\0')
	{
		#if MAC_PLATFORM
			snprintf (alertStr,sizeof(alertStr), "%s\r\r%s\r\r%s", gAlert1, buffer, gAlert2);
		#else
			snprintf (alertStr,sizeof(alertStr), "%s\n\n%s\n\n%s", gAlert1, buffer, gAlert2);
		#endif

		Console::displayString(alertStr);
	}
	else
	{
		Console::displayString("No embedded fonts to be removed.");
	}
}

/* EnumDocPDFontsSnip
** ------------------------------------------------------
** An embedded font can be safely removed if and only if the font:
**		-- is not multi-byte AND
**      -- encoding is not "Identity" (avoid encoding mismatching) AND
**		-- charset is Roman
** Multi-byte fonts are not considered removable here because of the complication of the
** fonts embedding restriction policy and the possible file rendering and printing problem.   
** Type2 CIDFonts (atually TrueType fonts) are not considered removeable because of the possible
** encoding mismatching.
**
** If subset  fonts, the initial tags for the fontName entry in the descriptor and
** the BaseFont entry should be stripped off.
** 
** In: PDFont - font
** Out: clientdata - names of the fonts removed
** 
*/
static ACCB1 ASBool ACCB2 EnumDocPDFonts (PDFont font, PDFontFlags *fontFlags, void* clientData)
{
	CosObj	fontObj;
	CosObj	fontDesc;
	PDEFont pdeFont = NULL;
	CosDoc cosDoc;
	ASAtom	pdeFontEncoding;
	const char *pdeFontEncodingName;
	ASBool isMultiByte = false;
	ASBool isIdentityEncoding = false;
	ASBool isRoman = false;
	ASBool hasRemoved = false;

	// If the PDFont is embedded, continue
	if (font && PDFontIsEmbedded (font))
	{
		// Get the equivalent PDFEFont by creating a PDEFont from PDFont
		fontObj = PDFontGetCosObj(font);
		pdeFont = PDEFontCreateFromCosObj(&fontObj);
		
		// Obtain the encoding name attribute from the pdefont
		pdeFontEncoding = PDEFontGetEncoding(pdeFont);
		pdeFontEncodingName = ASAtomGetString(pdeFontEncoding);
		ASInt32 error = 0;
	DURING

		isMultiByte = PDEFontIsMultiByte(pdeFont);
		isIdentityEncoding = (strstr(pdeFontEncodingName, "Identity") != NULL);
		isRoman = (PDEFontGetCharSet (pdeFont) == Roman_K );

		// For simplicity, we only remove the fonts that are:
		if (!isMultiByte &&					// not multi-byte
			!isIdentityEncoding &&			// non-identity encoding name
			isRoman )						// Roman charSet
		{			
			// Get the font descriptor entry
			cosDoc = CosObjGetDoc(fontObj);
			fontDesc = CosDictGetKeyString(fontObj, FontDescriptor_KStr);

			char *newFontNameOrg = new char[128];
			newFontNameOrg[0] = '\0';
			const char	*fontName = NULL;
			CosObj fontNameObj = CosDictGetKeyString(fontDesc, FontName_KStr);
			fontName = CosCopyNameStringValue(fontNameObj, NULL);

			/* Remove the embedded font below */
			fontDesc = CosDictGetKeyString(fontObj, FontDescriptor_KStr);
			if (CosObjGetType(fontDesc) == CosDict) 
			{
			// The embedded font stream can be stored under one of three key names: FontFile, FontFile2, FontFile3. 
			// Just call CosDictRemoveKeyString on each possible value; if the key doesn't exist, the routine does nothing.			
				CosDictRemoveKeyString(fontDesc, "FontFile");
				CosDictRemoveKeyString(fontDesc, "FontFile2");
				CosDictRemoveKeyString(fontDesc, "FontFile3");

				// Remove the initial subset tag from the fontName entry in the descriptor
				if (PDFontIsSubset(font)) 
				{
					const char	*newFontName;
					CosObj newFontNameObj;
					// Get font name and strip off initial tag such as "ABCDEF+" from the fontname
					// entry of the descriptor
					if (strlen(fontName) > 7 && fontName[6] == '+') 
					{
						newFontName = fontName+7;
						newFontNameObj = CosNewNameFromString(cosDoc, false, newFontName); // split out
						CosDictPutKeyString(fontDesc, FontName_KStr, newFontNameObj); //updated
						strncpy (newFontNameOrg, newFontName,sizeof(newFontNameOrg));
					}
					// Remove the CharSet key/value pair
					CosDictRemoveKeyString(fontDesc, CharSet_KStr);
				}
			}
			
			// Remove the initial subset tag from the BaseFont entry
			if (PDFontIsSubset(font))  
			{
				// remove the tag (like "ABCDEF+") from the BaseFont entry
				const char	*fontName, *newFontName;

				CosObj fontKeyValue = CosDictGetKeyString(fontObj, BaseFont_KStr);
				fontName = CosCopyNameStringValue(fontKeyValue, NULL);

				if (strlen(fontName) > 7 && fontName[6] == '+') 
				{
					newFontName = fontName+7;	
					CosObj newFontNameKeyValue = CosNewNameFromString(cosDoc, false, newFontName);
					CosDictPutKeyString(fontObj, BaseFont_KStr, newFontNameKeyValue);
				}
			}

			// Get the fontname passed
			if (strstr((char *)clientData, newFontNameOrg) != NULL ||
				strstr((char *)clientData, fontName) != NULL)
				hasRemoved = true;
			
			if (!hasRemoved){
				if (newFontNameOrg)
					strcat ((char *)clientData, newFontNameOrg);
				else if (fontName)
					strcat ((char *)clientData, fontName);
				strcat ((char *)clientData, "   ");
				hasRemoved = false;
			}
			delete newFontNameOrg;
		}		
	HANDLER
		error = ERRORCODE;
	END_HANDLER

		if (error)
			ASRaise (error);
	}	
	return true;
}

/* PDEFontGetEncoding
** ------------------------------------------------------
**
** Get the encoding attribute of a pdefont
*/
static ASAtom PDEFontGetEncoding (PDEFont pdeFont)
{
	PDEFontAttrs	attrs;
	if (pdeFont) {
		PDEFontGetAttrs (pdeFont, &attrs, sizeof(attrs));
		return attrs.encoding;
	}
	return ASAtomNull;
}


/* PDEFont_GetCharSet
** ------------------------------------------------------
**
** Get the charSet attribute of a pdefont. 
*/
static ASAtom PDEFontGetCharSet (PDEFont pdeFont)
{
	PDEFontAttrs	attrs;
	if (pdeFont) {
		PDEFontGetAttrs (pdeFont, &attrs, sizeof(attrs));
		return attrs.charSet;
	}
	return ASAtomNull;
}

/* PDFontIsSubset
** ------------------------------------------------------
**
** Check if a PDFont is subsetted. Return true if it is.
*/
static ASBool PDFontIsSubset(PDFont fontP)
{
	CosObj		fontObj = PDFontGetCosObj(fontP);
	CosObj		nameObj = CosDictGetKeyString(fontObj, "BaseFont");
	CosObj		descFontArray;
	char		*name; //updated from const char *

	if (CosObjGetType(nameObj) == CosNull)
		return false;

	// check if the base font has initial "ABCDEF+"
	name = CosCopyNameStringValue(nameObj, NULL);
	if (strlen(name) > 7 && name[6] == '+') {
		int i;
		for (i = 0; i < 6; i++) {
			if (name[i] < 'A' || name[i] > 'Z') {
				ASfree(name);
				return false;
			}
		}
		ASfree(name);
		return true;
	}

	// Try descendant fonts
	descFontArray = CosDictGetKeyString (fontObj, "DescendantFonts");
	if (CosObjGetType(descFontArray) == CosArray) {
		CosObj descFont = CosArrayGet (descFontArray, 0);
		if (CosObjGetType(descFont) == CosDict) {
			CosObj descFontBaseFont = CosDictGetKeyString (descFont, "BaseFont");
			if (CosObjGetType(descFontBaseFont) != CosNull) {
			//	ASAtom fontNameAtom = CosNameValue (descFontBaseFont);
				name = CosCopyNameStringValue(descFontBaseFont, NULL); // ASAtomGetString(fontNameAtom);
				if (name[0] == '@') {
					char *oldFontName = name;
					name = &(oldFontName[1]);
				}
				if (strlen(name) > 7 && name[6] == '+') {
					int i;
					for (i = 0; i < 6; i++) {
						if (name[i] < 'A' || name[i] > 'Z')
							ASfree(name);
							return false;
					}
					ASfree(name);
					return true;
				}
			}
		}
	}

	ASfree(name);
	return false;
}

SNIPRUN_REGISTER(RemoveEmbeddedFontSnip, "PD Layer:Fonts:Remove Embedded Fonts", "Removes Roman fonts that are safely removable.")

