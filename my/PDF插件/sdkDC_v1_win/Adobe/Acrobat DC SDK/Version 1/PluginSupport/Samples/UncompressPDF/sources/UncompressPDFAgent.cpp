/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2001-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/ 
/**
\file UncompressPDFAgent.cpp

 - Implementation of the UncompressPDFAgent.

*********************************************************************/

#ifdef WIN_PLATFORM
#include "PIHeaders.h"
#endif

#include "UncompressPDFAgent.h"
#include "ConsObTp.h"

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/* UncompressPDFAgent::ObjFound
** ----------------------------------------------------*/ 
/**
 ** Called when an object is found. If the stream has
 ** any filters associated with it, we return a copy of
 ** that stream with the filters removed.
 **
 ** @return AC_NEVERREVISIT | AC_NOCHANGE when we don't 
 ** change the object and AC_NEVERREVISIT | AC_REPLACE 
 ** when we do.
 **
 ** @see CosDictKnownKeyString
 ** @see CosObjGetDoc
 ** @see CosStreamDict
 ** @see CosObjCopy
 ** @see CosDictRemoveKeyString
 ** @see CosStreamOpenStm
 ** @see CosNewStream
 ** @see ASGetExceptionErrorCode
 ** @see ASStmClose
 **/
ConsultantAgentAction UncompressPDFAgent::ObjFound (CosObj hObj, const PDFObjType* pObjTypeHierarchy, 
								const ASUns32 iSizeObjHierarchy, ConsStack Stack, CosObj* phObjToReturn)
{
	ASBool bUpdateStream = true;
	ASInt32 errorCode = 0;
	static int fsf = 1;

	DURING
	
		// If the stream has no filter associated with it, then 
		// there's no need for us to touch it.

		if (!CosDictKnownKeyString(hObj, "Filter"))
			bUpdateStream = false;

	HANDLER
		bUpdateStream = false;
	END_HANDLER

	if (!bUpdateStream || fsf == 2)
		return AC_NEVERREVISIT | AC_NOCHANGE;

	// If we're here we've been passed a compressed/encoded stream and 
	// we need to remove the filters.

	CosDoc cosDoc;
	CosObj oNewStream, oOldStreamDict, oNewStreamDict;
	ASStm volatile dataStm = NULL;

	DURING

		// Grab a duplicate of the stream's attribute dicionary.
		cosDoc = CosObjGetDoc (hObj);
		oOldStreamDict = CosStreamDict (hObj);
		oNewStreamDict = CosObjCopy (oOldStreamDict, cosDoc, true);

		// Remove the encoding related key-value pairs. It's okay if they don't exist.
		CosDictRemoveKeyString (oNewStreamDict, "Filter");
		CosDictRemoveKeyString (oNewStreamDict, "DecodeParms");
		CosDictRemoveKeyString (oNewStreamDict, "Length");

		// Open a handle to the stream data.
		dataStm = CosStreamOpenStm (hObj, cosOpenFiltered);
		
		// Create the new stream object.
		oNewStream = CosNewStream (cosDoc, true, dataStm, -1, false, oNewStreamDict, CosNewNull(), -1);
		
	HANDLER
		errorCode = ASGetExceptionErrorCode ();
	END_HANDLER

	if (NULL != dataStm)
		ASStmClose (dataStm);

	if (0 == errorCode)
	{
		*phObjToReturn = oNewStream;
		return AC_NEVERREVISIT | AC_REPLACE;
	}

	return AC_NEVERREVISIT | AC_NOCHANGE;
}
