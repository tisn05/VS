/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2000-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 PDEContentExplorerSnip.cpp
 
*********************************************************************/
#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

ACCB1 void ACCB2 myPDEObjectDumpProc (PDEObject obj, const char* dumpInfo, void* clientData);
void dumpContent(PDEContent content, int numTabs);
/** Explores the PDE objects on the first page of the document. You can request either "brief" or "detailed" diagnostics to be dumped. For "brief",
	the type of object and the page co-ordinate bounding box for the object	are dumped to DebugWindow. For group and container objects, we recurse 
	down the encapsulated contents. You can use the AVPageViewDrawRectSnip snippet to highlight the bounding box for one of the objects (cut and 
	paste the co-ordinates from DebugWindow to the parameter dialog). For "detailed" information, we use the PDEObjectDump API call.  
	@testfile OCGSample1.pdf
	@requires pdf with page content
	@see PDPageAcquirePDEContent
	@see PDEContentGetNumElems
	@see PDEContentGetElem
	@see PDEObjectDump
	@see PDEElementGetBBox
	@see PDEObjectGetType
	@see PDEContainerGetContent
	@see PDEGroupGetContent
	@see AVPageViewDrawRectSnip
*/
void PDEContentExplorerSnip(ParamManager * thePM){
	// this snippet might take a while, update the UI
	// As the snippet is application modal, I know I was off before...
	SnippetRunnerUtils::toggleSnippetCheck();
	CDocument document;
	PDPage firstPage = (PDPage)document; 	//CDocument releases page
	if (firstPage == NULL) return;

	PDEContent pageContent = PDPageAcquirePDEContent(firstPage,gExtensionID);
	string level;
	if (ASBoolToBool(thePM->getNextParamAsString(level)) == true){
		if(level.compare("brief")==0){
			dumpContent(pageContent,0);
		}
		else if (level.compare("detailed")==0) {
			ASInt32 numElems = PDEContentGetNumElems(pageContent);
			for (int loop=0;loop<numElems;loop++){
				PDEElement theElement = PDEContentGetElem(pageContent ,loop);
				int tabLevel = 0;
				PDEObjectDump(reinterpret_cast<PDEObject>(theElement), 10, myPDEObjectDumpProc, &tabLevel);
			}
		}
		else {
			AVAlertNote("Only supports \"brief\" or \"detailed\"");
		}
	}
	PDPageReleasePDEContent(firstPage, gExtensionID);
	SnippetRunnerUtils::toggleSnippetCheck();
}

SNIPRUN_REGISTER_WITH_DIALOG(
	PDEContentExplorerSnip, 
	"PDE Layer:PDEContentExplorerSnip",
	"Enumerates the PDE content of the first page. \nParameters [brief|detailed]","brief")

ACCB1 void ACCB2 myPDEObjectDumpProc (PDEObject obj, const char* dumpInfo, void* clientData){
	int* numTabs = static_cast<int *>(clientData);
	char *buff = static_cast<char *>(ASmalloc(strlen(dumpInfo)+5));
	sprintf(buff,"%d %s",*numTabs,dumpInfo);
	Console::displayString(dumpInfo);
	ASfree(buff);
}

void dumpContent(PDEContent content, int numTabs){
	ASInt32 numberOfElements = PDEContentGetNumElems(content);

	for (int t=0;t<numberOfElements;t++){
		PDEElement pdeElement = PDEContentGetElem(content, t);

		char buffer[50];
		if (numTabs >0) {
			snprintf(buffer,sizeof(buffer),"%d ",numTabs);
		}
		else {
			snprintf(buffer,sizeof(buffer),"");
		}
		ASFixedRect bbox;
		PDEElementGetBBox(pdeElement, &bbox);		
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEText){
			strcat(buffer, "Found a text object");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
		}
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEForm){
			strcat(buffer, "Found a form object");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
		}
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEGroup){
			strcat(buffer, "Found a group object ---");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
			dumpContent(PDEGroupGetContent(reinterpret_cast<PDEGroup>(pdeElement)),numTabs+1);
			char buff2[8];
			if (numTabs>0){
				sprintf(buff2,"%d ---",numTabs);
			}
			else {
				sprintf(buff2,"---");
			}
			Console::displayString(buff2);			
		}
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEImage){
			strcat(buffer, "Found an image object");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
		}
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEPath){
			strcat(buffer, "Found a path object");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
		}
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEPS){
			strcat(buffer,"Found a postscript object");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
		}
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEShading){
			strcat(buffer, "Found a shading object");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
		}
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEXObject){
			strcat(buffer, "Found an XObject object");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
		}
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEUnknown){
			strcat(buffer, "Found an unknown object");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
		}
		if (PDEObjectGetType(reinterpret_cast<PDEObject>(pdeElement))==kPDEContainer){
			strcat(buffer, "Found a container object ---");
			SnippetRunnerUtils::DumpRectToDebugWindow(buffer,&bbox);
			dumpContent(PDEContainerGetContent(reinterpret_cast<PDEContainer>(pdeElement)),numTabs+1);
			char buff2[8];
			if (numTabs>0){
				snprintf(buff2,sizeof(buff2),"%d ---",numTabs);
			}
			else {
				snprintf(buff2,sizeof(buff2),"---");
			}
			Console::displayString(buff2);
		}
	}
}

