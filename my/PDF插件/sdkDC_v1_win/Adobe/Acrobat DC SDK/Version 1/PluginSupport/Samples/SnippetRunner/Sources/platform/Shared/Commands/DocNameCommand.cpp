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

/* Use the macro DocNameer for this class */
COMMAND_REGISTER(DocName,"DocName", "Displays the current document title maintained in the document properties dictionary. Itemizes the flags that are associated with the document.","Displays the current document name and document flags.")

void
DocNameCommand::executeCommand(string){

	PDDoc pdDoc = CDocument::GetPDDoc();
	if (!pdDoc)
	{
		Console::displayString("No document title found, is there a document open?\n");
	}	
	else
	{
		char buffer[50];

		int titleLength = PDDocGetInfo(pdDoc,"Title",buffer,49);
		if(titleLength)
		{
			string docName(buffer);
			Console::displayString(docName+'\n');			
		}
		else
			Console::displayString("No document title.\n");
		
		ASInt32 docFlags = PDDocGetFlags(pdDoc);
		if ((docFlags&PDDocNeedsSave)!=0) {
			Console::displayString("Document contents dirtied - require save.\n");
		}
		if ((docFlags&PDDocRequiresFullSave)!=0) {
			Console::displayString("Cannot save document incrementally.\n");
		}
		if ((docFlags&PDDocIsModified)!=0) {
			Console::displayString("Document has had non-content change.\n");
		}
		if ((docFlags&PDDocDeleteOnClose)!=0) {
			Console::displayString("Temporary document.\n");
		}
		if ((docFlags&PDDocWasRepaired)!=0) {
			Console::displayString("Document had error (fixed) on opening.\n");
		}
		if ((docFlags&PDDocNewMajorVersion)!=0) {
			Console::displayString("Major version is newer than current.\n");
		}
		if ((docFlags&PDDocNewMinorVersion)!=0) {
			Console::displayString("Minor version newer than current.\n");
		}
		if ((docFlags&PDDocOldVersion)!=0) {
			Console::displayString("Document's version is older than current.\n");
		}
		if ((docFlags&PDDocSuppressErrors)!=0) {
			Console::displayString("Document errors suppressed.\n");
		}
		if ((docFlags&PDDocIsEmbedded)!=0) {
			Console::displayString("Document embedded in compound document.\n");
		}
		if ((docFlags&PDDocIsLinearized)!=0) {
			Console::displayString("Document linearized.\n");
		}
		if ((docFlags&PDDocIsOptimized)!=0) {
			Console::displayString("Document optimized.\n");	
		}	
	}
}

string
DocNameCommand::executeServerCommand(string notUsed, string& resultString){

	resultString = "";
	PDDoc pdDoc = CDocument::GetPDDoc();
	if (!pdDoc)
	{
		resultString += "error: No document title found, is there a document open?\n";
	}	
	else
	{
		char buffer[50];

		int titleLength = PDDocGetInfo(pdDoc,"Title",buffer,49);
		if(titleLength)
		{
			string docName(buffer);
			resultString += docName+'\n';			
		}
		else
			resultString += "No document title.\n";
		
		ASInt32 docFlags = PDDocGetFlags(pdDoc);
		if ((docFlags&PDDocNeedsSave)!=0) {
			resultString += "Document contents dirtied - require save.\n";
		}
		if ((docFlags&PDDocRequiresFullSave)!=0) {
			resultString += "Cannot save document incrementally.\n";
		}
		if ((docFlags&PDDocIsModified)!=0) {
			resultString += "Document has had non-content change.\n";
		}
		if ((docFlags&PDDocDeleteOnClose)!=0) {
			resultString += "Temporary document.\n";
		}
		if ((docFlags&PDDocWasRepaired)!=0) {
			resultString += "Document had error (fixed) on opening.\n";
		}
		if ((docFlags&PDDocNewMajorVersion)!=0) {
			resultString += "Major version is newer than current.\n";
		}
		if ((docFlags&PDDocNewMinorVersion)!=0) {
			resultString += "Minor version newer than current.\n";
		}
		if ((docFlags&PDDocOldVersion)!=0) {
			resultString += "Document's version is older than current.\n";
		}
		if ((docFlags&PDDocSuppressErrors)!=0) {
			resultString += "Document errors suppressed.\n";
		}
		if ((docFlags&PDDocIsEmbedded)!=0) {
			resultString += "Document embedded in compound document.\n";
		}
		if ((docFlags&PDDocIsLinearized)!=0) {
			resultString += "Document linearized.\n";
		}
		if ((docFlags&PDDocIsOptimized)!=0) {
			resultString += "Document optimized.\n";	
		}	
	}
	
	return resultString;
}


