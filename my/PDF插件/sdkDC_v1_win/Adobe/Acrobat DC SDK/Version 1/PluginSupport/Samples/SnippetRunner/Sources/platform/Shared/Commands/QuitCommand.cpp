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
#include <string>


using namespace std;

/* Use the macro helper for this class */
COMMAND_REGISTER(Quit,"Quit","Quits the application","Quits the application")

void
QuitCommand::executeCommand(string){
	string tmp = "Quitting application\n";
	Console::displayString(tmp);
}


string QuitCommand::executeServerCommand(string parameterList, string& resultString) 
{
	resultString = "Quitting application\n";
	return resultString;
}

