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
COMMAND_REGISTER(CompareOutput,"compareoutput","compare's text in input file name with last resultString", "compare's output")

void
CompareOutputCommand::executeCommand(string notImplemented){
	//not implemented
}

//compares text in fileName with savedResultString
string
CompareOutputCommand::executeServerCommand(string fileName, string& outputResultString){
	
	ASPathName filePath = CDocument::MakeFullPath(reinterpret_cast<const ASUTF16Val*>(fileName.c_str()));
	ASFile asFile;
	ASErrorCode res = ASFileSysOpenFile(NULL, filePath, ASFILE_READ, &asFile);
	
	if(res == 0)
	{
		char buf[10000];
		ASTArraySize bytesRead = ASFileRead (asFile, buf, 10000);
		ASFileClose(asFile);
		CLIController* controller = CLIController::Instance();
		char foo[10000];
		strncpy(foo, controller->getLastResultString().c_str(), sizeof(foo) - 1);
		int cmpResult = strncmp(buf, controller->getLastResultString().c_str(), bytesRead);
		if(cmpResult == 0)
			outputResultString += "match";
		else
		{
			
			outputResultString += "difference";
			
			for(int i = 0; i< bytesRead;i++)
			{
				if(foo[i] != buf[i])
				{
					char st[20];
#ifdef MAC_PLATFORM
					snprintf(st, sizeof(st), " bytes %d, len %d, %d foo %c buf %c", bytesRead, strlen(foo), i, foo[i], buf[i]);
#else
					sprintf_s(st, sizeof(st), " bytes %d, len %d, %d foo %c buf %c", bytesRead, strlen(foo), i, foo[i], buf[i]);
#endif
					outputResultString += st;
				}
			}
		}
	}
	else
		outputResultString += "error: file not found";
	return outputResultString;
}
