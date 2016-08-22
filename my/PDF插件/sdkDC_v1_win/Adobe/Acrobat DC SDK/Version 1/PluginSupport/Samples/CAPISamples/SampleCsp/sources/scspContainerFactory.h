/*
	scsp::ContainerFactory

	This file was created by Adobe engineer.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#ifndef scspContainerFactory_INCLUDED_
#define scspContainerFactory_INCLUDED_

namespace scsp {

	class Container; 

	class ContainerFactory {
	public:

		// Determine if this is a registered container
		static bool isRegisteredContainer( LPCSTR inContainerName, bool bMachineKeySet );
		static bool isRegisteredContainer( LPCWSTR inContainerName, bool bMachineKeySet );

		// Make a new Container given the container name passed.
		// If the name is NULL - the default Container is constructed.
		// If the name is not a registered container - an unregistered Container is constructed.
		// If the name is a registered container - a regular Container is constructed.
		// Returns NULL on error. Check GetLastError() for details.
		// The constructed Container is added to the ContainerMgr. You must release it through the ContainerMgr.
		static Container* makeRegisteredContainer( LPCSTR inContainerName, bool inAllowUI, bool bMachineKeySet );
		static Container* makeRegisteredContainer( LPCWSTR inContainerName, bool inAllowUI, bool bMachineKeySet );
		static Container* makeNewContainer( LPCSTR inContainerName, bool inAllowUI, bool bMachineKeySet );
		static Container* makeNewContainer( LPCWSTR inContainerName, bool inAllowUI, bool bMachineKeySet );
		static Container* makeDefaultContainer( bool inAllowUI, bool bMachineKeySet );

		// Unregister a registered container
		static bool unregister( LPCSTR inContainerName, bool bMachineKeySet );
		static bool unregister( LPCWSTR inContainerName, bool bMachineKeySet );

		// check for conflicts with existing names
		static bool isNameExisting( LPCSTR inContainerName, bool bMachineKeySet );
		static bool isNameInRegistry( LPCSTR inContainerName, bool bMachineKeySet );
	};

}

#endif //scspContainerFactory_INCLUDED_
