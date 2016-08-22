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
#ifndef _SnippetHierarchyUtils_h_
#define _SnippetHierarchyUtils_h_

#include <string>
#include "PDExpT.h"
#include "PDCalls.h"
#include "SnippetRunner.h"

using namespace std;

/** This class represents the relationship between the SnippetRunner platform and the snippets
	supported by the instance of the platform. It maintains the objects in a hierarchy of cab
	objects. A cab object either represents a CodeSnippet (leaf) or a set of named cab objects 
	(node). The abstraction maintains a concept of current node. This is basically a pointer 
	to a cab object in the structure. Methods to navigate the pointer around the structure are
	provided, along with the ability to query the nodes or leafs at the current pointer.
*/
class SnippetHierarchyUtils {
protected:
	/** the ctor is protected to implement this class as a singleton */
	SnippetHierarchyUtils();
public:
	/** a static factory method */
	static SnippetHierarchyUtils * Instance();
	/** the delete instance method for tidy up */
	static void deleteInstance();
	/** dtor */
	virtual ~SnippetHierarchyUtils();

	/** Adds a snippet to the hierarchy. On Mac, just stores snippet for adding later
		by calling addSnippetsFromList.
		@param thisSnippet IN the snippet to add to the hierarchy.
		@return true if the snippet is added, false otherwise. This method will return
			false if the snippet is already present or if there is a snippet name clash.
	*/
	ASBool addSnippet(SnipRun * thisSnippet);
	
	/** Adds a snippet to the hierarchy from the SnippetList. Only does anything on Mac.
	*/
	void addSnippetsFromList();
	
	
	/** Sets the current path to that passed in. If the path is invalid, it is reset
		to the root node.
		@param toNewPath IN the new current path
		@return true if the update is successful, false otherwise. If the update was 
				not sucessful, the current value is unsafe.
	*/
	ASBool setCurrentPath(string toNewPath);
	
	/** returns the current path as a string, this is the placement of the "current" node
		pointer within the hierarchy. 
		@return the path to the current node.
	*/
	string getCurrentPath();
	
	/** returns the current cab, this is the placement of the "current" node
		pointer within the hierarchy. 
		@return the cab for the current node.
	*/
	ASCab getCurrentCab();

	
private:
	ASCab rootCab;
	ASCab current;
	
	/** Adds the snippet represented by the path to the node. Called recursively to add each 
		node in the tree. If we are dealing with the final part of the path (i.e. the leaf)
		we create a cab and populate it with the snippet. If the key is already created, we 
		return NULL (this is an error). If the path already exists (i.e. there exists an 
		ASCab with a key of the string partial), we recurse down the path. If the path does
		not exist, a new cab is created and we recurse with this cab. Attempting to add a path
		to a node with a snippet should result in returning null. 
		@param fromThisNode IN the node to add the snippet to (relative to the path passed in 
		@param thisPath IN the path to add the snippet at. Called recursively, this parameter
		eventually is reduced to the final snippet name.
		@param theSnippet IN the snippet to add to the hierarchy
		@return the ASCab is the leaf node (i.e. the cab for the snippet. Null on failure.
	*/
	ASCab addNode(ASCab fromThisNode, string thisPath, SnipRun * theSnippet);

	/** a pointer to us, so we can be a singleton */
	static SnippetHierarchyUtils* gSnippetHierarchyUtils;


};

#endif
