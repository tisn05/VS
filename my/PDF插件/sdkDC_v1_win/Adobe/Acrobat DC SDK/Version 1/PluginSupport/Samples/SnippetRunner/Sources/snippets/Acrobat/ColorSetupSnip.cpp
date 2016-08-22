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
ColorSetupSnip.cpp

This code snippet demonstrates how to access the color management settings programmatically using 
Adobe Color Engine (ACE). It enumerates all available system color settings, ICC Profiles, conversion engines, for example:

For Windows 2000
C:\Program Files\Common Files\Adobe\Color\Profiles 
C:\WINNT\system32\spool\drivers\color

For MacOS:
System Folder:Application Support:Adobe:Color:
Systems Folder:ColorSync Profiles:

Implementation:
-	enumerate the color settings
-	enumerate the ICC Profiles for RGB, CMYK, and Gray working spaces
-	enumerate the color engines

Usage:
-	select AcroSDK Snippets > Snippet Runner UI > ACE > Color Setup
-	expect to see information about color settings, working space ICC Profiles and color engines

**********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

struct ColorData // globals store the color setup info
{
	string gStrSettings;					// color settings
	string gStrWorkSpaceRGB;			// RGB work space
	string gStrWorkSpaceCMYK;	  // CMYK work space
	string gStrWorkSpaceGray;		// Gray work space
	string default_color_engine;			// color engine options
};

// working spaces
typedef enum
{
	kWorkingGray = 0,
	kWorkingRGB = 1,
	kWorkingCMYK = 2,

	kWorkingSpaces = 3
} WorkingSpace;

/* EnumerateWorkingList
-------------------------------------------------------------------------------
    Enumerate all the working space choices in RGB, Gray, and CMYK
  */
static void EnumerateWorkingList(AC_SelectorCode sel, string* strOut)
{
	AC_Error err;
	AC_String aceStr;
	ASUns32 count, profileCount, i;
	AC_ProfileList l;
	char desc[255];
	
	if((err = ACMakeProfileList(&l, sel)) == AC_Error_None)
	{
		if ((err = ACProfileListCount(l, &profileCount)) == AC_Error_None)
		{
			for(i = 0; i < profileCount;i++)
			{
				if((err = ACMakeString (&aceStr, (const char *)desc, NULL)) == AC_Error_None)
				{
					if((err = ACProfileListItemDescription(l, i, &aceStr)) == AC_Error_None)
					{
						if((err = ACStringASCII(aceStr, desc, &count, sizeof(desc))) == AC_Error_None)
						{
							*strOut += "\r\n\t- ";
							*strOut += desc;
						}
					}
				}
			}
		}
		ACUnReferenceProfileList(l);
	}
}

/* EnumerateSettingsList
-----------------------------------------------------------------
   Enumerate the list of settings and copy the info 
   in the settingsStr
  */
static void EnumerateSettingsList(string* settingsStr, char *pref)
{
	AC_Error err;
	AC_PresetList presetList;
	AC_String aceStr;
	char desc[255];
	ASUns16 UDesc[255];
#if !AS_ARCH_64BIT
	AC_FileSpec file;
#else
    AC_String file;
#endif
	AC_SettingsType type = AC_SettingsType_Color;	/* we don't care about Proof setup files */
	ASBool prefFound = false;
	
	if ((err = ACMakePresetList( &presetList, type)) == AC_Error_None)
	{
		ASUns32 i, presetCount, stringLen;
		if ((err = ACPresetListCount(presetList, &presetCount)) == AC_Error_None)
		{
#if !AS_ARCH_64BIT
			for (i = 0; i < presetCount; i++)
			{
				if ((err = ACPresetListItemFile(presetList, i, &file)) == AC_Error_None)
				{
					if ((err = ACPresetFileToName(&file, &aceStr)) == AC_Error_None)
					{	
						// Try localized folk
						if ((err = ACStringLocalized (aceStr, UDesc, &stringLen, sizeof(UDesc))) == AC_Error_None)
						{
							ASBool isPref = false;
							if (pref)
							{
								// Compare the ASCII strings
								if ((err = ACStringASCII(aceStr, desc, &stringLen, sizeof(desc))) == AC_Error_None)
								{
									isPref = !strcmp(pref, desc);
									if (isPref)
										prefFound = true;
								}
							}
							*settingsStr += "\r\n\t- ";
							*settingsStr += desc;
						}
						// Try a Unicode string
						else if ((err = ACStringUnicode(aceStr, UDesc, &stringLen, sizeof(UDesc) / sizeof(UDesc[0]))) == AC_Error_None)
						{
							// It retuns in unicode length
							stringLen *= sizeof(ASUns16);

							// Try unicode
							ASBool isPref = false;
							// Use typecasting to use correct constructor
							ASText tmpDesc = ASTextFromSizedUnicode((const ASUns16 *) UDesc, kUTF16HostEndian, (ASInt32) stringLen);

							if (pref)
							{
								// This doesn't work on many cases...but not much we can do about it
								isPref = !strcmp(pref, ASTextGetEncodedCopy(tmpDesc,(ASHostEncoding)PDGetHostEncoding()));
								if (isPref)
									prefFound = true;
							}
							char tmp [256];
							strcpy (tmp, ASTextGetEncodedCopy(tmpDesc,(ASHostEncoding)PDGetHostEncoding()));
							*settingsStr += "\r\n\t- ";
							*settingsStr += tmp;
						}
						// Fall back to ASCII string
						else if ((err = ACStringASCII(aceStr, desc, &stringLen, sizeof(desc))) == AC_Error_None)
						{
							// Fall back to the ASCII invariant
							ASBool isPref = false;
							if (pref)
							{
								isPref = !strcmp(pref, desc);
								if (isPref)
									prefFound = true;
							}
							*settingsStr += "\r\n\t- ";
							*settingsStr += desc;
						}
						ACUnReferenceString (aceStr);
					}
				}
			}
#else
            for (i = 0; i < presetCount; i++)
			{
				if ((err = ACPresetListItemFileAndNameU(presetList, i, &file, &aceStr)) == AC_Error_None)
				{
					// Try localized folk
						if ((err = ACStringLocalized (aceStr, UDesc, &stringLen, sizeof(UDesc))) == AC_Error_None)
						{
							ASBool isPref = false;
							if (pref)
							{
								// Compare the ASCII strings
								if ((err = ACStringASCII(aceStr, desc, &stringLen, sizeof(desc))) == AC_Error_None)
								{
									isPref = !strcmp(pref, desc);
									if (isPref)
										prefFound = true;
								}
							}
							*settingsStr += "\r\n\t- ";
							*settingsStr += desc;
						}
						// Try a Unicode string
						else if ((err = ACStringUnicode(aceStr, UDesc, &stringLen, sizeof(UDesc) / sizeof(UDesc[0]))) == AC_Error_None)
						{
							// It retuns in unicode length
							stringLen *= sizeof(ASUns16);
                            
							// Try unicode
							ASBool isPref = false;
							// Use typecasting to use correct constructor
							ASText tmpDesc = ASTextFromSizedUnicode((const ASUns16 *) UDesc, kUTF16HostEndian, (ASInt32) stringLen);
                            
							if (pref)
							{
								// This doesn't work on many cases...but not much we can do about it
								isPref = !strcmp(pref, ASTextGetEncodedCopy(tmpDesc,(ASHostEncoding)PDGetHostEncoding()));
								if (isPref)
									prefFound = true;
							}
							char tmp [256];
							strcpy (tmp, ASTextGetEncodedCopy(tmpDesc,(ASHostEncoding)PDGetHostEncoding()));
							*settingsStr += "\r\n\t- ";
							*settingsStr += tmp;
						}
						// Fall back to ASCII string
						else if ((err = ACStringASCII(aceStr, desc, &stringLen, sizeof(desc))) == AC_Error_None)
						{
							// Fall back to the ASCII invariant
							ASBool isPref = false;
							if (pref)
							{
								isPref = !strcmp(pref, desc);
								if (isPref)
									prefFound = true;
							}
							*settingsStr += "\r\n\t- ";
							*settingsStr += desc;
						}
						ACUnReferenceString (aceStr);
                        ACUnReferenceString (file);
				}
			}

#endif
			// don't forget to unreference the presetList
			ACUnReferencePresetList(presetList);
		}
	}	
}

#if !AS_ARCH_64BIT
static ASBool findSettingsFile (char *description, AC_FileSpec *foundSpec)
{
	AC_Error err;
	AC_PresetList presetList;
	AC_String aceStr;
	char desc[255];
	AC_SettingsType type = AC_SettingsType_Color;	/* we don't care about Proof setup files */
	ASBool prefFound = false;

	if ((err = ACMakePresetList( &presetList, type)) == AC_Error_None)
	{
		ASUns32 i, presetCount, stringLength;
		if ((err = ACPresetListCount(presetList, &presetCount)) == AC_Error_None)
		{
			for (i = 0; i < presetCount; i++)
			{
				if ((err = ACPresetListItemFile(presetList, i, foundSpec)) == AC_Error_None)
				{
					if ((err = ACPresetFileToName(foundSpec, &aceStr)) == AC_Error_None)
					{
						if ((err = ACStringASCII(aceStr, desc, &stringLength, sizeof(desc))) == AC_Error_None)
						{
							if (!strcmp(desc, description))
							{
								ACUnReferencePresetList(presetList);
								return true;
							}
							ACUnReferenceString (aceStr);
						}
					}
				}                
			}
			ACUnReferencePresetList(presetList);
		}
	}	
	return false;
}
#else

static ASBool findSettingsFile (char *description, AC_String *foundSpec)
{
	AC_Error err;
	AC_PresetList presetList;
	AC_String aceStr;
	char desc[255];
	AC_SettingsType type = AC_SettingsType_Color;	/* we don't care about Proof setup files */
	ASBool prefFound = false;
    
	if ((err = ACMakePresetList( &presetList, type)) == AC_Error_None)
	{
		ASUns32 i, presetCount, stringLength;
		if ((err = ACPresetListCount(presetList, &presetCount)) == AC_Error_None)
		{
			for (i = 0; i < presetCount; i++)
			{
				if ((err = ACPresetListItemFileAndNameU(presetList, i, foundSpec, &aceStr)) == AC_Error_None)
				{
						if ((err = ACStringASCII(aceStr, desc, &stringLength, sizeof(desc))) == AC_Error_None)
						{
							if (!strcmp(desc, description))
							{
								ACUnReferencePresetList(presetList);
								return true;
							}
							ACUnReferenceString (aceStr);
						}
                }
                
			}
			ACUnReferencePresetList(presetList);
		}
	}
	return false;
}

#endif

// Load a CSF by name
static AC_Settings loadSettings (char *name)
{
	AC_Settings settings;
#if !AS_ARCH_64BIT
	AC_FileSpec spec;
#else
    AC_String string;
#endif
    if (ACMakeSettings (&settings, AC_SettingsType_Color) != AC_Error_None)
		return NULL;
    
#if !AS_ARCH_64BIT
    if (!findSettingsFile (name, &spec))
        return NULL;
#else
    if (!findSettingsFile (name, &string))
        return NULL;
#endif
		
#if !AS_ARCH_64BIT
    if (ACLoadSettings (settings, &spec) == AC_Error_None)
		return settings;
#else
	if (ACLoadSettingsU (settings, string) == AC_Error_None)
		return settings;
#endif
	else
		return NULL;
}

/**
This code snippet demonstrates how to access the color management settings programmatically using Adobe Color Engine (ACE). 
It enumerates all available system color settings, ICC Profiles, conversion engines, for example:

For Windows 2000
C:\Program Files\Common Files\Adobe\Color\Profiles 
C:\WINNT\system32\spool\drivers\color

For MacOS:
System Folder:Application Support:Adobe:Color:
Systems Folder:ColorSync Profiles:

Implementation:
-	enumerate the color settings
-	enumerate the ICC Profiles for RGB, CMYK, and Gray working spaces
-	enumerate the color engines

@testfile none
@requires correct paths to color settings files and profiles

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
@see ACUnReferenceProfileList
@see ACProfileListItemDescription
@see ACMakeProfileList
@see ACProfileListCount
@see ASTextGetEncodedCopy
@see ASTextFromSizedUnicode
@see AVAppGetPreference
@see SnippetRunnerUtils::ShowDebugWindow()
@see SnippetRunnerUtils::DebugWindowFlush()
*/
void ColorSetupSnip()
{
	ColorData cd;
	
	AC_Error err;
	char *strP;
	AC_Settings settings;

	// enumerate the color settings list first 
	strP = (char *)AVAppGetPreference(avpColorSettingsFile);

	cd.gStrSettings += "\t\tCOLOR SETTINGS\r\n\r\nSETTING PROFILES\r\n\r\n";  
	EnumerateSettingsList(&cd.gStrSettings, strP);
	settings = loadSettings(strP);
	Console::displayString(cd.gStrSettings.c_str());

	// obtain the working space ICC Profile choices for RGB, CMYK, and Gray
	// get RGB ICC Profiles working space choices
	strP = (char *)AVAppGetPreference(avpWorkingRGB);

	cd.gStrWorkSpaceRGB += "\r\nWORKING SPACES \r\n\r\n\tStandard working RGB: "; 
	EnumerateWorkingList(AC_Selector_RGB_Standard, &cd.gStrWorkSpaceRGB);
	cd.gStrWorkSpaceRGB += "\r\nOther input capable RGB: ";
	EnumerateWorkingList(AC_Selector_RGB_OtherInputCapable, &cd.gStrWorkSpaceRGB);
	cd.gStrWorkSpaceRGB += "\r\nOther output capable RGB: ";
	EnumerateWorkingList(AC_Selector_RGB_OtherOutputCapable, &cd.gStrWorkSpaceRGB);
	Console::displayString(cd.gStrWorkSpaceRGB.c_str());

	// get ICC Profiles working space choices
	strP = (char *)AVAppGetPreference(avpWorkingCMYK);

	cd.gStrWorkSpaceCMYK += "\r\nStandard input CMYK: "; 
	EnumerateWorkingList(AC_Selector_CMYK_StandardInput, &cd.gStrWorkSpaceCMYK);
	cd.gStrWorkSpaceCMYK += "\r\nStandard output CMYK: ";
	EnumerateWorkingList(AC_Selector_CMYK_StandardOutput, &cd.gStrWorkSpaceCMYK);
	cd.gStrWorkSpaceCMYK += "\r\nOther output capable CMYK: ";
	EnumerateWorkingList(AC_Selector_CMYK_OtherOutputCapable, &cd.gStrWorkSpaceCMYK);
	Console::displayString(cd.gStrWorkSpaceCMYK.c_str());

	// now get the gray ICC Profiles working space choices
	strP = (char *)AVAppGetPreference(avpWorkingGray);

	cd.gStrWorkSpaceGray += "\r\nStandard working Gray: "; 
	EnumerateWorkingList(AC_Selector_Gray_Standard, &cd.gStrWorkSpaceGray);
	cd.gStrWorkSpaceGray += "\r\nOther input capable Gray: ";
	EnumerateWorkingList(AC_Selector_Gray_OtherInputCapable, &cd.gStrWorkSpaceGray);
	cd.gStrWorkSpaceGray += "\r\nOther output capable Gray: ";
	EnumerateWorkingList(AC_Selector_Gray_OtherOutputCapable, &cd.gStrWorkSpaceGray);
	Console::displayString(cd.gStrWorkSpaceGray.c_str());

	// now get the color engine choice
	ASUns32 engineCount;
	if (ACEngineCount(&engineCount) == AC_Error_None)
	{	
		ASInt64 prefCMM, prefCMS;
		ASUns32 cms, cmm, count, i;
		AC_String aceStr;
		char desc[255];
		ASUns16 UDesc[255];		

		// get CMM and CMS from the preferences panel
		prefCMM = (ASInt64) AVAppGetPreference(avpCMM);
		prefCMS = (ASInt64) AVAppGetPreference(avpCMS);

		cd.default_color_engine += "\r\nCOLOR ENGINES \r\n\r\n\t"; 
		for (i=0;i < engineCount;i++)
		{ 
			if((err = ACMakeString (&aceStr, (const char *)desc, NULL)) == AC_Error_None)
				if((err = ACEngineInfo(i, &aceStr, &cms, &cmm)) == AC_Error_None)					
					if((err = ACStringLocalized(aceStr, UDesc, &count, sizeof(UDesc))) == AC_Error_NoLocalized)
					{
						if((err = ACStringASCII(aceStr, desc, &count, sizeof(desc))) == AC_Error_None)
						{
							cd.default_color_engine += desc;
							cd.default_color_engine += "\r\n\t";
						}
					}
		}
	}
	// display the color engines in the debug window
	Console::displayString(cd.default_color_engine.c_str());

	// unreference the settings 
	ACUnReferenceSettings(settings);
}

// Register the code snippet here
SNIPRUN_REGISTER(ColorSetupSnip, "ACE:Color Setup", 
				 "Enumerate color management options: color settings, ICC Profiles, conversion engines.")
