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
#include "SandboxPIIPCDefs.h"			//Types, macros, structures, etc. required to implement the Sanbox Plugin Broker Extensions

#include "CorCalls.h"					//For SandboxHFT
#include "SandboxCalls.h"				//For SandboxHFT

#include "ASCalls.h"

// Imported HFTS
extern HFT gSandboxHFT;
extern ASUns32 gSandboxVersion;

SandboxPITransportT* g_ipc_transport = NULL;
SandboxPIServiceManager* g_service_manager = NULL;


//---------------------------------------------------------------------------
//The Plugin must implement this by providing its own ServiceManagerT Type.
//---------------------------------------------------------------------------
extern bool StartSandboxPIPluginServer(); 

namespace sandboxpisdk {

	bool InitSandboxPIBrokerIPCChannel(const std::wstring &piBroker) {
		
		if(g_ipc_transport == NULL)
		{
			HANDLE ipc_client = INVALID_HANDLE_VALUE;
			if(!HFTIsValid(gSandboxHFT))
			{
				gSandboxHFT = ASExtensionMgrGetHFT(ASAtomFromString("SandboxHFT"), SandboxHFT_LATEST_VERSION);
				gSandboxVersion = SandboxHFT_LATEST_VERSION;
			}
			if(HFTIsValid(gSandboxHFT) && SbxAppGetIsSandboxed())
				ipc_client = SbxLaunchPIBrokerExe(piBroker.c_str());
			if(ipc_client != INVALID_HANDLE_VALUE)
			{	
				g_ipc_transport = new PipeTransportWithMessagePump();
				if(g_ipc_transport)
					g_ipc_transport->OpenClient(ipc_client);
			}
		}
		return (g_ipc_transport && g_ipc_transport->IsConnected());
	} //InitSandboxPIIPCChannel

	bool TerminateSandboxPIBroker() {
		if(g_ipc_transport == NULL || !g_ipc_transport->IsConnected()) return false;

		delete g_ipc_transport;
		g_ipc_transport = NULL;
		return true;
	}

	bool ReleaseByteArray(ipc::ByteArray& ba)
	{
		if(ba.sz_ > 0 && ba.buf_ != NULL)
		{
			free((const_cast<char*>(ba.buf_)));
			ba.buf_ = NULL;
			ba.sz_ = 0;
			return true;
		}
		return false;
	}

	/*
	* This makes a cross call to the broker to create a named pipe and return its duplicate handle.
	* This named pipe is accessed within a new thread to service crosscalls made by the broker to the plugin.
	* This is useful to delegate any dangerous backend processing back to the plugin which runs in sandbox mode.
	* InitSandboxPIBrokerIPCChannel should be called successfully before making any cross calls.
	*/
	bool InitSandboxPIPluginIPCChannel(const std::wstring &piBroker){
		static bool retV = false;
		static bool setupAlreadyCalled = false;
		if(!setupAlreadyCalled)
		{
			setupAlreadyCalled = true;
			retV = InitSandboxPIBrokerIPCChannel(piBroker);
			if(retV)
				retV = StartSandboxPIPluginServer();
		}
		return retV;
	} //InitSandboxPIPluginIPCChannel

	//-------------------------------------------------------------------------------
	// This is the macro that defines the number of OUT parameters for the cross call.
	// simple-ipc-lib imposes a minimum of one and a maximum of ten parameters.
	// If you do not want any results back, even then you have to specify atleast a 
	// dummy return type which you may document as unused.
	// This CrossCallAPI, SamplePIBrokerDisplayMessage receives back the message
	// describing the action done by the user as a byte array in the OUT parameter.
	//-------------------------------------------------------------------------------
	DEFINE_IPC_MSG_CONV(kSbxPISDKGetHandleToPluginIPCChannelRecv, 2) {
		IPC_MSG_P1(DWORD, ULong32)            // Last error.
		IPC_MSG_P2(HANDLE, VoidPtr)        // NamedPipe handle.
	};
	//-------------------------------------------------------------------------------
	//This is the Rpc class for the CrossCallAPI, SamplePIBrokerDisplayMessage
	//This must be implemented in the plugin side. The corresponding Svc class 
	//which services this call is implemented in the plugin broker process.
	//As the name suggests, it will display some message box to the user from the broker process 
	//and return the user action back to the Plugin. 
	//The original message that gets displayed to the user is passed from the Plugin as an IN parameter.
	//The user response is received as an OUT parameter in the "response" buffer.
	//This demonstrates how to send IN and OUT parameters across the CrossCalls.
	//-------------------------------------------------------------------------------
	class SbxPISDKGetHandleToPluginIPCChannelRpc : public SandboxPIRpcBase,
		public ipc::MsgIn<kSbxPISDKGetHandleToPluginIPCChannelRecv, SbxPISDKGetHandleToPluginIPCChannelRpc, SandboxPIChannelT> {
	public:
		SbxPISDKGetHandleToPluginIPCChannelRpc() : status_(0), handle_(INVALID_HANDLE_VALUE) {}

		HANDLE Do() {

			DWORD targetPID = ::GetCurrentProcessId();
			size_t r = Recv(this, SendMsg(kSbxPISDKGetHandleToPluginIPCChannelSend,
				&ch_,
				targetPID)); //Dummy unused
			if (r) {
				return NULL;
			}
			if (status_) ::SetLastError(status_);
			return handle_;
		}

		size_t OnMsg(SandboxPIChannelT*, DWORD status, HANDLE handle) {
			status_ = status;
			handle_ = handle;
			return ipc::OnMsgReady;
		}

	private:

		//All the OUT parameters are saved as private members like this one.
		//These get saved inside OnMsg() above and returned to the caller from within the Do() call.

		//This as a placeholder for the OUT parameter, which saves the user response in the buffer
		//and copies it to the messageResponse before returning from the Do() call.
		DWORD status_;
		HANDLE handle_;
	};

	HANDLE __stdcall SandboxPISDKGetHandleToPluginIPCChannel() {
		SbxPISDKGetHandleToPluginIPCChannelRpc rpc;
		return rpc.Do();
	}

} //namespace sandboxpisdk

MsgHandlerBaseT* SandboxPIServiceManager::OnMessage(int msg_id) { return NULL; }