/*************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright [2010] Adobe Systems Incorporated
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
#ifndef SANDBOXPISDKCLIENTMACROS_H
#define SANDBOXPISDKCLIENTMACROS_H

#include <string>
#include "ipc_constants.h"
#include "SandboxPIIPCDefs.h"

#if ACROBAT_SANDBOX

using namespace ipc;
static void InitWireTypeParam(int wire_type, void* param)
{
	if(param == NULL || wire_type <= TYPE_NONE || wire_type >= TYPE_LAST)
		return;
	switch(wire_type)
	{
	case ipc::TYPE_VOIDPTR:
		static_cast<LPVOID>(param) = NULL;
		break;
	case ipc::TYPE_UINT32:
		(*static_cast<unsigned int*>(param)) = 0;
		break;
	case ipc::TYPE_BARRAY:
		{
			ByteArray& ba = *(static_cast<ipc::ByteArray*>(param));
			ba.sz_ = 0;
			ba.buf_ = NULL;
		}
	default:
		break;
	}
}

static bool CopyWireTypeParam(int wire_type, const void* from, void* to)
{
	bool ret_value = false;
	if(from == NULL || to == NULL || wire_type < TYPE_INT32 || wire_type > TYPE_BARRAY)
		return false;
	switch(wire_type)
	{
	case TYPE_BARRAY: 
		{
			const ByteArray* src = static_cast<const ByteArray*>(from);
			ByteArray* dest = static_cast<ByteArray*>(to);
			dest->sz_ = src->sz_;
			dest->buf_ = static_cast<const char*>(malloc(src->sz_));
			if(dest->buf_ != NULL)
			{
				if(memcpy_s(const_cast<char*>(dest->buf_), dest->sz_, src->buf_, src->sz_) == 0)
				{
					ret_value = true;
				}
				else
				{
					free((const_cast<char*>(dest->buf_)));
					dest->buf_ = NULL;
					dest->sz_ = 0;
				}
			}
			break;
		}
	case ipc::TYPE_VOIDPTR: 
		{
			*(static_cast<LPVOID*>(to)) = *(static_cast<const LPVOID*>(from));
			ret_value = true;
			break;
		}
	case ipc::TYPE_UINT32:
	case ipc::TYPE_INT32:
		{
			(*static_cast<unsigned int*>(to)) = (*static_cast<const unsigned int*>(from));
			ret_value = true;
			break;
		}
	case ipc::TYPE_LONG32:
	case ipc::TYPE_ULONG32:
		{
			(*static_cast<unsigned long*>(to)) = (*static_cast<const unsigned long*>(from));
			ret_value = true;
			break;
		}
	case ipc::TYPE_STDSTRING8:
		{
			(*static_cast<std::string*>(to)) = (*static_cast<const std::string*>(from));
			ret_value = true;
			break;
		}
	case ipc::TYPE_STDSTRING16:
		{
			(*static_cast<std::wstring*>(to)) = (*static_cast<const std::wstring*>(from));
			ret_value = true;
			break;
		}
	default:
		break;
	}
	return ret_value;
}

// IN-1 OUT-1
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_1_1 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
public:
	ClientTuple_1_1(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, typename OutClass::POut1_Type& out1)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		return ipc::OnMsgReady;
  }
};

// IN-2 OUT-1
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_2_1 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
public:
	ClientTuple_2_1(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, typename OutClass::POut1_Type& out1)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		return ipc::OnMsgReady;
  }
};

// IN-3 OUT-1
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_3_1 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
public:
	ClientTuple_3_1(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3, 
		typename OutClass::POut1_Type& out1)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		return ipc::OnMsgReady;
  }
};

// IN-4 OUT-1
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_4_1 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
public:
	ClientTuple_4_1(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3, 
		const typename InClass::PIn4_Type& in4, typename OutClass::POut1_Type& out1)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		return ipc::OnMsgReady;
  }
};

// IN-5 OUT-1
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_5_1 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
public:
	ClientTuple_5_1(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3, 
		const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, typename OutClass::POut1_Type& out1)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4, in5));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		return ipc::OnMsgReady;
  }
};

// IN-6 OUT-1
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_6_1 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
public:
	ClientTuple_6_1(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3, 
		const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, const typename InClass::PIn6_Type& in6, 
		typename OutClass::POut1_Type& out1)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4, in5, in6));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		return ipc::OnMsgReady;
  }
};

// IN-1 OUT-2
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_1_2 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
public:
	ClientTuple_1_2(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		return ipc::OnMsgReady;
  }
};

// IN-2 OUT-2
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_2_2 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
public:
	ClientTuple_2_2(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2,
			typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));
		
		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));
		
		return ipc::OnMsgReady;
  }
};

// IN-3 OUT-2
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_3_2 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
public:
	ClientTuple_3_2(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));
		
		return ipc::OnMsgReady;
  }
};

// IN-4 OUT-2
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_4_2 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
public:
	ClientTuple_4_2(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));
			
		return ipc::OnMsgReady;
  }
};

// IN-5 OUT-2
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_5_2 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
public:
	ClientTuple_5_2(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, typename OutClass::POut1_Type& out1, 
			typename OutClass::POut2_Type& out2)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4, in5));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));
			
		return ipc::OnMsgReady;
  }
};

// IN-6 OUT-2
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_6_2 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
public:
	ClientTuple_6_2(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, const typename InClass::PIn6_Type& in6, 
			typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4, in5, in6));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));
			
		return ipc::OnMsgReady;
  }
};

// IN-1 OUT-3
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_1_3 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
public:
	ClientTuple_1_3(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2,
			typename OutClass::POut3_Type& out3)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			const typename OutClass::POut3_Type& out3) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));

		return ipc::OnMsgReady;
  }
};

// IN-2 OUT-3
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_2_3 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
public:
	ClientTuple_2_3(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2,
			typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
		const typename OutClass::POut3_Type& out3) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));
		
		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));
		
		return ipc::OnMsgReady;
  }
};

// IN-3 OUT-3
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_3_3 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
public:
	ClientTuple_3_3(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			const typename OutClass::POut3_Type& out3) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));
		
		return ipc::OnMsgReady;
  }
};

// IN-4 OUT-3
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_4_3 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
public:
	ClientTuple_4_3(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2,
			typename OutClass::POut3_Type& out3)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			const typename OutClass::POut3_Type& out3) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));
			
		return ipc::OnMsgReady;
  }
};

// IN-5 OUT-3
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_5_3 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
public:
	ClientTuple_5_3(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, typename OutClass::POut1_Type& out1, 
			typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4, in5));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			typename OutClass::POut3_Type& out3) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));
			
		return ipc::OnMsgReady;
  }
};

// IN-6 OUT-3
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_6_3 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
public:
	ClientTuple_6_3(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, const typename InClass::PIn6_Type& in6, 
			typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4, in5, in6));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			const typename OutClass::POut3_Type& out3) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));
			
		return ipc::OnMsgReady;
  }
};


// IN-1 OUT-4
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_1_4 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
	typename OutClass::POut4_Type out4_;
public:
	ClientTuple_1_4(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2,
			typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;
		out4 = out4_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			const typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));

		CopyWireTypeParam(typename OutClass::POut4_WireType,reinterpret_cast<const void*>(&out4), reinterpret_cast<void*>(&out4_));

		return ipc::OnMsgReady;
  }
};

// IN-2 OUT-4
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_2_4 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
	typename OutClass::POut4_Type out4_;
public:
	ClientTuple_2_4(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2,
			typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3,
			typename OutClass::POut4_Type& out4)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;
		out4 = out4_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
		const typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));
		
		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));

		CopyWireTypeParam(typename OutClass::POut4_WireType,reinterpret_cast<const void*>(&out4), reinterpret_cast<void*>(&out4_));

		
		return ipc::OnMsgReady;
  }
};

// IN-3 OUT-4
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_3_4 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
	typename OutClass::POut4_Type out4_;
public:
	ClientTuple_3_4(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3, 
			typename OutClass::POut4_Type& out4)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;
		out4 = out4_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			const typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));

		CopyWireTypeParam(typename OutClass::POut4_WireType,reinterpret_cast<const void*>(&out4), reinterpret_cast<void*>(&out4_));
		
		return ipc::OnMsgReady;
  }
};

// IN-4 OUT-4
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_4_4 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
	typename OutClass::POut4_Type out4_;
public:
	ClientTuple_4_4(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2,
			typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;
		out4 = out4_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			const typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));

		CopyWireTypeParam(typename OutClass::POut4_WireType,reinterpret_cast<const void*>(&out4), reinterpret_cast<void*>(&out4_));
			
		return ipc::OnMsgReady;
  }
};

// IN-5 OUT-4
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_5_4 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
	typename OutClass::POut4_Type out4_;
public:
	ClientTuple_5_4(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, typename OutClass::POut1_Type& out1, 
			typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4, in5));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;
		out4 = out4_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));

		CopyWireTypeParam(typename OutClass::POut4_WireType,reinterpret_cast<const void*>(&out4), reinterpret_cast<void*>(&out4_));
			
		return ipc::OnMsgReady;
  }
};

// IN-6 OUT-4
template<class InClass, class OutClass, int msg_id_send, int msg_id_recv, class DerivedT>
class ClientTuple_6_4 : public SandboxPIRpcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT> {
	
protected:
	typename OutClass::POut1_Type out1_;
	typename OutClass::POut2_Type out2_;
	typename OutClass::POut3_Type out3_;
	typename OutClass::POut4_Type out4_;
public:
	ClientTuple_6_4(bool b_run_msg_pump) : SandboxPIRpcBase(b_run_msg_pump) { }
	bool Do(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, const typename InClass::PIn6_Type& in6, 
			typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3,
			typename OutClass::POut4_Type out4)
	{
		size_t r = ipc::OnMsgReady;
		try {
			r = Recv(this, SendMsg(msg_id_send,
                                  &ch_,
								  in1, in2, in3, in4, in5, in6));
		}
		catch (...) { }
		if (r) {
		  return false;
		}
		out1 = out1_;
		out2 = out2_;
		out3 = out3_;
		out4 = out4_;

		return true;
	}

	size_t OnMsg(SandboxPIChannelT*, const typename OutClass::POut1_Type& out1, const typename OutClass::POut2_Type& out2, 
			const typename OutClass::POut3_Type& out3, const typename OutClass::POut4_Type& out4) {
		CopyWireTypeParam(typename OutClass::POut1_WireType, reinterpret_cast<const void*>(&out1), reinterpret_cast<void*>(&out1_));

		CopyWireTypeParam(typename OutClass::POut2_WireType,reinterpret_cast<const void*>(&out2), reinterpret_cast<void*>(&out2_));

		CopyWireTypeParam(typename OutClass::POut3_WireType,reinterpret_cast<const void*>(&out3), reinterpret_cast<void*>(&out3_));

		CopyWireTypeParam(typename OutClass::POut4_WireType,reinterpret_cast<const void*>(&out4), reinterpret_cast<void*>(&out4_));
			
		return ipc::OnMsgReady;
  }
};

#define DECLARE_CROSS_CALL_START enum { \
		kCrossCallClientStart = kSbxPISDKLastId,
#define CROSS_CALL_ID(class_name)	k##class_name##Send, k##class_name##Recv,
#define DECLARE_CROSS_CALL_END kCrossCallClientEnd };

#define DEFINE_IPC_MSG_CLIENT(class_name, nIn) typedef MsgParamConverter<k##class_name##Send> PCIn_##class_name; \
			DEFINE_IPC_MSG_CONV(k##class_name##Send, nIn)

#define INPUT_PARAM_VOID_PTR(n) IPC_MSG_P##n (LPVOID, VoidPtr) \
	typedef LPVOID PIn##n##_Type; \
	static const int PIn##n##_WireType = ipc::TYPE_VOIDPTR;

#define INPUT_PARAM_INT(n) IPC_MSG_P##n (int, Int32) \
	typedef int PIn##n##_Type; \
	static const int PIn##n##_WireType = ipc::TYPE_INT32;

#define INPUT_PARAM_UINT(n) IPC_MSG_P##n (unsigned int, UInt32) \
	typedef unsigned int PIn##n##_Type; \
	static const int PIn##n##_WireType = ipc::TYPE_UINT32;

#define INPUT_PARAM_LONG(n) IPC_MSG_P##n (long, Long32) \
	typedef long PIn##n##_Type; \
	static const int PIn##n##_WireType = ipc::TYPE_LONG32;

#define INPUT_PARAM_ULONG(n) IPC_MSG_P##n (unsigned long, ULong32) \
	typedef unsigned long PIn##n##_Type; \
	static const int PIn##n##_WireType = ipc::TYPE_ULONG32;

#define INPUT_PARAM_STRING(n) IPC_MSG_P##n (std::string, StdString8) \
	typedef std::string PIn##n##_Type; \
	static const int PIn##n##_WireType = ipc::TYPE_STDSTRING8;

#define INPUT_PARAM_WSTRING(n) IPC_MSG_P##n (std::wstring, StdString16) \
	typedef std::wstring PIn##n##_Type; \
	static const int PIn##n##_WireType = ipc::TYPE_STDSTRING16;

#define INPUT_PARAM_BUFFER(n) IPC_MSG_P##n(ipc::ByteArray, ByteArray) \
	typedef ipc::ByteArray PIn##n##_Type; \
	static const int PIn##n##_WireType = ipc::TYPE_BARRAY;



#define DEFINE_CROSS_CALL_CLIENT(class_name, nIn, nOut) class class_name##Rpc : \
	public ClientTuple_##nIn##_##nOut<PCIn_##class_name, PCOut_##class_name, k##class_name##Send, k##class_name##Recv, class_name##Rpc> \
	{ \
	public:\
		class_name##Rpc(bool b_run_msg_pump = false) : ClientTuple_##nIn##_##nOut(b_run_msg_pump) { } \
} ;

#define MAKE_CROSS_CALL(class_name) class_name##Rpc class_name##_rpc; \
	class_name##_rpc.Do

#define MAKE_CROSS_CALL_WITH_MSG_PUMP(class_name) class_name##Rpc class_name##_rpc(true); \
	class_name##_rpc.Do

#define MAKE_CROSS_CALL_WITH_RETURN_VAL(class_name, ret_value) class_name##Rpc class_name##_rpc; \
	ret_value = class_name##_rpc.Do

#define MAKE_CROSS_CALL_WITH_MSG_PUMP_AND_RETURN_VAL(class_name, ret_value) class_name##Rpc class_name##_rpc(true); \
	ret_value = class_name##_rpc.Do


#endif // ACROBAT_SANDBOX

#endif // SANDBOXPISDKCLIENTMACROS_H