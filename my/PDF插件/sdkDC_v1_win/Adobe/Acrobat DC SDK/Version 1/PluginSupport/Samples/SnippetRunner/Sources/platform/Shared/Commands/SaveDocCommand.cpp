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
#include <string>


using namespace std;

/* Use the macro SaveDocer for this class */
COMMAND_REGISTER(SaveDoc,"SaveDoc", "Saves the current document to the path specified on input, relative to the executable directory.","Saves the current document")

void
SaveDocCommand::executeCommand(string path){
	if (path.length()==0) {
		Console::displayString("Need to specify a document name!\n");
	}
	else {
#ifdef PDFL_SDK_SAMPLE
		if (!CDocument::SaveDocument(path.c_str())){
			Console::displayString("Error saving document!\n");
		}
#else
		if (!CDocument::SaveDocument(reinterpret_cast<const ASUTF16Val*>(path.c_str()))){
			Console::displayString("Error saving document!\n");
		}
#endif
		else {
			Console::displayString("Document saved.\n");
		}
	}
	Console::flush();
}

string SaveDocCommand::executeServerCommand(string path, string& resultString) 
{
	if (path.length()==0) {
		resultString += "error: Need to specify a document name!\n";
	}
	else {

#ifdef PDFL_SDK_SAMPLE
		if (!CDocument::SaveDocument(path.c_str())){
			Console::displayString("Error saving document!\n");
		}
#else
		if (!CDocument::SaveDocument(reinterpret_cast<const ASUTF16Val*>(path.c_str()))){
			resultString += "error: Error saving document!\n";
		}
#endif
		else {
			resultString += "Document saved.\n";
		}
	}
	return resultString;
}


