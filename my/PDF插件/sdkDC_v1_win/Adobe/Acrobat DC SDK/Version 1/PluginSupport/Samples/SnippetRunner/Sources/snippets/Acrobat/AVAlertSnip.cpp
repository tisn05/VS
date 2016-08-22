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

 AVAlertSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"

/** an example of how to use an AVAlert (of type ALERT_QUESTION). The alert will have three buttons, "Yes", "No" and "Cancel". 
	The message displayed by the alert is passed in through the ParamManager, and the alert should be accompanied with a beep (last
	parameter to the call being true).
	
	@testfile none
	@see AVAlert
*/
void AVAlertSnip(ParamManager *thePM){
	string excepString = "", paramString;
	int count = thePM->getNumberParams();
	for(int i = 0; i<count; i++)
	{
		thePM->getNextParamAsString(paramString);
		if(i!=0)
			excepString += " ";
		excepString+= paramString;
	}
	
	ASInt32 returnVal = AVAlert(ALERT_QUESTION, excepString.c_str(),"Yes", "No", "Cancel", true);
	char * mess;
	switch (returnVal){
		case 1: mess = "Yes";break;
		case 2: mess = "No";break;
		case 3: mess = "Cancel";break;
		default: mess = "Don't know how you got here!";
	}
	AVAlertNote(mess);
}

SNIPRUN_REGISTER_WITH_DIALOG(AVAlertSnip, "AV Layer:Alert:AVAlert","A simple example showing how to use an ALERT_QUESTION","\"Any text you want here\"")


