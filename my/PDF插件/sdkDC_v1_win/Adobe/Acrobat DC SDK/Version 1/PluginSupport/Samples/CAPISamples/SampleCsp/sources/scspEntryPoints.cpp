/////////////////////////////////////////////////////////////////////////////
//  FILE          : csp.c                                                  //
//  DESCRIPTION   : Crypto API interface                                   //
//  AUTHOR        :                                                        //
//  HISTORY       :                                                        //
//                                                                         //
//  Copyright (C) 1993 Microsoft Corporation   All Rights Reserved         //
//																		   //
/*
	This file has been extensively modified by Adobe engineer.
	These modifications are intended to be made publicly available after legal review.
	All modifications are Copyright (C) Adobe Systems 2006.
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

using namespace scsp;
using namespace sample;

/**** Notes on the design have been moved to DesignNotes.htm ****/

/*
 -  CPAcquireContext
 -
 *  Purpose:
 *               The CPAcquireContext function is used to acquire a context
 *               handle to a cryptographic service provider (CSP).
 *
 *
 *  Parameters:
 *               OUT phProv         -  Handle to a CSP
 *               IN  szContainer    -  Pointer to a string which is the
 *                                     identity of the logged on user
 *               IN  dwFlags        -  Flags values
 *               IN  pVTable        -  Pointer to table of function pointers
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPAcquireContext(
    OUT HCRYPTPROV *phProv,
    IN  LPCSTR szContainer,
    IN  DWORD dwFlags,
    IN  PVTableProvStruc pVTable)
{
	// sanity checks
	if( NULL == phProv || NULL == pVTable ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	*phProv = (HCRYPTPROV)NULL;
	bool allowUI = true;
	if( dwFlags & CRYPT_SILENT )
		allowUI = false;
	bool bMachineKeySet = false;
	if( dwFlags & CRYPT_MACHINE_KEYSET ) 
		bMachineKeySet = true;

	// make sure what they are asking for is reasonable
	if( ( dwFlags & CRYPT_NEWKEYSET ) && ( dwFlags & CRYPT_DELETEKEYSET ) ) {
		SetLastError( ERROR_NOT_SUPPORTED );
		return FALSE;
	}
	if( NULL == szContainer && ( dwFlags & CRYPT_DELETEKEYSET ) ) {
		SetLastError( ERROR_NOT_SUPPORTED );
		return FALSE;
	}
	if( NULL != szContainer && !( dwFlags & CRYPT_NEWKEYSET ) 
		&& !ContainerFactory::isRegisteredContainer( szContainer, bMachineKeySet )) {
		SetLastError( static_cast<DWORD>(NTE_KEYSET_NOT_DEF) );
		return FALSE;
	}

	// are they asking to delete the container?
	if( dwFlags & CRYPT_DELETEKEYSET )
		return ( ContainerFactory::unregister( szContainer, bMachineKeySet ) ?  TRUE : FALSE );

	// try to open the container
	Container* container = NULL;
	if( dwFlags & CRYPT_NEWKEYSET ) 
		container = ContainerFactory::makeNewContainer( szContainer, allowUI, bMachineKeySet );
	else if( NULL == szContainer )
		container = ContainerFactory::makeDefaultContainer( allowUI, bMachineKeySet );
	else 
		container = ContainerFactory::makeRegisteredContainer( szContainer, allowUI, bMachineKeySet );
	if( NULL == container ) {
		SetLastError( static_cast<DWORD>(E_FAIL));
		return FALSE;
	}

	// pass back the container handle
	*phProv = reinterpret_cast<HCRYPTPROV>( container );
	return TRUE;
}


/*
-  CPAcquireContext
-
*  Purpose:
*               The CPAcquireContext function is used to acquire a context
*               handle to a cryptographic service provider (CSP).
*
*
*  Parameters:
*               OUT phProv         -  Handle to a CSP
*               IN  szContainer    -  Pointer to a string which is the
*                                     identity of the logged on user
*               IN  dwFlags        -  Flags values
*               IN  pVTable        -  Pointer to table of function pointers
*
*  Returns:
*/

extern "C" BOOL WINAPI
CPAcquireContextW(
				 OUT HCRYPTPROV *phProv,
				 IN  LPCWSTR szContainer,
				 IN  DWORD dwFlags,
				 IN  PVTableProvStrucW pVTable)
{
	// sanity checks
	if( NULL == phProv || NULL == pVTable ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	*phProv = (HCRYPTPROV)NULL;
	bool allowUI = true;
	if( dwFlags & CRYPT_SILENT )
		allowUI = false;
	bool bMachineKeySet = false;
	if( dwFlags & CRYPT_MACHINE_KEYSET ) 
		bMachineKeySet = true;

	// make sure what they are asking for is reasonable
	if( ( dwFlags & CRYPT_NEWKEYSET ) && ( dwFlags & CRYPT_DELETEKEYSET ) ) {
		SetLastError( ERROR_NOT_SUPPORTED );
		return FALSE;
	}
	if( NULL == szContainer && ( dwFlags & CRYPT_DELETEKEYSET ) ) {
		SetLastError( ERROR_NOT_SUPPORTED );
		return FALSE;
	}
	if( NULL != szContainer && ( dwFlags & CRYPT_NEWKEYSET )
		&& !ContainerFactory::isRegisteredContainer( szContainer, bMachineKeySet ) ) {
		SetLastError( static_cast<DWORD>(NTE_KEYSET_NOT_DEF) );
		return FALSE;
	}

	// are they asking to delete the container?
	if( dwFlags & CRYPT_DELETEKEYSET )
		return ( ContainerFactory::unregister( szContainer, bMachineKeySet ) ?  TRUE : FALSE );

	// try to open the container
	Container* container = NULL;
	if( dwFlags & CRYPT_NEWKEYSET ) 
		container = ContainerFactory::makeNewContainer( szContainer, allowUI, bMachineKeySet );
	else if( NULL == szContainer )
		container = ContainerFactory::makeDefaultContainer( allowUI, bMachineKeySet );
	else 
		container = ContainerFactory::makeRegisteredContainer( szContainer, allowUI, bMachineKeySet );
	if( NULL == container ) {
		SetLastError( static_cast<DWORD>(E_FAIL));
		return FALSE;
	}

	// pass back the container handle
	*phProv = reinterpret_cast<HCRYPTPROV>( container );
	return TRUE;
}


/*
 -      CPReleaseContext
 -
 *      Purpose:
 *               The CPReleaseContext function is used to release a
 *               context created by CryptAcquireContext.
 *
 *     Parameters:
 *               IN  phProv        -  Handle to a CSP
 *               IN  dwFlags       -  Flags values
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPReleaseContext(
    IN  HCRYPTPROV hProv,
    IN  DWORD /*dwFlags*/)
{
	// sanity check
	if( NULL == hProv ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// release the container via the container manager
	getGlobalData().m_ContainerMgr.removeContainer( container );
	return TRUE;
}


/*
 -  CPGenKey
 -
 *  Purpose:
 *                Generate cryptographic keys
 *
 *
 *  Parameters:
 *               IN      hProv   -  Handle to a CSP
 *               IN      Algid   -  Algorithm identifier
 *               IN      dwFlags -  Flags values
 *               OUT     phKey   -  Handle to a generated key
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPGenKey(
    IN  HCRYPTPROV hProv,
    IN  ALG_ID Algid,
    IN  DWORD dwFlags,
    OUT HCRYPTKEY *phKey)
{
	// sanity check
	if( NULL == hProv || NULL == phKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	HCRYPTPROV myProv = NULL;
	// if this is creating a public/private key pair - use the PFX container
	if( (AT_SIGNATURE == Algid) || (AT_KEYEXCHANGE == Algid) ) {

		if( container->isDefault() ) {
			SetLastError( ERROR_NOT_SUPPORTED );
			return FALSE;
		}

		if( !container->accessPrivateKey() ) {
			SetLastError( ERROR_INVALID_PASSWORD );
			return FALSE;
		}
		myProv = container->getProviderHandle();
	} else {
		myProv = getGlobalData().m_HostProvider.get();
	}

	if(!myProv) {
		SetLastError( ERROR_INVALID_DATA );
		return FALSE;
	}

	// generate the key using the provider handle determined above
	HCRYPTKEY hKey = NULL;
	if( !CryptGenKey( myProv, Algid, dwFlags, &hKey ) ) {
		return FALSE;
	}

	// stash this away and return handle to the caller
	KeyHandle* myKeyHandle = new KeyHandle( hKey );
	getGlobalData().m_HandleMgr.addHandle( myKeyHandle );
	*phKey = reinterpret_cast<HCRYPTKEY>( myKeyHandle );
	return TRUE;
}


/*
 -  CPDeriveKey
 -
 *  Purpose:
 *                Derive cryptographic keys from base data
 *
 *
 *  Parameters:
 *               IN      hProv      -  Handle to a CSP
 *               IN      Algid      -  Algorithm identifier
 *               IN      hBaseData -   Handle to base data
 *               IN      dwFlags    -  Flags values
 *               OUT     phKey      -  Handle to a generated key
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPDeriveKey(
    IN  HCRYPTPROV hProv,
    IN  ALG_ID Algid,
    IN  HCRYPTHASH hHash,
    IN  DWORD dwFlags,
    OUT HCRYPTKEY *phKey)
{
	// sanity check
	if( NULL == hProv ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// check which provider handle to use
	if( CALG_RSA_KEYX == Algid || CALG_RSA_SIGN == Algid ) {
		if( container->isDefault() ) {
			SetLastError( ERROR_NOT_SUPPORTED );
			return FALSE;
		}

		if( !container->accessPrivateKey() ) {
			SetLastError( ERROR_INVALID_PASSWORD );
			return FALSE;
		}
	}
	HCRYPTPROV myProv = container->getProviderHandle();

	if(!myProv) {
		SetLastError( ERROR_INVALID_DATA );
		return FALSE;
	}

	// derive the key using the provider handle determined above
	HCRYPTKEY hKey = NULL;
	if( !CryptDeriveKey( myProv, Algid, hHash, dwFlags, phKey ) )
		return FALSE;

	// stash this away and return handle to the caller
	KeyHandle* myKeyHandle = new KeyHandle( hKey );
	getGlobalData().m_HandleMgr.addHandle( myKeyHandle );
	*phKey = reinterpret_cast<HCRYPTKEY>( myKeyHandle );
	return TRUE;
}


/*
 -  CPDestroyKey
 -
 *  Purpose:
 *                Destroys the cryptographic key that is being referenced
 *                with the hKey parameter
 *
 *
 *  Parameters:
 *               IN      hProv  -  Handle to a CSP
 *               IN      hKey   -  Handle to a key
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPDestroyKey(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTKEY hKey)
{
	// sanity check
	if( NULL == hProv || NULL == hKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	KeyHandle* key = reinterpret_cast<KeyHandle*>( hKey );
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container )||
		!getGlobalData().m_HandleMgr.isKnown( key ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// destroy the handle
	getGlobalData().m_HandleMgr.removeHandle( key );
	return TRUE;
}


/*
 -  CPSetKeyParam
 -
 *  Purpose:
 *                Allows applications to customize various aspects of the
 *                operations of a key
 *
 *  Parameters:
 *               IN      hProv   -  Handle to a CSP
 *               IN      hKey    -  Handle to a key
 *               IN      dwParam -  Parameter number
 *               IN      pbData  -  Pointer to data
 *               IN      dwFlags -  Flags values
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPSetKeyParam(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTKEY hKey,
    IN  DWORD dwParam,
    IN  CONST BYTE * pbData,
    IN  DWORD dwFlags)
{
	// sanity check
	if( NULL == hProv || NULL == hKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	KeyHandle* key = reinterpret_cast<KeyHandle*>( hKey );
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container )||
		!getGlobalData().m_HandleMgr.isKnown( key ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// call CAPI
	return CryptSetKeyParam( key->get(), dwParam, pbData, dwFlags );
}


/*
 -  CPGetKeyParam
 -
 *  Purpose:
 *                Allows applications to get various aspects of the
 *                operations of a key
 *
 *  Parameters:
 *               IN      hProv      -  Handle to a CSP
 *               IN      hKey       -  Handle to a key
 *               IN      dwParam    -  Parameter number
 *               OUT     pbData     -  Pointer to data
 *               IN      pdwDataLen -  Length of parameter data
 *               IN      dwFlags    -  Flags values
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPGetKeyParam(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTKEY hKey,
    IN  DWORD dwParam,
    OUT LPBYTE pbData,
    IN OUT LPDWORD pcbDataLen,
    IN  DWORD dwFlags)
{
	// sanity check
	if( NULL == hProv || NULL == hKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	KeyHandle* key = reinterpret_cast<KeyHandle*>( hKey );
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container )||
		!getGlobalData().m_HandleMgr.isKnown( key ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// call CAPI
	return CryptGetKeyParam( key->get(), dwParam, pbData, pcbDataLen, dwFlags );
}


/*
 -  CPSetProvParam
 -
 *  Purpose:
 *                Allows applications to customize various aspects of the
 *                operations of a provider
 *
 *  Parameters:
 *               IN      hProv   -  Handle to a CSP
 *               IN      dwParam -  Parameter number
 *               IN      pbData  -  Pointer to data
 *               IN      dwFlags -  Flags values
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPSetProvParam(
    IN  HCRYPTPROV hProv,
    IN  DWORD dwParam,
    IN  CONST BYTE *pbData,
    IN  DWORD dwFlags)
{
	// sanity check
	if( NULL == hProv ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// check for parameters I handle before passing on to CAPI
	BOOL rc = TRUE;
	switch( dwParam ) {
		case PP_ADMIN_PIN:
		case PP_KEYEXCHANGE_PIN:
		case PP_SIGNATURE_PIN:
			if( NULL == pbData ) {
				SetLastError( ERROR_INVALID_PARAMETER );
				rc = FALSE;
			}
			container->setPin( reinterpret_cast<LPCWSTR>(pbData) );
			break;

		case PP_KEYSTORE_URI:
			if( NULL == pbData ) {
				SetLastError( ERROR_INVALID_PARAMETER );
				rc = FALSE;
			}
			rc = container->registerPFX( reinterpret_cast<LPCWSTR>(pbData) );
			break;

		case PP_CLIENT_HWND:
			if( NULL == pbData ) {
				SetLastError( ERROR_INVALID_PARAMETER );
				rc = FALSE;
			}
			container->setWindowHandle( (HWND)pbData );
			// fall through and set in the host CSP as well

		default:
			rc = CryptSetProvParam( 
				container->getProviderHandle(), 
				dwParam, 
				pbData, 
				dwFlags );
			break;
	}

	return rc;
}


/*
 -  CPGetProvParam
 -
 *  Purpose:
 *                Allows applications to get various aspects of the
 *                operations of a provider
 *
 *  Parameters:
 *               IN      hProv      -  Handle to a CSP
 *               IN      dwParam    -  Parameter number
 *               OUT     pbData     -  Pointer to data
 *               IN OUT  pdwDataLen -  Length of parameter data
 *               IN      dwFlags    -  Flags values
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPGetProvParam(
    IN  HCRYPTPROV hProv,
    IN  DWORD dwParam,
    OUT LPBYTE pbData,
    IN OUT LPDWORD pcbDataLen,
    IN  DWORD dwFlags)
{
	// sanity check
	if( NULL == hProv ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	BOOL rc = FALSE;
	switch( dwParam ) {
		case PP_UNIQUE_CONTAINER:
		case PP_CONTAINER: 
			{
				std::string name = container->getContainerName();
				const size_t sizeNeeded = name.size() + 1;
				if( NULL == pcbDataLen ) {
					SetLastError( ERROR_INVALID_PARAMETER );
				}
				else if( NULL == pbData ) {
					*pcbDataLen = static_cast<DWORD>( sizeNeeded );
					rc = TRUE;
				}
				else if( sizeNeeded > *pcbDataLen ) {
					SetLastError( ERROR_MORE_DATA );
				}
				else {
					memmove( pbData, name.c_str(), sizeNeeded );
					rc = TRUE;
				}
			}
			break;

		case PP_IMPTYPE: 
			if( NULL == pcbDataLen ) {
				SetLastError( ERROR_INVALID_PARAMETER );
			}
			else if( NULL == pbData ) {
				*pcbDataLen = sizeof(DWORD);
				rc = TRUE;
			}
			else if( sizeof(DWORD) > *pcbDataLen ) {
				SetLastError( ERROR_MORE_DATA );
			}
			else {
				*(reinterpret_cast<DWORD*>(pbData)) = CRYPT_IMPL_SOFTWARE;
				rc = TRUE;
			}
			break;

		case PP_KEYSTORE_URI: 
			{
				std::wstring path = container->getPath();
				const size_t sizeNeeded = sizeof( path[0] ) * ( path.size() + 1 );
				if( NULL == pcbDataLen ) {
					SetLastError( ERROR_INVALID_PARAMETER );
				}
				else if( NULL == pbData ) {
					*pcbDataLen = static_cast<DWORD>( sizeNeeded );
					rc = TRUE;
				}
				else if( sizeNeeded > *pcbDataLen ) {
					SetLastError( ERROR_MORE_DATA );
				}
				else {
					memmove( pbData, path.c_str(), sizeNeeded );
					rc = TRUE;
				}
			}
			break;

		case PP_NAME:
			if( NULL == pcbDataLen ) {
				SetLastError( ERROR_INVALID_PARAMETER );
			}
			else if( NULL == pbData ) {
				*pcbDataLen = sizeof(SAMPLECSP_NAME_UTF8);
				rc = TRUE;
			}
			else if( sizeof(SAMPLECSP_NAME_UTF8) > *pcbDataLen ) {
				SetLastError( ERROR_MORE_DATA );
			}
			else {
				memcpy( pbData, SAMPLECSP_NAME_UTF8, sizeof(SAMPLECSP_NAME_UTF8) );
				rc = TRUE;
			}
			break;

		case PP_VERSION:
			if( NULL == pcbDataLen ) {
				SetLastError( ERROR_INVALID_PARAMETER );
			}
			else if( NULL == pbData ) {
				*pcbDataLen = sizeof(DWORD);
				rc = TRUE;
			}
			else if( sizeof(DWORD) > *pcbDataLen ) {
				SetLastError( ERROR_MORE_DATA );
			}
			else {
				DWORD version = SAMPLECSP_VERSION;
				memcpy( pbData, reinterpret_cast<unsigned char*>(&version), sizeof(version) );
				rc = TRUE;
			}
			break;

		case PP_KEYEXCHANGE_PIN:
		case PP_ADMIN_PIN:
		case PP_SIGNATURE_PIN: // never expose the PIN
			SetLastError( ERROR_NOT_SUPPORTED );
			break;

		case PP_ENUMCONTAINERS: 
			if( ( dwFlags & CRYPT_MACHINE_KEYSET ) || container->isMachineKeySet() ) {
				SetLastError( ERROR_NO_MORE_ITEMS );
			}
			else if( NULL == pcbDataLen ) {
				SetLastError( ERROR_INVALID_PARAMETER );
			}
			else if( NULL == pbData ) {
				*pcbDataLen = MAX_CONTAINER_LEN;
				rc = TRUE;
			}
			else {
				// keep the current enumeration index
				// ** MSDN says this method is not required to be thread-safe **
				static DWORD currentIndex = 0;
				if( dwFlags & CRYPT_FIRST )
					currentIndex = 0;

				// enumerate the registered containers
				RegKeyHandle baseKey;
				if( ERROR_SUCCESS == RegOpenKeyExA( HKEY_CURRENT_USER, CONTAINER_BASE_KEY, 0, KEY_READ, &baseKey.hReg ) ) {
					DWORD err = RegEnumKeyExA( baseKey.hReg, currentIndex, reinterpret_cast<LPSTR>(pbData), pcbDataLen, 0, NULL, NULL, NULL );
					if( ERROR_SUCCESS == err ) {
						++currentIndex;
						rc = TRUE;
					}
					else if( ERROR_NO_MORE_ITEMS == err ) {
						SetLastError( err ); // The registry call does not set last error in this case
					}
				}
			}
			break;

		case PP_CRYPT_COUNT_KEY_USE:
		case PP_ENUMALGS:
		case PP_ENUMALGS_EX:
		case PP_ENUMEX_SIGNING_PROT:
		case PP_KEYSET_SEC_DESCR:
		case PP_KEYSET_TYPE:
		case PP_KEYSPEC:
		case PP_KEYSTORAGE:
		case PP_KEYX_KEYSIZE_INC:
		case PP_KEY_TYPE_SUBTYPE:
		case PP_PROVTYPE:
		case PP_SIG_KEYSIZE_INC:
		case PP_SGC_INFO:
		case PP_SYM_KEYSIZE:
		case PP_UI_PROMPT:
		case PP_USE_HARDWARE_RNG:
		default: // pass these calls through to the host CSP
			rc = CryptGetProvParam( 
				container->getProviderHandle(), 
				dwParam, 
				pbData, 
				pcbDataLen, 
				dwFlags );
			break;
	}

	return rc; 
}


/*
 -  CPSetHashParam
 -
 *  Purpose:
 *                Allows applications to customize various aspects of the
 *                operations of a hash
 *
 *  Parameters:
 *               IN      hProv   -  Handle to a CSP
 *               IN      hHash   -  Handle to a hash
 *               IN      dwParam -  Parameter number
 *               IN      pbData  -  Pointer to data
 *               IN      dwFlags -  Flags values
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPSetHashParam(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTHASH hHash,
    IN  DWORD dwParam,
    IN  CONST BYTE * pbData,
    IN  DWORD dwFlags )
{
	// sanity check
	if( NULL == hProv || NULL == hHash ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	HashHandle* hash = reinterpret_cast<HashHandle*>( hHash );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( hash ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// call CAPI
	return CryptSetHashParam( hash->get(), dwParam, pbData, dwFlags );
}


/*
 -  CPGetHashParam
 -
 *  Purpose:
 *                Allows applications to get various aspects of the
 *                operations of a hash
 *
 *  Parameters:
 *               IN      hProv      -  Handle to a CSP
 *               IN      hHash      -  Handle to a hash
 *               IN      dwParam    -  Parameter number
 *               OUT     pbData     -  Pointer to data
 *               IN      pdwDataLen -  Length of parameter data
 *               IN      dwFlags    -  Flags values
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPGetHashParam(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTHASH hHash,
    IN  DWORD dwParam,
    OUT LPBYTE pbData,
    IN OUT LPDWORD pcbDataLen,
    IN  DWORD dwFlags )
{
	// sanity check
	if( NULL == hProv || NULL == hHash ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	HashHandle* hash = reinterpret_cast<HashHandle*>( hHash );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( hash ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// call CAPI
	return CryptGetHashParam( hash->get(), dwParam, pbData, pcbDataLen, dwFlags );
}


/*
 -  CPExportKey
 -
 *  Purpose:
 *                Export cryptographic keys out of a CSP in a secure manner
 *
 *
 *  Parameters:
 *               IN  hProv         - Handle to the CSP user
 *               IN  hKey          - Handle to the key to export
 *               IN  hPubKey       - Handle to exchange public key value of
 *                                   the destination user
 *               IN  dwBlobType    - Type of key blob to be exported
 *               IN  dwFlags       - Flags values
 *               OUT pbData        -     Key blob data
 *               IN OUT pdwDataLen - Length of key blob in bytes
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPExportKey(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTKEY hKey,
    IN  HCRYPTKEY hPubKey,
    IN  DWORD dwBlobType,
    IN  DWORD dwFlags,
    OUT LPBYTE pbData,
    IN OUT LPDWORD pcbDataLen )
{
	// sanity check
	if( NULL == hProv || NULL == hKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	KeyHandle* key = reinterpret_cast<KeyHandle*>( hKey );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( key ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// call CAPI
	return CryptExportKey( key->get(), hPubKey, dwBlobType, dwFlags, pbData, pcbDataLen );
}


/*
 -  CPImportKey
 -
 *  Purpose:
 *                Import cryptographic keys
 *
 *
 *  Parameters:
 *               IN  hProv     -  Handle to the CSP user
 *               IN  pbData    -  Key blob data
 *               IN  dwDataLen -  Length of the key blob data
 *               IN  hPubKey   -  Handle to the exchange public key value of
 *                                the destination user
 *               IN  dwFlags   -  Flags values
 *               OUT phKey     -  Pointer to the handle to the key which was
 *                                Imported
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPImportKey(
    IN  HCRYPTPROV hProv,
    IN  CONST BYTE *pbData,
    IN  DWORD cbDataLen,
    IN  HCRYPTKEY hPubKey,
    IN  DWORD dwFlags,
    OUT HCRYPTKEY *phKey)
{
	// sanity check
	if( NULL == hProv || NULL == pbData || NULL == phKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// get the PUBLICKEYSTRUC header
	const PUBLICKEYSTRUC* header = reinterpret_cast<const PUBLICKEYSTRUC*>( pbData );

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	HCRYPTPROV myProv = NULL;
	// check which provider handle to use
	if( PRIVATEKEYBLOB == header->bType ) {

		if( container->isDefault() ) {
			SetLastError( ERROR_NOT_SUPPORTED );
			return FALSE;
		}

		if( !container->accessPrivateKey() ) {
			SetLastError( ERROR_INVALID_PASSWORD );
			return FALSE;
		}
		myProv = container->getProviderHandle();
	} else
		myProv = getGlobalData().m_HostProvider.get();

	if(!myProv) {
		SetLastError( ERROR_INVALID_DATA );
		return FALSE;
	}

	// import the key using the provider handle determined above
	HCRYPTKEY hKey = NULL;
	if( !CryptImportKey( myProv, pbData, cbDataLen, hPubKey, dwFlags, &hKey ) )
		return FALSE;

	// stash this away and return handle to the caller
	KeyHandle* myKeyHandle = new KeyHandle( hKey );
	getGlobalData().m_HandleMgr.addHandle( myKeyHandle );
	*phKey = reinterpret_cast<HCRYPTKEY>( myKeyHandle );
	return TRUE;
}


/*
 -  CPEncrypt
 -
 *  Purpose:
 *                Encrypt data
 *
 *
 *  Parameters:
 *               IN  hProv         -  Handle to the CSP user
 *               IN  hKey          -  Handle to the key
 *               IN  hHash         -  Optional handle to a hash
 *               IN  Final         -  Boolean indicating if this is the final
 *                                    block of plaintext
 *               IN  dwFlags       -  Flags values
 *               IN OUT pbData     -  Data to be encrypted
 *               IN OUT pdwDataLen -  Pointer to the length of the data to be
 *                                    encrypted
 *               IN dwBufLen       -  Size of Data buffer
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPEncrypt(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTKEY hKey,
    IN  HCRYPTHASH hHash,
    IN  BOOL fFinal,
    IN  DWORD dwFlags,
    IN OUT LPBYTE pbData,
    IN OUT LPDWORD pcbDataLen,
    IN  DWORD cbBufLen)
{
	// sanity check
	if( NULL == hProv || NULL == hKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	KeyHandle* key = reinterpret_cast<KeyHandle*>( hKey );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( key ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// encrypt the data using CAPI
	return CryptEncrypt( key->get(), hHash, fFinal, dwFlags, pbData, pcbDataLen, cbBufLen );
}


/*
 -  CPDecrypt
 -
 *  Purpose:
 *                Decrypt data
 *
 *
 *  Parameters:
 *               IN  hProv         -  Handle to the CSP user
 *               IN  hKey          -  Handle to the key
 *               IN  hHash         -  Optional handle to a hash
 *               IN  Final         -  Boolean indicating if this is the final
 *                                    block of ciphertext
 *               IN  dwFlags       -  Flags values
 *               IN OUT pbData     -  Data to be decrypted
 *               IN OUT pdwDataLen -  Pointer to the length of the data to be
 *                                    decrypted
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPDecrypt(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTKEY hKey,
    IN  HCRYPTHASH hHash,
    IN  BOOL fFinal,
    IN  DWORD dwFlags,
    IN OUT LPBYTE pbData,
    IN OUT LPDWORD pcbDataLen)
{
	// sanity check
	if( NULL == hProv || NULL == hKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	KeyHandle* key = reinterpret_cast<KeyHandle*>( hKey );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( key ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// decrypt the data using CAPI
	return CryptDecrypt( key->get(), hHash, fFinal, dwFlags, pbData, pcbDataLen );
}


/*
 -  CPCreateHash
 -
 *  Purpose:
 *                initiate the hashing of a stream of data
 *
 *
 *  Parameters:
 *               IN  hUID    -  Handle to the user identification
 *               IN  Algid   -  Algorithm identifier of the hash algorithm
 *                              to be used
 *               IN  hKey   -   Optional handle to a key
 *               IN  dwFlags -  Flags values
 *               OUT pHash   -  Handle to hash object
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPCreateHash(
    IN  HCRYPTPROV hProv,
    IN  ALG_ID Algid,
    IN  HCRYPTKEY hKey,
    IN  DWORD dwFlags,
    OUT HCRYPTHASH *phHash)
{
	// sanity check
	if( NULL == hProv ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// check which provider handle to use
	if( !container->isDefault() ) {
		if( !container->accessPrivateKey() ) {
			SetLastError( ERROR_INVALID_PASSWORD );
			return FALSE;
		}
	}
	HCRYPTPROV myProv = container->getProviderHandle();

	if(!myProv) {
		SetLastError( ERROR_INVALID_DATA );
		return FALSE;
	}

	// create the hash using the provider
	HCRYPTHASH hHash = NULL;
	if( !CryptCreateHash( myProv, Algid, hKey, dwFlags, &hHash ) )
		return FALSE;

	// stash this away and return handle to the caller
	HashHandle* myHashHandle = new HashHandle( hHash );
	getGlobalData().m_HandleMgr.addHandle( myHashHandle );
	*phHash = reinterpret_cast<HCRYPTHASH>( myHashHandle );
	return TRUE;
}


/*
 -  CPHashData
 -
 *  Purpose:
 *                Compute the cryptographic hash on a stream of data
 *
 *
 *  Parameters:
 *               IN  hProv     -  Handle to the user identification
 *               IN  hHash     -  Handle to hash object
 *               IN  pbData    -  Pointer to data to be hashed
 *               IN  dwDataLen -  Length of the data to be hashed
 *               IN  dwFlags   -  Flags values
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPHashData(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTHASH hHash,
    IN  CONST BYTE * pbData,
    IN  DWORD cbDataLen,
    IN  DWORD dwFlags)
{
	// sanity check
	if( NULL == hProv || NULL == hHash ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	HashHandle* hash = reinterpret_cast<HashHandle*>( hHash );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( hash ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// call CAPI
	return CryptHashData( hash->get(), pbData, cbDataLen, dwFlags );
}


/*
 -  CPHashSessionKey
 -
 *  Purpose:
 *                Compute the cryptographic hash on a key object.
 *
 *
 *  Parameters:
 *               IN  hProv     -  Handle to the user identification
 *               IN  hHash     -  Handle to hash object
 *               IN  hKey      -  Handle to a key object
 *               IN  dwFlags   -  Flags values
 *
 *  Returns:
 *               CRYPT_FAILED
 *               CRYPT_SUCCEED
 */

extern "C" BOOL WINAPI
CPHashSessionKey(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTHASH hHash,
    IN  HCRYPTKEY hKey,
    IN  DWORD dwFlags)
{
	// sanity check
	if( NULL == hProv || NULL == hHash || NULL == hKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	HashHandle* hash = reinterpret_cast<HashHandle*>( hHash );
	KeyHandle* key = reinterpret_cast<KeyHandle*>( hKey );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( hash ) || 
		!getGlobalData().m_HandleMgr.isKnown( key ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// call CAPI
	return CryptHashSessionKey( hash->get(), key->get(), dwFlags );
}


/*
 -  CPSignHash
 -
 *  Purpose:
 *                Create a digital signature from a hash
 *
 *
 *  Parameters:
 *               IN  hProv        -  Handle to the user identification
 *               IN  hHash        -  Handle to hash object
 *               IN  dwKeySpec    -  Key pair to that is used to sign with
 *               IN  sDescription -  Description of data to be signed
 *               IN  dwFlags      -  Flags values
 *               OUT pbSignature  -  Pointer to signature data
 *               IN OUT dwHashLen -  Pointer to the len of the signature data
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPSignHash(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTHASH hHash,
    IN  DWORD dwKeySpec,
    IN  LPCWSTR szDescription,
    IN  DWORD dwFlags,
    OUT LPBYTE pbSignature,
    IN OUT LPDWORD pcbSigLen)
{
	// sanity check
	if( NULL == hProv || NULL == hHash ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	HashHandle* hash = reinterpret_cast<HashHandle*>( hHash );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( hash ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// NOTE: For this to work - the hash object must have been created with PFX provider handle.
	//		 CAPI calls the provider to sign via internal data member not directly exposed.

	// call CAPI
	return CryptSignHash( hash->get(), dwKeySpec, szDescription, dwFlags, pbSignature, pcbSigLen );
}


/*
 -  CPDestroyHash
 -
 *  Purpose:
 *                Destroy the hash object
 *
 *
 *  Parameters:
 *               IN  hProv     -  Handle to the user identification
 *               IN  hHash     -  Handle to hash object
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPDestroyHash(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTHASH hHash)
{
	// sanity check
	if( NULL == hProv || NULL == hHash ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	HashHandle* hash = reinterpret_cast<HashHandle*>( hHash );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( hash ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// release the hash
	getGlobalData().m_HandleMgr.removeHandle( hash );
	return TRUE;
}


/*
 -  CPVerifySignature
 -
 *  Purpose:
 *                Used to verify a signature against a hash object
 *
 *
 *  Parameters:
 *               IN  hProv        -  Handle to the user identification
 *               IN  hHash        -  Handle to hash object
 *               IN  pbSignture   -  Pointer to signature data
 *               IN  dwSigLen     -  Length of the signature data
 *               IN  hPubKey      -  Handle to the public key for verifying
 *                                   the signature
 *               IN  sDescription -  String describing the signed data
 *               IN  dwFlags      -  Flags values
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPVerifySignature(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTHASH hHash,
    IN  CONST BYTE * pbSignature,
    IN  DWORD cbSigLen,
    IN  HCRYPTKEY hPubKey,
    IN  LPCWSTR szDescription,
    IN  DWORD dwFlags)
{
	// sanity check
	if( NULL == hProv || NULL == hHash || NULL == hPubKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	HashHandle* hash = reinterpret_cast<HashHandle*>( hHash );
	KeyHandle* pubkey = reinterpret_cast<KeyHandle*>( hPubKey );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) || 
		!getGlobalData().m_HandleMgr.isKnown( hash ) || 
		!getGlobalData().m_HandleMgr.isKnown(pubkey) ) 
	{
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// call CAPI
	return CryptVerifySignature( hash->get(), pbSignature, cbSigLen, pubkey->get(), szDescription, dwFlags );
}


/*
 -  CPGenRandom
 -
 *  Purpose:
 *                Used to fill a buffer with random bytes
 *
 *
 *  Parameters:
 *               IN  hProv         -  Handle to the user identification
 *               IN  dwLen         -  Number of bytes of random data requested
 *               IN OUT pbBuffer   -  Pointer to the buffer where the random
 *                                    bytes are to be placed
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPGenRandom(
    IN  HCRYPTPROV hProv,
    IN  DWORD cbLen,
    OUT LPBYTE pbBuffer)
{
	// sanity check
	if( NULL == hProv ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// call CAPI
	return CryptGenRandom( getGlobalData().m_HostProvider.get(), cbLen, pbBuffer );
}


/*
 -  CPGetUserKey
 -
 *  Purpose:
 *                Gets a handle to a permanent user key
 *
 *
 *  Parameters:
 *               IN  hProv      -  Handle to the user identification
 *               IN  dwKeySpec  -  Specification of the key to retrieve
 *               OUT phUserKey  -  Pointer to key handle of retrieved key
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPGetUserKey(
    IN  HCRYPTPROV hProv,
    IN  DWORD dwKeySpec,
    OUT HCRYPTKEY *phUserKey)
{
	// sanity check
	if( NULL == hProv ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// try to access to the private key
	if( !container->accessPrivateKey() ) {
		SetLastError( ERROR_INVALID_PASSWORD );
		return FALSE;
	}

	// call CAPI
	HCRYPTKEY hKey = NULL;
	if( !CryptGetUserKey( container->getProviderHandle(), dwKeySpec, &hKey ) )
		return FALSE;

	// stash this away and return handle to the caller
	KeyHandle* myKeyHandle = new KeyHandle( hKey );
	getGlobalData().m_HandleMgr.addHandle( myKeyHandle );
	*phUserKey = reinterpret_cast<HCRYPTKEY>( myKeyHandle );
	return TRUE;
}


/*
 -  CPDuplicateHash
 -
 *  Purpose:
 *                Duplicates the state of a hash and returns a handle to it.
 *                This is an optional entry.  Typically it only occurs in
 *                SChannel related CSPs.
 *
 *  Parameters:
 *               IN      hUID           -  Handle to a CSP
 *               IN      hHash          -  Handle to a hash
 *               IN      pdwReserved    -  Reserved
 *               IN      dwFlags        -  Flags
 *               IN      phHash         -  Handle to the new hash
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPDuplicateHash(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTHASH hHash,
    IN  LPDWORD pdwReserved,
    IN  DWORD dwFlags,
    OUT HCRYPTHASH * phHash)
{
	// sanity check
	if( NULL == hProv || NULL == hHash ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	HashHandle* hash = reinterpret_cast<HashHandle*>( hHash );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ||
		!getGlobalData().m_HandleMgr.isKnown( hash ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// duplicate the key
	HCRYPTHASH hDupHash = NULL;
	if( !CryptDuplicateHash( hash->get(), pdwReserved, dwFlags, &hDupHash ) )
		return FALSE;

	// stash this away and return handle to the caller
	HashHandle* myHashHandle = new HashHandle( hDupHash );
	getGlobalData().m_HandleMgr.addHandle( myHashHandle );
	*phHash = reinterpret_cast<HCRYPTHASH>( myHashHandle );
	return TRUE;
}


/*
 -  CPDuplicateKey
 -
 *  Purpose:
 *                Duplicates the state of a key and returns a handle to it.
 *                This is an optional entry.  Typically it only occurs in
 *                SChannel related CSPs.
 *
 *  Parameters:
 *               IN      hUID           -  Handle to a CSP
 *               IN      hKey           -  Handle to a key
 *               IN      pdwReserved    -  Reserved
 *               IN      dwFlags        -  Flags
 *               IN      phKey          -  Handle to the new key
 *
 *  Returns:
 */

extern "C" BOOL WINAPI
CPDuplicateKey(
    IN  HCRYPTPROV hProv,
    IN  HCRYPTKEY hKey,
    IN  LPDWORD pdwReserved,
    IN  DWORD dwFlags,
    OUT HCRYPTKEY * phKey)
{
	// sanity check
	if( NULL == hProv || NULL == hKey ) {
		SetLastError( ERROR_INVALID_PARAMETER );
		return FALSE;
	}

	// ownership check
	Container* container = reinterpret_cast<Container*>( hProv );
	KeyHandle* key = reinterpret_cast<KeyHandle*>( hKey );
	if( !getGlobalData().m_ContainerMgr.isKnown( container ) ||
		!getGlobalData().m_HandleMgr.isKnown( key ) ) {
		SetLastError( ERROR_INVALID_HANDLE );
		return FALSE;
	}

	// duplicate the key
	HCRYPTHASH hDupKey = NULL;
	if( !CryptDuplicateKey( key->get(), pdwReserved, dwFlags, &hDupKey ) )
		return FALSE;

	// stash this away and return handle to the caller
	KeyHandle* myKeyHandle = new KeyHandle( hDupKey );
	getGlobalData().m_HandleMgr.addHandle( myKeyHandle );
	*phKey = reinterpret_cast<HCRYPTHASH>( myKeyHandle );
	return TRUE;
}

