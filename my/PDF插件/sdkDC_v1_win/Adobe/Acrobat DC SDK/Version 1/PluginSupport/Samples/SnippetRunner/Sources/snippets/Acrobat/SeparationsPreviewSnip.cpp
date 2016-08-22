/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2002-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 SeparationsPreviewSnip.cpp

 On execution of this snippet, a slide-show like sequence is triggered
 to show separations preview of the active page(s) in each colorant 
 enumerated. The default wait time between separations is 3 seconds. 
 A dialog prompts at the end of the preview to signal preview completion. 
 Clicking away the dialog ends the execution.
  
*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

#define MAX_NUM_COLORANTS 16

static int gNumColorants;
static ACCB1 ASBool ACCB2 PageEnumInksProc(PDPageInk, void*);

/**
	This snippet code demonstrates the use of new
	AVPageView, PDPage API funcitons and associated callback
	routines to get color separations preview on a given page.

	The main routine first tries to enumerate inks (colorants)
	available on the given page and then cycles through the
	separations preview for each process color.
	@testfile cmykText.pdf
	@requires pdf with page content
	@see PDPageEnumInks
	@see AVPageViewSetVisibleInks
	@see AVPageViewSetInkPreview
*/

void SeparationsPreviewSnip()
{
	gNumColorants = 0;

	CDocument document;
	AVDoc avDoc = (AVDoc)document;
	if (!avDoc) return;

	AVPageView avPageView = AVDocGetPageView(avDoc);

	PDPage pdPage = AVPageViewGetPage(avPageView);

	PDPageInkRec pageInks[MAX_NUM_COLORANTS];

	// Enumerate inks used on the page
	PDPageEnumInksCallback cbEnumProc = ASCallbackCreateProto(PDPageEnumInksCallback, PageEnumInksProc);
	PDPageEnumInks(pdPage, cbEnumProc, pageInks, false);
	ASCallbackDestroy((void*) cbEnumProc );

	// see what PDPageInks we get back in the SnippetRunner DebugWindow
	char msg[256];
	for (int i=0; i<gNumColorants; i++) {
		sprintf(msg, "%s %d %d %d %2.5e %2.5e",
			ASAtomGetString(pageInks[i].colorantName),
			pageInks[i].r, pageInks[i].g, pageInks[i].b,
			pageInks[i].frequency, pageInks[i].angle);

		cout << msg << endl;
	}
	// request preview mode
	AVPageViewSetInkPreview(avPageView, 1);

	// cycle through the previews for each colorant
	for (int j=0; j<gNumColorants; j++)
	{
			AVPageViewSetVisibleInks(avPageView, 1, &pageInks[j]);
			AVPageViewDrawNow(avPageView);
			sprintf(msg, "%s plate", ASAtomGetString(pageInks[j].colorantName));
			AVAlertNote(msg);
	}

	// turn off preview mode
	AVPageViewSetInkPreview(avPageView, 0);
}

/**
	The PDPageEnumInksCallback routine to enumerate available inks
	on a given page that are to be used for separations preview purpose.

	@param ink IN/OUT is a PDPageInk object found on the page
	@param clientData IN/OUT is the user-supplied data to be processed
	       each time this routine is called; is the clientData passed in
	       to the PDPageEnumInks call
	@return true to continue enumerating inks, false otherwise
	@see PDPageEnumInksCallback
*/
static ACCB1 ASBool ACCB2 PageEnumInksProc(PDPageInk ink, void* clientData)
{
	PDPageInk tmp = (PDPageInk)clientData;

	tmp[gNumColorants].size = ink->size;				
	tmp[gNumColorants].colorantName = ink->colorantName;		
	tmp[gNumColorants].isProcessColor = ink->isProcessColor;		
	tmp[gNumColorants].whatToDo = kEmitPlate;
	tmp[gNumColorants].r = ink->r;
	tmp[gNumColorants].g = ink->g;
	tmp[gNumColorants].b = ink->b;			
	tmp[gNumColorants].frequency = ink->frequency;			
	tmp[gNumColorants].angle = ink->angle;			
	
	// debug output to DebugWindow
	cout << ASAtomGetString(tmp[gNumColorants].colorantName) << endl;

	gNumColorants++;

	return true;
}

SNIPRUN_REGISTER(SeparationsPreviewSnip, "AV Layer:Color Separations:Separations Preview",
				 "Enables previewing separations on current page.")

