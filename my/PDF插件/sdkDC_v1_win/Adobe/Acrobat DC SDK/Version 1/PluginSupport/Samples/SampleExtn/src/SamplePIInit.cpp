/*************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2011 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains
* the property of Adobe Systems Incorporated and its suppliers,
* if any.  The intellectual and technical concepts contained
* herein are proprietary to Adobe Systems Incorporated and its
* suppliers and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe Systems Incorporated.
**************************************************************************/
//------------------------------------------------------------------------
// SamplePIInit.cpp
// - Skeleton .cpp file for a plug-in. It implements the basic
//   handshaking methods required for all plug-ins.
//------------------------------------------------------------------------
// Acrobat Headers required for PI initialization
#include "PICommon.h"
#include "CorCalls.h"

#include "PIHeaders.h"

//For version number of SandboxHFT
#include "SandboxVers.h" 

#ifdef __cplusplus
extern "C" {
#endif

// extern HINSTANCE gHINSTANCE;
// Imported HFTS
HFT gSandboxHFT = 0;
ASUns32 gSandboxVersion = 0;

#ifdef __cplusplus
}
#endif

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/
// stuff for Menu set up 

#define SDK_MENUITEM_NAME			"Acrobat SDK"
#define SAMPLEUI_MENU_NAME			"SamplePI"
#define SAMPLEUI_MENU_ID			"ADBE:SampleMenu"
#define SAMPLEUI_MENU_TEXT			"SamplePI"

static AVMenuItem menuItem = NULL;
static AVMenu		gSampleMenu = (AVMenu)NULL;
static AVMenuItem	gSampleMenuItem = (AVMenuItem)NULL;

ACCB1 ASBool ACCB2 AddPluginMenuItems();

// callback functions implemented in file "SamplePI.cpp"
extern ACCB1 void ACCB2 DisplayMessageInBrokerCommand(void *clientData);
extern ACCB1 void ACCB2 DoSomeNastyStuffCommand(void *clientData);
extern ACCB1 ASBool ACCB2 MyPluginIsEnabled(void *clientData);

extern const char* MyPluginExtensionName;

/** 
	SamplePI is a plug-in template that provides a minimal implementation for 
	a plug-in to extend Sandbox Reader functionality.
	Developers may use this implementation as a basis for developing broker extensibility into their plug-in.
*/

/*-------------------------------------------------------
	Core Handshake Callbacks
-------------------------------------------------------*/

/* PluginExportHFTs
** ------------------------------------------------------
**/

/**
	Callback invoked by the application to give the plug-in an opportunity to register 
	an HFTServer with the application.
	@return true to indicate the plug-in should continue loading.
*/
ACCB1 ASBool ACCB2 PluginExportHFTs(void)
{
	return true;
}

/* PluginImportReplaceAndRegister
** ------------------------------------------------------
** */
/** 
	The application calls this function to allow it to
	<ul>
	<li> Import plug-in supplied HFTs.
	<li> Replace functions in the HFTs you're using (where allowed).
	<li> Register to receive notification events.
	</ul>

	@return true to continue loading plug-in,
	false to cause plug-in loading to stop.
*/
ACCB1 ASBool ACCB2 PluginImportReplaceAndRegister(void)
{
	//Import SandboxHFT
	//This is a new public HFT introduced to enable third party plug-in developers to extend reader broker
	//functionality at run time.
	gSandboxHFT = ASExtensionMgrGetHFT(ASAtomFromString("SandboxHFT"), SandboxHFT_LATEST_VERSION);
	gSandboxVersion = SandboxHFT_LATEST_VERSION;
	if (gSandboxHFT == NULL) {
		return false;
	}
	return true;
}

/* PluginInit
** ------------------------------------------------------
**/
/** 
	The main initialization routine.
	We register our action handler with the application.
	@return true to continue loading the plug-in, 
	false to cause plug-in loading to stop.
*/
ACCB1 ASBool ACCB2 PluginInit(void)
{
	return AddPluginMenuItems();
}

/* PluginUnload
** ------------------------------------------------------
**/
/** 
	The unload routine.
	Called when your plug-in is being unloaded when the application quits.
	Use this routine to release any system resources you may have
	allocated.

	Returning false will cause an alert to display that unloading failed.
	@return true to indicate the plug-in unloaded.
*/
ACCB1 ASBool ACCB2 PluginUnload(void)
{
	return true;
}

/* GetExtensionName
** ------------------------------------------------------
*/
/**
	Returns the unique ASAtom associated with your plug-in.
	@return the plug-in's name as an ASAtom.
*/
ASAtom GetExtensionName()
{
	return ASAtomFromString(MyPluginExtensionName);	/* Change to your extension's name */
}

/** PIHandshake
	function provides the initial interface between your plug-in and the application.
	This function provides the callback functions to the application that allow it to 
	register the plug-in with the application environment.

	Required Plug-in handshaking routine: <b>Do not change its name!</b>
	
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

/*-------------------------------------------------------
	Menu Utility
-------------------------------------------------------*/

/**
	A convenient function to add a menu item under Acrobat SDK menu.
	@param MyMenuItemTitle IN String for the menu item's title.
	@param MyMenuItemName IN String for the menu item's internal name.
	@return true if successful, false if failed.
	@see AVAppGetMenubar
	@see AVMenuItemNew
	@see AVMenuItemSetExecuteProc
	@see AVMenuItemSetComputeEnabledProc
	@see AVMenubarAcquireMenuItemByName
	@see AVMenubarAcquireMenuByName
*/
ACCB1 ASBool ACCB2 AddPluginMenuItems()
{
	AVMenubar menubar = AVAppGetMenubar();
	AVMenu volatile commonMenu = NULL;

	if (!menubar)
		return false;

	DURING

		gSampleMenu = AVMenuNew(SAMPLEUI_MENU_NAME, SAMPLEUI_MENU_ID, gExtensionID);
		if(gSampleMenu == NULL)
			ASRaise(GenError(genErrGeneral));

		gSampleMenuItem = AVMenuItemNew(SAMPLEUI_MENU_TEXT, SAMPLEUI_MENU_ID, gSampleMenu, false, NO_SHORTCUT, 0, NULL, gExtensionID);
		if(gSampleMenuItem == NULL)
			ASRaise(GenError(genErrGeneral));

		commonMenu = AVMenubarAcquireMenuByName (menubar, "ADBE:Acrobat_SDK");
		// if "Acrobat SDK" menu is not existing, create one.
		if (!commonMenu) {
			commonMenu = AVMenuNew (SDK_MENUITEM_NAME, "ADBE:Acrobat_SDK", gExtensionID);
			AVMenubarAddMenu(menubar, commonMenu, APPEND_MENU);		
		}
		
		AVMenuAddMenuItem(commonMenu, gSampleMenuItem, APPEND_MENUITEM);
		AVMenuRelease(commonMenu);
		AVMenuItemRelease(gSampleMenuItem);

		AVMenuItem DisplayMessageMenuItem = AVMenuItemNew("Display Message In Broker", "ADBE:DisplayMessage", (AVMenu)NULL, false,  NO_SHORTCUT, 0, NULL, gExtensionID);
		AVMenuItemSetExecuteProc(
			DisplayMessageMenuItem,
			ASCallbackCreateProto(AVExecuteProc, DisplayMessageInBrokerCommand),
			NULL);
		AVMenuItemSetComputeEnabledProc (DisplayMessageMenuItem,
					ASCallbackCreateProto(AVComputeEnabledProc, MyPluginIsEnabled), (void *)pdPermEdit);
		AVMenuAddMenuItem(gSampleMenu, DisplayMessageMenuItem, APPEND_MENUITEM);
		AVMenuItemRelease(DisplayMessageMenuItem);

		AVMenuItem DoSomeNastyStuffMenuItem = AVMenuItemNew("Launch Calc.exe", "ADBE:LaunchCalc.exe", (AVMenu)NULL, false,  NO_SHORTCUT, 0, NULL, gExtensionID);
		AVMenuItemSetExecuteProc(
			DoSomeNastyStuffMenuItem,
			ASCallbackCreateProto(AVExecuteProc, DoSomeNastyStuffCommand),
			NULL);
		AVMenuItemSetComputeEnabledProc (DoSomeNastyStuffMenuItem,
					ASCallbackCreateProto(AVComputeEnabledProc, MyPluginIsEnabled), (void *)pdPermEdit);
		AVMenuAddMenuItem(gSampleMenu, DoSomeNastyStuffMenuItem, APPEND_MENUITEM);
		AVMenuItemRelease(DoSomeNastyStuffMenuItem);


	HANDLER
		if (commonMenu)
			AVMenuRelease (commonMenu);
		return false;
	END_HANDLER

	return true;
}