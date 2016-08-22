/*
	scsp::GlobalData

	This file was created by Adobe engineer.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#ifndef scspGlobalData_INCLUDED_
#define scspGlobalData_INCLUDED_

#include "scspContainer.h"
#include "scspHandleMgr.h"

namespace scsp {
	class HandleMgr;
	class ContainerMgr;

	// This is the global data used by the CSP. It is a SINGLETON.
	class GlobalData {
	public:
		static void initialize( HANDLE );
		static void terminate();
		~GlobalData();

		HMODULE m_Instance;	
		HandleMgr m_HandleMgr;
		ContainerMgr m_ContainerMgr;
		sample::ProviderHandle m_HostProvider;

	private:
		GlobalData();
		GlobalData( const GlobalData& ); // not implemented
		GlobalData& operator=( const GlobalData& ); // not implemented
	};

	GlobalData& getGlobalData();
}

#endif //scspGlobalData_INCLUDED_
