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
 
   ExploreMetadataSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

#define Metadata_KStr "Metadata"
static ASBool gHasImageMetadata;

static void	ShowDocMetadata(PDDoc pdDoc);
static void ShowSectionMetadata(PDDoc pdDoc);
static void	ShowPageMetadata(PDDoc pdDoc);
static void	ShowImageMetadata(PDDoc pdDoc);
static void GetImageMetadata(PDEElement element, ASInt32 pgNum);

/**
	This snippet explores all metadata streams embedded in the document such as:
	<ul>
	<li> document metadata
	<li> page metadata
	<li> image metadata
	<li> section metadata
	</ul>
	A test example PDF file, ExploreMetadata.pdf, is provided in the ExampleFiles Folder.
	It contains all four types of metadata, document, page, section, and image metadata streams.

	@testfile ExploreMetadata.pdf
	@requires open pdf, likes one that contains metadata
	@see PDDocGetStructTreeRoot
	@see PDSTreeRootGetNumKids
	@see PDSTreeRootGetKid
	@see PDSElementGetKid
	@see PDSElementGetType
	@see CosDictGetXAPMetadata
	@see PDDocAcquirePage
	@see PDEContainerGetContent
	@see PDEContentGetElem
	@see CosDictSetXAPMetadata
	@see ASTextDestroy
	@see PDEObjectGetType
	@see ASTextSetPDText
	@see PDEImageGetCosObj

  */
void ExploreMetadataSnip(ParamManager * thePM)
{
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;

	if (pdDoc==NULL){
		return;
	}

	ASInt32 metadataType;
	thePM->getNextParamAsInt(metadataType);
	switch (metadataType) {
	case 1:
		ShowDocMetadata(pdDoc);
		break;
	case 2:
		ShowSectionMetadata(pdDoc);
		break;
	case 3:
		ShowPageMetadata(pdDoc);
		break;
	case 4:
		ShowImageMetadata(pdDoc);
		break;
	default:
		Console::displayString ("Select metadata type to be extracted (1-4)\n");
		break;
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(ExploreMetadataSnip, "PD Layer:Metadata:Explore Metadata",
"Explore all embedded metadata streams in a document.\n\
A test example PDF file, ExploreMetadata.pdf, \nis provided in the ExampleFiles Folder for demonstration \
purposes. \nIt contains all four types of metadata, document, page, section, and image metadata streams.\n\
Choose the type of metadata to be retrieved:\n \
1. Document metadata  \t\
2. Section metadata  \t\
3. Page metadata  \t\
4. Image metadata \t", "1")

/* GetImageMetadata
** ----------------------------------------------------------------------
**
** Iterate all images in a PDF, dump metadata stream if there is metadata attached 
** to an image.
**
*/
static void GetImageMetadata(PDEElement element, ASInt32 pgNum)
{	
	ASInt32 pdeType = PDEObjectGetType(reinterpret_cast<PDEObject>(element));
	char buf[150];							
	ASText metadata = NULL;

	if(pdeType == kPDEImage)
	{
		CosObj imageCosObj = CosNewNull();
		PDEImageGetCosObj(reinterpret_cast<PDEImage>(element), &imageCosObj );

		// Get metadata in the image cos oject dictionary
		CosDictGetXAPMetadata(imageCosObj, &metadata);
		if (metadata != NULL){
			sprintf (buf, "Page %d, image metadata: \n\n", pgNum+1);
			Console::displayString (buf);
			Console::displayString (metadata);
			gHasImageMetadata = true;
			ASTextDestroy(metadata);
			return;
		}
	}
	// Recursively call the function 
	else if (pdeType == kPDEContainer){
		PDEContent content = PDEContainerGetContent(reinterpret_cast<PDEContainer>(element));
		ASInt32 numElem = PDEContentGetNumElems (content);
		for (ASInt32 i = 0; i < numElem; i++){
			element = PDEContentGetElem(content, i);
			GetImageMetadata(element, pgNum);
		}
	}

	if (metadata!=NULL){
		ASTextDestroy (metadata);
	}
}

/* ShowImageMetadata
** ----------------------------------------------------------------------
**
** Get metadata attached to an image in PDF
*/
static void	ShowImageMetadata(PDDoc pdDoc)
{
	ASInt32 i, j, numElems, numPages;
	PDEContent pdeContent = NULL;
	PDPage pg = NULL;
	PDEElement pdeElement;
	numPages = PDDocGetNumPages(pdDoc);

	gHasImageMetadata = false;

	for(i = 0; i < numPages; i++)
	{
		pg = PDDocAcquirePage(pdDoc,i);
		pdeContent = PDPageAcquirePDEContent(pg, 0);

		numElems = PDEContentGetNumElems(pdeContent);
		for (j = 0; j < numElems; j++)
		{
			pdeElement = PDEContentGetElem(pdeContent, j); 
			GetImageMetadata(pdeElement, i);
		}

		if (pdeContent != NULL){
			PDPageReleasePDEContent(pg, 0);
		}
		if (pg != NULL){
			PDPageRelease(pg);
		}
	}

	if (gHasImageMetadata==false) {
		Console::displayString("This PDF file doesn't have image metadata.");
	}
}


/* ValidateCosDict
** ----------------------------------------------------------------------
**
** Only two types of cos objects can contain metadata: CosDict or CosStream
*/
static bool ValidateCosDict(CosObj cosObj)
{
    CosType cosObjType = CosObjGetType(cosObj);
    return ((cosObjType == CosDict) || (cosObjType == CosStream));
}


/* ShowDocMetadata
** ----------------------------------------------------------------------
**
** Get metadata stream for the document. 
** If there is a document level metadata stream, it should be beared in the catalog dictionary.
** There is a high level API abstraction for accessing doc metadata: PDDocGetXAPMetadata
*/
static void ShowDocMetadata(PDDoc pdDoc)
{
	ASText docMetadata = NULL;
	docMetadata = PDDocGetXAPMetadata(pdDoc);
	if(docMetadata!=NULL)
	{
		Console::displayString("Document Level Metadata:\n\n");
		Console::displayString(docMetadata);
		ASTextDestroy (docMetadata);
	}
}


/* ShowPageMetadata
** ----------------------------------------------------------------------
**
** Get metadata stream for each page of the document. Page metadata can be data about font info,
** page content, color info etc.
** If there is a page metadata stream, it is found in the page cos dictionary.
*/
static void	ShowPageMetadata(PDDoc pdDoc)
{
	char buf[200];
	ASInt32 i, numPages;
	PDPage pg = NULL;
	ASBool hasPageMetadata = false;
	CosObj pgCosObj;
	ASText metadata_astext = NULL;

	numPages = PDDocGetNumPages(pdDoc);
	
	for(i = 0; i < numPages; i++)
	{
		pg = PDDocAcquirePage(pdDoc,i);
		pgCosObj = PDPageGetCosObj (pg);
		// Check if the page cos dictionary had the /Metadata key
		if (CosObjGetType(pgCosObj) == CosDict && CosDictKnownKeyString(pgCosObj, Metadata_KStr)){
			CosDictGetXAPMetadata (pgCosObj, &metadata_astext);
			if (metadata_astext != NULL){
				sprintf (buf, "Page %d Metadata: \n\n", i+1);
				hasPageMetadata = true;
				Console::displayString (buf);
				Console::displayString (metadata_astext);
				ASTextDestroy(metadata_astext);
			}
		}
		if (pg!=NULL) {
			PDPageRelease(pg);
		}
	}
	if (hasPageMetadata==false){
		Console::displayString ("This document had no page metadata.\n");
	}
}

/* ShowSectionMetadata
** ----------------------------------------------------------------------
**
** Get metadata stream for each section of assembled document with PDF binder.
** If there is section metadata, the document should be a tagged PDF, 
** and the section metadata should be beared in a group element of "Document" type.
*/
static void ShowSectionMetadata(PDDoc pdDoc)
{
	PDSTreeRoot pdsTreeRoot;
	ASBool hasSectionMetadata = false;

	if (PDDocGetStructTreeRoot(pdDoc, &pdsTreeRoot)==false) {
		Console::displayString ("The document is not a tagged PDF. No section metadata in this document.");
		return;
	}

	ASInt32 numKids1 = PDSTreeRootGetNumKids(pdsTreeRoot);
	ASText metadata_astext = NULL;
	for (ASInt32 i = 0; i < numKids1; i++) {
		
		PDSElement element;
		PDSTreeRootGetKid(pdsTreeRoot, i, &element);
		ASAtom type = PDSElementGetType(element);
		
		CosObj cosObjKid, cosPage;
		
		// The document consists of multiple sections, that is 
		// the structure has grouping elements
		DURING
			if (strcmp (ASAtomGetString(type), "Document")==0){
				
				ASInt32 numKids2 = PDSElementGetNumKids(element);
				for (ASInt32 j = 0; j < numKids2; j++){
					char buf[50];
					ASAtom kid = PDSElementGetKid(element, j, &cosObjKid, NULL, &cosPage);
					
					if (CosObjGetType(cosObjKid) == CosDict && CosDictKnownKeyString(cosObjKid, Metadata_KStr)){
						if (ValidateCosDict(cosObjKid)){
							CosDictGetXAPMetadata(cosObjKid, &metadata_astext);
							if (metadata_astext != NULL){
								sprintf (buf, "Metadata for document section %d: \n\n", j+1);
								hasSectionMetadata = true;
								Console::displayString (buf);
								Console::displayString (metadata_astext);
								ASTextDestroy(metadata_astext);
								metadata_astext = NULL;
							}
						}
					}
				}
				
			}
			HANDLER				
				if (metadata_astext!=NULL) {
					ASTextDestroy (metadata_astext);
				}				
				char buffer[100];
				ASGetErrorString(ERRORCODE, buffer, sizeof(buffer));
				AVAlertNote(buffer);
			END_HANDLER
	}

	if (metadata_astext!=NULL){
		ASTextDestroy (metadata_astext);
	}
	if (hasSectionMetadata==false) {
		Console::displayString ("No section metadata in this document");
	}
}
