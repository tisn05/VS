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

/* Use the macro RevertDoc for this class */
COMMAND_REGISTER(RevertDoc,"RevertDoc","Reverts the current document", "Revert the document")

void
RevertDocCommand::executeCommand(string){
	if (!CDocument::RevertDocument()){
			Console::displayString("Error reverting document!\n");
		}
		else {
			Console::displayString("Document reverted.\n");
		}
}

string RevertDocCommand::executeServerCommand(string parameterList, string& resultString) 
{
	if (!CDocument::RevertDocument()){
			resultString += "error: Error reverting document!\n";
		}
		else {
			resultString += "Document reverted.\n";
		}
	return resultString;
}


