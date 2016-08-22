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
\file StampAH.h  Interface to Stamper annotation-related routines.

*********************************************************************/

#ifndef _H_StampAH
#define _H_StampAH

extern ASBool gStamperToolSelected;
extern AVUndoHandlerRec gUndoHandler;
extern AVAnnotHandlerRec gStamperAH;
extern ASAtom Stamper_K;

typedef enum{
	kCreateStamperAnnot,
	kMoveStamperAnnot,
	kDeleteStamperAnnot
}StamperAnnotUndoType;

typedef struct StamperUndoDataRec
{
	//while this starts as a real PDAnnot, it basically is used as a key into
	//an ASCab to figure out where the real annot is(which might have been deleted
	//and recreated.
	PDAnnot annot;
	
	StamperAnnotUndoType undoType;
	AVPageView pageView;
	AVDevRect rect;
}StamperUndoDataRec, *StamperUndoDataPtr;

/* CreateStampAnnotationAt
** Creates a Stamper annotation in the given page view at the given
** rectangle.
*/
PDAnnot CreateStampAnnotationAt(AVPageView pageView, AVDevRect *rect);

/* SetUpStamperAnnotHandler
** ------------------------------------------------------
**
** Initialize atoms and set up annotation callbacks.
*/
void SetUpStamperAnnotHandler(void);

/* UninitializeAnnotHandler
** ------------------------------------------------------
*/
void UninitializeAnnotHandler();

#endif /* !_H_StampAH */
