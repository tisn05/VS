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
#include "Console.h"
#include "ICommand.h"
#include "SnipManager.h"
#include "SnippetRunner.h"
#include "SnippetHierarchyUtils.h"
#include "CLIController.h"
#include <string>


using namespace std;

/* Use the macro helper for this class */
COMMAND_REGISTER(ChangeDir,"cd","Allows navigation around the snippet hierarchy.", "Changes directory")

static string newPath;

static ACCB1 ASBool ACCB2 myASCabEnumProc(ASCab theCab,const char* theKey, ASCabValueType itsType, void* clientData);


void
ChangeDirCommand::executeCommand(string pathIN){
	newPath = string("");

	// the SnippetHierarchyUtils manages the set of snippets.
	SnippetHierarchyUtils * theSHU = SnippetHierarchyUtils::Instance();

	ASCab currentNode = theSHU->getCurrentCab();

	if (pathIN.find("*")!= string::npos){
		string stringpart = pathIN.substr(0,pathIN.length()-1);		
		ASCabEnum(currentNode, myASCabEnumProc,(void *)stringpart.c_str());
	}
	else {
		newPath = pathIN;
	}
	if ((newPath.length()==0)||(theSHU->setCurrentPath(newPath)	== false)) {
		Console::displayString("Error in path specification\n");
	}
	CLIController * theController = CLIController::Instance();
	theController->setCommandPrompt(theSHU->getCurrentPath()+"> ");

}

string
ChangeDirCommand::executeServerCommand(string pathIN, string& resultString){
	resultString += "error: not implemented\n";
	return resultString;
}



ACCB1 ASBool ACCB2 myASCabEnumProc(ASCab theCab,const char* theKey, ASCabValueType itsType, void* clientData){
	string name((char *)clientData);
	string keyName(theKey);
	if (keyName.find(name)==0){
		newPath = string(theKey);
		return false;
	}
	return true;
}

