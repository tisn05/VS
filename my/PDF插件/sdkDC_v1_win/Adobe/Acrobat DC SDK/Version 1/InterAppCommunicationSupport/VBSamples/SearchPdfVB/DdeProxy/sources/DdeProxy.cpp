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

 DdeProxy.cpp

 - This module contains functions which can be called from Visual
   Basic to initiate DDE interactions with the Acrobat Search plugin.

*********************************************************************/

#include <windows.h>
#include <ddeml.h>
#include "ddeproxy.h"

/*-----------------------------------------------
	Constants/Declarations
-----------------------------------------------*/

typedef struct _DDEData {
	DWORD id;
	HSZ hszServerName;
	HSZ hszTopicName;
	HSZ hszItemName;
	HCONV hConversation;
} DDEData;

DDEData *s_ddeData;

#define DDE_ID			(s_ddeData->id)
#define DDE_SERVERNAME  (s_ddeData->hszServerName)
#define DDE_TOPICNAME	(s_ddeData->hszTopicName)
#define DDE_ITEMNAME	(s_ddeData->hszItemName)
#define DDE_HCONV		(s_ddeData->hConversation)

/*-----------------------------------------------
	Main Entrypoint
-----------------------------------------------*/

BOOL APIENTRY DllMain (HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

/*-----------------------------------------------
	DDE Implementation
-----------------------------------------------*/

HDDEDATA CALLBACK DDE_ProcessMessage(UINT wType, UINT wFmt, HCONV hConv, HSZ hsz1,
									 HSZ hsz2, HDDEDATA hData, DWORD lData1, DWORD lData2);

BOOL DDE_InitQuery(void)
{
	s_ddeData = (DDEData *)malloc (sizeof(DDEData));
	if (!s_ddeData)
		return FALSE;

	memset (s_ddeData, 0, sizeof(DDEData));

	if (DdeInitialize (&DDE_ID, &DDE_ProcessMessage, APPCMD_CLIENTONLY, 0) != DMLERR_NO_ERROR)
		return FALSE;

	DDE_SERVERNAME = DdeCreateStringHandle (DDE_ID, "Acrobat Search", 0);
	DDE_TOPICNAME = DdeCreateStringHandle (DDE_ID, "Acrobat Search", 0);
	DDE_ITEMNAME = DdeCreateStringHandle (DDE_ID, "Query", 0);

	return TRUE;
}

BOOL DDE_InitIndex (void)
{
	s_ddeData = (DDEData *)malloc (sizeof(DDEData));
	if (!s_ddeData)
		return FALSE;

	memset (s_ddeData, 0, sizeof(DDEData));

	if (DdeInitialize (&DDE_ID, DDE_ProcessMessage, APPCMD_CLIENTONLY, 0) != DMLERR_NO_ERROR)
		return FALSE;

	DDE_SERVERNAME = DdeCreateStringHandle (DDE_ID, "Acrobat Search", 0);
	DDE_TOPICNAME = DdeCreateStringHandle (DDE_ID, "Acrobat Search", 0);
	DDE_ITEMNAME = DdeCreateStringHandle (DDE_ID, "Index", 0);

	return TRUE;
}

HDDEDATA DDE_PokeData (void *cData, int nLen)
{
	HDDEDATA bRetVal;
	DWORD dwResult;
	DDE_HCONV = DdeConnect (DDE_ID, DDE_SERVERNAME, DDE_TOPICNAME, NULL);
	if (!DDE_HCONV)
		return FALSE;
	bRetVal = DdeClientTransaction ((unsigned char *)cData, nLen, DDE_HCONV, DDE_ITEMNAME,
										CF_TEXT, XTYP_POKE, 1000, &dwResult);

	DdeDisconnect(DDE_HCONV);
    return bRetVal;
}

void DDE_Term (void)
{
	DdeFreeStringHandle(DDE_ID, DDE_SERVERNAME);
	DdeFreeStringHandle(DDE_ID, DDE_TOPICNAME);
	DdeFreeStringHandle(DDE_ID, DDE_ITEMNAME);

	DdeUninitialize(DDE_ID);

	free(s_ddeData);
}

HDDEDATA CALLBACK DDE_ProcessMessage (UINT wType, UINT wFmt, HCONV hConv, HSZ hsz1, HSZ hsz2,
								HDDEDATA hData, DWORD lData1, DWORD lData2)
{
	return NULL;
}
