/*
	scsp::Container

	This file was created by Adobe engineer.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#ifndef scspContainer_INCLUDED_
#define scspContainer_INCLUDED_

namespace sample {
	class Buffer;
}

namespace scsp {

	class ContainerMgr;
	class ContainerFactory;

	// This is the class which holds the private key information
	class Container {
		friend ContainerMgr;
		friend ContainerFactory;

	public:
		~Container() {}
		void setMachineKeySet( bool inMachineKeySet ) { m_MachineKeySet = inMachineKeySet; }
		bool isMachineKeySet() { return m_MachineKeySet; }
		void setPin( LPCWSTR inPin );
		bool registerPFX( LPCWSTR inPath );
		bool accessPrivateKey();

		bool isDefault() const { return m_Default; }
		bool isRegistered() const { return m_Registered; }
		bool isLoaded() const { return ( 0 != m_PFX.size() ); }
		HCRYPTPROV getProviderHandle() const;
		std::string getContainerName() const { return m_ContainerName; }
		std::wstring getPath() const { return m_Path; }
		void setWindowHandle( HWND inHWND ) { m_HWND = inHWND; }

	private:

		// parameters
		bool m_AllowUI;
		bool m_Default;
		bool m_Registered;
		bool m_MachineKeySet;
		std::string m_ContainerName;
		std::wstring m_Path;
		sample::Buffer m_CertDigest;
		
		// state
		sample::Buffer m_PFX;
		sample::SecureBuffer m_SecuredPin;
		sample::StoreHandle m_Store;
		sample::ProviderHandle m_Provider;
		HWND m_HWND;

	private:
		Container( LPCSTR inContainer, bool inDefault, bool inAllowUI, bool bMachineKeySet );
		Container( LPCSTR inContainer, LPCWSTR inPath, const sample::Buffer& inCertDigest, bool inAllowUI, bool bMachineKeySet );
		bool getPin( sample::Buffer& outPin );
	};


	// This is the class which holds the open containers
	class ContainerMgr {
	public:
		ContainerMgr() {}

		~ContainerMgr() {
			std::for_each( m_Containers.begin(), m_Containers.end(), staticReleaseContainer );
		}

		void addContainer( Container* inContainer ) {
			if( inContainer )
				m_Containers.push_back( inContainer );
		}

		void removeContainer( Container* inContainer ) {
			Containers::iterator iter = std::find( m_Containers.begin(), m_Containers.end(), inContainer );
			if( m_Containers.end() != iter ) {
				staticReleaseContainer( *iter );
				m_Containers.erase( iter );
			}
		}

		bool isKnown( Container* inContainer ) {
			return ( m_Containers.end() != std::find( m_Containers.begin(), m_Containers.end(), inContainer ) );
		}

		bool isNameExisting( LPCSTR inContainerName, bool bMachineKeySet ) {
			Containers::iterator iter = m_Containers.begin();
			while( m_Containers.end() != iter) {
				if( !_stricmp( (*iter)->m_ContainerName.c_str(), inContainerName ) 
					&& bMachineKeySet == (*iter)->m_MachineKeySet )
					return true;
				iter++;
			}
			return false;
		}

	private:
		typedef std::vector<Container*> Containers;
		Containers m_Containers;

	private:
		explicit ContainerMgr( const ContainerMgr& ); // undefined
		ContainerMgr& operator=( const ContainerMgr& ); // undefined
		static void staticReleaseContainer( Container* inContainer ) { 
			delete inContainer; }
	};

}

#endif //scspContainer_INCLUDED_
