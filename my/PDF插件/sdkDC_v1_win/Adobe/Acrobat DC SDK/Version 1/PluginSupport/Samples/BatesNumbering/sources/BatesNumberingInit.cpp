/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1999-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 BatesNumberingInit.cpp

*********************************************************************/

// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

#include "PDBatesCalls.h"

/*-------------------------------------------------------
	Declarations/definitions
-------------------------------------------------------*/
extern ACCB1 void ACCB2 ExecuteExtractBates (void *data);
extern ACCB1 void ACCB2 ExecuteAddBates (void *data);
extern ACCB1 void ACCB2 ExecuteRemoveBates (void *data);

HFT gPDBatesHFT = NULL;
static AVMenuItem menuItem = NULL;

/*-------------------------------------------------------
	UI Callbacks
-------------------------------------------------------*/

/* ExtractBatesInfoComputeEnabled
** ------------------------------------------------------
**
** Determine whether our UI element(s) should be enabled.
*/
ACCB1 ASBool ACCB2 SDKBatesMenuEnabled (void *data)
{
	return (AVAppGetActiveDoc() != NULL);
}

/*-------------------------------------------------------
	Handshake Callbacks
-------------------------------------------------------*/

/* PluginExportHFTs
** ------------------------------------------------------
**
** Create and register the HFT's.
**
** Return true to continue loading plug-in.
** Return false to cause plug-in loading to stop.
*/
ACCB1 ASBool ACCB2 PluginExportHFTs ()
{
	return true;
}

/* PluginImportReplaceAndRegister
** ------------------------------------------------------
**
** This routine is where you can:
**	1) Import plug-in supplied HFTs.
**	2) Replace functions in the HFTs you're using (where allowed).
**	3) Register to receive notification events.
**
** Return true to continue loading plug-in.
** Return false to cause plug-in loading to stop.
*/
ACCB1 ASBool ACCB2 PluginImportReplaceAndRegister ()
{
#ifdef READER_PLUGIN
	return true;
#else
	gPDBatesHFT = ASExtensionMgrGetHFT(ASAtomFromString(PDBatesHFTName), PDBATESHFT_LATEST_VERSION);
	return (gPDBatesHFT != NULL);
#endif
}

/* PluginInit
** ------------------------------------------------------
**
** The main initialization routine.
**
** Return true to continue loading plug-in.
** Return false to cause plug-in loading to stop.
*/
ACCB1 ASBool ACCB2 PluginInit ()
{
	AVMenubar menubar = AVAppGetMenubar();
	AVMenu volatile commonMenu = NULL;

	commonMenu = AVMenubarAcquireMenuByName (menubar, "ADBE:Acrobat_SDK");
	// if "Acrobat SDK" menu is not existing, create one.
	if (!commonMenu) {
		commonMenu = AVMenuNew ("Acrobat SDK", "ADBE:Acrobat_SDK", gExtensionID);
		AVMenubarAddMenu(menubar, commonMenu, APPEND_MENU);		
	}
		
	// Create our main sub-menu
	AVMenu sdkBatesSubMenu = AVMenuNew("Bates Numbering", "ADBE:SDKBates", gExtensionID);
	// Create our main menu item and add it to the "Acrobat SDK" menu
	menuItem = AVMenuItemNew("Bates Numbering", "ADBE:SDKBates", sdkBatesSubMenu, 
		false, NO_SHORTCUT, 0, NULL, gExtensionID);
	// Add new menu item to "Acrobat SDK" menu
	AVMenuAddMenuItem(commonMenu, menuItem, APPEND_MENUITEM);

	// Add our menu items to this menu
	AVMenuItem sdkExtractBatesMenuItem = AVMenuItemNew("Extract...", 
		"ADBE:SDKExtractBates", NULL, false, NO_SHORTCUT, 0, NULL, gExtensionID);
	AVMenuItemSetExecuteProc(sdkExtractBatesMenuItem,
		ASCallbackCreateProto(AVExecuteProc, &ExecuteExtractBates), NULL);
	AVMenuItemSetComputeEnabledProc(sdkExtractBatesMenuItem,
		ASCallbackCreateProto(AVComputeEnabledProc, &SDKBatesMenuEnabled), NULL);
	AVMenuAddMenuItem(sdkBatesSubMenu, sdkExtractBatesMenuItem, APPEND_MENUITEM);

#ifndef READER_PLUGIN
	AVMenuItem sdkAddBatesMenuItem = AVMenuItemNew("Add...", "ADBE:SDKAddBates", NULL, false,
		NO_SHORTCUT, 0, NULL, gExtensionID);
	AVMenuItemSetExecuteProc(sdkAddBatesMenuItem,
		ASCallbackCreateProto(AVExecuteProc, &ExecuteAddBates), NULL);
	AVMenuItemSetComputeEnabledProc(sdkAddBatesMenuItem,
		ASCallbackCreateProto(AVComputeEnabledProc, SDKBatesMenuEnabled), NULL);
	AVMenuAddMenuItem(sdkBatesSubMenu, sdkAddBatesMenuItem, APPEND_MENUITEM);

	AVMenuItem sdkRemoveBatesMenuItem = AVMenuItemNew("Remove...", "ADBE:SDKRemoveBates", NULL, false,
		NO_SHORTCUT, 0, NULL, gExtensionID);
	AVMenuItemSetExecuteProc(sdkRemoveBatesMenuItem,
		ASCallbackCreateProto(AVExecuteProc, &ExecuteRemoveBates), NULL);
	AVMenuItemSetComputeEnabledProc(sdkRemoveBatesMenuItem,
		ASCallbackCreateProto(AVComputeEnabledProc, SDKBatesMenuEnabled), NULL);
	AVMenuAddMenuItem(sdkBatesSubMenu, sdkRemoveBatesMenuItem, APPEND_MENUITEM);
#endif

	// Release the menu resources.
	AVMenuRelease(commonMenu);
	AVMenuRelease(sdkBatesSubMenu);
	AVMenuItemRelease(sdkExtractBatesMenuItem);
#ifndef READER_PLUGIN
	AVMenuItemRelease(sdkAddBatesMenuItem);
	AVMenuItemRelease(sdkRemoveBatesMenuItem);
#endif

	return true;
}

/* PluginUnload
** ------------------------------------------------------
**
** Called when your plug-in is being unloaded when the
** application quits. Use this routine to release any
** system resources you may have allocated.
**
** Returning false will cause an alert to show warning
** that unloading failed.
*/
ACCB1 ASBool ACCB2 PluginUnload ()
{
	if (menuItem)
		AVMenuItemRemove(menuItem);

	return true;
}

/* GetExtensionName
** ------------------------------------------------------
**
** Return the unique ASAtom associated with your plug-in.
*/
ASAtom GetExtensionName ()
{
	return ASAtomFromString ("AcroSDK:ExtractBatesInfo");
}

/*
** PIHandshake
** Required Plug-in handshaking routine: Do not change it's name!
** PIMain.c "knows" the name PIHandshake and the type is dictated by Acrobat.
*/
ACCB1 ASBool ACCB2 PIHandshake(Uns32 handshakeVersion, void *handshakeData)
{
	if (handshakeVersion == HANDSHAKE_V0200) {
		/* Cast handshakeData to the appropriate type */
		PIHandshakeData_V0200 *hsData = (PIHandshakeData_V0200 *)handshakeData;

		/* Set the name we want to go by */
		hsData->extensionName = GetExtensionName();

		/* If you export your own HFT, do so in here */
		hsData->exportHFTsCallback = (void*)ASCallbackCreateProto(PIExportHFTsProcType, &PluginExportHFTs);

		/*
		** If you import plug-in HFTs, replace functionality, and/or want to register for notifications before
		** the user has a chance to do anything, do so in here.
		*/
		hsData->importReplaceAndRegisterCallback = (void*)ASCallbackCreateProto(PIImportReplaceAndRegisterProcType,
																		 &PluginImportReplaceAndRegister);

		/* Perform your plug-in's initialization in here */
		hsData->initCallback = (void*)ASCallbackCreateProto(PIInitProcType, &PluginInit);

		/* Perform any memory freeing or state saving on "quit" in here */
		hsData->unloadCallback = (void*)ASCallbackCreateProto(PIUnloadProcType, &PluginUnload);

		/* All done */
		return true;

	} /* Each time the handshake version changes, add a new "else if" branch */

	/*
	** If we reach here, then we were passed a handshake version number we don't know about.
	** This shouldn't ever happen since our main() routine chose the version number.
	*/
	return false;
}
