/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 InvokeAccessibilityCheckerCmdSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

// directly pull in AVCmdDefs.h which defines some command specific enums.
#include "AVCmdDefs.h"

/** Invokes the AccessibilityChecker command. This command has no inputs, but various parameters. 
	We indicate the command is to be executed with no UI.
	@testfile TwoColumnTaggedDoc.pdf
	@requires open pdf
	@see AVCommandNew
	@see ASCabNew
	@see ASCabPutInt
	@see AVCommandSetConfig
	@see ASCabPutBool
	@see ACCommandSetParams
	@see ASCabDestroy
	@see AVCommandDestroy
	@see AVCommandExecute
*/
void InvokeAccessibilityCheckerCmdSnip()
{
	ASAtom cmdName;
	AVCommand volatile cmd = NULL;
	AVCommandStatus cmdStatus;
	volatile ASCab config = NULL;
	volatile ASCab params = NULL;
	ASBool errorState = false;
	DURING
		// Create an instance of the command.
		cmdName = ASAtomFromString(kAVCommandAccessibilityCheck);
		cmd = AVCommandNew(cmdName);
		if (cmd==NULL){
			// if we cannot get the command, just alert. 
			AVAlertNote("Cannot obtain the command");
			errorState = true;
			
		}
		if (errorState == false){
			// We wish to invoke this command without any user interaction. See AVExpT.h for other UI policies
			config = ASCabNew();
			ASCabPutInt(config, kAVCommandUIPolicy, kAVCommandUISilent);

			// Set command configuration 
			cmdStatus = AVCommandSetConfig(cmd, config);

			// this command has no inputs.
			
			// we set up the parameters here, we will leave most as the default values.
			params = ASCabNew();

			// we are running the command in batch mode
			ASCabPutBool(params,kAccCheckCmdKeyInBatch,true);

			// indicate we want repair hints
			ASCabPutBool(params,kAccCheckCmdKeyHints,true);
			
			// indicate we wish to output a log file
			ASCabPutBool(params,kAccCheckCmdKeyCreateLog,false);
			
			// indicate we do not want alternate descriptions to be checked
			// I do this to add a bit of variety...
			ASCabPutBool(params,kAccCheckCmdKeyAlternateText,false);
			
			// the report is in the form of new annotations added to the document.
			ASCabPutBool(params,kAccCheckCmdKeyCreateComments,true);
			
			cmdStatus = AVCommandSetParams(cmd, params);
		}
			
	HANDLER
		cmdStatus = kAVCommandInError;
	END_HANDLER

	if (ASBoolToBool(errorState) == true){
		return;
	}
	
	if (params != NULL){
		ASCabDestroy(params);
	}
	if (config!=NULL){
		ASCabDestroy(config);
	}
		
	// we have set up the command, make sure there isn't a latent error
	// We're in silent mode, so we won't throw an alert dialog.
	if (kAVCommandInError == cmdStatus) {
		AVCommandDestroy(cmd);
		return;
	}

	// we should be good to go at this point, execute the command.
	cmdStatus = AVCommandExecute(cmd);

	// Release the resources associated with the command.
	AVCommandDestroy(cmd);
	
	Console::displayString("Accessibility checker invoked\n");
}

SNIPRUN_REGISTER(InvokeAccessibilityCheckerCmdSnip, "AV Layer:Commands:Invoke AccessibilityChecker Cmd", "Invokes the AccessibilityChecker command.")


