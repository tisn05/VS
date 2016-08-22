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

ShowPermissions.cpp

  - This sample calls the PDDocPermRequest() function to check permissions
  for the front PDF under the current Acrobat product, then write the results 
  to a text file and a Excel file in a folder the user selected.
  

*********************************************************************/

#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

/*-------------------------------------------------------
	Declarations
-------------------------------------------------------*/
// Helper functions
ASPathName SelectFolder(char* title);
ASInt32 SaveStringToFile(char* path,  char* data);
ASInt32 SaveStringToFile(ASText fname,  char* data);

/*-------------------------------------------------------
	Constants
-------------------------------------------------------*/

// Internal data structure
struct PermissionStruct {
	int ReqObj;
	char ReqObjStr[24];
	int ReqOpr;
	char ReqOprStr[24];
	char Perm[24];  // yes, no, n/a, unknown...
};

// Internal permission matrix according to PDF permission document.
// It's from the header file PDExpT.h and Acrobat permissions document.
PermissionStruct PermData[] =

	{{PDPermReqObjDoc,	"Doc", PDPermReqOprAll,			"All",		"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprModify,		"modify",	"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprCopy,		"Copy",		"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprAccessible,	"Access",	"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprSelect,		"Select",	"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprOpen,		"Open",		"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprSecure,		"Secure",	"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprPrintHigh,	"High Res Print",	"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprPrintLow,	"Low Res Print",	"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprFullSave,	"Full Save",		"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprImport,		"Import",	"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprExport,		"Export",	"Yes"}, 
	{PDPermReqObjDoc,   "Doc", PDPermReqOprAny,			"Any",		"Yes"}, 

	{PDPermReqObjPage,  "Page", PDPermReqOprAll,		"All",		"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprCreate,		"Create",	"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprDelete,		"Delete",	"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprCopy,		"Copy",		"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprRotate,		"Rotate",	"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprCrop,		"Crop",		"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprInsert,		"Insert",	"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprReplace,	"Replace",	"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprReorder,	"Reorder",	"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprModify,		"Modify",	"Yes"}, 
	// not used {PDPermReqObjPage,  "Page", PDPermReqOprExport,		"Export",	"Yes"}, 
	{PDPermReqObjPage,  "Page", PDPermReqOprAny,		"Any",		"Yes"}, 

	{PDPermReqObjLink,  "Link", PDPermReqOprAll,		"All",		"Yes"}, 
	{PDPermReqObjLink,  "Link", PDPermReqOprCreate,		"Create",	"Yes"}, 
	{PDPermReqObjLink,  "Link", PDPermReqOprDelete,		"Delete",	"Yes"}, 
	{PDPermReqObjLink,  "Link", PDPermReqOprModify,		"Modify",	"Yes"}, 
	// not used {PDPermReqObjLink,  "Link", PDPermReqOprImport,		"Import",	"Yes"}, 
	{PDPermReqObjLink,  "Link", PDPermReqOprExport,		"Export",	"Yes"}, 
	{PDPermReqObjLink,  "Link", PDPermReqOprAny,		"Any",		"Yes"}, 

	{PDPermReqObjBookmark,  "Bookmark", PDPermReqOprAll,		"All",		"Yes"}, 
	{PDPermReqObjBookmark,  "Bookmark", PDPermReqOprCreate,		"Create",	"Yes"}, 
	{PDPermReqObjBookmark,  "Bookmark", PDPermReqOprDelete,		"Delete",	"Yes"}, 
	{PDPermReqObjBookmark,  "Bookmark", PDPermReqOprModify,		"Modify",	"Yes"}, 
	{PDPermReqObjBookmark,  "Bookmark", PDPermReqOprExport,		"Export",	"Yes"}, 
	{PDPermReqObjBookmark,  "Bookmark", PDPermReqOprAny,		"Any",		"Yes"}, 

	{PDPermReqObjThumbnail,  "Thumbnail", PDPermReqOprAll,		"All",		"Yes"}, 
	{PDPermReqObjThumbnail,  "Thumbnail", PDPermReqOprCreate,	"Create",	"Yes"}, 
	{PDPermReqObjThumbnail,  "Thumbnail", PDPermReqOprDelete,	"Delete",	"Yes"}, 
	{PDPermReqObjThumbnail,  "Thumbnail", PDPermReqOprExport,	"Export",	"Yes"}, 
	{PDPermReqObjThumbnail,  "Thumbnail", PDPermReqOprAny,		"Any",		"Yes"}, 

	{PDPermReqObjAnnot,  "Annot", PDPermReqOprAll,		"All",		"Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprCreate,	"Create",	"Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprDelete,	"Delete",	"Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprModify,	"Modify",	"Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprSummarize,"Summarize","Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprImport,	"Import",	"Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprExport,	"Export",	"Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprCopy,		"Copy",	"Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprOnline,	"Online",	"Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprSummaryView,	"Summary View",	"Yes"}, 
	{PDPermReqObjAnnot,  "Annot", PDPermReqOprAny,		"Any",		"Yes"}, 

	{PDPermReqObjForm,  "Form", PDPermReqOprAll,		"All",		"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprCreate,		"Create",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprDelete,		"Delete",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprModify,		"Modify",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprFillIn,		"Fill In",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprImport,		"Import",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprExport,		"Export",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprSubmitStandalone,		"Submit Standalone",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprBarcodePlaintext,		"Barcode Plain Text",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprSpawnTemplate,			"Spawn Template",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprOnline,		"Online",	"Yes"}, 
	{PDPermReqObjForm,  "Form", PDPermReqOprAny,		"Any",		"Yes"}, 

	{PDPermReqObjSignature,  "Signature", PDPermReqOprAll,		"All",		"Yes"}, 
	{PDPermReqObjSignature,  "Signature", PDPermReqOprCreate,	"Create",	"Yes"}, 
	{PDPermReqObjSignature,  "Signature", PDPermReqOprDelete,	"Delete",	"Yes"}, 
	{PDPermReqObjSignature,  "Signature", PDPermReqOprModify,	"Modify",	"Yes"}, 
	{PDPermReqObjSignature,  "Signature", PDPermReqOprFillIn,	"Fill In",	"Yes"}, 
	{PDPermReqObjSignature,  "Signature", PDPermReqOprAny,		"Any",		"Yes"}, 

	{PDPermReqObjEF,  "EmbeddedFlies", PDPermReqOprAll,		"All",		"Yes"}, 
	{PDPermReqObjEF,  "EmbeddedFlies", PDPermReqOprCreate,	"Create",	"Yes"}, 
	{PDPermReqObjEF,  "EmbeddedFlies", PDPermReqOprDelete,	"Delete",	"Yes"}, 
	{PDPermReqObjEF,  "EmbeddedFlies", PDPermReqOprModify,	"Modify",	"Yes"}, 
	{PDPermReqObjEF,  "EmbeddedFlies", PDPermReqOprImport,	"Import",	"Yes"}, 
	{PDPermReqObjEF,  "EmbeddedFlies", PDPermReqOprExport,	"Export",	"Yes"}, 
	{PDPermReqObjEF,  "EmbeddedFlies", PDPermReqOprAny,		"Any",		"Yes"}, 

	{0, NULL, 0, NULL, NULL}}; 

ASFileSys outputFileSys = NULL;

/*-------------------------------------------------------
	Functions
-------------------------------------------------------*/

// Helper function
char* GetPermStringFromStatus(int status)
{
	// PDPermReqDenied = -1,
	if(status == -1) 
		return("No  ");
	// PDPermReqGranted = 0,
	else if(status == 0) 
		return("Yes ");
	// PDPermReqUnknownObject = 1,
	else if(status == 1) 
		return("Unknown Object");
	// PDPermReqUnknownOperation = 2,
	else if(status == 2) 
		return("Unknown Operation");
	// PDPermReqOperationNA = 3
	else if(status == 3) 
		return("NA");
	else if(status == 4) 
		return("Pending");
	else
		return("Unknown");
}
		
/**	
	The menu function to check PDF permissions  and write it to a text file.
*/ 
ACCB1 void ACCB2 MyPluginCommand(void *clientData)
{
	char strTab[6000];
	char strTxt1[3000]; 
	char strTxt2[3000];
	char str1[80];
	ASInt32 ret;

	// try to get front PDF document 
	AVDoc avDoc = AVAppGetActiveDoc();
	if(avDoc==NULL) return;

	PDDoc pdDoc = AVDocGetPDDoc (avDoc);

  DURING
	// product
	const char* prodName = (const char*) ASGetConfiguration(ASAtomFromString("Product"));
	strcpy(strTab, "Acrobat\tProduct:\t");
	strcpy(strTxt1, "Acrobat\tProduct:\t");
	strcat(strTab, prodName);
	strcat(strTxt1, prodName);

	// version
	AVTVersionNumPart major, minor;
	AVAppGetVersion(&major, &minor);
	sprintf(str1, "\nAcrobat\tVersion:\t%i.%i.%i", major, (minor&0xff00)>>8, (minor&0x00ff));
	strcat(strTab, str1);
	strcat(strTxt1, str1);
	
	// permissions
	strcat(strTab, "\n\nPermission List\n");
	strcat(strTxt1, "\n\nPermission List\n");
	strcat(strTab, "\nObject\t.Operation\tPermission");
	sprintf(str1, "\n                %s        %s         %s\n", "Object", ".Operation", "Permission");
	strcat(strTxt1, str1);
	strcpy(strTxt2, "");
	
	void* pSecData = NULL;
	int status;
	int i;

	for(i=0;PermData[i].ReqObj != 0; i++) {
		sprintf(str1, "\n%s\t.%s\t", PermData[i].ReqObjStr, PermData[i].ReqOprStr);
		strcat(strTab, str1);
		status = PDDocPermRequest(pdDoc, PermData[i].ReqObj, PermData[i].ReqOpr, NULL);
		strcat(strTab, (const char*) GetPermStringFromStatus(status));
		sprintf(str1, "\n%20s%20s%15s", PermData[i].ReqObjStr, PermData[i].ReqOprStr,
										(const char*) GetPermStringFromStatus(status));
		if(i<29)
			strcat(strTxt1, str1);
		else
			strcat(strTxt2, str1);
	}

  HANDLER
	char buf[512];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, sizeof(buf)));

  END_HANDLER

	ASPathName OutFolder = SelectFolder("Choose an output folder");
	ret = 1;

	if(OutFolder!=NULL){
		
		// Save to a tab delimited text file which can be opened by Excel
		// as well as a text file.	
		
		ASFile pdffile = PDDocGetFile (AVDocGetPDDoc(avDoc));
		ASPathName pdfpath = ASFileAcquirePathName (pdffile);

		// Construct output file name from the selected output folder and the 
		// input docuemnt name.
#ifdef WIN_PLATFORM		
		ASText pdf_fname = ASTextNew();
		ASText tab_fname = ASTextNew();
		ASText xls_fname = ASTextNew();
		ASText disp_path = ASTextNew();
		ASFileSysDisplayASTextFromPath(ASGetDefaultUnicodeFileSys(), OutFolder, disp_path);
		ASFileSysGetNameFromPathAsASText(ASGetDefaultUnicodeFileSys(), pdfpath, pdf_fname);
		ASScript bestScript = ASTextGetBestScript(pdf_fname, kASEUnicodeScript);
		const char* pdf_name = ASTextGetScriptText(pdf_fname, bestScript);
		char* p = (char*) strrchr(pdf_name, '.');
		p[0] = '\0';
		pdf_fname = ASTextFromScriptText(pdf_name, bestScript);
		ASTextCatMany(xls_fname, disp_path, ASTextFromScriptText("\\", kASRomanScript), pdf_fname, ASTextFromScriptText("-permissions.xls", kASRomanScript), NULL );
		const ASUTF16Val* xls_name = ASTextGetUnicode(xls_fname);
		ASTextCatMany(tab_fname, disp_path, ASTextFromScriptText("\\", kASRomanScript), pdf_fname, ASTextFromScriptText("-permissions.txt", kASRomanScript), NULL );
		const ASUTF16Val* tab_name = ASTextGetUnicode(tab_fname);
#else
		ASPlatformPath aspPath = NULL;
		ASFileSysAcquirePlatformPath(ASGetDefaultUnicodeFileSys(), OutFolder, ASAtomFromString("POSIXPath"), &aspPath);
		POSIXPath_Ptr outpath = ASPlatformPathGetPOSIXPathPtr(aspPath);
		
		ASFileSysAcquirePlatformPath(ASGetDefaultUnicodeFileSys(), pdfpath, ASAtomFromString("POSIXPath"), &aspPath);
		POSIXPath_Ptr fname = ASPlatformPathGetPOSIXPathPtr(aspPath);
		char* p = strrchr((char*)fname, '/');
		p++;
		p[strlen(p)-4] = '\0';
		
		char xls_fname[128], tab_fname[128];
		sprintf(xls_fname, "%s%s-permissions.xls", outpath, p);
		sprintf(tab_fname, "%s%s-permissions.txt", outpath, p);
#endif			
		// Call the helper routines to save permission info to output

		ret = SaveStringToFile(xls_fname, strTab);
		if(ret!=0)
			AVAlertNote("Failed to save the Excel output file.");

		strcpy (strTab, strTxt1);
		strcat (strTab, strTxt2);
		ret = SaveStringToFile(tab_fname, strTab);
		if(ret!=0)
			AVAlertNote("Failed to save the text output file.");
	}	

	ASFileSysReleasePath (outputFileSys, OutFolder);

	// message
	if(ret==0) {
		sprintf(str1, "The permission list for the PDF was saved to files in the output folder chosen.");
	}
	else {
		sprintf(str1, "The permission list for the PDF was not saved.");
	}
	AVAlertNote(str1);
}



/* internal function */
ASPathName SelectFolder(char* title)
{
		ASPathName thePath;

   DURING
		// Configure the dialog box parameters.
		AVOpenSaveDialogParamsRec dialogParams;
		memset (&dialogParams, 0, sizeof (AVOpenSaveDialogParamsRec));
		dialogParams.size = sizeof(AVOpenSaveDialogParamsRec);
		dialogParams.initialFileSys = ASGetDefaultUnicodeFileSys();
		dialogParams.flags |= kAVOpenSaveAllowForeignFileSystems;
		dialogParams.windowTitle = ASTextFromScriptText(title,kASRomanScript);

		// pop up to get user selection
		AVAppBeginModal(NULL);
		if(ASBoolToBool(AVAppChooseFolderDialog(&dialogParams, &outputFileSys, &thePath)) != true){
			AVAppEndModal();
			AVAlertNote("Failed to select a folder");
			E_RETURN(NULL);
		}		
		AVAppEndModal();

   HANDLER
		char buf[512];
		AVAlertNote(ASGetErrorString(ERRORCODE, buf, sizeof(buf)));

   END_HANDLER

	    return thePath;
}

/* Helper function */
ASInt32 SaveStringToFile(char* path,  char* data)
{
	ASInt32 ret = 1;
	
  DURING
  
	ASFileSys fileSys = ASGetDefaultFileSysForPath(ASAtomFromString("POSIXPath"), path);
	ASPathName outFilename = ASFileSysCreatePathName (fileSys,
							ASAtomFromString("POSIXPath"), path, 0);

	ASFile fp;
	if(outFilename==NULL) {
		AVAlertNote("Failed to create output file path.");
	}
	else {
		// open and write
		ret = ASFileSysOpenFile (fileSys, outFilename,
				ASFILE_CREATE|ASFILE_WRITE, &fp);
		if(ret==0){
			ASFileWrite (fp, data, strlen (data));
			ASFileClose(fp);
		}
		else {
			AVAlertNote("Failed to open output file.");
		}
	}

  HANDLER
	char buf[512];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, sizeof(buf)));

  END_HANDLER

	return ret; 
}

// Overloaded to handle Windows output for Unicode file name support
ASInt32 SaveStringToFile(ASText fname,  char* data)
{
	ASInt32 ret = 1;
	
  DURING
	ASFileSys fileSys = ASGetDefaultFileSysForPath(ASAtomFromString("ASTextPath"), fname);
	
	ASPathName outFilename = ASFileSysCreatePathName (fileSys,
						ASAtomFromString("ASTextPath"), fname, 0);
	
	ASFile fp;
	if(outFilename==NULL) {
		AVAlertNote("Failed to create output file path.");
	}
	else {
		// open and write
		ret = ASFileSysOpenFile (fileSys, outFilename,
				ASFILE_CREATE|ASFILE_WRITE, &fp);
		if(ret==0){
			ASFileWrite (fp, data, strlen (data));
			ASFileClose(fp);
		}
		else {
			AVAlertNote("Failed to open output file.");
		}
	}

  HANDLER
	char buf[512];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, sizeof(buf)));

  END_HANDLER

	return ret; 
}



