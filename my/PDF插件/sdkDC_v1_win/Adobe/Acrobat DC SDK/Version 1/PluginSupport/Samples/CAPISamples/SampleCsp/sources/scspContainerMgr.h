/*
	scsp::ContainerMgr

	This file was created by Joe Steele.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#ifndef scspContainerMgr_INCLUDED_
#define scspContainerMgr_INCLUDED_

#include "scspContainer.h"

namespace scsp {

	class Container;

	// This is the class which holds the open containers
	class ContainerMgr {
	public:
		ContainerMgr() {}
		
		~ContainerMgr() {
			std::for_each( m_Containers.begin(), m_Containers.end(), staticReleaseContainer );
		}

		void addContainer( const Container& inContainer ) {
			m_Containers.push_back( inContainer );
		}

		void releaseContainer( const Container& inContainer ) {
			Containers::iterator iter = std::find( m_Containers.begin(), m_Containers.end(), inContainer );
			if( m_Containers.end() != iter ) {
				staticReleaseContainer( *iter );
				m_Containers.erase( iter );
			}
		}

	private:
		typedef std::vector<Container> Containers;
		Containers m_Containers;

	private:
		static void staticReleaseContainer( Container& inContainer ) {
			inContainer.release();
		}
	};

}

#endif //scspContainerMgr_INCLUDED_
