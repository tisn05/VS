// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <comdef.h>

#ifndef __INTELLISENSE__
#import "libid:95E23C91-BC5A-49F3-8CD1-1FC515597048" version("11.5") \
      rename("GetCommandLine", "VGGetCommandLine") \
      rename("CopyFile", "VGCore") \
      rename("FindWindow", "VGFindWindow")
#else
#include "Debug/VGCoreAuto.tlh"
#endif
