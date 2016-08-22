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
#include "CDocument.h"
#include "CLIController.h"
#include <string>


using namespace std;

/* Use the macro OpenDoc for this class */
COMMAND_REGISTER(OpenDoc,"OpenDoc","Opens a document Specify a path relative to the\
 executable directory (for example, \"../ExampleFiles/AboutSnippetRunner.pdf\")", "Opens a document.")

void
OpenDocCommand::executeCommand(string path){
	if (path.length()==0) {
		Console::displayString("Specify a document name\n");
		return;
	}
#ifdef PDFL_SDK_SAMPLE
	if (!CDocument::OpenDocument(path.c_str())){
		Console::displayString("Error opening document \'" + path + "\'!\n");
	}
#else
	if (!CDocument::OpenDocument(reinterpret_cast<const ASUTF16Val *>(path.c_str()))){
		Console::displayString("Error opening document \'" + path + "\'!\n");
	}
#endif
	else {
		Console::displayString("Document " + path + " opened.\n");
	}
}

string OpenDocCommand::executeServerCommand(string path, string& resultString) 
{
	if (path.length()==0) {
		resultString += "error: Specify a document name\n";
		return resultString;
	}
#ifdef PDFL_SDK_SAMPLE
	if (!CDocument::OpenDocument(path.c_str())){
		Console::displayString("Error opening document \'" + path + "\'!\n");
	}
#else
	if (!CDocument::OpenDocument(reinterpret_cast<const ASUTF16Val *>(path.c_str()))){
		resultString += "error: Error opening document \'" + path + "\'!\n";
	}
#endif
	else {
		resultString += "Document " + path + " opened.\n";
	}
	return resultString;
}


