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
\file RplcFileSystem.cpp

 - Implements a replacement file system.

 - The implementation is basically a wrapper around the default file system.
   The point is to demonstrate the mechanism for adding a new file system
   without becoming bogged down with complex implementation considerations.

*********************************************************************/

// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif
#ifndef UNIX_PLATFORM
#endif

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/

ASFileSysRec fileSys;
static ASFileSys defFileSys;
static ASAtom rplcFileSys_K;

/*-------------------------------------------------------
	UI Callbacks
-------------------------------------------------------*/

/* OpenThroughRplcFileSystem
** ------------------------------------------------------
** */ 
/** Prompt the user to select a PDF document and open
 ** it through our replacement file system.
 ** @see ASTextSetEncoded
 ** @see ASScriptToHostEncoding
 ** @see AVAppOpenDialog
 ** @see AVDocOpenFromFile
 ** @see ASGetErrorString
 ** @see ASGetExceptionErrorCode
 ** @see AVAlertNote
 ** @see ASTextDestroy
 ** @see ASFileSysReleasePath
 ** */
ACCB1 void ACCB2 OpenThroughRplcFileSystem (void *clientData)
{
	AVOpenSaveDialogParamsRec dialogParams;
	AVFileFilterRec filterRec, *filterRecP;
	// Set up the PDF file filter description.
	AVFileDescRec descRec[2] = {{"pdf", 0, 0}, {"", 'PDF ', 0}};
	ASPathName * volatile pathName = NULL;
	char errorBuf[256];

	memset (&filterRec, 0, sizeof(AVFileFilterRec));
	filterRec.fileDescs = &descRec[0];
	filterRec.numFileDescs = 2;
	filterRecP = &filterRec;

	// Configure the dialog box parameters.
	memset (&dialogParams, 0, sizeof (AVOpenSaveDialogParamsRec));
	dialogParams.size = sizeof(AVOpenSaveDialogParamsRec);
	dialogParams.fileFilters = &filterRecP;
	dialogParams.numFileFilters = 1;

	dialogParams.flags = kAVOpenSaveAllowForeignFileSystems;
	dialogParams.initialFileSys = (ASFileSys)&fileSys;

	DURING

		// The following methods can raise so we wrap them within
		// the exception handling macros.
		char* strFilterDesc = "Adobe PDF Files";
		filterRec.filterDescription = ASTextFromUnicode(reinterpret_cast <ASUTF16Val *>(strFilterDesc), 
			kUTF8);

		char* strWindowTitle = "Select PDF Document To Open";
		dialogParams.windowTitle = ASTextFromUnicode(reinterpret_cast <ASUTF16Val *>(strWindowTitle), 
			kUTF8);

		ASFileSys outFileSys = NULL;
		// Pop the dialog.
		if (AVAppOpenDialog(&dialogParams, &outFileSys, (ASPathName**)&pathName, NULL, NULL))
			AVDocOpenFromFile (*pathName, &fileSys, NULL);
	HANDLER
		// Display error message to user.
		memset(errorBuf, 0x00, 256);
		ASGetErrorString (ASGetExceptionErrorCode(), errorBuf, 256);
		AVAlertNote (errorBuf);
	END_HANDLER

	// We retain ownership of the ASText objects so we have to destroy them.
	ASTextDestroy (filterRec.filterDescription);
	ASTextDestroy (dialogParams.windowTitle);

	if (pathName)
		ASFileSysReleasePath (NULL, *pathName);
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
static ACCB1 ASInt32 ACCB2 MyASFileSysOpenProc (ASPathName pathName, ASUns16 mode, MDFile *fP)
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
static ACCB1 ASSize_t ACCB2 MyASFileSysReadProc (void *ptr, ASSize_t size, ASSize_t count, MDFile f, ASInt32 *pError)
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
static ACCB1 ASSize_t ACCB2 MyASFileSysWriteProc (void *ptr, ASSize_t size, ASSize_t count, MDFile f, ASInt32 *pError)
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
static ACCB1 ASInt32 ACCB2 MyASFileSysRenameProc (ASMDFile* f, ASPathName oldPath, ASPathName newPath)
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
static ACCB1 ASBool ACCB2 MyASFileSysIsSameFileProc (ASMDFile f, ASPathName pathName, ASPathName newPathName)
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
static ACCB1 ASInt32 ACCB2 MyASFileSysGetNameProc (ASPathName pathName, char *name, ASInt32 maxLength)
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
static ACCB1 ASInt32 ACCB2 MyASFileSysGetNameAsASTextProc (ASPathName pathName, ASText name)
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
static ACCB1 ASPathName ACCB2 MyASFileSysGetTempPathNameProc (ASPathName pathName)
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
static ACCB1 ASPathName ACCB2 MyASFileSysCopyPathNameProc (ASPathName pathName)
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
static ACCB1 char * ACCB2 MyASFileSysDiPathFromPathProc (ASPathName path, ASPathName relativeToThisPath)
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
static ACCB1 ASPathName ACCB2 MyASFileSysPathFromDIPathProc (const char * diPath, ASPathName relativeToThisPath)
{
	return defFileSys->pathFromDIPath (diPath, relativeToThisPath);
}

/* MyASFileSysDisposePathNameProc
** ------------------------------------------------------
** */ 
/** Frees the ASPathName object. Does not do file-level operations.
*/
static ACCB1 void ACCB2 MyASFileSysDisposePathNameProc (ASPathName pathName)
{
	defFileSys->disposePathName (pathName);
	return;
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
	return rplcFileSys_K;
}

/* MyASFileSysGetStorageFreeSpaceProc
** ------------------------------------------------------
** */ 
/** Gets the free space on the same volume as the pathname
 **
 ** @return the free space in bytes. The upper limit is 4 GB, because it returns an ASInt32
 ** */
static ACCB1 ASUns32 ACCB2 MyASFileSysGetStorageFreeSpaceProc (ASPathName pathName)
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
static ACCB1 ASInt32 ACCB2 MyASFileSysFlushVolumeProc (ASPathName pathName)
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
static ACCB1 ASPathName ACCB2 MyASFileSysCreatePathNameProc (ASAtom pathSpecType, const void *pathSpec, const void *mustBeZero)
{
	return defFileSys->createPathName (pathSpecType, pathSpec, mustBeZero);
}

/* MyASFileSysGetItemPropsProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysGetItemPropsProc (ASPathName pathName, ASFileSysItemProps props)
{
	return defFileSys->getItemProps (pathName, props);
}

/* MyASFileSysFirstFolderItemProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 ASFolderIterator ACCB2 MyASFileSysFirstFolderItemProc (ASPathName folderPath, ASFileSysItemProps props, ASPathName *itemPath)
{
	return defFileSys->firstFolderItem (folderPath, props, itemPath);
}

/* MyASFileSysNextFolderItemProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 ASBool ACCB2 MyASFileSysNextFolderItemProc (ASFolderIterator folderIter, ASFileSysItemProps props, ASPathName *itemPath)
{
	return defFileSys->nextFolderItem (folderIter, props, itemPath);
}

/* MyASFileSysDestroyFolderIteratorProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 void ACCB2 MyASFileSysDestroyFolderIteratorProc (ASFolderIterator folderIter)
{
	defFileSys->destroyFolderIterator (folderIter);
}

/* MyASFileSysURLFromPathProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 char * ACCB2 MyASFileSysURLFromPathProc (ASPathName path)
{
	return defFileSys->urlFromPath (path);
}

/* MyASFileSysGetParentProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 ASPathName ACCB2 MyASFileSysGetParentProc (ASPathName path)
{
	return defFileSys->getParent (path);
}

/* MyASFileSysCreateFolderProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysCreateFolderProc (ASPathName path)
{
	return defFileSys->createFolder (path);
}

/* MyASFileSysRemoveFolderProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysRemoveFolderProc (ASPathName path)
{
	return defFileSys->removeFolder (path);
}

/* MyASFileSysDisplayStringFromPathProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 char * ACCB2 MyASFileSysDisplayStringFromPathProc (ASPathName path)
{
	return defFileSys->displayStringFromPath (path);
}

/* MyASFileSysSetTypeAndCreatorProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 void ACCB2 MyASFileSysSetTypeAndCreatorProc (ASPathName path, ASUns32 type, ASUns32 creator)
{
#if MAC_PLATFORM
	defFileSys->setTypeAndCreator (path, type, creator);
#endif
}

/* MyASFileSysGetTypeAndCreatorProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 void ACCB2 MyASFileSysGetTypeAndCreatorProc (ASPathName path, ASUns32 *type, ASUns32 *creator)
{
#if MAC_PLATFORM
	defFileSys->getTypeAndCreator (path, type, creator);
#endif
}

/* MyASFileSysAcquirePlatformPathProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 ASInt32 ACCB2 MyASFileSysAcquirePlatformPathProc
	(ASPathName path, ASAtom platformPathType, ASPlatformPath *platformPath)
{
	return defFileSys->acquirePlatformPath (path, platformPathType, platformPath);
}

/* MyASFileSysReleasePlatformPathProc
** ------------------------------------------------------
**
**
**
**
*/
static ACCB1 void ACCB2 MyASFileSysReleasePlatformPathProc
	(ASPlatformPath platformPath)
{
	return defFileSys->releasePlatformPath (platformPath);
}
/* InitializeFileSystem
** ------------------------------------------------------
** */ 
/** Sets up all the callbacks for the file system.
 **
 ** @return true if the file system was registered
 ** successfully.
 ** */
ASBool InitializeFileSystem ()
{
	defFileSys = ASGetDefaultFileSys();
	rplcFileSys_K = ASAtomFromString("ADBE:RplcFileSys");

	memset (&fileSys, 0, sizeof(ASFileSysRec));

	fileSys.size = sizeof(ASFileSysRec);

	fileSys.open = ASCallbackCreateProto (ASFileSysOpenProc, MyASFileSysOpenProc);
	fileSys.close = ASCallbackCreateProto (ASFileSysCloseProc, MyASFileSysCloseProc);
	fileSys.flush = ASCallbackCreateProto (ASFileSysFlushProc, MyASFileSysFlushProc);
	fileSys.setpos = ASCallbackCreateProto (ASFileSysSetPosProc, MyASFileSysSetPosProc);
	fileSys.getpos = ASCallbackCreateProto (ASFileSysGetPosProc, MyASFileSysGetPosProc);
	fileSys.seteof = ASCallbackCreateProto (ASFileSysSetEofProc, MyASFileSysSetEofProc);
	fileSys.geteof = ASCallbackCreateProto (ASFileSysGetEofProc, MyASFileSysGetEofProc);
	fileSys.read = ASCallbackCreateProto (ASFileSysReadProc, MyASFileSysReadProc);
	fileSys.write = ASCallbackCreateProto (ASFileSysWriteProc, MyASFileSysWriteProc);
	fileSys.remove = ASCallbackCreateProto (ASFileSysRemoveProc, MyASFileSysRemoveProc);
	fileSys.rename = ASCallbackCreateProto (ASFileSysRenameProc, MyASFileSysRenameProc);
	fileSys.isSameFile = ASCallbackCreateProto (ASFileSysIsSameFileProc, MyASFileSysIsSameFileProc);
	fileSys.getName = ASCallbackCreateProto (ASFileSysGetNameProc, MyASFileSysGetNameProc);
	fileSys.getNameAsASText = ASCallbackCreateProto (ASFileSysGetNameAsASTextProc, MyASFileSysGetNameAsASTextProc);
	fileSys.getTempPathName = ASCallbackCreateProto (ASFileSysGetTempPathNameProc, MyASFileSysGetTempPathNameProc);
	fileSys.copyPathName = ASCallbackCreateProto (ASFileSysCopyPathNameProc, MyASFileSysCopyPathNameProc);
	fileSys.diPathFromPath = ASCallbackCreateProto (ASFileSysDiPathFromPathProc, MyASFileSysDiPathFromPathProc);
	fileSys.pathFromDIPath = ASCallbackCreateProto (ASFileSysPathFromDIPathProc, MyASFileSysPathFromDIPathProc);
	fileSys.disposePathName = ASCallbackCreateProto (ASFileSysDisposePathNameProc, MyASFileSysDisposePathNameProc);
	fileSys.getFileSysName = ASCallbackCreateProto (ASFileSysGetFileSysNameProc, MyASFileSysGetFileSysNameProc);
	fileSys.getStorageFreeSpace = ASCallbackCreateProto (ASFileSysGetStorageFreeSpaceProc, MyASFileSysGetStorageFreeSpaceProc);
	fileSys.flushVolume = ASCallbackCreateProto (ASFileSysFlushVolumeProc, MyASFileSysFlushVolumeProc);
	fileSys.createPathName = ASCallbackCreateProto (ASFileSysCreatePathNameProc, MyASFileSysCreatePathNameProc);

	fileSys.getItemProps = ASCallbackCreateProto (ASFileSysGetItemPropsProc, MyASFileSysGetItemPropsProc);
	fileSys.firstFolderItem = ASCallbackCreateProto (ASFileSysFirstFolderItemProc, MyASFileSysFirstFolderItemProc);
	fileSys.nextFolderItem = ASCallbackCreateProto (ASFileSysNextFolderItemProc, MyASFileSysNextFolderItemProc);
	fileSys.destroyFolderIterator = ASCallbackCreateProto (ASFileSysDestroyFolderIteratorProc, MyASFileSysDestroyFolderIteratorProc);
	fileSys.urlFromPath = ASCallbackCreateProto (ASFileSysURLFromPathProc, MyASFileSysURLFromPathProc);
	fileSys.getParent = ASCallbackCreateProto (ASFileSysGetParentProc, MyASFileSysGetParentProc);
	fileSys.createFolder = ASCallbackCreateProto (ASFileSysCreateFolderProc, MyASFileSysCreateFolderProc);
	fileSys.removeFolder = ASCallbackCreateProto (ASFileSysRemoveFolderProc, MyASFileSysRemoveFolderProc);
	fileSys.displayStringFromPath = ASCallbackCreateProto (ASFileSysDisplayStringFromPathProc, MyASFileSysDisplayStringFromPathProc);
	fileSys.setTypeAndCreator = ASCallbackCreateProto (ASFileSysSetTypeAndCreatorProc, MyASFileSysSetTypeAndCreatorProc);
	fileSys.getTypeAndCreator = ASCallbackCreateProto (ASFileSysGetTypeAndCreatorProc, MyASFileSysGetTypeAndCreatorProc);

	//These need to be implemented for 7.0 Windows
	fileSys.acquirePlatformPath = ASCallbackCreateProto (ASFileSysAcquirePlatformPathProc, MyASFileSysAcquirePlatformPathProc);
	fileSys.releasePlatformPath = ASCallbackCreateProto (ASFileSysReleasePlatformPathProc, MyASFileSysReleasePlatformPathProc);
	
	/* The following are other callbacks we don't implement.

	fileSys.getFileFlags
	fileSys.readAsync
	fileSys.writeAsync
	fileSys.abortAsync
	fileSys.yield
	fileSys.mreadRequest
	fileSys.getStatus
	fileSys.acquireFileSysPath
	fileSys.clearOutstandingMReads
	fileSys.setFileMode
	fileSys.reopen
	fileSys.hardFlush
	fileSys.getItemPropsAsCab;
	fileSys.canPerformOpOnItem;
	fileSys.performOpOnItem;
	
	fileSys.displayASTextFromPath;
	fileSys.rangeArrived;
	fileSys.canSetEof;
	fileSys.diPathFromPathEx;
	fileSys.pathFromDIPathEx;
	fileSys.getfileposlimit;
	fileSys.open64;
	fileSys.setpos64;
	fileSys.getpos64;
	fileSys.seteof64;
	fileSys.geteof64;
	fileSys.getNameForDisplay;
	fileSys.ASFileSysGetStorageFreeSpace64Proc
	*/

	return ASFileRegisterFileSys (gExtensionID, &fileSys);
}

/* UninitializeFileSystem
** ------------------------------------------------------
*/
void UninitializeFileSystem()
{
	ASFileUnregisterFileSys (gExtensionID, &fileSys);
}
