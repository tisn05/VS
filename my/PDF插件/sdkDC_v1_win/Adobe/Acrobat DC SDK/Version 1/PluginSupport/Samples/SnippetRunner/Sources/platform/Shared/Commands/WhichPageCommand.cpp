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

/* Use the macro or this class */
COMMAND_REGISTER(WhichPage,"WhichPage","Displays the current page number, 0 is the first page.", "Outputs the current page number.")

void
WhichPageCommand::executeCommand(string){

	PDPage thePage = CDocument::GetPDPage();
	if (thePage == NULL){
		Console::displayString("Cannot get the current page, is there a document open?\n");
	}
	else {
		ASInt32 pageNum = PDPageGetNumber(thePage);
		char buff[56];
#ifdef MAC_PLATFORM
		snprintf(buff,sizeof(buff),"Current page number is %d\n",pageNum);
#else
		sprintf_s(buff,sizeof(buff),"Current page number is %d\n",pageNum);
#endif
		Console::displayString(buff);
	}
	Console::flush();
}


string WhichPageCommand::executeServerCommand(string parameterList, string& resultString) 
{
	PDPage thePage = CDocument::GetPDPage();
	if (thePage == NULL){
		resultString += "error: Cannot get the current page.\n";
	}
	else {
		ASInt32 pageNum = PDPageGetNumber(thePage);
		char buff[56];
#ifdef MAC_PLATFORM
		snprintf(buff,sizeof(buff),"Current page number is %d\n",pageNum);
#else
		sprintf_s(buff,sizeof(buff),"Current page number is %d\n",pageNum);
#endif
		resultString += buff;
	}
	return resultString;
}

