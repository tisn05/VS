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

 ACEEnumSettingsSnip.cpp

**********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

static void EnumerateSettings(AC_SettingsType type);

/**
This code snippet demonstrates how to access color Setup files programmatically using Adobe Color Engine (ACE). 
It enumerates all available system Setup Files and Proof Setup Files per windows

@testfile none
@requires color setup files
@see ACStringLocalized
@see ACUnReferenceSettings
@see ACLoadSettings
@see ACMakeSettings
@see ACStringUnicode
@see ACUnReferencePresetList
@see ACPresetFileToName
@see ACPresetListItemFile
@see ACPresetListCount
@see ACMakePresetList
@see ASTextFromSizedUnicode
*/
void ACEEnumSettingsSnip()
{
	// Enumerate global setup files
	Console::displayString ("Global Setup Files\n---------------------");
	AC_SettingsType type = AC_SettingsType_Color;
	// Pass in AC_SettingsType_Proof as AC_SettingsType to enumerate proof setup files
	EnumerateSettings(type);
}

// Register the code snippet here
SNIPRUN_REGISTER(ACEEnumSettingsSnip, "ACE:Enum Color Settings", 
				 "Enumerate color settings and dump info to Snippet Runner output pane.")

/*	Enumerate the list of setup files
	AC_SettingsType type IN the type of setup files, can be either global per system or
	"proof" per window settings
  */
static void EnumerateSettings(AC_SettingsType type)
{
	AC_Error err;
	AC_PresetList presetList;
	AC_String aceStr;
	char buffer[128];
#if !AS_ARCH_64BIT
	AC_FileSpec file;
#else
    AC_String file;
#endif
	ASBool prefFound = false;
	
	if ((err = ACMakePresetList( &presetList, type)) == AC_Error_None)
	{
		ASUns32 i, presetCount;
		if ((err = ACPresetListCount(presetList, &presetCount)) == AC_Error_None)
		{
			sprintf (buffer, "Found %d Setup Files:\n", presetCount);
			Console::displayString (buffer);
#if !AS_ARCH_64BIT
			for (i = 0; i < presetCount; i++)
			{
				if ((err = ACPresetListItemFile(presetList, i, &file)) == AC_Error_None)
				{
					if ((err = ACPresetFileToName(&file, &aceStr)) == AC_Error_None)
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
#else
            for (i = 0; i < presetCount; i++)
			{
				if ((err = ACPresetListItemFileAndNameU(presetList, i, &file, &aceStr)) == AC_Error_None)
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
                    ACUnReferenceString(file);
            }
#endif
		}
		// don't forget to unreference the presetList
		ACUnReferencePresetList(presetList);
	}	
}
