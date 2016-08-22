/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 ResetFormSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"


/** Sets all fields of the form to their default value.
	@see AFPDDocLoadPDFields
	@see AVDocClearSelection
	@see ResetForm
*/
void ResetFormSnip()
{
	AVDoc avDoc = NULL;
	PDDoc pdDoc;
	char errorBuf[256];

	CDocument document;
	avDoc = (AVDoc)document;
	if(!avDoc)
		return;
	DURING
		// Get PDDoc for active AVDoc.
		pdDoc = AVDocGetPDDoc (avDoc);

		// Ensure that all PDFields in form get created, then reset.
		AFPDDocLoadPDFields (pdDoc);

		//need to clear selection so that reset is recorded
		AVDocClearSelection (avDoc, true);
		ResetForm (pdDoc, CosNewNull(), true);
	HANDLER
		ASGetErrorString (ASGetExceptionErrorCode(), errorBuf, 256);
		AVAlertNote (errorBuf);
	END_HANDLER
}

SNIPRUN_REGISTER(ResetFormSnip, "AV Layer:Forms:Reset Form", "Resets the form to default values.")

