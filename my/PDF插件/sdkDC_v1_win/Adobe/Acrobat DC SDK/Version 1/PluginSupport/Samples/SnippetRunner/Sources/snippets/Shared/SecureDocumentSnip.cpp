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

 SecureDocumentSnip.cpp

 - The SetSecurity samples demonstrates applying security settings
    programmatically.

 - After the toolbutton is pressed, the standard security dialog is
   shown if the user has the appropriate permissions. The user can
   specify security settings that will be applied to the file. If the
   user commits the selection, the settings are applied to the file.

 - If SILENT_SECURE is defined, the plug-in applies a pre-defined set of
   security settings

 - Note that the calls that modify the security settings are followed by
   PDDocSetFlags ((PDDoc)data, PDDocRequiresFullSave); Acrobat does not
   dirty the document unless this call is made.

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

#if ACROBAT_LIBRARY
#define SILENT_SECURE
#define MAX_PWD_LEN 100

ASBool AVCryptGetPassword (PDDoc pdDoc, PDPerms permWanted, void** authData)
{
	char *password = (char *)ASmalloc( MAX_PWD_LEN );
	Console::displayString( "Enter the owner password : " );
	strncpy( password, Console::getString().c_str(), MAX_PWD_LEN );
	*authData = password;
	return true;
}

#endif

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/* SetSecurity
** ------------------------------------------------------
**
** If SILENT_SECURE has been defined, silently applies
** security settings to the document, otherwise they
** are configured interactively.
*/
static void SetSecurity (PDDoc pdDoc, void *existingSecData)
{
	StdSecurityData volatile securityData = NULL;
	char errorBuf[256];
	ASBool bSecurityWasSet = false;

	DURING

		// Set the new security handler for the document. Get a newly
		// allocated authData structure.

		PDDocSetNewCryptHandler (pdDoc, ASAtomFromString("Standard"));
		securityData = (StdSecurityData)PDDocNewSecurityData (pdDoc);

#ifdef SILENT_SECURE
		{
			// Warning! These settings limit the compatability of
			// the document to Acrobat 5 or later.

			securityData->size = sizeof(StdSecurityDataRec);
			securityData->newUserPW = true;
			securityData->hasUserPW = true;
			strcpy (securityData->userPW, "secure");
			securityData->perms = pdPrivPermFillandSign | pdPrivPermAccessible;
			securityData->keyLength = 16;

#else
		// If there are existing security settings, copy these into
		// our data structure so that the dialog is initialized with
		// those settings.

		if (existingSecData)
			memcpy ((void*)securityData, existingSecData, sizeof(StdSecurityDataRec));

		// Display security settings dialog.
		if (AVCryptDoStdSecurity (pdDoc, (void*)securityData)) {
#endif

			// Changing the security settings requires a full save.
			PDDocSetNewSecurityData (pdDoc, (void*)securityData);
			PDDocSetFlags(pdDoc, PDDocRequiresFullSave);
			bSecurityWasSet = true;
		}
	HANDLER
		ASGetErrorString (ASGetExceptionErrorCode(), errorBuf, 255);
		AVAlertNote (errorBuf);
	END_HANDLER

	if (bSecurityWasSet)
		Console::displayString("Security settings have been applied to the file.");
	else if (securityData)
		ASfree ((void*)securityData);

	return;
}

/* IsStandardSecurity
** ------------------------------------------------------
**
** Returns true if the document has been secured using
** standard handler.
*/
static ASBool IsStandardSecurity (PDDoc pdDoc)
{
	ASAtom atom = PDDocGetNewCryptHandler (pdDoc);
	return (atom == ASAtomFromString("Standard"));
}

/** Verifies that the user has the permissions required
	to modify the securiy settings. If so, we go ahead
	and modify them.
	@testfile blank.pdf
	@requires open pdf
	@see PDDocSetNewCryptHandler
	@see PDDocNewSecurityData
	@see AVCryptDoStdSecurity
	@see PDDocSetNewSecurityData
	@see PDDocSetFlags
	@see PDDocGetNewCryptHandler
	@see PDDocGetSecurityData
	@see PDDocPermRequest
	@see AVCryptGetPassword
*/
ACCB1 void ACCB2 SecureDocumentSnip()
{
	void *existingSecData = NULL;
	char errorBuf[256];
	void * volatile pwd = NULL;

	DURING
		// Get the active document.

		CDocument document;
		PDDoc pdDoc = (PDDoc)document;
		if(pdDoc == NULL) E_RTRN_VOID;

		existingSecData = (StdSecurityData)PDDocGetSecurityData(pdDoc);
		if (!existingSecData) {

			// An un-secured doc!
			SetSecurity (pdDoc, NULL);
			E_RTRN_VOID;
		}

		if (IsStandardSecurity(pdDoc)) {

			// Check to see if the current user is authorized to modify
			// the security on the document.

			if (PDPermReqGranted == PDDocPermRequest(pdDoc, PDPermReqObjDoc,
														PDPermReqOprSecure, NULL)) {
				SetSecurity (pdDoc, existingSecData);
				E_RTRN_VOID;
			}

			// They don't have permission. Prompt for the owner password.
			AVCryptGetPassword (pdDoc, pdPermSecure, (void **)&pwd);
			if (PDPermReqGranted == PDDocPermRequest(pdDoc, PDPermReqObjDoc,
												PDPermReqOprSecure, (void*)pwd))
				SetSecurity (pdDoc, existingSecData);
			else
				AVAlertNote("Owner password is incorrect");
		}
		else
			AVAlertNote ("Plug-in only supports standard security handler");

	HANDLER
		ASGetErrorString (ASGetExceptionErrorCode(), errorBuf, 255);
		AVAlertNote (errorBuf);
	END_HANDLER

	// Release the memory allocated for the password.
	if (pwd)
		ASfree ((void*)pwd);
}


SNIPRUN_REGISTER(SecureDocumentSnip, "PD Layer:Secure Document", "Sets security on the document.")
