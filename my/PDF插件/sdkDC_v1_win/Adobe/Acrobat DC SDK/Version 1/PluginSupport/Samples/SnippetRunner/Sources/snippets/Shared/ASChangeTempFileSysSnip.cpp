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

ASChangeTempFileSysSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** The ASChangeTempFileSysSnip snippet demonstrate how to change the
    temporary file system implementation for a platform.

	@testfile none
	@see ASGetRamFileSys
	@see ASGetTempFileSys
	@see ASSetTempFieSys
	@see ASRamFileSysSetLimitKB
*/

/*-----------------------------------------------------------------------
	Constant
------------------------------------------------------------------------*/
static const ASInt32 RAMFILESYS_LIMIT = 100000;	// 100 MB

/*-----------------------------------------------------------------------
	Implementation
-----------------------------------------------------------------------*/

void ASChangeTempFileSysSnip()
{
	char strBuf[256];

	// Get the temporary file system implementation for the platform
	ASFileSys tmpFileSys = ASGetTempFileSys();

	// Get the name of the temporary fie system implementation
	string strTmpFsName = ASAtomGetString(tmpFileSys->getFileSysName());

	sprintf(strBuf, "The default temporary file system is: %s\n", strTmpFsName.c_str());
	Console::displayString(strBuf);

	Console::displayString("Change the temporary file system to in-memory file system...\n");

	// Get the in-memory file system implementation for the platform
	ASFileSys imFileSys = ASGetRamFileSys();

	// Change the ram file limit
	ASRamFileSysSetLimitKB(RAMFILESYS_LIMIT);

	// Set the temporary file system to in-memory file system
	ASSetTempFileSys(imFileSys);

	// Get the new temporary file system implementation for the platform
	ASFileSys newTmpFileSys = ASGetTempFileSys();

	// Get the name of the new temporary file system implementation
	string strNewTmpFsName = ASAtomGetString(newTmpFileSys->getFileSysName());

	sprintf(strBuf, "The new temporary file system is: %s\n", strNewTmpFsName.c_str());
	Console::displayString(strBuf);

	sprintf(strBuf, "Change the temporary file system back to %s...\n", strTmpFsName.c_str());
	Console::displayString(strBuf);

	ASSetTempFileSys(tmpFileSys);
}

SNIPRUN_REGISTER(ASChangeTempFileSysSnip, "AS Layer:Change Temporary File System", 
				 "Demonstrates how to change the temporary file system \
implementation for a platform.")


