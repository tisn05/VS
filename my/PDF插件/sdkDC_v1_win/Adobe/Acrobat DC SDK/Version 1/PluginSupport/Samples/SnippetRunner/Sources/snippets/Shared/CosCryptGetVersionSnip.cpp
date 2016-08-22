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

 CosCryptGetVersionSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/** Reports the version of the current encryption algorithm.
	@testfile none
	@see CosCryptGetVersion
	@see CosDecryptGetMaxKeyBytes
	@see CosEncryptGetMaxKeyBytes
*/
void CosCryptGetVersionSnip(void){
	ASInt32 versionNumber = CosCryptGetVersion();
	ASInt32 decryptMaxKeyBytes = CosDecryptGetMaxKeyBytes(versionNumber);
	ASInt32 encryptMaxKeyBytes = CosEncryptGetMaxKeyBytes(versionNumber);
	char buffer[100];
	snprintf(buffer,sizeof(buffer),"Version: %d. Maximum decryption key size: %d bytes. Maximum encryption key size: %d bytes.",versionNumber,decryptMaxKeyBytes,encryptMaxKeyBytes);
	Console::displayString(buffer);
}

SNIPRUN_REGISTER(CosCryptGetVersionSnip, "COS Layer:Encryption:Get Version", "Reports the version number of the encryption algorithm, along \
with the maximum size of the encryption/decryption keys")


