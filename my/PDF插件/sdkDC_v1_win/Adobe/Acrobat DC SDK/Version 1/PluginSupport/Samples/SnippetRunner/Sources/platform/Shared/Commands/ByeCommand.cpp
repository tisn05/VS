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
COMMAND_REGISTER(Bye,"Bye","Signals client is closing connection","Signals client is closing connection")

void
ByeCommand::executeCommand(string){
	string tmp = "bye\n";
	Console::displayString(tmp);
}


string ByeCommand::executeServerCommand(string parameterList, string& resultString) 
{
	resultString = "bye\n";
	return resultString;
}

