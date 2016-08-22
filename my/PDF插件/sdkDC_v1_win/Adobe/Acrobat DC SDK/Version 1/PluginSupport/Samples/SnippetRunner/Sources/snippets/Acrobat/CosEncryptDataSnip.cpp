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

 CosEncryptDataSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** Encrypts the data passed in with respect to the specified key. the output (to debug window) makes use ofthe 32 bit based memory
	method provided by SnippetRunnerUtils. If you enter five characters, there will be two 32 bit hex values in debug window. If you
	then decrypt the stream using CosDecryptDataSnip, you should cut and paste the whole output.
	@testfile none
	@see CosEncryptData
*/
void CosEncryptDataSnip(ParamManager *thePM){
	string key,dataStream;
	thePM->getNextParamAsString(key);
	thePM->getNextParamAsString(dataStream);
	ASInt32 strLen = dataStream.length();
	void * dataOut = ASmalloc(strLen+1);	
	char * keyVal = static_cast<char *>(ASmalloc(key.length()+1));
	strcpy(keyVal,key.c_str());
	memcpy(dataOut,dataStream.c_str(),strLen+1);
	
	// we encrypt the NULL string terminator as well, for simplicity.
	CosEncryptData(dataOut, strLen+1, dataOut,keyVal, key.length());
	Console::displayString(dataOut,(strLen-1)/4+1);
	ASfree(dataOut);
	ASfree(keyVal);
}

SNIPRUN_REGISTER_WITH_DIALOG(CosEncryptDataSnip, "COS Layer:Encryption:Encrypt", "Pass in key and data (separated by space). This snippet \nencrypts the \
passed in data with respect to the specified \nkey. \
See the Common SnippetRunner Interface for output.","\"key\" \"Data\"")


