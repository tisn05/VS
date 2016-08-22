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

 PDEPathDrawRectSnip.cpp

*********************************************************************/

/* 
**
** This snippet demonstrates simple rectangle drawing with PDEPath marking APIs
** in the following steps:
**
** <ul>
** <li> Acquire the current page form the front-most open document </li>
** <li> Acquire the PDEContent object for the current page </li>
** <li> Create a new empty path </li>
** <li> Set up line stroking and filling option for the path </li>
** <li> Set up intended graphic states for the path </li>
** <li> Define and set up the path data </li>
** <li> Add resulting path to the PDEContent </li>
** <li> Set the modified PDEContent object back to the page </li>
** </ul> 
**
** The resulting effect is a cyan rectangle with thick red border 
** in the center of the current page
**
	@testfile blank.pdf
	@requires open pdf file
** @see PDEPageAcquirePDEContent
** @see PDEContentAddElem
** @see PDPageSetPDEContentCanRaise
** @see PDPageReleasePDEContent
** @see PDERelease
** @see PDPageRelease
** @see PDEPathCreate
** @see PDEColorSpaceCreateFromName
** @see PDEPathSetPaintOp
** @see PDEElementSetGState
** @see PDEPathSetData
*/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static PDEGraphicState SetGraphicState(ASFixed, ASFixed, ASFixed, ASFixed);
PDEPath DrawRect(ASFixed, ASFixed, ASFixed, ASFixed, int, ASBool, ASBool, int, int, int);

void PDEPathDrawRectSnip()
{
	CDocument doc;
	PDPage pdPage = (PDPage)doc;
	if(pdPage == NULL) return;

	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, 0);

	ASFixedRect mediaBox;
	PDPageGetMediaBox(pdPage, &mediaBox);
	// Create the path with location, whether to fill and/or stroke, 
	// line width, whether to stroke, to fill, and RGB color components for the path
	PDEPath rect = DrawRect(mediaBox.left+ASInt32ToFixed(72), mediaBox.bottom+ASInt32ToFixed(72), 
		mediaBox.right-ASInt32ToFixed(72*2), mediaBox.top-ASInt32ToFixed(72*2), 36, true, false, 1, 0, 0);

	// Add the newly created PDEPath object to the PDEContent
	PDEContentAddElem(pdeContent, kPDEAfterLast, (PDEElement)rect);

DURING
	// Set the modified PDEContent Object back to the PDPage
	PDPageNotifyContentsDidChange(pdPage);
	PDPageSetPDEContentCanRaise(pdPage, 0);
	
	Console::displayString("pde rect path created");
	
HANDLER
	PDPageReleasePDEContent(pdPage, 0);
	PDERelease((PDEObject)rect);

	char buf[256];
	ASInt32 err = ERRORCODE;
	ASGetErrorString(err, buf, 256);
	AVAlertNote(buf);
END_HANDLER
	// Clean up
	PDPageReleasePDEContent(pdPage, 0);
	PDERelease((PDEObject)rect);
}

/*
**
** Helper function for DrawRect to set the graphic state of the PDEPath object
**
*/
static PDEGraphicState SetGraphicState(ASFixed w, ASFixed r, ASFixed g, ASFixed b)
{
	PDEGraphicState gState;
	PDEColorSpec strokeClrSpec, fillClrSpec;
	PDEColorSpace clrSpace;
	PDEColorValue strokeClrValue, fillClrValue;

	memset(&strokeClrValue, 0 , sizeof(PDEColorValue));
	memset(&fillClrValue, 0 , sizeof(PDEColorValue));
	strokeClrValue.color[0] = r;
	strokeClrValue.color[1] = g;
	strokeClrValue.color[2] = b;
	// Use complement color for the fill operation
	fillClrValue.color[0] = fixedOne - r;
	fillClrValue.color[1] = fixedOne - g;
	fillClrValue.color[2] = fixedOne - b;

	// Use RGB color space
	clrSpace = PDEColorSpaceCreateFromName(ASAtomFromString("DeviceRGB"));
	
	strokeClrSpec.space = fillClrSpec.space = clrSpace;
	strokeClrSpec.value = strokeClrValue;
	fillClrSpec.value = fillClrValue;

	memset(&gState, 0, sizeof(PDEGraphicState));
	gState.fillColorSpec = fillClrSpec;
	gState.strokeColorSpec = strokeClrSpec;
	gState.lineWidth = w;
	gState.miterLimit = fixedTen;
	gState.flatness = fixedOne;
	
	return gState;
}

/* 
**
** This function wraps the complexity of PDEPath marking APIs to provide 
** an easy interfact for drawing a rectangle on the page
**
** @param x1 IN is the x coordinate of the lower-left corner of the rect, in User Space coordinates
** @param y1 IN is the y coordinate of the lower-left corner of the rect, in User Space coordinates
** @param width IN is the width of the rectangle to be drawn, in User Space coordinates
** @param height IN is the height of the rectangle to be drawn, in User Space coordinates
** @param lineWidth IN is the intended stroke width for the border of the rectangle
** @param toStroke IN is the boolean value to specify whether to stroke the border of the rectangle
** @param toFill IN is the boolean value to specify whether to fill the interior of the rectangle
** @param r IN is the intended color value for the red component of the stroking color
** @param g IN is the intended color value for the green component of the stroking color
** @param b IN is the intended color value for the blue component of the stroking color
** @return a PDEPath object to be added to the PDEContent of the current page
*/
PDEPath DrawRect(ASFixed x1, ASFixed y1, ASFixed width, ASFixed height, 
				 int lineWidth, ASBool toStroke, ASBool toFill, int r, int g, int b)
{
	PDEPath path = PDEPathCreate();

	ASInt32 paintOp = 0x00;

	if (toStroke)
		paintOp |= kPDEStroke;
	if (toFill)
		paintOp |= kPDEFill;

	PDEPathSetPaintOp(path, paintOp);

	PDEGraphicState gState = SetGraphicState(ASInt32ToFixed(lineWidth), 
		ASInt32ToFixed(r), ASInt32ToFixed(g), ASInt32ToFixed(b));
	PDEElementSetGState((PDEElement) path, &gState, sizeof(PDEGraphicState));

	ASFixed pathData[5];
	pathData[0] = kPDERect;
	pathData[1] = x1;
	pathData[2] = y1;
	pathData[3] = width;
	pathData[4] = height;

	PDEPathSetData(path, pathData, sizeof(pathData));

	return path;
}
SNIPRUN_REGISTER(PDEPathDrawRectSnip, "PDE Layer:Path:Draw Rectangle", 
				 "Demonstrates how to draw simple rectangle with PDEPath marking APIs")
