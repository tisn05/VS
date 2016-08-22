/*
	This file contains all the miscellaneous classes 
	used by the CSP which did not warrant their own files.

	This file was created by Adobe engineer.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#ifndef sampleTypes_INCLUDED_
#define sampleTypes_INCLUDED_

namespace sample {

	// Exception class used by all my functions
	struct Exception {
		Exception(DWORD _err, const char* _desc, long _line, const char* _file) : 
			err(_err), desc(_desc), line(_line), file(_file) {}

		DWORD err;
		std::string desc;
		long line;
		const char* file;
	};

	// Standard buffer type used
	class Buffer {
	public:
		Buffer() : ptr( NULL ), len( 0 ) {}

		Buffer( const Buffer& inBuffer ) : ptr( new BYTE[inBuffer.size()] ), len( inBuffer.size() ) {
			for( size_t i = 0; i < len; ++i )
				ptr[i] = inBuffer[i];
		}

		~Buffer() {
			clear();
		}

		inline Buffer& operator=( const Buffer& inBuffer ) {
			Buffer tmp( inBuffer );
			swap( tmp );
			return *this;
		}

		inline BYTE& operator[]( size_t inIndex ) {
			return ptr[inIndex];
		}

		inline const BYTE& operator[]( size_t inIndex ) const {
			return ptr[inIndex];
		}

		inline size_t size() const {
			return len;
		}

		inline void swap( Buffer& ioBuffer ) throw() {
			std::swap( ptr, ioBuffer.ptr );
			std::swap( len, ioBuffer.len );
		}

		inline BYTE* begin() {
			return ptr;
		}

		inline BYTE* end() {
			return ptr + len;
		}

		// This will preserve the contents when the size is <=len and >0
		inline void resize( size_t inSize ) {
			if( 0 == inSize || inSize > len )
				clear();
			if( inSize > len ) {
				ptr = new BYTE[inSize];
				if( NULL == ptr )
					throw Exception( GetLastError(), "out of memory", __LINE__, __FILE__ );
			}
			len = inSize;
		}

		inline void clear() {
			if( ptr )
				SecureZeroMemory( ptr, len );
			delete[] ptr;
			ptr = NULL;
			len = 0;
		}

	private:
		BYTE* ptr;
		size_t len;
	};

	inline bool operator==( const Buffer& in1, const Buffer& in2 ) {
		if( in1.size() != in2.size() )
			return false;
		for( size_t i = 0; i < in1.size(); ++i )
			if( in1[i] != in2[i] )
				return false;
		return true;
	}

	inline bool operator!=( const Buffer& in1, const Buffer& in2 ) {
		return !( in1 == in2 );
	}

	// This class allows me to keep a secure copy of data passed in
	class SecureBuffer {
	public:
		SecureBuffer() {}

		bool empty() const { 
			return ( 0 == buffer.size() );
		}

		bool setProtectedData( const BYTE* inData, const size_t inSize ) {
			DATA_BLOB unprot;
			unprot.pbData = const_cast<BYTE*>( inData );
			unprot.cbData = static_cast<DWORD>( inSize );
			LocalAllocBuffer prot;
			if( !CryptProtectData( &unprot, TEXT("adobe"), NULL, NULL, NULL, 0, &prot ) )
				return false;
			buffer.resize( prot.cbData );
			for( size_t i = 0; i < prot.cbData; ++i )
				buffer[i] = prot.pbData[i];
			return true;
		}

		bool getUnprotectedData( Buffer& outUnprotected ) {
			DATA_BLOB prot;
			prot.pbData = buffer.begin();
			prot.cbData = static_cast<DWORD>( buffer.size() );
			LocalAllocBuffer unprot;
			if( !CryptUnprotectData( &prot, NULL, NULL, NULL, NULL, 0, &unprot ) )
				return false;
			outUnprotected.resize( unprot.cbData );
			for( size_t i = 0; i < unprot.cbData; ++i )
				outUnprotected[i] = unprot.pbData[i];
			return true;
		}
	
	private:
		Buffer buffer;
		SecureBuffer( const SecureBuffer& inData ); // not implemented
		SecureBuffer& operator=( const SecureBuffer& inData ); // not implemented

		// This class allows parameters passed to CryptProtectData to be cleaned up properly
		class LocalAllocBuffer : public DATA_BLOB
		{
		public:
			LocalAllocBuffer() { 
				pbData = NULL; 
				cbData = 0; 
			}

			~LocalAllocBuffer() {
				if( pbData ) {
					SecureZeroMemory( pbData, cbData );
					LocalFree( pbData );
				}
			}
		};
	};

}

#endif //sampleTypes_INCLUDED_

