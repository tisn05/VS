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

/* Use the macro CloseDocer for this class */
COMMAND_REGISTER(CloseDoc,"CloseDoc","Closes the current document", "Closes the current document")

void
CloseDocCommand::executeCommand(string path)
{
	if (!CDocument::CloseDocument())
	{
		Console::displayString("Error closing document! Is a document open?\n");
	}
	else {
		Console::displayString("Document closed.\n");
	}

}

string
CloseDocCommand::executeServerCommand(string path, string& resultString)
{
	resultString = "";
	if (!CDocument::CloseDocument())
	{
		resultString += "error: Error closing document!\n";
	}
	else {
		resultString += "Document closed.\n";
	}
	return resultString;
}

