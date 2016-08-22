/*
//
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
#include "PlatformUtils.h"

#include <string>

using namespace std;

/* Use the macro helper for this class */
COMMAND_REGISTER(SetDir,"setdir","Set's the current working directory", "Sets directory")

void
SetDirCommand::executeCommand(string pathIN)
{

//pathIN must be a full POSIX path on the Mac	
#ifdef MAC_PLATFORM	
		if (pathIN.c_str()[0] != '/') {
			Console::displayString("Please enter the full POSIX path for the directory. Base directory not updated.");
			return;
		}
#endif
	
		CDocument::SetBaseDirectory(pathIN);
		ASPathName newPath = CDocument::GetBaseDirectory();
		char* newPathString = ASFileSysDisplayStringFromPath (NULL, newPath);
		string resultString = "";
		resultString += "path changed to ";
		resultString += newPathString;
		resultString += "\n";
		ASfree(newPathString);
		Console::displayString(resultString);
}

string SetDirCommand::executeServerCommand(string pathIN, string& resultString) 
{
	//firefox 2.0 has a problem when double-clicking the swf
	if(strncmp(pathIN.c_str(), "localhost", 9) == 0)
		pathIN = &(pathIN.c_str())[9];
	//IE 7 has a change in that it uses URL encoded strings
	int pos = 0;
	while((pos = pathIN.find("%20")) != string::npos)
		pathIN.replace(pos, 3, " ");

//pathIN must be a full POSIX path on the Mac	
#ifdef MAC_PLATFORM	
		if (pathIN.c_str()[0] != '/') {
			resultString = "Please enter the full POSIX path for the directory. Base directory not updated.";
			return resultString;
		}
#endif
	
	CDocument::SetBaseDirectory(pathIN);
	ASPathName newPath = CDocument::GetBaseDirectory();
	char* newPathString = ASFileSysDisplayStringFromPath (NULL, newPath);
	resultString += "path changed to ";
	resultString += newPathString;
	resultString += "\n";
	ASfree(newPathString);
	return resultString;
}

