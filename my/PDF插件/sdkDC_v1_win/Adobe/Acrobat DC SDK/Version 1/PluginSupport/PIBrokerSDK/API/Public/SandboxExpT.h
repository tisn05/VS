/*************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2011 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains
* the property of Adobe Systems Incorporated and its suppliers,
* if any.  The intellectual and technical concepts contained
* herein are proprietary to Adobe Systems Incorporated and its
* suppliers and may be covered by U.S. and Foreign Patents,
* patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe Systems Incorporated.
**************************************************************************/

/*************************************************************************
SandboxExpT.h

 - Types, macros, structures, etc. required to use the Sandbox HFT.
**************************************************************************/
#ifndef SANDBOXEXPT_H
#define SANDBOXEXPT_H

#define MAX_PIPENAME_LEN 240			//Max pipe name length
#define NMP_DEFAULT_TIMEOUT_VALUE 2000	//DefaultTimeOut value in milliseconds for CreateNamedPipe

static const wchar_t kPipePrefix[] = L"\\\\.\\pipe\\";
static const wchar_t kPipePattern[] =  L"ko.%x.%x.%x";
static const int kPipeBufferSz = 4 * 1024;

#endif //SANDBOXEXPT_H