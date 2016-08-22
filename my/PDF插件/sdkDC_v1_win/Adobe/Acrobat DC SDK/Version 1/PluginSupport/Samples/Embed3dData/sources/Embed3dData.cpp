/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2008 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/
/** 
\file Embed3dData.cpp

  - This file demonstrate how to programmatically creating 3D annotation in a PDF. 
  - The file is used in an Acrobat Plug-in sample. 

  - Program procedures:
    1. Create a new PDF with one page.
	2. Add a 3D annotation to the page.
	3. Build entries for the 3D annotation. 
		.Create 3D stream with the 3D data.
			.Embed optional JavaScript code.
			.Build optional VA key.
			.Build  optional animation dictionary.
		.Specify key-values in the annot dictionary.
			.Create the required appearance dictionary for the annot.
			 The code shows two possible ways to create a form XObject 
			 for the 3D annotation's appearance. The source can come
			 from either a pdf file that contains an image or a text watermark
			 generated on the fly.
		.Build optional activation dictionary.
	
	Important Note:
	When building the 3D stream object, if you are going to include
	optional entries in the stream dictionary, you should include those 
	optional entries right from the start. The value for those keys should 
	be indirect Cos objects. 
	
	The reason to put those entries in from the start is that once the 
	file is saved, if an optional entry is added at a later time, when 
	the file is incrementally saved, the *entire* 3D stream object 
	(including the megabytes of model data) will have to be rewritten. 
	
	The reason to make their values indirect is similar -- if the value is 
	changed for a key, the indirect-ness of the Cos object will mean that 
	only the Cos object will need to be rewritten, whereas if it were direct, 
	the entire stream object would have to be rewritten.

  - The output PDF is supposed to work with Acrobat/Adobe Reader 7.0 or newer 
	version. 
			
*********************************************************************/
// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

static const size_t BUF_SIZE = 256;
static const ASUns32 NUM_VIEWS = 6;
// external names for views
static char* VIEW_NAMES[NUM_VIEWS] = {"Left", "Top", "Front", "Right", "Bottom", "Back"};
// text used in generating the watermark
static const char* gsTextPoster = "3D Annotation";

// *** functions
ACCB1 void ACCB2 CreatePDFWith3DAnnotation(void);
PDDoc CreatePDFWithWatermark(void);
CosObj GetFormXObjectFromPosterFile(ASPathName asPosterPathName, PDDoc TargetPdDoc);
CosObj GetFormXObjectFromWatermark(PDDoc TargetPdDoc);
CosObj GetFormXObjFromWatermarkInPDF(PDDoc pdDoc);
ASBool EmbedDataTo3dAnnot(PDPage pdPage,	// page
			PDAnnot theAnnot,				// 3D annot
			ASPathName u3dPathName,			// ASPathName to the U3D file
			ASPathName* asPosterPathName,	// ASPathName to the poster file
			const char* JsFileName);		// Path to the JavaScript file
void DisplayExceptionMsg(void);
ASBool GetU3dFilePathName(ASPathName** outASPathName);
ASBool GetPosterPathName(ASPathName** outASPathName);
void BuildJsFile(const char* strFileName);
CosObj BuildFormXObj(ASPathName* asPosterPathName, PDDoc pdDoc);
CosObj BuildOneView(PDDoc pdDoc, CosObj cosAnnot, char* strName);
CosObj GetFormXObjectFromPosterFile(ASPathName asPosterPathName, PDDoc pdDocTarget);

/** 
	DisplayExceptionMsg
	------------------------------------------------------

	Function to display exception messages
*/
void DisplayExceptionMsg(void)
{
	ASInt32 iErrCode = ASGetExceptionErrorCode();
	char errBuf[BUF_SIZE];
	memset(errBuf, 0x00, BUF_SIZE);
	ASGetErrorString(iErrCode, errBuf, BUF_SIZE);
	if(strlen(errBuf))
		AVAlertNote(errBuf);
	else
		AVAlertNote("Unknown error");
}

/**
	GetU3dFilePathName
	-------------------------------------------------------
	
	Function to get the ASPathName of the U3D model file
*/

ASBool GetU3dFilePathName(ASPathName** outASPathName)
{
	AVOpenSaveDialogParamsRec dialogParams;
	AVFileFilterRec filterRec, *filterRecP;
	AVFileDescRec descRec = {"u3d", 0, 0};

	// Configure the dialog box parameters
	memset(&filterRec, NULL, sizeof(AVFileFilterRec));
	filterRec.fileDescs = &descRec;
	filterRec.numFileDescs = 1;
	filterRecP = &filterRec;
	memset(&dialogParams, NULL, sizeof(AVOpenSaveDialogParamsRec));
	dialogParams.size = sizeof(AVOpenSaveDialogParamsRec);
	dialogParams.fileFilters = &filterRecP;
	dialogParams.numFileFilters = 1;

	filterRec.filterDescription = ASTextNew();
	ASTextSetEncoded(filterRec.filterDescription, "u3d files", ASScriptToHostEncoding(kASRomanScript));
	dialogParams.windowTitle = ASTextNew();
	ASTextSetEncoded(dialogParams.windowTitle, "Select U3D file", ASScriptToHostEncoding(kASRomanScript));
	dialogParams.flags = kAVOpenSaveAllowForeignFileSystems;
	dialogParams.initialFileSys = ASGetDefaultUnicodeFileSys(); 

	ASFileSys outFileSys = NULL;
	ASBool bRet = AVAppOpenDialog(&dialogParams, &outFileSys, outASPathName, NULL, NULL);

	// We retain ownership of the ASText objects so we have to destroy them
	ASTextDestroy(filterRec.filterDescription);
	ASTextDestroy(dialogParams.windowTitle);

	return bRet;
}

/**
	GetPosterPathName
	-------------------------------------------------------
	
	Function to get the ASPathName of the poster file
*/
ASBool GetPosterPathName(ASPathName** outASPathName)
{
	AVOpenSaveDialogParamsRec dialogParams;
	AVFileFilterRec filterRec, *filterRecP;
	AVFileDescRec descRec = {"pdf", 0, 0};

	// Configure the dialog box parameters
	memset(&filterRec, NULL, sizeof(AVFileFilterRec));
	filterRec.fileDescs = &descRec;
	filterRec.numFileDescs = 1;
	filterRecP = &filterRec;
	memset(&dialogParams, NULL, sizeof(AVOpenSaveDialogParamsRec));
	dialogParams.size = sizeof(AVOpenSaveDialogParamsRec);
	dialogParams.fileFilters = &filterRecP;
	dialogParams.numFileFilters = 1;

	filterRec.filterDescription = ASTextNew();
	ASTextSetEncoded(filterRec.filterDescription, "pdf files", ASScriptToHostEncoding(kASRomanScript));
	dialogParams.windowTitle = ASTextNew();
	ASTextSetEncoded(dialogParams.windowTitle, "Select poster for the 3D annotation", ASScriptToHostEncoding(kASRomanScript));
	dialogParams.flags = kAVOpenSaveAllowForeignFileSystems;
	dialogParams.initialFileSys = ASGetDefaultUnicodeFileSys(); 

	ASFileSys outFileSys = NULL;
	ASBool bRet = AVAppOpenDialog(&dialogParams, &outFileSys, outASPathName, NULL, NULL);

	// We retain ownership of the ASText objects so we have to destroy them
	ASTextDestroy(filterRec.filterDescription);
	ASTextDestroy(dialogParams.windowTitle);

	return bRet;
}

/**
	BuildJsFile
	------------------------------------------------------------

	Function to build a js file on the fly
*/

void BuildJsFile(const char* strFileName)
{
	FILE* jsFile = fopen(strFileName, "w");
	if(jsFile) {
		fprintf(jsFile, "runtime.overrideViewChange = true\n");
		fprintf(jsFile, "activeCamera = scene.cameras.getByIndex( 0 );\n");
		fprintf(jsFile, "bounds = scene.computeBoundingBox();\n");
		fprintf(jsFile, "bounds.center = bounds.max.blend( bounds.min, .5 );\n");
		fprintf(jsFile, "diag = bounds.max.subtract( bounds.min ).length;\n");
		fprintf(jsFile, "dist =(diag * 0.5) / Math.tan(0.6 * activeCamera.fov);\n");
		fprintf(jsFile, "activeCamera.position.set3(bounds.center.x, bounds.center.y - dist, bounds.center.z);\n");
		fprintf(jsFile, "activeCamera.targetPosition.set(bounds.center);\n");
		fprintf(jsFile, "activeCamera.up.set3(0, 0, 1);\n");
		fprintf(jsFile, "activeCamera.roll = 0;\n");
		fprintf(jsFile, "scene.lightScheme = scene.LIGHT_MODE_HEADLAMP;\n");
		fprintf(jsFile, "scene.renderMode = scene.RENDER_MODE_SHADED_SOLID_WIRFRAME;\n");
		fprintf(jsFile, "\n");
		fprintf(jsFile, "\n");
		fprintf(jsFile, "\n");
		fclose(jsFile);
	}
}

/**
	CreatePDFWith3DAnnotation
	-------------------------------------------------------

	Main function to create a new PDF with a 3D annotation  
*/
ACCB1 void ACCB2 CreatePDFWith3DAnnotation(void)
{
#if WIN_PLATFORM
	const char* strJsName = "C:\\setDefaultCamera.js";
#else
	const char* strJsName = "/setDefaultCamera.js";
#endif

	ASPathName * asU3dPathName = NULL;
	// Get the U3D model file from user
	ASBool bRet = GetU3dFilePathName(&asU3dPathName);
	if(bRet && asU3dPathName != NULL) {
		ASPathName *asPosterPathName = NULL;

		// Ask user whether he wants to specify a poster to go with the annotation 
		ASInt32 iRet = AVAlert(ALERT_QUESTION, 
			"Do you want to specify a poster for the 3D annotation?",
			"Yes", "No", NULL, false);
		if(iRet == 1) {
			bRet = GetPosterPathName(&asPosterPathName);
		}
			
		// Build the JavaScript file on the fly
		BuildJsFile(strJsName);

		// Create a new PDF with one page 
		PDDoc pdDoc = PDDocCreate();                        
		ASFixedRect mediaBox = {fixedZero, ASInt32ToFixed(612), ASInt32ToFixed(792), fixedZero};  

		// Add a page to the PDF      
		PDPage pdPage = PDDocCreatePage(pdDoc, PDBeforeFirstPage, mediaBox);
		AVDoc avDoc = AVDocOpenFromPDDoc(pdDoc, NULL);

		PDAnnot theAnnot;

		// Create a 3D annotation in the page
		DURING
			// Specify BBox for the annotation
			ASFixedRect annotBox = {ASInt32ToFixed(108), ASInt32ToFixed(612 - 108),
				 ASInt32ToFixed(792 - 108), ASInt32ToFixed(108)};

			// Add new 3D annot to page
			theAnnot = PDPageAddNewAnnot(pdPage, -2, ASAtomFromString("3D"), &annotBox);
			PDAnnotSetFlags(theAnnot, pdAnnotPrint | pdAnnotReadOnly);
		HANDLER
			DisplayExceptionMsg();
			ASFileSysReleasePath(ASGetDefaultUnicodeFileSys(), *asU3dPathName);
			PDPageRelease(pdPage);
			if(asPosterPathName != NULL)
				ASFileSysReleasePath(ASGetDefaultUnicodeFileSys(), *asPosterPathName);
			return;
		END_HANDLER

		// Create the 3d annot with 3D data
		ASBool rc = EmbedDataTo3dAnnot(pdPage,
			theAnnot,			// the newly created 3D annot
			*asU3dPathName,		// ASPathName to the U3D file
			asPosterPathName,	// ASPathName to the poster
			strJsName);			// Path to the JS file

		ASFileSysReleasePath(ASGetDefaultUnicodeFileSys(), *asU3dPathName);;
		PDPageRelease(pdPage);

#if WIN_PLATFORM
		_unlink(strJsName);
#else
	    NSString *JsName = [NSString stringWithCString:strJsName encoding:NSASCIIStringEncoding];
        NSFileManager *fileManager = [NSFileManager defaultManager];
        [fileManager removeItemAtPath:JsName error:NULL];
        
#endif
		if(asPosterPathName != NULL)
			ASFileSysReleasePath(ASGetDefaultUnicodeFileSys(), *asPosterPathName);

		if(!rc) {
			return;
		}

		PDTextAnnotSetOpen(theAnnot, true);
	}
	return;
}

/**
	BuildFormXObj
	------------------------------------------------------

	Function to get Form XObject for the required "AP" entry
*/
CosObj BuildFormXObj(ASPathName* asPosterPathName, PDDoc pdDoc)
{
	CosObj formXObj;
	if(asPosterPathName != NULL) {
		formXObj = GetFormXObjectFromPosterFile(*asPosterPathName, pdDoc);
	}

	if(CosObjEqual(formXObj, CosNewNull())) {
		// retrieve the form XObject through creating a watermark on the fly
		formXObj = GetFormXObjectFromWatermark(pdDoc);
	}

	return formXObj;
}

/**
	BuildOneView
	-------------------------------------------------------

	Skeleton function to show how to construct views for the "VA" entry
*/
CosObj BuildOneView(PDDoc pdDoc, CosObj cosAnnot, char* strName) 
{
	CosDoc cosDoc = CosObjGetDoc(cosAnnot);

	CosObj viewDict = CosNewDict(cosDoc, false, 1);
	if(CosObjGetType(viewDict) == CosDict) {
		CosDictPutKeyString(viewDict, "XN", CosNewString(cosDoc, 
			false, strName, strlen(strName)));
		CosDictPutKeyString(viewDict, "Type", CosNewName(cosDoc, false, ASAtomFromString("3DView")));

		// Now you are supposed to parse the U3D file and acquire the format spec
		// such as transformation matrix for the view. Then construct the "MS", "CO", "C2W"
		// (if value for "MS" is "M"), etc. entries.
	}
	return viewDict;
}

/**
	EmbedDataTo3dAnnot
	----------------------------------------------------

	Function to create the 3d annot with embedded data	
*/
ASBool EmbedDataTo3dAnnot(PDPage pdPage,
	PDAnnot theAnnot,				// 3D annot
	ASPathName u3dPathName,			// ASPathName to the U3D file
	ASPathName* asPosterPathName,	// ASPathName to the poster file
	const char* strJsFileName)		// Path to the JavaScript file
{
    CosObj cosAnnot = PDAnnotGetCosObj(theAnnot);
    CosDoc cosDoc = CosObjGetDoc(cosAnnot);
	ASPathName jsPathName = NULL;
	ASBool bRet = true;

DURING

    // *** Create the required 3D stream dictionary      
	// Open data file
	ASFile asU3dFile = NULL;
	ASInt32 iRet = ASFileSysOpenFile(ASGetDefaultUnicodeFileSys(), u3dPathName, ASFILE_READ, &asU3dFile);
	if ((iRet != 0) || (asU3dFile == NULL)) {
		AVAlertNote("error opening 3D data file.");
		E_RETURN (false);
	}
		
	// Read data stream from the file
	ASStm fileStm = ASFileStmRdOpen(asU3dFile, 0);
	if(fileStm == NULL){
		AVAlertNote("empty 3D data stream.");
		E_RETURN (false);
	}

	// Set the stream's entries
	CosObj attrObj = CosNewDict(cosDoc, false, 1);
    CosDictPutKeyString(attrObj, "Subtype", CosNewName(cosDoc, false, ASAtomFromString("U3D")));
    CosDictPutKeyString(attrObj, "Type", CosNewNameFromString(cosDoc, false, "3D"));
	CosDictPutKeyString(attrObj, "Filter", CosNewNameFromString(cosDoc, false, "FlateDecode"));

	PDDoc pdDoc = PDPageGetDoc(pdPage);

	// Create the optional "VA" entry
	CosObj cosArray = CosNewArray(cosDoc, true, 0);
	if(CosObjGetType(cosArray) == CosArray) {
		CosDictPutKeyString(attrObj, "VA", cosArray);
		ASUns32 i = 0;
		for(ASUns32 iCounter = 0; iCounter < NUM_VIEWS; iCounter++) {
			CosObj cosOneView = BuildOneView(pdDoc, cosAnnot, VIEW_NAMES[iCounter]);
			if(CosObjGetType(cosOneView) == CosDict) {
				CosArrayPut(cosArray, i, cosOneView);
				i++;
			}
		}
	}

	// Create a new 3D stream and set it under 3DD key in the annot dictionary
    CosObj stm3D = CosNewStream(cosDoc, true, fileStm, 0, true, attrObj, CosNewNull(), -1);
   	CosDictPutKeyString(cosAnnot, "3DD", stm3D);
    ASStmClose(fileStm);
	ASFileClose(asU3dFile);

	// Embed optional JavaScript code in the 3D stream
	// Open JavaScript file
#ifdef	WIN_PLATFORM
	const char* strPathFlag = "Cstring";
#else
	const char* strPathFlag = "POSIXPath";
#endif
	jsPathName = ASFileSysCreatePathName (ASGetDefaultFileSys(),
						ASAtomFromString(strPathFlag), strJsFileName, 0);

	ASFile asJsFile = NULL;
	iRet = ASFileSysOpenFile(ASGetDefaultFileSys(), jsPathName, ASFILE_READ, &asJsFile);
	ASFileSysReleasePath (ASGetDefaultFileSys(), jsPathName);

	if ((iRet == 0) && (asJsFile != NULL)) {	
		// Read js code  from the js file and set it in the 3D stream  
		ASStm jsFileStm = ASFileStmRdOpen(asJsFile, 0);
		if (jsFileStm != NULL) {
			CosObj cosJs = CosNewDict(cosDoc, false, 1);
			CosDictPutKeyString(cosJs, "Filter", CosNewNameFromString(cosDoc, false, "FlateDecode"));
			CosObj stm3dJsCode = CosNewStream(cosDoc, true, jsFileStm, 0, true, cosJs, CosNewNull(), -1);
			CosDictPutKeyString(attrObj, "OnInstantiate", stm3dJsCode);
		}
			
	    ASStmClose(jsFileStm);
		ASFileClose(asJsFile);
	}

	// Set the optional animation style dictionary in the 3D stream
	CosObj animationDict = CosNewDict(cosDoc, false, 1);
	if(CosObjGetType(animationDict) == CosDict) {
		CosDictPutKeyString(attrObj, "AN", animationDict);
		CosDictPutKeyString(animationDict, "Type", 
			CosNewName(cosDoc, false, ASAtomFromString("3DAnimationStyle")));
		CosDictPutKeyString(animationDict, "Subtype",
			CosNewName(cosDoc, false, ASAtomFromString("Linear")));
		CosDictPutKeyString(animationDict, "PC", CosNewInteger(cosDoc, false, -1));
		CosDictPutKeyString(animationDict, "TM", CosNewInteger(cosDoc, false, 1));
	}

	// Set the optional indirect reference to the page object in the annot dictionary
    CosDictPutKeyString(cosAnnot, "P", PDPageGetCosObj(pdPage));
	// Set the optional "Contents" entry in the annot dictionary
	CosDictPutKeyString(cosAnnot, "Contents", CosNewString(cosDoc, false, "3D Model", strlen("3D Model")));

	// Create a poster for the 3D annotation
	CosObj formXObj = BuildFormXObj(asPosterPathName, pdDoc);
	if (!CosObjEqual(formXObj, CosNewNull())){
		// Build the required "AP" entry for the 3D annotation
		CosObj apprDict = CosNewDict(cosDoc, false, 1);
		CosDictPutKeyString(apprDict, "N", formXObj);
		CosDictPutKeyString(cosAnnot, "AP", apprDict);
	}

	// *** Set the optional default initial view of the 3D artwork
	// You are supposed to parse the U3D model file and retrieve the format spec
	// such as transformation matrix for the view. Then construct the "MS", "CO", "C2W"
	// (if value for "MS" is "M"), etc. entries.	

    // *** Set the optional activation dictionary 
	CosObj activationDict = CosNewDict(cosDoc, false, 1);
    if(CosObjGetType(activationDict) == CosDict) {
		CosDictPutKeyString(cosAnnot, "3DA", activationDict);
        CosDictPutKeyString(activationDict, "DIS", CosNewName(cosDoc, false, ASAtomFromString("I")));
		
		// Set the optional activation choice
		CosDictPutKeyString(activationDict, "A", CosNewName(cosDoc, false, ASAtomFromString("PO")));
   }


HANDLER
	DisplayExceptionMsg();
	bRet = false;
END_HANDLER

	return bRet;
}


/**
	GetFormXObjectFromPosterFile
	-----------------------------------------------------------------------

	Function to convert the first page of a image PDF document into a PDEForm XObject. 
	Input - ASPathName asPosterPathName is the ASPathName of a poster image PDF file.
	Input - PDDoc pdDocTarget is the PDDoc which will use the form XObject returned.
*/
CosObj GetFormXObjectFromPosterFile(ASPathName asPosterPathName, PDDoc pdDocTarget)
{
	PDDoc pdDocPoster = NULL;
	PDPage pdPage = NULL;
	CosObj contentFormXObject = CosNewNull();
	CosObj formXObject = CosNewNull();

DURING
	// Open the image PDF file
	pdDocPoster = PDDocOpen(asPosterPathName, ASGetDefaultUnicodeFileSys(), NULL, true);
	
	// Get the page containing the image 
	// The image must be on the first page
	pdPage = PDDocAcquirePage(pdDocPoster, 0);

	// Convert the image on the first page into form XObject
	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, 0);
	
	PDEContentAttrs pdeContentAttrs;
	memset(&pdeContentAttrs, 0x00, sizeof(PDEContentAttrs));
	PDEContentGetAttrs(pdeContent, &pdeContentAttrs, sizeof(pdeContentAttrs));

	CosObj contentResources = CosNewNull();
	CosDoc cosDoc = PDDocGetCosDoc(pdDocPoster);
	PDEContentToCosObj (pdeContent, 
						kPDEContentToForm,			// to Form
						&pdeContentAttrs,			// PDEContentAttrsP: no
						sizeof(pdeContentAttrs),	// attrsSize,
						cosDoc,						// COSDoc
						NULL,						// PDEFilterArrayP
						&contentFormXObject,		// resulting form COS object
						&contentResources);			// resulting resource COS object  
	
	if(!CosObjEqual(contentFormXObject, CosNewNull()) &&  
		!CosObjEqual(contentResources, CosNewNull())) {
		// Put resources into the Form XObject stream dictionary
		CosDictPutKeyString(contentFormXObject, "Resources", contentResources);

		// Set BBox key for the Form XObject according to the image page
		ASFixedRect boundingBox;  
		PDPageGetMediaBox(pdPage, &boundingBox);
		CosObj BBoxArray = CosNewArray(cosDoc, 4, false);
		CosArrayPut(BBoxArray, 0, CosNewInteger(cosDoc, false, ASFixedRoundToInt16(boundingBox.left)));
		CosArrayPut(BBoxArray, 1, CosNewInteger(cosDoc, false, ASFixedRoundToInt16(boundingBox.bottom)));
		CosArrayPut(BBoxArray, 2, CosNewInteger(cosDoc, false, ASFixedRoundToInt16(boundingBox.right)));
		CosArrayPut(BBoxArray, 3, CosNewInteger(cosDoc, false, ASFixedRoundToInt16(boundingBox.top)));

		CosDictPutKeyString(contentFormXObject, "BBox", BBoxArray);

		// Set matrix key for the Form XObject 
		ASFixedMatrix defaultMatrix;
		PDPageGetDefaultMatrix(pdPage, &defaultMatrix);
		CosObj MatrixArray = CosNewArray(cosDoc, 6, false);
		CosArrayPut(MatrixArray, 0, CosNewFixed(cosDoc, false, defaultMatrix.a));
		CosArrayPut(MatrixArray, 1, CosNewFixed(cosDoc, false, defaultMatrix.b));
		CosArrayPut(MatrixArray, 2, CosNewFixed(cosDoc, false, defaultMatrix.c));
		CosArrayPut(MatrixArray, 3, CosNewFixed(cosDoc, false, defaultMatrix.d));
		CosArrayPut(MatrixArray, 4, CosNewFixed(cosDoc, false, defaultMatrix.h));
		CosArrayPut(MatrixArray, 5, CosNewFixed(cosDoc, false, defaultMatrix.v));

		CosDictPutKeyString(contentFormXObject, "Matrix", MatrixArray);
	}

	// Make a copy of PDEFormCosobj
	formXObject = CosObjCopy (contentFormXObject, PDDocGetCosDoc(pdDocTarget), true); 

HANDLER
	DisplayExceptionMsg();
END_HANDLER
	if(pdPage) 
		PDPageRelease(pdPage);
	PDDocClose(pdDocPoster);
	
	return formXObject;
}

/**
	CreatePDFWithWatermark
	--------------------------------------------------------------

	Function to create a PDF with a watermark.  
*/
PDDoc CreatePDFWithWatermark(void)
{
	// Create a new PDF document 
	PDDoc pdDoc = PDDocCreate();
	ASFixedRect mediaBox = {fixedZero, Int16ToFixed(11 * 72), Int16ToFixed(8.5 * 72), fixedZero};
	// Insert a pdPage on which to add the watermark  
	PDPage pdPage = PDDocCreatePage(pdDoc, PDBeforeFirstPage, mediaBox);

	// Parameters for the watermark
	PDDocAddWatermarkParamsRec addWatermarkParamsRec;
	memset(&addWatermarkParamsRec, 0x00, sizeof(addWatermarkParamsRec));

	addWatermarkParamsRec.size = sizeof(addWatermarkParamsRec);
	PDPageRange pdPageRange = {0, 0, PDAllPages};
	addWatermarkParamsRec.targetRange = pdPageRange;
	addWatermarkParamsRec.zOrderTop = true;
	addWatermarkParamsRec.horizAlign = kPDHorizCenter;
	addWatermarkParamsRec.vertAlign = kPDVertCenter;
	addWatermarkParamsRec.scale = 1.0;
	
	PDDocWatermarkTextParamsRec addWatermarkTextParamsRec;
	memset(&addWatermarkTextParamsRec, 0x00, sizeof(addWatermarkTextParamsRec));

	addWatermarkTextParamsRec.size = sizeof(addWatermarkTextParamsRec);
	addWatermarkTextParamsRec.srcText = ASTextFromScriptText(gsTextPoster, kASRomanScript);
	addWatermarkTextParamsRec.pdeFont = NULL;
	// Use a system font
	// Make sure it is available on the machine
	addWatermarkTextParamsRec.sysFontName = ASAtomFromString("Courier");
	addWatermarkTextParamsRec.fontSize = 20.0f;
	addWatermarkTextParamsRec.textAlign = kPDHorizCenter; 
	PDColorValueRec pdColorValue = {PDDeviceRGB, fixedZero, fixedZero, fixedOne}; // pure blue
	addWatermarkTextParamsRec.color = pdColorValue;

DURING
	PDDocAddWatermarkFromText(pdDoc,					// target document 
							&addWatermarkTextParamsRec,	// text parameters 
							&addWatermarkParamsRec);	// parameters

	// now, the watermark is a PDEForm XObject in the new PDF file 
HANDLER
	DisplayExceptionMsg();
END_HANDLER

	return pdDoc;
}

/**
	GetFormXObjFromWatermarkInPDF
	----------------------------------------------------------------

	Function to get the Form XObject of watermark in the PDF file 
	Input - PDDoc pdDoc is the PDF file which contains the watermark
*/
CosObj GetFormXObjFromWatermarkInPDF(PDDoc pdDoc)
{
	if(pdDoc == NULL)
		return CosNewNull();

	CosObj cosObj = CosNewNull();
	ASInt32 i, numElems, pdeType, pdeType2, numSubElems;
	PDEContent pdeContent = NULL, pdeContent2 = NULL;
	PDPage pdPage = NULL;
	PDEElement pdeElement, pdeSubElem;
	PDEForm pdeForm;

DURING
	// go through the first page to find the Form XObject
	pdPage = PDDocAcquirePage(pdDoc, 0);
	pdeContent = PDPageAcquirePDEContent(pdPage, 0);
	numElems = PDEContentGetNumElems(pdeContent);
	for (i = 0; i < numElems; i++) {
		pdeElement = PDEContentGetElem(pdeContent, i); 
		pdeType = PDEObjectGetType((PDEObject) pdeElement);

		// if found, get its Cos object
		if(pdeType == kPDEForm) {
			pdeForm = (PDEForm)pdeElement;
			PDEFormGetCosObj (pdeForm, &cosObj);
			if(!CosObjEqual(cosObj, CosNewNull()))
				break;
		} else if(pdeType == kPDEContainer) {
			ASBool bFound = false;
			pdeContent2 = PDEContainerGetContent ((PDEContainer) pdeElement);
			numSubElems = PDEContentGetNumElems (pdeContent2);
			for (i = 0; i < numSubElems; i++){
				pdeSubElem = PDEContentGetElem(pdeContent2, i);
				pdeType2 = PDEObjectGetType((PDEObject) pdeSubElem);

				// if found, get its Cos object
				if(pdeType2 == kPDEForm) {
					pdeForm = (PDEForm) pdeSubElem;
					PDEFormGetCosObj (pdeForm, &cosObj);
					if(!CosObjEqual(cosObj, CosNewNull())){
						bFound = true;
						break;
					}
				}
			}
			if(bFound)
				break;
		}
	}
HANDLER
	DisplayExceptionMsg();
END_HANDLER
	
	if (pdeContent != NULL)
		PDPageReleasePDEContent(pdPage, 0);
	if (pdPage != NULL)
		PDPageRelease(pdPage);

	// return COS object of the PDEForm XObject
	return cosObj;
}

/**
	GetFormXObjectFromWatermark
	------------------------------------------------------------------------

	Function to get the Form XObject from generating a watermark
	Input - PDDoc pdDocTarget is the PDDoc which will use the form XObject returned.
*/
CosObj GetFormXObjectFromWatermark(PDDoc pdDocTarget)
{
	PDDoc pdDoc = CreatePDFWithWatermark();
	CosObj formXObject = GetFormXObjFromWatermarkInPDF(pdDoc);
	PDDocClose(pdDoc);

	return CosObjCopy(formXObject, PDDocGetCosDoc(pdDocTarget), true);
}



