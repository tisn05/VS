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
#ifndef SADNBOXPIIPCDEFS_H
#define SADNBOXPIIPCDEFS_H

#include "ipc_channel.h"
#include "ipc_codec.h"
#include "ipc_msg_dispatch.h"
#include <string>

#include "SandboxExpT.h"
#include "PipeWinWithMessagePump.h"

//---------------------------------------------------------------------------------------------------
// - Types, macros, structures, etc. required to implement the Sanbox Plugin Broker Extensions
//---------------------------------------------------------------------------------------------------

static const wchar_t kCmdLinePipeEq[] = L" --ipc=";
static const wchar_t kCmdLineProcEq[] = L" --proc=";

typedef PipeTransportWithMessagePump SandboxPITransportT;
typedef ipc::Channel<SandboxPITransportT, ipc::Encoder, ipc::Decoder> SandboxPIChannelT;

class SandboxPIServiceManager;

extern SandboxPITransportT* g_ipc_transport;
extern SandboxPIServiceManager* g_service_manager;

template <typename ChannelT>
class MsgHandlerBase {
public:
  virtual size_t OnMsgIn(int msg_id, ChannelT* ch,
                         const ipc::WireType* const args[], 
                         int count) = 0;

  virtual bool OnMsgArgCountError(int count) = 0;
  virtual bool OnMsgArgConvertError(int code) = 0;
};

typedef MsgHandlerBase<SandboxPIChannelT> MsgHandlerBaseT;

class SandboxPIServiceManager
{
public:
	virtual MsgHandlerBaseT* OnMessage(int msg_id);
};

namespace sandboxpisdk {

	inline MsgHandlerBaseT* SandboxPIServiceGetMessageHandler(int msg_id)
	{
		return g_service_manager?g_service_manager->OnMessage(msg_id):NULL;
	}

	template <class ServiceManagerT>
	bool SandboxPIStartServiceManager(SandboxPITransportT* transport)
	{
		bool retVal = true;
		try {
			if(!g_service_manager)
				g_service_manager = new ServiceManagerT(transport);
			if(!g_service_manager)
				retVal = false;
		} catch(...)
		{
			retVal = false;
		}
		return retVal;
	}

	inline void SandboxPIStopServiceManager()
	{
		if(g_service_manager)
			delete g_service_manager;
		g_service_manager = NULL;
		if(g_ipc_transport)
			delete g_ipc_transport;
		g_ipc_transport = NULL;

	}
}

class MessageHandler : public MsgHandlerBaseT {
public:
  bool OnMsgArgCountError(int /*count*/) {
    return false;
  }

  bool OnMsgArgConvertError(int /*code*/) {
    return false;
  }
};

class BadMessageRecv : public MessageHandler {
public:
  virtual size_t OnMsgIn(int /*msg_id*/, SandboxPIChannelT* /*ch*/, const ipc::WireType* const args[], int /*count*/) {
    args;
    return static_cast<size_t>(-1);
  }
};

class MsgDispatch {
public:
  MsgDispatch()
    : bad_msg_() {
  }

  MsgHandlerBaseT* MsgHandler(int msg_id) {  
	  MsgHandlerBaseT* pimsghandler = sandboxpisdk::SandboxPIServiceGetMessageHandler(msg_id);
	  if (pimsghandler)
		  return pimsghandler;
	  else return &bad_msg_;
  }

  void* OnNewTransport() { return NULL; }

private:
  BadMessageRecv   bad_msg_;
};

//---------------------------------------------------------------------------------------------------
//Base for remote IPC call message class for each Brokered API, implemented in SandboxPI plugin.
//---------------------------------------------------------------------------------------------------
class SandboxPIRpcBase : public ipc::MsgOut<SandboxPIChannelT> {
public:
	SandboxPIRpcBase(bool b_run_msg_pump = false) : ch_(g_ipc_transport) 
	{ if(g_ipc_transport) g_ipc_transport->SetRunMsgPump(b_run_msg_pump); }

  bool OnMsgArgCountError(int /*count*/) {
    ::OutputDebugStringA("!! rpc:OnMsgArgCountError");
    return false;
  }

  bool OnMsgArgConvertError(int /*code*/) {
    ::OutputDebugStringA("!! rpc:OnMsgArgConvertError");
    return false;
  }

  void* OnNewTransport() { return NULL; }

protected: 
  template <typename TReceiver>
  size_t Recv(TReceiver* recv, size_t result) {
      if (result) {
          __debugbreak();
          return 1;
      }
      result = ch_.Receive(recv);
      if (result != ipc::OnMsgReady) {
          return result;
      }
      return ipc::RcOK;
  }

  SandboxPIChannelT ch_;
};

//---------------------------------------------------------------------------------------------------
//Used by Sandbox PIBroker process to implement the Brokered Calls.
//---------------------------------------------------------------------------------------------------
#define INTERNAL_MSG_REFLECT(Channel)                                                               \
  virtual size_t OnMsgIn(int msg_id, Channel* ch, const ipc::WireType* const args[], int count) {   \
    return OnMsgInX(msg_id, ch, args, count);                                                       \
  }
//---------------------------------------------------------------------------------------------------
//Base for remote IPC call message class for each Brokered API, implemented by Sandbox PIBroker side.
//---------------------------------------------------------------------------------------------------
class SandboxPISvcBase : public MsgHandlerBaseT,
                      public ipc::MsgOut<SandboxPIChannelT> {
public:
  bool OnMsgArgCountError(int /*count*/) {
    ::OutputDebugStringA("!! svc:OnMsgArgCountError");
    return false;
  }

  bool OnMsgArgConvertError(int /*code*/) {
    ::OutputDebugStringA("!! svc:OnMsgArgCountError");
    return false;
  }
};


enum {
	//Starting from kMessagePrivLastId to avoid any conflicts with some internal messagesIDs used by simple-ipc-lib
	//Crosscalls implemented by SandboxPISDK
	kSbxPISDKGetHandleToPluginIPCChannelSend = ipc::kMessagePrivLastId,			
	kSbxPISDKGetHandleToPluginIPCChannelRecv,								
	kSbxPISDKLastId
};

namespace sandboxpisdk {
HANDLE __stdcall SandboxPISDKGetHandleToPluginIPCChannel();
}

#endif //SADNBOXPIIPCDEFS_H