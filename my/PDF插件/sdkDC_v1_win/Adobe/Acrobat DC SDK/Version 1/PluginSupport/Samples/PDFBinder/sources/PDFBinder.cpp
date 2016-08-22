/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/ 
/**
  \file PDFBinder.cpp

 - Implements a method to convert multiple files into PDFs and bind the PDFs 
 into one PDF file. The code shows how to use AVConversionToPDFHandler to 
 convert files to PDF and how to use PDDocInsertPages() to combind PDF files. 

*********************************************************************/
// Headers.
#include <stdio.h>
#include <ctype.h>

#ifndef MAC_PLATFORM
#include <direct.h> 
#include "PIHeaders.h"
#endif

// Defines ----------
// Define this flag to show AVConversionToPDFEnumHandler info. in log file. 
#define SHOW_TOPDF_HANDLER_TYPES    

// define this flag to use file type filter  
#define USE_FILE_FILTER    
// ------------------

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/
// Message display control.  Ture: display
ASBool bEcho = true;

// File location. User can change the following filenames.
#if WIN_PLATFORM
	const char* STRING_PDFBinderFolder = "C:\\PDFBinder";
	const char* FOLDERSYMBOL="\\";
#endif
#if MAC_PLATFORM
	const char* STRING_PDFBinderFolder = "/PDFBinder";
	const char* FOLDERSYMBOL="/";
#endif

char* OutputPdfFileName = "PDFBinderOutput.pdf";
char* LogFileName = "PDFBinderLog.txt";

const int MAX_FILENAME_LENGTH = 256;
char sLogFileName[MAX_FILENAME_LENGTH];

// Filter -----------
// Filter is a list of file types which can be converted to PDF in this program.
// Use a filter may fit user's specific needs and ensure a smooth automation process.
// Of course, the user can turn off the filter in two ways: 
//   (1) gPDFBinderFileFilter = "";
//   (2) comment out //USE_FILE_FILTER  
// The filter should be consistent with the Acrobat viewer's menu capability: 
//   Create PDF from multiple files ...
// Here we put the types which have been tested, but user can expand it.
const char* gPDFBinderFileFilter = "pdf,bmp,gif,jpg,png,tif,ps,doc,ppt,rtf,xls,pps,PDF,BMP,GIF,JPG,PNG,TIF,PS,DOC,PPT,RTF,XLS,PPS";
// ------------------

const int MAX_STRING_LENGTH = 2048;
AVConversionToPDFHandler gHandler;

// functions in this file.
int	ConvertAndInsertFileToPDF(ASPathName aspItem, char* aFileName, PDDoc  PDFfile);
void BindingFileToPDF(ASPathName aspItem, char* aFilename, ASFileSysItemPropsRec nFileAttrb, PDDoc targetPDdDoc, 
					  int *ipTotal, int *ipConverted);
ASBool PassPDFBinderFileFilter(char* filename);  


// callbacks
ACCB1 ASBool ACCB2 MyAVConversionToPDFEnumProc(AVConversionToPDFHandler handler,
											 AVConversionEnumProcData data);
ACCB1 ASBool ACCB2 AVConversionToPDFEnumFindHandler(AVConversionToPDFHandler handler,
											 AVConversionEnumProcData data);

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/ 

/**    This sample implements a method to convert multiple files into PDF 
  and bind the PDFs into one PDF file .

	This sample shows developers how to grammatically implement To-PDF file 
  converson functionality.  Basically, the code uses AVConversionToPDFHandler 
  to convert files to PDF and PDDocInsertPages()to combine PDF files. 
  This plug-in is designed to run from a menu item, and by setting bEcho = false 
  it may be run without any graphics user interface on the screen. Therefore, it  
  can be executed from other programs such as C IAC, VB IAC, and JavaScript
  to meet enterprise workflow needs.
  
	How to run it :
	- This sample adds a menu item "PDF Binder" under the Acrobat SDK submenu. Two ways to 
	execute it: 1) click the menu item to run the program using a fixed file location 
	hard coded; 2) press the shift key and click the menu item to choose a folder where 
	files to be converted are located.
	- Without folder selection, this original code is to convert and bind files in a 
	directory	C\test\PDFBinder for Win, or a folder   MacHD:test:PDFBinder for Mac. 
	You can copy the test files in the project's testfiles folder to the appropriate location  
	for testing. You may locate files to be converted in another directory/folder, and 
	specify the location in the string variable PDFBinderFolder in the code.
	- When you have the files to be converted ready in the specified location, 
	run Acrobat 6 menu Advanced->AcrobatSDK->PDF Binder to start the operation.  There are 
	echo messages informing you of the operation start and end, but you can change and 
	rebuild the code to turn off the display on screen.
	- An output file PDFBinderOutput.pdf in the same location is created 
	when the program succeeds. A text log file  PDFBinderLog.txt in the same 
	location records the process and results.
	-  You can set bEch = false in the code to turn off any display on the screen. 
	This is necessary when you  call the menu function from within other programs 
	of C IAC, VB IAC, JavaScript.  
	-  An optional file filter is used to pre-process the files. The filter only allows 
	the files with predefined types to be processed. You can change the file type list 
	as you wish. Using a filter may ensure a smooth automation process, since you can put 
	only well-tested file types in the filter. To turn off the filter, you can set 
	gPDFBinderFileFilter = "" or not to define USE_FILE_FILTER  
.
	- Note this is a sample only, developers need to make further improvement for 
	their actual use. For example, you can set up a list of files to use the method for 
	the conversion and binding. And you can add code to handle special file types. 

  	@see AVConversionConvertToPDFWithHandler
	@see AVConversionEnumToPDFConverters
	@see ASFileSysDestroyFolderIterator
	@see PDDocOpen
	@see PDDocClose
	@see PDDocInsertPages
	@see PDDocCreatePage
	@see PDDocDeletePages
	@see PDPageRelease
	@see ASFileSysCreatePathName
	@see ASFileSysFirstFolderItem
	@see ASFileSysNextFolderItem
	@see ASFileSysGetNameFromPath
	@see ASFileSysReleasePath
*/
ACCB1 void ACCB2 PDFBinderCommand(void*)
{
	// if the menu item clicked with Shift key pressed down, go to interactive mode.
	// let the user to choose a folder and process all the file in the folder.
	
	ASText dispText = ASTextNew();
	char PDFBinderFolder[MAX_STRING_LENGTH];

	ASBool shiftKeyIsDown = ((AVSysGetModifiers() & AV_SHIFT) != 0);
	if(shiftKeyIsDown) {
		AVOpenSaveDialogParamsRec dialogParams;
		// Configure the dialog box parameters.
		memset (&dialogParams, 0, sizeof (AVOpenSaveDialogParamsRec));
		dialogParams.size = sizeof(AVOpenSaveDialogParamsRec);

		dialogParams.windowTitle = ASTextFromScriptText("Choose folder with files to bind",kASRomanScript);
		dialogParams.flags |= kAVOpenSaveAllowForeignFileSystems;
		dialogParams.initialFileSys = ASGetDefaultUnicodeFileSys();

		ASPathName thePath;
		ASFileSys fileSys;
		AVAppBeginModal(NULL);
		if(ASBoolToBool(AVAppChooseFolderDialog(&dialogParams, &fileSys, &thePath)) != true){
			AVAppEndModal();
			AVAlertNote("Failed to select the folder");
			return;
		}		
		AVAppEndModal();

#ifdef WIN_PLATFORM		
		ASFileSysDisplayASTextFromPath(ASGetDefaultUnicodeFileSys(), thePath, dispText);
		ASHostEncoding bestEnc = ASTextGetBestEncoding(dispText, (ASHostEncoding)PDGetHostEncoding());
		strncpy(PDFBinderFolder, ASTextGetEncoded(dispText, bestEnc), MAX_STRING_LENGTH - 1);
#else		
		ASPlatformPath aspPath;
		ASFileSysAcquirePlatformPath(fileSys, thePath, ASAtomFromString("POSIXPath"), &aspPath);
		strncpy(PDFBinderFolder, (char *)ASPlatformPathGetPOSIXPathPtr(aspPath), MAX_STRING_LENGTH - 1);
		ASFileSysReleasePlatformPath(fileSys, aspPath);

		// remove extra path separator 
		char e = PDFBinderFolder[strlen(PDFBinderFolder)-1];
		if(e == ':' || e == '/') 
			PDFBinderFolder[strlen(PDFBinderFolder)-1]='\0';

		dispText = ASTextFromScriptText(PDFBinderFolder, kASEUnicodeScript);
#endif
		ASFileSysReleasePath(fileSys, thePath);

	}
	// Otherwise, the hard coded file path will be used. 
	else {
		strcpy(PDFBinderFolder, STRING_PDFBinderFolder);
		dispText = ASTextFromScriptText(PDFBinderFolder, kASRomanScript);
	}

	// instruction message
	char strMsg[MAX_STRING_LENGTH] = "";
	sprintf(strMsg, "This function will convert and bind files in the selected directory into one PDF document. "); 
	strcat(strMsg, " You need to copy your files or the test files in PDFBinder sample project to the directory." ); 
	strcat(strMsg, " If you are ready, click OK to continue, or click Cancel to quit. " ); 
	strcat(strMsg, " \nNote you can press down Shift key and click the menu item to have a folder selection. " ); 
	strcat(strMsg, " \nNote you can turn off the echo message by setting bEcho = false and rebuild the code. " ); 
	if(ASBoolToBool(bEcho)==true){
		ASInt32 choice = AVAlert(ALERT_CAUTION, strMsg, "OK", "Cancel", NULL, true);
		if(choice==2) {
			return;
		}
	}
	
	// create a new target pdf file with one empty page
	PDDoc TargetPDF = PDDocCreate();
	ASFixedRect mediaBox = { fixedZero, ASInt32ToFixed(792), ASInt32ToFixed(612), fixedZero };
	PDPage emptyPage = PDDocCreatePage (TargetPDF, PDBeforeFirstPage, mediaBox);
	PDPageRelease (emptyPage);
	
	//	 get available headlers and set our file filter
	char ValidExt[MAX_STRING_LENGTH];
	memset(ValidExt, 0, MAX_STRING_LENGTH);

#ifdef SHOW_TOPDF_HANDLER_TYPES	

	// enumerate AVConversioToPDF handlers to get all available file 
	// types which can be converted to PDF. 
	AVConversionEnumToPDFConverters(MyAVConversionToPDFEnumProc, 
									(AVConversionEnumProcData) ValidExt);
#endif

#ifdef USE_FILE_FILTER     
	
	// set File filter
	strcpy(ValidExt, gPDFBinderFileFilter);

#endif

	// process all files in the directory
	int iNumFiles = 0;
	int iNumFilesConverted = 0;
	char* Done = "Converted";
	char* NotDone = " - ";
	char fileName[MAX_FILENAME_LENGTH+1];
	
	ASFileSysItemPropsRec props;
	ASFolderIterator iter;
	ASPathName aspItem = NULL;

	memset(&props, 0, sizeof(props));
	props.size = sizeof(props);

	ASPathName theFolder = NULL;
	ASFileSys fileSys = NULL;

	DURING

#ifdef WIN_PLATFORM	
	fileSys = ASGetDefaultFileSysForPath(ASAtomFromString("ASTextPath"), dispText);
	theFolder = ASFileSysCreatePathName (fileSys,
							ASAtomFromString("ASTextPath"), dispText, 0);
#else
	fileSys = ASGetDefaultFileSysForPath(ASAtomFromString("POSIXPath"), PDFBinderFolder);
	theFolder = ASFileSysCreatePathName (fileSys,
							ASAtomFromString("POSIXPath"), PDFBinderFolder, 0);
#endif
							
	if (theFolder==NULL) {
		AVAlertNote("Fail ro create ASPathName for the selected folder.");
	    E_RTRN_VOID
	}

    // Find first file in current directory 
    if((iter = ASFileSysFirstFolderItem(fileSys, theFolder, &props, &aspItem))==FALSE) {
		AVAlertNote("This is either an invalid folder or the folder contains no files.");
	    E_RTRN_VOID
	}
	ASFileSysReleasePath(fileSys, theFolder);

	// get filename only 
	if(ASFileSysGetNameFromPath(fileSys, aspItem, fileName, sizeof(fileName))) {
	    E_RTRN_VOID
	}
	
	HANDLER	
		
		// exception handling
		char errorMsg[256];
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);
		if(aspItem!=NULL) ASFileSysReleasePath(fileSys, aspItem);
		if(theFolder!=NULL) ASFileSysReleasePath(fileSys, theFolder);

		// display an error message.
		if(ASBoolToBool(bEcho)==true) {
			AVAlertNote (errorMsg);
		}
		return;
	
	END_HANDLER

	// go to binding if it's a valid type of file 
	if(props.type == kASFileSysFile && PassPDFBinderFileFilter(fileName)) { 
		BindingFileToPDF(aspItem, fileName, props, TargetPDF, 
						&iNumFiles, &iNumFilesConverted);
	}
	ASFileSysReleasePath(fileSys, aspItem);
	
	// process all other files  
	while(ASFileSysNextFolderItem(fileSys, iter, &props, &aspItem)) {

		// get filename only 
		if(ASFileSysGetNameFromPath(fileSys, aspItem, fileName, sizeof(fileName))) {
			return;
		}
		
		// go to binding if it's a valid type of file 
		if(props.type == kASFileSysFile && PassPDFBinderFileFilter(fileName))  {
			BindingFileToPDF(aspItem, fileName, props, TargetPDF, 
							&iNumFiles, &iNumFilesConverted);
		}

		ASFileSysReleasePath(fileSys, aspItem);
	}

	ASPathName pdfPathName = NULL;

	DURING
	
	// close the findfile
	ASFileSysDestroyFolderIterator(fileSys, iter);

	// save and close the output pdf file
	// create path

	ASText pathText = ASTextNew();
	ASTextCatMany(
		pathText, 
		dispText,
		ASTextFromScriptText(FOLDERSYMBOL, kASRomanScript),
		ASTextFromScriptText(OutputPdfFileName, kASRomanScript),
		NULL);
		
#ifdef WIN_PLATFORM
	ASFileSys fileSys = ASGetDefaultFileSysForPath(ASAtomFromString("ASTextPath"), pathText);
	pdfPathName = ASFileSysCreatePathName (fileSys,
							ASAtomFromString("ASTextPath"), pathText, 0);
#else
	char path[MAX_STRING_LENGTH];
	strncpy(path, ASTextGetEncoded(pathText, 
		ASTextGetBestEncoding(pathText, (ASHostEncoding)PDGetHostEncoding())), MAX_STRING_LENGTH - 1);
	ASFileSys fileSys = ASGetDefaultFileSysForPath(ASAtomFromString("POSIXPath"), path);
	pdfPathName = ASFileSysCreatePathName (fileSys,
							ASAtomFromString("POSIXPath"), path, 0);	
#endif

	if(pdfPathName==NULL) {
		E_RTRN_VOID
	}

	// delete the empty page
	if(PDDocGetNumPages(TargetPDF)>1) {
		PDDocDeletePages (TargetPDF, 0,0,NULL, NULL);
	}
		
	// save
	PDDocSave (TargetPDF, PDSaveFull | PDSaveCollectGarbage | PDSaveLinearized,
				pdfPathName, ASGetDefaultUnicodeFileSys(),	NULL, NULL);
	PDDocClose (TargetPDF);
	ASFileSysReleasePath(fileSys, pdfPathName);

	HANDLER	
		
		// exception handling
		char errorMsg[256];
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);
		if(pdfPathName!=NULL) ASFileSysReleasePath(fileSys, pdfPathName);
	
		// display an error message.
		if(ASBoolToBool(bEcho)==true) {
			AVAlertNote (errorMsg);
		}
		return;
	
	END_HANDLER
	
	// use this line to show the massage on screen if you need
	if(ASBoolToBool(bEcho)==true) {
		AVAlertNote ("Document binding completed. Output PDF file generated.");
	}
	return;
}


/* ConvertAndInsertFileToPDF
** ------------------------------------------------------
** */ 
/** Internal function \n
 ** Convert a file to PDF, then insert it to target PDF file.
 ** @param aFileName IN  char string, filename.
 ** @param PDFfile IN/OUT PDDoc of the target PDF file.
 **
 ** @return 0 if OK, 1 if failed.
 ** */

int	ConvertAndInsertFileToPDF(ASPathName aPathName, char* aFileName, PDDoc PDFfile)
{
	PDDoc tempPDF;
	char* Ext;
		
	DURING

		// get file extention
		Ext = strrchr(aFileName,'.');
		if(Ext) {
			Ext++;
		}
		else {
			E_RETURN (1);
		}
		
		// if the file is PDF, open PDDoc
		if(strcmp(Ext,"PDF")==0 || strcmp(Ext,"pdf")==0) {
			tempPDF = PDDocOpen (aPathName, ASGetDefaultFileSys(), NULL, true);
			if(tempPDF==NULL) {
				PDDocClose(tempPDF);
				E_RETURN (1);
			}
		}
		 
		// else convert it to PDF
		else 
		{
			// let it automatically find right handler for the file and do the job.
			AVConversionStatus stat = AVConversionConvertToPDF
				(kAVConversionAsyncOkay, //kAVConversionNoFlags 
				  aPathName, ASGetDefaultFileSys(), &tempPDF, NULL);

			// check if successful
			if(stat != kAVConversionSuccess) {
				AVAlertNote("Cannot convert the file.");
				E_RETURN (1);
			}
		}

	HANDLER	
		
		// exception handling
		char errorMsg[256];
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);
	
		// display an error message.
		if(ASBoolToBool(bEcho) == true) {
			AVAlertNote (errorMsg);
		}
		
		return 1;
	
	END_HANDLER

	// insert new PDF into target PDF
	DURING
		// insert all pages of new pdfc to TargetPDFfile.
		PDDocInsertPages(PDFfile, PDLastPage, tempPDF, 0, 
						PDAllPages, PDInsertAll, NULL, NULL, NULL, NULL);

		PDDocClose(tempPDF);

	HANDLER
	 	char errorMsg[256];
		ASGetErrorString (ASGetExceptionErrorCode(), errorMsg, 256);
		if(ASBoolToBool(bEcho)==true) {
			AVAlertNote(errorMsg);
		}
	END_HANDLER
	
		if(strcmp(Ext,"doc")==0) {
			char fname[40];
			sprintf(fname,"%s.pdf",aFileName);
			remove(fname);
		}
		return 0;
}


/* MyAVConversionToPDFEnumProc
** ------------------------------------------------------
** */ 
/** Internal callback function \n
 ** called for every AVConversionToPDFHandler.
 ** get the valid type (file extention) for the conversion.  
 ** @return true.
*/
ACCB1 ASBool ACCB2 MyAVConversionToPDFEnumProc(AVConversionToPDFHandler handler,
											 AVConversionEnumProcData data)
{
	ASUns16 numFileExt = handler->convFilter.numFileDescs;

	for (int i = 0; i < numFileExt; i++) {
		char* ext = handler->convFilter.fileDescs[i].extension;
		if(strlen(ext)>0) {
			strcat((char*) data, ext);
			strcat((char*) data, ",");
		}
	}

	return true;
}

/* this is alternative code to get right conversion handler for a specific file
   to be converted. It works with function AVConversionConvertToPDFWithHandler.
   It's not used now, but kept for user's reference.

ACCB1 ASBool ACCB2 AVConversionToPDFEnumFindHandler(AVConversionToPDFHandler handler,
											 AVConversionEnumProcData data)
{
	ASUns16 numFileExt = handler->convFilter.numFileDescs;

	for ( int i = 0; i < numFileExt; i++)
	{
		char* ext = handler->convFilter.fileDescs[i].extension;
		if(strlen(ext)) {
			if(!strcmp((char*) data, ext)) {
				gHandler = handler;
				return false;
			}
		}
	}

	return true;
}
*/


/* BindingFileToPDF
** ------------------------------------------------------
** */
/** Internal callback function for binding process \n
 ** IN: char* aFilename, ASFileSysItemPropsRec nFileAttrb \n
 **     PDDoc targetPDdDoc, int *ipTotal, \n
 **     int *ipConverted, FILE *logfile \n
 ** OUT: int *ipTotal, int *ipConverted. \n
 ** */
void BindingFileToPDF(ASPathName aPathName, char* aFilename, ASFileSysItemPropsRec nFileAttrb, PDDoc targetPDdDoc, 
					  int *ipTotal, int *ipConverted)
{
	// ignore system, hidden files, and our target PDF and log file.
	if( nFileAttrb.isHidden  
		|| (strcmp(aFilename,".")==0) || (strcmp(aFilename,".." )==0)
		|| (strcmp(aFilename,OutputPdfFileName)==0) || (strcmp(aFilename,LogFileName)==0)) {
			return;
	}
	
	(*ipTotal)++;
	
	char msg[MAX_STRING_LENGTH] = "";
	char* Done = "Converted";
	char* NotDone = " - ";

	// try to convert and insert it to target PDF file. 
	// rc would be 0 for success, otherwise 1.
	int rc = ConvertAndInsertFileToPDF(aPathName, aFilename, targetPDdDoc);
	
	// if converted
	if(rc==0) {
		(*ipConverted)++;
	}
}

/* PassPDFBinderFileFilter
** ------------------------------------------------------
** */
/** Internal callback function for binding process
 ** @return true if the IN aFilename is a valid file for PDF conversion, 
 ** otherwise false.
 ** */
ASBool PassPDFBinderFileFilter(char* aFilename)
{
#ifdef USE_FILE_FILTER

	// if the filter is empty, any file will pass.
	if(strlen(gPDFBinderFileFilter)==0)
		return true;

	char* Ext = strrchr(aFilename,'.');
	if(Ext!=NULL) {
		Ext++;
	
		if(strstr(gPDFBinderFileFilter, Ext)) {
			return true;
		}
	}

	return false;

#else
		// not use filter, any file will pass.
		return true;
#endif

}

