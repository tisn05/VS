/*
	SampleCsp::ContainerMgr
*/


#include "stdafx.h"
#include "scspPinDialog.h"

using namespace scsp;
using namespace sample;


static HCERTSTORE openPFX( const Buffer& inPFX, LPCWSTR inPassword )
{
	DATA_BLOB pfxBlob;
	pfxBlob.pbData = const_cast<BYTE*>( &inPFX[0] );
	pfxBlob.cbData = static_cast<DWORD>( inPFX.size() );
	return PFXImportCertStore( &pfxBlob, inPassword, 0 );
}

static bool makeContainerName( std::string& outContainerName )
{
	UUID uuid;
	RPC_STATUS status = UuidCreate( &uuid );
	if( RPC_S_UUID_LOCAL_ONLY != status && RPC_S_UUID_NO_ADDRESS != status && RPC_S_OK != status ) 
		return false;

	RpcStringHandle uuidString;
	if( RPC_S_OK != UuidToStringA( &uuid, &uuidString.rpcString ) ) 
		return false;

	outContainerName = CONTAINER_PREFIX;
	outContainerName += reinterpret_cast<LPSTR>( uuidString.rpcString );
	return true;
}

static bool makeContainer( const std::string& inContainerName, unsigned int inVersion, const std::wstring& inPath, const Buffer& inDigest, bool bMachineKeySet )
{
	// open the base registry key
	RegKeyHandle baseKey;
	if( bMachineKeySet ) {
		if( ERROR_SUCCESS != RegCreateKeyExA( HKEY_LOCAL_MACHINE, CONTAINER_BASE_KEY, 0, NULL, 0, KEY_WRITE, NULL, &baseKey.hReg, NULL ) ) 
			return false;
	} else {
		if( ERROR_SUCCESS != RegCreateKeyExA( HKEY_CURRENT_USER, CONTAINER_BASE_KEY, 0, NULL, 0, KEY_WRITE, NULL, &baseKey.hReg, NULL ) ) 
			return false;
	}

	// create registry key using this container name
	RegKeyHandle key;
	if( ERROR_SUCCESS != RegCreateKeyExA( baseKey.hReg, inContainerName.c_str(), 0, NULL, 0, KEY_WRITE, NULL, &key.hReg, NULL ) ) 
		return false;

	// create values for the container under this key
	if( ERROR_SUCCESS != RegSetValueExA( key.hReg, CONTAINER_VERSION_KEY, 0, REG_DWORD, reinterpret_cast<BYTE*>(&inVersion), sizeof(inVersion) ) )
		return false;
	
	DWORD pathSize = static_cast<DWORD>( ( inPath.size() + 1 ) * sizeof( inPath[0] ) );
	if( ERROR_SUCCESS != RegSetValueExW( key.hReg, CONTAINER_PATH_KEY, 0, REG_SZ, reinterpret_cast<const BYTE*>(inPath.c_str()), pathSize ) )
		return false;
	
	if( ERROR_SUCCESS != RegSetValueExA( key.hReg, CONTAINER_DIGEST_KEY, 0, REG_BINARY, &inDigest[0], static_cast<DWORD>(inDigest.size()) ) ) 
		return false;

	return true;
}

/*
** Container implementation
*/

scsp::Container::Container( LPCSTR inContainer, bool inDefault, bool inAllowUI, bool bMachineKeySet ) :
m_AllowUI( inAllowUI ),
m_Default( inDefault ),
m_Registered( false ),
m_MachineKeySet( bMachineKeySet ),
m_ContainerName( inDefault ? CONTAINER_DEFAULT : "" ),
m_Path(),
m_CertDigest(),
m_PFX(),
m_SecuredPin(),
m_Store(),
m_Provider(),
m_HWND(NULL)
{
	// if not the default container - make a new container name
	if( !inDefault ) {
		if( NULL == inContainer ) {
			if( !makeContainerName( m_ContainerName ) )
				throw sample::Exception( GetLastError(), 
					"failed creating container name", 
					__LINE__, __FILE__ );
		} else
			m_ContainerName = inContainer;
	}
}

scsp::Container::Container( LPCSTR inContainer, LPCWSTR inPath, const Buffer& inCertDigest, bool inAllowUI, bool bMachineKeySet ) :
m_AllowUI( inAllowUI ),
m_Default( false ),
m_Registered( true ),
m_MachineKeySet( bMachineKeySet ),
m_ContainerName( inContainer ),
m_Path( inPath ),
m_CertDigest( inCertDigest ),
m_PFX(),
m_SecuredPin(),
m_Store(),
m_Provider(),
m_HWND()
{
	// load the PFX into memory
	if( !sample::loadFileFromUri( inPath, inAllowUI, m_PFX ) )
		throw sample::Exception( GetLastError(), "failed loading PFX", __LINE__, __FILE__ );

	// verify this is a PFX
	DATA_BLOB blob;
	blob.pbData = m_PFX.begin();
	blob.cbData = static_cast<DWORD>( m_PFX.size() );
	if( !PFXIsPFXBlob( &blob ) )
		throw sample::Exception( GetLastError(), "not a valid PFX", __LINE__, __FILE__ );
}

struct FindCertToRegister
{
	bool found;
	Buffer provInfo;
	Buffer digest;

	FindCertToRegister() : found(false) {}

	bool operator()( PCCERT_CONTEXT inCert ) {
		if( getCertificateProperty( inCert, CERT_KEY_PROV_INFO_PROP_ID, &provInfo ) && 
			getCertificateProperty( inCert, CERT_HASH_PROP_ID, &digest ) ) {
			found = true;
		}

		return !found;
	}

};

// This call will load the PFX from the location specified and register the container
bool scsp::Container::registerPFX( LPCWSTR inPath )
{
	// am I already registered?
	if( isRegistered() ) {
		SetLastError( ERROR_NOT_SUPPORTED );
		return false;
	}

	// is this a supported URI?
	if( !sample::isUriSupported( inPath ) ) {
		SetLastError( ERROR_NOT_SUPPORTED );
		return false;
	}

	// load the PFX into memory
	Buffer pfx;
	if( !sample::loadFileFromUri( inPath, m_AllowUI, pfx ) ) 
		return false;

	// get the PIN
	Buffer pinBuffer;
	if( !getPin( pinBuffer ) ) 
		return false;

	LPCWSTR pin = reinterpret_cast<LPCWSTR>( pinBuffer.begin() );

	// open the PFX as a certificate store
	StoreHandle pfxStore( openPFX( pfx, pin ) );
	pinBuffer.clear(); // clear out the PIN
	if( NULL == pfxStore.get() ) {
		sample::displayErrMsg (L"Container::registerPFX:PFXImportCertStore");
		return false;
	}

	//XXX	If multiple certificates with private keys are found, we could allow 
	//		the user to choose. Just a thought. For now - first one wins.

	// find certificate to register
	FindCertToRegister finder;
	EnumerateCerts( pfxStore.get(), finder );
	if( !finder.found ) {
		return false;
	}
	CRYPT_KEY_PROV_INFO* provInfoPtr = reinterpret_cast<CRYPT_KEY_PROV_INFO*>( finder.provInfo.begin() );

	// acquire the PFX container
	HCRYPTPROV hProv = NULL;
	if( !CryptAcquireContextW( &hProv, provInfoPtr->pwszContainerName, provInfoPtr->pwszProvName, PROV_RSA_FULL, 0) ){
		sample::displayErrMsg(L"Container::registerPFX:CryptAcquireContextW");
		return false;
	}
	ProviderHandle pfxProv( hProv );

	// update container state
	m_Path = inPath;
	m_PFX.swap( pfx );
	m_Provider.swap( pfxProv );
	m_Store.swap( pfxStore );

	// register the container
	return makeContainer( m_ContainerName, CONTAINER_VERSION, m_Path, finder.digest, m_MachineKeySet );
}

bool scsp::Container::getPin( Buffer& outPin )
{
	// get the PIN directly from the user if allowed
	if( m_AllowUI )
		return getPinFromUser( outPin, m_HWND );
	// do I have a pin to use?
	else if( !m_SecuredPin.empty() && !m_SecuredPin.getUnprotectedData( outPin ) ) 
		return false;
	
	return false;
}

// Save a secure copy and a digested copy of the PIN
void scsp::Container::setPin( LPCWSTR inPin ) 
{
	if( inPin ) {
		const size_t pinSize = ( wcslen( inPin ) + 1 ) * sizeof(wchar_t);
		m_SecuredPin.setProtectedData( reinterpret_cast<const BYTE*>(inPin), pinSize );
	}
}

struct FindCertAndGetContainer
{
	Buffer digest;
	bool found;
	std::wstring provName;
	std::wstring container;

	FindCertAndGetContainer( Buffer& inDigest ) : digest(), found(false), provName(), container() {
		digest.swap( inDigest );
	}

	bool operator()( PCCERT_CONTEXT inCert ) {
		Buffer thisDigest;
		if( getCertificateProperty( inCert, CERT_HASH_PROP_ID, &thisDigest ) && digest == thisDigest ) {
			Buffer provInfo;
			if( getCertificateProperty( inCert, CERT_KEY_PROV_INFO_PROP_ID, &provInfo ) ) {
				CRYPT_KEY_PROV_INFO* provInfoPtr = reinterpret_cast<CRYPT_KEY_PROV_INFO*>( provInfo.begin() );
				provName = provInfoPtr->pwszProvName;
				container = provInfoPtr->pwszContainerName;
				found = true;
			}
		}

		return !found; 
	}
};

// open the PFX using the password and cache the handles needed
bool scsp::Container::accessPrivateKey() 
{
	// special case for default and unregistered containers
	if( isDefault() || !isRegistered() ) 
		return false;

	// don't try again if I have already opened the PFX
	if( NULL == m_Store.get() ) {
		// get the PIN
		Buffer pinBuffer;
		if( !getPin( pinBuffer ) ) 
			return false;
		
		LPCWSTR pin = reinterpret_cast<LPCWSTR>( pinBuffer.begin() );

		// open the PFX as a store
		StoreHandle tmpStoreHandle( openPFX( m_PFX, pin ) );
		pinBuffer.clear(); // clear out the password
		if( NULL == tmpStoreHandle.get() ) {
			sample::displayErrMsg (L"Container::accessPrivateKey:PFXImportCertStore");
			return false;
		}
		m_Store.swap( tmpStoreHandle );

		// find the certificate in the PFX matching the container
		FindCertAndGetContainer finder( m_CertDigest );
		EnumerateCerts( m_Store.get(), finder );
		if( !finder.found ) 
			return false;

		// acquire the PFX container
		HCRYPTPROV hProv = NULL;
		if( !CryptAcquireContextW( &hProv, finder.container.c_str(), finder.provName.c_str(), PROV_RSA_FULL, 0) ){
			sample::displayErrMsg (L"Container::accessPrivateKey:CryptAcquireContextW");
			return false;
		}
		ProviderHandle tmpProviderHandle( hProv );
		m_Provider.swap( tmpProviderHandle );
	}

	return true;
}

HCRYPTPROV scsp::Container::getProviderHandle() const 
{ 
	if( isDefault() )
		return scsp::getGlobalData().m_HostProvider.get();

	return m_Provider.get();
}
