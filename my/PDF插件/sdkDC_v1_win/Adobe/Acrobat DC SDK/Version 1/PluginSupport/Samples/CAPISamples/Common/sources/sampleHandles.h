/*
This file contains the common handle holder classes.

This file was created by Adobe engineer.
This is intended to be made publicly available after legal review.
Copyright (C) Adobe Systems 2006.
*/

#ifndef sampleHandles_INCLUDED_
#define sampleHandles_INCLUDED_

namespace sample {

	// this wrapper is just for destroying the handle
	struct RegKeyHandle {
		HKEY hReg;
		RegKeyHandle() : hReg( NULL ) {}
		~RegKeyHandle() { if( NULL != hReg ) RegCloseKey( hReg ); }
	private:
		RegKeyHandle( const RegKeyHandle& ); // undefined
		RegKeyHandle& operator=(  const RegKeyHandle&  ); // undefined
	};

	// this wrapper is just for destroying the handle
	struct RpcStringHandle {
		BYTE* rpcString;
		RpcStringHandle() : rpcString( NULL ) {}
		~RpcStringHandle() { if( NULL != rpcString ) RpcStringFreeA( &rpcString ); }
	private:
		RpcStringHandle( const RpcStringHandle& ); // undefined
		RpcStringHandle& operator=(  const RpcStringHandle&  ); // undefined
	};

	/*
	** Base class for all managed handle wrappers
	*/

	struct HandleWrapper { virtual ~HandleWrapper() {} };

	/***************************************************************************
	** Generic wrappers for releasing Windows handles
	***************************************************************************/

	// Wrapper for destroy function returning int and taking (T) as the parameter
	template<typename T, int (*D)(T), T Invalid>
	class HandleWrapper_int_0_data : public HandleWrapper {
	public:
		explicit HandleWrapper_int_0_data( T inData = Invalid ) : m_Data(inData) {}
		~HandleWrapper_int_0_data() { if( Invalid != m_Data ) D( m_Data ); }
		void swap( HandleWrapper_int_0_data& in ) { std::swap( m_Data, in.m_Data ); }
		T get() const { return m_Data; }
	private:
		T m_Data;
	private:
		explicit HandleWrapper_int_0_data( const HandleWrapper_int_0_data<T, D, Invalid>& ); // undefined
		HandleWrapper_int_0_data<T, D, Invalid>& operator=( const HandleWrapper_int_0_data<T, D, Invalid>& ); // undefined
	};

	// Wrapper for destroy function returning BOOL and taking (T) as the parameter
	template<typename T, BOOL (WINAPI * D)(T), T Invalid>
	class HandleWrapper_BOOL_0_data : public HandleWrapper {
	public:
		explicit HandleWrapper_BOOL_0_data( T inData = Invalid ) : m_Data(inData) {}
		~HandleWrapper_BOOL_0_data() { if( Invalid != m_Data ) D( m_Data ); }
		void swap( HandleWrapper_BOOL_0_data& in ) { std::swap( m_Data, in.m_Data ); }
		T get() const { return m_Data; }
	private:
		T m_Data;
	private:
		explicit HandleWrapper_BOOL_0_data( const HandleWrapper_BOOL_0_data<T, D, Invalid>& ); // undefined
		HandleWrapper_BOOL_0_data<T, D, Invalid>& operator=( const HandleWrapper_BOOL_0_data<T, D, Invalid>& ); // undefined
	};

	// Wrapper for destroy function returning BOOL and taking (T, DWORD) as the parameters
	template<typename T, BOOL (WINAPI * D)(T, DWORD), T Invalid>
	class HandleWrapper_BOOL_0_data_1_DWORD : public HandleWrapper {
	public:
		explicit HandleWrapper_BOOL_0_data_1_DWORD( T inData = Invalid ) : m_Data(inData) {}
		~HandleWrapper_BOOL_0_data_1_DWORD() { if( Invalid != m_Data ) D( m_Data, 0 ); }
		void swap( HandleWrapper_BOOL_0_data_1_DWORD& in ) { std::swap( m_Data, in.m_Data ); }
		T get() const { return m_Data; }
	private:
		T m_Data;
	private:
		explicit HandleWrapper_BOOL_0_data_1_DWORD( const HandleWrapper_BOOL_0_data_1_DWORD<T, D, Invalid>& ); // undefined
		HandleWrapper_BOOL_0_data_1_DWORD<T, D, Invalid>& operator=( const HandleWrapper_BOOL_0_data_1_DWORD<T, D, Invalid>& ); // undefined
	};

	/***************************************************************************
	** Wrappers defined for the handle types I am using
	***************************************************************************/

	typedef HandleWrapper_int_0_data<int, _close, 0> FileHandle;
	typedef HandleWrapper_BOOL_0_data<HCRYPTKEY, CryptDestroyKey, NULL> KeyHandle;
	typedef HandleWrapper_BOOL_0_data<HCRYPTHASH, CryptDestroyHash, NULL> HashHandle;
	typedef HandleWrapper_BOOL_0_data<PCCERT_CONTEXT, CertFreeCertificateContext, NULL> CertHandle;
	typedef HandleWrapper_BOOL_0_data<HINTERNET, InternetCloseHandle, NULL> InternetHandle;
	typedef HandleWrapper_BOOL_0_data_1_DWORD<HCRYPTPROV, CryptReleaseContext, NULL> ProviderHandle;
	typedef HandleWrapper_BOOL_0_data_1_DWORD<HCERTSTORE, CertCloseStore, NULL> StoreHandle;

}

#endif //sampleHandles_INCLUDED_

