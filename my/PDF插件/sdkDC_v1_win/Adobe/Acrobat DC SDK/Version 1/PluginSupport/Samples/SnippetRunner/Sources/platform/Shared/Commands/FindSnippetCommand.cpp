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
#include <string>


using namespace std;

/* Use the macro for this class */
COMMAND_REGISTER(FindSnippet,"FindSnip", "Displays snippets that contain the text passed in as a parameter. If only a single result is found\
 the user can execute the snippet, or find more infomation about the snippet regardless of the current directory the user is in.","Find a snippet")

void
FindSnippetCommand::executeCommand(string matchstring){
	ASInt32 numFound = 0;
	SnipManager * sm = SnipManager::Instance();
	if (sm == NULL){
		Console::displayString("The snippet manager is null, this is a very bad thing\n");
	}
	// use this to cache the found snippet. If only one snippet is found, we can 
	SnipRun * tmpSnip = NULL;
	SnipRun * snippet = sm->getFirstSnippet();
	while (snippet!=NULL){
		string name = string(snippet->GetUIName());
		// found our snippet
		if ((strcmp(matchstring.c_str(), "") == 0) || name.find(matchstring)!=string::npos){
			if(!numFound)
				Console::displayString("Snippets Found:\n");
			numFound++;
			char buff[256];
#ifdef MAC_PLATFORM
			snprintf(buff,sizeof(buff),"%d) %s\n",numFound, name.c_str());
#else
			sprintf_s(buff,sizeof(buff),"%d) %s\n",numFound, name.c_str());
#endif
			Console::displayString(buff);
			Console::flush();
			tmpSnip = snippet;
		}
		snippet = sm->getNextSnippet();
	}
	if(numFound == 0)
		Console::displayString("Snippet not found.\n");
	else
	{
		if (tmpSnip == NULL){
			Console::displayString("Found snippet reported as NULL!");
		}
		else {
			Console::displayString("Execute a snippet or show info? [y/i/n]");
			string resp = Console::getString();

			if (resp.compare(string("n")) == 0)
			{
				Console::displayString("Command cancelled.");
				return;
			}
			if(numFound > 1)
			{
				Console::displayString("Execute or get info for which snippet(choose number/other cancels)? ");
				string resp2 = Console::getString();
				int snippetNum = atoi(resp2.c_str());
				if(snippetNum > 0 && snippetNum <= numFound)
				{
					//find snippet chosen, and run it
					SnipRun * snippet = sm->getFirstSnippet();
					numFound = 0;
					while (snippet!=NULL)
					{
						string name = string(snippet->GetUIName());
						// found our snippet
						if (name.find(matchstring)!=string::npos)
						{
							numFound++;
							if(numFound == snippetNum)
							{
								tmpSnip = snippet;
								break;
							}
						}
						snippet = sm->getNextSnippet();
					}
				}
			}

			if(tmpSnip != NULL)
			{
				if (resp.compare(string("y")) == 0) {
					SnipRun::SnipCallback(tmpSnip);
				}
				if (resp.compare(string("i")) == 0) {
					Console::displayString(tmpSnip->GetDescription());
					Console::displayString("\n");
				}
			}
		}
	}

	Console::flush();
}


string
FindSnippetCommand::executeServerCommand(string matchstring, string& resultString){
	ASInt32 numFound = 0;
	resultString = "";
	SnipManager * sm = SnipManager::Instance();

	// use this to cache the found snippet. If only one snippet is found, we can 
	SnipRun * tmpSnip = NULL;
	SnipRun * snippet = sm->getFirstSnippet();
	while (snippet!=NULL){
		string name = string(snippet->GetUIName());
		// found our snippet
		if ((strcmp(matchstring.c_str(), "") == 0) || name.find(matchstring)!=string::npos){
			numFound++;
			resultString += name.c_str();
			resultString += "\n";
			tmpSnip = snippet;
		}
		snippet = sm->getNextSnippet();
	}

	if(numFound == 0)
		resultString += "error: No snippets found\n";
	
	return resultString;
}

