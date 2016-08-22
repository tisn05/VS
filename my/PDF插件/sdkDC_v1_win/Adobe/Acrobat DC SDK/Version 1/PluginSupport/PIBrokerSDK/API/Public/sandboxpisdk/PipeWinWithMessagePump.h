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
#ifndef SDKSANDBOXPIPEWIN_H
#define SDKSANDBOXPIPEWIN_H

//////////////////////////////////////////////////////////////////////////
// Copied Chrome Code below;
// modified for Adobe Reader purposes
// 
// The file from which the code is copied is
// Imports\Open\Chrome\Chrome\src\simple-ipc-lib\src\pipe_win.h
// Copyright (c) 2010 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////

#include "os_includes.h"
#include "ipc_constants.h"

class PipeWinWithMessagePump {
public:
	PipeWinWithMessagePump();
	~PipeWinWithMessagePump();

	// These calls take ownership of the handle
	// and close it when the object (of this class) is destroyed.
	// Callers should not close the handle after the OpenClient/OpenServer calls.
	bool OpenClient(HANDLE pipe);
	bool OpenServer(HANDLE pipe, bool connect = false);

	bool Write(const void* buf, size_t sz);
	bool Read(void* buf, size_t* sz);
	
	bool IsConnected() const { return INVALID_HANDLE_VALUE != pipe_; }

	void SetRunMsgPump(bool b_run_msg_pump) { b_run_msg_pump_ = b_run_msg_pump; }


private:
	HANDLE pipe_; // We own this handle.
	bool b_run_msg_pump_;
};

class PipeTransportWithMessagePump : public PipeWinWithMessagePump {
public:
	static const size_t kBufferSz = 4096;

	size_t Send(const void* buf, size_t sz) {
		 return Write(buf, sz) ? ipc::RcOK : ipc::RcErrTransportWrite;
	}

	char* Receive(size_t* size);

private:
	IPCCharVector buf_;
};

#endif //SDKSANDBOXPIPEWIN_H