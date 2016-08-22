// SampleRegistrar.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace sample;

#define ERR_PARAMS			-1
#define ERR_FILE			-2
#define ERR_PASSWORD		-3
#define ERR_BAD_PFX			-4
#define ERR_CAPI			-5
#define ERR_UNICODE			-6
#define ERR_UNKNOWN			-7

#if _DEBUG
#define DEBUG 1
#endif

// The discussion of overall design and decisions made is in "DesignNotes.htm" in the SampleCsp project

/*
	This application has these functions:
	 1> install certificates into the MY store and link them to the SampleCsp
	 2> uninstall certificate from the MY store that are linked to the SampleCsp
	 3> list the certificates with key containers linked to the SampleCsp

    Here is the usage:
	-r <file> <password>			==> register first certificate from this PFX/PFX
	-u <hex-encoded-sha1-digest>	==> un-register certificate matching this digest
	-l                              ==> list registered certificates
*/

struct FindCertToRegister
{
	bool found;
	Buffer cert;
	DWORD keySpec;

	FindCertToRegister() : found(false), cert(), keySpec(0) {}

	bool operator()( PCCERT_CONTEXT inCert ) {
		Buffer provInfo;
		if( getCertificateProperty( inCert, CERT_KEY_PROV_INFO_PROP_ID, &provInfo ) ) {
			cert.resize( inCert->cbCertEncoded );
			for( size_t i = 0; i < inCert->cbCertEncoded; ++i )
				cert[i] = inCert->pbCertEncoded[i];
			CRYPT_KEY_PROV_INFO* provInfoPtr = reinterpret_cast<CRYPT_KEY_PROV_INFO*>( provInfo.begin() );
			keySpec = provInfoPtr->dwKeySpec;
			found = true;
		}

		return !found;
	}

};

static int registerPFX( LPCTSTR inPath, LPCTSTR inPassword ) 
{
	// load the PFX
	Buffer pfx;
#if UNICODE
	if( !sample::isUriSupported( inPath ) || !sample::loadFileFromUri( inPath, true, pfx ) ) {
		_tprintf( TEXT( "registerPFX: failed to load the PFX file. PFX registration failed.\n" ) );
		return ERR_FILE;
	}
#else
	std::wstring wide_path;
	if( !sample::convertUTF8ToUTF16( inPath, wide_path ) ) {
		printf( "registerPFX: failed convert the PFX name to UTF-16. PFX registration failed.\n" );
		return ERR_UNICODE;
	}
	if( !sample::isUriSupported( wide_path.c_str() ) || !sample::loadFileFromUri( wide_path.c_str(), true, pfx ) ) {
		printf( "registerPFX: failed to load the PFX file. PFX registration failed.\n" );
		return ERR_FILE;
	}
#endif

	// try to open the PFX
	DATA_BLOB pfxBlob;
	pfxBlob.pbData = pfx.begin();
	pfxBlob.cbData = static_cast<DWORD>( pfx.size() );
	StoreHandle pfxStore(  PFXImportCertStore( &pfxBlob, inPassword, 0 ) );
	if( NULL == pfxStore.get() ) {
		sample::displayErrMsg( L"registerPFX:PFXImportCertStore" );
		return ERR_PASSWORD;
	}

	// find a certificate to register
	FindCertToRegister finder;
	EnumerateCerts( pfxStore.get(), finder );
	if( !finder.found ) {
		_tprintf( TEXT( "registerPFX: failed to find a certificate to register. PFX registration failed.\n" ) );
		return ERR_BAD_PFX;
	}

	// get a new container from the CSP
	HCRYPTPROV hProv = NULL;
	if( !CryptAcquireContextW( &hProv, NULL, SAMPLECSP_NAME_UTF16, PROV_RSA_FULL, CRYPT_NEWKEYSET ) ) {
		sample::displayErrMsg( L"registerPFX:CryptAcquireContextW" );
		return ERR_CAPI;
	}
	ProviderHandle provHandle( hProv );

	// tell the CSP what the password is
#if UNICODE
	if( !CryptSetProvParam( hProv, PP_ADMIN_PIN, reinterpret_cast<const BYTE*>(inPassword), 0 ) ) {
		sample::displayErrMsg( L"registerPFX:CryptSetProvParam" );
		return ERR_CAPI;
	}
#else
	std::wstring wide_password;
	if( !sample::convertUTF8ToUTF16( inPassword, wide_password ) ){
		printf( "registerPFX: failed to convert PFX password to UTF-16.\n" );
		return ERR_UNICODE;
	}
	if( !CryptSetProvParam( hProv, PP_ADMIN_PIN, reinterpret_cast<const BYTE*>(wide_password.c_str()), 0 ) ) {
		printf( "registerPFX: failed to set password for the PFX file.\n" );
		sample::displayErrMsg( L"registerPFX:CryptSetProvParam" );
		return ERR_CAPI;
	}
#endif

	// tell the CSP where the PFX file is
	// NOTE: The CSP will try to open the file and get the container info
#if UNICODE
	if( !CryptSetProvParam( hProv, PP_KEYSTORE_URI, reinterpret_cast<const BYTE*>(inPath), 0 ) )
		return ERR_CAPI;
#else
	if( !CryptSetProvParam( hProv, PP_KEYSTORE_URI, reinterpret_cast<const BYTE*>(wide_path.c_str()), 0 ) ){
		sample::displayErrMsg( L"registerPFX:CryptSetProvParam" );
		return ERR_CAPI;
	}
#endif

	// get the container name
	DWORD size = 0;
	if( !CryptGetProvParam( hProv, PP_CONTAINER, NULL, &size, 0 ) ){
		sample::displayErrMsg( L"registerPFX:CryptGetProvParam" );
		return ERR_CAPI;
	}
	std::string container;
	container.resize( size, '\0' );
	if( !CryptGetProvParam( hProv, PP_CONTAINER, reinterpret_cast<BYTE*>( &container[0] ), &size, 0 ) ){
		sample::displayErrMsg( L"registerPFX:CryptGetProvParam" );
		return ERR_CAPI;
	}
	container.reserve();
	std::wstring wide_container;
	if( !sample::convertUTF8ToUTF16( container, wide_container ) ) 
		return ERR_UNICODE;

	// add this certificate to "My" store as an encoded blob
	PCCERT_CONTEXT hCert = NULL;
	StoreHandle myStore( CertOpenStore( CERT_STORE_PROV_SYSTEM, ALL_ENCODINGS, NULL, CERT_SYSTEM_STORE_CURRENT_USER, MY_STORE_NAME ) );
	if( !CertAddEncodedCertificateToStore( myStore.get(), ALL_ENCODINGS, finder.cert.begin(), static_cast<DWORD>(finder.cert.size()), CERT_STORE_ADD_NEW, &hCert ) ) {
		sample::displayErrMsg( L"registerPFX:CertAddEncodedCertificateToStore" );
		// certificate registration did not go through, remove the registry entry
		HCRYPTPROV hProv1 = NULL;
		CryptAcquireContextW( &hProv1, wide_container.c_str(), SAMPLECSP_NAME_UTF16, PROV_RSA_FULL, CRYPT_DELETEKEYSET );
		return ERR_CAPI;
	}
	CertHandle newCert( hCert );

	// link certificate to the container
	CRYPT_KEY_PROV_INFO newProvInfo;
	newProvInfo.pwszProvName = SAMPLECSP_NAME_UTF16;
	newProvInfo.dwProvType = PROV_RSA_FULL;
	newProvInfo.pwszContainerName = &wide_container[0];
	newProvInfo.dwKeySpec = finder.keySpec;
	newProvInfo.dwFlags = 0;
	newProvInfo.cProvParam = 0;
	newProvInfo.rgProvParam = NULL;
	if( !CertSetCertificateContextProperty( hCert, CERT_KEY_PROV_INFO_PROP_ID, 0, &newProvInfo ) ) {
		sample::displayErrMsg( L"registerPFX:CertSetCertificateContextProperty" );
		HCRYPTPROV hProv1 = NULL;
		// certificate registration could not complete, remove the registry entry
		CryptAcquireContextW( &hProv1, wide_container.c_str(), SAMPLECSP_NAME_UTF16, PROV_RSA_FULL, CRYPT_DELETEKEYSET );
		return ERR_CAPI;
	}
	
	// link the certificate to the CSP
	CERT_KEY_CONTEXT newKeyContext;
	newKeyContext.cbSize = sizeof( newKeyContext );
	newKeyContext.dwKeySpec = finder.keySpec;
	newKeyContext.hCryptProv = hProv;
	if( !CertSetCertificateContextProperty( hCert, CERT_KEY_CONTEXT_PROP_ID, CERT_SET_PROPERTY_IGNORE_PERSIST_ERROR_FLAG, &newKeyContext ) )  {
		sample::displayErrMsg( L"registerPFX:CertSetCertificateContextProperty" );
		HCRYPTPROV hProv1 = NULL;
		// certificate registration could not complete, remove the registry entry
		CryptAcquireContextW( &hProv1, wide_container.c_str(), SAMPLECSP_NAME_UTF16, PROV_RSA_FULL, CRYPT_DELETEKEYSET );
		return ERR_CAPI;
	}

	_tprintf( TEXT( "Succeeded in registering %s.\n" ), inPath );

	return 0;
}

struct DeleteCertWithMatchingDigest
{
	Buffer digest;
	bool found;
	std::wstring szContainer;

	DeleteCertWithMatchingDigest( Buffer& inDigest ) : digest(inDigest), found(false), szContainer(TEXT("")) {
		digest.swap( inDigest );
	}

	bool operator()( PCCERT_CONTEXT inCert ) {

		// if the digest for this certificate matches the digest I was given, delete the certificate from the store.
		// this duplicates the context first to avoid hosing the enumeration in progress.
		Buffer thisDigest;
		if( getCertificateProperty( inCert, CERT_HASH_PROP_ID, &thisDigest ) && ( digest == thisDigest ) ) {
			// does this certificate have an associated key container?
			Buffer provInfoBuffer;
			if( getCertificateProperty( inCert, CERT_KEY_PROV_INFO_PROP_ID, &provInfoBuffer ) ) {
				// does this certificate refer to my key provider?
				CRYPT_KEY_PROV_INFO* provInfo = reinterpret_cast<CRYPT_KEY_PROV_INFO*>( provInfoBuffer.begin() );
				szContainer = provInfo->pwszContainerName;
			}
			CertDeleteCertificateFromStore( CertDuplicateCertificateContext( inCert ) ); 
			found = true;
		}
		return !found; 
	}
};

static int unregisterPFX( LPCTSTR inDigest ) 
{
	// convert the hex digest to binary
	Buffer passedDigest;
#if _UNICODE
	std::string hex;
	if( !sample::convertUTF16ToUTF8( inDigest, hex ) ) 
		return ERR_UNICODE;
	if( !sample::convertHexToBinary( hex, passedDigest ) ) 
		return ERR_PARAMS;
#else
	if( !sample::convertHexToBinary( inDigest, passedDigest ) ) {
		_tprintf( TEXT( "unregisterPFX: failed to convert digest to binary. Failed to unregister %s.\n" ), inDigest );
		return ERR_PARAM;
	}
#endif

	// open up the "My" store
	StoreHandle myStore( CertOpenStore( CERT_STORE_PROV_SYSTEM, ALL_ENCODINGS, NULL, CERT_SYSTEM_STORE_CURRENT_USER, MY_STORE_NAME ) );
	if( NULL == myStore.get() ) {
		sample::displayErrMsg( L"unregisterPFX:CertOpenStore" );
		return ERR_CAPI;
	}


	// enumerate the certificates until I find one with the matching digest
	DeleteCertWithMatchingDigest finder( passedDigest );
	EnumerateCerts( myStore.get(), finder );
	if( !finder.found ) {
		_tprintf( TEXT( "unregisterPFX:No maching digest %s.\n" ), inDigest );
		return ERR_PARAMS;
	}

	if( !finder.szContainer.empty()) {
		HCRYPTPROV hProv = NULL;
		if( !CryptAcquireContextW( &hProv, finder.szContainer.c_str(), SAMPLECSP_NAME_UTF16, PROV_RSA_FULL, CRYPT_DELETEKEYSET ) ) {
			sample::displayErrMsg( L"unregisterPFX:CryptAcquireContextW" );
			return ERR_CAPI;
		}
	}

	_tprintf( TEXT( "Succeeded in unregistering certificate with digest %s.\n" ), inDigest );
	return 0;		
}

struct CertReporter
{
	bool operator()( PCCERT_CONTEXT inCert ) {

		// does this certificate have an associated key container?
		Buffer provInfoBuffer;
		if( getCertificateProperty( inCert, CERT_KEY_PROV_INFO_PROP_ID, &provInfoBuffer ) ) {

			// does this certificate refer to my key provider?
			CRYPT_KEY_PROV_INFO* provInfo = reinterpret_cast<CRYPT_KEY_PROV_INFO*>( provInfoBuffer.begin() );
			if( 0 == wcscmp( SAMPLECSP_NAME_UTF16, provInfo->pwszProvName ) ) {
				
				// add a separator
				std::cout << std::endl;

				// report the subject
				size_t numChars = CertGetNameStringA( inCert, CERT_NAME_RDN_TYPE, 0, NULL, NULL, 0 );
				if( 0 == numChars ) {
					std::cerr << "Failed to get subject." << std::endl;
					return true;
				}
				std::string subject( numChars, '\0' );
				if( numChars != CertGetNameStringA( inCert, CERT_NAME_RDN_TYPE, 0, NULL, &(*subject.begin()), static_cast<DWORD>(numChars) ) ) {
					std::cout << "Failed to get subject." << std::endl;
					return true;
				}
				 std::cout << "   subject: " << subject.c_str() << std::endl;

				 // report the digest
				 Buffer calculatedDigest;
				 if( !getCertificateProperty( inCert, CERT_HASH_PROP_ID, &calculatedDigest ) ) {
					 std::cerr << "Failed to get certificate digest." << std::endl;
					 return true;
				 }
				 std::string hexDigest;
				 sample::convertBinaryToHex( calculatedDigest.begin(), calculatedDigest.size(), hexDigest );
				 std::cout << "    digest: " << hexDigest << std::endl;

				 // report the container
				 std::wcout << " container: " << provInfo->pwszContainerName << std::endl;

				 // get the container name
				 std::string containerName;
				 if( !sample::convertUTF16ToUTF8( provInfo->pwszContainerName, containerName ) ) {
					 std::cerr << "Failed to convert container name to UTF-8." << std::endl;
					 return true;
				 }

				// parse the container
				unsigned int version = 0;
				std::wstring path;
				Buffer containerDigest;
				if( !sample::getContainerInfo( containerName.c_str(), version, path, containerDigest, false ) ) {
					std::cerr << "Could not find container." << std::endl;
					return true;
				}

				// report the container contents
				std::cout << "       ver: " << version << std::endl;
				std::wcout << "       uri: " << path << std::endl;

				// check the digest
				if( calculatedDigest != containerDigest ) {
					std::cerr << "Calculated digest did not match container digest." << std::endl;
					return true;
				}
			}
		}

		return true; 
	}
};

static int listRegisteredPFXs() 
{
	// open the MY store
	StoreHandle myStore( CertOpenStore( CERT_STORE_PROV_SYSTEM, ALL_ENCODINGS, NULL, CERT_SYSTEM_STORE_CURRENT_USER, MY_STORE_NAME ) );
	if( NULL == myStore.get() ) {
		sample::displayErrMsg( L"listRegisteredPFXs:CertOpenStore" );
		return ERR_CAPI;
	}
		
	// enumerate the certificates
	CertReporter reporter;
	EnumerateCerts( myStore.get(), reporter );
	_tprintf( TEXT( "Finished listing registered PFXs.\n") );
	return 0;		
}

int _tmain(int argc, _TCHAR* argv[])
{
	// usage
	if( 1 >= argc ) {
		std::cout << "SampleRegistrar (copyright Adobe Systems 2006)" << std::endl;
		std::cout << "  to list registered PFXs: registrar -l" << std::endl;
		std::cout << "  to register PFX: registrar -r mycert.pfx password" << std::endl;
		std::cout << "  to unregister PFX: registrar -u <hex-encoded-sha1-cert-digest>" << std::endl;
		std::cout << std::endl;
		return 0;
	}

	else if( 4 == argc && (argv[1][1] == TEXT('r') || argv[1][1] == TEXT('R')) ) {
		return registerPFX( argv[2], argv[3] );
	}

	else if( 3 == argc && (argv[1][1] == TEXT('u') || argv[1][1] == TEXT('U')) ) {
		return unregisterPFX( argv[2] );
	}

	else if( 2 == argc && (argv[1][1] == TEXT('l') || argv[1][1] == TEXT('L')) ) {
		return listRegisteredPFXs();
	}

	std::cout << "Bad parameters." << std::endl;
	return ERR_PARAMS;
}

