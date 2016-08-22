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

  DocMetadataSnip.cpp

 - Acrobat's metadata features will be demonstrated in two aspects:

   1. Document level metadata, which describes document content or
      use, such as title, creator, author, creation time, etc.

 	 - get (export) metadata, which allows user to obtain the default
 	   metadata in the form of XML file.
 	 - set (import) metadata, which allows user to replace the default
 	   document level metadata by the user specified XML file.
 	 - extend existing metadata, which allows user to extend metadata
 	   by adding the base URL in the doc.

 		a. search the base URLs in doc, and display them in UI
 		b. if there is a base URL, add a baseURL metadata key, otherwise, add the user entered base URL.

   2. Metadata Dialog UI in Acrobat 5.0 is implemented in touch-up
      plug-in using ADM. A similar user interface is implemented to
      demonstrate usage.

 - Since the Metadata Dialog UI in Acrobat 5.0 is implemented in the
   touch-up plug-in, the touch-up plug-in must be loaded to use this
   sample plug-in. If you have a PDF browser plug-in such as Enfocus
   PDF object browser installed, you can examine the metadata added
   from the PDF object browser as well.
 - NOTE: 
   This snippet is converted from the DocMetadata plug-in sample with 
   a couple of files combined.
   NEW in Acrobat 6.0: PDDocSetXAPMetadataProperty is provided to set XMP
   metadata directly.
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"
#include <assert.h>

#ifdef WIN_PLATFORM
#include "resource.h"
#else
#include "MacResource.h"
#endif

// Maximum XML file size allowed. Extend this constant accordingly if needed
#define metaDataLength 100000
#define URI_K ASAtomFromString("URI")
char * metadata = new char[metaDataLength] ;
char gBaseURL[128];

ASErr ASAPI AdmMetaDataDialog(void * data);
ACCB1 void ACCB2 GetDocMetaData(char * data);

/** CosCStringValue
** ----------------------------------------------------------------------
** Filter the string
*/
static char *CosCStringValue(char *cBuf, CosObj coValue, int nLen)
{
	Int32 nCount;
	char *c = CosStringValue(coValue, &nCount);

	if (nCount > nLen - 1)
		nCount = nLen - 1;

	while(nCount--)
		*cBuf++ = *c++;
	*cBuf = '\0';

	return cBuf;
}
/** CosObjIsValid
** ----------------------------------------------------------------------
** Check if a non-null given cos object type
*/
static ASBool CosObjIsValid(CosObj co, CosType ct)
{
	return !(  (CosObjGetType(co) == CosNull) || CosObjGetType(co) != ct);
}

/** PDDocGetBaseURI
** ----------------------------------------------------------------------
** returns true if the pdd has a Base URI. If so, a
** pointer to the base URI is returned in the cBase variable. If cBase ==
** NULL or nLen == 0 on entry, PDDocGetBaseURI returns false. nLen is expected
** to be the length of the buffer pointed to by cBase. 
*/
ACCB1 ASBool ACCB2 PDDocGetBaseURI(PDDoc pdd, char *cBase, Uns16 nLen)
{
	CosObj coDict, coBase;

	/* Get the catalog. */
	coDict = CosDocGetRoot(PDDocGetCosDoc(pdd));
	if (!CosObjIsValid(coDict, CosDict))
		return false;

	/* Get the URI dictionary. */
	coDict = CosDictGet(coDict, URI_K);
	if (!CosObjIsValid(coDict, CosDict))
		return false;

	/* Get the Base key. */
	coBase = CosDictGetKeyString(coDict, "Base");
	if (!CosObjIsValid(coBase, CosString))
		return false;

	CosCStringValue(cBase, coBase, nLen);
	return true;
}

/** 
** ----------------------------------------------------------------------
** Obtain the existing document level metadata if there is.
** Return NULL if there isn't, return the metadata buffer string if there is.
*/
ACCB1 void ACCB2 GetDocMetaData(char *data)
{
	CDocument document;
	PDDoc activePDDoc = (PDDoc)document;
	if(activePDDoc == NULL)
		return; 

	ASText metaData = NULL;
	char* metadataString;
DURING

	ASInt32 metaStrLen;

	// Get the doc level metadata from doc info dictionary
	metaData = PDDocGetXAPMetadata (activePDDoc);
	if (!metaData)
			E_RTRN_VOID;

	metadataString = ASTextGetPDTextCopy (metaData, &metaStrLen);
	strncpy (data, metadataString, metaDataLength - 1);

HANDLER
	char buf[256];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, 255));
END_HANDLER
	if (metaData)
		ASTextDestroy (metaData); // User is responsible to free ASText
	if(metadataString)
		ASfree(metadataString);
}


/** setDocMetaData
** ----------------------------------------------------------------------
** Add XML metadata to PDF from input file. Replace the existing metadata if these is.
** Return TURE if successful, otherwise, return FALSE.
*/
ACCB1 ASBool ACCB2 SetDocMetaData(char *fileStr)
{
	CDocument document;
	PDDoc activePDDoc = (PDDoc)document;
	if(activePDDoc == NULL)
		return false;

	ASText asTextXML = NULL;


DURING
	asTextXML=ASTextFromEncoded (fileStr, AVAppGetLanguageEncoding());

	if (asTextXML){
		// Replace the doc level metadata by the input XML metadata
		// XML syntax validation is also checked in the API
		PDDocSetXAPMetadata (activePDDoc, asTextXML);
	}
	else
		E_RETURN (false);

HANDLER

	char * buf = new char[256];

	AVAlertNote(ASGetErrorString(ERRORCODE, buf, 255));
	delete buf;
	if (asTextXML)
		ASTextDestroy (asTextXML);
	return (false);
END_HANDLER

	ASTextDestroy (asTextXML);
	return true;
}

/** Set metadata properties with Acrobat 6.0 APIs, PDDocSetXAPMetadataProperty. 
     Similarly, using PDDocGetXAPMetadataProperty method to obtain certain property with 
     specified namespace, path, and prefix. Acrobat 5.0 is the first version that supports XMP metadata. But 
     in v5.0,  the XMP metadata properties can only be get or set indirectly through PDDocInfo get/set APIs. */
ASBool SetMetaProperty(char *URLkey)
{
	CDocument document;
	PDDoc activePDDoc = (PDDoc)document;
	if(activePDDoc == NULL)
		return false;
	ASText namespaceName = ASTextFromPDText("http://ns.adobe.com/pdfx/1.3/");
	ASText namespacePrefix = ASTextFromPDText("pdfx");
	ASText path = ASTextFromPDText("BaseURL");
	//ASText newValue = ASTextFromPDText(gBaseURL);
	ASText newValue = ASTextFromPDText(URLkey);
	
DURING
	PDDocSetXAPMetadataProperty (activePDDoc,
                                    namespaceName,
                                    namespacePrefix,
                                    path, 
                                    newValue);

HANDLER
	char buf[256];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, 255));
	return false;
END_HANDLER
	return true;
}

#if MAC_PLATFORM
//fixing link error in MSL C.PPC.lib
extern "C" int GetToolTrapAddress ()
{ return 0; }
#endif

/**
   OpenSavePDF utility functions
*/
bool OpenFile(char * filetype, char *data, PDDoc *docs, ASInt32 *numDocs, ASFile *asfileP);
bool SaveFile(PDDoc doc, char * filetype, char *data);
ASBool MyAuthProc(PDDoc doc);
ASPathName PlatformOpenForWriteProc(char * filename);
void ReadXMLFile(ASFile *printFile, char *data);
void WriteXMLFile(ASFile *printFile, char *data);
#define MAX_NUM_OPENFILE 10

/** Implementation of OpenSavePDF utilities */
/* openFile
** ----------------------------------------------------------------------
** Open a file by prompting a file open dialog.
** Input Param: filetype - used to specify the file extension
** Output param: data - a buffer that stores the file
** Return: true if successful, false otherwise
*/
bool OpenFile(char * filetype, char *data, PDDoc *docs, ASInt32 *numDocs, ASFile *fP)
{
	AVOpenSaveDialogParamsRec dialogParams;
	ASPathName *pathNames;
	ASUns16 numPathnames;
	ASFileSys fileSys = NULL;

	AVFileFilterRec filterRec, *filterRecP;
	AVFileDescRec descRec;

	filterRecP = &filterRec;
DURING
	memset(&dialogParams, 0, sizeof(dialogParams));
	filterRec.filterDescription = ASTextNew();

	ASTextSetEncoded(filterRec.filterDescription, filetype, AVAppGetLanguageEncoding());

	filterRec.fileDescs = &descRec;
	filterRec.numFileDescs = 1;

	strcpy(descRec.extension, filetype);
	descRec.macFileType = 0;
	descRec.macFileCreator = 0;

	dialogParams.size = sizeof(dialogParams);
	if (!strcmp(filetype, "PDF"))
		dialogParams.flags = kAVOpenSaveAllowMultiple;
	else
		dialogParams.flags = kAVOpenSaveAllowAllFlag;
	dialogParams.parentWindow = NULL;
	dialogParams.windowTitle = NULL;
	dialogParams.actionButtonTitle = NULL;
	dialogParams.initialFileSys = NULL;
	dialogParams.initialPathName = NULL;
	dialogParams.initialFileName = ASTextNew();
	dialogParams.fileFilters = &filterRecP;
	dialogParams.numFileFilters = 1;
	dialogParams.settingsComputeEnabledProc = NULL;
	dialogParams.settingsExecuteProc = NULL;
	dialogParams.settingsProcData = NULL;

	if(!AVAppOpenDialog (&dialogParams,  &fileSys, &pathNames,
									&numPathnames,
									NULL))
	{
		ASTextDestroy(dialogParams.initialFileName);
		E_RETURN(false);
	}
	ASTextDestroy(dialogParams.initialFileName);
	
	ASInt32 ret;

	for (int i = 0; i <= numPathnames-1; i++)
	{
		char buffer[metaDataLength];
		ret = ASFileSysGetNameFromPath (ASGetDefaultFileSys(),pathNames[i], buffer, metaDataLength);

		if (!strcmp(filetype, "PDF"))
		{
			docs[i] = PDDocOpen( pathNames[i],ASGetDefaultFileSys(), NULL, true );
		}
		else if (!strcmp(filetype, "XML")){
			ASInt32 ret1 = ASFileSysOpenFile (ASGetDefaultFileSys(), pathNames[i],
				ASFILE_READ, &fP[i]);

			ReadXMLFile (&fP[i], data);
		}

		else{
			ASInt32 ret1 = ASFileSysOpenFile (ASGetDefaultFileSys(), pathNames[i],
				ASFILE_READ, &fP[i]);
		}

		ASFileSysReleasePath (NULL, pathNames[i]);
	}

	*numDocs = numPathnames;

	if (filterRec.filterDescription)
		ASTextDestroy(filterRec.filterDescription);
HANDLER
	char buf[64];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, 63));
	return false;
END_HANDLER
	return true;
}

/** ReadXMLFile
** ----------------------------------------------------------------------
** Read the ASFile into a memeory buffer.
*/
void ReadXMLFile(ASFile *printFile, char *data)
{
	Int32 nRead;
	char buf[100];
	if (printFile) {
		ASFileSetPos(*printFile, 0);
		strcpy (data, "");
		while ((nRead = ASFileRead(*printFile, buf, sizeof(buf)-1)) != 0){
			strncat (data, buf, nRead);
		}
	}
	ASFileClose(*printFile);
}

/** saveFile
** ----------------------------------------------------------------------
** Save a file by prompting a file save dialog.
** Input Param: filetype - used to specify the file extension doc - PD doc
** Output param: data - a buffer that stores the file
** Return: true if successful, false otherwise
*/
bool SaveFile(PDDoc doc, char * filetype, char *data)
{
	AVOpenSaveDialogParamsRec dialogParams;
	ASPathName pathNames = NULL;
	ASFileSys fileSys = NULL;

	AVFileFilterRec filterRec, *filterRecP;
	AVFileDescRec descRec;

	filterRecP = &filterRec;
DURING
	memset(&dialogParams, 0, sizeof(dialogParams));
	filterRec.filterDescription = ASTextNew();

	ASTextSetEncoded(filterRec.filterDescription, filetype, AVAppGetLanguageEncoding());

	filterRec.fileDescs = &descRec;
	filterRec.numFileDescs = 1;

	strcpy(descRec.extension, filetype);
	descRec.macFileType = 0;
	descRec.macFileCreator = 0;

	dialogParams.size = sizeof(dialogParams);
	dialogParams.flags = kAVOpenSaveAllowAllFlag;
	dialogParams.parentWindow = NULL;
	dialogParams.windowTitle = NULL;
	dialogParams.actionButtonTitle = NULL;
	dialogParams.initialFileSys = NULL;
	dialogParams.initialPathName = NULL;
	dialogParams.initialFileName = ASTextNew();
	dialogParams.fileFilters = &filterRecP;
	dialogParams.numFileFilters = 1;
	dialogParams.settingsComputeEnabledProc = NULL;
	dialogParams.settingsExecuteProc = NULL;
	dialogParams.settingsProcData = NULL;

	if(!AVAppSaveDialog (&dialogParams,  &fileSys, &pathNames, NULL ))
	{
		ASTextDestroy(dialogParams.initialFileName);
		E_RETURN(false);
	}
	
	ASTextDestroy(dialogParams.initialFileName);

	char buffer[metaDataLength];
	ASInt32 ret = ASFileSysGetNameFromPath (ASGetDefaultFileSys(), pathNames, buffer, metaDataLength);

	if (!strcmp(filetype, "PDF"))
	{
		PDDocSave (doc, PDSaveFull | PDSaveLinearized, pathNames, ASGetDefaultFileSys(), NULL, NULL);
	}
	else{

		ASFile fP;
		ASInt32 ret1 = ASFileSysOpenFile (ASGetDefaultFileSys(), pathNames,
			ASFILE_CREATE|ASFILE_WRITE, &fP);
		WriteXMLFile (&fP, data);
	}

	ASFileSysReleasePath (NULL, pathNames);
	if (filterRec.filterDescription)
		ASTextDestroy(filterRec.filterDescription);
HANDLER
	char buf[64];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, 255));
	return false;
END_HANDLER
	return true;
}

/** WriteXMLFile
** ----------------------------------------------------------------------
** Write the ASFile from a memeory buffer.
*/
void WriteXMLFile(ASFile *printFile, char *data)
{
	Int32 nWrite = ASFileWrite (*printFile, data, strlen (data));
	ASFileClose(*printFile);
}

/** ADM dilaog utility functions */

/* DoGetMetadataButton
** ----------------------------------------------------------------------
** Obtain the existing document level metadata if there is, and display the
** XML source in the dialog and output the XML file.
*/
static ACCB1 void ACCB2 DoGetMetadataButton(ADMItemRef item, ADMNotifierRef inNotifier )
{
	sADMItem->DefaultNotify(item, inNotifier);

	/* Get document level metadata */
	char * docMetadata = new char [metaDataLength];
	GetDocMetaData(docMetadata);
	
	/* Get item's dialog */
	ADMDialogRef dialog = sADMItem->GetDialog(item);
	/* Get the data display item */
	ADMItemRef item1 = sADMDialog->GetItem(dialog, IDC_DATA);

	/* Output the doc level metadata as an XML file */
	AVAlertNote ("Save the doc metadata as a XML file.");
	if (!SaveFile(NULL, "XML", docMetadata))
		AVAlertNote ("Fail in saving the doc metadata as a XML file.");
	else{
		char buf[255];
		/* Display the instruction about the action results */
		sprintf(buf, "Document metadata file has been generated. %s",
			"Choose Advanced/Document Metadata/ to see metadata.");
		/* Set text for item */
		sADMItem->SetFont(item1, kADMBoldDialogFont);
		ASText text = ASTextFromScriptText(buf,	kASRomanScript);
		ADMUnicode* unicodeText = (ADMUnicode*) ASTextGetUnicode(text);
		sADMItem->SetTextW (item1, unicodeText);
		ASTextDestroy(text);
	}

	/* Update item */
	sADMItem->Update(item1);

	if (docMetadata)
		delete docMetadata;
	return;
}


/** DoAddetMetadataButton
** ----------------------------------------------------------------------
** Add XML metadata to PDF from input file. Replace the existing metadata if these is,
** and display it in dialog.
*/
static ACCB1 void ACCB2 DoAddMetadataButton(ADMItemRef item, ADMNotifierRef inNotifier )
{
	sADMItem->DefaultNotify(item, inNotifier);
	char * docMetadata = new char [metaDataLength];
	ASInt32 numOfDocs;
	ASFile *fP = (ASFile *) ASmalloc(MAX_NUM_OPENFILE * sizeof(ASFile));

	/* Get the metadata from an input file */
	if (!OpenFile("XML", docMetadata, NULL, &numOfDocs, fP))
	{
		strcpy (docMetadata, "Wrong input file\n");
		
		return;
	}
	ASfree(fP);
	/* Set metadata to the doc */
	if (!SetDocMetaData(docMetadata)){
		AVAlertNote("Error: metadata wasn't added.");
		return;
	}

	ADMDialogRef dialog = sADMItem->GetDialog(item);
	ADMItemRef item1 = sADMDialog->GetItem(dialog, IDC_DATA);

	if (item1 != NULL){
		char buf[256];
		sprintf(buf, "The old document level metadata has been replaced by input XML metadata. %s",
			"Choose Advanced/Document Metadata to see the document metadata change.");
		sADMItem->SetFont(item1, kADMBoldDialogFont);

		ASText text = ASTextFromScriptText(buf,	kASRomanScript);
		ADMUnicode* unicodeText = (ADMUnicode*) ASTextGetUnicode(text);
		sADMItem->SetTextW (item1, unicodeText);
		ASTextDestroy(text);
	}

	sADMItem->Update(item1);
	delete docMetadata;
	return;
}


/** DoSearchURLButton
** ----------------------------------------------------------------------
** Obtain all URLs appeared in the doc, and display them in the UI
*/
static ACCB1 void ACCB2 DoSearchURLButton(ADMItemRef item, ADMNotifierRef inNotifier )
{
	sADMItem->DefaultNotify(item, inNotifier);
	ADMDialogRef dialog = sADMItem->GetDialog(item);
	ADMItemRef item0 = sADMDialog->GetItem(dialog, IDC_DATA);
	
	ADMItemRef item1 = sADMDialog->GetItem(dialog, IDC_URL_LIST);
	CDocument document;
	PDDocGetInfo ((PDDoc)document, "BaseURL", gBaseURL, sizeof (gBaseURL)-1);
	
	ASText text = ASTextFromScriptText(gBaseURL, kASRomanScript);
	ADMUnicode* unicodeText = (ADMUnicode*) ASTextGetUnicode(text);
	sADMItem->SetTextW (item1, unicodeText);
	ASTextDestroy(text);
	sADMItem->SetFont(item1, kADMBoldItalicDialogFont);

	ASText text1 = ASTextFromScriptText("Add the base URL as metadata if exists, or type in your own base URL and click Add URL button to add the URL in document metadata.", 
		kASRomanScript);
	ADMUnicode* unicodeText1 = (ADMUnicode*) ASTextGetUnicode(text1);
	sADMItem->SetTextW (item0, unicodeText1);
	ASTextDestroy(text1);

	sADMItem->Update(item0);
	sADMItem->Update(item1);
}

/** DoAddURLButton
** ----------------------------------------------------------------------
** High light one URL in the displayed list, add it into doc info, which in turn is added
** as a metadata attribute (a Property Name/Value pair).
*/
static ACCB1 void ACCB2 DoAddURLButton(ADMItemRef item, ADMNotifierRef inNotifier)
{
	sADMItem->DefaultNotify(item, inNotifier);

	ADMDialogRef dialog = sADMItem->GetDialog(item);
	ADMItemRef item0 = sADMDialog->GetItem(dialog, IDC_URL_LIST);
	sADMItem->Update(item0);

	ADMUnicode unicText[127];
	sADMItem->GetTextW(item0, unicText, 127);
	ASText asText = ASTextFromUnicode(unicText,kUTF16HostEndian);	
	strcpy( gBaseURL, ASTextGetScriptText(asText, kASRomanScript));
	ASTextDestroy(asText);

	/* Add metadata to the doc level metadata */
	assert(SetMetaProperty (gBaseURL) == 1);
	ADMItemRef item1 = sADMDialog->GetItem(dialog, IDC_DATA);
	ASText text = ASTextFromScriptText("The base URL has been added into document metadata. Choose Advanced/Document Properties to display metadata.", 
		kASRomanScript);
	ADMUnicode* unicodeText = (ADMUnicode*) ASTextGetUnicode(text);
	sADMItem->SetTextW (item1, unicodeText);
	ASTextDestroy(text);

	sADMItem->SetFont(item1, kADMBoldDialogFont);
	sADMItem->Update(item1);
}

/** DoCancelButton
** ----------------------------------------------------------------------
** Main dialog exits.
*/
static ACCB1 void ACCB2 DoCancelButton(ADMItemRef item, ADMNotifierRef inNotifier )
{
	sADMItem->DefaultNotify(item, inNotifier);
	// destroy dialog
	sADMDialog->Destroy(sADMItem->GetDialog(item));
	ADMUtils::ReleaseADM();
}

/** DialogOnInit
** ----------------------------------------------------------------------
** The main dialog created when Metadata menu item is choosed from the menu bar.
** Initialize items, and set up which procedure should be notified for each item action
*/
static ASErr ASAPI DialogOnInit(ADMDialogRef dialog)
{

	SPErr error = kSPNoError;
	ADMItemRef item = NULL;

	// initialize items
	if (dialog != NULL){
		item = sADMDialog->GetItem(dialog, IDC_GET_METADATA);
		if (item != NULL)
			sADMItem->SetNotifyProc(item, DoGetMetadataButton);
		item = sADMDialog->GetItem(dialog, IDC_ADD_METADATA);
		if (item != NULL)
			sADMItem->SetNotifyProc(item, DoAddMetadataButton);
		item = sADMDialog->GetItem(dialog, IDC_SEARCH_URL);
		if (item != NULL)
			sADMItem->SetNotifyProc(item, DoSearchURLButton);
		item = sADMDialog->GetItem(dialog, IDC_CANCEL);
		if (item != NULL)
			sADMItem->SetNotifyProc(item, DoCancelButton);
		item = sADMDialog->GetItem(dialog, IDC_ADD_URL);
		if (item != NULL)
			sADMItem->SetNotifyProc(item, DoAddURLButton);
			
#ifdef MAC_PLATFORM

	// get the rect of dummy control we created, then destroy dummy control
	item = sADMDialog->GetItem(dialog, IDC_DATA);
	ASRect bounds;
	sADMItem->GetBoundsRect(item, &bounds); 
	sADMItem->Destroy(item);

	// create ADM scroll bar
	item = sADMDialog->CreateItem(dialog, IDC_DATA, 
							kADMTextEditMultiLineType, &bounds, NULL, NULL, 0);
	
	// get the rect of dummy control we created, then destroy dummy control
	item = sADMDialog->GetItem(dialog, IDC_URL_LIST);
	sADMItem->GetBoundsRect(item, &bounds); 
	sADMItem->Destroy(item);

	// create ADM scroll bar
	item = sADMDialog->CreateItem(dialog, IDC_URL_LIST, 
							kADMTextEditMultiLineType, &bounds, NULL, NULL, 0);
						
							
#endif

	}
	else{
		error = kSPBadParameterError;
	}
	return error;
}



/** AdmMetaDataDialog
** ----------------------------------------------------------------------
** Initialize ADM dialog
*/
ASErr ASAPI AdmMetaDataDialog(void * data)
{
	SPErr error = kSPNoError;
	ADMUtils::InitializeADM();
	ADMDialogRef dialog = sADMDialog->Create(sADMPluginRef,
										"ADBE:metaDataDlg",
										IDD_METADATA, kADMFloatingDialogStyle,
										DialogOnInit, NULL, 0);

	if(!dialog)
		error = kSPBadParameterError;

	return error;
}

/** Extract URL utilities */

/* The PDURILinkEnumProc callback type defines the callback which is
called by PDDocEnumURILinks for each link with a URI. The pageNum
parameter tells you which page the URI was found on. The annotNum
is the index into the annotation array for pageNum. The URIstring
is a pointer to the URI. ClientData is a pointer to user data and
is set up when PDDocEnumURILinks is first called.  */
typedef ACCBPROTO1 ASBool (ACCBPROTO2 *PDURILinkEnumProc)(PDPage page, PDAnnot annot, char *URIstring, void *ClientData);

/* PDDocEnumURILinks will walk each of the annotations on the specified
page(s), calling the linkProc for each link annotation with a URI.
If the linkProc returns false, the enumeration will stop then, rather
than continue to enumerate links on the page.  */
ACEX1 void ACEX2 PDDocEnumURILinks(PDDoc pdd, Int32 startPageNum, Int32 endPageNum,
    PDURILinkEnumProc linkProc, void *vClientData);

#define MAX_URI_SIZE 2048
#define COSTOACTION(obj) (*(PDAction *)&(obj))

/** PDAnnotGetAction
** ----------------------------------------------------------------------
** Get the annotation action
*/
PDAction PDAnnotGetAction(PDAnnot pdan)
{
	CosObj coAction;
	CosObj coAnnot = PDAnnotGetCosObj(pdan);
	CosObj coNull = CosNewNull();

	if (!CosObjIsValid(coAnnot, CosDict))
		return COSTOACTION(coNull);

	/* Get the Action dict. */
	coAction = CosDictGetKeyString(coAnnot, "Action");
	if (CosObjGetType(coAction) != CosDict) {
		coAction = CosDictGetKeyString(coAnnot, "A");
		if (CosObjGetType(coAction) != CosDict)
			return COSTOACTION(coNull);
	}
	return PDActionFromCosObj(coAction);
}

/** PDActionGetURI
** ----------------------------------------------------------------------
** Get URL string from a given PDAction, which is returned in cURI
*/
ASBool PDActionGetURI(PDAction pdac, char *cURI, Uns16 nLen)
{
	CosObj coAction, coURI;

	/* Make sure we have a valid action. */
	if (!PDActionIsValid(pdac))
		return false;

	/* Make sure that the subtype is URI. */
	if (PDActionGetSubtype(pdac) != URI_K)
		return false;

	coAction = PDActionGetCosObj(pdac);

	/* Get the value of the URI key.*/
	coURI = CosDictGet(coAction, URI_K);
	if (!CosObjIsValid(coURI, CosString))
		return false;
	CosCStringValue(cURI, coURI, nLen);

	return true;
}

/** PDActionSetURI
** ----------------------------------------------------------------------
** Set a URI to doc
*/
ASBool PDActionSetURI(PDDoc pddoc, PDAction pdac, char *cURI)
{
	CosObj coAction, coURI;
	CosDoc cdoc;

	/* Make sure we have a valid action. */
	if (!PDActionIsValid(pdac))
		return false;

	/* Make sure that the subtype is URI. */
	if (PDActionGetSubtype(pdac) != URI_K)
		return false;

	coAction = PDActionGetCosObj(pdac);

	if(CosObjGetType(coAction) != CosDict)
		return false;
	/* Set the value of the URI key.*/
	cdoc = PDDocGetCosDoc(pddoc);
	coURI = CosNewString(cdoc, false, cURI, strlen(cURI));
	CosDictPut(coAction, URI_K, coURI);
	return true;
}

/** PDDocEnumURILinks
*/
ACEX1 void ACEX2 PDDocEnumURILinks(PDDoc pdd, Int32 nStartPage, Int32 nEndPage,
	PDURILinkEnumProc linkProc, void *vClientData)
{
	Int32 nNumPages;
	Int32 nError;
	register Int32 i;

	if (linkProc == NULL) {
    	ASRaise(GenError(genErrBadParm));
	}

	nError = 0;
	DURING
	    nNumPages = PDDocGetNumPages(pdd);
	    if (nStartPage < 0 || nEndPage > nNumPages)
			E_RTRN_VOID;

		/* Loop over each requested page. */
		for (i = nStartPage; i <= nEndPage; i++){
			PDPage pdp = PDDocAcquirePage(pdd, i);
			Int32 nNumAnnots;
			register Int32 j;
			char cURI[MAX_URI_SIZE];
			PDAction pdac;

			/* Loop over all annotations on the page. */
			nNumAnnots = PDPageGetNumAnnots(pdp);
			for (j = 0; j < nNumAnnots; j++){
				/* Get the annotation and check to see if it's a link. */
				PDAnnot pdan;
				pdan = PDPageGetAnnot(pdp, j);
				if (!PDAnnotIsValid(pdan) || PDAnnotGetSubtype(pdan)
					!= ASAtomFromString("Link"))
					continue;

				/* Try to get the action dictionary. */
				pdac = PDAnnotGetAction(pdan);
				if (PDActionGetURI(pdac, cURI, sizeof(cURI))){
					if (!linkProc(pdp, pdan, cURI, vClientData)){
						/* linkProc has asked us to stop. */
						PDPageRelease(pdp);
						E_RTRN_VOID;
					}
				}
			}

			PDPageRelease(pdp);
		}

	HANDLER
		nError = ERRORCODE;
	END_HANDLER
	if (nError)
	    ASRaise(nError);
}

/** - This snippet demonstrate how to set, get, and extend document level metadata.
 	 
	 - Get Metadata: Click the button to get the document metadata in the form of XML
	   text, and export it to a XML file.
 	 - Set Metadata: Click the button to replace the document level metadata by a 
	   user specified XML file.
 	 - Extend existing metadata: allows user to extend metadata by adding the base URL in the doc.
 		a. Search Base URLs : Click the button to search for the base URLs in doc. if found, display them in the right window.
 		b. Add Base URLs : Type in new BaseURLs in the right window, then click the button. 
		   The base URLs will be added in the document metadata. 
	 - The user can use Acrobat menu Advanced>Document Metadata... to check the above operation. 
		
 - Since the Metadata Dialog UI in Acrobat 6.0 and above is implemented in the
   touch-up plug-in, the touch-up plug-in must be loaded to use this
   sample plug-in. If you have a PDF browser plug-in such as Enfocus
   PDF object browser installed, you can examine the metadata added
   from the PDF object browser as well.
 - NOTE: This snippet is coverted from the DocMetadata plug-in sample with 
   a couple of files combined. 
   NEW in Acrobat 6.0: PDDocSetXAPMetadataProperty is provided to set XMP
   metadata directly.

   @see CosStringValue
   @see CosObjGetType
   @see CosDocGetRoot
   @see CosObjIsValid
   @see CosDictGet
   @see ASTextNew
   @see PDDocGetXAPMetadata
   @see ASTextGetPDTextCopy
   @see ASTextDestroy
   @see ASTextFromEncoded
   @see PDDocSetXAPMetadata
   @see ASTextFromPDText
   @see PDDocSetXAPMetadataProperty
   @see AVAppOpenDialog
   @see ASFileSysGetNameFromPath
   @see ASGetDefaultFileSys
   @see PDDocOpen
   @see ASFileSysOpenFile
   @see ASFileSysReleasePath
   @see ASTextDestroy
   @see ASFileRead
   @see ASFileClose
   @see ASTextSetEncoded
   @see AVAppSaveDialog
   @see ASFileWrite
   @see PDAnnotGetCosObj
   @see CosNewNull
   @see PDActionIsValid
   @see PDActionGetSubtype
   @see PDActionGetCosObj
   @see PDDocGetCosDoc
   @see CosNewString
   @see PDDocAcquirePage
   @see PDPageGetNumAnnots
   @see PDPageGetAnnot
   @see PDAnnotGetAction
*/
void DocMetadataSnip()
{
	// need an active doc
	CDocument document;
	PDDoc activePDDoc = (PDDoc)document;
	if(activePDDoc == NULL)
		return;
	
	// check base URL
	ASBool thereIsBaseURL = PDDocGetBaseURI(activePDDoc, gBaseURL, 128);  // Get the base URL of PDF
	if (!thereIsBaseURL)
		strcpy (gBaseURL, "There is no base URL for this PDF.");

	// pop up dialog
	AdmMetaDataDialog(NULL);
}

/** Register snippet */
SNIPRUN_REGISTER(DocMetadataSnip, "PD Layer:Metadata:Document Metadata","Work with document metadata. For this sample, the maximum number of files you can open is 10.")
