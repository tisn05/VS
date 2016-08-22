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
#ifndef _DocumentUtils_h_
#define _DocumentUtils_h_

#include <string>
#include "PDExpT.h"
#include "PDCalls.h"

using namespace std;

/** This class represents the relationship between SnippetRunner and PDF documents.
	Currently it maintains a single document. Access to this document is governed through
	this class. It is a singleton class. Future enhancements include the ability to have 
	a set of documents.
*/
class DocumentUtils {
protected:
	/** the ctor is protected to implement this class as a singleton */
	DocumentUtils();
public:
	/** a static factory method */
	static DocumentUtils * Instance();
	/** the delete instance method for tidy up */
	void deleteInstance();
	/** dtor */
	virtual ~DocumentUtils();
	/** open the document specified by the pathname
		@param pathname IN the path to the document we want to open
		@return an errorcode specifying the result of the open, 0 for success, -1 for failure
	*/
	int openDocument(string pathName);
	/** close the current document 
		@return an erorcode specifying the result of the close, 0 for success, -1 for failure
	*/
	int closeDocument();
	/** returns the document name
		@return the document's name
	*/
	string getDocumentName();
	/** saves the document. This will always perform a saveas, even when specifying the same document
		@param name IN name to save the document to
		@return an errorcode specifying the result of the save, 0 for success, -1 for failure
	*/
	int saveDocument(string toName);
	
	/** reverts the current document back to match the version on disk.
		@return -1 if the document could not be reverted
	*/
	int revertDocument();
	/** returns the current opened document, or NULL if there isn't a document.
		@return the current opened document, or NULL if no document is opened.
	*/
	PDDoc getDocument();
	
	/** returns the current PDPage, if none has been set, it defaults to page 0 of the open document
		@return the current page of the document, or NULL if there is no open document.
	*/
	PDPage getPage();
	
	/** sets the current page to the one specified and returns the associated PDPage object.
		@param pageNUM IN the page we want
		@return the required page object
	*/
	PDPage getPage(ASInt32 pageNum);
	
	/** sets the current PDPage in the document and returns the page. If the (zero based) page
		number is not valid, it returns NULL and does not change the currentpage.
		@param pageNumber IN the page number t set the current page to, first page is 0.
		@return the new current page, or NULL if the page number is invalid, or if there is no open doc.
	*/
	PDPage setPage(ASInt32 pageNumber);
private:
	/** the document we are watching */
	PDDoc theCurrentDoc;
	/** the current page, this is analogous to the page under the current view in Acrobat */
	PDPage theCurrentPage;
	/** a pointer to us, so we can be a singleton */
	static DocumentUtils* gDocumentUtils;
	/** we cache the pathname passed in for the revertdoc command */
	string myPathName;
	/** allows us to open a document based on a relative filename.
		@param fileName IN the file we wish to open
		@return the document if it exists,otherwise NULL
	*/
	PDDoc SRPDDocOpen(char *fileName);

};

#endif

