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
COMMAND_REGISTER(GetSnipName,"SnipName","Provides information on the snippet specified. Usage \"snipinfo snippetname\"", "Get snippet info.")

void
GetSnipNameCommand::executeCommand(string matchstring)
{	
	// the SnippetHierarchyUtils manages the set of snippets.
	SnippetHierarchyUtils * theSHU = SnippetHierarchyUtils::Instance();

	ASCab currentNode = theSHU->getCurrentCab();

	SnipRun * theSnippet = (SnipRun *)ASCabGetUns(currentNode, matchstring.c_str(), 0);
	if (theSnippet == NULL){
		Console::displayString("No such snippet\n");
	}
	else {
		 const char * snipName = theSnippet->GetSnippetName();
		 Console::displayString(snipName);
		 Console::displayString("\n");
	}
}

string GetSnipNameCommand::executeServerCommand(string snippetUIName, string& resultString) 
{
	SnipRun * snippet = FindSnippet(snippetUIName);
	
	if (snippet == NULL){
		resultString += "No such snippet\n";
	}
	else {
		resultString += snippet->GetSnippetName();
		resultString += '\n';
	}
	
	return resultString;
}

