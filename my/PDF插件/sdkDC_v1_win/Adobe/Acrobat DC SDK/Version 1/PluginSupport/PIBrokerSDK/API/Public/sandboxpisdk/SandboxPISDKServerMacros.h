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
#ifndef SANDBOXPISDKSERVERMACROS_H
#define SANDBOXPISDKSERVERMACROS_H

#include <string>
#include "ipc_constants.h"
#include "SandboxPIIPCDefs.h"
#include <map>


#if ACROBAT_SANDBOX

using namespace ipc;

// IN-1 OUT-1
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_1_1 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_1_1() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, typename OutClass::POut1_Type& out1) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1) {
		typename OutClass::POut1_Type out1;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, out1);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1);

    }
};

// IN-2 OUT-1
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_2_1 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_2_1() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, typename OutClass::POut1_Type& out1) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2) {
		typename OutClass::POut1_Type out1;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, out1);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1);

    }
};

// IN-3 OUT-1
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_3_1 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_3_1() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
				typename OutClass::POut1_Type& out1) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3) {
		typename OutClass::POut1_Type out1;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, out1);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1);

    }
};

// IN-4 OUT-1
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_4_1 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_4_1() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
				const typename InClass::PIn4_Type& in4, typename OutClass::POut1_Type& out1) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4) {
		typename OutClass::POut1_Type out1;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, out1);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1);

    }
};

// IN-5 OUT-1
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_5_1 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_5_1() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
				const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, typename OutClass::POut1_Type& out1) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5) {
		typename OutClass::POut1_Type out1;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, in5, out1);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1);

    }
};

// IN-6 OUT-1
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_6_1 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_6_1() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
				const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, const typename InClass::PIn6_Type& in6, 
				typename OutClass::POut1_Type& out1) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
			const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, const typename InClass::PIn6_Type& in6) {
		typename OutClass::POut1_Type out1;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, in5, in6, out1);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1);

    }
};

// IN-1 OUT-2
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_1_2 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_1_2() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, out1, out2);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2);

    }
};

// IN-2 OUT-2
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_2_2 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_2_2() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, out1, out2);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2);

    }
};

// IN-3 OUT-2
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_3_2 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_3_2() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, out1, out2);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2);

    }
};

// IN-4 OUT-2
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_4_2 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_4_2() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		const typename InClass::PIn4_Type& in4, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		const typename InClass::PIn3_Type& in3, const typename InClass::PIn4_Type& in4) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, out1, out2);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2);

    }
};

// IN-5 OUT-2
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_5_2 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_5_2() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		const typename InClass::PIn3_Type& in3, const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, in5, out1, out2);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2);

    }
};

// IN-6 OUT-2
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_6_2 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_6_2() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, const typename InClass::PIn6_Type& in6, 
		typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		const typename InClass::PIn3_Type& in3, const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5,
		const typename InClass::PIn6_Type& in6) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, in5, in6, out1, out2);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2);

    }
};

// IN-1 OUT-3
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_1_3 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_1_3() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2,
		typename OutClass::POut3_Type& out3) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, out1, out2, out3);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3);

    }
};

// IN-2 OUT-3
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_2_3 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_2_3() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, out1, out2, out3);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3);

    }
};

// IN-3 OUT-3
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_3_3 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_3_3() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, out1, out2, out3);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3);

    }
};

// IN-4 OUT-3
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_4_3 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_4_3() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		const typename InClass::PIn4_Type& in4, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		const typename InClass::PIn3_Type& in3, const typename InClass::PIn4_Type& in4) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, out1, out2, out3);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3);

    }
};

// IN-5 OUT-3
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_5_3 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_5_3() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2,
		typename OutClass::POut3_Type& out3) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		const typename InClass::PIn3_Type& in3, const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, in5, out1, out2, out3);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3);

    }
};

// IN-6 OUT-3
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_6_3 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_6_3() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, const typename InClass::PIn6_Type& in6, 
		typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		const typename InClass::PIn3_Type& in3, const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5,
		const typename InClass::PIn6_Type& in6) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut2_Type out3;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, in5, in6, out1, out2, out3);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3);

    }
};

// IN-1 OUT-4
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_1_4 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_1_4() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2,
		typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		typename OutClass::POut4_Type out4;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, out1, out2, out3, out4);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3, out4);

    }
};

// IN-2 OUT-4
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_2_4 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_2_4() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3, 
		typename OutClass::POut4_Type& out4) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		typename OutClass::POut4_Type out4;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, out1, out2, out3, out4);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3, out4);

    }
};

// IN-3 OUT-4
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_3_4 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_3_4() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3, 
		typename OutClass::POut4_Type& out4) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		typename OutClass::POut4_Type out4;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, out1, out2, out3, out4);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3, out4);

    }
};

// IN-4 OUT-4
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_4_4 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_4_4() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		const typename InClass::PIn4_Type& in4, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3,
		typename OutClass::POut4_Type& out4) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		const typename InClass::PIn3_Type& in3, const typename InClass::PIn4_Type& in4) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		typename OutClass::POut4_Type out4;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, out1, out2, out3, out4);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3, out4);

    }
};

// IN-5 OUT-4
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_5_4 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_5_4() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2,
		typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		const typename InClass::PIn3_Type& in3, const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut3_Type out3;
		typename OutClass::POut4_Type out4;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, in5, out1, out2, out3, out4);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3, out4);

    }
};

// IN-6 OUT-4
template<class InClass, class OutClass, class DerivedT, int msg_id_send, int msg_id_recv >
class ServerTuple_6_4 : public SandboxPISvcBase, public ipc::MsgIn<msg_id_recv, DerivedT, SandboxPIChannelT>  {	

public:
	ServerTuple_6_4() { }
	virtual void Proc(const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, const typename InClass::PIn3_Type& in3,
		const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5, const typename InClass::PIn6_Type& in6, 
		typename OutClass::POut1_Type& out1, typename OutClass::POut2_Type& out2, typename OutClass::POut3_Type& out3, typename OutClass::POut4_Type& out4) = 0;
	size_t OnMsg(SandboxPIChannelT* ch, const typename InClass::PIn1_Type& in1, const typename InClass::PIn2_Type& in2, 
		const typename InClass::PIn3_Type& in3, const typename InClass::PIn4_Type& in4, const typename InClass::PIn5_Type& in5,
		const typename InClass::PIn6_Type& in6) {
		typename OutClass::POut1_Type out1;
		typename OutClass::POut2_Type out2;
		typename OutClass::POut2_Type out3;
		typename OutClass::POut4_Type out4;
		try {
		static_cast<DerivedT*>(this)->Proc(in1, in2, in3, in4, in5, in6, out1, out2, out3, out4);
		}
		catch (...) { }

		return SendMsg(msg_id_send, ch, out1, out2, out3, out4);

    }
};

#define DEFINE_IPC_MSG_SERVER(class_name, nOut) typedef MsgParamConverter<k##class_name##Recv> PCOut_##class_name; \
			DEFINE_IPC_MSG_CONV(k##class_name##Recv, nOut)

#define OUTPUT_PARAM_VOID_PTR(n) IPC_MSG_P##n (LPVOID, VoidPtr) \
	typedef LPVOID POut##n##_Type; \
	static const int POut##n##_WireType = ipc::TYPE_VOIDPTR;

#define OUTPUT_PARAM_INT(n) IPC_MSG_P##n (int, Int32) \
	typedef int POut##n##_Type; \
	static const int POut##n##_WireType = ipc::TYPE_INT32;

#define OUTPUT_PARAM_UINT(n) IPC_MSG_P##n (unsigned int, UInt32) \
	typedef unsigned int POut##n##_Type; \
	static const int POut##n##_WireType = ipc::TYPE_UINT32;

#define OUTPUT_PARAM_LONG(n) IPC_MSG_P##n (long, Long32) \
	typedef long POut##n##_Type; \
	static const int POut##n##_WireType = ipc::TYPE_LONG32;

#define OUTPUT_PARAM_ULONG(n) IPC_MSG_P##n (unsigned long, ULong32) \
	typedef unsigned long POut##n##_Type; \
	static const int POut##n##_WireType = ipc::TYPE_ULONG32;

#define OUTPUT_PARAM_STRING(n) IPC_MSG_P##n (std::string, StdString8) \
	typedef std::string POut##n##_Type; \
	static const int POut##n##_WireType = ipc::TYPE_STDSTRING8;

#define OUTPUT_PARAM_WSTRING(n) IPC_MSG_P##n (std::wstring, StdString16) \
	typedef std::wstring POut##n##_Type; \
	static const int POut##n##_WireType = ipc::TYPE_STDSTRING16;

#define OUTPUT_PARAM_BUFFER(n) IPC_MSG_P##n(ipc::ByteArray, ByteArray) \
	typedef ipc::ByteArray POut##n##_Type; \
	static const int POut##n##_WireType = ipc::TYPE_BARRAY;


#define DEFINE_CROSS_CALL_SERVER_START(class_name, nIn, nOut) class class_name##Svc : \
	public ServerTuple_##nIn##_##nOut<PCIn_##class_name, PCOut_##class_name, class_name##Svc, k##class_name##Recv, k##class_name##Send> { \
public: \
	INTERNAL_MSG_REFLECT(SandboxPIChannelT)
#define DEFINE_CROSS_CALL_SERVER_END };


#define DEFINE_CROSS_CALL_HANDLER_START(PINAME) class PINAME##ServiceManager : public SandboxPIServiceManager { \
		private: \
		typedef std::map<int,  MsgHandlerBaseT*> map_msg_handler_t; \
		map_msg_handler_t map_msg_handler_; \
		public: \
  MsgHandlerBaseT* OnMessage(int msg_id) { \
		map_msg_handler_t::iterator it = map_msg_handler_.find(msg_id); \
		if(it != map_msg_handler_.end()) \
			return (it->second); \
        return SandboxPIServiceManager::OnMessage(msg_id); \
    } \
protected: \
  SandboxPITransportT* transport_; \
public: \
	PINAME##ServiceManager(SandboxPITransportT* transport) \
			: transport_(transport) { \

#define ADD_MSG_HANDLER(class_name) static class_name##Svc class_name##_svc_; \
			map_msg_handler_[class_name##Svc::MSG_ID] = &class_name##_svc_; \

#define DEFINE_CROSS_CALL_HANDLER_END } \
			};	

#define DEFINE_START_SERVER_CALL(PINAME) int StartSandboxPIServer(HINSTANCE instance, const wchar_t* cmdline) { \
	SandboxPIServer PINAME##PIServer; \
	if(!PINAME##PIServer.StartBrokerService<PINAME##ServiceManager>(cmdline)) \
		return 1; \
	return 0; \
}

#endif // ACROBAT_SANDBOX

#endif // SANDBOXPISDKSERVERMACROS_H