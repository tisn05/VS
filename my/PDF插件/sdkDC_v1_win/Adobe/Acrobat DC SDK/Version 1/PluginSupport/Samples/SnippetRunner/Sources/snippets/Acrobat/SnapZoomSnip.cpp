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

 SnapZoomSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static AVPageViewClickProc gClickCallback = NULL;



/* SnapZoomDoClick
** ------------------------------------------------------
**
** Called to handle main mouse button clicks when our
** tool is active. We modify the zoom by a factor of
** two, then return to the previous view when the
** mouse is released.
**
** Return true if we handled the mouse click.
*/
ACCB1 ASBool ACCB2 SnapZoomDoClick (AVPageView pageView,
AVDevCoord xHit, AVDevCoord yHit, AVFlagBits16 flags, AVTCount clickNo,
void* data){
	ASFixed currentZoom;
	AVDevRect apertureRect;
	AVSDKDependentInteger width, height;
	ASFixedPoint p;
	ASBool bEndedOperation = false;

	DURING
		// Wrap all the page view manipulation so that the changes
		// appear as a single operation on the navigation stack.
		AVPageViewBeginOperation (pageView);

		// Convert mouse position to user space.
		AVPageViewDevicePointToPage(pageView, xHit, yHit, &p);
		currentZoom = AVPageViewGetZoom (pageView);

		// Zoom the page view by a factor of two.
		if (AVSysGetModifiers() & AV_OPTION)
			AVPageViewZoomTo (pageView, AVZoomNoVary, ASFixedMul(currentZoom, fixedHalf));
		else
			AVPageViewZoomTo (pageView, AVZoomNoVary, ASFixedMul(currentZoom, fixedTwo));

		// Convert the mouse postion back to device space. We use
		// this value center the pageview on the mouse click.
		AVPageViewPointToDevice (pageView, &p, &xHit, &yHit);

		// Change the aperture to center on the mouse click. The
		// aperture is the rectangular region of the window in
		// which the document is drawn, specified in device space.
		AVPageViewGetAperture (pageView, &apertureRect);
		width = apertureRect.right - apertureRect.left;
		height = apertureRect.bottom - apertureRect.top;
		AVPageViewScrollTo (pageView, xHit - (width / 2), yHit - (height / 2));

		// End pageview changes and force a redraw of the page.
		AVPageViewEndOperation (pageView);
		bEndedOperation = true;
		AVPageViewDrawNow (pageView);

		// After mouse button is released, zoom back to previous view.
		while (AVSysMouseIsStillDown()) {}
		AVPageViewZoomTo(pageView, AVZoomNoVary, currentZoom);
	HANDLER

		// It's important that we don't corrupt the view history stack.
		if (!bEndedOperation)
			AVPageViewEndOperation (pageView);

		return false;
	END_HANDLER
	return true;
}

/** This snippet registers a page click callback that allows you by clicking 
	to zoom in on a section of the page and then "snap" back to your original view
	when you release the mouse.
   upon releasing the mouse button.
   @see AVPageViewBeginOperation
   @see AVPageViewDevicePointToPage
   @see AVPageViewGetZoom
   @see AVSysGetModifiers
   @see AVPageViewZoomTo
   @see AVPageViewPointToDevice
   @see AVPageViewGetAperture
   @see AVPageViewScrollTo
   @see AVPageViewEndOperation
   @see AVPageViewDrawNow
   @see AVSysMouseIsStillDown
   @see AVPageViewGoBack
   @see AVPageViewEndOperation
   @see ASCallbackCreateProto
   @see AVAppRegisterForPageViewClicks
   @see AVAppUnregisterForPageViewClicks
   @see ASCallbackDestroy
*/
void SnapZoomSnip()
{
	if (SnippetRunnerUtils::toggleSnippetCheck()==false)
	{
		gClickCallback =
			ASCallbackCreateProto(AVPageViewClickProc, SnapZoomDoClick);

		AVAppRegisterForPageViewClicks(gClickCallback, NULL);
	}
	else
	{
		AVAppUnregisterForPageViewClicks((AVPageViewClickProc)gClickCallback);
		ASCallbackDestroy((void*)gClickCallback);
		gClickCallback = NULL;
	}
}


SNIPRUN_REGISTER(SnapZoomSnip, "AV Layer:Document:Snap Zoom", "Zooms in on the mouse location when the mouse is clicked.")


