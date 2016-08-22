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
	
ACEEnumProfilesSnip.cpp

**********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"
	
/**
	This snippet enumerates the profiles installed on the system of a given type. 
	It makes a profile list, gets a count of how many profiles are in the list, 
	gets the description for each item in the list, dumps info in DebugWindow.
	
	For simplicity, the snippets allows user to enumerate profiles three types of pre-defined
	standard profiles:
		1. Standard RGB
		2. Standard Gray
		3. Standard CMYK

	@testfile none
	@requires color profiles installed on the machine
	@see ACStringUnicode
	@see ACUnReferencePresetList
	@see ACProfileListItemDescription
	@see ACMakeProfileList
	@see ACProfileListCount
	@see ASTextFromSizedUnicode
*/
static void GetProfiles(AC_SelectorCode sel);

void ACEEnumProfilesSnip(ParamManager * thePM)
{
	ASInt32 opType;	

	// Get input parameters
	if (ASBoolToBool(thePM->getNextParamAsInt(opType)) != true || opType < 1 || opType > 3){
		AVAlertNote ("Enter a valid operation type (1-3).");
		return;
	}

	switch (opType) {
	case 1:
		/* Standard, recommended RGB profiles. These profiles are always bi-directional.*/
		GetProfiles(AC_Selector_RGB_Standard);
		break;
	case 2:
		/* Standard, recommended grayscale profiles. These profiles are always bi-directional.*/
		GetProfiles(AC_Selector_Gray_Standard);
		break;
	case 3:
		/* Standard, recommended CMYK profiles that can be used as a source. These profiles may or
		may not be useable as a destination.*/
		GetProfiles(AC_Selector_CMYK_StandardInput);
		break;
	default:
		AVAlertNote ("Enter a correct operation type (1-3)\n");
		break;
	}
}

SNIPRUN_REGISTER_WITH_DIALOG(ACEEnumProfilesSnip, "ACE:Enum Color Profiles",
"Enumerate profiles (1-3): \n\
1. Standard RGB\n\
2. Standard Gray\n\
3. Standard CMYK", "1")

static void GetProfiles(AC_SelectorCode sel)
{
	AC_Error err;
	AC_String aceStr;
	ASUns32 profileCount, i;
	AC_ProfileList list;
	char desc[255];
	
	// Creates a list of device color profiles of a given type. Builds a list of those profiles from the 
	// database that meet the criterion of the specified selector.
	if((err = ACMakeProfileList(&list, sel)) == AC_Error_None)
	{
		if ((err = ACProfileListCount(list, &profileCount)) == AC_Error_None)
		{
			for(i = 0; i < profileCount;i++)
			{
				if((err = ACMakeString (&aceStr, (const char *)desc, NULL)) == AC_Error_None)
				{
					if((err = ACProfileListItemDescription(list, i, &aceStr)) == AC_Error_None)
					{
						ASText astext = NULL;
						ASBool isUnicode = false;
						char buf[255];

						ASBool ok = SnippetRunnerUtils::GetStringFromACString (&aceStr, buf, &astext, &isUnicode, 255);
						if (!ok){
							AVAlertNote ("Error in processing ACE string.");
							return;
						}

						if (ASBoolToBool(isUnicode) == true){
							Console::displayString (astext);
							
							ASTextDestroy (astext);
						}
						else
							Console::displayString (buf);
					}
					ACUnReferenceString(aceStr);
				}
			}
		}
		ACUnReferenceProfileList(list);
	}
}
