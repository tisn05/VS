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
* suppliers and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe Systems Incorporated.
**************************************************************************/

#include <windows.h>

extern int StartSandboxPIServer(HINSTANCE instance, const wchar_t* cmdline);

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE, wchar_t*, int) {
  // This process is the plugin broker process
  const wchar_t* cmdline = ::GetCommandLineW();
  return StartSandboxPIServer(instance, cmdline);
}

