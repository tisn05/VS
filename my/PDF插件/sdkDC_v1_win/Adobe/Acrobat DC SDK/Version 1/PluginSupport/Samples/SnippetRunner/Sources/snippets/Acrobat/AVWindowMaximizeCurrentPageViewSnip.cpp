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

 AVWindowMaximizeCurrentPageView.cpp

*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"


/** This snippet finds the AVWindow that contains the current pageview, and maximizes it.
	It closes the other windows that share the same AVDoc.
	
	The ability to have multiple windows that contain the same AVDoc is new for Acrobat 7.

	@see AVPageViewGetAVWindow
	@see AVDocGetNumWindows
	@see AVDocGetNthWindow
	@see AVWindowMaximize
	@see AVWindowUserClose
*/
void AVWindowMaximizeCurrentPageViewSnip()
{
	CDocument document;
	
	AVDoc avDoc = (AVDoc)document;
	if(avDoc == NULL)
		return;

	AVPageView currentPageView = (AVPageView)document;
	AVWindow currentPageViewWindow = AVPageViewGetAVWindow(currentPageView);
	
	ASCount numWindows = AVDocGetNumWindows(avDoc);
	
	//since we are closing, count down to keep indexes valid.
	for(int i = numWindows-1; i>=0; i--)
	{
		AVWindow win = AVDocGetNthWindow(avDoc, i);
		if(win == currentPageViewWindow)
			AVWindowMaximize(win, true);
		else
			AVWindowUserClose(win, false);
	}
	
}

/* Register snippet */
SNIPRUN_REGISTER(AVWindowMaximizeCurrentPageViewSnip, "AV Layer:PageView:Maximize Current PageView",
				 "Find the AVWindow that contains the current pageview, and maximizes it.")

