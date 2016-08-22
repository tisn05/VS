// sampleCommon.h : 
// 	Defines used for all modules under the CspSamples
//

#ifndef sampleCommon_INCLUDED_
#define sampleCommon_INCLUDED_

#define SAMPLECSP_VERSION		0x00000001
#define SAMPLECSP_NAME_UTF8		"Adobe Sample CSP"
#define SAMPLECSP_NAME_UTF16	L"Adobe Sample CSP"

#define CONTAINER_VERSION		1
#define CONTAINER_PREFIX		"Adobe_"
#define CONTAINER_BASE_KEY		"Software\\Adobe\\Adobe Sample CSP"
#define CONTAINER_VERSION_KEY	"Version"
#define CONTAINER_DIGEST_KEY	"Digest"
#define CONTAINER_PATH_KEY		L"Path" // path is always stored as UTF-16
#define CONTAINER_DEFAULT		"default"
#define MAX_CONTAINER_LEN		64

#define ALL_ENCODINGS			(X509_ASN_ENCODING|PKCS_7_ASN_ENCODING)
#define MY_STORE_NAME			TEXT("MY")
#define CA_STORE_NAME			TEXT("CA")
#define ROOT_STORE_NAME			TEXT("ROOT")
#define MAX_PFX_CHUNK			1024
#define MAX_PFX_FILE_SIZE		1024*1024
#define MAX_PIN_CHARS			512 

// This define is used with CryptGet/SetProvParam for getting at the key container
#define PP_KEYSTORE_URI			101	// The parameter passed is the uri as UTF-16

#define MAX_KEY_LENGTH 255

namespace sample {
	
	class Buffer;

	bool convertUTF16ToUTF8( const std::wstring& inUTF16, std::string& outUTF8 );
	bool convertUTF8ToUTF16( const std::string& inUTF8, std::wstring& outUTF16 ); 
	void convertBinaryToHex( const BYTE* inBinary, const size_t inSize, std::string& outHex );
	bool convertHexToBinary( const std::string& inHex, Buffer& outBin );

	bool getContainerInfo( LPCSTR inContainerName, unsigned int& outVersion, std::wstring& outPath, Buffer& outDigest, bool bMachineKeySet );

	bool isUriSupported( LPCWSTR inUri );
	bool loadFileFromUri( LPCWSTR inUri, bool inAllowUI, Buffer& outData );

	void displayErrMsg(LPTSTR lpszFunction);

	// Enumerate all certificate in this store and call the passed function
	// The passed function should return true to continue enumerating, false to stop.
	template<typename Func>
	inline void EnumerateCerts( HCERTSTORE inStore, Func& inFunc )
	{
		PCCERT_CONTEXT thisCert = CertEnumCertificatesInStore( inStore, NULL );
		while( NULL != thisCert && inFunc( thisCert ) )
			thisCert = CertEnumCertificatesInStore( inStore, thisCert );
		if( thisCert ) 
			CertFreeCertificateContext( thisCert );
	}

	// Test if property exists and return it on request
	bool getCertificateProperty( PCCERT_CONTEXT inCert, DWORD inProperty, Buffer* outProperty );
}

#endif //sampleCommon_INCLUDED_