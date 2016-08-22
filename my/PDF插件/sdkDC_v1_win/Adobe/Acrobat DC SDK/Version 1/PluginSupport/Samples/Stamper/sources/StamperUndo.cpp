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

 StamperUndo.cpp

*********************************************************************/

#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif
#include "StamperAnnotHandler.h"

ACCB1 ASBool ACCB2 StamperUndoVerifyProc(AVUndo undo);
ACCB1 ASBool ACCB2 StamperUndoExecuteProc(AVUndo undo);
ACCB1 ASBool ACCB2 StamperRedoVerifyProc(AVUndo undo);
ACCB1 ASBool ACCB2 StamperRedoExecuteProc(AVUndo undo);
ACCB1 void ACCB2 StamperGetUndoTitleProc(AVUndo undo, ASText title);
ACCB1 void ACCB2 StamperGetRedoTitleProc(AVUndo undo, ASText title);
ACCB1 void ACCB2 StamperReleaseProc(AVUndo undo);
ACCB1 void ACCB2 StamperBeginUndoProc(AVDoc doc, ASBool bUndo);
ACCB1 void ACCB2 StamperEndUndoProc(AVDoc doc, ASBool bUndo);

#define STAMPER_UNDO_TYPE "Stamper Undo Handler"

AVUndoHandlerRec gUndoHandler = { 
	sizeof(AVUndoHandlerRec),
	STAMPER_UNDO_TYPE,
	StamperUndoVerifyProc,
	StamperUndoExecuteProc,
	StamperRedoVerifyProc,
	StamperRedoExecuteProc,
	StamperGetUndoTitleProc,
	StamperGetRedoTitleProc,
	StamperReleaseProc,
	StamperBeginUndoProc,
	StamperEndUndoProc };
	

//A problem arises if I create a annot, delete, and undo the delete (which creates a new annot).
//At this point the create undo record has lost track of the annot, so it can't undo the create.
//so I keep track of the new annot in this cab.
ASCab undoCab = NULL;

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/** Helper routine to add annot 
 **
 ** @see ASCabGetPointer
 ** @see ASFree
 ** @see ASCabPutPointer
 */
void AddStamperAnnot(StamperUndoDataPtr data)
{
	//need to update annot ptr
	PDAnnot* newAnnotPtr = (PDAnnot*)ASmalloc(sizeof(PDAnnot));
	*newAnnotPtr = CreateStampAnnotationAt(data->pageView, &data->rect);
	
	//and store updated info
	char buf[18];
	sprintf(buf, "%x%x", data->annot.a, data->annot.b);
	PDAnnot* oldAnnotPtr;
	//clean up old one
	if(undoCab == NULL)
		undoCab = ASCabNew();
	else if((oldAnnotPtr  = (PDAnnot*)ASCabGetPointer(undoCab, buf, PDAnnot*)) != NULL)
		ASfree(oldAnnotPtr);
	ASCabPutPointer(undoCab, buf, PDAnnot*, newAnnotPtr, NULL);

}

/** Helper routine to delete annot
 **
 ** @see PDPageGetAnnotIndex
 ** @see ASCabGetPointer
 ** @see PDPageRemoveAnnot
 */
void RemoveStamperAnnot(StamperUndoDataPtr data)
{
	//store rect so we can undo in the right place
	gStamperAH.GetAnnotViewBBox(&gStamperAH, data->pageView, data->annot, &data->rect);

	//get the index, remove the annot
	PDPage pdPage = AVPageViewGetPage(data->pageView);
	
	//check to figure out where this annot is located
	ASInt32 index;
	if((index = PDPageGetAnnotIndex (pdPage, data->annot)) == -1)//undoCab != NULL)
	{
		char buf[18];
		if (undoCab == NULL)
			return;
		sprintf(buf, "%x%x", data->annot.a, data->annot.b);
		PDAnnot* annotPtr  = ASCabGetPointer(undoCab, buf, PDAnnot*);
		index = PDPageGetAnnotIndex (pdPage, *annotPtr);
		if(index!=-1)
			PDPageRemoveAnnot (pdPage, index);
	}
	else
		PDPageRemoveAnnot (pdPage, index);
}

/** This returns true always, but if your modification could be 
	modified further or removed by other actions, you might occasionally need
	to return false */
ACCB1 ASBool ACCB2 StamperUndoVerifyProc(AVUndo undo)
{
	StamperUndoDataPtr data = (StamperUndoDataPtr)AVUndoGetData(undo);
	return PDAnnotIsValid(data->annot);
}

/** This returns true always, but if this fails, you should return false */
ACCB1 ASBool ACCB2 StamperUndoExecuteProc(AVUndo undo)
{
	StamperUndoDataPtr data = (StamperUndoDataPtr)AVUndoGetData(undo);
	
	if(data->undoType == kCreateStamperAnnot)
		RemoveStamperAnnot(data);
	else if(data->undoType == kDeleteStamperAnnot)
		AddStamperAnnot(data);
	
	return true;
}

/** This implementation returns true always, but if  for some reason it could fail, you should return false */
ACCB1 ASBool ACCB2 StamperRedoVerifyProc(AVUndo undo)
{
	StamperUndoDataPtr data = (StamperUndoDataPtr)AVUndoGetData(undo);
	return  PDAnnotIsValid(data->annot);
}

/** This implementation returns true always, but if for some reason it could fails you should return false */
ACCB1 ASBool ACCB2 StamperRedoExecuteProc(AVUndo undo)
{
	StamperUndoDataPtr data = (StamperUndoDataPtr)AVUndoGetData(undo);
		
	if(data->undoType == kDeleteStamperAnnot)
		RemoveStamperAnnot(data);
	else if(data->undoType == kCreateStamperAnnot)
		AddStamperAnnot(data);
	
	return true;
}

ACCB1 void ACCB2 StamperGetUndoTitleProc(AVUndo undo, ASText title)
{
	StamperUndoDataPtr data = (StamperUndoDataPtr)AVUndoGetData(undo);
	if(data->undoType == kCreateStamperAnnot)
		ASTextSetScriptText(title, "Undo Stamper annotation creation", kASRomanScript);
	else if(data->undoType == kDeleteStamperAnnot)
		ASTextSetScriptText(title, "Undo Stamper annotation deletion", kASRomanScript);

}

ACCB1 void ACCB2 StamperGetRedoTitleProc(AVUndo undo, ASText title)
{
	StamperUndoDataPtr data = (StamperUndoDataPtr)AVUndoGetData(undo);
	if(data->undoType == kCreateStamperAnnot)
		ASTextSetScriptText(title, "Redo Stamper annotation creation", kASRomanScript);
	else if(data->undoType == kDeleteStamperAnnot)
		ASTextSetScriptText(title, "Redo Stamper annotation deletion", kASRomanScript);

}

/**
 **	This would be called when undos are cleared, either by someone directly 
 **	calling ACDocClearUndos or when the document is closed
 **	Also, if you have done one or more undos, and then a new undo record is created 
 **	those undo records on the redo stack will be released.
 */
ACCB1 void ACCB2 StamperReleaseProc(AVUndo undo)
{
	//clean up dataptr
	StamperUndoDataPtr data = (StamperUndoDataPtr)AVUndoGetData(undo);
	ASfree(data);

}

ACCB1 void ACCB2 StamperBeginUndoProc(AVDoc doc, ASBool bUndo)
{
	// If you needed to allocate memory, or start 
	// a progress bar this would be a good place
}

ACCB1 void ACCB2 StamperEndUndoProc(AVDoc doc, ASBool bUndo)
{
	// If you needed to deallocate memory, or stop 
	// a progress bar this would be a good place
}


