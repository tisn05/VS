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

 ReplaceMethod.cpp

 - Replaces the AVAlert method. with a function that reverses the
   string before displaying it using the original AVAlert method.

 - The replacement method affects ALL calls to AVAlert, not just those
   made by this plug-in. In addition, methods including AVAlertNote
   and AVAlertConfirm call AVAlert so they too are affected.

*********************************************************************/

#include "SnippetRunner.h"


/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

static ASCallback cbAVAlert = NULL;

/*-------------------------------------------------------
	Callbacks
-------------------------------------------------------*/

/* AVAlertProc
** ------------------------------------------------------
**
** Reverses the string to be displayed, and calls the
** default AVAlert().
**
** Returns the ID of the dialog item that the user
** clicked to dismiss the dialog.
*/
static ACCB1 ASInt32 ACCB2 AVAlertProc (ASInt32 iconType, const char *msg, const char *button1,
	const char *button2, const char *button3, ASBool beep)
{
	char *gsm = NULL;
	ASInt32 msgLength = strlen(msg), retVal = 0, err = 0;

	gsm = (char*)ASmalloc(sizeof(char) * (msgLength + 1));
	if (gsm) {

		// Reverse the string.
		for (ASInt32 i = 0; i < msgLength; i++)
			gsm[i] = msg[msgLength - (i + 1)];

		gsm[msgLength] = '\0';
	}

	DURING
		retVal = CALL_REPLACED_PROC(gAcroViewHFT, AVAlertSEL, cbAVAlert)
						(iconType, (gsm) ? gsm : msg, button1, button2, button3, beep);
	HANDLER
		// Our existence is transparent to clients of AVAlert so
		// we'll just re-raising the exception to them after we
		// release the memory we alloc'd.
		err = ERRORCODE;
	END_HANDLER

	// Release the memory for gsm if we alloc'd it.
	if (gsm)
		ASfree ((void*)gsm);

	if (0 != err)
		ASRaise (err);

	return retVal;
}

/* ReplaceAVAlert
** ------------------------------------------------------
**
** Can't remove without restarting Acrobat
*/
void ReplaceMethodSnip ()
{
	if(cbAVAlert == NULL)
	{
		// Replace the AVAlert method and give it a whirl.
		cbAVAlert = (void*)ASCallbackCreateReplacement(AVAlertSEL, &AVAlertProc);
		REPLACE(gAcroViewHFT, AVAlertSEL, cbAVAlert);

		AVAlertNote("Replace Method installed");		
	}
	else
		AVAlertNote("Replace Method previously installed");

}


SNIPRUN_REGISTER(ReplaceMethodSnip, "AV Layer:Replace Method", "Replaces AVAlert Method")
	

