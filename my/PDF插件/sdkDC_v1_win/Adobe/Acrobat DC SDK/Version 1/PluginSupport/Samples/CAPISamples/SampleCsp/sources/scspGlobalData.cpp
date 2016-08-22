/*
	scsp::GlobalData

	This file was created by Adobe engineer.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#include "stdafx.h"

using namespace scsp;

// This is where the SINGLETON instance of GlobalData is stored
namespace scsp {
	GlobalData* globalData = NULL;
}


// This is where the SINGLETON instance of GlobalData is accessed
// This will throw an exception if called before GlobalData::initialize().
GlobalData& scsp::getGlobalData()
{
	assert( NULL != globalData );
	if( NULL == globalData )
		throw sample::Exception( ERROR_INVALID_ACCESS, "GlobalData not initialized", __LINE__, __FILE__ );
	return *globalData;
}


// This is the construction of the GlobalData
// This is provided to allow the construction to happen at a time of my choosing
void scsp::GlobalData::initialize( HANDLE inInstance )
{
	// make sure we have an instance
	assert( NULL == globalData );
	if( NULL == globalData ) 
		globalData = new GlobalData();
	if( NULL == globalData ) 
		throw sample::Exception( GetLastError(), "Cannot create GlobalData", __LINE__, __FILE__ );

	// save the instance handle
	globalData->m_Instance = static_cast<HMODULE>( inInstance );

	// acquire the host CSP to use for pass-thru
	HCRYPTPROV hProv = NULL;
	if( !CryptAcquireContextW( &hProv, NULL, MS_ENHANCED_PROV_W, PROV_RSA_FULL, 0 ) ) {

		// check if missing keyset was the problem
		if( GetLastError() != NTE_BAD_KEYSET )
			throw sample::Exception(GetLastError(), "Cannot acquire " MS_ENHANCED_PROV_A, __LINE__, __FILE__ );

		// create a new keyset
		if(!CryptAcquireContextW( &hProv, NULL, MS_ENHANCED_PROV_W, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			throw sample::Exception(GetLastError(), "Cannot create new keyset", __LINE__, __FILE__ );
	}

	sample::ProviderHandle tmpProvider( hProv );
	globalData->m_HostProvider.swap( tmpProvider );
}


// This is the destruction of GlobalData
// This is provided to allow the destruction to happen at a time of my choosing
void scsp::GlobalData::terminate()
{
	delete globalData;
	globalData = NULL;
}


scsp::GlobalData::~GlobalData()
{
}


scsp::GlobalData::GlobalData() : 
m_Instance(NULL), m_HandleMgr(), m_ContainerMgr(), m_HostProvider(NULL)
{
}
