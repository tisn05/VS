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

 PDEPathExplorerSnip.cpp

*********************************************************************/

/* 
**
** This snippet explores PDEPath objects on the current page by breaking
** them down to PDEPath marking operators and operand(s):
**
** <ul>
** <li> Acquire PDEContent object </li>
** <li> Enumerate (up to 10) PDEElement objects on the current page </li>
** <li> For evey PDEElement that is of type PDEPath, display the PDEPath
** marking operators and associated operand(s) </li>
** </ul>
**
	@testfile ShapeTest.pdf
	@requires open pdf with path content
** @see PDEPageAcquirePDEContent
** @see PDEContentGetElem
** @see PDEContentGetNumElems
** @see PDPageSetPDEContentCanRaise
** @see PDPageReleasePDEContent
** @see PDERelease
** @see PDPageRelease
** @see PDEPathGetData
** @see PDEObjectGetType
** @see PDEClipGetNumElems
** @see PDEClipGetElem
**
*/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static void EnumeratePDEElems(PDEElement);
static int outputLimit;

void PDEPathExplorerSnip()
{
	CDocument doc;
	//page is owned by CDocument object. Don't release it.
	PDPage pdPage = (PDPage)doc;
	if (pdPage == NULL) return;
	PDEContent pdeContent = PDPageAcquirePDEContent(pdPage, 0);
	PDEElement pdeElem = NULL;

DURING
	ASInt32 numElems = PDEContentGetNumElems(pdeContent);

	// Limit the Debug Window output to 200 lines to avoid
	// automatic reset on reaching default text length
	for (int i=0; i<numElems; i++)
	{
		if ((i+1)*outputLimit > 200) break;
		pdeElem = PDEContentGetElem(pdeContent, i);
		EnumeratePDEElems(pdeElem);
	}
HANDLER
	PDPageReleasePDEContent(pdPage, 0);

	char buf[256];
	ASInt32 err = ERRORCODE;
	ASGetErrorString(err, buf, 256);
	AVAlertNote(buf);
END_HANDLER
	// Clean up
	outputLimit = 0;
	PDPageReleasePDEContent(pdPage, 0);
}

static void EnumeratePDEElems(PDEElement pdeElem)
{
	ASInt32* data;
	ASInt32 dataSize;
	char msg[128];

	if (PDEObjectGetType((PDEObject)pdeElem) == kPDEPath)
	{
		Console::displayString("------------------------\n");
		Console::displayString("Operator: Operand(s)\n");
		dataSize = PDEPathGetData((PDEPath)pdeElem, NULL, 0);
		data = (ASInt32*)ASmalloc(dataSize*sizeof(ASInt32));
		PDEPathGetData((PDEPath)pdeElem, data, dataSize);
	
		// Parse PDEPath object data array for operators and associated operands
		int i = 0;
		while (i<dataSize)
		{
			memset(msg, 0, sizeof(msg));

			switch(data[i]) {
			case kPDEMoveTo:
				sprintf(msg, "m(moveto): (%d, %d)\n", 
					ASFixedRoundToInt16(data[i+1]), ASFixedRoundToInt16(data[i+2]));
				Console::displayString(msg);
				i+=3;
				break;
			case kPDELineTo:
				sprintf(msg, "l(lineto): (%d, %d)\n", 
					ASFixedRoundToInt16(data[i+1]), ASFixedRoundToInt16(data[i+2]));
				Console::displayString(msg);
				i+=3;
				break;
			case kPDECurveTo:
				sprintf(msg, "c(curveto): (%d, %d), (%d, %d), (%d, %d)\n",
					ASFixedRoundToInt16(data[i+1]), ASFixedRoundToInt16(data[i+2]),
					ASFixedRoundToInt16(data[i+3]), ASFixedRoundToInt16(data[i+4]),
					ASFixedRoundToInt16(data[i+5]), ASFixedRoundToInt16(data[i+6]));
				Console::displayString(msg);
				i+=7;
				break;
			case kPDECurveToV:
				sprintf(msg, "v(curveto): (%d, %d), (%d, %d)\n",
					ASFixedRoundToInt16(data[i+1]), ASFixedRoundToInt16(data[i+2]),
					ASFixedRoundToInt16(data[i+3]), ASFixedRoundToInt16(data[i+4]));
				Console::displayString(msg);
				i+=5;
				break;
			case kPDECurveToY:
				sprintf(msg, "y(curveto): (%d, %d), (%d, %d)\n",
					ASFixedRoundToInt16(data[i+1]), ASFixedRoundToInt16(data[i+2]),
					ASFixedRoundToInt16(data[i+3]), ASFixedRoundToInt16(data[i+4]));
				Console::displayString(msg);
				i+=5;
				break;
			case kPDERect:
				sprintf(msg, "re: (%d, %d), (%d, %d)\n",
					ASFixedRoundToInt16(data[i+1]), ASFixedRoundToInt16(data[i+2]),
					ASFixedRoundToInt16(data[i+3]), ASFixedRoundToInt16(data[i+4]));
				Console::displayString(msg);
				i+=5;
				break;
			case kPDEClosePath:
				sprintf(msg, "h(closepath): none\n");
				Console::displayString(msg);
				i++;
				break;
			default:
				i++;
				ASfree(data);
				return;
			}
			outputLimit++;
		}

		ASfree(data);
	} else {
		if (PDEObjectGetType((PDEObject)pdeElem) == kPDEContainer) 
		{
			PDEContent pdeContent = PDEContainerGetContent((PDEContainer)pdeElem);
			PDEElement elem;

			ASInt32 numElems = PDEContentGetNumElems(pdeContent);
			for (int i=0; i<numElems; i++) 
			{
				Console::displayString("----- PDEContainer -----\n");
				// Recursively enumerates PDEPath object contained in PDEContainer objects.
				elem = PDEContentGetElem(pdeContent, i);
				EnumeratePDEElems(elem);
			}
		}
	} 
}

SNIPRUN_REGISTER(PDEPathExplorerSnip, "PDE Layer:Path:Explore PDEPath Objects", 
				 "Enumerating PDEPath objects for operators and associated operands")
