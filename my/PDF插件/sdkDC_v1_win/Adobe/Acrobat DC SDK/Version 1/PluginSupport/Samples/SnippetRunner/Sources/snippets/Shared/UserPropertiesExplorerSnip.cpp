/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 UserPropertiesExplorerSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static ACCB1 ASBool ACCB2 EnumUserPropProc(ASText, ASText, void*);
static ACCB1 ASBool ACCB2 EnumElemWithUserPropProc(PDSElement, PDSElement, void*);
static int rows;		  

/** 
	This snippet demonstrates the use of PDSEdit APIs to enumerate and
	obtain UserProperties attributes (aka Object Data) for PDS elements. 

	The UserProperties property/value pair is written to the debug window.

	<h3>Implementation</h3>
	<ul>
	<li> Tests whether the current open document has UserProperties</li>
	<li> If no, return and terminate</li>
	<li> If yes,
		<ul>
			<li> Enumerate all PDSElements that has UserProperties	</li>
			<li> For eacd PDSElement that has UserProperties attributes/classes, 
			displays the property name and value.	</li>
			<li> Enumeration continues until either:
			<ul>
				<li> the enumerator runs out of PDSElements containing UserProperties, or</li>
				<li> the number of display strings exceeds a predefined threshold (100), to
				prevent debug window overflow</li>
			</ul>
			</li>
		</ul>
	</li>
	</ul>
 
	@see PDDocHasUserProperties
	@see PDDocEnumAllPDSElementsWithUserProperties
	@see EnumElementsWithUsePropertiesProc
	@see PDSElementGetType
	@see PDSElementHasUserProperties
	@see PDSEnumUserPropertiesAsASText
	@see PDSElementEnumUserPropertiesAsASTextProc
	@see ASAtomGetString
	@see ASTextNew
	@see ASTextGetBestScript
	@see ASTextFromScriptText
	@see ASTextCatMany
*/
static void UserPropertiesExplorerSnip()
{
	rows = 0;
	CDocument doc;
	PDDoc pdDoc = (PDDoc) doc;
	if (!pdDoc) return;

	// 
	// Check the document for presence of UserProperties structure elements
	//

	if (!PDDocHasUserProperties(pdDoc))
	{
		AVAlertNote("This document does not contain UserProperties elements. Try another one.");
		return;
	}

	//
	// Enumerates the elements in the document's structure tree that have 
	// UserProperties attributes or classes,calling the supplied enumeration 
	// procedure for each such element found. 
	//

	EnumElementsWithUserPropertiesProc cbEnumElemUP = ASCallbackCreateProto(EnumElementsWithUserPropertiesProc, EnumElemWithUserPropProc);
	PDDocEnumPDSElementsWithUserProperties(pdDoc, cbEnumElemUP, NULL);

}

/**
	Callback for PDDocEnumAllPDSElementsWithUserProperties.

	This callback procedure is called by PDDocEnumAllPDSElementsWithUserProperties 
	once for each PDS element found that has UserProperties attributes or classes,
	
	The procedure returns true to continue enumeration, or false to halt enumeration.
*/
static ACCB1 ASBool ACCB2 EnumElemWithUserPropProc(PDSElement elem, PDSElement ancestorElem, void *clientData)
{
	// Make sure the PDSElement has UserProperties attributes

	if (!PDSElementHasUserProperties(elem))
	{
		Console::displayString("Something's wrong with the PDS element enumerator. Execution aborted.");
		return false;
	}

	// Get the name of the element type
	ASAtom subtype = PDSElementGetType(elem);
	char elemtype[128];

	sprintf(elemtype, "* %s", ASAtomGetString(subtype));
	Console::displayString(elemtype);

	// 	Enumerates the PDSElement's user properties

	PDSElementEnumUserPropertiesAsASTextProc cbEnumUP = ASCallbackCreateProto(PDSElementEnumUserPropertiesAsASTextProc, EnumUserPropProc);
	PDSElementEnumUserPropertiesAsASText(elem, cbEnumUP, NULL, false);

	if (++rows > 100) return false;
	return true;
}

/**
	Callback for PDSElementEnumUserPropertiesAsASText.

	Called by PDSElementEnumUserPropertiesAsASText for each entry in the 
	properties array. The enumeration proc receives the property information 
	as a pair of ASTexts, for the property name, and the property value. 
	
	The enumeration	continues as long as the callback returns true
*/
static ACCB1 ASBool ACCB2 EnumUserPropProc(ASText propName, ASText propVal, void *clientData)
{
	ASText property = ASTextNew();
	ASScript bestScript = ASTextGetBestScript(propName, kASRomanScript);
	ASText lBracket = ASTextFromScriptText("[", bestScript);
	ASText rBracket = ASTextFromScriptText("]", bestScript);
	ASText fSlash = ASTextFromScriptText(" / ", bestScript);

	if (ASTextIsEmpty(propVal))
		ASTextCatMany(property, lBracket, propName, rBracket, NULL);
	else
		ASTextCatMany(property, lBracket, propName, fSlash, propVal, rBracket, NULL);
	
	Console::displayString(property);
	//Console::displayString("\n");

	if (++rows > 100) return false;

	return true;
}

SNIPRUN_REGISTER(UserPropertiesExplorerSnip, "PDS Layer:Object Data Exploration", 
								 "This snippet enumerates and displays UserProperties of objects. Output is written to the debug window.")

