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

 CosDecryptDataSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** Decrypts the data with respect to the specified key.
	Input of the hex stream from the paramater manager class is 32 bit-based. This snippet makes use of the 32 bit parameter dialog input method. 
	The implication of this is you should align the data, adding padding to the end of the input steam so there is a multiple of 8 
	bytes entered. If you use the output of the CosDecryptDataSnip as input to this snippet, please copy and paste all the output. For 
	example, fabdead you should input fabdead0
	
	@testfile none
	@see CosDecryptData
*/
void CosDecryptDataSnip(ParamManager *thePM){
	string key;
	void * dataPtr=NULL;
	ASInt32 numEntries;
	thePM->getNextParamAsString(key);
	thePM->getNextParamAsHexData(&dataPtr, numEntries);

	void * dataOut = ASmalloc(numEntries+1);	
	memset(dataOut,0,numEntries+1);
	char * keyVal = static_cast<char *>(ASmalloc(key.length()+1));
	strcpy(keyVal,key.c_str());

	CosDecryptData(dataPtr, numEntries, dataOut,keyVal,key.length());
	
	Console::displayString(static_cast<char *>(dataOut));
	ASfree(dataOut);
	ASfree(keyVal);
}

SNIPRUN_REGISTER_WITH_DIALOG(CosDecryptDataSnip, "COS Layer:Encryption:Decrypt", "Pass in key and data (separated by space). This snippet \ndecrypts the passed \
in data with repect to the specified \nkey. Use the encrypt data snippet to generate data streams \nto be decoded. See the the Common SnippetRunner \nInterface for output.", 
"\"key\" 8c400d4f")


