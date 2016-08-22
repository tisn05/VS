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

 CosObjCompressionSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

// Forward declaration of the object enumeration callback
static ACCB1 ASBool ACCB2 ObjEnumProc(CosObj, CosObj, void*);

/** This sample demonstrates the use of some new Cos layer APIs
*	to perform full compression of objects in a document.
*	The resulting fully compressed PDF document is saved as a 
*	new PDF file named "compressed.pdf" by default with a destination 
*	path of choice.
*
	@testfile TwoColumnTaggedDoc.pdf
	@requires open pdf doc
*	@see CosDocEnumIndirect
*	@see CosNewObjCollection
*	@see CosObjGetCompressibility
*	@see CosObjIsCompressed
*	@see CosObjIsIndirect
*	@see CosObjAddToCollection
*/

void CosObjCompressionSnip()
{
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (!pdDoc) return;

	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);

	// Create a new object collection to store compressed streams
	CosObjCollection objCollection = CosNewObjCollection(cosDoc);
	
	ASCallback cosObjEnumProc = (void *)ASCallbackCreateProto(CosObjEnumProc, ObjEnumProc);

DURING	
	Console::displayString("Object compression may take a while ...\n");	

	// Enumerate indirect objects for compression

	if (!CosDocEnumIndirect(cosDoc, (CosObjEnumProc)cosObjEnumProc, &objCollection))
		AVAlertNote("Failed to enumerate indirect objects for compression.");
	
	// Save the compressed object collections to a new document

	ASPathName pathName;
	
	if (NULL == (pathName = SnippetRunnerUtils::getOutputPath("compressed.pdf")))
	{
		CosObjDestroy(objCollection);
		AVAlertNote("Writing compressed file failed");
		E_RTRN_VOID;
	} else
		PDDocSave(pdDoc, PDSaveFull|PDSaveCollectGarbage|PDSaveLinearized, pathName, NULL, NULL, NULL);

HANDLER
	CosObjDestroy(objCollection);
	ASInt32 error = ERRORCODE;
	char err[256];
	ASGetErrorString(error, err, sizeof(err));
	AVAlertNote(err);
END_HANDLER
	Console::displayString("Document objects compressed successfully.");
	ASCallbackDestroy(cosObjEnumProc);
}

static ACCB1 ASBool ACCB2 ObjEnumProc(CosObj obj, CosObj value, void* clientData)
{
	CosObjCollection* objCollection = (CosObjCollection*) clientData;

	CosType objType = CosObjGetType(obj);
	if (objType != CosStream) {
		// Add compressible objects to the object collection:
		// Make sure that the object is indirect, compressible and non-compressed yet
		if (CosObjIsIndirect(obj) && CosObjGetCompressibility(obj) && !CosObjIsCompressed(obj))
			CosObjAddToCollection(*objCollection, obj);
	}	
	
	return true;
}

SNIPRUN_REGISTER(CosObjCompressionSnip, "COS Layer:CosObjCompressionSnip",
	"Fully compress indirect Cos objects in a document into Object Streams in an object collection. \
It will take a while for documents with hundreds or thousands of pages, in whch case the viewer \
may seem unresponsive. Just be patient.")
 

