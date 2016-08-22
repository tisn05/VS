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

 SnippetRunnerInit.cpp

 - Implements handshake and exporting HFT routines.

*********************************************************************/

// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif
// Macro to Fix Visual C++ Errors
#define _ALLOW_KEYWORD_MACROS

#include <string>
#include <algorithm>

#include "CLIController.h"
#include "SnippetRunner.h"
#include "SnipManager.h"
#include "SnippetRunnerUtils.h"
#ifndef UNIX_PLATFORM
#include "DebugWindowOStream.h"
#endif

#include "DebugWindowHFT.h"

using namespace std;

// we keep a static menu item in case we have to unload.
static AVMenuItem menuItem = NULL;
static HFTServer gDebugWindowHFTServer;
HFT gDebugWindowHFT;


HFT gAcroFormHFT;

/*-------------------------------------------------------
	Core Handshake Callbacks
-------------------------------------------------------*/
/*-------------------------------------------------------
	HFT Support Methods
-------------------------------------------------------*/
ACCB1 void ACCB2 DebugEmitStringImpl (const char *msg)
{
	string strMsg = "[hft] ";
	strMsg += msg;
	Console::displayStringNow(strMsg);
}


/* CreateDebugWindowHFT
** ----------------------------------------------------------------------
**/
/** Create our HFT and populate it.
*/
static void CreateDebugEmitStringHFT (void)
{
	gDebugWindowHFT = HFTNew (gDebugWindowHFTServer, DebugWindowNUMSELECTORS);
	HFTReplaceEntry (gDebugWindowHFT, DebugEmitStringSEL,
		(void*)ASCallbackCreateReplacement(DebugEmitStringSEL, &DebugEmitStringImpl), 0);
}

/** ------------------------------------------------------
***/
/** Called when another plug-in attempts to acquire
	our HFT. Create the HFT if we haven't done so.

	@return the HFT, or NULL if the user has requested
	version number that we don't support.
*/
static ACCB1 HFT ACCB2 ProvideDebugWindowHFT (HFTServer server, Uns32 version, void *clientData)
{
	if (version != 1)
		return NULL;

	if (!gDebugWindowHFT)
		CreateDebugEmitStringHFT();

	return gDebugWindowHFT;
}
/* PluginExportHFTs
** ------------------------------------------------------
**
** Create and register the HFT's.
**
** Return true to continue loading plug-in.
** Return false to cause plug-in loading to stop.
*/
ACCB1 ASBool ACCB2 PluginExportHFTs(void)
{
	gDebugWindowHFT = NULL;
	gDebugWindowHFTServer = HFTServerNew ("ADBE:DebugWindow",
		ASCallbackCreateProto(HFTServerProvideHFTProc, &ProvideDebugWindowHFT), NULL, NULL);

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
ACCB1 ASBool ACCB2 PluginImportReplaceAndRegister(void)
{
	// Get HFT for AcroForm plug-in.
	gAcroFormHFT = Init_AcroFormHFT;

	return true;
}

/* PluginInit
** ------------------------------------------------------
**
** The main initialization routine.
**
** Return true to continue loading plug-in.
** Return false to cause plug-in loading to stop.
*/
ACCB1 ASBool ACCB2 PluginInit(void)
{
	// force the creation of SRU
	SnippetRunnerUtils::Instance();
	
	//create an idle proc to fetch commands
	AVAppRegisterIdleProc (CLIController::CLIIdleProc, NULL, 20);


	return true;
}


/* PluginUnload
** ------------------------------------------------------
**
** The unload routine.
**
** Called when your plug-in is being unloaded when the application quits.
** Use this routine to release any system resources you may have
** allocated.
**
** Returning false will cause an alert to display that unloading failed.
*/
ACCB1 ASBool ACCB2 PluginUnload(void)
{
	SnippetRunnerUtils::deleteInstance();
	return true;
}

/* GetExtensionName
** ------------------------------------------------------
**
** Get the extension name associated with this plugin
*/
ASAtom GetExtensionName()
{
	return ASAtomFromString("ADBE:SnippetRunnerInterface");	/* Change to your extension's name */
}

/*
** PIHandshake
** Required Plug-in handshaking routine: Do not change it's name!
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


