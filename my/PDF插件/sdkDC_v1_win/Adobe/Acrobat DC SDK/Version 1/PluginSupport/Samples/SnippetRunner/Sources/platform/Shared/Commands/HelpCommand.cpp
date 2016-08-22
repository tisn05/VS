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
#include "CLIController.h"
#include <string>

using namespace std;

static string helpText = "The SnippetRunner environment provides infrastructure to explore small \
units of code (code snippets) with the PDFL library. There is a peer platform that is delivered as part of the \
Acrobat SDK, and a lot of the snippets are common to both the Acrobat and PDFL SDKs.\n\n\
The snippets are arranged in a hierarchy, not unlike traditional file systems. The commands for navigating this \
hierarchy are also similar to those found to support file systems (cd, pwd, etc.). \n\n\
\"findsnip\" can be used to search for snippets matching a certain \
pattern. While the commands are case insensative, correct case must be used for snippets. If \"findsnip\" returns a single \
match, you can execute this snippet directly from anywhere in the hierarchy. \n\n\
The commands support a simple notion of wildcards. \
Specifying some sub-pattern (\"cd PD L*\" for example) will result in the command being applied to the entity that first matches \
the partial string.\n\n\
The environment has the concept of an open document. Snippets execute on this document and commands exist to open, save and close the\
document. If the document is modified, the prompt reflects this with an asterix. the \"DocName\" command gives the current state of \
the open document.\n";


/* Use the macro helper for this class */
COMMAND_REGISTER(Help,"Help", "\"Help\" for generic Help, \"Help commandname\" for help on a specific command","Provides help on commands")

void
HelpCommand::executeCommand(string param){
	CLIController *theController = CLIController::Instance();
	if (param.length() == 0){
		string helpList = theController->getCommandList();
		Console::displayString(helpList);
		Console::displayString("More? [y/n]");
		if (Console::getString().compare("y")==0){
				Console::displayString(helpText);
		}
	}
	else {
		ICommand * theCommand = theController->getCommand(param);
		if (theCommand == NULL){
			Console::displayString("No such command\n");
		}
		else {
			Console::displayString(theCommand->getDescription()+'\n');
		}
	}
	Console::flush();
}

string HelpCommand::executeServerCommand(string parameterList, string& resultString) 
{
	CLIController *theController = CLIController::Instance();
	if (parameterList.length() == 0){
		string helpList = theController->getCommandList();
		resultString += helpList;
		resultString += "\n\n";
		resultString += helpText;
	}
	else {
		ICommand * theCommand = theController->getCommand(parameterList);
		if (theCommand == NULL){
			resultString += "error: No such command\n";
		}
		else {
			resultString += theCommand->getDescription()+'\n';
		}
	}

	return resultString;
}

