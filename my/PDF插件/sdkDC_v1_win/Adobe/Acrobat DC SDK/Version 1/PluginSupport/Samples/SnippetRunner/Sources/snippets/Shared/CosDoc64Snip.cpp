/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 CosDoc64Snip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static ACCB1 ASBool ACCB2 EnumEOFsProc64(CosDoc cosDoc,
  ASFileOffset64 fileOffset, void *clientData);

/** 
	This snippet demonstrates 64-bit file position access into CosDoc objects.

	@testfile TwoColumnTaggedDoc.pdf
	@requires any pdf file
	@see PDDocGetCosDoc
	@see CosDocGetInfoDict
	@see CosDocEnumEOFs64
	@see CosDocEnumEOFsProc64
	@see CosDocObjIsWithinRange64
*/
static void CosDoc64Snip()
{
	CDocument doc;
	PDDoc pdDoc = (PDDoc) doc;
	if (!pdDoc) return;

	// 
	// Gets the CosDoc and its Info dictionary object
	//

	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);
	CosObj infoDict = CosDocGetInfoDict(cosDoc);

	//
	// Enumerates the EOFs in the Cos document
	//

	CosDocEnumEOFsProc64 cbEnumEOFs = ASCallbackCreateProto(CosDocEnumEOFsProc64, EnumEOFsProc64);
	CosDocEnumEOFs64(cosDoc, cbEnumEOFs, (void*)&infoDict);
	ASCallbackDestroy((void*)cbEnumEOFs);
}

/**
	Callback for CosDocEnumEOFs64.

	Called by CosDocEnumEOFs64 for each EOF in a given CosDoc.
	We check,for each EOF found, whether the Info dictionary CosObj 
	is within	the byte range from the beginning of document to the 
	current EOF offset.
	
	The enumeration	continues as long as the callback returns true
*/
static ACCB1 ASBool ACCB2 EnumEOFsProc64(CosDoc cosDoc,
  ASFileOffset64 fileOffset, void *clientData)
{
	CosObj* infoDict = (CosObj*) clientData;
	ASFilePos64 byteRange[2];

	byteRange[0] = 0;
	byteRange[1] = fileOffset;

	char buf[128];
	memset(buf, 0, sizeof(buf));

	//
	// Invokes method CosDocObjIsWithinRange64 to determine
	// whether the Info dictionary object of the document is
	// within the byte ranges of the beginning of the document
	// and various %%EOF offsets (if more than one)
	//

	ASBool objIsWithinRange = CosDocObjIsWithinRange64(*infoDict, byteRange, 2);
#ifdef WIN_PLATFORM
	sprintf(buf, "EOF found at byte offset 0x%016I64x.\nInfo dictionary %s within range.\n", 
		fileOffset, (objIsWithinRange) ? "is" : "is not");
#else
	sprintf(buf, "EOF found at byte offset 0x%016llx.\nInfo dictionary %s within range.\n", 
		fileOffset, (objIsWithinRange) ? "is" : "is not");
#endif
	Console::displayString(buf);

	return true;
}

SNIPRUN_REGISTER(CosDoc64Snip, "COS Layer:64 bit CosDoc access", 
	"This snippet demonstrates 64-bit file position access into CosDoc objects. \
Requires an open document. Output is written to the Common SnippetRunner Interface.")

