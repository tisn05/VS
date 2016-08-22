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

 DMSIntegrationInit.cpp

*********************************************************************/

// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

extern ACCB1 ASBool ACCB2 InitilizeDMSFileSystems(void);

extern ACCB1 ASBool ACCB2 MyAVAppOpenDialog(
	AVOpenSaveDialogParams dialogParams,			
	ASFileSys* outFileSys,			
	ASPathName** outASPathNames,		
	AVArraySize* outNumASPathNames,		
	AVFilterIndex	*ioChosenFilterIndex);

extern ACCB1 ASBool ACCB2 MyAVAppChooseFolderDialog(
	AVOpenSaveDialogParams dialogParams,
	ASFileSys* outFileSys,
	ASPathName* outASPathName);

extern ACCB1 ASBool ACCB2 MyAVAppSaveDialog(
	AVOpenSaveDialogParams dialogParams,			
	ASFileSys		*outFileSys,			
	ASPathName		*outASPathName,			
	AVFilterIndex	*ioChosenFilterIndex);

/*-------------------------------------------------------
	Declarations/definitions
-------------------------------------------------------*/
// Callback(s) For Replaced Functions
ASCallback gcbAVAppOpenDialog;
ASCallback gcbAVAppChooseFolderDialog;
ASCallback gcbAVAppSaveDialog;

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
	// Create the replacement function callback(s)
	// Replace AVAppOpenDialog
	gcbAVAppOpenDialog = (void*)ASCallbackCreateReplacement(AVAppOpenDialogSEL, 
		MyAVAppOpenDialog);
	REPLACE(gAcroViewHFT, AVAppOpenDialogSEL, gcbAVAppOpenDialog);
	
	// Replace AVAppSaveDialog
	gcbAVAppSaveDialog = (void*)ASCallbackCreateReplacement(AVAppSaveDialogSEL, 
		MyAVAppSaveDialog);
	REPLACE(gAcroViewHFT, AVAppSaveDialogSEL, gcbAVAppSaveDialog);

	// Replace AVAppChooseFolderDialog
	gcbAVAppChooseFolderDialog = (void*)ASCallbackCreateReplacement(
		AVAppChooseFolderDialogSEL, 
		MyAVAppChooseFolderDialog);
	REPLACE(gAcroViewHFT, AVAppChooseFolderDialogSEL, gcbAVAppChooseFolderDialog);

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
	return true;
}

/* GetExtensionName
** ------------------------------------------------------
**
** Return the unique ASAtom associated with your plug-in.
*/
ASAtom GetExtensionName ()
{
	return ASAtomFromString ("AcroSDK:DMSIntegration");
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
		hsData->initCallback = (void*)ASCallbackCreateProto(PIInitProcType, &InitilizeDMSFileSystems);

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
