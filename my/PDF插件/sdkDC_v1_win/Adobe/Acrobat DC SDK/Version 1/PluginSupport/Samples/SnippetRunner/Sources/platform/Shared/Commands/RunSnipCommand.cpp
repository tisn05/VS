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
COMMAND_REGISTER(RunSnip,"Run","Executes a single snippet. The parameter you pass in will be compared against each snippet. \
						The first snippet that returns a match will be executed. Choose your pattern carefully.", "Runs a snippet")

static SnipRun * theSnippet = NULL;

void
RunSnipCommand::executeCommand(string matchstring){
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

//looks up snippet globally, runs first one, if found.
string
RunSnipCommand::executeServerCommand(string snippetUINameAndParams, string& resultString)
{
	//look for params should be 'snippetname-params' (no spaces around '-'!!), if any
	int index = snippetUINameAndParams.find_first_of('-');
	string snippetUIName, params = "";
	bool foundParams = false;
	if(index > 0)
	{
		snippetUIName = snippetUINameAndParams.substr(0, index);
		const char* temp1 = snippetUIName.c_str();
		int len = snippetUINameAndParams.length();
		params = snippetUINameAndParams.substr(index+1);
		const char* temp2 = params.c_str();
		foundParams = true;
		printf("running: %s with params: %s\n", temp1, temp2);
	}
	else
	{
		snippetUIName = snippetUINameAndParams;
		printf("running: %s with no params\n", snippetUIName.c_str());
		
	}
	
	SnipRun * snippet = FindSnippet(snippetUIName);
	
	if (snippet == NULL)
	{
		resultString += "error: No such snippet\n";
	}
	else 
	{
		if(foundParams)
			snippet->SetParams(params);
		SnipRun::SnipServerCallback(snippet);
		resultString += "snippet done\n";
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

