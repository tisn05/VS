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

 DdeSrch.cpp

*********************************************************************/

#include <windows.h>
#include "ddeproxy.h"

QueryData gqd;
int gCurOffset, gCurIndexOffset;
#define MAXLEN 1024
#define MAXLENINDEX 150

char gQueryBuf[MAXLEN];
char gIndexBuf[MAXLEN];

/*
Legal language types include:
Simple: no boolean searching
CQL: allows bolean searching as defined in Acrobat Search plugins on-line help
Passthrough: Verity's BooleanPlus query language
*/
void __stdcall SetQueryLanguageType(eQLangType language)
{
	switch(language){
		case QLangType_Simple:
		case QLangType_CQL:
		case QLangType_Passthrough:
			gqd.qlt = language;
			break;
		default: break;
	}
}

int gOptIndex;

/*
Search plugin allows a max of 999 docs. Default value is 100.
Protect against:
 values >999 -- use 999
 values < 1 -- do nothing
*/

void __stdcall SetQueryMaxDocs(short int newmaxDocs)
{
	if(newmaxDocs > 999)
		gqd.nMaxDocs = 999;
	else if(newmaxDocs > 0)
		gqd.nMaxDocs = newmaxDocs;
}

void __stdcall SetQueryOverrideWordOptions(void)
{
	gqd.bOverrideWordOptions = 1;
}

void __stdcall SetQueryWordOptions(unsigned long flags)
{
	gqd.nWordOptions = flags;
}

void __stdcall SetQueryWordTarget(char *target)
{
	int len;
	if(target){
		len = strlen(target);
		if(gCurOffset+len < MAXLEN){
			strcpy(&gQueryBuf[gCurOffset], target);
			gqd.nQueryOffset = gCurOffset;
			gCurOffset += (len+1);
		}
	}
}

void __stdcall SetQuerySortOption(char *field, int ascending)
{
	int len;
	if(field){
		len = strlen(field);
		if(len + gCurOffset < MAXLEN){
			gqd.bSortWays[gOptIndex]  = ascending;
			gqd.nSortOffset[gOptIndex] = gCurOffset;
			gqd.nNumSorts++;
			gOptIndex++;
			strcpy(&gQueryBuf[gCurOffset], field);
			gCurOffset += (len+1);
		}
	}
}
/* Index manipulation functions */
IndexData gid;

void __stdcall SetIndexAction(short int action)
{
	switch(action){
		case IndexAction_Add:
		case IndexAction_Remove:
		case IndexAction_Enable:
		case IndexAction_Disable:
			gid.eAction = (IndexActionType) action;
			break;
		default: break;
	}
}

void __stdcall SetIndexName(char *indexName)
{
	int len;
	if(indexName){
		len = strlen(indexName);
		if(len + gCurOffset < MAXLENINDEX){
			gid.nIndexOffset = gCurIndexOffset;
			strcpy(&gIndexBuf[gCurIndexOffset], indexName);
			gCurIndexOffset += (len+1);
		}
	}
}

/* the temporary name thing is a bit nasty. A placeholder must
exist for the temporary name, even if it's just a null string.
For this reason, SetTempIndexName should be able to accept a null
string specifying a dummy non-existent temp. name. This has been
left as an exercise for the reader.
*/
void __stdcall SetIndexTempName(char *tempindexName)
{
	int len;
	if(tempindexName){
		len = strlen(tempindexName);
		if(len + gCurOffset < MAXLENINDEX){
			gid.nTempNameOffset = gCurIndexOffset;
			strcpy(&gIndexBuf[gCurIndexOffset], tempindexName);
			gCurIndexOffset += (len+1);
		}
	}
}

void __stdcall SrchDDEInitQuery(void)
{
	gCurOffset = gCurIndexOffset = 0;
	if(!DDE_InitQuery())
		MessageBox(NULL, "failed", "x", MB_OK);
	memset(gQueryBuf, 0, MAXLEN);
	gqd.qlt = QLangType_CQL;
	gqd.bOverrideWordOptions = 0;
	gqd.nWordOptions = 0;
	gqd.nMaxDocs = 100;	/* same as Viewer's initial value */
	gqd.nQueryOffset = 0;
	gqd.nNumSorts = 0;
	gOptIndex = 0;
}

void __stdcall SrchDDEInitIndex(void)
{
/* initialize the Index structure */
	gid.eAction = IndexAction_Add;	/* some default value */
	gid.nIndexOffset = 0;
	gid.nTempNameOffset = 0;
	gCurIndexOffset = 0;
	DDE_InitIndex();
}
/* SrchDDESendQuery() packages up the accumulated query data and
feeds it to the search plugin via DDE.
*/

void __stdcall SrchDDESendQuery(void)
{
	unsigned int size, i=0;
	QueryData *p;
	size = sizeof(QueryData)+gCurOffset;
	p = (QueryData *)malloc(size );
	if (p) {
		memset(p, 0, size);
		p->bOverrideWordOptions = gqd.bOverrideWordOptions;
		p->nWordOptions = gqd.nWordOptions;
		p->qlt = gqd.qlt;
		p->nQueryOffset = gqd.nQueryOffset;
		p->nMaxDocs = gqd.nMaxDocs;
		p->nNumSorts = gqd.nNumSorts;
		for(i = 0; i<gqd.nNumSorts; i++){
			p->nSortOffset[i] = gqd.nSortOffset[i];
			p->bSortWays[i] = gqd.bSortWays[i];
		}
		i = 0;
		/* this algorithm assumes that the query buf has been filled out
			correctly. */
		while(strlen(&gQueryBuf[i]) > 0){
			strcpy((char *)&p->cData[i], &gQueryBuf[i]);
//			MessageBox(NULL, &gQueryBuf[i], "x", MB_OK);
			i += strlen(&gQueryBuf[i]) + 1;
		}
		/* By now, we're ready to send it */
		DDE_PokeData(p , size);
		DDE_Term();
		free(p);
	}
}

/*
SrchDDESendIndex() takes the accumulated index change, packages it
up, then sends the DDE query.
*/
void __stdcall SrchDDESendIndex(void)
{
	IndexData *qd;
	int nsize;
	nsize = sizeof(IndexData) + gCurIndexOffset;
    qd = (IndexData *)malloc (nsize);
    memset (qd, 0, nsize);
    if (qd) {
    	qd->eAction = gid.eAction;
    	qd->nIndexOffset = gid.nIndexOffset;
    	qd->nTempNameOffset = gid.nTempNameOffset;
        if( gid.nTempNameOffset > gid.nIndexOffset){
        	strcpy((char *)&qd->cData[gid.nIndexOffset], &gIndexBuf[gid.nIndexOffset]);
        	strcpy((char *)&qd->cData[gid.nTempNameOffset], &gIndexBuf[gid.nTempNameOffset]);
        }else{
            strcpy((char *)&qd->cData[gid.nTempNameOffset], &gIndexBuf[gid.nTempNameOffset]);
        	strcpy((char *)&qd->cData[gid.nIndexOffset], &gIndexBuf[gid.nIndexOffset]);
        }
    	DDE_PokeData(qd, nsize);
    	DDE_Term();
    }
    free(qd);
}
