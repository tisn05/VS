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

 AddImageMetadataSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

// We define the metadata to add to the image as a static string. In a practical application, this might be from a filesystem, 
// database, or some other source.
const char * XMPMetadata = "<?adobe-xap-filters esc=\"CRLF\"?> \
<x:xmpmeta xmlns:x='adobe:ns:meta/' x:xmptk='XMP toolkit 2.9-9, framework 1.6'> \
<rdf:RDF xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#' xmlns:iX='http://ns.adobe.com/iX/1.0/'> \
 <rdf:Description rdf:about='' \
  xmlns:xapGImg='http://ns.adobe.com/xap/1.0/g/img/'> \
  <xapGImg:Caption>This is the first image of the opened PDF document. </xapGImg:Caption> \
 </rdf:Description> \
</rdf:RDF> \
</x:xmpmeta>";

ASBool gHasImage = false;

/* Check if a cos object is valid to add metadata, must be either CosDict or CosStream */
static ASBool ValidateCosDict(CosObj cosObj)
{
    CosType cosObjType = CosObjGetType(cosObj);
    return ((cosObjType == CosDict) || (cosObjType == CosStream));
}

// this function does the work of adding the metadata to the first image encountered in the tree, the root of which is passed in.
void AddMetadata(PDEElement element)
{	
	ASInt32 pdeType = PDEObjectGetType(reinterpret_cast<PDEObject>(element));
								
	if(pdeType == kPDEImage) {
		CosObj imageCosObj = CosNewNull();
		PDEImageGetCosObj(reinterpret_cast<PDEImage>(element), &imageCosObj);
		ASText metaData = ASTextNew();

		// Set metadata to the image cos oject dictionary
		ASTextSetPDText(metaData, XMPMetadata);
		if(ASBoolToBool(ValidateCosDict(imageCosObj)) == true){
			CosDictSetXAPMetadata(imageCosObj, metaData);
		}

		if (metaData!= NULL) {
			ASTextDestroy(metaData);
		}

		gHasImage = true;

		return;
	}
	// Recursively call the function
	else if (pdeType == kPDEContainer){
		PDEContent content = PDEContainerGetContent(reinterpret_cast<PDEContainer>(element));
		ASInt32 numElem = PDEContentGetNumElems(content);
		for (ASInt32 i = 0; i < numElem; i++){
			element = PDEContentGetElem(content, i);
			AddMetadata(element);
		}
	}
	return;
}

/** This snippet shows how to add XMP metadata to an image xobject in PDF. The XMP data is 
	statically defined, the snippet searches for the first image xobject and applies the 
	metadata to it.
	
	@testfile image.pdf
	@requires pdf file with an image
	
	@see PDDocAcquirePage
	@see PDEContainerGetContent
	@see PDEContentGetElem
	@see CosDictSetXAPMetadata
	@see ASTextDestroy
	@see PDEObjectGetType
	@see ASTextSetPDText
	@see PDEImageGetCosObj
*/
void AddImageMetadataSnip()
{
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if(pdDoc == NULL) {
		return; 
	}
	ASInt32 i, j, numElems, numPages;
	PDEContent pdeContent = NULL;
	PDPage pg = NULL;
	PDEElement pdeElement;
	numPages = PDDocGetNumPages(pdDoc);
	
	for(i = 0; i < numPages; i++)
	{
		pg = PDDocAcquirePage(pdDoc,i);
		pdeContent = PDPageAcquirePDEContent(pg, 0);

		numElems = PDEContentGetNumElems(pdeContent);
		for (j = 0; j < numElems; j++)
		{
			pdeElement = PDEContentGetElem(pdeContent, j); 
			AddMetadata(pdeElement);
			if (ASBoolToBool(gHasImage) == true){
				break;
			}
		}

		if (pdeContent != NULL){
			PDPageReleasePDEContent(pg, 0);
		}
		if (pg != NULL){
			PDPageRelease(pg);
		}
		
		if (ASBoolToBool(gHasImage) == true){
			break;
		}
	}

	if (gHasImage==false){
		AVAlertNote("This PDF file doesn't have image.");
	}
	else{
		Console::displayString("\nThe given XMP metadata stream has been \
successfully added to the cos dictionary of first image in the PDF file. Save the document to preserve the metadata.");
	}
}

/* Register snippet */
SNIPRUN_REGISTER(AddImageMetadataSnip, "PD Layer:Metadata:Add Image Metadata",
				 "Add external metadata to the first image of the opened PDF file.")

