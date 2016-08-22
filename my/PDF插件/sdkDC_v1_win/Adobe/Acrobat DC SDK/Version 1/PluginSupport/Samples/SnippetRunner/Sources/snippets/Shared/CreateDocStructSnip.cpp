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

 CreateDocStructSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

// Use standard structure types for grouping elements. We use "Document"
// and "Part" standard types here
#define Part_K ASAtomFromString ("Part")
#define Document_K ASAtomFromString ("Document")
#define Chapter_K ASAtomFromString ("Chapter")
#define Div_K ASAtomFromString ("Div")
#define MyAttrClass_K ASAtomFromString ("MyAttrClass")

// User defined tag name for the PDE container
#define PageElem_K ASAtomFromString ("PageElem")
// The owner of the attribute
#define PDFLClient_K ASAtomFromString ("PDFLClient")
#define AcrobatSdkPlugIn_K ASAtomFromString ("AcrobatSdkPlugIn")
const ASBool PDFL_SDK = false;

void AddPageStructElem(PDPage page, PDSElement elem, ASInt32 pgNum);

/** This snippet shows how to create a new tagged PDF structure.
	A hard coded tagging structure scheme is as the follow:

	Document
		Chapter 1
			page 1
				content A
				content B
				...
			Page 2
				content X
				content Y
			...
		Chapter 2
			Page 3
			...
		Chapter 3
		...

	If the PDF is not tagged, a new structure will be created as above, otherwise, return a warning message. Create ClassMap and one 
	non-standard attribute that is owned by the client application (PDFL_client or AcrobatSdkPlugIn). Create 
	RoleMap and one user defined structure type "Chapter" that is mapped to the standrad structure type
	"Div". Other elements are either "Document" or "Part" standard type. The class map and role map can be further
	populated with the ClassMapSnip and RoleMapSnip.
 
    NOTE: This is a relative complex snippet that demonstrates creation of a complete structural tree with a chapter-based scheme. 
    However, it is not as robust as the MakeAccessible plug-in. Limitations are:
		* Chapter 1 always starts from page 1 without consideration of existence of "Table of Content" or "Title" page.
		* The chapters have to be in incremental sequence

	@testfile blank.pdf
	@requires any pdf without structure
   	@see PDDocGetStructTreeRoot
	@see PDSTreeRootGetNumKids
	@see PDSTreeRootGetKid
	@see PDSElementGetKid
	@see PDSElementGetType
	@see PDSElementInsertMCAsKid
	@see PDSTreeRootRemoveKid
	@see PDSTreeRootInsertKid
	@see PDSTreeRootGetClassMap
	@see PDSTreeRootCreateClassMap
	@see PDSClassMapAddAttrObj
	@see PDSTreeRootGetRoleMap
	@see PDSTreeRootCreateRoleMap
	@see PDSRoleMapMap
 */
void CreateDocStructSnip(ParamManager * thePM)
{
	CDocument document;
	PDDoc pd = (PDDoc)document;
	if (pd == NULL){
		return;
	}

	PDSTreeRoot pdsTreeRoot;
	PDSClassMap classMap;
	PDSRoleMap roleMap;

	ASBool isMarkedPDF = false;
	ASBool wasTagged = false;

	// Get the structure tree root. If doesn't exist, create one
	if (!PDDocGetStructTreeRoot (pd, &pdsTreeRoot)) {
		PDDocCreateStructTreeRoot (pd, &pdsTreeRoot);
	}
	else{
		AVAlertNote ("Need to open a non-structural PDF to test this snippet.");
	}

	// Get the "marked" key value in the "MarkInfo" dictionary
	// Pass in false for the second param to indicate that all we want is
	// if the PDF is tagged, we don't want to create a tagged PDF yet
	isMarkedPDF = SnippetRunnerUtils::PDDocIsMarkedPDF(pd, false);

	// If it is a legacy PDF
	if (isMarkedPDF == false){
		// We set the marked key in the MarkedInfo doctionary
		SnippetRunnerUtils::PDDocIsMarkedPDF(pd, true);
	}

	// If there is no class map, create an empty class map
	if (!PDSTreeRootGetClassMap (pdsTreeRoot, &classMap))
		PDSTreeRootCreateClassMap (pdsTreeRoot, &classMap);

	PDSAttrObj attrObj = CosNewNull();
	if (PDFL_SDK == 1)
		// We create an dummy attribute owned by the client application 
		PDSAttrObjCreate (pd, PDFLClient_K, true, &attrObj);
	else
		PDSAttrObjCreate (pd, AcrobatSdkPlugIn_K, true, &attrObj);
		
	PDSClassMapAddAttrObj (classMap, MyAttrClass_K, attrObj);

	// If there is no role map, create an empty role map
	if (!PDSTreeRootGetRoleMap (pdsTreeRoot, &roleMap))
		PDSTreeRootCreateRoleMap (pdsTreeRoot, &roleMap);
	// We create an user defined structure type "Chapter" and map it to the standard structure type "Div"
	PDSRoleMapMap (roleMap, Chapter_K, Div_K);

	PDSElement elem, chapter_elem;
	PDSElementCreate(pd, &elem);
	PDSElementSetType(elem, Document_K);
	PDSElementSetTitle(elem, reinterpret_cast<const ASUns8*> ("Document"), strlen("Document"));
	PDSTreeRootInsertKid(pdsTreeRoot, elem, kPDSAfterLast);
	
	PDPage pg; 
	ASInt32 numPages = PDDocGetNumPages (pd);
	ASInt32 i = 0;
	ASInt32 chapter = 1;
	ASInt32 startPg = 0;
	ASInt32 endPg = 0;
	char buf[64];
	buf[0] = '\0';

	thePM->getNextParamAsInt (startPg);
	thePM->getNextParamAsInt (endPg);
	if (startPg != 1 || endPg < startPg || endPg > numPages){
		AVAlertNote ("Wrong input. Enter the chapter starting page numbers. The chapter 1 must start from page 1, enter 1 for the first number.");
		return;
	}
	
	ASBool endOfParam = false;

	while (startPg <= numPages && startPg <= endPg)
	{
		PDSElementCreate(pd, &chapter_elem);
		PDSElementSetType(chapter_elem, Chapter_K);
		sprintf (buf, "Chapter %d", chapter);
		PDSElementSetTitle(chapter_elem, reinterpret_cast<const ASUns8*> (buf), strlen(buf));

		if (numPages == 1){
			pg = PDDocAcquirePage(pd, 0);
			AddPageStructElem(pg, chapter_elem, 0);
			if (pg)
				PDPageRelease (pg);
			PDSElementInsertKid(elem, chapter_elem, kPDSAfterLast);
			return;
		}else{
			for (i = startPg - 1; i < endPg - 1; i++){
				pg = PDDocAcquirePage(pd, i);
				AddPageStructElem(pg, chapter_elem, i);
				if (pg)
					PDPageRelease (pg);
			}
		}
		PDSElementInsertKid(elem, chapter_elem, kPDSAfterLast);

		startPg = endPg;
		thePM->getNextParamAsInt (endPg);
		
		if (ASBoolToBool(endOfParam) == true)
			break;
		
		if (endPg == 0){
			endPg = numPages+1;
			endOfParam = true;
		}
		chapter++;
	}
	
	Console::displayString("Document structure created");
}

/* AddPageStructElem
** ----------------------------------------------------------------------
** Insert the structure elements appeared on a page to the parent structure element
*/
void AddPageStructElem(PDPage page, PDSElement elem, ASInt32 pgNum)  
{	
	ASInt32 err = 0;
	ASInt32 length = 0;
	PDEContent volatile content = NULL;
	PDEContent volatile subContent = NULL;
	PDEContainer volatile container = NULL;
	PDEElement volatile current = NULL;
	PDSElement structElem;
	char buf[30];
	buf[0] = '\0';
	sprintf (buf, "Page %d", pgNum+1);

	PDDoc pd = PDPageGetDoc(page);

	// Create a "part" type of structure element for each page
	// And set the name of the element as "Page x"
	PDSElementCreate(PDPageGetDoc(page), &structElem);
	PDSElementSetType(structElem, Part_K);
	PDSElementSetTitle(structElem, reinterpret_cast<const ASUns8*> (buf), strlen(buf));

	// We iterate all elements in a page content. For each element, create a new
	// container with a marked tag "PageElem" and the old element as content, and replace the old element with
	// the new container element
	DURING
		content = PDPageAcquirePDEContent(page, 0);		
		length = PDEContentGetNumElems(content);
		for (ASInt32 i=0; i < length; i++) {
			//It is optional to create a new Marked Content dictionary for the container
			CosObj mcDict = CosNewDict(PDDocGetCosDoc(pd), false, 1);
			subContent = PDEContentCreate();
			container = PDEContainerCreate(PageElem_K, &mcDict, true);

			current = PDEContentGetElem(content, i);
			PDEContentAddElem(subContent, kPDEBeforeFirst, current);
			PDEContainerSetContent(container, subContent);		

			PDEContentRemoveElem(content, i);
			PDEContentAddElem(content, i-1, reinterpret_cast<PDEElement>(container));
			
			// Insert the content into the structure tree
			PDSElementInsertMCAsKid(structElem, PDPageGetCosObj(page), container, kPDSAfterLast);
			
			if (subContent != NULL) {
				PDERelease(reinterpret_cast<PDEObject>(subContent));
				subContent = NULL;
			}
			if (container != NULL) {
				PDERelease(reinterpret_cast<PDEObject>(container));
				container = NULL;
			}
		}
		PDSElementInsertKid(elem, structElem, kPDSAfterLast);
		PDPageSetPDEContent(page, 0);
	HANDLER
		err = ERRORCODE;
	END_HANDLER

	if (content != NULL) {
		PDPageReleasePDEContent(page, 0);
	}

	if (err != 0) {
		ASRaise(err);
	}
}

/* Register snippet */
SNIPRUN_REGISTER_WITH_DIALOG(CreateDocStructSnip, "PDS Layer:TaggedPDF:Create Doc Structure",
				 "Create a chapter-based structure tree. \nEnter the starting and ending page numbers for each chapter, \
for example, \"1 3 5\",\nwill create three chapters with page1 as the starting page of chapter1,\n\
page3 as the starting page of the chapter2, \nand page5 as the starting page of the chapter3.","1 1")
