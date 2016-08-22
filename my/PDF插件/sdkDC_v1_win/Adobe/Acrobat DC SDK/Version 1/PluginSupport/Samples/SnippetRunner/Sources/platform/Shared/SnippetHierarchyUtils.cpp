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

#include <string>
#include "PDExpT.h"
#include "PDCalls.h"
#include "SnippetHierarchyUtils.h"
#include "SnippetRunnerUtils.h"

const char * gSnipPathKey = "SnippetPath";
SnippetHierarchyUtils* SnippetHierarchyUtils::gSnippetHierarchyUtils  = NULL;

//used on Mac PDFL
typedef struct SnippetListElem
{
	SnipRun* snippet;
	SnippetListElem* next;
}SnippetListElem;

SnippetListElem* snippetList = NULL;
SnippetListElem* endSnippetList = NULL;

ASCab 
SnippetHierarchyUtils::addNode(ASCab fromThisNode, string thisPath, SnipRun * theSnippet){
	//if the fromThisNode is NULL, it will be because rootCab is null. 
	if (fromThisNode == NULL){
		if (rootCab!=NULL){
			Console::displayString("server: Illegal use of root cab.");
			return NULL;
		}
		// we are called before the library is intialised, potentially, 
		// we need to ensure the library is initialised. 
		// This call will initialise the lib only if that is required.
	#ifdef PDFL_SDK_SAMPLE
		SnippetRunnerUtils::initLibrary();
	#endif

		rootCab = ASCabNew();
		current = rootCab;
		
		ASCabPutText(rootCab, gSnipPathKey, ASTextFromScriptText("/",kASRomanScript));
		return this->addNode(rootCab,thisPath,theSnippet);
	}
	
	// we have a valid node, we should take off the front of the "thisPath" and compare it 
	// with any of the cabs contained in the node. If they match, we burrow down. If we do not find
	// a match, we create a new node and recurse.

	ASInt32 position = thisPath.find(":");
	// if there is no delimeter, we are left with the name of the snippet. 
	if (position == string::npos){
		// if the cab is known, just return null, indicating the snippet is a duplicate.
		if (ASCabKnown(fromThisNode, thisPath.c_str())){
			Console::displayString("server:Duplicate snippet addition.");
			return NULL;
		}
		// the cab is not known, we are dealing with the final part of the snippet. Add the snippet
//		ASCabPutPointer(fromThisNode, thisPath.c_str(), SnipRun, (void *)theSnippet, NULL);
		ASCabPutUns64(fromThisNode,thisPath.c_str(),(ASUns64)theSnippet);
		return fromThisNode;
	}
	
	// if we are here it means we have not got a terminal node, we create a cab in the hierarchy,
	// and recurse down the structure
	string head = thisPath.substr(0,position);
	string newTail = thisPath.substr(position+1,string::npos);
	// if the node already exists, just recurse
	if (ASCabKnown(fromThisNode,head.c_str())){
		return this->addNode(ASCabGetCab(fromThisNode, head.c_str()),newTail,theSnippet);
	}
	// the node doesn't exist, create a new cab and add it.
	ASCab tmpCab = ASCabNew();
	ASText parentPath = ASCabGetText(fromThisNode, gSnipPathKey);
	const char * tmp = ASTextGetScriptText(parentPath,kASRomanScript);
	ASText childPath = ASTextNew();
	ASTextCopy(childPath, parentPath);
	// only add the '/' if we are not adding to the reoot node.
	const char * tmp2 = ASTextGetScriptText(childPath,kASRomanScript);
	ASText pPath = ASTextFromScriptText("/",kASRomanScript);
	if(ASTextCmp(parentPath, pPath) != 0) {
		ASTextCat(childPath,pPath);
	}
	ASTextDestroy(pPath);
	ASText cPath = ASTextFromScriptText(head.c_str(),kASRomanScript);
	ASTextCat(childPath,cPath);
	ASTextDestroy(cPath);
	ASCabPutText(tmpCab, gSnipPathKey, childPath);
	ASCabPutCab(fromThisNode, head.c_str(), tmpCab);
	return this->addNode(tmpCab,newTail,theSnippet);
}

ASBool
SnippetHierarchyUtils::addSnippet(SnipRun * thisSnippet)
{
#if 0
// addSnippet() gets called before main().
// addNode() uses the library ASCab functions.
// For "old" single-threaded PDF Libraries, this worked.
// With multi-threaded ones, you have to call PDFLInit first.
	string snipName(thisSnippet->GetUIName());
	if (this->addNode(rootCab,snipName,thisSnippet)==NULL){
		return false;
	} else {
		return true;
	}
#else
	SnippetListElem* elem = new SnippetListElem;
	elem->snippet = thisSnippet;
	elem->next = NULL;
	if(!snippetList)
	{
		snippetList = endSnippetList = elem;
	}
	else
	{
		endSnippetList->next = elem;
		endSnippetList = elem;
	}
	return true;
#endif
}

void SnippetHierarchyUtils::addSnippetsFromList()
{
	SnippetListElem* current = snippetList;
	//add snippets from list, cleaning up as we go.
	while(current)
	{
		string snipName(current->snippet->GetUIName());
		this->addNode(rootCab,snipName,current->snippet);
		SnippetListElem* next = current->next;
		delete current;
		current = next;
	}

}

SnippetHierarchyUtils::SnippetHierarchyUtils(){
	rootCab = NULL;
	current = NULL;
}

SnippetHierarchyUtils::~SnippetHierarchyUtils(){
	ASCabDestroy(rootCab);
}

void
SnippetHierarchyUtils::deleteInstance(){
	if (gSnippetHierarchyUtils!=NULL){
		delete gSnippetHierarchyUtils;
		gSnippetHierarchyUtils = NULL;
	}
}

SnippetHierarchyUtils * 
SnippetHierarchyUtils::Instance()
{
	if (gSnippetHierarchyUtils == NULL)
	{
		gSnippetHierarchyUtils = new SnippetHierarchyUtils();
	}
	return gSnippetHierarchyUtils;
}

ASBool
SnippetHierarchyUtils::setCurrentPath(string toNewPath){

	// deal with empty strings, this resets the current to root
	if (toNewPath.length()==0){
		current=rootCab;
		return true;
	}
	// deal with absolute paths, string beginning with "/".
	if (toNewPath[0] == '/'){
		toNewPath=toNewPath.substr(1,string::npos);
		current = rootCab;
		// Handle setting as root
		if (toNewPath.length()==0){
			return true;
		}
	}
	// deal with relative paths now (at this point, all paths are relative).
	if (toNewPath.find("..",0)!= string::npos){
		ASText myPath = ASCabGetText(current,gSnipPathKey);
		string path(ASTextGetScriptText(myPath,kASRomanScript));
		int delPos = path.rfind("/");
		if (delPos == string::npos){
			// assert
			// this indicates a mal-formed path, return
			return false;
		}
		if (path.compare("/")==0) {
			// we are left with the root. just return
			return false;
		}
		path = path.substr(0,delPos);
		// reset the current path with the relative qualifier
		if (this->setCurrentPath(path)==false) {
			return false;
		}
		// reset the current path with the rest of the new path
		if (toNewPath.find("/")!=string::npos){
			toNewPath=toNewPath.substr(3,string::npos);
		} else {
			toNewPath=toNewPath.substr(2,string::npos);		
		}
			
		if (toNewPath.length()==0){
			return true;
		}
		return this->setCurrentPath(toNewPath);
	}

	int thePos = toNewPath.find("/");
	if (thePos!=string::npos){
		// take the head off. Head should be one of the members of current. Set current to this value.
		string head = toNewPath.substr(0,thePos);
		string rest = toNewPath.substr(thePos+1,string::npos);
		// we know this is not a leaf, so it is a cab we are after.
		ASCab newCurrent = ASCabGetCab(current, head.c_str());
		if (newCurrent == NULL){
			return false;
		}
		else {
			current = newCurrent;
		}
		return this->setCurrentPath(rest);
	}
	// now we deal with the final node.
	ASCab newCurrent = ASCabGetCab(current, toNewPath.c_str());
	if (newCurrent == NULL){
		return false;
	}
	else {
		current = newCurrent;
	}
	return true;
}

string 
SnippetHierarchyUtils::getCurrentPath(){
	ASText snipPath = ASCabGetText(current,gSnipPathKey);
	return string(ASTextGetScriptText(snipPath,kASRomanScript));
}

ASCab
SnippetHierarchyUtils::getCurrentCab(){
	return current;
}
