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
#ifndef SAMPLEPICROSSCALLS_H
#define SAMPLEPICROSSCALLS_H

/*
* This file declares the prototypes for the extended APIS.
* Each API makes a cross call to the PIBroker process.
* Each API is governed by a pair of enums, k<CrossCallAPI>Send and k<CrossCallAPI>Recv
*/
#include "SandboxPISDKClientMacros.h"
#include "SandboxPISDKServerMacros.h"
#include <string>

// define the IDs for every cross-call here

DECLARE_CROSS_CALL_START
	CROSS_CALL_ID(SamplePIBrokerDoSomeNastyStuff)
	CROSS_CALL_ID(SamplePIBrokerDisplayMessage)
DECLARE_CROSS_CALL_END



// define the input and output parameters for every cross call
DEFINE_IPC_MSG_CLIENT(SamplePIBrokerDoSomeNastyStuff, 1) {
	INPUT_PARAM_UINT(1) // unused - dummy parameter
};

DEFINE_IPC_MSG_SERVER(SamplePIBrokerDoSomeNastyStuff, 1) {
	OUTPUT_PARAM_UINT(1) // unused - dummy parameter
};


DEFINE_IPC_MSG_CLIENT(SamplePIBrokerDisplayMessage, 2) {
	INPUT_PARAM_VOID_PTR(1)
	INPUT_PARAM_STRING(2) // input string to be displayed by broker
};

DEFINE_IPC_MSG_SERVER(SamplePIBrokerDisplayMessage, 1) {
	OUTPUT_PARAM_STRING(1)
};




//Declare your Broker APIs in some namspace like <plugin>ipc_remote
//These functions are implemented in SamplePIClient.cpp
namespace sampleipc_remote {
bool __stdcall SamplePIBrokerDoSomeNastyStuff();
bool __stdcall SamplePIBrokerDisplayMessage(HWND hWnd, const std::string& inMessage, std::string& outMessage);
}

#endif //SAMPLEPICROSSCALLS_H