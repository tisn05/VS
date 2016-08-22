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

 AVAppPrefsSnip.cpp


*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/**  	This snippet demonstrates getting and setting viewer preferences for 
		Full Screen mode, specifically avpFullScreenEscape, avpFullScreenLoop 
		and avpFullScreenTransitionType 
		
		 
	@see AVAppGetPreference 
	@see AVAppSetPreference
	
*/
/*
		It specifically gets and sets preferences regarding Full Screen mode.
		It toggles:
			avpFullScreenEscape =	true/false to use Escape key to exit 
									Full Screen mode
			-- and --
			avpFullScreenLoop =		true/false to loop back to beginning of document.  

		It reports the old transition preference and, if it is not GlitterDown, 
		sets it to "GlitterDown".
		If transition preference is already GlitterDown, it updates it to "WipeLeft", 
		so change will always occur.
			avpFullScreenTransitionType  
*/

void AVAppPrefsSnip()
{
	char str[100]; //debug message length


	//if already in full screen mode, pop out
	if(AVAppDoingFullScreen())
		AVAppEndFullScreen();

	// This snip wants an open document, otherwise quit
	CDocument document;
	AVDoc avDoc = (AVDoc)document;
	if(avDoc == NULL)
		return;

	// collect current Full Screen preferences for update
ASBool escapePref = ASPtrToBool(AVAppGetPreference(avpFullScreenEscape));
ASBool loopPref = ASPtrToBool(AVAppGetPreference(avpFullScreenLoop));

	char *transitionPref = (char *)AVAppGetPreference(avpFullScreenTransitionType);

	//report old preferences and update prefs
	Console::displayString("\nFullScreen Settings:\n");

	if(escapePref){
		Console::displayString("Hit Escape to Exit was on, will be turned OFF (use ctrl-L).\n");
		AVAppSetPreference(avpFullScreenEscape, (void *)false); // if it was on, turn it off
	}
	else {
		Console::displayString("Hit Escape to Exit was off, will be turned ON.\n");
		AVAppSetPreference(avpFullScreenEscape, (void *)true); //if it was off, turn it on
	}

	if(loopPref) {
		Console::displayString("Document Looping was on, will be turned OFF.\n");
		AVAppSetPreference(avpFullScreenLoop, (void *)false); //i fit was on, turn it off
	}
	else {
		Console::displayString("Document Looping was off, will be turned ON.\n");
		AVAppSetPreference(avpFullScreenLoop, (void *)true); //if it was off, turn it on
	}

	//updated transitionPref string for special case of No Transition
	if(strlen(transitionPref)==0)
		transitionPref = "No Transition"; //app uses blank for no transition, update for the debug string

	Console::displayString(transitionPref);

	//if transition is GlitterDown, set it to WipeLeft
	if (strcmp (transitionPref, "GlitterDown") == 0) {
		AVAppSetPreference(avpFullScreenTransitionType, (void *)"WipeLeft"); 
		sprintf(str, "The default transition was %s.\n Transition changed to WipeLeft.\n",transitionPref);
		Console::displayString(str);
	}
	//else set it to GlitterDown
	else {			
		AVAppSetPreference(avpFullScreenTransitionType, (void *)"GlitterDown");
		sprintf(str, "The default transition was %s.\n Transition changed to GlitterDown.\n",transitionPref);
		Console::displayString(str);
	}

	//Start full screen mode
	PDColorValue myColorPtr = NULL;
	AVAppBeginFullScreen(myColorPtr);

	}

SNIPRUN_REGISTER(AVAppPrefsSnip, "AV Layer:Update App Prefs for Full Screen mode", "Demonstrates getting and setting application preferences for Full Screen mode.")

