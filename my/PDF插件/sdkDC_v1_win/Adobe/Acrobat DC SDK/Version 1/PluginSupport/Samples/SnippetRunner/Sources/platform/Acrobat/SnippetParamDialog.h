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

#ifndef _SnippetParamDialog_
#define _SnippetParamDialog_

#include <string>
#include "PIHeaders.h"

using namespace std;

/** provides a simple singlton class to manage the dialog for the input of parameters to snippets
*/
class SnippetParamDialog
{
	protected:
		/** This is a singleton class, so the constructor is protected.
		*/
		SnippetParamDialog(){};
		/** Destructor 
		*/
		virtual ~SnippetParamDialog(){};

	public:
		/** single public interface. This method creates an instance of the dialog class, 
			populates it with data, handles the dialog and marshals the data back to the
			calling class. Maintain this class as a singleton. Basically is responsible for creation, use and deletion.
			The language constructs prevent more than one party using this class.
			@param name IN the name of the dialog (snippet name)
			@param desc IN descriptive text associated with the snippet
			@param defparams IN set of default parameters to be applied to the snippet
			@param params OUT set of parameters as specified by the user
			@return return code from the dialog (IDOK or IDCANCEL)
		*/
		static ASInt32 DoDialog(const char * name, const char * desc, const char * defparams, const char * inparams, string &params);

	private:
		/** main entry point to the UI code
			@return return code from the dialog (IDOK or IDCANCEL)
		*/
		ASInt32 PromptForInfo();
		/** called when the user clicks on OK. Static to produce the right (C based) linkeage
			@param  item IN the Okay button
			@param inNotifier IN broadcast the changes using this notifier
		*/
		static void  ASAPI PromptOnOK(ADMItemRef item, ADMNotifierRef inNotifier );
		/** called when initially setting the dialog.
			@param dialogRef IN the dialog to initialise
			@return error code on failure
		*/
		static ASErr  ASAPI PromptDialogOnInit(ADMDialogRef dialogRef);
		/** called to set the dialog controls to the default state (when default
			button clicked, say).
			@param item IN the item to be updated
			@param inNotifier IN broadcast the changes using this notifier
		*/
		static void ASAPI PromptOnDefault(ADMItemRef item, ADMNotifierRef inNotifier);
		/** name of the dialog (snippet) */
		const char * fName;
		/** description associated with the snippet */
		const char * fDescription;
		/** parameters passed back by the snippet */
		string fParams;
		/** default parameters used by the snippet */
		string fDefaultParams;
		/** single instance of the dialog class maintained by the gSnippetParamDialog pointer */
		static SnippetParamDialog* gSnippetParamDialog;
};



#endif

// End, SnippetParamDialog.h
