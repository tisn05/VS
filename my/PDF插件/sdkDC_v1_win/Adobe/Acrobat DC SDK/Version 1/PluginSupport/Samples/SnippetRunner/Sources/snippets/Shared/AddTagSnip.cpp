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

 AddTagSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

#define Document_K ASAtomFromString ("Document")
#define Part_K ASAtomFromString ("Part")
#define MCItem_K ASAtomFromString ("MCItem")

/** This snippet shows how to create a partial structure tree to a un-tagged PDF, 
	add tags, then add three types of content items (marked-content, structure element, and complete PDF objects). 
	
Implementation:	  
   * create a structure tree root
   * create class and role maps 
   * mark the "marked" key in the MarkInfo doctioary
   * add three content items to a structure element
		a marked-content (the first PDEElement on the viewing page) 
		two complete PDF objects (a page obj and an annot obj)

	@testfile cmykText.pdf
	@requires pdf file without structure
	
	@see PDDocGetStructTreeRoot
	@see PDSTreeRootGetNumKids
	@see PDSTreeRootGetKid
	@see PDSElementGetKid
	@see PDSElementSetTitle
	@see PDSElementSetType
	@see PDSElementInsertOBJAsKid
  */
void AddTagSnip(void)
{
	CDocument document;
	PDDoc pd = (PDDoc)document;
	if (pd == NULL){
		return;
	}

	PDPage pg = (PDPage)document; //CDocument releases page

	PDSTreeRoot pdsTreeRoot;
	CosObj pageObj = PDPageGetCosObj (pg);
	CosDoc cd = (CosDoc)document;

	// Get the structure tree root. If doesn't exist, create one
	if (!PDDocGetStructTreeRoot (pd, &pdsTreeRoot)) {
		PDDocCreateStructTreeRoot (pd, &pdsTreeRoot);
	}
	else{
		AVAlertNote ("This document has structure tags. Open a PDF with no tags");
		return;
	}

	// create a new PDS structure element
	PDSElement aElem, newElem;
	PDSElementCreate(pd, &newElem);
	PDSElementCreate(pd, &aElem);
	
	char buf1[64], buf2[64];
	strcpy (buf1, "A new structure element");
	strcpy (buf2, "A kid"); 

	// set its type as "Document" standard type
	PDSElementSetType(aElem, Document_K);
	// set its title
	PDSElementSetTitle(aElem, reinterpret_cast<const ASUns8*> (buf1), strlen(buf1));
	// insert the kid to the tree root
	PDSTreeRootInsertKid (pdsTreeRoot, aElem, kPDSAfterLast);
	// set its type as "Part" standard type
	PDSElementSetType(newElem, Part_K);
	PDSElementSetTitle(newElem, reinterpret_cast<const ASUns8*> (buf2), strlen(buf2));
	PDSElementInsertKid (aElem, newElem, kPDSAfterLast);
	Console::displayString ("Added a StructElem content item.");

	// insert a page object as a content item of the newly created PDE Element
	PDSElementInsertOBJAsKid (newElem, pageObj, pageObj, kPDEAfterLast);
	Console::displayString ("Added a page OBJR content item.");

	// add the first annotation on the page as an OBJR content item
	ASInt32 numAnnot = PDPageGetNumAnnots (pg);
	if (numAnnot > 0){
		PDAnnot annot = PDPageGetAnnot (pg, 0);
		if (ASBoolToBool(PDAnnotIsValid (annot)) == true){
			CosObj annotObj = PDAnnotGetCosObj (annot);
			PDSElementInsertOBJAsKid (newElem, pageObj, annotObj, kPDEAfterLast);
			Console::displayString ("Added an annotation OBJR content item.");
		}
	}
	// add the first PDE Element on the viewing page as a marked-content item
	PDEContent content = NULL;
	PDEContent subContent = NULL;
	PDEContainer container = NULL;
	PDEElement current = NULL;

	content = PDPageAcquirePDEContent(pg, 0);		

	// it is optional to create a new Marked Content dictionary for the container
	CosObj mcDict = CosNewDict(PDDocGetCosDoc(pd), false, 1);
	subContent = PDEContentCreate();
	container = PDEContainerCreate(MCItem_K, &mcDict, true);

	current = PDEContentGetElem(content, 0);
	PDEContentAddElem(subContent, kPDEBeforeFirst, current);
	PDEContainerSetContent(container, subContent);		
	PDEContentRemoveElem(content, 0);
	PDEContentAddElem(content, -1, reinterpret_cast<PDEElement>(container));
	
	// insert the content into the structure tree
	PDSElementInsertMCAsKid(newElem, pageObj, container, kPDSAfterLast);
	Console::displayString ("Added a MC content item.");
	PDPageSetPDEContent(pg, 0);
			
	if (subContent != NULL) {
		PDERelease(reinterpret_cast<PDEObject>(subContent));
	}

	if (container != NULL) {
		PDERelease(reinterpret_cast<PDEObject>(container));
	}

	if (content != NULL) {
		PDPageReleasePDEContent(pg, 0);
	}
}

/* register snippet */
SNIPRUN_REGISTER(AddTagSnip, "PDS Layer:TaggedPDF:Add Tag", "Add tags and content items.")
