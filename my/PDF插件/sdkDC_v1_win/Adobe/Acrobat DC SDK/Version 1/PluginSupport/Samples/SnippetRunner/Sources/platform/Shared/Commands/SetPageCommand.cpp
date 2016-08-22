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
COMMAND_REGISTER(SetPage,"SetPage","Sets the current page of the current document, 0 for the first page.", "Sets the current page of the current document")

void
SetPageCommand::executeCommand(string pagenum){
	if (pagenum.length()==0) {
		Console::displayString("Specify a document page number\n");
		return;
	}
	ASInt32 pageNum;
#ifdef MAC_PLATFORM
	sscanf(pagenum.c_str(),"%d",&pageNum);
#else
	sscanf_s(pagenum.c_str(),"%d",&pageNum);
#endif
	if (!CDocument::SetCurrentPage(pageNum)){
		Console::displayString("Cannot set the current page, is there a document open?\n");
	}
	else {
		Console::displayString("page " + pagenum + " set.\n");
	}
}

string SetPageCommand::executeServerCommand(string pagenum, string& resultString) 
{
	if (pagenum.length()==0) {
		resultString += "error: Specify a document page number\n";
		return resultString;
	}
	ASInt32 pageNum;
#ifdef MAC_PLATFORM
	sscanf(pagenum.c_str(),"%d",&pageNum);
#else
	sscanf_s(pagenum.c_str(),"%d",&pageNum);
#endif
	if (!CDocument::SetCurrentPage(pageNum)){
		resultString += "error: Cannot set the current page.\n";
	}
	else {
		resultString += "page " + pagenum + " set.\n";
	}
	return resultString;
}

