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
#include "ICommand.h"
#include "Console.h"
#include "SnipManager.h"
#include "SnippetRunner.h"
#include "SnippetHierarchyUtils.h"
#include <string>


using namespace std;

/* Use the macro helper for this class */
COMMAND_REGISTER(PrintWorkingDir,"pwd","Reports the current directory.", "reports the directory")

void
PrintWorkingDirCommand::executeCommand(string){
	// the SnippetHierarchyUtils manages the set of snippets.
	SnippetHierarchyUtils * theSHU = SnippetHierarchyUtils::Instance();

	ASCab currentNode = theSHU->getCurrentCab();
	Console::displayString(theSHU->getCurrentPath() +"\n");
}


string PrintWorkingDirCommand::executeServerCommand(string parameterList, string& resultString) 
{
	resultString += "error: not implemented\n";
	return resultString;
}

