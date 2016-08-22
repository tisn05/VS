/*************************************************************************
* WARNING: All additions to this file must go at the end of the file.
* This file is #included to build a table of function pointers (an HFT).
* All components which make use of this HFT rely on the order and
* positioning of these functions (e.g. plug-ins). Adding entries in the 
* middle of this file or modifying existing entries will cause these 
* components to break.
**************************************************************************/

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

/*
** Functions exported by the SandboxHFT.
*/

/**
	Returns true iff the application is running in sandboxed mode
*/
NPROC(ASBool, SbxAppGetIsSandboxed, (void))
/*
	Launches the plug-in broker process and returns the NamedPipe name.
	This is used by plug-ins implementing broker extensibility.
	@param piBroker IN Name of the piBroker process including the extension, example FeatBroker.exe
	@param brokerPipeName OUT Receives the named pipe name for Broker owned pipe
	@since PI_SANDBOX_VERSION >= 0x00010000
*/
NPROC(HANDLE, SbxLaunchPIBrokerExtn, (const wchar_t* piBroker, wchar_t (*brokerPipeName)[MAX_PIPENAME_LEN]))


/*
	Launches the plug-in broker process and returns valid handle to the named pipe in case of success
	otherwise INVALID_HANDLE_VALUE.
	This is used by plug-ins implementing broker extensibility.
	@param piBroker IN Name of the piBroker process including the extension, example FeatBroker.exe
	@since PI_SANDBOX_VERSION >= 0x00020000
*/
NPROC(HANDLE, SbxLaunchPIBrokerExe, (const wchar_t* piBroker))