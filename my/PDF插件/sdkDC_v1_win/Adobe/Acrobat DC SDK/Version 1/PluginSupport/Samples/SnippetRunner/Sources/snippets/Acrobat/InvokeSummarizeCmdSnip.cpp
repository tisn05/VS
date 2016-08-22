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

 InvokeSummarizeCmdSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

// directly pull in AVCmdDefs.h which defines some command specific enums.
#include "AVCmdDefs.h"

/** Invokes the summarize comments command. This command has no inputs, but various parameters. 
	We indicate the command is to be executed with no UI.
	@testfile comments.pdf
	@requires comments to summarize
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
void InvokeSummarizeCmdSnip()
{
	ASAtom cmdName;
	AVCommand volatile cmd = NULL;
	AVCommandStatus cmdStatus;
	volatile ASCab config = NULL;
	volatile ASCab params = NULL;
	ASBool errorState = false;
	DURING
		// Create an instance of the command.
		cmdName = ASAtomFromString(kAVCommandSummarizeComments);
		cmd = AVCommandNew(cmdName);
		if (cmd==NULL){
			// if we cannot get the command, just alert. 
			AVAlertNote("Cannot obtain the command");
			errorState = true;
			
		}
		if (errorState == false){
			// We wish to invoke this command without any user interaction. See AVExpT.h for other UI policies
			config = ASCabNew();
			ASCabPutInt(config, "UIPolicy", kAVCommandUISilent);

			// Set command configuration 
			cmdStatus = AVCommandSetConfig(cmd, config);

			// this command has no inputs.
			
			// we set up the parameters here
			params = ASCabNew();

			// the font is defined in AVCmdDefs.h
			ASCabPutInt(params,"FontSize",kAVFontSizeSmall);

			// we want single page output
			ASCabPutBool(params, "TwoUp", true);
			
			// use sequence numbers
			ASCabPutBool(params, "UseSeqNum", true);

			ASCabPutBool(params, "IncludeAll", true);

			// Set the command parameters.
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
	
	Console::displayString("Summarize Comments Cmd invoked\n");
}

SNIPRUN_REGISTER(InvokeSummarizeCmdSnip, "AV Layer:Commands:Invoke Summarize Cmd", "Invokes the summarize comments command.")


