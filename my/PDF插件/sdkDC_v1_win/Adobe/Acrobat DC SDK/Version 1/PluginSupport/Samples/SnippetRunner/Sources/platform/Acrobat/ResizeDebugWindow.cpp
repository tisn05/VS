/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

Implements the dialog used to get parameters passed to the snippet
from the user

  ResizeDebugWindow.cpp

 - Simple logging mechanism
*********************************************************************/


#include <string>
#ifdef WIN_PLATFORM
#include "PIHeaders.h"
#include "resource.h"
#else
#include "MacResource.h"
#endif

#include "ResizeDebugWindow.h"

/* allocate the storage for the static */
ADMDialogRef DebugWindow::gADMDebugWindow = NULL;
ASText DebugWindow::textToDisplay = NULL;
ASRect LastDialogRect;


/*
	Factory methods. Single instance used.
*/
DebugWindow* DebugWindow::gDebugWindow  = NULL;

/*
	maintain this class as a singleton
*/
DebugWindow* 
DebugWindow::Instance()
{
	if (gDebugWindow == NULL)
	{
		gDebugWindow = new DebugWindow();
	}
	return gDebugWindow;
}


/*
*/
void 
DebugWindow::DeleteInstance()
{
//	if (gDebugWindow != NULL)
//		delete gDebugWindow;
//	gDebugWindow = NULL;
}


DebugWindow::DebugWindow(){
	if (DebugWindow::gADMDebugWindow==NULL){
		textToDisplay = ASTextNew();
		CreateDebugWindow();
	}
}

DebugWindow::~DebugWindow(){
}



ADMBoolean ASAPI 
DebugWindow::CalculateResizeDialog(ADMDialogRef dialogRef, ASRect& pNewRect )
{
	// get new dialog bounds
	ASRect dialogRect;		
	sADMDialog->GetBoundsRect(dialogRef, &dialogRect);

	// check if size changed
	int DeltaH = (dialogRect.bottom - dialogRect.top) - (LastDialogRect.bottom - LastDialogRect.top);
	int DeltaW = (dialogRect.right - dialogRect.left) - (LastDialogRect.right - LastDialogRect.left);

	// no more size change, return false
	if(DeltaH == 0 && DeltaW == 0) 
		return false;

	// remember last bounds
	LastDialogRect.left = dialogRect.left;
	LastDialogRect.right = dialogRect.right;
	LastDialogRect.top = dialogRect.top;
	LastDialogRect.bottom = dialogRect.bottom;

	// adjust text filed item bounds
	pNewRect.left = 20;
	pNewRect.right = dialogRect.right - dialogRect.left - 20;
	pNewRect.top = 34;
	pNewRect.bottom = dialogRect.bottom - dialogRect.top - 20*2;

	return true;
}

void ASAPI 
DebugWindow::DebugWindowNotifyProc (ADMDialogRef dialogRef, ADMNotifierRef notifierRef)
{
	char notifierType[256];
	sADMNotifier->GetNotifierType (notifierRef, notifierType, 256);

	sADMDialog->DefaultNotify (dialogRef, notifierRef);

	if (sADMNotifier->IsNotifierType(notifierRef, kADMBoundsChangedNotifier)) {

		ASRect NewTextRect;
		ASBool rc = CalculateResizeDialog(dialogRef, NewTextRect);
		
		if(rc==true)  {
			ADMItemRef TextFieldItem = sADMDialog->GetItem (dialogRef, IDC_EDITWIN);
			sADMItem->SetBoundsRect(TextFieldItem, &NewTextRect);
		}
	}

}


void ASAPI 
DebugWindow::DebugWindowDestroyProc(ADMDialogRef dialogRef)
{
	DebugWindow::gADMDebugWindow = NULL;
	if (textToDisplay!=NULL){
		ASTextDestroy(textToDisplay);
	}
	ADMUtils::ReleaseADM();
}





void 
DebugWindow::CreateDebugWindow (void)
{
	ADMUtils::InitializeADM();

	// Display floating dialog.
	DebugWindow::gADMDebugWindow = sADMDialog->Create (sADMPluginRef, "ADBE:ShowDebugWin",
							IDD_DEBUGWINDOW_RESIZE, kADMResizingFloatingDialogStyle, NULL, NULL, NULL);

	sADMDialog->SetText (DebugWindow::gADMDebugWindow, "Snippet Debug Window");
	sADMDialog->SetDestroyProc (DebugWindow::gADMDebugWindow, DebugWindowDestroyProc);
	sADMDialog->SetNotifyProc (DebugWindow::gADMDebugWindow, DebugWindowNotifyProc);
	sADMDialog->Show (DebugWindow::gADMDebugWindow, false);

	if (DebugWindow::gADMDebugWindow) {

		ADMItemRef itemRef = sADMDialog->GetItem (DebugWindow::gADMDebugWindow, IDC_EDITWIN);

#ifdef MAC_PLATFORM
		// init last dialog as something not equal to actuall dialog size. 
		LastDialogRect.left = 0;
		LastDialogRect.right = 0;
		LastDialogRect.top = 100;
		LastDialogRect.bottom = 100;

		// adjust the text control size
		ASRect NewTextRect;
		ASBool rc = CalculateResizeDialog(DebugWindow::gADMDebugWindow, NewTextRect);
		
		if(rc==true)  {
			ADMItemRef TextFieldItem = sADMDialog->GetItem (DebugWindow::gADMDebugWindow, IDC_EDITWIN);
			sADMItem->SetBoundsRect(TextFieldItem, &NewTextRect);
		}
#endif	

		

		// Set the maximum text length.
		this->DebugEmitString("Welcome to DebugWindow...\n");
		
		itemRef = sADMDialog->GetItem(DebugWindow::gADMDebugWindow, IDC_CLEAR_RESIZE);
		sADMItem->SetNotifyProc(itemRef, PromptOnClear);

		// initialize a rectangle for resizing dialog
		ASRect dialogRect;		
		sADMDialog->GetBoundsRect(DebugWindow::gADMDebugWindow, &dialogRect);
		LastDialogRect.left = dialogRect.left;
		LastDialogRect.right = dialogRect.right;
		LastDialogRect.top = dialogRect.top;
		LastDialogRect.bottom = dialogRect.bottom;


		//we don't do anything with this, so not sure why we allocate it.
//		gAVWindow = AVWindowNewFromPlatformThing (AVWLfloating, 0, NULL, gExtensionID,
//													sADMDialog->GetWindowRef(DebugWindow::gADMDebugWindow));
	}
}

void ASAPI 
DebugWindow::PromptOnClear(ADMItemRef item, ADMNotifierRef inNotifier )
{
	sADMItem->DefaultNotify(item, inNotifier);
	ADMItemRef itemRef = sADMDialog->GetItem (DebugWindow::gADMDebugWindow, IDC_EDITWIN);
	ASTextMakeEmpty(textToDisplay);		
	sADMItem->SetTextW(itemRef, ASTextGetUnicode(textToDisplay));
}

void  
DebugWindow::ShowDebugWindow(void)
{
	if (DebugWindow::gADMDebugWindow==NULL){
		CreateDebugWindow();
	}
	sADMDialog->Show(DebugWindow::gADMDebugWindow, true);
}

void
DebugWindow::HideDebugWindow(void)
{
	if (DebugWindow::gADMDebugWindow!=NULL){
		sADMDialog->Show(DebugWindow::gADMDebugWindow, false);
	}
}

void  
DebugWindow::DebugEmitString(const char * str){
	ASText tmpText = ASTextFromScriptText(str, kASRomanScript);
	this->DebugEmitString(tmpText);
	ASTextDestroy(tmpText);
}

void  
DebugWindow::DebugEmitString(ASText str)
{
	char * volatile outputString = NULL;
	ASInt32 windowTextLen;
	ADMItemRef itemRef;

	if (!DebugWindow::gADMDebugWindow)
		CreateDebugWindow();

	itemRef = sADMDialog->GetItem(DebugWindow::gADMDebugWindow, IDC_EDITWIN);
	windowTextLen = sADMItem->GetTextLengthW(itemRef);
	// if current length > 16000, just reset the widget
	if (windowTextLen > 16000){
		ASTextMakeEmpty(textToDisplay);		
	}
	ASTextCat(textToDisplay, str);
	ASTextNormalizeEndOfLine(textToDisplay);
	// Finally, we can apply this new text to the control.
	const ASUns16 * text = ASTextGetUnicode(textToDisplay);
	sADMItem->SetTextW(itemRef, text);
}

void  
DebugWindow::DebugWindowFlush(void)
{
	if (DebugWindow::gADMDebugWindow!=NULL){
		ADMItemRef item = sADMDialog->GetItem (DebugWindow::gADMDebugWindow, IDC_EDITWIN);
		ASTextMakeEmpty(textToDisplay);		
		sADMItem->SetTextW(item, ASTextGetUnicode(textToDisplay));
	}
}
