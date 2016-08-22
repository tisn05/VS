/*
	SampleCsp::HandleMgr

	This file was created by Adobe engineer.
	This is intended to be made publicly available after legal review.
	Copyright (C) Adobe Systems 2006.
*/

#ifndef scspHandleMgr_INCLUDED_
#define scspHandleMgr_INCLUDED_

namespace scsp {

	/* 
	** This class holds a list of handles.
	*/

	class HandleMgr {
	public:
		HandleMgr() {}

		~HandleMgr() { 
			std::for_each( m_Handles.begin(), m_Handles.end(), deleteHandle ); 
		}
		
		void addHandle( sample::HandleWrapper* inHandle ) { 
			if( NULL != inHandle) 
				m_Handles.push_back( inHandle ); 
		}
		
		void removeHandle( sample::HandleWrapper* inHandle ) {
			HandleContainer::iterator iter = std::find( m_Handles.begin(), m_Handles.end(), inHandle );
			if( m_Handles.end() != iter ) {
				deleteHandle( *iter );
				m_Handles.erase( iter );
			}
		}

		bool isKnown( sample::HandleWrapper* inHandle ) {
			return ( m_Handles.end() != std::find( m_Handles.begin(), m_Handles.end(), inHandle ) );
		}

	private:
		static void deleteHandle( sample::HandleWrapper* inHandle ) { 
			if( NULL != inHandle ) 
				delete inHandle; 
		}

	private:
		typedef std::vector<sample::HandleWrapper*> HandleContainer;
		HandleContainer m_Handles;
	};

}

#endif //scspHandleMgr_INCLUDED_
