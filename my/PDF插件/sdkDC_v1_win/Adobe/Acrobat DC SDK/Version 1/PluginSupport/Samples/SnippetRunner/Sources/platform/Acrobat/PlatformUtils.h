/*
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright (C) 2000-2006 Adobe Systems Incorporated
//  All rights reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the Adobe license agreement
//  accompanying it. If you have received this file from a source other
//  than Adobe, then your use, modification, or distribution of it
//  requires the prior written permission of Adobe.
//
*/
#ifndef _PlatformUtils_h_
#define _PlatformUtils_h_
#ifdef MAC_ENV
#ifdef PDFL_SDK_SAMPLE
#include "macUtils.h"
#endif
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "ASCalls.h"
#include <string>

using namespace std;

/** Provide simple utilities to the peddler application. Currently this is just a bunch of
	simple static methods that allow me to convert (naively) between different filesystem
	path types. 
*/
class PlatformUtils {
public:	
	/** given a path, returns the appropriate path for the platform.
	given a unix, win or mac path, this method converts the path to 
	the appropriate one for the platform. It does this using a simple 
	heuristic, '../' maps to '../', '..\' or '::'. 'a/b' maps to 
	'a/b', 'a\b' and 'a:b'. I should handle the dos style 'c:blah/yah'
	but don't.
	@param path IN the path we wish to convert
	@return the path for the platform we are running on
	*/
	static string getSysPath(string path);
private:
 	static ASBool IsUnixPath(string path);
	static ASBool IsMacPath(string path);
	static ASBool IsWinPath(string path); 
};

#endif

