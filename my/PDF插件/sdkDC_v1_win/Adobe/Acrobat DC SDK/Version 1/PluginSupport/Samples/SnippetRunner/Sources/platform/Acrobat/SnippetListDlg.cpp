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

 this file contains the implementation for the Snippet dialog.


*********************************************************************/
#ifdef WIN_PLATFORM
#include "PIHeaders.h"
#include "resource.h"
#else
#include "MacResource.h"
#endif

#include "SnippetRunner.h"
#include "SnipManager.h"
#include "SnippetListDlg.h"
#include <string.h>
#include "CAVAlert.h"

#include <fcntl.h>

/** title length */
const int MAX_TITLE_LENGTH = 64;

SnippetListDlg* SnippetListDlg::gSnippetListDlg  = NULL;


ASCallback idleProc;

ASBool 
SnippetListDlg::toggleMarkedState(SnipRun * thisSnip){

	if (thisSnip == NULL){
		AVAlertNote("Attempt to toggle null snippet?");
		return false;
	}
	ADMItemRef ListBoxItem = sADMDialog->GetItem (gSnippetListDlg->fDialogRef, gSnippetListDlg->gListBoxItemID);
	ADMHierarchyListRef theHierarchyList = static_cast<ADMHierarchyListRef>((sADMItem->GetUserData(ListBoxItem)));

	ASInt32 numberOfSnippets = sADMHierarchyList->NumberOfLeafEntries(theHierarchyList);
	for (int t = 0;t < numberOfSnippets; t++){
		ADMListEntryRef entry = sADMHierarchyList->IndexLeafEntry(theHierarchyList,t);
		SnipRun * theSnip = static_cast<SnipRun *>(sADMListEntry->GetUserData(entry));
		if (theSnip==thisSnip) {
			// found the snippet, get the state of the entry, then invert it
			ASBool currentState = sADMListEntry->IsChecked(entry);
			if (currentState == true){
				unsigned short *entryName = static_cast<unsigned short *>(ASmalloc(100));
				sADMListEntry->GetTextW(entry,entryName,49);
				ASText uniText = ASTextFromUnicode(entryName,kUTF16HostEndian);	
				ASfree(entryName);
				const char * basicText = ASTextGetScriptText(uniText, kASRomanScript);
				char newBasicText[100];
				strcpy(newBasicText,basicText);
				string existingLabel(newBasicText);
				if (existingLabel.find("...running")!=string::npos){
					existingLabel = existingLabel.replace(existingLabel.find("...running"),10,"");
					ASText newtext = ASTextFromScriptText(existingLabel.c_str(), kASRomanScript);
					const ADMUnicode* unicodeText = ASTextGetUnicode(newtext);
					sADMListEntry->SetTextW(entry,unicodeText);
					ASTextDestroy(newtext);
				}
				else {
					AVAlertNote("Error forming dialog string");
				}
				ASTextDestroy(uniText);
			}
			else
			{
				unsigned short *entryName = static_cast<unsigned short *>(ASmalloc(100));
				sADMListEntry->GetTextW(entry,entryName,49);
				ASText uniText = ASTextFromUnicode(entryName,kUTF16HostEndian);	
				ASfree(entryName);
				const char * basicText = ASTextGetScriptText (uniText, kASRomanScript);
				char newBasicText[100];
				strcpy(newBasicText,basicText);
				string existingLabel(newBasicText);
				if (existingLabel.find("...running")==string::npos){
					existingLabel += "...running";
					ASText newtext = ASTextFromScriptText(existingLabel.c_str(), kASRomanScript);
					const ADMUnicode* unicodeText = ASTextGetUnicode(newtext);
					sADMListEntry->SetTextW(entry,unicodeText);
					ASTextDestroy(newtext);
				}
				else {
					AVAlertNote("Error reforming dialog string");
				}
				ASTextDestroy(uniText);
			
			}
			sADMListEntry->Check(entry,!currentState);
			return currentState;
		}
	}
	AVAlertNote("Attempt to toggle check state of snippet with no UI");
	return false;
}


void ASAPI 
SnippetListDlg::OnCloseButton (ADMItemRef itemRef, ADMNotifierRef notifierRef)
{
	sADMItem->DefaultNotify(itemRef, notifierRef);

	// destroy dialog
	sADMDialog->Destroy(sADMItem->GetDialog(itemRef));
	delete SnippetListDlg::gSnippetListDlg;
	SnippetListDlg::gSnippetListDlg = NULL;
}

void ASAPI 
SnippetListDlg::OnListBoxItemNotify(ADMItemRef itemRef, ADMNotifierRef notifierRef)
{
	// default
	sADMItem->DefaultNotify(itemRef, notifierRef);
	ADMDialogRef dialog = sADMItem->GetDialog(itemRef);

	// get list
	ADMItemRef ListBoxItem = sADMDialog->GetItem (sADMItem->GetDialog(itemRef), gSnippetListDlg->gListBoxItemID);
	ADMHierarchyListRef theHierarchyList = (ADMHierarchyListRef) (sADMItem->GetUserData(ListBoxItem));
	
	// get selected entry
	ADMListEntryRef listEntry = sADMHierarchyList->GetActiveLeafEntry(theHierarchyList);
	int ID = sADMListEntry->GetID(listEntry);
	if (ID == 0){
		ADMItemRef item = sADMDialog->GetItem (dialog, IDC_STATIC_ITEMDESCIP);
		ASText text = ASTextFromScriptText("", kASRomanScript);
		const ADMUnicode* unicodeText = ASTextGetUnicode(text);
		sADMItem->SetTextW (item,unicodeText);
		ASTextDestroy(text);
		sADMListEntry->Select(listEntry,false);
		ADMItemRef exeButton = sADMDialog->GetItem (dialog, IDC_BUTTON_EXECUTE);
		sADMItem->Enable(exeButton, false);
	}
	else {
		SnipRun * theSnip = static_cast<SnipRun *>(sADMListEntry->GetUserData(listEntry));
		ADMItemRef exeButton = sADMDialog->GetItem (dialog, IDC_BUTTON_EXECUTE);
		sADMItem->Enable(exeButton, true);
		const char * description = theSnip->GetDescription();
		string strdesc(description);
#if MAC_PLATFORM
		ASInt32 numnChars = 100; 
#else
		ASInt32 numnChars = 300; 
#endif
		char* appendStr =  "... (see cpp file for more detail)";
		int len = strlen(appendStr);
		if (strdesc.length()>(unsigned int)numnChars+len){
			strdesc = strdesc.substr(0,numnChars-1);
			int rind = strdesc.rfind(' ');
			strdesc = strdesc.substr(0,rind);
			strdesc.append(appendStr);
		}
		
		ADMItemRef item = sADMDialog->GetItem (dialog, IDC_STATIC_ITEMDESCIP);
		ASText text = ASTextFromScriptText(strdesc.c_str(), kASRomanScript);
		const ADMUnicode* unicodeText = ASTextGetUnicode(text);
		sADMItem->SetTextW (item,unicodeText);
		ASTextDestroy(text);
	}
}

void ASAPI 
SnippetListDlg::OnDescriptionButton(ADMItemRef itemRef, ADMNotifierRef notifierRef)
{
	sADMItem->DefaultNotify(itemRef, notifierRef);
	
	// get list
	ADMItemRef ListBoxItem = sADMDialog->GetItem (sADMItem->GetDialog(itemRef), SnippetListDlg::gSnippetListDlg->gListBoxItemID);
	ADMHierarchyListRef theHierarchyList = (ADMHierarchyListRef) (sADMItem->GetUserData(ListBoxItem));
	
	// get selected entry
	ADMListEntryRef listEntry = sADMHierarchyList->GetActiveLeafEntry(theHierarchyList);		
	if (listEntry == NULL){
		AVAlertNote("Select a leaf node");
	}
	else{		
		SnipRun *theSnip = static_cast<SnipRun *>(sADMListEntry->GetUserData(listEntry));
		theSnip->invokeDocumentation();
	}
}

void ASAPI 
SnippetListDlg::OnExecuteButton (ADMItemRef itemRef, ADMNotifierRef notifierRef)
{
	sADMItem->DefaultNotify(itemRef, notifierRef);
	
	// get list
	ADMItemRef ListBoxItem = sADMDialog->GetItem (sADMItem->GetDialog(itemRef), SnippetListDlg::gSnippetListDlg->gListBoxItemID);
	ADMHierarchyListRef theHierarchyList = (ADMHierarchyListRef) (sADMItem->GetUserData(ListBoxItem));
	
	// get selected entry
	ADMListEntryRef listEntry = sADMHierarchyList->GetActiveLeafEntry(theHierarchyList);		
	if (listEntry == NULL){
		AVAlertNote("Select a leaf node");
	}
	else{		
		SnipRun * theSnip = static_cast<SnipRun *>(sADMListEntry->GetUserData(listEntry));
		theSnip->thisSnip = theSnip;
		if (theSnip!=NULL){
			DURING
				ASInt32 errcode = IDOK;
				ParamManager * thePM = NULL;
				if ((theSnip->GetDefaultParams().size())!=0){
					if (theSnip->GetParams().compare(string("")) == 0){
						theSnip->SetParams(theSnip->GetDefaultParams());
					}
					thePM = new ParamManager();
					if (theSnip->suppressDialog() == false){
						errcode = thePM->doDialog(theSnip->GetName(),theSnip->GetDescription(),theSnip->GetDefaultParams().c_str(),theSnip->GetParams().c_str());
					}
				}
				if (errcode == IDOK){
					if (thePM!=NULL){
						theSnip->SetParams(thePM->getParamList());
					}
					theSnip->Run(thePM);
				}
				if (thePM != NULL){
					delete thePM;
				}
			HANDLER
				char message[255];
				ASGetErrorString(ERRORCODE, message, sizeof(message));
				char message1[255];
				strcpy(message1,"Exception in snippet ");
				strcat(message1, theSnip->GetName());
				AVAlertNote(message1); 
				AVAlertNote(message);
			END_HANDLER
		}
	}
}

bool 
SnippetListDlg::ADMCreateListEntryFromString(ADMHierarchyListRef hierarchyList, string name, SnipRun * theSnip)  
{
	const char* foo = name.c_str();
	ASBool isLeaf = false;
	if(name.find_first_of(":") == string::npos)
		isLeaf = true;
	
	ASBool finished = false;
	int rpos = name.find_first_of(":");
	if (rpos ==string::npos){
		finished = true;
	}
	string head = name.substr(0,rpos);
	ASText headText = ASTextFromScriptText(head.c_str(), kASRomanScript);
	name = name.substr(rpos+1,string::npos);


	// we want to search the hierarchy list for an entry coressponding to head

	// insert a new entry
	int numEntries = sADMHierarchyList->NumberOfEntries(hierarchyList);
	int loopcnt=0;
	ADMListEntryRef headEntry = NULL;
	for (;loopcnt<numEntries;loopcnt++){
		headEntry = sADMHierarchyList->IndexEntry(hierarchyList,loopcnt);
		ASInt32 nameLen = sADMListEntry->GetTextLengthW(headEntry);
		ADMUnicode tmpName [50];
		if (nameLen >49){
			AVAlertNote("Snippet UI name too long");
			return 1;
		}
		sADMListEntry->GetTextW(headEntry,tmpName,nameLen+1);
		ASText curEntryText = ASTextFromUnicode(tmpName, kUTF16HostEndian);
		if (ASTextCmp(curEntryText, headText) == 0){
			ASTextDestroy(curEntryText);
			break;
		}
		else {
			ASTextDestroy(curEntryText);
			headEntry = NULL;
		}
	}
	if (headEntry == NULL) {
		// we could not find a match, create a new entry and if we are not
		// the terminal node a new child list
		headEntry = sADMHierarchyList->InsertEntry(hierarchyList, numEntries);
		sADMListEntry->SetUserData(headEntry, static_cast<void *>(theSnip));
		
		ADMColor color = (ADMColor)0x0000FFFF;
		sADMListEntry->SetTextColor(headEntry, color);
		
		//set if a leaf
		if(!isLeaf)
			sADMListEntry->SetID(headEntry, 0);
		else
			sADMListEntry->SetID(headEntry, 1);

		// set the check state...
		ASBool theState = theSnip->getCheckedState();
		sADMListEntry->Check(headEntry,theState);

		//convert text to unicode, and set in dialog
		ASText text = ASTextFromScriptText(head.c_str(), kASRomanScript);
		const ADMUnicode* unicodeText = ASTextGetUnicode(text);
		sADMListEntry->SetTextW(headEntry,unicodeText);
		ASTextDestroy(text);
		if ((finished==true) && (theState==true)){
			unsigned short *entryName = static_cast<unsigned short *>(ASmalloc(100));
			sADMListEntry->GetTextW(headEntry,entryName,49);
			ASText uniText = ASTextFromUnicode(entryName,kUTF16HostEndian);	
			const char * basicText = ASTextGetScriptText(uniText, kASRomanScript);
			char newBasicText[100];
			strcpy(newBasicText,basicText);
			string existingLabel(newBasicText);
			if (existingLabel.find("...running")==string::npos){
				existingLabel += "...running";
				ASText newtext = ASTextFromScriptText(existingLabel.c_str(), kASRomanScript);
				const ADMUnicode* unicodeText = ASTextGetUnicode(newtext);
				sADMListEntry->SetTextW(headEntry,unicodeText);
				ASTextDestroy(newtext);
			}
			else {
				AVAlertNote("Error reforming dialog string");
			}
			
			sADMListEntry->Check(headEntry,true);
			ASfree(entryName);
			ASTextDestroy(uniText);
		}

		if (!finished) {
      // not the terminal node, create the child list and recurse
			ADMHierarchyListRef childList = sADMListEntry->CreateChildList(headEntry);
			sADMListEntry->SetTextColor(headEntry, kADMTextColor);
			sADMListEntry->ExpandHierarchy(headEntry,false);
			ADMCreateListEntryFromString(childList,name,theSnip);
		}
	}
	else{
		if (!finished){
			ADMHierarchyListRef childList = sADMListEntry->GetChildList(headEntry);
			if (childList == NULL){
				sADMListEntry->CreateChildList(headEntry);
				sADMListEntry->SetTextColor(headEntry, kADMTextColor);
			}
			ADMCreateListEntryFromString(childList,name,theSnip);
		}
	}
	ASTextDestroy(headText);
	return 0;
}

void ASAPI 
SnippetListDlg::MyDialogNotifyProc(ADMDialogRef dialogRef, ADMNotifierRef notifierRef)
{
	// if the user hits ESC key or SYSTEM CLOSE icon, close dialog and clean up.
	if (sADMNotifier->IsNotifierType(notifierRef, kADMCloseHitNotifier))
    {
#ifdef WIN_PLATFORM
		sADMDialog->Destroy(dialogRef);
#endif
		delete SnippetListDlg::gSnippetListDlg;
		SnippetListDlg::gSnippetListDlg = NULL;
    }

	// default
    sADMDialog->DefaultNotify (dialogRef, notifierRef);
}

ASErr ASAPI 
SnippetListDlg::SnippetListDlgInit(ADMDialogRef dialog)
{

	SPErr error = kSPBadParameterError;

	// initialize items
	ADMItemRef item = NULL;
#ifdef WIN_PLATFORM
	item = sADMDialog->GetItem (dialog, IDOK);
#else  // MAC_PLATFORM
	item = sADMDialog->GetItem (dialog, IDC_BUTTON_CLOSE);
#endif
	if (item != NULL)
		sADMItem->SetNotifyProc (item, SnippetListDlg::OnCloseButton);

	item = sADMDialog->GetItem (dialog, IDC_BUTTON_EXECUTE);
	if (item != NULL)
	{
		sADMItem->SetNotifyProc (item, SnippetListDlg::OnExecuteButton);
		sADMItem->Enable(item, false);
	}

	item = sADMDialog->GetItem (dialog, IDC_BUTTON_DESCIPTION);
	sADMItem->Show(item, false);
	if (item != NULL) 
		sADMItem->SetNotifyProc (item, SnippetListDlg::OnDescriptionButton);
	
	item = sADMDialog->GetItem (dialog, IDC_STATIC_ITEMDESCIP);
	if (item != NULL) {
		ASText text = ASTextFromScriptText("Select a list leaf entry, then press Execute or Document button.", kASRomanScript);
		const ADMUnicode* unicodeText = ASTextGetUnicode(text);
		sADMItem->SetTextW (item,unicodeText);
		ASTextDestroy(text);
	}

	// dialog notification
	sADMDialog->SetNotifyProc (dialog, MyDialogNotifyProc);

	// get the rect of dummy control created in resource, then destroy dummy control
	item = sADMDialog->GetItem (dialog, IDC_LIST_USERBOX);
	if(!item) return error;
	ASRect bounds;
	sADMItem->GetBoundsRect(item, &bounds); 
	sADMItem->Destroy(item);

	// create ADM Hierarchy List Box
	ADMItemRef ListBoxItem = sADMDialog->CreateItem (dialog, kADMUniqueItemID, 
							kADMHierarchyListBoxType, &bounds, NULL, NULL, 0);
	SnippetListDlg::gSnippetListDlg->gListBoxItemID = sADMItem->GetID(ListBoxItem);

	// set its notify callback
	sADMItem->SetNotifyProc (ListBoxItem, SnippetListDlg::OnListBoxItemNotify);

	// get Hierarchy List
	ADMHierarchyListRef theHierarchyList = sADMItem->GetHierarchyList(ListBoxItem);
	sADMItem->SetUserData(ListBoxItem, (ADMUserData) theHierarchyList);

	// Hierarchy List settings
	const int SCROLLBAR_WIDTH = 20;
	int tw = bounds.right - bounds.left - SCROLLBAR_WIDTH;
	int th = 14;
	ASRect TextBounds;
	TextBounds.left = 0;
	TextBounds.right = tw;
	TextBounds.top = 0;
	TextBounds.bottom = th;
	sADMHierarchyList->SetEntryWidthRecursive(theHierarchyList, tw);
	sADMHierarchyList->SetEntryTextRectRecursive(theHierarchyList, &TextBounds);

	SnipManager * theSM = SnipManager::Instance();

	SnipRun * theSnip = theSM->getFirstSnippet();
	while (theSnip!=NULL){
		const char * name = theSnip->GetName();
		SnippetListDlg::gSnippetListDlg->ADMCreateListEntryFromString(theHierarchyList,name, theSnip);
		theSnip = theSM->getNextSnippet();
	}
	return kSPNoError;
}



void ASAPI 
SnippetListDlg::doSnippetListDlgUI(void *)
{
	SPErr error = kSPNoError;
	
	if (SnippetListDlg::gSnippetListDlg !=NULL){
		return;
	}
	SnippetListDlg::gSnippetListDlg = new SnippetListDlg();

	// pop up a floating dialog 
	ADMDialogRef dialog = sADMDialog->Create(sADMPluginRef,
										"ADBE:SnippetListUI",
										IDD_DIALOG_HIER_LIST, kADMFloatingDialogStyle,
										SnippetListDlg::SnippetListDlgInit, NULL, NULL);

	gSnippetListDlg->fDialogRef = dialog;
	if(dialog == NULL){
		AVAlertNote("Error creating snippet dialog");
		delete SnippetListDlg::gSnippetListDlg;
	}
	sADMDialog->SetText(dialog, "SDK Code Snippets");
}

