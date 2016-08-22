/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1999-2008 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 DMSIntegration.cpp

 -	DMSIntegration implementation file.
 -	Demonstrate how a DMS system can integrate with Acrobat.
 -	You need write your own ASFileSys for your DMS system.
 -  The ASFileSys implementation in this sample is basically a wrapper around 
	the default file system. 
 -  Demonstrate how a DMS system can replace the following three procedures
	AVAppChooseFolderDialog 
	AVAppOpenDialog 
	AVAppSaveDialog 
*********************************************************************/

#include "PIHeaders.h"

static ASFileSysRec dmsFileSys;
static ASFileSys defFileSys;

extern ASCallback gcbAVAppOpenDialog;
extern ASCallback gcbAVAppChooseFolderDialog;
extern ASCallback gcbAVAppSaveDialog;

/*-------------------------------------------------------------------------
	Implementation of callback to replace method AVAppOpenDialog.
	Your implementation should bring up the appropriate UI for the DMS. 
-----------------------------------------------------------------------------*/
ACCB1 ASBool ACCB2 MyAVAppOpenDialog(
	AVOpenSaveDialogParams dialogParams,			
	ASFileSys* outFileSys,			
	ASPathName** outASPathNames,		
	AVArraySize* outNumASPathNames,		
	AVFilterIndex	*ioChosenFilterIndex)
{
	ASBool bRet = false;

	ASBool bCreateWindowTitle = false;
	char* strDms = " (DMS)";
	ASText asText = ASTextFromUnicode(reinterpret_cast <ASUTF16Val *>(strDms), 
			kUTF8);

	if(!dialogParams->windowTitle) {
		char * strWindowTitle = "Open (DMS)";
		dialogParams->windowTitle = ASTextFromUnicode(
			reinterpret_cast <ASUTF16Val *>(strWindowTitle), 
			kUTF8);
		bCreateWindowTitle = true;
	} else {
		ASTextCat(dialogParams->windowTitle, asText);
	}

	// This sample makes use of the default AVAppOpenDialog, you should implement your
	// own open dialog function for your DMS.
	if(CALL_REPLACED_PROC(gAcroViewHFT, AVAppOpenDialogSEL, gcbAVAppOpenDialog)
		(dialogParams, outFileSys, outASPathNames, 
		outNumASPathNames, ioChosenFilterIndex)) {
			bRet = true;
			*outFileSys = (ASFileSys)&dmsFileSys;
	}

	if(bCreateWindowTitle){
		ASTextDestroy(dialogParams->windowTitle);
		dialogParams->windowTitle = NULL;
	}

	ASTextDestroy(asText);

	return bRet;
}

/*---------------------------------------------------------------------
	Implementation of callback to replace method AVAppSaveDialog.
	Your implementation should bring up the appropriate UI for the DMS. 
------------------------------------------------------------------------*/
ACCB1 ASBool ACCB2 MyAVAppSaveDialog(
	AVOpenSaveDialogParams dialogParams,			
	ASFileSys		*outFileSys,			
	ASPathName		*outASPathName,			
	AVFilterIndex	*ioChosenFilterIndex)
{
	ASBool bRet = false;

	ASBool bCreateWindowTitle = false;
	char * strDms = " (DMS)";
	ASText asText = ASTextFromUnicode(reinterpret_cast <ASUTF16Val *>(strDms), 
			kUTF8);

	if(!dialogParams->windowTitle) {
		char * strWindowTitle = "DMS Save As";
		dialogParams->windowTitle = ASTextFromUnicode(
			reinterpret_cast <ASUTF16Val *>(strWindowTitle), 
			kUTF8);
		bCreateWindowTitle = true;
	} else {
		ASTextCat(dialogParams->windowTitle, asText);
	}

	// This sample makes use of the default AVAppSaveDialog. You should implement
	// your own save dialog function for your DMS.
	if(CALL_REPLACED_PROC(gAcroViewHFT, AVAppSaveDialogSEL, gcbAVAppSaveDialog)
		(dialogParams, outFileSys, outASPathName, ioChosenFilterIndex)) {
			bRet = true;
			*outFileSys = (ASFileSys)&dmsFileSys;
	}

	if(bCreateWindowTitle){
		ASTextDestroy(dialogParams->windowTitle);
		dialogParams->windowTitle = NULL;
	}

	ASTextDestroy(asText);

	return bRet;
}

/*----------------------------------------------------------------------------
	Implementation of callback to replace method AVAppChooseFolderDialog.
	Your implementation should bring up the appropriate UI for the DMS. 
-----------------------------------------------------------------------------*/
ACCB1 ASBool ACCB2 MyAVAppChooseFolderDialog(
	AVOpenSaveDialogParams dialogParams,
	ASFileSys* outFileSys,
	ASPathName* outASPathName)
{
	ASBool bRet = false;

	ASBool bCreateWindowTitle = false;
	char * strDms = " (DMS)";
	ASText asText = ASTextFromUnicode(reinterpret_cast <ASUTF16Val *>(strDms), 
			kUTF8);

	if(!dialogParams->windowTitle) {
		char * strWindowTitle = "DMS Browse For Folder";
		dialogParams->windowTitle = ASTextFromUnicode(
			reinterpret_cast <ASUTF16Val *>(strWindowTitle), 
			kUTF8);
		bCreateWindowTitle = true;
	} else {
		ASTextCat(dialogParams->windowTitle, asText);
	}

	// This sample makes use of the default AVAppChooseFolderDialog. You should implement
	// your own shoose folder dialog function for your DMS.
	if(CALL_REPLACED_PROC(gAcroViewHFT, AVAppChooseFolderDialogSEL, 
		gcbAVAppChooseFolderDialog)
		(dialogParams, outFileSys, outASPathName)) {
			bRet = true;
			*outFileSys = (ASFileSys)&dmsFileSys;
	}

	if(bCreateWindowTitle){
		ASTextDestroy(dialogParams->windowTitle);
		dialogParams->windowTitle = NULL;
	}

	ASTextDestroy(asText);

	return bRet;
}

/*-------------------------------------------------------
	File System Callbacks
-------------------------------------------------------*/

/* MyASFileSysOpenProc
** ------------------------------------------------------
** */ 
/** Called when opening a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysOpenProc 
	(ASPathName pathName, ASUns16 mode, MDFile *fP)
{
	return defFileSys->open (pathName, mode, fP);
}

/* MyASFileSysCloseProc
** ------------------------------------------------------
** */
/** Called when closing a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysCloseProc (MDFile f)
{
	return defFileSys->close (f);
}

/* MyASFileSysFlushProc
** ------------------------------------------------------
** */ 
/** Called when flushing a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysFlushProc (MDFile f)
{
	return defFileSys->flush (f);
}

/* MyASFileSysSetPosProc
** ------------------------------------------------------
** */ 
/** Called when setting a position within a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysSetPosProc (MDFile f, ASUns32 pos)
{
	return defFileSys->setpos (f,pos);
}

/* MyASFileSysGetPosProc
** ------------------------------------------------------
** */ 
/** For getting the marker position in a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysGetPosProc (MDFile f, ASUns32 *pos)
{
	return defFileSys->getpos (f, pos);
}

/* MyASFileSysSetEofProc
** ------------------------------------------------------
** */ 
/** For setting the end of file marker
 **
 ** @return 0 if success, else a platform-dependent error code
*/
static	ACCB1 ASInt32 ACCB2 MyASFileSysSetEofProc (MDFile f, ASUns32 pos)
{
	return defFileSys->seteof (f, pos);
}

/* MyASFileSysGetEofProc
** ------------------------------------------------------
** */ 
/** For getting the end of file marker
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysGetEofProc (MDFile f, ASUns32 *pos)
{
	return defFileSys->geteof (f, pos);
}

/* MyASFileSysReadProc
** ------------------------------------------------------
** */ 
/** Used for reading from the file
 **
 ** @return the number of bytes read, or 0 if there was an error
*/
static ACCB1 ASSize_t ACCB2 MyASFileSysReadProc 
	(void *ptr, ASSize_t size, ASSize_t count, MDFile f, ASInt32 *pError)
{
	return defFileSys->read (ptr, size, count, f, pError);
}

/* MyASFileSysWriteProc
** ------------------------------------------------------
** */ 
/** Used for writing to a file
 **
 ** @return the number of bytes written, or 0 if there was an error
 ** */
static ACCB1 ASSize_t ACCB2 MyASFileSysWriteProc 
	(void *ptr, ASSize_t size, ASSize_t count, MDFile f, ASInt32 *pError)
{
	return defFileSys->write (ptr, size, count, f, pError);
}

/* MyASFileSysRemoveProc
** ------------------------------------------------------
** */ 
/** Called when removing a file
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysRemoveProc (ASPathName pathName)
{
	return defFileSys->remove (pathName);
}

/* MyASFileSysRenameProc
** ------------------------------------------------------
** */ 
/** Called when renaming a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysRenameProc 
	(ASMDFile* f, ASPathName oldPath, ASPathName newPath)
{
	return defFileSys->rename (f, oldPath, newPath);
}

/* MyASFileSysIsSameFileProc
** ------------------------------------------------------
** */ 
/** Used for comparing two ASPathNames to see if they are the same file
 **
 ** @return true if they are the same file
 ** */
static ACCB1 ASBool ACCB2 MyASFileSysIsSameFileProc 
	(ASMDFile f, ASPathName pathName, ASPathName newPathName)
{
	return defFileSys->isSameFile(f, pathName, newPathName);
}

/* MyASFileSysGetNameProc
** ------------------------------------------------------
** */ 
/** Called to get the name of a file, (not the full path)
 **
 ** @return 0 if success, else a platform-dependent error code
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysGetNameProc 
	(ASPathName pathName, char *name, ASInt32 maxLength)
{
	return defFileSys->getName (pathName, name, maxLength);
}

/*MyASFileSysGetNameAsASTextProc
** -------------------------------------------------------
** */
/** Called to get the file name as an ASText object
 **
 ** @return 0 if success, else a platform-dependent error code
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysGetNameAsASTextProc 
	(ASPathName pathName, ASText name)
{
	return defFileSys->getNameAsASText (pathName, name);
}

/* MyASFileSysGetTempPathNameProc
** ------------------------------------------------------
** */ 
/** Called to get a valid temporary file name, if pathName is not null, will return a path that
 ** allows for renaming of the file
 **
 ** @return ASPathName pointing at file, or NULL.
 ** */
static ACCB1 ASPathName ACCB2 MyASFileSysGetTempPathNameProc 
	(ASPathName pathName)
{
	return defFileSys->getTempPathName (pathName);
}

/* MyASFileSysCopyPathNameProc
** ------------------------------------------------------
** */ 
/** Creates a copy of the path
 **
 ** @return new ASPathName object
*/
static ACCB1 ASPathName ACCB2 MyASFileSysCopyPathNameProc 
	(ASPathName pathName)
{
	return defFileSys->copyPathName (pathName);
}

/* MyASFileSysDiPathFromPathProc
** ------------------------------------------------------
** */ 
/** Returns a Device independent path.
 ** This string should be freed with ASfree when the
 ** caller is done with it
 **
 ** @return a string with that path. 
 ** */
static ACCB1 char * ACCB2 MyASFileSysDiPathFromPathProc 
	(ASPathName path, ASPathName relativeToThisPath)
{
	return defFileSys->diPathFromPath (path, relativeToThisPath);
}

/* MyASFileSysPathFromDIPathProc
** ------------------------------------------------------
** */ 
/** Converts a device independent path to a file system dependent path
 **
 ** @return ASPathName for the path
 ** */
static ACCB1 ASPathName ACCB2 MyASFileSysPathFromDIPathProc 
	(const char * diPath, ASPathName relativeToThisPath)
{
	return defFileSys->pathFromDIPath (diPath, relativeToThisPath);
}

/* MyASFileSysDisposePathNameProc
** ------------------------------------------------------
** */ 
/** Frees the ASPathName object. Does not do file-level operations.
*/
static ACCB1 void ACCB2 MyASFileSysDisposePathNameProc 
	(ASPathName pathName)
{
	defFileSys->disposePathName (pathName);
}

/* MyASFileSysGetFileSysNameProc
** ------------------------------------------------------
** */ 
/** Get the name of the file system
 **
 ** @return an ASAtom containing the file system name
 ** */
static ACCB1 ASAtom ACCB2 MyASFileSysGetFileSysNameProc (void)
{
	return ASAtomFromString ("SampleFileSystem");;
}

/* MyASFileSysGetStorageFreeSpaceProc
** ------------------------------------------------------
** */ 
/** Gets the free space on the same volume as the pathname
 **
 ** @return the free space in bytes. The upper limit is 4 GB, 
 ** because it returns an ASInt32
 ** */
static ACCB1 ASUns32 ACCB2 MyASFileSysGetStorageFreeSpaceProc 
	(ASPathName pathName)
{
	return defFileSys->getStorageFreeSpace (pathName);
}

/* MyASFileSysFlushVolumeProc
** ------------------------------------------------------
** */ 
/** FlushVolume is not present in default filesys on
 ** Windows, but it is present on the Macintosh.
 **
 ** @return 0 if success, otherwise returns a platform-
 ** dependent error code.
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysFlushVolumeProc 
	(ASPathName pathName)
{
#if MAC_PLATFORM
	return defFileSys->flushVolume (pathName);
#endif
	return 0;

}

/* MyASFileSysCreatePathNameProc
** ------------------------------------------------------
** */ 
/** Prompt user for a file.
 **
 ** @return ASPathName pointing at file, or NULL
 ** */
static ACCB1 ASPathName ACCB2 MyASFileSysCreatePathNameProc 
	(ASAtom pathSpecType, const void *pathSpec, const void *mustBeZero)
{
	return defFileSys->createPathName (pathSpecType, 
		pathSpec, mustBeZero);
}

/* MyASFileSysGetItemPropsProc
** ------------------------------------------------------
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysGetItemPropsProc 
	(ASPathName pathName, ASFileSysItemProps props)
{
	return defFileSys->getItemProps (pathName, props);
}

/* MyASFileSysFirstFolderItemProc
** ------------------------------------------------------
*/
static ACCB1 ASFolderIterator ACCB2 MyASFileSysFirstFolderItemProc 
	(ASPathName folderPath, ASFileSysItemProps props, ASPathName *itemPath)
{
	return defFileSys->firstFolderItem (folderPath, props, itemPath);
}

/* MyASFileSysNextFolderItemProc
** ------------------------------------------------------
*/
static ACCB1 ASBool ACCB2 MyASFileSysNextFolderItemProc 
	(ASFolderIterator folderIter, ASFileSysItemProps props, ASPathName *itemPath)
{
	return defFileSys->nextFolderItem (folderIter, props, itemPath);
}

/* MyASFileSysDestroyFolderIteratorProc
** ------------------------------------------------------
*/
static ACCB1 void ACCB2 MyASFileSysDestroyFolderIteratorProc 
	(ASFolderIterator folderIter)
{
	defFileSys->destroyFolderIterator (folderIter);
}

/* MyASFileSysURLFromPathProc
** ------------------------------------------------------
*/
static ACCB1 char * ACCB2 MyASFileSysURLFromPathProc (ASPathName path)
{
	return defFileSys->urlFromPath (path);
}

/* MyASFileSysGetParentProc
** ------------------------------------------------------
*/
static ACCB1 ASPathName ACCB2 MyASFileSysGetParentProc (ASPathName path)
{
	return defFileSys->getParent (path);
}

/* MyASFileSysCreateFolderProc
** ------------------------------------------------------
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysCreateFolderProc (ASPathName path)
{
	return defFileSys->createFolder (path);
}

/* MyASFileSysRemoveFolderProc
** ------------------------------------------------------
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysRemoveFolderProc (ASPathName path)
{
	return defFileSys->removeFolder (path);
}

/* MyASFileSysDisplayStringFromPathProc
** ------------------------------------------------------
*/
static ACCB1 char * ACCB2 MyASFileSysDisplayStringFromPathProc 
	(ASPathName path)
{
	return defFileSys->displayStringFromPath (path);
}

/* MyASFileSysSetTypeAndCreatorProc
** ------------------------------------------------------
*/
static ACCB1 void ACCB2 MyASFileSysSetTypeAndCreatorProc 
	(ASPathName path, ASUns32 type, ASUns32 creator)
{
#if MAC_PLATFORM
	defFileSys->setTypeAndCreator (path, type, creator);
#endif
}

/* MyASFileSysGetTypeAndCreatorProc
** ------------------------------------------------------
*/
static ACCB1 void ACCB2 MyASFileSysGetTypeAndCreatorProc 
	(ASPathName path, ASUns32 *type, ASUns32 *creator)
{
#if MAC_PLATFORM
	defFileSys->getTypeAndCreator (path, type, creator);
#endif
}

/* MyASFileSysAcquirePlatformPathProc
** ------------------------------------------------------
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysAcquirePlatformPathProc
	(ASPathName path, ASAtom platformPathType, ASPlatformPath *platformPath)
{
	return defFileSys->acquirePlatformPath (path, 
		platformPathType, platformPath);
}

/* MyASFileSysReleasePlatformPathProc
** ------------------------------------------------------
*/
static ACCB1 void ACCB2 MyASFileSysReleasePlatformPathProc
	(ASPlatformPath platformPath)
{
	return defFileSys->releasePlatformPath (platformPath);
}

/* MyASFileSysAcquireFileSysPathProc
** -----------------------------------------------------------
*/
static ACCB1 ASPathName ACCB2 MyASFileSysAcquireFileSysPathProc
	(ASPathName pathName, ASFileSys newFileSys)
{
	return defFileSys->acquireFileSysPath(pathName, newFileSys);
}

/* MyASFileSysOpen64Proc
** --------------------------------------------------------
*/
static ACCB1 ASErrorCode ACCB2 MyASFileSysOpen64Proc
	(ASPathName pathName, ASFileMode mode, ASMDFile *fP)
{
	return defFileSys->open64(pathName, mode, fP);
}

/* MyASFileSysGetFilePositionLimitProc
** --------------------------------------------------------
*/
static ACCB1 ASErrorCode ACCB2 MyASFileSysGetFilePositionLimitProc(ASFilePos64 *pos)
{
	return defFileSys->getfileposlimit(pos);
}

/* MyASFileSysSetPos64Proc
** -------------------------------------------------------
*/
static ACCB1 ASErrorCode ACCB2 MyASFileSysSetPos64Proc
	(ASMDFile f, ASFilePos64 pos)
{
	return defFileSys->setpos64(f, pos);
}

/* MyASFileSysGetPos64Proc
** --------------------------------------------------------
*/
static ACCB1 ASErrorCode ACCB2 MyASFileSysGetPos64Proc
	(ASMDFile f, ASFilePos64 *pos)
{
	return defFileSys->getpos64(f, pos);
}

/* MyASFileSysSetEof64Proc
** ----------------------------------------------------------
*/
static ACCB1 ASErrorCode ACCB2 MyASFileSysSetEof64Proc
	(ASMDFile f, ASFilePos64 pos)
{
	return defFileSys->seteof64(f, pos);
}

/* MyASFileSysGetEof64Proc
** --------------------------------------------------------
*/
static ACCB1 ASErrorCode ACCB2 MyASFileSysGetEof64Proc
	(ASMDFile f, ASFilePos64 *pos)
{
	return defFileSys->geteof64(f, pos);
}

/* MyASFileSysGetNameForDisplayProc
** ---------------------------------------------------------
*/
static ACCB1 ASErrorCode ACCB2 MyASFileSysGetNameForDisplayProc
	(ASPathName pathName, ASText nameForDisplay)
{
	return defFileSys->getNameForDisplay(pathName, nameForDisplay);
}

/* MyASFileSysGetStorageFreeSpace64Proc
** ------------------------------------------------------------
*/
static ACCB1 ASDiskSpace64 ACCB2 MyASFileSysGetStorageFreeSpace64Proc
	(ASPathName pathName)
{
	return defFileSys->getStorageFreeSpace64(pathName);
}

/* MyASFileSysDisplayASTextFromPathProc
** ----------------------------------------------------------------
*/
static ACCB1 ASErrorCode ACCB2 MyASFileSysDisplayASTextFromPathProc
	(ASPathName path, ASText displayText)
{
	return defFileSys->displayASTextFromPath(path, displayText);
}

/* MyASFileSysCanSetEofProc
** --------------------------------------------------------
*/
static ACCB1 ASBool ACCB2 MyASFileSysCanSetEofProc
	(ASMDFile f, ASFilePos pos)
{
	return defFileSys->canSetEof(f, pos);
}

/* MyASFileSysDIPathFromPathExProc
** ---------------------------------------------------------
*/
static ACCB1 ASErrorCode ACCB2 MyASFileSysDIPathFromPathExProc
		(ASPathName path, ASPathName relativeToThisPath, ASText diPathText)
{
	return defFileSys->diPathFromPathEx(path, relativeToThisPath, diPathText);
}

/* MyASFileSysPathFromDIPathExProc
** ----------------------------------------------------
*/
static ACCB1 ASPathName ACCB2 MyASFileSysPathFromDIPathExProc
		(ASConstText diPathText, ASPathName relativeToThisPath)
{
	return defFileSys->pathFromDIPathEx(diPathText, relativeToThisPath);
}

/* MyASFileSysIsInUseProc
** ------------------------------------------------------
*/
static ACCB1 ASBool ACCB2 MyASFileSysIsInUseProc(ASPathName pathName)
{
	return defFileSys->isInUse(pathName);
}

/* InitializeFileSystem
** ------------------------------------------------------
** */ 
/** Sets up all the callbacks for the file system.
 **
 ** @return true if the file system was registered
 ** successfully.
 **
 ** The main initialization routine.
 **
 ** Return true to continue loading plug-in.
 ** Return false to cause plug-in loading to stop.
 ** */
ACCB1 ASBool ACCB2 InitilizeDMSFileSystems(void)
{
	defFileSys = ASGetDefaultFileSys();

	memset (&dmsFileSys, 0, sizeof(ASFileSysRec));

	dmsFileSys.size = sizeof(ASFileSysRec);

	dmsFileSys.open = ASCallbackCreateProto (ASFileSysOpenProc, MyASFileSysOpenProc);
	dmsFileSys.close = ASCallbackCreateProto (ASFileSysCloseProc, MyASFileSysCloseProc);
	dmsFileSys.flush = ASCallbackCreateProto (ASFileSysFlushProc, MyASFileSysFlushProc);
	dmsFileSys.setpos = ASCallbackCreateProto (ASFileSysSetPosProc, MyASFileSysSetPosProc);
	dmsFileSys.getpos = ASCallbackCreateProto (ASFileSysGetPosProc, MyASFileSysGetPosProc);
	dmsFileSys.seteof = ASCallbackCreateProto (ASFileSysSetEofProc, MyASFileSysSetEofProc);
	dmsFileSys.geteof = ASCallbackCreateProto (ASFileSysGetEofProc, MyASFileSysGetEofProc);
	dmsFileSys.read = ASCallbackCreateProto (ASFileSysReadProc, MyASFileSysReadProc);
	dmsFileSys.write = ASCallbackCreateProto (ASFileSysWriteProc, MyASFileSysWriteProc);
	dmsFileSys.remove = ASCallbackCreateProto (ASFileSysRemoveProc, MyASFileSysRemoveProc);
	dmsFileSys.rename = ASCallbackCreateProto (ASFileSysRenameProc, MyASFileSysRenameProc);
	dmsFileSys.isSameFile = ASCallbackCreateProto (ASFileSysIsSameFileProc, MyASFileSysIsSameFileProc);
	dmsFileSys.getName = ASCallbackCreateProto (ASFileSysGetNameProc, MyASFileSysGetNameProc);
	dmsFileSys.getTempPathName = ASCallbackCreateProto (ASFileSysGetTempPathNameProc, MyASFileSysGetTempPathNameProc);
	dmsFileSys.copyPathName = ASCallbackCreateProto (ASFileSysCopyPathNameProc, MyASFileSysCopyPathNameProc);
	dmsFileSys.diPathFromPath = ASCallbackCreateProto (ASFileSysDiPathFromPathProc, MyASFileSysDiPathFromPathProc);
	dmsFileSys.pathFromDIPath = ASCallbackCreateProto (ASFileSysPathFromDIPathProc, MyASFileSysPathFromDIPathProc);
	dmsFileSys.disposePathName = ASCallbackCreateProto (ASFileSysDisposePathNameProc, MyASFileSysDisposePathNameProc);
	dmsFileSys.getFileSysName = ASCallbackCreateProto (ASFileSysGetFileSysNameProc, MyASFileSysGetFileSysNameProc);
	dmsFileSys.getStorageFreeSpace = ASCallbackCreateProto (ASFileSysGetStorageFreeSpaceProc, MyASFileSysGetStorageFreeSpaceProc);
	dmsFileSys.flushVolume = ASCallbackCreateProto (ASFileSysFlushVolumeProc, MyASFileSysFlushVolumeProc);
	dmsFileSys.createPathName = ASCallbackCreateProto (ASFileSysCreatePathNameProc, MyASFileSysCreatePathNameProc);

	dmsFileSys.acquireFileSysPath = ASCallbackCreateProto (ASFileSysAcquireFileSysPathProc, MyASFileSysAcquireFileSysPathProc);

	dmsFileSys.getItemProps = ASCallbackCreateProto (ASFileSysGetItemPropsProc, MyASFileSysGetItemPropsProc);
	dmsFileSys.firstFolderItem = ASCallbackCreateProto (ASFileSysFirstFolderItemProc, MyASFileSysFirstFolderItemProc);
	dmsFileSys.nextFolderItem = ASCallbackCreateProto (ASFileSysNextFolderItemProc, MyASFileSysNextFolderItemProc);
	dmsFileSys.destroyFolderIterator = ASCallbackCreateProto (ASFileSysDestroyFolderIteratorProc, MyASFileSysDestroyFolderIteratorProc);
	dmsFileSys.urlFromPath = ASCallbackCreateProto (ASFileSysURLFromPathProc, MyASFileSysURLFromPathProc);
	dmsFileSys.getParent = ASCallbackCreateProto (ASFileSysGetParentProc, MyASFileSysGetParentProc);
	dmsFileSys.createFolder = ASCallbackCreateProto (ASFileSysCreateFolderProc, MyASFileSysCreateFolderProc);
	dmsFileSys.removeFolder = ASCallbackCreateProto (ASFileSysRemoveFolderProc, MyASFileSysRemoveFolderProc);
	dmsFileSys.displayStringFromPath = ASCallbackCreateProto (ASFileSysDisplayStringFromPathProc, MyASFileSysDisplayStringFromPathProc);
	dmsFileSys.setTypeAndCreator = ASCallbackCreateProto (ASFileSysSetTypeAndCreatorProc, MyASFileSysSetTypeAndCreatorProc);
	dmsFileSys.getTypeAndCreator = ASCallbackCreateProto (ASFileSysGetTypeAndCreatorProc, MyASFileSysGetTypeAndCreatorProc);

	dmsFileSys.acquirePlatformPath = ASCallbackCreateProto (ASFileSysAcquirePlatformPathProc, MyASFileSysAcquirePlatformPathProc);
	dmsFileSys.releasePlatformPath = ASCallbackCreateProto (ASFileSysReleasePlatformPathProc, MyASFileSysReleasePlatformPathProc);

	dmsFileSys.getNameAsASText = ASCallbackCreateProto (ASFileSysGetNameAsASTextProc, MyASFileSysGetNameAsASTextProc);
	dmsFileSys.displayASTextFromPath = ASCallbackCreateProto (ASFileSysDisplayASTextFromPathProc, MyASFileSysDisplayASTextFromPathProc);
	dmsFileSys.canSetEof = ASCallbackCreateProto (ASFileSysCanSetEofProc, MyASFileSysCanSetEofProc);
	dmsFileSys.diPathFromPathEx = ASCallbackCreateProto (ASFileSysDIPathFromPathExProc, MyASFileSysDIPathFromPathExProc);
	dmsFileSys.pathFromDIPathEx = ASCallbackCreateProto (ASFileSysPathFromDIPathExProc, MyASFileSysPathFromDIPathExProc);
	dmsFileSys.getfileposlimit = ASCallbackCreateProto (ASFileSysGetFilePositionLimitProc, MyASFileSysGetFilePositionLimitProc);
	dmsFileSys.open64 = ASCallbackCreateProto (ASFileSysOpen64Proc, MyASFileSysOpen64Proc);
	dmsFileSys.setpos64 = ASCallbackCreateProto (ASFileSysSetPos64Proc, MyASFileSysSetPos64Proc);
	dmsFileSys.getpos64 = ASCallbackCreateProto (ASFileSysGetPos64Proc, MyASFileSysGetPos64Proc);
	dmsFileSys.seteof64 = ASCallbackCreateProto (ASFileSysSetEof64Proc, MyASFileSysSetEof64Proc);
	dmsFileSys.geteof64 = ASCallbackCreateProto (ASFileSysGetEof64Proc, MyASFileSysGetEof64Proc);
	dmsFileSys.getNameForDisplay = ASCallbackCreateProto (ASFileSysGetNameForDisplayProc, MyASFileSysGetNameForDisplayProc);
	dmsFileSys.getStorageFreeSpace64 = ASCallbackCreateProto (ASFileSysGetStorageFreeSpace64Proc, MyASFileSysGetStorageFreeSpace64Proc);
	dmsFileSys.isInUse = ASCallbackCreateProto (ASFileSysIsInUseProc, MyASFileSysIsInUseProc);

	
	/* The following are other callbacks we don't implement.

    dmsFileSys.getFileFlags
    dmsFileSys.readAsync
    dmsFileSys.writeAsync
    dmsFileSys.abortAsync
    dmsFileSys.yield
    dmsFileSys.mreadRequest
    dmsFileSys.getStatus
    dmsFileSys.clearOutstandingMReads
    dmsFileSys.setFileMode
    dmsFileSys.getItemPropsAsCab;
    dmsFileSys.canPerformOpOnItem;
	dmsFileSys.performOpOnItem;	
	dmsFileSys.rangeArrived;
	*/

	return ASFileRegisterFileSys (gExtensionID, &dmsFileSys);
}


