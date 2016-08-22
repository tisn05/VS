/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2008 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/
/** 
\file wxPlugin.cpp

  - This file implements the functionality of the wxPlugin.
*********************************************************************/


// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

#include "wx/app.h"
#include "wx/msgdlg.h"
#include "wx/artprov.h"
#include "wx/imaglist.h"
#include "wx/image.h"
#include "wxInit.h"
#include "dialogs.h"

#include "PIMain.h"
#include "AVCalls.h"


/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/
 bool show = true;
 MyModelessDialog* m_dialog;
 
// this plug-in's name, you should specify your own unique name here.
const char* MyPluginExtensionName = "ADBE:wxPlugin";
typedef ACCB1 void ACCB2 (*menuCommand)(void *clientData);

/* A convenient function to add a menu item for your plugin.
** A policy in Acrobat 6 is to add menu items for developers' plugins under Advanced menu.
** If specify bUnderAcrobatSDKSubMenu as false, you directly add your menu item under Advanced menu.
** If specify bUnderAcrobatSDKSubMenu as true, you add the menu item under Acrobat SDK sub menu.
*/
ACCB1 ASBool ACCB2 PluginMenuItem(char* MyMenuItemTitle, char* MyMenuItemName, 
								  menuCommand menuFunction, AVComputeMarkedProc isMarkedProc);

/*-------------------------------------------------------
	Functions
-------------------------------------------------------*/
/**	
	Open a standard frame
*/ 
ACCB1 void ACCB2 OpenFrame(void *clientData)
{
	MyFrame *frame = new MyFrame((wxFrame *)mainFrame, _T("wxWidgets Frame example"));
	
	// Show the frame
	frame->Centre(wxBOTH);
    frame->Show(true);
}

/**	
	Open a modal dialog
*/ 
ACCB1 void ACCB2 OpenModal(void *clientData)
{
#ifdef WIN_PLATFORM
	HWND CapturehWnd=NULL, hParent=NULL;

	CapturehWnd = GetCapture();
    if ( CapturehWnd != NULL )
    {
        ReleaseCapture();
    }

	hParent = WinAppGetModalParent(AVAppGetActiveDoc());

	wxDialog *win = new wxDialog();
    win->SetHWND(hParent);
    
	MyModalDialog* dlg = new MyModalDialog(win);
	AVWindow dlgWindow = AVWindowNewFromPlatformThing(
		AVWLmodal, NULL, NULL, gExtensionID, reinterpret_cast<AVPlatformWindowRef>(dlg->GetHWND()));
#endif
#ifdef MAC_PLATFORM
	MyModalDialog* dlg = new MyModalDialog(mainFrame);
	WindowRef windowRef = (WindowRef) dlg->MacGetTopLevelWindowRef();
	AVWindow dlgWindow = AVWindowNewFromPlatformThing(
		AVWLmodal, NULL, NULL, gExtensionID,reinterpret_cast<AVPlatformWindowRef>(windowRef));
#endif
	
	AVAppBeginModal(dlgWindow);
	int returnCode = dlg->ShowModal();
    AVAppEndModal();

#ifdef WIN_PLATFORM
	BringWindowToTop(hParent);
	AVWindowDestroy(dlgWindow);
#endif
#ifdef MAC_PLATFORM //do this to remove all hints of window, so menus reenable
	DisposeWindow(windowRef);
#endif
	dlg->Destroy();
	delete dlg;
}


 /**	
	Open or close a modeless dialog, depending on current state
*/ 
ACCB1 void ACCB2 OpenModeless(void *clientData)
{
	AVWindow dlgWindow = NULL;
    if ( show )
	{
#ifdef WIN_PLATFORM
		HWND CapturehWnd = NULL, hParent = NULL;

		CapturehWnd = GetCapture();
		if ( CapturehWnd != NULL )
		{
			ReleaseCapture();
		}

		hParent = WinAppGetModalParent(AVAppGetActiveDoc());
		wxDialog *win = new wxDialog();
		win->SetHWND(hParent);

		m_dialog = new MyModelessDialog(win);
         
		dlgWindow = AVWindowNewFromPlatformThing(AVWLfloating, NULL, NULL, gExtensionID, reinterpret_cast<AVPlatformWindowRef>(m_dialog->GetHWND()));
		WinAppRegisterModelessDialog((HWND)m_dialog->GetHWND());  
#else
		if ( !m_dialog )
        {
            m_dialog = new MyModelessDialog(mainFrame);
        }
#endif
		m_dialog->Show(true);
		show = false;
    }
    else // hide
    {
        if (m_dialog)
            m_dialog->Hide();
#ifdef WIN_PLATFORM
		WinAppUnRegisterModelessDialog((HWND)m_dialog->GetHandle());
		AVWindowDestroy(dlgWindow);
#endif
		show = true;
    }
}

ASBool ModelessDialogMenuItemComputeMarkedProc ( void* data)
{
	return !show;
}

/* MyPluginSetmenu
** ------------------------------------------------------
**
** Function to set up menu for the plugin.
** It calls a convenient function PluginMenuItem.
** Return true if successful, false if failed.
*/
ACCB1 ASBool ACCB2 MyPluginSetmenu()
{
	// Add new menu items under Acrobat SDK submenu.
	PluginMenuItem("WX Frame", "ADBE:WXFrame", OpenFrame, NULL); 
	PluginMenuItem("WX Modal", "ADBE:WXModal", OpenModal, NULL); 
	return PluginMenuItem("WX Modeless", "ADBE:WXModeless", OpenModeless, ModelessDialogMenuItemComputeMarkedProc); 
}
	



/* MyPluginIsEnabled
** ------------------------------------------------------
** Function to control if a menu item should be enabled.
** Return true to enable it, false not to enable it.
*/
ACCB1 ASBool ACCB2 MyPluginIsEnabled(void *clientData)
{
	return true;
}

