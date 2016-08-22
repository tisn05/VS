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

 AddPageMetadataSnip.cpp

***********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* The metadata stream is given XML format that is conform to Adobe XMP platform*/
const char * XMPPageMetadata = "<?adobe-xap-filters esc=\"CRLF\"?> \
<x:xmpmeta xmlns:x='adobe:ns:meta/' x:xmptk='XMP toolkit 2.9-9, framework 1.6'> \
<rdf:RDF xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#' xmlns:iX='http://ns.adobe.com/iX/1.0/'> \
 <rdf:Description rdf:about='' \
  xmlns:pdfx='http://ns.adobe.com/pdfx/1.3/'> \
  <pdfx:Page>This is a page. </pdfx:Page> \
 </rdf:Description> \
</rdf:RDF> \
</x:xmpmeta>";

/* Check if a cos object is valid to add metadata, must be either CosDict or CosStream */
static ASBool ValidateCosDict(CosObj cosObj)
{
    CosType cosObjType = CosObjGetType(cosObj);
    return ((cosObjType == CosDict) || (cosObjType == CosStream));
}

/** This snippet shows how to add XMP metadata to a page in a PDF document

	@testfile blank.pdf
	@requires a pdf file
	
	@see PDDocAcquirePage
	@see PDPageGetCosObj
	@see CosDictSetXAPMetadata
	@see ASTextDestroy
*/
void AddPageMetadataSnip(ParamManager * thePM)
{ 
	ASInt32 pgNum;
	thePM->getNextParamAsInt(pgNum);

	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if(pdDoc == NULL) {
		return; 
	}
		
	ASInt32 numPages = PDDocGetNumPages(pdDoc);
	if (pgNum <= 0 || pgNum > numPages){
		AVAlertNote ("Invalid page number.\n");
		return;
	}

	PDPage pdp = PDDocAcquirePage(pdDoc, pgNum-1);
	CosObj pageCosobj = PDPageGetCosObj(pdp);
	ASText volatile metaData = ASTextNew();
	ASInt32 error = 0;
DURING
	// Create a ASText metadata object from an in-memory string
	ASTextSetPDText(metaData, XMPPageMetadata);
	// Make sure the cos obj is allowed to add metadata
	if(ASBoolToBool(ValidateCosDict(pageCosobj))== true){
		CosDictSetXAPMetadata (pageCosobj, metaData);
	}
HANDLER
	error = ERRORCODE;
END_HANDLER
	char buf[255];
	if (error!= 0){
		ASGetErrorString(ERRORCODE, buf, 255);
		AVAlertNote (buf);
	}
	if (pdp!=NULL) {
		PDPageRelease(pdp);
	}
	if (metaData!=NULL) {
		ASTextDestroy(metaData);
	}
	if (error == 0){
		sprintf (buf, "Adding metadata stream to page %d is successful. \
To preserve the added page metadata, save the PDF file.", pgNum); 
		Console::displayString (buf);
	}
}

/* Register snippet */
SNIPRUN_REGISTER_WITH_DIALOG(AddPageMetadataSnip, "PD Layer:Metadata:Add Page Metadata",
				 "Enter a page number that the metadata stream will be added to.", "1")
