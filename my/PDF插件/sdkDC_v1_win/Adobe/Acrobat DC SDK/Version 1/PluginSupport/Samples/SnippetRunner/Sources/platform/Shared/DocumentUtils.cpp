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

#include "ASCalls.h"

#include "DocumentUtils.h"
#include "Console.h"
#if MAC_PLATFORM
#ifdef PDFL_SDK_SAMPLE
#include "macutils.h"
#endif
#endif

#ifdef PDFL_SDK_SAMPLE
#include "MyPDFLibUtils.h"
#include "CLIController.h"
#else
#include "AVCalls.h"
#endif

#ifdef MAC_PLATFORM
#ifdef ACRO_SDK_PLUGIN_SAMPLE
#define UTILSNOTAVAIL 1
#endif
#endif

/*
	Factory methods. Single instance used.
*/
DocumentUtils* DocumentUtils::gDocumentUtils  = NULL;

/*
	maintain this class as a singleton
*/
DocumentUtils* 
DocumentUtils::Instance()
{
	if (gDocumentUtils == NULL)
	{
		gDocumentUtils = new DocumentUtils();
	}
	return gDocumentUtils;
}


/*
	We delete the controller, making sure we delete all the commands it maintains.
*/
void 
DocumentUtils::deleteInstance()
{
	if (gDocumentUtils->theCurrentDoc!=NULL) {
		this->closeDocument();
	}
	if (gDocumentUtils != NULL)
		delete gDocumentUtils;
	gDocumentUtils = NULL;
}

/*
	ctor
*/
DocumentUtils::DocumentUtils()
{
	theCurrentPage = NULL;
	theCurrentDoc = NULL;
}

/*
	dtor just deletes the instance, if it exists (though, it should exist)
*/
DocumentUtils::~DocumentUtils()
{
	if (gDocumentUtils != NULL){
		DocumentUtils::deleteInstance();
	}
}

int
DocumentUtils::openDocument(string path){
	if (this->theCurrentDoc!=NULL){
		ASInt32 docFlags = PDDocGetFlags(theCurrentDoc);
		// A regular pdf document cannot be saved in Reader
#if !READER_PLUGIN
		if ((docFlags&PDDocNeedsSave)!=0) {
			Console::displayString("Save current document? [y/n]");
			string resp = Console::getString();
			if (resp.compare(string("y")) == 0) {
				Console::displayString("Enter path for file (or return to replace existing file): ");
				string newpath = Console::getString();
				if (newpath.length()>0){
					this->saveDocument(newpath);
				}
				else {
					PDDocSave(theCurrentDoc,PDSaveIncremental,NULL, NULL,NULL, NULL);
				}
			}
		}
#endif

		if (this->closeDocument()!=0){
			return -1;
		}
	}
	char * myFile = static_cast<char *>(ASmalloc(path.length()+1));
#ifdef MAC_PLATFORM
	strlcpy(myFile,path.c_str(), sizeof(myFile));
#else
	strcpy_s(myFile,sizeof(myFile),path.c_str());
#endif
	if ((this->theCurrentDoc=this->SRPDDocOpen(myFile))==NULL){
		ASfree(myFile);
		myFile = NULL;
		return -1;
	}
	ASfree(myFile);
	myFile = NULL;
	this->myPathName = path;
	return 0;
}


PDDoc 
DocumentUtils::SRPDDocOpen(char *fileName)
{
#ifdef UTILSNOTAVAIL
	Console::displayString("server:SRPDDocOpen not implemented for exchange on the mac!");
	return NULL;
#else
	ASFileSys asFileSys = ASGetDefaultFileSys();

	#if MAC_PLATFORM
	FSSpec fsSpec;
	GetOneSampleFile_Mac(fileName, &fsSpec);
	#endif

	volatile ASPathName asPathName	= NULL;
	volatile PDDoc		pdDoc		= NULL;

	DURING
		/* Create asPathName from file.*/

		#if MAC_PLATFORM
		asPathName = GetMacPath(fileName);
		#else
		asPathName = ASFileSysCreatePathName(asFileSys, ASAtomFromString("Cstring"), fileName, 0);
		#endif

		/* Open pdDoc from asPathName.*/
		pdDoc = PDDocOpen(asPathName, NULL, NULL, true);
		fprintf(stdout, "Successfully opened %s\n", fileName);

#ifdef ACRO_SDK_PLUGIN_SAMPLE
		AVDocOpenFromPDDoc(pdDoc,NULL);
#endif

	HANDLER
		fprintf(stderr, "Unable to open %s\n", fileName);
		pdDoc = NULL;
	END_HANDLER

	/* Release asPathName.*/
	if (asPathName)
		ASFileSysReleasePath(asFileSys, asPathName);

	return pdDoc;
#endif // Acrobat/mac
}

int
DocumentUtils::revertDocument(){
	if (this->theCurrentDoc == NULL){
		Console::displayString("No document currently open");
		return -1;
	}
	string path =this->myPathName;
	PDDocClose(this->theCurrentDoc);
	this->theCurrentDoc=NULL;
	return DocumentUtils::openDocument(path);
}

int
DocumentUtils::closeDocument(){
   // Close the PDF file 
	if (this->theCurrentDoc == NULL){
		return -1;
	}
	
	ASInt32 docFlags = PDDocGetFlags(theCurrentDoc);
	// A regular pdf document can not be saved in Reader
#if !READER_PLUGIN
	if ((docFlags&PDDocNeedsSave)!=0) {
		Console::displayString("Save current document? [y/n]");
		string resp = Console::getString();
		if (resp.compare(string("y")) == 0) {
			Console::displayString("Enter path for file (or return to replace existing file): ");
			string newpath = Console::getString();
			if (newpath.length()>0){
				this->saveDocument(newpath);
			}
			else {
				PDDocSave(theCurrentDoc,PDSaveIncremental,NULL, NULL,NULL, NULL);
			}
		}
	}
#endif

	PDDocClose(this->theCurrentDoc);
	this->theCurrentDoc = NULL;
	this->theCurrentPage = NULL;
	this->myPathName = string("");
	return 0;
}

// A regular pdf document could not be saved in Reader
#if !READER_PLUGIN
int 
DocumentUtils::saveDocument(string path){
#ifdef UTILSNOTAVAIL
	Console::displayString("server:DocumentUtils::saveDocument not implemented for exchange on mac");
	return -1;
#else
	volatile ASPathName fPath;
	if (this->theCurrentDoc == NULL){
		return -1;
	}
	int error_code = 0;
	DURING
#if MAC_PLATFORM
	char * myFile = static_cast<char *>(ASmalloc(path.length()+1));
	strlcpy(myFile,path.c_str(), sizeof(myFile));

	fPath = GetMacPath(myFile);
	ASfree(myFile);
	myFile = NULL;
#else
	fPath = ASFileSysCreatePathName(NULL, ASAtomFromString("Cstring"), path.c_str(), 0);
#endif
		PDDocSave(this->theCurrentDoc, PDSaveFull|PDSaveLinearized, fPath, NULL, NULL, NULL);
	HANDLER
		error_code = -1;
	END_HANDLER
	return error_code;
#endif // utils available
}
#endif

string
DocumentUtils::getDocumentName(){
	if (this->theCurrentDoc==NULL){
		return string("");
	}
	char buffer[50];
	PDDocGetInfo(this->theCurrentDoc,"Title",buffer,49);
	return string(buffer);	
}

PDDoc 
DocumentUtils::getDocument(){
	return this->theCurrentDoc;
}

PDPage
DocumentUtils::getPage(){
	if (theCurrentPage==NULL){
		DocumentUtils::setPage(0);
	}
	return theCurrentPage;
}

PDPage
DocumentUtils::getPage(ASInt32 pageNum){
	DocumentUtils::setPage(pageNum);
	return theCurrentPage;
}


PDPage
DocumentUtils::setPage(ASInt32 pageNumber){
	if (theCurrentDoc == NULL){
		return NULL;
	}
	volatile PDPage tmpPage = NULL;
	ASBool invalidPage = false;
	DURING
		tmpPage = PDDocAcquirePage(theCurrentDoc, pageNumber);	
	HANDLER
		invalidPage = true;
	END_HANDLER		
	if (invalidPage == false){
		if (theCurrentPage != NULL){
			PDPageRelease(theCurrentPage);			
		}
		theCurrentPage = tmpPage;
	}
	return theCurrentPage;
}



