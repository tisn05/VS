/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 RaiseExcepSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "ParamManager.h"

/** example of a custom error string. This snippet registers the custom error string with the application, it then uses the associated
	error code that is returned to invoke an exception. We depend on the SnippetRunner exception handler to catch this exception.
	@testfile none
	@see ASRegisterErrorString
	@see ASRaise
*/
void RaiseExcepSnip(ParamManager * thePM){
	bool caughtException = false;
	string infoRequired;
	if (ASBoolToBool(thePM->getNextParamAsString(infoRequired))!= true){
		AVAlertNote("Enter an error string");
	}

	DURING
	{
		// register the error string
		ASInt32 errorCode = ASRegisterErrorString(ErrAlways,infoRequired.c_str());

		// The error string is registered in Acrobat now, and a newly created error code 
		// returned by Acrobat which is an long integer. 
		// The error code returned by Acrobat is different in each Acrobat session. 
		// Users don't need to remember the number, but need to assign this error code 
		// to a variable,  so that it can be used in their code. 

		// inform the user 
		Console::displayString("The error string has been registered in Acrobat. The next line is a test ...");
		
		// we just test this error code here ...
		ASRaise(errorCode);
	}
	HANDLER
		Console::displayString("Caught the raised exception");
		caughtException = true;
	END_HANDLER
	
	if(!caughtException)
		AVAlertNote("Didn't catch the raised exception!");

}

SNIPRUN_REGISTER_WITH_DIALOG(RaiseExcepSnip, "AS Layer:Exception:Raise custom","Shows how to raise a custom exception (place text in quotes)","\"Some exception text\"")


