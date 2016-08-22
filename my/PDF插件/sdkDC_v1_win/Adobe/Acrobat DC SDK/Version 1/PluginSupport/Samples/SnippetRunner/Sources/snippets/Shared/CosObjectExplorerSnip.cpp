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

 CosObjectExplorerSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** This snippet allows you to see the contents of an arbitrary indirect cos object. You pass the 
	COS ID and generation number (currently unused) in through the parameter dialog, you can also
	indicate whether you want a brief or extended description to be displayed in DebugWindow.
	You can call into the Console::displayString(CosObj) API to start the process off, 
	this will give you an object that might include references to other CosObjs. You can then use
	this snippet, passing in these references to navigate around the cos layer objects. If you select
	a detailed dump via the command line, all inline cos objects are expanded to their actual values.

	For large object graphs (such as a detailed dump of the whole document) you will overrun the 
	display buffer for debug window. This snippet is intended to be used for pretty well contained
	cos objects. 
	
	@testfile blank.pdf
	@requires any pdf file
*/
void CosObjectExplorerSnip(ParamManager * thePM)
{
	ASInt32 cosID;
	thePM->getNextParamAsInt(cosID);
	ASInt32 genID;
	thePM->getNextParamAsInt(genID);
	string brief;
	thePM->getNextParamAsString(brief);
	CDocument document;
	CosDoc theDoc = (CosDoc)document;
	if (theDoc == NULL){
		return;
	}
	CosObj theObj = CosDocGetObjByID(theDoc, cosID);
	// If it is not "detailed" we assume it is "brief"
	if (brief.compare("detailed")!=0){
		Console::displayString(theObj,true);
	}
	else {
		Console::displayString(theObj,false);	
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(CosObjectExplorerSnip, "COS Layer:Explore Cos Objects", "Pass in cos id, the generation number and brief|detailed \n(separated \
by spaces). The cos object is dumped to the \nCommon SnippetRunner Interface.", "0 0 \"brief\"") 

