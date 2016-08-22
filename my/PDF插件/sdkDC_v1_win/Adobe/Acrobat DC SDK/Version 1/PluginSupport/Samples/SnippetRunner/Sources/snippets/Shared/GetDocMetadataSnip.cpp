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

 GetDocMetadataSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

const ASInt32 metaStrLen = 1024;

/** example of how to extract PDF document metadata in XML format.
	@testfile ExploreMetadata.pdf
	@requires open document, likes one with metadata
	@see PDDocGetXAPMetadata
*/
void GetDocMetadataSnip()
{
	CDocument document;
	PDDoc activePDDoc = (PDDoc)document;
	if(activePDDoc == NULL)
		return; 

	ASText metaData = NULL;

DURING


	// Get the doc level metadata 
	metaData = PDDocGetXAPMetadata (activePDDoc);
	if (!metaData)
			E_RTRN_VOID;

HANDLER
	char buf[metaStrLen+1];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, metaStrLen));
	if (metaData)
		ASTextDestroy (metaData);
	return;
END_HANDLER

	// display the metadata XML text
	Console::displayString(metaData);
	ASTextDestroy (metaData); // User is responsible to free ASText

}

/** Register snippet */
SNIPRUN_REGISTER(GetDocMetadataSnip, "PD Layer:Metadata:Get Document Metadata","Get PDF document metadata in XML format.")
