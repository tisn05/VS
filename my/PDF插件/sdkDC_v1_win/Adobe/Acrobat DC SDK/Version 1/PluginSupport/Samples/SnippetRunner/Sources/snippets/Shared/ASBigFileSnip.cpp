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

ASFileBigFileSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** The ASBigFileSnip snippet demonstrate how to manipulate files 
    over big in length

	@testfile none

	@see ASFileGetEOF64
	@see ASFileGetPos64
	@see ASFileSetEOF64
	@see ASFileSetPos64
	@see ASFileOpenFile64
	@see ASFileSysCreatePathName
	@see ASFileWrite
	@see ASFileSysReleasePath
	@see AVAlertNote
	@see ASFileClose
	@see ASRaise
*/

/*-----------------------------------------------------------------------
	Constant
------------------------------------------------------------------------*/
static const char * STR_BIG_FILE_NAME = "bigFile.out";
static const int SIZE_PER_WRITE = 100000;
// This will take a long time to generate the file, and no Windows application
// will be able open such a big-sized file.
//static const int NUM_WRITES = 20000;	
// For demonstration only, this will generate a relatively small size, but 
// runs much faster and can be viewed by an existing application
static const int NUM_WRITES = 2; 

/*-----------------------------------------------------------------------
	Implementation
-----------------------------------------------------------------------*/
void ASBigFileSnip()
{
	char strMsg[512];
	const ASInt32 BUF_SIZE = 256;
	char strErr[BUF_SIZE];

	DURING
		// Create an ASPathName object
		ASPathName asPathName;
		asPathName = SnippetRunnerUtils::getOutputPath(STR_BIG_FILE_NAME);
		if(asPathName == NULL) {
			AVAlertNote("Output path is not set! Exiting from ASBigFileSnip...");
			E_RTRN_VOID;
		}

		// Create the file 
		ASFile asFile;
		ASInt32 iRet = ASFileSysOpenFile64(ASGetDefaultFileSys(), asPathName, 
			ASFILE_CREATE | ASFILE_WRITE, &asFile);

		if(iRet != 0) {
			sprintf(strMsg, "Failed to create the file %s, reason: %s\n", 
				STR_BIG_FILE_NAME,
				ASGetErrorString(iRet, strErr, BUF_SIZE));
			AVAlertNote(strMsg);
			ASFileSysReleasePath(NULL, asPathName);
			E_RTRN_VOID;
		}

		Console::displayString("Created file");
		char strBuf[256];
		memset(strBuf, 0x00, sizeof(strBuf));
		strcpy(strBuf, "This is the start of the big file.");

		// Write the content in strBuf to the file
		if(ASFileWrite(asFile, strBuf, strlen(strBuf)) != strlen(strBuf)) {
			ASFileSysReleasePath(NULL, asPathName);
			ASFileClose(asFile);
			ASRaise(PDDocErrorAlways(pdErrUnableToWrite));
		}

		char *pstrBigBuf = (char *)ASmalloc(SIZE_PER_WRITE);
		if(!pstrBigBuf) {
			ASFileSysReleasePath(NULL, asPathName);
			ASFileClose(asFile);
			AVAlertNote("Failed to allocate the buffer, cannot write the big chunk of data into the file.");
			E_RTRN_VOID;
		}

		// Write a big chunk of data into the file
		// Change the values of NUM_WRITES and/or SIZE_PER_WRITE to change the size of data written into the file
		for(int j = 0; j < NUM_WRITES; j++) {
			memset(pstrBigBuf, 0x00, SIZE_PER_WRITE);
			for(int i = 0; i < SIZE_PER_WRITE; i++) {
				pstrBigBuf[i] = 65; // Latin capital letter A
			}
			if(ASFileWrite(asFile, pstrBigBuf, SIZE_PER_WRITE) != SIZE_PER_WRITE) {
				ASFileSysReleasePath(NULL, asPathName);
				ASFileClose(asFile);
				ASfree(pstrBigBuf);
				ASRaise(PDDocErrorAlways(pdErrUnableToWrite));
			}
		}
		// Free the memory
		ASfree(pstrBigBuf);

		// Close the file
		ASFileClose(asFile);
		
		Console::displayString("Wrote and closed file");

		// Open the file for more writing
		iRet = ASFileSysOpenFile64(ASGetDefaultFileSys(), asPathName, ASFILE_WRITE,
			&asFile);

		if(iRet != 0) {
			sprintf(strMsg, "Failed to open the file %s for writing, reason: %s\n", 
				STR_BIG_FILE_NAME,
				ASGetErrorString(iRet, strErr, BUF_SIZE));
			AVAlertNote(strMsg);
			ASFileSysReleasePath(NULL, asPathName);
			E_RTRN_VOID;
		}

		// Get the file size
		ASFilePos64 size = ASFileGetEOF64(asFile);

		// Seek to the end of the file
		ASFileSetPos64(asFile, size);

		// Write more data for easy identification
		memset(strBuf, 0x00, sizeof(strBuf));
		strcpy(strBuf, "This is the end of the big file.");

		if(ASFileWrite(asFile, strBuf, strlen(strBuf)) != strlen(strBuf)) {
			ASFileSysReleasePath(NULL, asPathName);
			ASFileClose(asFile);
			ASRaise(PDDocErrorAlways(pdErrUnableToWrite));
		}

		Console::displayString("Opened and wrote more data file");
		
		// Get the updated file size, truncate any data after this position by calling
		// ASFileSetEOF64()
		size = ASFileGetEOF64(asFile);

		// Write more data to the end of the file. The sentence will be removed later
		memset(strBuf, 0x00, sizeof(strBuf));
		strcpy(strBuf, "A sentence to be deleted later.");

		if(ASFileWrite(asFile, strBuf, strlen(strBuf)) != strlen(strBuf)) {
			ASFileSysReleasePath(NULL, asPathName);
			ASFileClose(asFile);
			ASRaise(PDDocErrorAlways(pdErrUnableToWrite));
		}

		// Remove the sentence "A sentence to be deleted later." 
		ASFileSetEOF64(asFile, size);

		// Close the file
		ASFileClose(asFile);

		// Release the ASPathName object
		ASFileSysReleasePath(NULL, asPathName);
		
		Console::displayString("FInished big file handling");
	HANDLER
		ASInt32 iErrCode = ASGetExceptionErrorCode();
		char strErr[BUF_SIZE];
		AVAlertNote(ASGetErrorString(iErrCode, strErr, BUF_SIZE));
	END_HANDLER
}

SNIPRUN_REGISTER(ASBigFileSnip, "AS Layer:Create Big File", 
				 "Demonstrates how to manipulate files big in length. \
This snippet will create a file called \"bigFile.out\" in the \
\"OutputFiles\" directory.")


