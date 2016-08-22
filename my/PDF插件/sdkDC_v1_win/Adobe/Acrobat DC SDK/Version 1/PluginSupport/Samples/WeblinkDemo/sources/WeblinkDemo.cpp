/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/ 
/**
\file  WeblinkDemo.cpp

 - The plug-in demonstrates how to register a new Weblink driver with
    the Weblink plug-in.

 - This approach could be used to communicate with a web browser or
   application that is not included in the standard list, or send
   information to Netscape through a different interface than the
   standard Weblink driver.

*********************************************************************/

// Acrobat Headers
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif
#ifndef UNIX_PLATFORM
#include "DebugWindowHFT.h"
#endif

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

static WebDriverVector weblinkDriver;

/*-------------------------------------------------------
	Weblink Driver Callbacks
-------------------------------------------------------*/

/* SupportPredicate
** ----------------------------------------------------*/ 
/**
 ** Called when a browser exe is selected from the
 ** preference menu.
 **/
static ACCB1 ASBool ACCB2 SupportPredicate (void *clientData, WDAppSpecifier theApp)
{
#ifndef UNIX_PLATFORM
	if(gDebugWindowHFT)
		DebugEmitString ("WeblinkDemo : SupportPredicate.");
#else
	printf("WeblinkDemo : SupportPredicate.\n");
#endif
	return true;
}

/* FollowLink
** ----------------------------------------------------*/ 
/**
 ** Function called when a weblink is clicked.
 **
 ** @return true if success, else return false
 **/
ASBool FollowLink (void *clientData, AVDoc avDoc, char *sURL, char *sFormData)
{
	char buf[255];

	if (!sURL || !strlen(sURL))
		return false;

	// Follow URL here.
#ifndef UNIX_PLATFORM
	sprintf(buf, "WeblinkDemo : URL - %s", sURL);
	if(gDebugWindowHFT)
		DebugEmitString(buf);
#else
	printf("WeblinkDemo : URL - %s\n", sURL);
#endif

	return true;
}


/* BecomeDriver
** ----------------------------------------------------*/ 
/**
 ** Set driver's active state depending on bActivate
 ** parameter.
 */
static ACCB1 ASBool ACCB2 BecomeDriver (void *clientData, ASBool bActivate)
{
#ifndef UNIX_PLATFORM
	if(gDebugWindowHFT)
	{
		if (bActivate)
			DebugEmitString ("WeblinkDemo : Become driver - activate.");
		else
			DebugEmitString ("WeblinkDemo : Become driver - de-activate.");
	}
#else
	if(bActivate)
		printf("WeblinkDemo : Become driver - activate.\n");
	else
		printf("WeblinkDemo : Become driver - de-activate.\n");
#endif
	return true;
}

/* wLinkBringFront
** ----------------------------------------------------*/ 
/**
 ** Called when the Web button is clicked. Bring the
 ** web browser to the front
 **/
static ACCB1 void ACCB2 BringFront (void *clientData)
{
#ifndef UNIX_PLATFORM
	if(gDebugWindowHFT)
		DebugEmitString ("WeblinkDemo : Bring browser to front.");
#else
	printf("WeblinkDemo : Bring browswer to front.\n");
#endif
	return;
}

/* OptionsFunction
** ----------------------------------------------------*/ 
/**
 ** Show options dialog.
 */
static ACCB1 void ACCB2 OptionsFunction (void *clientData)
{
#ifndef UNIX_PLATFORM
	if(gDebugWindowHFT)
		DebugEmitString ("WeblinkDemo : Show Options.");
#else
	printf("WeblinkDemo : Show Options.\n");
#endif
	return;
}

/* RegisterWLinkDriver
** ----------------------------------------------------*/ 
/**
 ** register driver, return false if fails
 **/
ASBool RegisterWeblinkDriver (void)
{
	memset (&weblinkDriver, 0, sizeof (WebDriverVector));
	weblinkDriver.WDVRev = WDVCURRENTREV;
	weblinkDriver.doYouSupport = ASCallbackCreateProto (WDAppSupportPredicate, &SupportPredicate);
	weblinkDriver.followLink = ASCallbackCreateProto (WDFollowLinkFunction, &FollowLink);
	weblinkDriver.becomeDriver = ASCallbackCreateProto (WDBecomeDriverFunction, &BecomeDriver);
	weblinkDriver.bringFront = ASCallbackCreateProto (WDBringFrontFunction, &BringFront);
	weblinkDriver.options = ASCallbackCreateProto (WDOptionsFunction, &OptionsFunction);

	return RegisterWebDriver("Sample Weblink Driver", NULL, &weblinkDriver);
}

