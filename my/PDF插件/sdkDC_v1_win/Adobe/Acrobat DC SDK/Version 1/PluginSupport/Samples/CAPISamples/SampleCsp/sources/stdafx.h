// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef stdafx_INCLUDED_
#define stdafx_INCLUDED_

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include <windows.h>
#include <WinCrypt.h>
#include <WinInet.h>
#include "cspdk.h"
#include <Rpc.h>

//#ifdef SCARD_CSP
//#include <winscard.h>
//#endif

#include <tchar.h>
#include <assert.h>
#include <iostream>
#include <strstream>
#include <sstream>
#include <fcntl.h>
#include <share.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

#include <vector>
#include <list>
#include <algorithm>

#include "sampleCommon.h"
#include "sampleTypes.h"
#include "sampleHandles.h"
#include "scspExportDefine.h"
#include "scspGlobalData.h"
#include "scspContainer.h"
#include "scspContainerFactory.h"
#include "scspHandleMgr.h"
#include "scspResources.h"

#endif //stdafx_INCLUDED_