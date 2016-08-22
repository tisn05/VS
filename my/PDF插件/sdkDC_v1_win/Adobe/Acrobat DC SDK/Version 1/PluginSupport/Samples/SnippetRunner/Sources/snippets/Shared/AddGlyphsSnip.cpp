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
AddGlyphsSnip.cpp

-	This snippet adds an English, Chinese, Korean, or Japanese glyph run 
to the bottom left corner of the current viewing page. It demonstrates font 
embedding/subsetting and CJK double-byte fonts support in Acrobat. Fonts 
used in this sample are CourierStd for English, AdobeMingStd-Light for Chinese, 
KozMinProVI-Regular for Japanese, and AdobeMyungjoStd-Medium for Korean.

The non-Roman fonts are available as part of the "View Adobe PDF : Asian Language 
Support" feature. If Asian Language Support is is not available in the current viewer
installation, a "System Font Not Found" error may occur when a non-English 
selection is made.

Implementation
-	Check document permissions for editing and acquire the page.
-	Create a PDEFont reference from a system font and encoding. Acrobat allows you to check font 
 policies (embed, print, preview, and editing). The font policy should be checked before 
 embedding or subsetting them in PDF. 
-	Initialize the graphics state, color space, and text matrix to be associated with the text.
-	Create the PDEGlyphRun element from the hardcoded array of characters' glyph or Unicode IDs.
-	Add the PDEGlyphRun to the contents of the page.

Usage:
-	Execute the snippet
-	Specify the language in the dialog box: 1 for English, 2 for Chinese, 3 for Japanese 
	and 4 for Korean. Default is English.
Note: To test Chinese, Korean or Japanese, the Acrobat Asian Language Pack should be installed. 
Otherwise, you may see a font or encoding missing warning message.

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"


/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/
const ASInt16 textSize = 12; //size of text to insert

const char *gRFont = "CourierStd";
const char *gCFont = "AdobeSongStd-Light";
const char *gJFont = "KozMinProVI-Regular"; 
const char *gKFont = "AdobeMyungjoStd-Medium";

// Enter appropriate font types for the above system fonts
// For OpenType fonts, a "PostScript Outlines" font is a Type 1 font
// and a "TrueType Outlines" font is a TrueType Font
const char *gRFontType = "Type1";
const char *gCFontType = "Type1";
const char *gJFontType = "Type1";
const char *gKFontType = "Type1";

/* Choose desired PDF font encoding */
/* The Identity−H and Identity−V CMaps can be used to refer to glyphs 
directly by their CIDs when showing a text string.
This works with any CID Font, independent of its character collection.
However, more general purpose encodings, for Win, Mac and UNIX,
are UTF-16 CMAPs (as well as other Unicode CMAPs). These encodings also allow you to obtain
your glyph IDs from Unicode character code mapping tables.
Use BMP (Basic Multilangual Plane) code points. They provide 4 hex digits per character, which we
divide into 2 bytes. For instance, 589E becomes { 0x58, 0x9E } in our
text matrix. 
Glyph IDs can be obtain through a variety of font/character viewing tools as well as 
Unicode mapping tables. Two Adobe tools are Adobe InDesign's glyph pallet and 
the Adobe FDK (font developer kit) TX tool */

const char * gREncName = "UniCNS-UTF16-H"; //This can be "" for non-CID font and use CIDs
const char * gCEncName = "UniGB-UTF16-H"; 
const char * gJEncName = "UniJIS-UTF16-H";
const char * gKEncName = "UniKS-UTF16-H";

//Along with encoding name, need to know if we are using
//specific encodings that end up doing things like defining
//characters as 2-bytes (even for Roman), requiring system
//encoding information to create the PDE font, etc.
const ASBool gREncFlag = true;
const ASBool gCEncFlag = true;
const ASBool gJEncFlag = true;
const ASBool gKEncFlag = true;

/* A string works for Roman text and default encoding (gREncName = ""), 
   but does not demonstrate using glyph IDs */
//unsigned char gRGlyphs[] = "Add Text";

//For CID font and gREncName = "Identity-H", CIDs for "Add Text" can be used 
/*unsigned char gRGlyphs[] = { 
	0x00, 0x22, 0x00, 0x45, 0x00, 0x45, 0x00, 0x01, 
	0x00, 0x35, 0x00, 0x46, 0x00, 0x59, 0x00, 0x55
}; */

//Courier Std is not a CID font, so the Identity encoding will not work.
// Use its Windows Unicode glyph IDs with a Unicode encoding (we chose UTF16)
// these are represented as 2-bytes per char
unsigned char gRGlyphs[] = {
	0x00, 0x41, 0x00, 0x64, 0x00, 0x64, 0x00, 0x20,
	0x00, 0x54, 0x00, 0x65, 0x00, 0x78, 0x00, 0x74
};

//Chinese text 
//Use Unicode IDs for "增加文本" from AdobeSongStd-Light font and UTF16 encoding
unsigned char gCGlyphs[] = {
	0x58, 0x9E, 0x52, 0xA0, 0x65, 0x87, 0x67, 0x2C
//alternatively, are the glyph IDs for use with Identity encoding
//	0x11, 0x39, 0x08, 0x46, 0x0E, 0xD3, 0x04, 0x27
};

// Japanese text
// Use Unicode glyph IDs for "テキストを加えなさい" for KozMinProVI-Regular
unsigned char gJGlyphs[] = { 
	0x30, 0xC6, 0x30, 0xAD, 0x30, 0xB9, 0x30, 0xC8, 
	0x30, 0x92, 0x52, 0xA0, 0x30, 0x48, 0x30, 0x6A, 
	0x30, 0x55, 0x30, 0x44 
};

// Korean text
// Use Unicode glyph IDs for "원본을 추가하십시요" for AdobeMyungjoStd-Medium
unsigned char gKGlyphs[] = { 
	0xC6, 0xD0, 0xBC, 0xF8, 0xC7, 0x44, 0x00, 0x20,
	0xCD, 0x94, 0xAC, 0x00, 0xD5, 0x58, 0xC2, 0xED,
	0xC2, 0xDC, 0xC6, 0x94
};

typedef struct GlyphData
{
	ASScript add_text_type;
	unsigned char *glyphsPtr;
	ASInt32 glyphsLength;
	const char * font;
	const char * fontType;
	const char * encodingName;
	ASBool encodingFlag;

} GlyphData;

/*********************************************************************************************/
// Populate PDEGlyphRun g_run with the glyphs in g_data->glyphsPtr 
//Pass in position values matrixH and matrixV and the pdeFont created from the sys font

void CreateGlyphRun(GlyphData g_data, PDEGlyphRunP glyphRunP, PDEFont pdefont, ASFixed matrixH, ASFixed matrixV)
{
	double posH = ASFixedToFloat(matrixH);
	double posV = ASFixedToFloat(matrixV);
	double advance;
	ASInt32 glyphLen = g_data.glyphsLength;
	unsigned char *ptr = g_data.glyphsPtr;

	//for each glyph, fill in the glyphs[i] member of the glyphRun 
	for (ASInt16 i = 0; i < glyphLen; i++) {

		ASUns32 glyphID; 

		//for our encoding, shift left and OR to get 4 hex digits of glyphID 
		//That is, reverse { 0xZZ, 0xYY } back to ZZYY
		if (ASBoolToBool(g_data.encodingFlag) == true) {	
			glyphID = *(ptr++) << 8;	
			glyphID = glyphID | *(ptr++);
		}
		else	{	
			glyphID = *(ptr++);
		}
		
		glyphRunP->glyphs[i].glyphID = glyphID;
		glyphRunP->glyphs[i].charIndices = (ASInt16*)malloc(1 * sizeof(ASInt16));
		*(glyphRunP->glyphs[i]).charIndices = i;
		glyphRunP->glyphs[i].charIndicesLen = 1;
		glyphRunP->glyphs[i].xPosition = (ASReal)posH;
		glyphRunP->glyphs[i].yPosition = (ASReal)posV;

		//calculate widths so can advance position for next glyph
		//returns width in EM space
		if (g_data.add_text_type == kASRomanScript) {
			advance = PDEFontSumWidths(pdefont, ptr-1, 1);	
		}
		else	{
			advance = PDEFontSumWidths(pdefont, ptr-2, 2);	
		}

		//advance is in Character Space Units (1000 EM units)
		//Multiply by font size to convert to user space
		advance *= textSize; 
		//convert to text space by dividing by 1000
		advance /= 1000.0;	

		//increment horizontal position for next glyph
		posH += advance;

		//add UTF16 glyph to array of Unicode characters (UTF32HE format)
		glyphRunP->uniText[i] = glyphID + 0x100;
	}
	return;	
}


/*********************************************************************************************/
void AddGlyphRun(GlyphData myGlyphData)
{

	char errorMsg[256];
	ASInt32 errorCode = 0;
	PDSysFont	sysFont = NULL;
	PDSysEncoding sysEncoding =  NULL;
	ASFixedRect cropBox;

	// Get the currently displayed page and get its crop box.
	CDocument document;

	PDDoc pdDoc = (PDDoc)document;
	if(pdDoc == NULL){
		return;
	}

	// Check permissions - regular first, and then if need be Reader-Extension (UB)
	void *pSecData = PDDocGetSecurityData( pdDoc );
	if (PDDocPermRequest( pdDoc, PDPermReqObjDoc, PDPermReqOprModify, pSecData ) != PDPermReqGranted)
	{
		if (PDDocPermRequestNoUB( pdDoc, PDPermReqObjDoc, PDPermReqOprModify, pSecData ) != PDPermReqGranted)
			AVAlertNote( "Required permissions (Doc Modify) not available unless Reader-Extensions are removed." );
		else
			AVAlertNote( "Required permissions (Doc Modify) not available" );	
		return;
	}
	//page is owned by CDocument object. Don't release it.
	PDPage pdPage = (PDPage)document;
	if (pdPage == NULL){
		return;
	}

	// get the page crop box
	PDPageGetCropBox(pdPage, &cropBox);

	// Initialize the font descriptor then create the font reference.
	PDEFont pdeFont = NULL;
	PDEFontAttrs pdeFontAttrs;
	ASUns32 fontCreateFlags;
	memset(&pdeFontAttrs, 0, sizeof(pdeFontAttrs));

	DURING
			// Try find the matching system font
			pdeFontAttrs.name = ASAtomFromString(myGlyphData.font);

			if ((myGlyphData.fontType == "TrueType") || (myGlyphData.fontType == "Type1")) {
				sysFont = PDFindSysFont(&pdeFontAttrs, sizeof(pdeFontAttrs), 0); 
				}
			else  {
				sysFont = PDFindSysFont(&pdeFontAttrs, sizeof(pdeFontAttrs), 
															kPDSysFontMatchFontType);
			}

			if (sysFont==NULL){
				AVAlertNote("Can't find matching system font.");
				E_RTRN_VOID;
			}

			// Get the font attributes
			PDSysFontGetAttrs(sysFont, &pdeFontAttrs, sizeof(pdeFontAttrs));

			// Create the PDE font from the system font.
			// Check the font policy whether it is allowed to print, preview, or edit
			// Based on the font permission policy, decide whether to embed/subset the font
			// If it is double-byte font, embed and subset. If it is a roman font and the editable bit is set, 
			// embed the whole font
			// See AddTextSnip.cpp for info on font permissions.

			// if editing OK
			if ((pdeFontAttrs.protection & kPDEFontNoEditableEmbedding) == 0) {
				if (myGlyphData.add_text_type == kASRomanScript) {
					//Embed the font for Roman
					fontCreateFlags = kPDEFontCreateToUnicode|kPDEFontCreateEmbedded;
				}
				else {
					// Embed and subset the font 
					fontCreateFlags = kPDEFontCreateToUnicode|kPDEFontCreateEmbedded|kPDEFontWillSubset;
				}
			}
			else if ((pdeFontAttrs.protection & kPDEFontNoEmbedding) == 0) {
				// Print and Preview Embedding OK, Editing is NOT OK
				// Subset the embedded font
				fontCreateFlags = kPDEFontCreateToUnicode|kPDEFontCreateEmbedded|kPDEFontWillSubset;		
			}
			else {
				fontCreateFlags = kPDEFontDoNotEmbed;
			}

		if (myGlyphData.add_text_type != kASRomanScript) { //if CMAP font (in this case)
			sysEncoding = PDSysEncodingCreateFromCMapName(ASAtomFromString(myGlyphData.encodingName));
			pdeFont = PDEFontCreateFromSysFontAndEncoding(sysFont, sysEncoding, 
											pdeFontAttrs.name, fontCreateFlags);
		}
		else	{ 
			pdeFont = PDEFontCreateFromSysFont(sysFont, fontCreateFlags);
			}

	HANDLER
		ASGetErrorString(ASGetExceptionErrorCode(), errorMsg, 256);
		AVAlertNote (errorMsg);
		return;
	END_HANDLER

	volatile PDEContent pdeContent = NULL;
	volatile PDEText pdeText = NULL;
	PDEColorSpace pdeColorSpace = PDEColorSpaceCreateFromName(ASAtomFromString("DeviceGray"));
	PDEGraphicState gState;
	ASFixedMatrix textMatrix;

	// The graphics state controls the various style properties of the text
	// including color, weight, and so forth.
	memset(&gState, 0, sizeof(PDEGraphicState));
	gState.strokeColorSpec.space = gState.fillColorSpec.space = pdeColorSpace;
	gState.miterLimit = fixedTen;
	gState.flatness = fixedOne;
	gState.lineWidth = fixedOne;

	// Fill out the text matrix, which determines the point
	// size of the text and where it will be drawn on the page.
	memset (&textMatrix, 0, sizeof(textMatrix));
	textMatrix.a = ASInt16ToFixed(textSize);
	textMatrix.d = ASInt16ToFixed(textSize);
	textMatrix.h = cropBox.left + fixedThirtyTwo;
	textMatrix.v = cropBox.bottom + fixedThirtyTwo;

	PDEGlyphRun myGlyphRun;

	PDEGlyphDescriptionP glyphs = (PDEGlyphDescriptionP)malloc(myGlyphData.glyphsLength * sizeof(PDEGlyphDescription));
	ASUTF32Val *uniText = (ASUTF32Val*)malloc(myGlyphData.glyphsLength * sizeof(ASUTF32Val));
	memset(glyphs, 0, myGlyphData.glyphsLength * sizeof(PDEGlyphDescription));
	myGlyphRun.glyphs = glyphs;
	myGlyphRun.glyphLen = myGlyphData.glyphsLength; 
	myGlyphRun.uniText = uniText;
	myGlyphRun.uniTextLen = myGlyphData.glyphsLength; 


	DURING
		//Fill myGlyphRun.glyphs array with the data from myGlyphData and pdeFont to use in PDETextAddGlyphs
		CreateGlyphRun(myGlyphData, &myGlyphRun, pdeFont, textMatrix.h, textMatrix.v);
	
		// Create a new PDEText element and add a kPDETextRun object to it.
		pdeText = PDETextCreate();

		PDETextAddGlyphs(pdeText, kPDETextRun, 0, &myGlyphRun,
					pdeFont, &gState, sizeof(gState), 0, 0, &textMatrix, NULL);

		// Acquire the PDEContent associated with the page.
		pdeContent = PDPageAcquirePDEContent(pdPage, gExtensionID);

		// Insert text element into page content.
		PDEContentAddElem(pdeContent, kPDEAfterLast, (reinterpret_cast<PDEElement>(pdeText)));

		// Commit the changes to the PDEContent.
		PDPageSetPDEContent(pdPage, gExtensionID);

		/* Subset the font.  This must be done
		* after the text block has been created so that we know what characters
		* to subset.  In addition, subsetting the font must occur after the page
		* contents have been set.
		*/

		if (fontCreateFlags == (kPDEFontCreateEmbedded|kPDEFontWillSubset)) {
			PDEFontSubsetNow(pdeFont, PDDocGetCosDoc(pdDoc));
		}
		else if (fontCreateFlags == kPDEFontCreateEmbedded) {
			PDEFontEmbedNowDontSubset(pdeFont, PDDocGetCosDoc(pdDoc));
		}
		// Advertise that we changed the contents so the viewer redraws the
		// page and other clients can re-acquire the page contents if needed.
		PDPageNotifyContentsDidChange(pdPage);

	HANDLER
		// Store the error code.
		errorCode = ASGetExceptionErrorCode();
	END_HANDLER

	// Release any objects we may have created or acquired.
	// Note : PDERelease correctly handles NULL, so we don't
	// need to test for valid objects.
	if (pdeFont != NULL) {
		PDERelease((PDEObject)pdeFont);
	}
	if (pdeText != NULL){
		PDERelease((PDEObject)pdeText);
	}
	if (pdeColorSpace != NULL){
		PDERelease((PDEObject)pdeColorSpace);
	}
	if (pdeContent != NULL){
		PDPageReleasePDEContent(pdPage, gExtensionID);
	}
	if (sysEncoding !=NULL){
		PDERelease((PDEObject)sysEncoding);
	}

	// If we were thrown an error, let the user know.
	if (errorCode != 0) {
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);
		AVAlertNote (errorMsg);
	}
}

/** This snippet adds English, Chinese, Korean, or Japanese text 
to the bottom left corner of the current viewing page. It demonstrates font 
embedding/subseting and CJK double-byte fonts support in Acrobat. Fonts 
used in this sample are Verdana for English, SimSun for Chinese, Batang for 
Korean, and MS Mincho for Japanese.

Implementation details
<ul>
<li>	Acquire the page.
<li>	Create a PDEFont reference from a system font. Acrobat 6.x allows to check font 
 policies (embed, print, preview, and editing). The font policy should be checked before 
 embedding or subset them in PDF. 
<li>	Initialize the graphics state, color space, and text matrix to be associated with the text.
<li>	Create the PDEText element and add it to the contents of the page.
</ul>
	@param pointer to GlyphData struct, containing string, encoding
	and font.
	@see PDFindSysFont
	@see PDSysFontGetAttrs
	@see PDEFontCreateFromSysFont
	@see PDEFontCreateFromSysFontAndEncoding
	@see PDEColorSpaceCreateFromName
	@see PDETextCreate
	@see PDETextAddGlyphs
	@see PDPageAcquirePDEContent
	@see PDEContentAddElem
	@see PDPageSetPDEContent
	@see PDEFontSubsetNow
	@see PDEFontEmbedNowDontSubset
	@see PDPageNotifyContentsDidChange
	@see PDERelease
	@see PDPageReleasePDEContent
	@see PDPageRelease
	@see PDDocPermRequest
	@see PDDocGetSecurityData
	@see ASGetExceptionErrorCode
	@see ASGetErrorString
*/
void AddGlyphsSnip (ParamManager * thePM)
{	
	ASInt32 textType; //default text type is Roman (param = 1)
	thePM->getNextParamAsInt(textType);
	GlyphData theGlyphData;

switch (textType) {
	case 1:
		theGlyphData.add_text_type = kASRomanScript;
		theGlyphData.glyphsPtr = gRGlyphs;

		// if entered as one-byte, glyphsLength = sizeof(gRGlyphs) 
		// if entered as a string, glyphsLength = sizeof(gRGlyphs)-1
		// for two-byte, here is glyphsLength
		theGlyphData.glyphsLength = sizeof(gRGlyphs) / 2; 
		theGlyphData.font = gRFont;
		theGlyphData.fontType = gRFontType;
		theGlyphData.encodingName = gREncName;
		theGlyphData.encodingFlag = gREncFlag;
		break;
	case 2:
		theGlyphData.add_text_type=kASSimplifiedChineseScript;
		theGlyphData.font = gCFont;
		theGlyphData.glyphsPtr = gCGlyphs;
		theGlyphData.glyphsLength = sizeof(gCGlyphs) / 2;
		theGlyphData.fontType = gCFontType;
		theGlyphData.encodingName = gCEncName;
		theGlyphData.encodingFlag = gCEncFlag;
		break;
	case 3:
		theGlyphData.add_text_type=kASJapaneseScript;
		theGlyphData.font = gJFont;
		theGlyphData.glyphsPtr = gJGlyphs;
		theGlyphData.glyphsLength = sizeof(gJGlyphs) / 2;
		theGlyphData.encodingName = gJEncName;
		theGlyphData.encodingFlag = gJEncFlag;
		break;
	case 4:
		theGlyphData.add_text_type=kASKoreanScript;
		theGlyphData.font = gKFont;
		theGlyphData.glyphsPtr = gKGlyphs;
		theGlyphData.glyphsLength = sizeof(gKGlyphs) / 2;
		theGlyphData.fontType = gKFontType;
		theGlyphData.encodingName = gKEncName;
		theGlyphData.encodingFlag = gREncFlag;
		break;
	default:
		AVAlertNote("Select the text type (1-4)");
		return;
	}

	AddGlyphRun(theGlyphData); //, &theGlyphRun);
}


SNIPRUN_REGISTER_WITH_DIALOG(AddGlyphsSnip, "PDE Layer:Text:Add Glyphs",
							 "Allows you to add a glyph run as an footer into the content layer of the \
current page. \n Select the language~font:\n\
1, Roman (CourierStd)\t\
2, Chinese (AdobeSongStd-Light)\n\
3, Japanese (KozMinProVI-Regular)\t\
4, Korean (AdobeMyungjoStd-Medium)","1")


