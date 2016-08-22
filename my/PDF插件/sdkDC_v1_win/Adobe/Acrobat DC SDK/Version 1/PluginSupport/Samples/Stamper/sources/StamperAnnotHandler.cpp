/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/ 
/**
\file StamperAnnotHandler.cpp

 - Implementation of the Stamper's annotation-related routines.

*********************************************************************/
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif
#include "StamperAnnotHandler.h"
#include "DebugWindowHFT.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

extern ASAtom Annotation_K;

ASAtom FaceNormal_K;
ASAtom FaceRollover_K;
ASAtom FaceDown_K;

char *State_KStr;
char *Appearance_KStr;
char *Flags_KStr;
char *Length_KStr;

// The annotation handler callback record
AVAnnotHandlerRec gStamperAH;

/*-------------------------------------------------------
	Utility Methods
-------------------------------------------------------*/

/* PDAnnotGetState
** ------------------------------------------------------
** */
/** @param pdan the annotation whose state you want
 **
 ** @return the current state specified by the AS key in the annotation's
 ** dictionary. Returns ASAtomNull if no AS key found.
 **
 ** @see PDAnnotIsValid
 ** @see PDAnnotGetCosObj
 ** @see CosDictGetKeyString
 ** @see CosNameValue
 */
ASAtom PDAnnotGetState(PDAnnot pdan)
{
	ASAtom asaState = ASAtomNull;

	if (PDAnnotIsValid(pdan)) {
		DURING
			CosObj co = PDAnnotGetCosObj(pdan);
			if (CosDictKnownKeyString(co, State_KStr)) {
				co = CosDictGetKeyString(co, State_KStr);
				asaState = CosNameValue(co);
			}
		HANDLER
			asaState = ASAtomNull;
		END_HANDLER
	}
	else
		asaState = ASAtomNull;
	return asaState;
}

/* PDAnnotGetApppearance
** ------------------------------------------------------
** */ 
/** Returns the appearance CosStream for the annotation in question.
 **
 ** We handle two forms here. In the simple case there is a single face for
 ** each state (e.g. N, R, D, X are streams). In the complex case, there are
 ** several faces for each appearance, referenced by name inside the
 ** N, R, D, and X dicts. You can get the sub-face name by using PDAnnotGetState().
 ** 
 ** @param pdan the annotation in question
 ** @param asaFace indicates which face (Normal, Rollover, Down, etc.) is to be used
 ** @param asaSubFace indicates which subface to use.  If asaSubFace is ASAtomNull, then the asaFace
 ** is assumed to only have one face and therefore it is returned as a CosStream.
 **
 ** @return CosObj appearance CosStream of pdan
 **
 ** @see PDAnnotIsValid
 ** @see CosDictKnownKeyString
 ** @see CosDictGetKeyString
 ** @see CosObjGetType
 ** @see CosDictKnown
 ** @see CosDictGet
 ** @see CosNewNull
 */
CosObj PDAnnotGetAppearance(PDAnnot pdan, ASAtom asaFace, ASAtom asaSubFace)
{
	CosObj co;
	ASBool bOk = false;

	if (PDAnnotIsValid(pdan) && asaFace != ASAtomNull) {
		DURING
			co = PDAnnotGetCosObj(pdan);
			if (CosDictKnownKeyString(co, Appearance_KStr)) {
				co = CosDictGetKeyString(co, Appearance_KStr);
				if (CosDict == CosObjGetType(co) && CosDictKnown(co, asaFace)) {
					co = CosDictGet(co, asaFace);
					if (asaSubFace != ASAtomNull) {
						if (CosObjGetType(co) == CosDict &&	CosDictKnown(co, asaSubFace)) {
							bOk = true;
							co = CosDictGet(co, asaSubFace);
						} else if (CosObjGetType(co) == CosStream)
							bOk = true;
					} else
						bOk = true;
				}
			}
		HANDLER
			bOk = false;
		END_HANDLER
	}
	else
		bOk = false;

	return bOk ? co : CosNewNull();
}

/* AnnotIsSelected
** ------------------------------------------------------
** */ 
/** Returns true if the given annotation is selected in the AVDoc
 ** corresponding to the given AVPageView.
 **
 ** @see AVPageViewGetAVDoc
 ** @see AVDocGetSelection
 ** @see PDAnnotEqual
 */
static ASBool AnnotIsSelected(AVPageView pageView, PDAnnot annot)
{
	AVDoc doc = AVPageViewGetAVDoc(pageView);
	ASAtom selType = AVDocGetSelectionType(doc);
	ASBool result = false;

	/* Make sure the selection is of the right type
	*/
	if (selType == Annotation_K)
	{
		PDAnnot *selection = (PDAnnot *)AVDocGetSelection(doc);
		if (selection)
			/* Compare the selection.  Don't compare objects directly,
			** use the provided comparison functions instead!
			*/
			result = PDAnnotEqual(annot, *selection);
	}

	return result;
}

/* AVDevRectSet
** ------------------------------------------------------
** */ 
/** Utility routine for setting the fields of an AVDevRect.
 */
static void AVDevRectSet(AVDevRect *avdRect, Int16 left, Int16 top, Int16 right,
	Int16 bottom)
{
	avdRect->left = left;
	avdRect->top = top;
	avdRect->right = right;
	avdRect->bottom = bottom;
}

/* AVDevRectEqual
** ------------------------------------------------------
** */ 
/** Utility routine for comparing two AVDevRects.
 */
static ASBool AVDevRectEqual(const AVDevRect *avdr1, const AVDevRect *avdr2)
{
	return (avdr1->left == avdr2->left) && (avdr1->top == avdr2->top)
		&& (avdr1->right == avdr2->right) && (avdr1->bottom == avdr2->bottom);
}

/* FixedRectSet
** ------------------------------------------------------
** */ 
/** Utility routine for setting the fields of a FixedRect.
 */
static void FixedRectSet(ASFixedRect *fixedRect, ASFixed left, ASFixed top, ASFixed right,
	ASFixed bottom)
{
	fixedRect->left = left;
	fixedRect->top = top;
	fixedRect->right = right;
	fixedRect->bottom = bottom;
}

/*-------------------------------------------------------
	AnnotationHandler Callbacks
-------------------------------------------------------*/

/* AnnotGetViewBBox
** ------------------------------------------------------
** */ 
/** Returns the rectangle (in device space) of a given Stamper annotation.
 */
static ACCB1 void ACCB2 AnnotGetViewBBox(AVAnnotHandler annotHandler,
	AVPageView pageView, PDAnnot annot, AVDevRect *bbox)
{
	ASFixedRect pdRect;

	if (gDebugWindowHFT)
		DebugEmitString("AVAnnotHandlerGetAnnotViewBBoxProc");

	PDAnnotGetRect(annot, &pdRect);
	AVPageViewRectToDevice(pageView, &pdRect, bbox);
}

/* AnnotDraw
** ------------------------------------------------------
** */ 
/** AVAnnotHandler callback for drawing the Stamper annotation.  Calls
 ** PDAnnotGetState() to get the current state of the Annotation (from the
 ** AS key), then calls PDAnnotGetAppearance() to get the current appearance
 ** from the AP key), then gets the annotation rectangle with AVPageViewGetAnnotRect()
 ** and draws the annotation from the data stored in the annotations appearance
 ** dictionary with AVPageViewDrawCosObj()
 **
 ** @see AVPageViewGetAnnotRect
 ** @see AVPageViewDrawCosObj
 ** @see PDAnnotGetState
 ** @see AVPageViewSetColor
 ** @see AVPageViewDrawRectOutlineWithHandles
 ** @see AVPageViewDrawRectOutline
 */
static ACCB1 void ACCB2 AnnotDraw(AVAnnotHandler handler, PDAnnot annot, AVPageView pageView)
{
	ASAtom coState;
	CosObj coApp;
	AVDevRect avdr;

	if(gDebugWindowHFT)
		DebugEmitString("AVAnnotHandlerDrawProc");

	coState = PDAnnotGetState(annot);
	coApp = PDAnnotGetAppearance(annot, FaceNormal_K, coState);
	AVPageViewGetAnnotRect(pageView, annot, &avdr);
	AVPageViewDrawCosObj(pageView, coApp, &avdr);

	if(gStamperToolSelected)
		if(AnnotIsSelected(pageView, annot))
		{
			PDColorValueRec red;
			/* Define red */
			red.space = PDDeviceRGB;
			red.value[0] = ASInt32ToFixed(1);
			red.value[1] = 0;
			red.value[2] = 0;
			AVPageViewSetColor (pageView, &red);
			AVPageViewDrawRectOutlineWithHandles (pageView, &avdr, true, true,
							NULL, 0);
		}
		else
		{
			PDColorValueRec black;
			/* Define red */
			black.space = PDDeviceRGB;
			black.value[0] = 0;
			black.value[1] = 0;
			black.value[2] = 0;
			AVPageViewSetColor (pageView, &black);
			AVPageViewDrawRectOutline(pageView, &avdr, 1, NULL, 0);
		}
}


/* AnnotGetType
** ------------------------------------------------------
** */ 
/** Returns the Stamper annotation handler's annotation subtype
 */
static ACCB1 ASAtom ACCB2 AnnotGetType(AVAnnotHandler handler)
{
	if (gDebugWindowHFT)
		DebugEmitString("AVAnnotHandlerGetTypeProc");

	return Stamper_K;
}

/* AnnotAdjustCursor
** ------------------------------------------------------
** */ 
/** If the cursor is over a Stamper annotation, set it to the default
 ** (arrow) cursor.
 **
 ** @return true if cursor adjusted
 */
static ACCB1 ASBool ACCB2 AnnotAdjustCursor(AVAnnotHandler handler,
	PDAnnot annot, AVPageView pageView, AVDevCoord x, AVDevCoord y)
{
	if (gDebugWindowHFT)
		DebugEmitString("AVAnnotHandlerAdjustCursorProc");

	AVSysSetCursor(NULL);

	/* Returning true here indicates that the Stamper annotation
	** does indeed care about the cursor's appearance, and did
	** set it accordingly.
	*/
	return true;
}

/* AnnotGetLayer
** ------------------------------------------------------
** */ 
/** What layer is the stamp at?
 */
static ACCB1 ASFixed ACCB2 AnnotGetLayer(AVAnnotHandler annotHandler, PDAnnot annot)
{
	if (gDebugWindowHFT)
		DebugEmitString("AVAnnotHandlerGetLayerProc");

	return LINK_LAYER;
}

/* AnnotDoClick
** ------------------------------------------------------
** */ 
/** A click was detected in the Stamper annotation.  Select it if
** it was not already selected, and drag it around if the user
 ** holds the mouse down.
 **
 ** @return true if click handled
 **
 ** @see AVDocSetSelection
 ** @see AVPageViewGetAVDoc
 ** @see AVSysMouseIsStillDown
 ** @see AVRectHandleHitTest
 ** @see AVPageViewDragRect
 ** @see AVPageViewDeviceRectToPage
 ** @see AVPageViewDragRect
 ** @see PDAnnotSetRect
 */
static ACCB1 ASBool ACCB2 AnnotDoClick(AVAnnotHandler handler,
	PDAnnot annot, AVPageView pageView, AVDevCoord x, AVDevCoord y,
	AVFlagBits16 flags, AVTCount clickNo)
{
	if (gDebugWindowHFT)
		DebugEmitString("AVAnnotHandlerDoClickProc");

	// If we aren't active don't do anything
	if(!gStamperToolSelected)
		return false;

	// Only select the annotation if it isn't already selected.
	if (!AnnotIsSelected(pageView, annot))
	{
		PDAnnot *anAnnot = (PDAnnot *)ASmalloc(sizeof(annot));

		*anAnnot = annot;
		AVDocSetSelection(AVPageViewGetAVDoc(pageView),
			Annotation_K, anAnnot, true);
	}

	// Don't enter the drag loop if the user has already let go of the mouse.
	if (AVSysMouseIsStillDown())
	{
		AVDevRect oldLocation, newLocation;

		/* Get the old location of the annotation in device space.  This
		** is needed by AVPageViewDragRect to know where to begin dragging.
		** It is also used to determine the size of the rectangle to be
		** dragged.
		*/
		AnnotGetViewBBox(handler, pageView, annot, &oldLocation);

		/* Call AVPageViewDragRect.  This tracks the mouse position, dragging
		** a gray rect around underneath it while the mouse button is still down.
		** The selector code 0 (there should be a macro, but there isn't yet)
		** indicates we are moving the entire rectangle, rather than simply
		** dragging a corner.  Since dragging is constrained only by the bounds
		** of the AVPageView, we can pass NULL for the extrema.
		*/

		ASInt16 hitLocation  = AVRectHandleHitTest (&oldLocation, true, x, y);
		AVPageViewDragRect(pageView, x, y, &oldLocation, &newLocation, hitLocation, NULL);

		/* If oldLocation == newLocation, there is no need to change the annotation's
		** position.
		*/
		if (!AVDevRectEqual(&oldLocation, &newLocation))
		{
			ASFixedRect newPDLocation;

			/* Convert the newLocation (in device space) to user space, and call
			** PDAnnotSetRect.  The viewer takes care of the screen update.
			*/
			AVPageViewDeviceRectToPage(pageView, &newLocation, &newPDLocation);
			PDAnnotSetRect(annot, &newPDLocation);
		}
	}

	return true;
}
/* AnnotNotifyAddedToSelection
** ------------------------------------------------------
** */ 
/** Draw the knobs on the selected annotation.
 **
 ** @see AVPageViewGetPage
 ** @see PDPageGetAnnotIndex
 ** */
static ACCB1 void ACCB2 AnnotNotifyAddedToSelection(AVAnnotHandler handler, PDAnnot annot, AVPageView pageView)
{
	/* We might have been selected programatically, so we shouldn't assume
	** the annotation is on the current page.  The easy way to check for this
	** is to ask for the PDPage's index of this PDAnnot.
	*/
	PDPage pdPage = AVPageViewGetPage(pageView);
	Int32 index = PDPageGetAnnotIndex(pdPage, annot);

	if (gDebugWindowHFT)
		DebugEmitString("AVAnnotHandlerNotifyAnnotAddedToSelectionProc");

	// If index == -1, the annot is not on this page.  Don't draw anything.
	if (index != -1)
	{
		AVDevRect annotRect;
		AnnotGetViewBBox(handler, pageView, annot, &annotRect);
		AnnotDraw( NULL, annot, pageView);
	}
}

/* AnnotNotifyRemovedFromSelection
** ------------------------------------------------------
** */ 
/** Erase the knobs on the selected annotation.
 */
static ACCB1 void ACCB2 AnnotNotifyRemovedFromSelection(AVAnnotHandler handler,
	PDAnnot annot, AVPageView pageView)
{
	PDPage pdPage = AVPageViewGetPage(pageView);
	Int32 index = PDPageGetAnnotIndex(pdPage, annot);

	if (gDebugWindowHFT)
		DebugEmitString("AVAnnotHandlerNotifyAnnotRemovedFromSelectionProc");

	// Make sure the annotation is on the current page.
	if (index != -1)
	{
		AVDevRect annotRect;
		AnnotGetViewBBox(handler, pageView, annot, &annotRect);
		AnnotDraw( NULL, annot, pageView);
	}
}

/* AnnotDoKeyDownEx
** ------------------------------------------------------
** */ 
/** Handle the delete key by creating an undo record
 ** but return false to get default deletion handling
 **
 ** @return true if key handled
 **
 ** @see AVAppShouldKeyDeleteObject
 ** @see AVUndoNew
 ** @see AVPageViewGetAVDoc
 */
static ACCB1 ASBool ACCB2 AnnotDoKeyDownEx(AVAnnotHandler annotHandler, PDAnnot annot,
AVPageView pageView, AVKeyCode key, AVFlagBits16 flags)
{
	//delete, if the right key
	if(AVAppShouldKeyDeleteObject (key, flags)) 
	{
		StamperUndoDataPtr  undoDataPtr = (StamperUndoDataPtr)ASmalloc(sizeof(StamperUndoDataRec));
		undoDataPtr->annot = annot;
		undoDataPtr->undoType = kDeleteStamperAnnot;
		undoDataPtr->pageView = pageView;
		//store rect so we can undo in the right place
		gStamperAH.GetAnnotViewBBox(&gStamperAH, pageView, annot, &undoDataPtr->rect);
		AVUndoNew(AVPageViewGetAVDoc(pageView), &gUndoHandler, undoDataPtr);
	}
	return false;
}

/*-------------------------------------------------------
	AnnotationHandler Initialization
-------------------------------------------------------*/

/* SetUpStamperAnnotHandler
** ------------------------------------------------------
** */ 
/** Initialize atoms and set up annotation callbacks.
 */
void SetUpStamperAnnotHandler(void)
{

	//use strings when possible
	State_KStr = "AS";
	Appearance_KStr = "AP";
	Flags_KStr = "F";
	Length_KStr = "Length";

	//ASAtoms otherwise
	FaceNormal_K = ASAtomFromString("N");
	FaceRollover_K = ASAtomFromString("R");
	FaceDown_K = ASAtomFromString("D");
	Annotation_K = ASAtomFromString("Annotation");


	// zero out the unused fields of the Stamper annotation handler.
	memset(&gStamperAH, 0, sizeof(gStamperAH));

	// Set up the size field of the Stamper annotation.  If you don't do
	// this your plug-in won't load with future versions of the viewer!
	gStamperAH.size = sizeof(gStamperAH);

	// Stamper annotations appear in the same layer as link annotations.
	gStamperAH.flags = LINK_LAYER;

	// Set up the callbacks.
	gStamperAH.GetType = ASCallbackCreateProto(AVAnnotHandlerGetTypeProc, &AnnotGetType);
	gStamperAH.Draw = ASCallbackCreateProto(AVAnnotHandlerDrawProc, &AnnotDraw);
	gStamperAH.GetAnnotViewBBox = ASCallbackCreateProto(AVAnnotHandlerGetAnnotViewBBoxProc,
		&AnnotGetViewBBox);
	gStamperAH.AdjustCursor = ASCallbackCreateProto(AVAnnotHandlerAdjustCursorProc,
		&AnnotAdjustCursor);
	gStamperAH.DoClick = ASCallbackCreateProto(AVAnnotHandlerDoClickProc, &AnnotDoClick);
	gStamperAH.NotifyAnnotAddedToSelection = ASCallbackCreateProto(AVAnnotHandlerNotifyAnnotAddedToSelectionProc,
		&AnnotNotifyAddedToSelection);
	gStamperAH.NotifyAnnotRemovedFromSelection = ASCallbackCreateProto(AVAnnotHandlerNotifyAnnotRemovedFromSelectionProc,
		&AnnotNotifyRemovedFromSelection);
	gStamperAH.GetLayer = ASCallbackCreateProto(AVAnnotHandlerGetLayerProc, &AnnotGetLayer);

	 gStamperAH.DoKeyDownEx = ASCallbackCreateProto(AVAnnotHandlerDoKeyDownExProc, &AnnotDoKeyDownEx);

	//need to define these to get the PDAnnotHandler to work
	//gStamperAH.GetInfo  = ASCallbackCreateProto(AVAnnotHandlerGetInfoProc, &AnnotGetInfo);
	//gStamperAH.DeleteInfo  = ASCallbackCreateProto(AVAnnotHandlerDeleteInfoProc, &AnnotDeleteInfo);
	
	//if you were implemented a non-square shaped annot, you could use this to calculate hit testing
	//AVAnnotHandlerPtInAnnotViewBBoxProc PtInAnnotViewBBox;

	//for adding additional attributes. Since stamper annots are simple, we do all at creation time
	//AVAnnotHandlerNewProc New;
	
	//used by AVDocCopyAnnot to copy annot directly, not by cut, copy and paste
	//AVAnnotHandlerCopyProc Copy;

	//older form of DoKeyDownEx
	//AVAnnotHandlerDoKeyDownProc DoKeyDown;

	//unused by Acrobat
	//AVAnnotHandlerNotifyDestroyProc NotifyDestroy;

	//display UI for changing properties
	//AVAnnotHandlerDoPropertiesProc DoProperties;

	//don't need specific cursor handling
	//AVAnnotHandlerCursorEnterProc CursorEnter;
	//AVAnnotHandlerCursorExitProc CursorExit;

	//AVAnnotHandlerDoClickProc DoRightClick;

	//AVAnnotHandlerCanPerformOpProc CanPerformOp;
	//AVAnnotHandlerPerformOpProc PerformOp;
	
	
	// Use if you want an update rect for specific parts of the annot
	//AVAnnotHandlerDrawExProc DrawEx;
	
	// Register the annotation handler.
	
	AVAppRegisterAnnotHandler(&gStamperAH);
}

/* CreateStampAnnotationAt
** ------------------------------------------------------
** */ 
/** Clients may call this to create a stamp annotation centered on the given coordinates.
 **
 ** @see AVPageViewGetPage
 ** @see AVPageViewGetAVDoc
 ** @see AVPageViewDeviceRectToPage
 ** @see PDPageAddnewAnnot
 ** @see PDAnnotGetCosObj
 ** @see PDDocGetCosDoc
 ** @see CosNewInteger
 ** @see CosDictPutKeyString
 ** @see CosNewDict
 ** @see CosNewNull
 ** @see ASMemStmRdOpen
 ** @see AVAlertNote
 ** @see ASGetErrorString
*/
PDAnnot CreateStampAnnotationAt(AVPageView pageView, AVDevRect *rect)
{
	ASFixedRect pdLocation;
	PDAnnot annot = {0,0};
	CosObj cAnnotObj, cIntObj, cStmObj, attributesDict, LengthEntry, cNullObj;
	CosObj cAPDictObj, cStmDictObj, cBBoxObj, cProcSetObj, cResObj;
	CosObj fontResObj, fontResObj2, cObj;
	CosDoc cdoc;
	ASStm stm;
	char buf[512];
	PDPage pdPage;
	ASUns32 s;

	DURING
	
		//handle clicks (with no drag, or only in one direction), by creating a default size annot
		if((rect->top == rect->bottom) || (rect->right == rect->left))
		{
			rect->top +=75;
			rect->right += 150;
		}
		
		pdPage = AVPageViewGetPage(pageView);
		AVDoc avDoc = AVPageViewGetAVDoc(pageView);

		AVPageViewDeviceRectToPage ( pageView, rect, &pdLocation);

		// Add the new annotation to the page.
		annot = PDPageAddNewAnnot(pdPage, -1, Stamper_K, &pdLocation);

		cAnnotObj = PDAnnotGetCosObj(annot);
		cdoc = PDDocGetCosDoc(PDPageGetDoc(pdPage));

		// See PDF Specification for more information on the Flags key.
		cIntObj = CosNewInteger (cdoc, false, 4L);
		CosDictPutKeyString(cAnnotObj, Flags_KStr, cIntObj);

		cAPDictObj = CosNewDict(cdoc, false, 1L);
		cNullObj = CosNewNull();

		// Read in the stream for the appearance key.
		memset(&buf, 0, sizeof(buf));
		strcat(buf, "BT /STAMPR 24 Tf 10 40 TD ( Stamp)Tj ET q 10 w 0 1 0 RG 5 5 90 90 re S Q");
		s = (ASUns32)strlen(buf);
		stm = ASMemStmRdOpen(buf, s);

		attributesDict	= CosNewDict(cdoc, false, 5);
		LengthEntry = CosNewInteger(cdoc, false, s);
		CosDictPutKeyString(attributesDict, Length_KStr, LengthEntry);

		cStmObj = CosNewStream(cdoc, true, stm, 0, true, attributesDict, cNullObj, s);
		cStmDictObj = CosStreamDict(cStmObj);

		CosDictPutKeyString(cStmDictObj, "Type", CosNewNameFromString(cdoc, false, "XObject"));
		CosDictPutKeyString(cStmDictObj, "Subtype", CosNewNameFromString(cdoc, false, "Form"));

		CosDictPutKeyString(cStmDictObj, "FormType", CosNewInteger(cdoc, false, 1L));

		cBBoxObj = CosNewArray(cdoc, false, 4L);
		CosArrayInsert(cBBoxObj, 0L, CosNewInteger(cdoc, false, 0L));
		CosArrayInsert(cBBoxObj, 1L, CosNewInteger(cdoc, false, 0L));
		CosArrayInsert(cBBoxObj, 2L, CosNewInteger(cdoc, false, 100L));
		CosArrayInsert(cBBoxObj, 3L, CosNewInteger(cdoc, false, 100L));
		CosDictPutKeyString(cStmDictObj, "BBox", cBBoxObj);

		// Create Resource dictionary for XObject.
		cResObj = CosNewDict(cdoc, false, 5L);

		// Add Font resource to Resources dictionary.
		fontResObj = CosNewDict(cdoc, true, 1L);
		fontResObj2 = CosNewDict(cdoc, true, 5L);
		cObj = CosNewNameFromString(cdoc, false, "Font");
		CosDictPutKeyString(fontResObj2, "Type", cObj);
		cObj = CosNewNameFromString(cdoc, false, "Type1");
		CosDictPutKeyString(fontResObj2, "Subtype", cObj);
		cObj = CosNewNameFromString(cdoc, false, "STAMPR");
		CosDictPutKeyString(fontResObj2, "Name", cObj);
		cObj = CosNewNameFromString(cdoc, false, "Helvetica");
		CosDictPutKeyString(fontResObj2, "BaseFont", cObj);
		CosDictPutKeyString(fontResObj, "STAMPR", fontResObj2);
		CosDictPutKeyString(cResObj, "Font", fontResObj);

		// Set up ProcSet Resources.
		cProcSetObj = CosNewArray(cdoc, false, 2L);
		CosArrayPut(cProcSetObj, 0L, CosNewNameFromString(cdoc, false, "PDF"));
		CosArrayPut(cProcSetObj, 1L, CosNewNameFromString(cdoc, false, "Text"));
		CosDictPutKeyString(cResObj, "ProcSet", cProcSetObj);

		// Put Resources dictionary in XObject's stream dictionary.
		CosDictPutKeyString(cStmDictObj, "Resources", cResObj);

		// Set the stream object (now a Forms XObject) as the normal (N) key
		// and add to the appearance (AP) key.
		CosDictPut(cAPDictObj, FaceNormal_K, cStmObj);
		CosDictPutKeyString(cAnnotObj, Appearance_KStr, cAPDictObj);
	HANDLER
		AVAlertNote(ASGetErrorString(ERRORCODE, buf, sizeof(buf)));
	END_HANDLER

	return annot;
}

/* UninitializeAnnotHandler
** ------------------------------------------------------
*/
void UninitializeAnnotHandler()
{
	AVAppUnregisterAnnotHandler(&gStamperAH); 
}