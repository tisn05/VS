/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 DdeProxy.h

*********************************************************************/

#include "srchdde.h"

#define DDEPROXY_API __declspec(dllexport)

extern "C" {

	// Exported functions.
	DDEPROXY_API void __stdcall SetQueryLanguageType (eQLangType language);
	DDEPROXY_API void __stdcall SetQueryMaxDocs (short int newmaxDocs);
	DDEPROXY_API void __stdcall SetQueryOverrideWordOptions (void);
	DDEPROXY_API void __stdcall SetQueryWordOptions (unsigned long flags);
	DDEPROXY_API void __stdcall SetQueryWordTarget (char *target);
	DDEPROXY_API void __stdcall SetQuerySortOption (char *field, BOOL ascending);

	DDEPROXY_API void __stdcall SetIndexAction (short int action);
	DDEPROXY_API void __stdcall SetIndexName (char *indexName);
	DDEPROXY_API void __stdcall SetIndexTempName (char *tempindexName);

	DDEPROXY_API void __stdcall SrchDDEInitQuery (void);
	DDEPROXY_API void __stdcall SrchDDEInitIndex (void);
	DDEPROXY_API void __stdcall SrchDDESendQuery (void);
	DDEPROXY_API void __stdcall SrchDDESendIndex (void);
}

// Not exported, used internally.
BOOL DDE_InitQuery(void);
BOOL DDE_InitIndex(void);

HDDEDATA DDE_PokeData (void *cData, int nLen);
void DDE_Term (void);
