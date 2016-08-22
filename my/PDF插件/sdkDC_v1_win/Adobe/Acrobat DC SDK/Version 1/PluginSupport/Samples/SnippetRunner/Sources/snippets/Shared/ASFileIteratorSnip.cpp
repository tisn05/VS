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

 ASFileIteratorSnip.cpp

*********************************************************************/
#include "SnippetRunner.h" 
#include "SnippetRunnerUtils.h"

#if MAC_PLATFORM
	#define PathType_K ASAtomFromString ("POSIXPath")
#else
	#define PathType_K ASAtomFromString ("ASTextPath")
#endif
	
static void EnumFolderFile(ASFileSys folderFileSys, ASPathName asp, ASInt32 &numFiles);

/** This snippet shows how to use AS layer APIs to iterate the name of the files
    at a given file system location

	@see ASFileSysCreatePathName
	@see ASFileSysGetItemProps
	@see ASFileSysGetNameFromPath
	@see ASFileSysFirstFolderItem
	@see ASFileSysNextFolderItem
	@see ASFileSysDestroyFolderIterator
	@see ASFileSysReleasePath
  	@see ASGetDefaultFileSys
*/

void ASFileIteratorSnip()
{
	AVOpenSaveDialogParamsRec dialogParams;
	
	memset(&dialogParams, 0, sizeof(dialogParams));
	dialogParams.size = sizeof(dialogParams);
	dialogParams.parentWindow = NULL;
	dialogParams.windowTitle = NULL;
	dialogParams.actionButtonTitle = NULL;
	dialogParams.initialFileSys = ASGetDefaultUnicodeFileSys();
	dialogParams.initialPathName = NULL;
	dialogParams.initialFileName = NULL;
	dialogParams.fileFilters = NULL;
	dialogParams.numFileFilters = 1;
	dialogParams.settingsComputeEnabledProc = NULL;
	dialogParams.settingsExecuteProc = NULL;
	dialogParams.settingsProcData = NULL;
	dialogParams.flags |= kAVOpenSaveAllowForeignFileSystems;

	ASFileSys folderFileSys;
	ASPathName asp = NULL;

	if (!AVAppChooseFolderDialog(&dialogParams, &folderFileSys, &asp)) 
		return;
	
	ASInt32 numFiles = 0;
	EnumFolderFile(folderFileSys, asp, numFiles);

	if (asp != NULL)
		ASFileSysReleasePath(folderFileSys, asp);

	if (numFiles > 0){
		char buf[256];
		ASText astPath = ASTextNew();
		ASFileSysGetNameFromPathAsASText(folderFileSys, asp, astPath);
		sprintf(buf, "\nThere are %d files in the folder %s\n", 
			numFiles, ASTextGetScriptText(astPath, kASEUnicodeScript));
		Console::displayString(buf);
	}
	else{
		Console::displayString("The selected path contains no files.");
	}
}

/* Recursively iterate files in the folder */
static void EnumFolderFile(ASFileSys folderFileSys, ASPathName asp, ASInt32 &numFiles)
{
	ASFileSysItemPropsRec props;
	ASFolderIterator iter = NULL;
	props.size = sizeof(props);
	ASPathName aspItem = NULL;

	ASFileSysGetItemProps(folderFileSys, asp, &props);
	
	if (!props.isThere){
		return;
	}
	
	if (props.type == kASFileSysFile){ // If it is a file, output the file name
		ASText astFileName = ASTextNew();
		if (ASFileSysGetNameFromPathAsASText(folderFileSys, asp, astFileName) == 0){
			Console::displayString(ASTextGetScriptText(astFileName, kASEUnicodeScript));
		}
		numFiles++;
	} 

	if (props.type == kASFileSysFolder) { // If it is a folder, keep going
		iter = ASFileSysFirstFolderItem(folderFileSys, asp, &props, &aspItem);
		
		while (aspItem != NULL) {
			char* filenm = ASFileSysDisplayStringFromPath(NULL, aspItem);
			ASInt32 len = strlen(filenm);
			// this is somewhat naive, but it stops a recursive crawl up the dir structure
			if (filenm[len-1]!='.') {
				EnumFolderFile(folderFileSys, aspItem, numFiles);
			}
			
			// Don't forget to release the AS file path
			if (aspItem != NULL)
				ASFileSysReleasePath(folderFileSys, aspItem);
			aspItem = NULL;

			ASFileSysNextFolderItem(folderFileSys, iter, &props, &aspItem);
		}

		if (iter != NULL)
	 		ASFileSysDestroyFolderIterator(folderFileSys, iter);
	} 
}

SNIPRUN_REGISTER(ASFileIteratorSnip, "AS Layer:File Iterator",
"Iterate all files in a given file system folder path. Enter a folder path. \nIf the path name contains \
spaces, the path name string should be \nplaced in quotes.")


