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

 PDEPathDrawCurveSnip.cpp

*********************************************************************/

/* 
**
** This snippet demonstrates simple spline curve drawing with 
** PDEPath marking APIs in the following steps:
**
** <ul>
** <li> Acquire the current page form the front-most open document </li>
** <li> Acquire the PDEContent object for the current page </li>
** <li> Create a new empty path </li>
** <li> Set up intended graphic states for the path </li>
** <li> Define and set up the path data </li>
** <li> Add resulting path to the PDEContent </li>
** <li> Set the modified PDEContent object back to the page </li>
** </ul> 
**
	@testfile blank.pdf
	@requires open pdf
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
PDEPath DrawCurve(ASFixed, ASFixed, ASFixed, ASFixed, ASFixed, ASFixed, ASFixed, ASFixed, int, int, int, int);

void PDEPathDrawCurveSnip()
{
	CDocument doc;
	PDPage pdPage = (PDPage)doc;
	if (pdPage == NULL) return;


	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, 0);

	ASFixedRect mediaBox;
	PDPageGetMediaBox(pdPage, &mediaBox);

	// Create the path with location, whether to fill and/or stroke, 
	// line width, whether to stroke, to fill, and RGB color components for the path
	PDEPath rect = DrawCurve(mediaBox.left+ASInt32ToFixed(72*3), mediaBox.bottom+ASInt32ToFixed(72*2), 
		mediaBox.left+ASInt32ToFixed(72), mediaBox.top-ASInt32ToFixed(72*2), 
		mediaBox.right-ASInt32ToFixed(72*2), mediaBox.bottom+ASInt32ToFixed(72*2),
		mediaBox.right-ASInt32ToFixed(72*4), mediaBox.top-ASInt32ToFixed(72*3),
		18, 1, 0, 0);

	// Add the newly created PDEPath object to the PDEContent
	PDEContentAddElem(pdeContent, kPDEAfterLast, (PDEElement)rect);

DURING
	// Set the modified PDEContent Object back to the PDPage
	PDPageSetPDEContentCanRaise(pdPage, 0);
	PDPageNotifyContentsDidChange(pdPage);
	
	Console::displayString("Snippet worked");
	
HANDLER
	PDPageReleasePDEContent(pdPage, 0);
	PDERelease((PDEObject)rect);

	char buf[256];
	ASInt32 err = ERRORCODE;
	ASGetErrorString(err, buf, 256);
	Console::displayString(buf);
END_HANDLER
	// Clean up
	PDPageReleasePDEContent(pdPage, 0);
	PDERelease((PDEObject)rect);
}

/*
**
** Helper function for DrawCurve to set the graphic state of the PDEPath object
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
** an easy interfact for drawing a spline curve on the page
** 
** @param x IN is the x coordinate of the starting point of the curve, in User Space coordinates
** @param y IN is the y coordinate of the starting point of the curve, in User Space coordinates
** @param x1 IN is the x coordinate of the first control point of the curve, in User Space coordinates
** @param y1 IN is the y coordinate of the first control point of the curve, in User Space coordinates
** @param x2 IN is the x coordinate of the second control point of the curve, in User Space coordinates
** @param y2 IN is the y coordinate of the second control point of the curve, in User Space coordinates
** @param x3 IN is the x coordinate of the end point of the curve, in User Space coordinates
** @param y3 IN is the y coordinate of the end point of the curve, in User Space coordinates
** @param lineWidth IN is the intended stroke width for the border of the rectangle
** @param r IN is the intended color value for the red component of the stroking color
** @param g IN is the intended color value for the green component of the stroking color
** @param b IN is the intended color value for the blue component of the stroking color
** @return a PDEPath object to be added to the PDEContent of the current page
*/
PDEPath DrawCurve(ASFixed x, ASFixed y, ASFixed x1, ASFixed y1, ASFixed x2, ASFixed y2, 
				  ASFixed x3, ASFixed y3, int lineWidth, int r, int g, int b)
{
	PDEPath path = PDEPathCreate();

	PDEPathSetPaintOp(path, kPDEStroke);

	PDEGraphicState gState = SetGraphicState(ASInt32ToFixed(lineWidth), 
		ASInt32ToFixed(r), ASInt32ToFixed(g), ASInt32ToFixed(b));
	PDEElementSetGState((PDEElement) path, &gState, sizeof(PDEGraphicState));

	ASFixed pathData[10];
	pathData[0] = kPDEMoveTo;
	pathData[1] = x;
	pathData[2] = y;
	pathData[3] = kPDECurveTo;
	pathData[4] = x1;
	pathData[5] = y1;
	pathData[6] = x2;
	pathData[7] = y2;
	pathData[8] = x3;
	pathData[9] = y3;

	PDEPathSetData(path, pathData, sizeof(pathData));

	return path;
}
SNIPRUN_REGISTER(PDEPathDrawCurveSnip, "PDE Layer:Path:Draw Curve", 
				 "Demonstrates how to draw simple spline curve with PDEPath marking APIs")
