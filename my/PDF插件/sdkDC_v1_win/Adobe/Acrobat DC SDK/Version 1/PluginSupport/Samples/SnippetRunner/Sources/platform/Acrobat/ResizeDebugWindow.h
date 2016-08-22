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

 DebugWindow.h

 - Simple logging mechanism

*********************************************************************/
#ifndef _DebugWindow_h_
#define _DebugWindow_h_

#include <iostream>
#include "DebugWindowOStream.h"

/** this class maintains a simple debugging window class. It is a singleton.
*/
class DebugWindow {
protected:
	/**
		Constructor is protected because this is a singleton.
	*/
	DebugWindow();
	
private:
	// single instance definition
	static DebugWindow* gDebugWindow;
	/** Create the debug window class */
	void CreateDebugWindow(void);
	/** receive notification */
	static void ASAPI DebugWindowNotifyProc(ADMDialogRef dialogRef, ADMNotifierRef notifierRef);
	/** destroy the window */
	static void ASAPI DebugWindowDestroyProc(ADMDialogRef dialogRef);
	/** the user hit the clear button */
	static void ASAPI PromptOnClear(ADMItemRef item, ADMNotifierRef inNotifier);
	/** calculate to resize window */
	static ADMBoolean ASAPI CalculateResizeDialog(ADMDialogRef dialogRef, ASRect& pNewRect );
	
	/** the window (as an ADMDialog) */
	static ADMDialogRef gADMDebugWindow;
	/** the window as an AVWindow */
	AVWindow gAVWindow;
	/** the window handler record structure */
	AVWindowHandlerRec gDebugWindowHandler;

	/** ptr to the window buffer to delete it later */
	static DebugWindowBuf * newClogWB;

	/** ptr to the original buffer, we need to restore it. */
	static streambuf * oldClogWB;
	static streambuf* oldCoutWB;
	/** holds the text being displayed */
	static ASText textToDisplay;
public:
	/**
		Returns the global instance of this singleton, call like this SnipRunManager::Instance().
		@return the global instance of this singleton class.
	*/
	static DebugWindow* Instance();

	/**
		Destroys the single global instance of this singleton.
	*/
	static void DeleteInstance();
	/** dtor */
	virtual ~DebugWindow();
	/**  Displays an ASText string on DebugWindow */
	void DebugEmitString(ASText str);
	/**  Displays an ASCII string on DebugWindow */
	void DebugEmitString(const char * str);
	/** show the debug window */
	void ShowDebugWindow();
	/** hide the debug window */
	void HideDebugWindow();
	/** Flushes the debug window */
	void DebugWindowFlush(void);

	// friend to the ostream that takes ASText
	friend std::ostream& operator << (std::ostream& output,ASText msg);
};

#endif