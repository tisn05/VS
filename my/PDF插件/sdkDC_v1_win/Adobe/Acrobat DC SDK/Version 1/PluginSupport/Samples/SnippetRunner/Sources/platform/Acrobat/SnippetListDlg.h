/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

Abstraction for the dialog used to get parameters passed to the snippet
from the user

*********************************************************************/

#ifndef _SnippetListDlg_
#define _SnippetListDlg_

#include <string>
#include "PIHeaders.h"
#include "SnippetRunner.h"

using namespace std;

/** provides a simple singlton class to manage the snippet hierarchy list UI
*/
class SnippetListDlg
{
	protected:
		/** This is a singleton class, so the constructor is protected.
		*/
		SnippetListDlg(){ADMUtils::InitializeADM();};
		/** Destructor 
		*/
		virtual ~SnippetListDlg(){ADMUtils::ReleaseADM();};

	public:
		/** single public interface. 
		*/
		static void ASAPI doSnippetListDlgUI(void *);
		
		/** toggles the check applied to the ADMListEntry
			@return previous state applies to the list element
		*/
		static ASBool toggleMarkedState(SnipRun *thisSnip);

	private:
		/** initialisation for dialog */
		static ASErr ASAPI SnippetListDlgInit(ADMDialogRef dialog);
		
		/** adds the string to the control, associates the SnipRun with the leaf node */
		bool ADMCreateListEntryFromString(ADMHierarchyListRef hierarchyList, string name, SnipRun * theSnip);

		/** method invoked when user hits description button */
		static void ASAPI OnDescriptionButton(ADMItemRef itemRef, ADMNotifierRef notifierRef);

		/** method invoked when user hits execute */
		static void ASAPI OnExecuteButton (ADMItemRef itemRef, ADMNotifierRef notifierRef);

		/** notification method, invoked when listbox is updated */
		static void ASAPI OnListBoxItemNotify(ADMItemRef itemRef, ADMNotifierRef notifierRef);

		/** method invoked when the dialog is closed. We need to clean up this state */
		static void ASAPI OnCloseButton (ADMItemRef itemRef, ADMNotifierRef notifierRef);

		/** method invoked when the system close icon or ESC is hit */
		static void ASAPI MyDialogNotifyProc(ADMDialogRef dialogRef, ADMNotifierRef notifierRef);

		/** single instance of the dialog class maintained by the gSnippetParamDialog pointer */
		static SnippetListDlg* gSnippetListDlg;

		/** cache the listbox id */
		ASInt32 gListBoxItemID;
		ADMDialogRef fDialogRef;
};

#endif

// End, SnippetParamDialog.h
