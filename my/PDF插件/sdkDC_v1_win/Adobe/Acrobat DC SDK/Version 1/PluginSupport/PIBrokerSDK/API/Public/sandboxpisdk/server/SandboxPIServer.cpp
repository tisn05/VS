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

#include "SandboxPIIPCDefs.h"

SandboxPITransportT* g_ipc_transport = NULL;
SandboxPIServiceManager* g_service_manager = NULL;

SandboxPITransportT* SbxGetBrokerTransport() {
	static SandboxPITransportT broker_transport;
	return &broker_transport;
}

DEFINE_IPC_MSG_CONV(kSbxPISDKGetHandleToPluginIPCChannelSend, 1) {
	IPC_MSG_P1(DWORD, ULong32)            // targetPID to duplicate pipe handle
};

class SbxPISDKGetHandleToPluginIPCChannelSvc : public SandboxPISvcBase,
	public ipc::MsgIn<kSbxPISDKGetHandleToPluginIPCChannelSend, SbxPISDKGetHandleToPluginIPCChannelSvc, SandboxPIChannelT> { 
public:
	INTERNAL_MSG_REFLECT(SandboxPIChannelT)

		size_t OnMsg(SandboxPIChannelT* ch,
		DWORD targetPID) {

			static LONG g_pipe_seq = 0;
			DWORD gle = 0;

			wchar_t name[8*3 + sizeof(kPipePattern)];
			::wsprintfW(name, kPipePattern, ::GetCurrentProcessId(), ::GetTickCount(), 
				::InterlockedIncrement(&g_pipe_seq));

			std::wstring pipename(kPipePrefix);
			pipename.append(name);

			HANDLE target_pipe = INVALID_HANDLE_VALUE;
			HANDLE target_process = OpenProcess(PROCESS_DUP_HANDLE, 0, targetPID);
			if(target_process)
			{
				SECURITY_ATTRIBUTES sa = {sizeof(sa), NULL, TRUE};
				HANDLE srv_pipe = ::CreateNamedPipeW(pipename.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
					PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
					1, kPipeBufferSz, kPipeBufferSz, NMP_DEFAULT_TIMEOUT_VALUE, &sa);
				if(INVALID_HANDLE_VALUE != srv_pipe)
				{
					if (!::DuplicateHandle(::GetCurrentProcess(), srv_pipe, target_process, &target_pipe,
						0, FALSE, DUPLICATE_CLOSE_SOURCE |
						DUPLICATE_SAME_ACCESS)) {
							gle = ::GetLastError();
							::CloseHandle(srv_pipe);
							target_pipe = INVALID_HANDLE_VALUE;
					}
				}
				else
					gle = ::GetLastError();
				::CloseHandle(target_process);
			}

			size_t retV = SendMsg(kSbxPISDKGetHandleToPluginIPCChannelRecv, ch, gle, target_pipe);
			if(INVALID_HANDLE_VALUE != target_pipe)
			{
				g_ipc_transport = SbxGetBrokerTransport();
				if(g_ipc_transport)
				{
					HANDLE cln_pipe = INVALID_HANDLE_VALUE;
					SECURITY_ATTRIBUTES sa = {sizeof(sa), NULL, FALSE};
					DWORD attributes = SECURITY_SQOS_PRESENT | SECURITY_IDENTIFICATION;
					for (;;) { //connect to the Named Pipe.
						BOOL result = ::WaitNamedPipeW(pipename.c_str(), NMPWAIT_USE_DEFAULT_WAIT);
						if(result) {
							cln_pipe = ::CreateFileW(pipename.c_str(), GENERIC_READ | GENERIC_WRITE, 0, &sa,
								OPEN_EXISTING, attributes, NULL);
							if (INVALID_HANDLE_VALUE == cln_pipe) {
								if (ERROR_PIPE_BUSY != ::GetLastError()) {
									break;
								}
								// wait and retry.
								::Sleep(25);
							} else {
								break;
							}
						}
						// wait and retry.
						::Sleep(25);
					}
					g_ipc_transport->OpenClient(cln_pipe);
				}
			}

			return retV;
	}
};

MsgHandlerBaseT* SandboxPIServiceManager::OnMessage(int msg_id) {

	static SbxPISDKGetHandleToPluginIPCChannelSvc sbxpisdkgethandletopluginipcchannel_svc_;

	switch(msg_id)
	{
	case SbxPISDKGetHandleToPluginIPCChannelSvc::MSG_ID:
		return &sbxpisdkgethandletopluginipcchannel_svc_;
	default:
		return NULL;
	}
}