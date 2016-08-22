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

 AVDocShowAnnotProperties.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static ASCallback gClickCallback = NULL;


ACCB1 ASBool ACCB2 ShowPropertiesClickProc (AVPageView pageView,
		AVDevCoord xHit, AVDevCoord yHit, AVFlagBits16 flags, AVTCount clickNo, void* data)
{
	PDAnnot hitAnnot;
	if(AVPageViewIsAnnotAtPoint (pageView, xHit, yHit, &hitAnnot))
	{
		CDocument document;
		AVDocDoAnnotProperties((AVDoc)document, AVPageViewGetPageNum(pageView), hitAnnot);
	}
	return true;
}


/** 
	When on, shows the properties dialog for any annot clicked on, if there is one.
	
	@see AVDocDoAnnotProperties
	@see AVAppRegisterForPageViewClicks
	@see AVAppUnregisterForPageViewClicks
	@see AVPageViewIsAnnotAtPoint
*/
void AVDocShowAnnotPropertiesSnip()
{
	CDocument document;
	AVDoc avDoc = (AVDoc)document;
	if (avDoc == NULL)	return;
	
	if(SnippetRunnerUtils::toggleSnippetCheck() == false)
	{
		gClickCallback =
			(void*)ASCallbackCreateProto(AVPageViewClickProc, ShowPropertiesClickProc);
		AVAppRegisterForPageViewClicks((AVPageViewClickProc)gClickCallback, NULL);
		AVAlertNote("Click on an annot to see the properties.");
	}
	else
	{
		if(gClickCallback)
		AVAppUnregisterForPageViewClicks((AVPageViewClickProc)gClickCallback);
	}
}

SNIPRUN_REGISTER(AVDocShowAnnotPropertiesSnip, "AV Layer:Document:Toggle Show Annot Properties","When on, a click on an annot will show its properties dialog, if any.")
	

