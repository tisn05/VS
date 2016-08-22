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
\file UncompressPDFAgent.h

 - Interface for the UncompressPDFAgent class.

*********************************************************************/

#ifndef _UncompressPDFAgent_h
#define _UncompressPDFAgent_h

#include "ConsExpT.h"

class UncompressPDFAgent : public ConsultantAgentObj
{
	/////////////////////////////////////////////////////////////////
	// Protect unintended use of class.

protected:
	UncompressPDFAgent ();
	UncompressPDFAgent (UncompressPDFAgent& a);
	UncompressPDFAgent& operator= (UncompressPDFAgent& a);

	/////////////////////////////////////////////////////////////////
	// Construction/destruction

public:
	
	UncompressPDFAgent (const PDFObjType* pRequestedObjs, ASUns32 numRequestedObjs) :
	  ConsultantAgentObj (pRequestedObjs, numRequestedObjs) {}

	virtual ~UncompressPDFAgent () {}

	/////////////////////////////////////////////////////////////////
	// Instance methods

	virtual void PostProcess () {}
	virtual ConsultantAgentAction ObjFound (CosObj hObj, const PDFObjType *pObjTypeHierarchy,
	  				const ASUns32 iSizeObjHierarchy, ConsStack Stack, CosObj* phObjToReturn);
};

#endif	// _UncompressPDFAgent_h
