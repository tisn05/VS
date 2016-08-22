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

 AVPageViewDrawRectSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** Draws a rectangle in the page view. The rectangle is specified in page co-ordinates, 
	and converted to device co-ordinates. The default draws a square in the lower left 
	corner of the document page (assuming the  origin of the page is the lower left corner - the pdf producing application
	defins where the origin of the page is). Acrobat displays the cropped page, so if the page has been cropped, the rectangle is still
	drawn relative to the origin of the page, it may appear that the rectangle is drawn slightly off the page in this instance.
	Note, this snippet does not mark the page. The square only remains until the next screen re-draw.
	@see AVDocGetPageView
	@see AVAppGetActiveDoc
	@see AVDocGetPageView
	@see AVPageViewRectToDevice
	@see AVPageViewSetColor
*/
void AVPageViewDrawRectSnip(ParamManager * thePM){
	CDocument document;
	AVPageView thePageView = (AVPageView)document;
	if (thePageView != NULL){
		ASFixed left,top,right,bottom;
		if ((ASBoolToBool(thePM->getNextParamAsFixed(left)) == true)&&
			(ASBoolToBool(thePM->getNextParamAsFixed(top)) == true) &&
			(ASBoolToBool(thePM->getNextParamAsFixed(right)) == true) &&
			(ASBoolToBool(thePM->getNextParamAsFixed(bottom)) == true)) {
			ASFixedRect rect;
			rect.left = left;
			rect.top = top;
			rect.right = right;
			rect.bottom = bottom;

			PDColorValueRec pdVal;

			pdVal.space=PDDeviceRGB;
			pdVal.value[0]=0;
			pdVal.value[1]=0xffff;
			pdVal.value[2]=0;
			AVDevRect newRect;
			AVPageViewRectToDevice(thePageView,&rect, &newRect);

			AVPageViewSetColor(thePageView,&pdVal);
			AVPageViewDrawRect(thePageView, &newRect);
		}
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(AVPageViewDrawRectSnip, "AV Layer:PageView:Draw Rect", "Draws a temporary green rectangle\
 given 4 values (left top right bottom). The origin is the bottom left corner of the page. The units are points (72 per inch).\
 Default is 1\" square at the origin. The rectangle disappears when the view changes.","0 72 72 0")


