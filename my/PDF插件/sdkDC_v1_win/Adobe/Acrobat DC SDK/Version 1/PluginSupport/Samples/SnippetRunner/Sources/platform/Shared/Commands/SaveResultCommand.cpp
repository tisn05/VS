/*
//  ADOBE SYSTEMS INCORPORATED
//  Copyright (C) 2000-2006 Adobe Systems Incorporated
//  All rights reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the Adobe license agreement
//  accompanying it. If you have received this file from a source other
//  than Adobe, then your use, modification, or distribution of it
//  requires the prior written permission of Adobe.
//
*/
#include "CDocument.h"
#include "ICommand.h"
#include "CLIController.h"

#include <string>

using namespace std;

/* Use the macro helper for this class */
COMMAND_REGISTER(SaveResult,"saveResult","save last resultString to file specified", "Saves result")


void
SaveResultCommand::executeCommand(string notImplemented){
	//not implemented
}

//Saves the last result in the specified file
string
SaveResultCommand::executeServerCommand(string fileName, string& outputResultString){
	
	ASPathName filePath = CDocument::MakeFullPath(reinterpret_cast<const ASUTF16Val*>(fileName.c_str()));
	ASFile asFile;
	ASErrorCode res = ASFileSysOpenFile(NULL, filePath, ASFILE_WRITE | ASFILE_CREATE, &asFile);
	
	if(res == 0)
	{
		CLIController* controller = CLIController::Instance();
		ASTArraySize bytesWritten = ASFileWrite (asFile, controller->getLastResultString().c_str(), 
													controller->getLastResultString().size());
		ASFileClose(asFile);
		if(bytesWritten != 0)
			outputResultString += "file written";
		else
			outputResultString = "error: error writing file";
	}
	else
		outputResultString += "error: file not found";
	return outputResultString;
}
