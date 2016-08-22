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
#include "ASExtraExpT.h"
#include <string>


using namespace std;

// forward declaration
ACCB1 ASBool ACCB2 myASCabEnumProc(ASCab theCab,const char* theKey, ASCabValueType itsType, void* clientData);

/* Use the macro helper for this class */
COMMAND_REGISTER(ListSnips,"ls", "Lists the contents of the current node.","Lists all snippets")

void
ListSnipsCommand::executeCommand(string){
	SnipManager * sm = SnipManager::Instance();
	if (sm == NULL){
		Console::displayString("The snippet manager is null, this is a very bad thing\n");
	}

	// the SnippetHierarchyUtils manages the set of snippets.
	SnippetHierarchyUtils * theSHU = SnippetHierarchyUtils::Instance();

	ASCab currentNode = theSHU->getCurrentCab();
	
	string currentPath = theSHU->getCurrentPath();
	Console::displayString("Directory : " + currentPath +"\n");
	string result;
	ASCabEnum(currentNode, myASCabEnumProc, &result);
	Console::displayString(result);
	Console::displayString("\n");
	Console::flush();
}

string ListSnipsCommand::executeServerCommand(string parameterList, string& resultString) 
{
	resultString += "error: not implemented\n";
	return resultString;
}

ACCB1 ASBool ACCB2 myASCabEnumProc(ASCab theCab,const char* theKey, ASCabValueType itsType, void* clientData){
	
	string* resultString = (string*)clientData;
	
	// each directory has a hidden element, its path
	if (strcmp(theKey,"SnippetPath")== 0){
		return true;
	}
/* If we ever are here, something grave is wrong....
	if ((itsType != kASValueUns) && (itsType != kASValueCabinet)){
		Console::displayString("Unknown type in snippet hierarchy!");
		return false;
	}*/
	string output(theKey);
	if (itsType != kASValueUns){
	output = string("[") + output + string("]");
	}
	else {
		output.append("*");
	}
	*resultString += output;
	*resultString += '\t';
	return true;
}

