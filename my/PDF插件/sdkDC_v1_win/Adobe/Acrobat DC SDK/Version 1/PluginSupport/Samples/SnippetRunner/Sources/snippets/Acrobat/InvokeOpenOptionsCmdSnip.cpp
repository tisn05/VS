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
	  
	InvokeOpenOptionsCmdSnip.cpp
		
*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

// directly pull in AVCmdDefs.h which defines some command specific enums.
#include "AVCmdDefs.h"

/** Invokes the OpenOptions command.This command specifies how a document is opened. Invoking this 
	command sets the open options for the current front document. After saving the document, the 
	next time it is opened it will open in single page mode and with toolbars turned off. 
	Although this snippet does not directly need the front document (or a document opened), the
	command it invokes does so we check for an open document before executing the command.
	@requires open document
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
void InvokeOpenOptionsCmdSnip()
{
	CDocument document;
	PDDoc pdDoc = (PDDoc)document;
	if (pdDoc == NULL){
		return;
	}
	ASAtom cmdName;
	AVCommand volatile cmd = NULL;
	AVCommandStatus cmdStatus;
	volatile ASCab config = NULL;
	volatile ASCab params = NULL;
	ASBool errorState = false;
	
	DURING
		// Create an instance of the command.
		cmdName = ASAtomFromString(kAVCommandSetOpenOptions);
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
			
			// this command has no inputs, just using Active Doc.
			
			// set parameters.
			params = ASCabNew();
			// for this command, we set it to single page mode, and turn
			// off the toolbar. See AVCmdDefs.h for the other definitions.
			ASCabPutInt(params, kOpenOptionsCmdKeyPageLayout, PDLayoutSinglePage);
			ASCabPutInt(params, kOpenOptionsCmdKeyPageMode, PDUseNone );
			ASCabPutBool(params, kOpenOptionsCmdKeyHideToolbar, true );
			
			// as well as setting the new value in the parameter cab,
			// we also need to indicate through the LeaveAsIs parameter
			// cab that we do not want these two settings "left as is"
			ASCab newLeaveAsIs = ASCabNew();
			ASCabPutBool(newLeaveAsIs,kOpenOptionsCmdKeyPageLayout,false);
			ASCabPutBool(newLeaveAsIs,kOpenOptionsCmdKeyPageMode,false);
			ASCabPutBool(newLeaveAsIs,kOpenOptionsCmdKeyHideToolbar,false);
			
			ASCabPutCab(params,kDocCmdKeyLeaveAsIs,newLeaveAsIs);
			cmdStatus = AVCommandSetParams(cmd, params);
		}
	
	HANDLER
		cmdStatus = kAVCommandInError;
	END_HANDLER
		
        if (ASBoolToBool(errorState) == true){
			return;
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
        
        Console::displayString("Open Options Cmd invoked\n");
}

SNIPRUN_REGISTER(InvokeOpenOptionsCmdSnip, "AV Layer:Commands:Invoke Open Option Cmd", "Invokes the OpenOption command.")
	

