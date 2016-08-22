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
#include "SandboxPIClientDefs.h"
#include "SamplePICrossCalls.h"

//-------------------------------------------------------------------------
// This file contains the definitions for all the CrossCallAPIs implemented
// by the SamplePI plugin 
//-------------------------------------------------------------------------

// the following macro generates the client side code which are required to 
// do a cross call - this has to be done for every cross-call-id
// DEFINE_CROSS_CALL_CLIENT (cross-call-id, num-of-input-params, num-of-output-params)
DEFINE_CROSS_CALL_CLIENT(SamplePIBrokerDoSomeNastyStuff, 1, 1); 


DEFINE_CROSS_CALL_CLIENT(SamplePIBrokerDisplayMessage, 2, 1);


// following function needs to be defined and is reserved as of now
 bool StartSandboxPIPluginServer() {
	return false; //We simply return false.
}

namespace sampleipc_remote {

	bool __stdcall SamplePIBrokerDoSomeNastyStuff() {
		unsigned int unusedIn, unusedOut;

		// this makes the cross-call
		MAKE_CROSS_CALL(SamplePIBrokerDoSomeNastyStuff)(unusedIn, unusedOut);

		return true;
	}

	bool __stdcall SamplePIBrokerDisplayMessage(HWND hWnd, const std::string& inMessage, std::string& outString) {
		const ipc::ByteArray inStr(inMessage.size(), inMessage.c_str());
		ipc::ByteArray outArray;
		bool return_val = false;

		// following macro is used to make cross-calls -- if return_val is not required which indicates success or failure
		// of the cross-call then MAKE_CROSS_CALL macro can be used.
		MAKE_CROSS_CALL_WITH_MSG_PUMP_AND_RETURN_VAL(SamplePIBrokerDisplayMessage, return_val)(static_cast<const LPVOID>(hWnd), inMessage, outString);
		
		return return_val;

	}

} //namespace sampleipc_remote