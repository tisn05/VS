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
#include "SandboxPIServerDefs.h"
#include "SamplePICrossCalls.h"

// the following macro generates the server(broker) side code. The Proc function 
// gets called whenever a cross-call is made from the client(PI) side - 
// this has to be done for every cross-call-id
// DEFINE_CROSS_CALL_SERVER_START (cross-call-id, num-of-input-params, num-of-output-params)
//		virtual void Proc(input params as const reference, output params as reference)
// DEFINE_CROSS_CALL_SERVER_END

DEFINE_CROSS_CALL_SERVER_START(SamplePIBrokerDoSomeNastyStuff, 1, 1)
	virtual void Proc(const unsigned int& in1, unsigned int& out1)
	{
		 if(IDOK == ::MessageBoxW(NULL, L"Do you want to launch Calc.exe?", L"SamplePIBroker Process", MB_OKCANCEL))
		{

			STARTUPINFOW si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_NORMAL;
			PROCESS_INFORMATION proc_info = {};
			out1 = CreateProcessW(L"Calc.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &proc_info);
		}
	}
DEFINE_CROSS_CALL_SERVER_END


DEFINE_CROSS_CALL_SERVER_START(SamplePIBrokerDisplayMessage, 2, 1)
	virtual void Proc(const LPVOID& hWnd, const std::string& in1, std::string& out1)
	{
		char okselected[] = "OK button was selected.";
		char cancelselected[] = "CANCEL button was selected.";
		int button = ::MessageBoxA(static_cast<HWND>(hWnd), in1.c_str(), "SamplePI Broker", MB_OKCANCEL);
		if(button == IDOK)
		{
			out1.append(okselected);
		}
		else
		{
			out1.append(cancelselected);
		}  
	  
	}
DEFINE_CROSS_CALL_SERVER_END



// ADD_MSG_HANDLER has to be done for every cross-call which would route 
// the cross-call to actual Proc function defined above whenever a cross-call is 
// made from the client(PI) side
DEFINE_CROSS_CALL_HANDLER_START(SamplePlugin)
	ADD_MSG_HANDLER(SamplePIBrokerDoSomeNastyStuff)
	ADD_MSG_HANDLER(SamplePIBrokerDisplayMessage)
DEFINE_CROSS_CALL_HANDLER_END


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// this starts the broker service which waits for cross-call from client(PI) side
DEFINE_START_SERVER_CALL(SamplePlugin)