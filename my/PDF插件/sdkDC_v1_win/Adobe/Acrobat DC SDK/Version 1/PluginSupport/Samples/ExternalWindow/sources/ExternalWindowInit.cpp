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

/** \file ExternalWindowInit.cpp

 - Implements the core handshake callbacks.

 - Installs the Advanced > Acrobat SDK > Open PDF In External Window menuitem.

*********************************************************************/

// Acrobat Headers.
#ifdef WIN_PLATFORM
#include "PIHeaders.h"
#endif


/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

static AVMenuItem menuItem = NULL;

extern ASBool InitializeWindowHandler (void);
extern ACCB1 void ACCB2 OpenExternalWindow (void* clientData);
extern ACCB1 ASBool ACCB2 ExternalWindowIsEnabled (void *clientData);

/*-------------------------------------------------------
	Core Handshake Callbacks
-------------------------------------------------------*/

/**
	Callback invoked by the application to give the plug-in an opportunity to register an HFTServer with the application.
	We do not provide any external functionality to the application core, we return true to indicate that the plug-in should 
	continue loading.
	@return true to indicate the plug-in should continue loading.
*/
ACCB1 ASBool ACCB2 PluginExportHFTs(void)
{
	return true;
}

/** 
	The application calls this function to allow it to
	<ul>
	<li> Import plug-in supplied HFTs.
	<li> Replace functions in the HFTs you're using (where allowed).
	<li> Register to receive notification events.
	</ul>
	@return true to continue loading the plug-in.
*/
ACCB1 ASBool ACCB2 PluginImportReplaceAndRegister(void)
{
	return true;
}

/** 
	The main initialization routine.
	Add our menu item to the nenubar
	@see AVMenubarAcquireMenuByName
	@see AVMenuNew
	@see AVMenuItemNew
	@see AVMenuItemSetExecuteProc
	@see AVMenuItemSetExecuteProc
	@see AVMenuAddMenuItem
	@see AVMenuItemRelease
	@see AVMenubarAcquireMenuItemByName
	@see AVMenuGetMenuItemIndex
	@see AVMenuAddMenuItem
	@see AVMenuRelease
	@return true to continue loading the plug-in
*/
ACCB1 ASBool ACCB2 PluginInit(void)
{
	AVMenubar menubar = AVAppGetMenubar();
	AVMenu volatile commonMenu = NULL;

	if (!menubar)
		return false;

	DURING

		// Acquire the common "Acrobat SDK" menu, creating it if necessary.
		commonMenu = AVMenubarAcquireMenuByName (menubar, "ADBE:Acrobat_SDK");
		// if "Acrobat SDK" menu is not existing, create one.
		if (!commonMenu) {
			commonMenu = AVMenuNew ("Acrobat SDK", "ADBE:Acrobat_SDK", gExtensionID);
			AVMenubarAddMenu(menubar, commonMenu, APPEND_MENU);		
		}

    	// Create our main menuitem and add to the common menu
		menuItem = AVMenuItemNew ("Open PDF In External Window", "ADBE:ExternWin", NULL, true, NO_SHORTCUT, 0, NULL, gExtensionID);
		AVMenuItemSetExecuteProc (menuItem, ASCallbackCreateProto(AVExecuteProc, OpenExternalWindow), NULL);
		AVMenuItemSetComputeEnabledProc (menuItem,
							ASCallbackCreateProto(AVComputeEnabledProc, ExternalWindowIsEnabled), NULL);
		AVMenuAddMenuItem (commonMenu, menuItem, APPEND_MENUITEM);
		AVMenuItemRelease (menuItem);

		AVMenuRelease (commonMenu);
	HANDLER
		if (commonMenu)
			AVMenuRelease (commonMenu);
		return false;
	END_HANDLER

	return InitializeWindowHandler();
}

/** 
	The unload routine.
	Called when your plug-in is being unloaded when the application quits.
	Use this routine to release any system resources you may have
	allocated. We remove our menu item.

	Returning false will cause an alert to display that unloading failed.
	@return true to indicate the plug-in unloaded.
*/
ACCB1 ASBool ACCB2 PluginUnload(void)
{
	// Remove any menu items created.
	if (menuItem)
		AVMenuItemRemove (menuItem);

	return true;
}

/**
	Return the unique ASAtom associated with your plug-in.
	@return the plug-ins name as an ASAtom.
*/
ASAtom GetExtensionName()
{
	return ASAtomFromString("ADBE:ExternWin");	/* Change to your extension's name */
}

/**
	Function that provides the initial interface between your plug-in and the application.
	This function provides the callback functions to the application that allow it to 
	register the plug-in with the application environment.

	Required Plug-in handshaking routine: <b>Do not change it's name!</b>
	
	@param handshakeVersion the version this plug-in works with. There are two versions possible, the plug-in version 
	and the application version. The application calls the main entry point for this plug-in with its version.
	The main entry point will call this function with the version that is earliest. 
	@param handshakeData OUT the data structure used to provide the primary entry points for the plug-in. These
	entry points are used in registering the plug-in with the application and allowing the plug-in to register for 
	other plug-in services and offer its own.
	@return true to indicate success, false otherwise (the plug-in will not load).
*/
ACCB1 ASBool ACCB2 PIHandshake(Uns32 handshakeVersion, void *handshakeData)
{
	if (handshakeVersion == HANDSHAKE_V0200) {
		/* Cast handshakeData to the appropriate type */
		PIHandshakeData_V0200 *hsData = (PIHandshakeData_V0200 *)handshakeData;

		/* Set the name we want to go by */
		hsData->extensionName = GetExtensionName();

		/* If you export your own HFT, do so in here */
		hsData->exportHFTsCallback = ASCallbackCreateProto(PIExportHFTsProcType, &PluginExportHFTs);

		/*
		** If you import plug-in HFTs, replace functionality, and/or want to register for notifications before
		** the user has a chance to do anything, do so in here.
		*/
		hsData->importReplaceAndRegisterCallback = ASCallbackCreateProto(PIImportReplaceAndRegisterProcType,
																		 &PluginImportReplaceAndRegister);

		/* Perform your plug-in's initialization in here */
		hsData->initCallback = ASCallbackCreateProto(PIInitProcType, &PluginInit);

		/* Perform any memory freeing or state saving on "quit" in here */
		hsData->unloadCallback = ASCallbackCreateProto(PIUnloadProcType, &PluginUnload);

		/* All done */
		return true;

	} /* Each time the handshake version changes, add a new "else if" branch */

	/*
	** If we reach here, then we were passed a handshake version number we don't know about.
	** This shouldn't ever happen since our main() routine chose the version number.
	*/
	return false;
}
