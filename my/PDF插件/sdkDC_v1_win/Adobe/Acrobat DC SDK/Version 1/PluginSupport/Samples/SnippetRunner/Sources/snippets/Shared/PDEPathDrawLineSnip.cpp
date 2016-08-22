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

 PDEPathDrawLineSnip.cpp

*********************************************************************/

/* 
**
** This snippet demonstrates simple line drawing with PDEPath marking APIs
** in the following steps:
**
** <ul>
** <li> Acquire the current page form the front-most open document </li>
** <li> Acquire the PDEContent object for the current page </li>
** <li> Create a new empty path </li>
** <li> Set up line stroking option for the path </li>
** <li> Set up intended graphic states for the line path </li>
** <li> Add path segment to form the line path </li>
** <li> Add resulting path to the PDEContent </li>
** <li> Set the modified PDEContent object back to the page </li>
** </ul> 
**
** The resulting effect is a big blue cross mark in the center of the current page
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
** @see PDEPathAddSegment
*/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static PDEGraphicState SetGraphicState(ASFixed, ASFixed, ASFixed, ASFixed);
PDEPath DrawLine(ASFixed, ASFixed, ASFixed, ASFixed, int, int, int, int);

void PDEPathDrawLineSnip()
{
	CDocument doc;
	PDPage pdPage = (PDPage)doc;
	if(pdPage == NULL) return;

	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, 0);

	ASFixedRect mediaBox;
	PDPageGetMediaBox(pdPage, &mediaBox);
	// Create the path with location, whether to fill and/or stroke, 
	// line width, and RGB color components for the path
	PDEPath lineOne = DrawLine(mediaBox.left+ASInt32ToFixed(72), mediaBox.bottom+ASInt32ToFixed(72), 
		mediaBox.right-ASInt32ToFixed(72), mediaBox.top-ASInt32ToFixed(72), 3, 0, 0, 1);
	PDEPath lineTwo = DrawLine(mediaBox.left+ASInt32ToFixed(72), mediaBox.top-ASInt32ToFixed(72), 
		mediaBox.right-ASInt32ToFixed(72), mediaBox.bottom+ASInt32ToFixed(72), 3, 0, 0, 1);

	// Add the newly created PDEPath (line) object to the PDEContent
	PDEContentAddElem(pdeContent, kPDEAfterLast, (PDEElement)lineOne);
	PDEContentAddElem(pdeContent, kPDEAfterLast, (PDEElement)lineTwo);
DURING
	// Set the modified PDEContent Object back to the PDPage
	PDPageSetPDEContentCanRaise(pdPage, 0);
	PDPageNotifyContentsDidChange(pdPage);
	
	Console::displayString("pde line path created");
HANDLER
	PDPageReleasePDEContent(pdPage, 0);
	PDERelease((PDEObject)lineOne);
	PDERelease((PDEObject)lineTwo);

	char buf[256];
	ASInt32 err = ERRORCODE;
	ASGetErrorString(err, buf, 256);
	AVAlertNote(buf);
END_HANDLER
	// Clean up
	PDPageReleasePDEContent(pdPage, 0);
	PDERelease((PDEObject)lineOne);
	PDERelease((PDEObject)lineTwo);
}

/*
**
** Helper function for DrawLine to set the graphic state of the PDEPath object
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
	fillClrValue.color[0] = fixedOne - r;
	fillClrValue.color[1] = fixedOne - g;
	fillClrValue.color[2] = fixedOne - b;

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
** an easy interfact for drawing a single line on the page
**
** @param x1 IN is the x coordinate of the starting point of the line, in User Space coordinates
** @param y1 IN is the y coordinate of the starting point of the line, in User Space coordinates
** @param x2 IN is the x coordinate of the ending point of the line, in User Space coordinates
** @param y1 IN is the y coordinate of the ending point of the line, in User Space coordinates
** @param width IN is the intended line width
** @param r IN is the intended color value for the red component
** @param g IN is the intended color value for the green component
** @param b IN is the intended color value for the blue component
**
*/
PDEPath DrawLine(ASFixed x1, ASFixed y1, ASFixed x2, ASFixed y2, int width, int r, int g, int b)
{
	PDEPath path = PDEPathCreate();
	
	PDEPathSetPaintOp(path, kPDEStroke);

	PDEGraphicState gState = SetGraphicState(ASInt32ToFixed(width), ASInt32ToFixed(r), ASInt32ToFixed(g), ASInt32ToFixed(b));
	PDEElementSetGState((PDEElement) path, &gState, sizeof(PDEGraphicState));

	PDEPathAddSegment(path, kPDEMoveTo, x1, y1, fixedZero, fixedZero, fixedZero, fixedZero);
	PDEPathAddSegment(path, kPDELineTo, x2, y2,	fixedZero, fixedZero, fixedZero, fixedZero);

	return path;
}
SNIPRUN_REGISTER(PDEPathDrawLineSnip, "PDE Layer:Path:Draw Line", 
				 "Demonstrates how to draw lines with PDEPath marking APIs")
