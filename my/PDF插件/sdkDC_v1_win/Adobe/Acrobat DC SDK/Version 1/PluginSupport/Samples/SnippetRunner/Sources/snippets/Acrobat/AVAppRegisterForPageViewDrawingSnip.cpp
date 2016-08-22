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

 AVAppRegisterForPageViewDrawingSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------

	Implementation

-------------------------------------------------------*/

typedef struct drawInfo
{
	AVDoc doc;
	ASInt32 pageNum;
	AVPageView pageView;
	ASFixedRect	rect;

} DrawInfoRec;
DrawInfoRec gCurrentDrawInfo;

ACCB1 void ACCB2 myAVAppRegisterForPageViewDrawingProc(AVPageView pageView, 
								AVDevRect* updateRect, void* data)
{
	AVDevRect rect;
	DrawInfoRec* info = (DrawInfoRec*)data;

	//see if right page view being updated
	if((info->doc != AVPageViewGetAVDoc(pageView)) || 
			(info->pageNum != AVPageViewGetPageNum(pageView)))
		return;

	//get scaled and scrolled rect coordinates
	AVPageViewRectToDevice(pageView, &info->rect, &rect);

	PDColorValueRec color;
	color.space = PDDeviceRGB;
	color.value[0] = ASInt32ToFixed(1);
	color.value[1] = 0;
	color.value[2] = 0;

	//set color
	AVPageViewSetColor(pageView, &color);

	//draw rect
	AVPageViewDrawRect(pageView, &rect);
}

/**  
	Registers for the PageViewDrawing callback. Requires an open document.
	When active, draws a box on the center of the current page that 
	persists through scrolling, zooming, switching focus, and mildew buildup.

	Inserting or removing pages will confuse this, but hopefully you are keyed
	on some aspect of the page other than page number, like I am. Also, should
	probably listen for the AVDocClose notification and remove the callback then.

	I have code to make sure I don't crash, but I could avoid that if I was.

	@testfile blank.pdf
	@requires open document
	@see AVAppRegisterForPageViewDrawing
	@see AVAppUnregisterForPageViewDrawingEx
	@see AVDocGetNthPageView
	@see AVPageViewGetPageNum
	@see AVPageViewGetPage
	@see PDPageGetMediaBox
	@see AVDocIsDead
	@see AVPageViewPageNumIsVisible
	@see AVPageViewRectToDevice
	@see AVPageViewInvalidateRect
	@see AVPageViewDrawNow
	@see AVPageViewGetAVDoc
	@see AVPageViewGetPageNum
	@see AVPageViewSetColor
	@see AVPageViewDrawRect
*/

void AVAppRegisterForPageViewDrawingSnip()
{
	static AVPageViewDrawProc myDrawingProc = ASCallbackCreateProto(AVPageViewDrawProc, myAVAppRegisterForPageViewDrawingProc);

	if (SnippetRunnerUtils::toggleSnippetCheck()==false)
	{
			CDocument document;
			AVDoc doc  = (AVDoc)document;
			if (doc == NULL){
				return;
			}

		//get the info I need to draw same sized square on right page
		gCurrentDrawInfo.doc = doc;
		AVPageView pageView = AVDocGetNthPageView(doc, 0);
		gCurrentDrawInfo.pageView = pageView;
		gCurrentDrawInfo.pageNum = AVPageViewGetPageNum(pageView);
		PDPage pdPage = AVPageViewGetPage(pageView);
		ASFixedRect bbox;

		PDPageGetMediaBox(pdPage, &bbox);

		//want rect that is quarter size of page, centered.
		gCurrentDrawInfo.rect.top = 3*bbox.top/4;
		gCurrentDrawInfo.rect.bottom = bbox.top/4;
		gCurrentDrawInfo.rect.right = 3*bbox.right/4;
		gCurrentDrawInfo.rect.left = bbox.right/4;

		AVAppRegisterForPageViewDrawing( myDrawingProc, &gCurrentDrawInfo);		
		
		//draw the first time directly
		myAVAppRegisterForPageViewDrawingProc(pageView, NULL, &gCurrentDrawInfo);
		AVAlertNote("registered for page view drawing");
	} else {
		AVAppUnregisterForPageViewDrawingEx(myDrawingProc, &gCurrentDrawInfo);
		AVAlertNote("unregistered for page view drawing");

		//force redraw if page is still visible
		//make sure doc is open and page is visible

		if(!AVDocIsDead(gCurrentDrawInfo.doc)) 
		{
			if(AVPageViewPageNumIsVisible(gCurrentDrawInfo.pageView, 
											gCurrentDrawInfo.pageNum))
			{
				AVDevRect rect;
				AVPageViewRectToDevice(gCurrentDrawInfo.pageView, &gCurrentDrawInfo.rect, &rect);
				AVPageViewInvalidateRect(gCurrentDrawInfo.pageView, &rect);
				AVPageViewDrawNow(gCurrentDrawInfo.pageView);
			}
		}
	}
}

SNIPRUN_REGISTER(AVAppRegisterForPageViewDrawingSnip, "Callback:Application:PageView Drawing", "Registers for the PageView Draw Page callback.")
