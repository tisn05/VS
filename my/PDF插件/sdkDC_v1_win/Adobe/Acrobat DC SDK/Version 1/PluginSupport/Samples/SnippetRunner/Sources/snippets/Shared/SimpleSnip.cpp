/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 SimpleSnip.cpp

	Optional Comment here
*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/**  This plug-in is the framework for a snippet. 

	<h3>Simple rules</h3>
	<ul>
	<li>	One registered snippet per source file </li>
	<li>	File name needs to end in 'Snip.cpp'</li>
	<li>	File name and entry point should be the same</li>
	<li>	This comment should immediately preceed the entry point</li>
	<li>	All relevant API routines should be documented using the @see comment.</li>
	</ul>

	<h3>Details</h3>
	<h4>Setting Context</h4>
	A snippet should focus on a particular set of APIs. If the APIs require
	some context (an open document, an active selection, etc.) the snippet can
	assume the user has set up this context prior to the snippet's execution. 
	The code that obtains this context should live in the SnippetRunnerUtils class.
	For example, if you need an active selection, you should write/use a function within
	SnippetRunnerUtils. This function should already make use of other functions in the 
	class - such as	SnippetRunnerUtils:getFrontPDDoc(). The utility will alert the user
	the condition is not met, and return NULL. Your snippet should check for NULL but
	not alert the user.

	<h4>Feedback to the User</h4>
	A general reporting mechanism is Console::displayString().
	For failures that the user can fix (i.e. no open document, or wrong document open) or when you are posting errors, use AVAlertNote.

	<h4>Synchronous Snippets</h4>
	Synchronous ("one-shot") snippets are executed, perform some action then the 
	logical thread of execution for that snippet is finished. They generally have a single
	entry point. These snippets are the most common and require no special considerations.

	<h4>Asynchronous Snippets</h4>
	Asynchronous ("multi-shot") snippets are identified by more than a single entry point. 
	Snippet that demonstrate callbacks or notifications would fall into this category. The 
	snippet is unable to determine when the logical thread of execution is complete. 
	Boolean state of snippets is controlled through the SnippetRunnerUtils class - the 
	toggleSnippetCheck() can be used to both determine the current state, and alter the UI
	to indicate wether the snippet is active. Note, this API is only guaranteed to work in the 
	primary entry function for the snippet. If you need to manipulate the state of a snippet 
	within some asynchronous function, you should cache the snippet locally from within the 
	main entry function.

	<h4>Parameters</h4>
	Using the SNIPRUN_REGISTER_WITH_DIALOG macro, a generic mechanism for passing parameters into
	a snippet is available. Before the snippet executes, a dialog requests the parameters from the user.
	The paramater management class is responsible for marshalling the types. The snippet is passed this
	class and can extract the parameters with calls such as getNextParamAsFixed. If you need more types
	than are available, you should enhance the class.
	
	Some snippets have a toggle behavour (execute to turn it on/off). You can control the use of the
	parameters dialog through the SnippetRunnerUtils::turnParamDialogOn/Off or 
	SnippetRunnerUtils::toggleParamDialog calls.

	<h4>Sample files</h4>
	If you have a snippet that needs an example file the file should be placed into the examplefiles 
	folder found under the SnippetRunner parent folder. To use these files within the snippet, you 
	should make use of the SnippetRunnerUtils::openSnippetExampleAVDoc or 
	SnippetRunnerUtils::openSnippetExampleASFile APIs. These APIs depend on the user setting the 
	location of the SnippetRunner parent folder. This preference persists in the application's plug-ins
	directory
 
 	@testfile none
	@see SnippetRunnerUtils for more information on the utility APIs
	@see ParamManager for more details on how to pass parameters to a snippet
*/
void SimpleSnip()
{
	Console::displayString("This is a simple snippet.");
	Console::displayString("Simple snippet executed\n");
}

SNIPRUN_REGISTER(SimpleSnip, "Simple Framework", "SimpleSnip creates a framework for a snippet.")

