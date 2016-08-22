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
 
   ExploreStructSnip.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

#define MC_K ASAtomFromString("MC")
#define StructElem_K ASAtomFromString("StructElem")
#define OBJR_K ASAtomFromString("OBJR")

static void GetElemInfo(PDSElement element);
/**
	This snippet explores the structure and content of a tagged PDF and dump the structure info to console or the
	debug window. It demonstrates how to obtain the in-memory representation of the structure 
	tree and explore the content of a structure element that may consist of one or more items of 
	any of the following three kinds:

	  * marked-content
	  * complete PDF objects
	  * other structure elements

	For each tag, the information is dumped in the forms of:
	
	  * Type of tag (StructElem, MC, or OBJR)
	  * Subtype of tag (document, H1, p, sect, part etc)
	  * Title of tag if exists (Document, First Topic, introduction etc )
	  * MCID for marked-content (an unique required entry in the a marked-content reference dictionary)

	NOTE: The order of the tags displayed is simply the recursion sequence of PDS Elements 
	in the program. For the coding simplicity of the snippet, the tags are not categorized in a proper 
	tree structure as in Acrobat tag pane.

	A test example PDF file, ExploreStructure.pdf, is provided in the ExampleFiles Folder.
	It contains all three types of PDS elements.

	The snippet can be extended to access and extract the marked-content stream such as text and images
	through the PDSMCInfo data structure returned from PDSElementGetKidWithMCInfo.

	@testfile TwoColumnTaggedDoc.pdf
	@requires open pdf, likes one with structure
	@see PDDocGetStructTreeRoot
	@see PDSTreeRootGetNumKids
	@see PDSTreeRootGetKid
	@see PDSElementGetKid
	@see PDSElementGetType
	@see PDSElementGetKidWithMCInfo
	@see PDSOBJGetParent
	@see CosObjEqual

  */
void ExploreStructSnip(void)
{
	CDocument document;
	PDDoc pd = (PDDoc)document;
	if (pd == NULL){
		return;
	}

	PDSTreeRoot pdsTreeRoot;

	// Get the structure tree root.
	if (!PDDocGetStructTreeRoot (pd, &pdsTreeRoot)) {
		AVAlertNote("This document is not a tagged PDF.");
		return;
	}

	ASInt32 numRootKids = PDSTreeRootGetNumKids (pdsTreeRoot);
	PDSElement aElem;

	for (ASInt32 i = 0; i < numRootKids; i++){
		char title[128], type[256];
		title[0] = type[0] = '\0';

		PDSTreeRootGetKid (pdsTreeRoot, i, &aElem);
		// Print the type and title info for the first level structure elements
		sprintf (type, " - %s - ", ASAtomGetString(PDSElementGetType(aElem)));
		ASInt32 title_size = PDSElementGetTitle (aElem, reinterpret_cast<ASUns8*>(title));
		if (title_size>0){
			strcat (type, reinterpret_cast<char *>(title)); 
		}
		Console::displayString (reinterpret_cast<char *>(type));
		
		// starting from second level PDS elements, recursively get structure 
		// info for all other elements using PDSElementGetKidWithMCInfo API
		GetElemInfo(aElem);
	}
}

static void GetElemInfo(PDSElement element)
{
	ASInt32 numKids = PDSElementGetNumKids(element);
    ASInt32 kidIndex;

    for (kidIndex = 0; kidIndex < numKids; ++kidIndex) {

        PDSElement kid;
        PDSMCInfo mcidInfo;

		//Get the kid info
        ASAtom kidType = PDSElementGetKidWithMCInfo(element,  //The PDSElement containing the kid that is retrieved
                                                    kidIndex, //The index of the kid.
                                                    &kid,     //The kid being accessed (depending on the kid's type) or NULL.
                                                    &mcidInfo,//The kid's information object or NULL.
                                                    NULL,     //Pointer to the kid or NULL.
                                                    NULL);    //The CosObj of the page containing the kid or NULL
		char buf[256], title[128];
		buf[0] = title[0] = '\0';

		if (kidType == OBJR_K) {
			PDSElement parent = CosNewNull();
			//Get the parent of the OBJR kid which contains the real info for the objr kid
			PDSOBJGetParent (kid, &parent);
			if (!CosObjEqual (parent, CosNewNull())){
				ASInt32 title_size = PDSElementGetTitle (parent, reinterpret_cast<ASUns8*> (title));
           		sprintf (buf, "\tOBJR-elem - parent - %s - %s ",ASAtomGetString(PDSElementGetType(parent)), title);
			}
			else
				strcpy (buf, "\tOBJR-elem");
        } else if (kidType == MC_K) {
			//we print out the MCID (marked-content identifier of the content kid in question.) info only here. But other info and the real content stream
			//can be obtained from PDSMCInfo data structure such as:
			//the stream object containing the marked content in question
			//and The object owning the stream
			sprintf (buf, "\t\tMC-elem - mcid=%d",mcidInfo.mcid);
        } else if (kidType == StructElem_K){
			//We print out the type and title info then recursively get info for its kids
			sprintf (buf, "\tStruct-elem - %s ",ASAtomGetString(PDSElementGetType(kid)));
			ASInt32 title_size = PDSElementGetTitle (kid, reinterpret_cast<ASUns8*> (title));
			strcat (buf, reinterpret_cast<char *>(title));
			GetElemInfo(kid);
        }			
		Console::displayString (buf);
    }
}

/* Register snippet */
SNIPRUN_REGISTER(ExploreStructSnip, "PDS Layer:TaggedPDF:Explore Structure","Explore structure of a tagged PDF.")


