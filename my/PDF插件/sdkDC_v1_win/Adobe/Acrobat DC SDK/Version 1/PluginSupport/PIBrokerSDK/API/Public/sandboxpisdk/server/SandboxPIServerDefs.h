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
#ifndef SADNBOXPISERVERDEFS_H
#define SADNBOXPISERVERDEFS_H

#include "SandboxPIIPCDefs.h"
#include <new>

class SandboxPIServer {
public:
	SandboxPIServer()
		:svc_thread_(NULL)
	{
		pisdk_init_pass_event_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		pisdk_init_fail_event_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	~SandboxPIServer() {
		if(svc_thread_)
		{
			svc_thread_ = NULL;
		}
		::CloseHandle(pisdk_init_pass_event_);
		::CloseHandle(pisdk_init_fail_event_);
	}

	template <typename ServiceManagerT>
	bool StartBrokerService(const wchar_t* cmdline);

private:
	struct Context {
		SandboxPIServer* me;
		HANDLE  svc_pipe;
		Context(SandboxPIServer* ob, HANDLE pipe)
			: me(ob), svc_pipe(pipe) {}
	};

	template <typename ServiceManagerT>
	static DWORD __stdcall SandboxPIBrokerService(void* p);

	template <typename ServiceManagerT>
	DWORD ServiceThread(Context* ctx);

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

	HANDLE svc_thread_;
	HANDLE pisdk_init_pass_event_;
	HANDLE pisdk_init_fail_event_;
};

template <typename ServiceManagerT>
bool SandboxPIServer::StartBrokerService(const wchar_t* cmdline) 
{
	bool retVal = false;

	Context* pCtxt = NULL;
	HANDLE parent_process = NULL;
	HANDLE ipc_server = INVALID_HANDLE_VALUE;

	std::wstring cmd_line(cmdline);
	size_t pipepos = cmd_line.find(kCmdLinePipeEq, 0);

	if (std::wstring::npos != pipepos) {

		pipepos += 7;
		size_t procpos = cmd_line.find(kCmdLineProcEq, pipepos);

		if (std::wstring::npos != procpos) {

			std::wstring pipename(kPipePrefix);
			pipename.append(cmd_line.substr(pipepos, (procpos-pipepos)));
			procpos += 8;
			std::wstring procID(cmd_line.substr(procpos));
			DWORD processID = _wtol(procID.c_str());

			parent_process = ::OpenProcess(SYNCHRONIZE, FALSE, processID);

			if(parent_process) {

				ipc_server = ::CreateNamedPipeW(pipename.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE | FILE_FLAG_OVERLAPPED,
					PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
					1, kPipeBufferSz, kPipeBufferSz, NMP_DEFAULT_TIMEOUT_VALUE, NULL);

				if(INVALID_HANDLE_VALUE != ipc_server)
				{
					if((pCtxt = new (std::nothrow) Context(this, ipc_server)) != NULL)
					{
						DWORD tID = 0;
						::ResetEvent(pisdk_init_pass_event_);
						::ResetEvent(pisdk_init_fail_event_);
						svc_thread_ = ::CreateThread(NULL, NULL, SandboxPIBrokerService<ServiceManagerT>, pCtxt, 0, &tID);
						if (svc_thread_) {
							// The ServiceThread takes ownership of the ipc_server handle.
							// So not close it if the thread creation succeeded.
							ipc_server = INVALID_HANDLE_VALUE;
							if(EnterPISDKInitResponseLoop())
								retVal = true;
						}
					}
				}
			}
		}
	}
	if(retVal)
	{
		HANDLE handles[2] = {parent_process, svc_thread_};
		switch(::WaitForMultipleObjects (2, handles, FALSE, INFINITE))
		{
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0 + 1:
			retVal = true;
			break;
		default:
			retVal = false;
			break;
		}
	}
	if(ipc_server != INVALID_HANDLE_VALUE) {
		::CloseHandle(ipc_server);
	}
	if(parent_process) {
		::CloseHandle(parent_process);
	}
	if(pCtxt)
	{
		delete pCtxt;
		pCtxt = NULL;
	}
	return retVal;
}

// static method
template <typename ServiceManagerT>
DWORD __stdcall SandboxPIServer::SandboxPIBrokerService(void* p) {
  Context* ctx = reinterpret_cast<Context*>(p);
  return ctx->me->ServiceThread<ServiceManagerT>(ctx);
}

template <typename ServiceManagerT>
DWORD SandboxPIServer::ServiceThread(Context* ctx) {
  PipeTransportWithMessagePump transport;
  if (!transport.OpenServer(ctx->svc_pipe, true)) {
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

#endif //SADNBOXPISERVERDEFS_H