/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------
	
ACEGetWorkingSpaceSnip.cpp

**********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"
/**
	This snippet gets working space profiles (RGB, Gray, and CMYK) information
	such as size, description and color spaces information, and dumps info in DebugWindow.

	@testfile none
	@requires working space profiles
	
	@see ACStringUnicode
	@see ACUnReferencePresetList
	@see ACProfileListItemDescription
	@see ACMakeProfileList
	@see ACProfileListCount
	@see ASTextFromSizedUnicode
*/
static void GetWSProfiles(ACWorkingSpace sel, char * wsname);

void ACEGetWorkingSpaceSnip(ParamManager * thePM)
{
	ASInt32 opType;	

	// Get input parameters
	if (ASBoolToBool(thePM->getNextParamAsInt(opType)) != true || opType < 1 || opType > 3){
		AVAlertNote ("Enter a valid operation type (1-3).");
		return;
	}

	switch (opType) {
	case 1:
		GetWSProfiles(kACWorkingRGB, "RGB working space");
		break;
	case 2:
		GetWSProfiles(kACWorkingGray, "Gray working space");
		break;
	case 3:
		GetWSProfiles(kACWorkingCMYK, "CMYK working space");
		break;
	default:
		AVAlertNote ("Enter a correct operation type (1-3)\n");
		break;
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(ACEGetWorkingSpaceSnip, "ACE:Get Working Spaces",
"Get working space profiles, select type (1-3): \n\
1. RGB\n\
2. Gray\n\
3. CMYK", "1")

static void GetWSProfiles(ACWorkingSpace space, char *wsname)
{

	AC_Error err = AC_Error_None;
	AC_Profile workingProfile;
	ASUns32 size;
	AC_String aceStr;
	char buf[255];

	err = ACGetWorkingSpaceProfile (space, &workingProfile);
	if (err != AC_Error_None)
		AVAlertNote ("Failed to get working space profile.");

	err = ACProfileSize (workingProfile, &size);
	if (err != AC_Error_None){
		AVAlertNote ("Failed to get working space profile size.");
	}
	else{
		sprintf (buf, "\n%s profile \nsize: %d\nDescription: ", wsname, size);
		Console::displayString (buf);
	} 

	err = ACProfileDescription (workingProfile, &aceStr);
		if (err != AC_Error_None){
		AVAlertNote ("Failed to get working space profile size.");
	}
	else{
		sprintf (buf, "%s profile size: %d.", wsname, size);
		ASText astext = NULL;
		ASBool isUnicode = false;
		ASBool ok = SnippetRunnerUtils::GetStringFromACString (&aceStr, buf, &astext, &isUnicode, 255);
		if (ok == false){
			AVAlertNote ("Error in converting ACE string to ASCII string or ASText.");
			return;
		}
		if (ASBoolToBool(isUnicode) == true){
			Console::displayString (astext);
			ASTextDestroy (astext);
		}
		else
			Console::displayString (buf);

		ACUnReferenceString (aceStr);
	}
}
