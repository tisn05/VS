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
COMMAND_REGISTER(GetSnipParams,"SnipParams","If the snippet requires parameters, returns default parameters, else an empty string.", "Get snippet parameter info.")

void
GetSnipParamsCommand::executeCommand(string matchstring)
{	
	// the SnippetHierarchyUtils manages the set of snippets.
	SnippetHierarchyUtils * theSHU = SnippetHierarchyUtils::Instance();

	ASCab currentNode = theSHU->getCurrentCab();

	SnipRun * theSnippet = (SnipRun *)ASCabGetUns(currentNode, matchstring.c_str(), 0);
	if (theSnippet == NULL){
		Console::displayString("No such snippet\n");
	}
	else {
		string defaultParams = theSnippet->GetDefaultParams();
		 const char * snipInfo = defaultParams.c_str();
		 Console::displayString(snipInfo);
		 Console::displayString("\n");
	}
}

string GetSnipParamsCommand::executeServerCommand(string snippetUIName, string& resultString) 
{
	SnipRun * theSnippet = FindSnippet(snippetUIName);
	

	if (theSnippet == NULL){
		resultString += "error: No such snippet\n";
	}
	else {
		  resultString += theSnippet->GetDefaultParams();
		 resultString += "\n";
	}
	return resultString;
}

