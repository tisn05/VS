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

#include "ASExtraCalls.h"


using namespace std;

static ACCB1 ASBool ACCB2 myASCabEnumProc(ASCab theCab,const char* theKey, ASCabValueType itsType, void* clientData);

/* Use the macro helper for this class */
COMMAND_REGISTER(RunFullPathSnip,"RunFullPathSnip","Executes a single snippet. The parameter you pass in will be compared against each snippet. \
						The first snippet that returns a match will be executed. Choose your pattern carefully.", "Runs a snippet")

static SnipRun * theSnippet = NULL;

void
RunFullPathSnipCommand::executeCommand(string matchstring){
	// the SnippetHierarchyUtils manages the set of snippets.
	theSnippet = NULL;
	SnippetHierarchyUtils * theSHU = SnippetHierarchyUtils::Instance();


	ASCab currentNode = theSHU->getCurrentCab();
	if (matchstring.find("*")!= string::npos){
		string stringpart = matchstring.substr(0,matchstring.length()-1);		
		ASCabEnum(currentNode, myASCabEnumProc,(void *)stringpart.c_str());
	}
	else {
		theSnippet = (SnipRun *)ASCabGetUns(currentNode, matchstring.c_str(), 0);
	}
	if (theSnippet == NULL){
		Console::displayString("No such snippet\n");
	}
	else {
		SnipRun::SnipCallback(theSnippet);
	}
}

string
RunFullPathSnipCommand::executeWCommand(string matchstring, string resultString){
	ASInt32 numFound = 0;
	resultString = "";
	SnipManager * sm = SnipManager::Instance();
	if (sm == NULL){
		resultString += "error: SnipManger isn't found";
		return resultString;
	}
	// use this to cache the found snippet. If only one snippet is found, we can 
	SnipRun * tmpSnip = NULL;
	SnipRun * snippet = sm->getFirstSnippet();
	while (snippet!=NULL){
		string name = string(snippet->GetUIName());
		// found our snippet
		if (name.find(matchstring)!=string::npos){
			numFound++;
			resultString += name.c_str();
			break;
		}
		snippet = sm->getNextSnippet();
	}
	
	if (snippet == NULL){
		resultString += "No such snippet\n";
	}
	else {
		SnipRun::SnipCallback(snippet);
		resultString += " snippet done\n";
	}
	
	return resultString;
}


ACCB1 ASBool ACCB2 myASCabEnumProc(ASCab theCab,const char* theKey, ASCabValueType itsType, void* clientData){
	string name((char *)clientData);
	string keyName(theKey);
	if (keyName.find(name)==0){
		// we have found a matching snippet
		theSnippet = (SnipRun *)ASCabGetUns(theCab, theKey, 0);
		return false;
	}
	return true;
}

