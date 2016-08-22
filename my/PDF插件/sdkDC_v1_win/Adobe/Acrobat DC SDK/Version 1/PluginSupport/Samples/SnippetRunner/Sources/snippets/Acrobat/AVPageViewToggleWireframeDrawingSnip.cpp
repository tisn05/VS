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

 AVPageViewToggleWireframeDrawing.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** 
	Toggles the state of wireframe drawing.
	
	@see AVPageViewGetWireframeDrawing
	@see AVPageViewSetWireframeDrawing
*/
void AVPageViewToggleWireframeDrawingSnip()
{
	CDocument document;
	if(((AVPageView)document) == NULL)
		return;
	if(AVPageViewGetWireframeDrawing((AVPageView) document))
	{
		AVPageViewSetWireframeDrawing((AVPageView) document, false);
		AVAlertNote("Turning Wireframe Drawing off.");
	}
	else
	{
		AVPageViewSetWireframeDrawing((AVPageView) document, true);
		AVAlertNote("Turning Wireframe Drawing on.");
	}
	
	//not common design pattern, but I can get current toggle from the 'Get' call.
	SnippetRunnerUtils::toggleSnippetCheck();
}

SNIPRUN_REGISTER(AVPageViewToggleWireframeDrawingSnip, "AV Layer:PageView:Toggle Wireframe Drawing","Toggle Wireframe Drawing Mode.")


