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
#ifndef SADNBOXPICLIENTDEFS_H
#define SADNBOXPICLIENTDEFS_H

#include "SandboxPIIPCDefs.h"

class SandboxPIServiceManager;

namespace sandboxpisdk {
	bool InitSandboxPIBrokerIPCChannel(const std::wstring &piBroker);
	bool InitSandboxPIPluginIPCChannel(const std::wstring &piBroker);
	bool TerminateSandboxPIBroker();
	bool ReleaseByteArray(ipc::ByteArray& ba);
}

class SandboxPIClient {
public:
	SandboxPIClient()
		:clt_thread_(NULL)
	{
		pisdk_init_pass_event_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		pisdk_init_fail_event_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	~SandboxPIClient() {
		if(clt_thread_)
			clt_thread_ = NULL;
		::CloseHandle(pisdk_init_pass_event_);
		::CloseHandle(pisdk_init_fail_event_);
	}

  template <typename ServiceManagerT>
  bool StartClientService();

private:

  template <typename ServiceManagerT>
  static DWORD __stdcall SandboxPIClientService(void* p);

  template <typename ServiceManagerT>
  DWORD ServiceThread();
  
  bool EnterPISDKInitResponseLoop(){
	MSG msg;
	HANDLE handles[2] = {pisdk_init_pass_event_, pisdk_init_fail_event_};
	while(true)
	{
		switch(::MsgWaitForMultipleObjects(2, handles, FALSE, INFINITE, QS_ALLINPUT)) {
			case WAIT_OBJECT_0:
				return true;
			case WAIT_OBJECT_0+1:
				return false;
			case (WAIT_OBJECT_0+2):
				{
					// we have a message - peek and dispatch it 
					// Multiple messages may be received together. So process all of them.
					while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE|PM_NOYIELD)) {
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
				}
				break;
			default:
				return false;
		}
	}
  }

  void ExitPISDKInitResponseLoop(HANDLE pisdk_init_event){
	  ::SetEvent(pisdk_init_event);
  }

  HANDLE clt_thread_;
  HANDLE pisdk_init_pass_event_;
  HANDLE pisdk_init_fail_event_;
};

template <typename ServiceManagerT>
bool SandboxPIClient::StartClientService()
{
	bool retVal = false;

	//return false if the service has already started for one named pipe
	if(!clt_thread_)
	{
		DWORD tID = 0;
		::ResetEvent(pisdk_init_pass_event_);
		::ResetEvent(pisdk_init_fail_event_);
		clt_thread_ = ::CreateThread(NULL, NULL, SandboxPIClientService<ServiceManagerT>, this, 0, &tID);
		if (clt_thread_) {
			if(EnterPISDKInitResponseLoop())
				retVal = true;
		}
	}
	return retVal;
}

// static methods
template <typename ServiceManagerT>
DWORD __stdcall SandboxPIClient::SandboxPIClientService(void* p) {
  SandboxPIClient* clt = reinterpret_cast<SandboxPIClient*>(p);
  return clt->ServiceThread<ServiceManagerT>();
}

template <typename ServiceManagerT>
DWORD SandboxPIClient::ServiceThread()
{
	HANDLE clt_pipe = sandboxpisdk::SandboxPISDKGetHandleToPluginIPCChannel();
	if(INVALID_HANDLE_VALUE == clt_pipe)
	{
		::OutputDebugStringA("!! clt:SandboxPISDKGetHandleToPluginIPCChannel");
		ExitPISDKInitResponseLoop(pisdk_init_fail_event_);
		return 1;
	}

	// Once passed into OpenServer, clt_pipe is now owned by PipeTransportWithMessagePump. Don't close it.
	PipeTransportWithMessagePump transport;
	if (!transport.OpenServer(clt_pipe, true)) {
		ExitPISDKInitResponseLoop(pisdk_init_fail_event_);
		return 1;
	}
	SandboxPIChannelT channel(&transport);
	if (!transport.IsConnected()) {
		ExitPISDKInitResponseLoop(pisdk_init_fail_event_);
		return 2;
	}

	sandboxpisdk::SandboxPIStartServiceManager<ServiceManagerT>(&transport);
	ExitPISDKInitResponseLoop(pisdk_init_pass_event_);

	MsgDispatch top_dispatch;
	channel.Receive(&top_dispatch);
	sandboxpisdk::SandboxPIStopServiceManager();
	return 0;
}

#endif //SADNBOXPICLIENTDEFS_H