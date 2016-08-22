/*
	scsp::FileLoader

	This file was created by Adobe engineer.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#include "stdafx.h"

using namespace scsp;
using namespace sample;


bool scsp::ContainerFactory::isRegisteredContainer( LPCSTR inContainerName, bool bMachineKeySet )
{
	// is this the default container?
	if( NULL == inContainerName ) 
		return false;

	// is this a registered container?
	// - this call verifies that the container exists and has the values I need
	unsigned int version;
	std::wstring path;
	Buffer digest;
	return getContainerInfo( inContainerName, version, path, digest, bMachineKeySet );
}

bool scsp::ContainerFactory::isRegisteredContainer( LPCWSTR inContainerName, bool bMachineKeySet )
{
	if( NULL == inContainerName ) 
		return false;

	// get a UTF-8 copy of the container name
	std::string utf8;
	if( ! sample::convertUTF16ToUTF8( inContainerName, utf8 ) ) 
		return false;
	
	return isRegisteredContainer( utf8.c_str(), bMachineKeySet );
}

Container* scsp::ContainerFactory::makeRegisteredContainer( LPCSTR inContainerName, bool inAllowUI, bool bMachineKeySet )
{
	if( NULL == inContainerName ) 
		return NULL;

	try {
		// is this a registered container?
		unsigned int version;
		std::wstring path;
		Buffer digest;
		if( getContainerInfo( inContainerName, version, path, digest, bMachineKeySet ) ) {
			Container* container = new Container( inContainerName, path.c_str(), digest, inAllowUI, bMachineKeySet );
			getGlobalData().m_ContainerMgr.addContainer( container );
			return container;
		} 
	}
	catch( Exception& ) {
		//XXX logging?
	}
	catch(...) {
		//XXX logging?
	}

	return NULL;
}

Container* scsp::ContainerFactory::makeRegisteredContainer( LPCWSTR inContainerName, bool inAllowUI, bool bMachineKeySet )
{
	if( NULL == inContainerName ) 
		return NULL;

	// get a UTF-8 copy of the container name
	std::string utf8;
	if( ! sample::convertUTF16ToUTF8( inContainerName, utf8 ) ) 
		return NULL;
	
	return makeRegisteredContainer( utf8.c_str(), inAllowUI, bMachineKeySet );
}

bool scsp::ContainerFactory::isNameInRegistry( LPCSTR inContainerName, bool bMachineKeySet )
{
	// open the base registry key
 	RegKeyHandle baseKey;
	if( bMachineKeySet ) {
		if( ERROR_SUCCESS != RegOpenKeyExA( HKEY_LOCAL_MACHINE, CONTAINER_BASE_KEY, 0, KEY_READ, &baseKey.hReg ) ) 
 			return true;
	} else {
		if( ERROR_SUCCESS != RegOpenKeyExA( HKEY_CURRENT_USER, CONTAINER_BASE_KEY, 0, KEY_READ, &baseKey.hReg ) ) 
 			return true;
	}
  
    DWORD  cSubKeys = 0;      
    // open the base registry key
    if(RegQueryInfoKey(baseKey.hReg, NULL, NULL, NULL, &cSubKeys, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) 
 		return true;
  
    // Enumerate the subkeys, until RegEnumKeyEx fails.   
    if (cSubKeys) {
		CHAR achKey[MAX_KEY_LENGTH];
		DWORD cbName;
        for (unsigned int i = 0; i < cSubKeys; i++) { 
            cbName = MAX_KEY_LENGTH;
 			if( RegEnumKeyExA(baseKey.hReg, i, achKey, &cbName, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) 
 				return true;
			if( !_stricmp( achKey, inContainerName )) 
				return true;
         }
    }  
	return false;
}
		
bool scsp::ContainerFactory::isNameExisting( LPCSTR inContainerName, bool bMachineKeySet )
{
	// Name already existing in registry?
	if( isNameInRegistry( inContainerName, bMachineKeySet ) )
		return true;
	
	// Name for one of the containers in ContainerManager?
	if( getGlobalData().m_ContainerMgr.isNameExisting( inContainerName, bMachineKeySet ) )
		return true;

	return false;
}

Container* scsp::ContainerFactory::makeNewContainer( LPCSTR inContainerName, bool inAllowUI, bool bMachineKeySet )
{
	try {
		if( NULL != inContainerName ) {
			// check for conflicts with existing names
			if( isNameExisting( inContainerName, bMachineKeySet ) ) 
				return NULL;
		}
		Container* container = new Container( inContainerName, false, inAllowUI, bMachineKeySet );
		getGlobalData().m_ContainerMgr.addContainer( container );
		return container;
	}
	catch( Exception& ) {
		//XXX logging?
	}
	catch(...) {
		//XXX logging?
	}

	return NULL;
}

Container* scsp::ContainerFactory::makeNewContainer( LPCWSTR inContainerName, bool inAllowUI, bool bMachineKeySet )
{
	std::string utf8;

	if( NULL != inContainerName ) {
		// get a UTF-8 copy of the container name
		if( ! sample::convertUTF16ToUTF8( inContainerName, utf8 ) ) 
			return NULL;
	}

	return makeNewContainer( utf8.c_str(), inAllowUI, bMachineKeySet );
}

Container* scsp::ContainerFactory::makeDefaultContainer( bool inAllowUI, bool bMachineKeySet )
{
	try {
		Container* container = new Container( NULL, true, inAllowUI, bMachineKeySet );
		getGlobalData().m_ContainerMgr.addContainer( container );
		return container;
	}
	catch( Exception& ) {
		//XXX logging?
	}
	catch(...) {
		//XXX logging?
	}

	return NULL;
}

bool scsp::ContainerFactory::unregister( LPCSTR inContainerName, bool bMachineKeySet )
{
	if( inContainerName ) {

		// open the base registry key
		RegKeyHandle baseKey;
		if( bMachineKeySet ) {
			if( ERROR_SUCCESS != RegOpenKeyExA( HKEY_LOCAL_MACHINE, CONTAINER_BASE_KEY, 0, KEY_READ|KEY_SET_VALUE, &baseKey.hReg ) ) 
				return false;
		} else {
			if( ERROR_SUCCESS != RegOpenKeyExA( HKEY_CURRENT_USER, CONTAINER_BASE_KEY, 0, KEY_READ|KEY_SET_VALUE, &baseKey.hReg ) ) 
				return false;
		}

		// delete the key specified by the container name under this key
		if( ERROR_SUCCESS != RegDeleteKeyA( baseKey.hReg, inContainerName ) ) 
			return false;

		return true;
	}

	return false;
}

bool scsp::ContainerFactory::unregister( LPCWSTR inContainerName, bool bMachineKeySet )
{
	if( inContainerName ) {
		
		// get a UTF-8 copy of the container name
		std::string utf8;
		if( ! sample::convertUTF16ToUTF8( inContainerName, utf8 ) ) 
			return false;
		
		return unregister( utf8.c_str(), bMachineKeySet );
	}

	return false;
}
