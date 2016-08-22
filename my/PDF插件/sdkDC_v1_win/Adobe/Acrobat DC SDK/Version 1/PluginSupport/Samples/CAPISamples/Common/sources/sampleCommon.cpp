/*
	scsp::FileLoader

	This file was created by Adobe engineer.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#include "stdafx.h"

struct Chunk { 
	char data[MAX_PFX_CHUNK]; 
};

typedef std::list<Chunk> ChunkList;


// This buffer should do the correct conversion from UTF-16 to UTF-8
bool sample::convertUTF16ToUTF8( const std::wstring& inUTF16, std::string& outUTF8 )
{
	// calculate the size of the string after conversion - this includes terminator
	int numChars = WideCharToMultiByte( CP_UTF8, 0, inUTF16.c_str(), -1, NULL, 0, NULL, NULL );
	if( 0 == numChars ) {
		displayErrMsg(L"convertUTF16ToUTF8:WideCharToMultiByte");
		return false;
	}

	// make sure we have the buffer we need
	outUTF8.resize( numChars, '\0' );

	// convert the string
	int charsWritten = WideCharToMultiByte( CP_UTF8, 0, inUTF16.c_str(), -1, &(*outUTF8.begin()), numChars, NULL, NULL );
	if( 0 == charsWritten ) {
		displayErrMsg(L"convertUTF16ToUTF8:WideCharToMultiByte");
		return false;
	}

	// make sure string is properly sized
	outUTF8.reserve();
	return true;
}

// This buffer should do the correct conversion from UTF-8 to UTF-16
bool sample::convertUTF8ToUTF16( const std::string& inUTF8, std::wstring& outUTF16 )
{
	// calculate the size of the string after conversion - this includes terminator
	int numChars = MultiByteToWideChar( CP_UTF8, 0, inUTF8.c_str(), -1, NULL, 0 );
	if( 0 == numChars ) {
		displayErrMsg(L"convertUTF8ToUTF16:MultiByteToWideChar");
		return false;
	}

	// make sure we have the buffer we need
	outUTF16.resize( numChars, L'\0' );

	// convert the string
	int charsWritten = MultiByteToWideChar( CP_UTF8, 0, inUTF8.c_str(), -1, &(*outUTF16.begin()), numChars );
	if( 0 == charsWritten ) {
		displayErrMsg(L"convertUTF8ToUTF16:MultiByteToWideChar");
		return false;
	}

	// make sure string is properly sized
	outUTF16.reserve();
	return true;
}

inline char toHex( unsigned char inNum )
{
	if( 0 <= inNum && 9 >= inNum )
		return ( inNum + '0' );

	if( 10 <= inNum && 15 >= inNum )
		return ( inNum - 10 + 'A' );

	assert( false ); // should never reach here
	return '\0';
}

inline unsigned char toBin( char inHex )
{
	if( '0' <= inHex && '9' >= inHex )
		return ( inHex - '0' );

	if( 'A' <= inHex && 'F' >= inHex )
		return ( inHex - 'A' + 10 );
	
	if( 'a' <= inHex && 'f' >= inHex )
		return ( inHex - 'a' + 10 );

	assert( false ); // should never reach here
	return 0xFF;
}

inline unsigned char upperNibble( unsigned char inNum )
{
	return ( ( inNum & 0xF0 ) >> 4 );
}

inline unsigned char lowerNibble( unsigned char inNum )
{
	return ( inNum & 0x0F );
}

void sample::convertBinaryToHex( const unsigned char* inBinary, const size_t inSize, std::string& outHex )
{
	outHex.resize( (inSize * 2) + 1, '\0' );
	std::string::iterator iter = outHex.begin();
	for( size_t i = 0; i < inSize; ++i ) {
		*iter++ = toHex( upperNibble( inBinary[i] ) );
		*iter++ = toHex( lowerNibble( inBinary[i] ) );
	}
}

bool sample::convertHexToBinary( const std::string& inHex, Buffer& outBin )
{
	outBin.resize( inHex.size() >> 1 );
	size_t i = 0;
	std::string::const_iterator iter = inHex.begin();

	while( inHex.end() != iter && '\0' != *iter) {
		unsigned char upperNibble = toBin( *iter++ );
		if( 0xFF == upperNibble || inHex.end() == iter ) {
			return false;
		}
		unsigned char lowerNibble = toBin( *iter++ );
		if( 0xFF == lowerNibble ) {
			return false;
		}
		outBin[i] = ( upperNibble << 4 ) + lowerNibble;
		++i;
	}

	return true;
}

void sample::displayErrMsg(LPTSTR lpszFunction)
{
	TCHAR szBuf[512]; 
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    wsprintf(szBuf, 
        L"%s failed with error %d: %s", 
        lpszFunction, dw, lpMsgBuf); 
 
    MessageBox(NULL, szBuf, L"Error", MB_OK); 

    LocalFree(lpMsgBuf);
}

bool sample::loadFileFromUri( LPCWSTR inUri, bool inAllowUI, sample::Buffer& outData )
{
	if( NULL == inUri ) {
		return false;
	}

	ChunkList buffers;
	Chunk readBuffer;
	size_t totalBytesToCopy = 0;
	DWORD bytesRead = 0;

	// for file://
	std::wstring fileScheme = L"file://"; 
	if( 0 == wmemcmp( inUri, fileScheme.c_str(), fileScheme.size() ))
	{
		// inUri format is file:///Drive:/dir/subdir/filename
		std::wstring localFilePath = inUri;
		localFilePath.erase (0, fileScheme.size () + 1);
		std::wstring::size_type nPos = 0;
		while( (nPos = localFilePath.find (L'/')) != std::wstring::npos )
			localFilePath.replace (nPos, 1, 1, L'\\');

		sample::FileHandle handle( _wopen( localFilePath.c_str (), _O_RDONLY | _O_BINARY ) );
		if( -1 == handle.get()) {
			_tprintf(TEXT("loadFileFromUri: failed to open local file.\n"));
			return false;
		}
		
		// read the file into a linked list of fixed size buffers
		while( ( bytesRead = _read( handle.get(), &readBuffer, sizeof(readBuffer) ) ) > 0 ) {
			totalBytesToCopy += bytesRead;
			buffers.insert( buffers.end(), readBuffer );
		}
		if( -1 == bytesRead ) {
			_tprintf(TEXT("loadFileFromUri: failed to read local file.\n"));
			return false;
		}
	} else {
		// initialize my internet connection
		sample::InternetHandle session( InternetOpenW( 
			SAMPLECSP_NAME_UTF16, 
			INTERNET_OPEN_TYPE_PRECONFIG, 
			NULL, 
			NULL, 
			0 ) );
		if( NULL == session.get() ) {
			displayErrMsg(L"loadFileFromUri:InternetOpenW");
			return false;
		}

		// open the file handle
		sample::InternetHandle handle( InternetOpenUrlW(
			session.get(),
			inUri,
			NULL,
			0,
			(inAllowUI ? INTERNET_FLAG_NO_CACHE_WRITE : INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_NO_UI),
			NULL ) );

		if( NULL == handle.get() ) {
			displayErrMsg(L"loadFileFromUri:InternetOpenUrlW");
			return false;
		}

		// read the file into a linked list of fixed size buffers
		while( InternetReadFile( handle.get(), &readBuffer, sizeof(readBuffer), &bytesRead ) && (0 < bytesRead) ) {
			totalBytesToCopy += bytesRead;
			buffers.insert( buffers.end(), readBuffer );
		}
		if( ERROR_SUCCESS != GetLastError() ) {
			displayErrMsg(L"loadFileFromUri:InternetReadFile");
			return false;
		}
	}

	// copy the data from the linked list of buffers into my output buffer
	outData.resize( totalBytesToCopy );
	size_t bufferOffset = 0;
	for( ChunkList::iterator iter = buffers.begin(); buffers.end() != iter; ++iter ) {
		Chunk& chunk = *iter;
		const size_t bytesToCopy = min( totalBytesToCopy, sizeof(Chunk) );
		for( size_t i = 0; i < bytesToCopy; ++i )
			outData[bufferOffset + i] = chunk.data[i];
		totalBytesToCopy -= bytesToCopy;
		bufferOffset += bytesToCopy;
	}

	return true;
}

bool sample::isUriSupported( LPCWSTR inUri )
{
	if( NULL == inUri ) 
		return false;

	// test for file://
	const wchar_t fileScheme[] = L"file://"; 
	if( 0 == memcmp( inUri, fileScheme, sizeof(fileScheme) - sizeof(fileScheme[0]) ) )
		return true;

	// test for http://
	const wchar_t httpScheme[] = L"http://"; 
	if( 0 == memcmp( inUri, httpScheme, sizeof(httpScheme) - sizeof(httpScheme[0]) ) )
		return true;

	// test for https://
	const wchar_t httpsScheme[] = L"https://";
	if( 0 == memcmp( inUri, httpsScheme, sizeof(httpsScheme) - sizeof(httpsScheme[0]) ) )
		return true;

	// test for ftp://
	const wchar_t ftpScheme[] = L"ftp://";
	if( 0 == memcmp( inUri, ftpScheme, sizeof(ftpScheme) - sizeof(ftpScheme[0]) ) )
		return true;

	_tprintf(TEXT("isUriSupported: URI not supported.\n"));
	return false;
}

bool sample::getContainerInfo( LPCSTR inContainerName, unsigned int& outVersion, std::wstring& outPath, Buffer& outDigest, bool bMachineKeySet )
{
	if( NULL == inContainerName) 
		return false;

	// open the base registry key
	RegKeyHandle baseKey;
	if( bMachineKeySet ) {
		if( ERROR_SUCCESS != RegOpenKeyExA( HKEY_LOCAL_MACHINE, CONTAINER_BASE_KEY, 0, KEY_READ, &baseKey.hReg ) ) 
			return false;
	} else {
		if( ERROR_SUCCESS != RegOpenKeyExA( HKEY_CURRENT_USER, CONTAINER_BASE_KEY, 0, KEY_READ, &baseKey.hReg ) ) 
			return false;
	}

	// open the key specified by the container name under this key
	RegKeyHandle key;
	if( ERROR_SUCCESS != RegOpenKeyExA( baseKey.hReg, inContainerName, 0, KEY_READ, &key.hReg ) ) 
		return false;

	// read in the values for the container under this key
	DWORD type;
	DWORD size = sizeof(outVersion);
	if( ERROR_SUCCESS != RegQueryValueExA( key.hReg, CONTAINER_VERSION_KEY, 0, &type, reinterpret_cast<BYTE*>(&outVersion), &size ) || REG_DWORD != type ) 
		return false;
	if( ERROR_SUCCESS != RegQueryValueExW( key.hReg, CONTAINER_PATH_KEY, 0, &type, NULL, &size ) || REG_SZ != type ) 
		return false;
	outPath.resize( size, L'\0' );
	if( ERROR_SUCCESS != RegQueryValueExW( key.hReg, CONTAINER_PATH_KEY, 0, &type, reinterpret_cast<BYTE*>( &*outPath.begin() ), &size ) ) 
		return false;
	outPath.reserve();
	if( ERROR_SUCCESS != RegQueryValueExA( key.hReg, CONTAINER_DIGEST_KEY, 0, &type, NULL, &size ) || REG_BINARY != type ) 
		return false;
	outDigest.resize( size );
	if( ERROR_SUCCESS != RegQueryValueExA( key.hReg, CONTAINER_DIGEST_KEY, 0, &type, outDigest.begin(), &size ) ) 
		return false;

	return true;
}
// This will check if property exists. 
//   If it does not exist, it returns false.
//   If it does exist, the buffer is sized appropriately and it is copied into the buffer.
bool sample::getCertificateProperty( PCCERT_CONTEXT inCert, DWORD inProperty, Buffer* outProvInfo )
{
	DWORD cbData = 0;
	if( !CertGetCertificateContextProperty( inCert, inProperty, NULL, &cbData ) ) 
		return false;

	if( outProvInfo ) {
		outProvInfo->resize( cbData );
		if( !CertGetCertificateContextProperty( inCert, inProperty, outProvInfo->begin(), &cbData ) ) 
			return false;
	}

	return true;
}
