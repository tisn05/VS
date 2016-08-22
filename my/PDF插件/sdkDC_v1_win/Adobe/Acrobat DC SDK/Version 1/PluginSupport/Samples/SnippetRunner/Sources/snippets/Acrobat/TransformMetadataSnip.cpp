/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------
 TransformMetadataSnip.cpp

 - Process XML metadata strings that are generated from batch
   processing to merge the standard Adobe Metadata schema sections into
   one single XML file.
 - Export the generated XML with a hard coded file name.
 - Generate a hard coded XSL transformation template and export it to
   the same directory as the XML output.
 - Tranform XML to XHTML
 - This sample works on Windows only.
*********************************************************************/
#define UNICODE
#ifdef WIN_PLATFORM

#include "SnippetRunner.h"
#include <assert.h>
#include <windows.h>
#include <windowsx.h>

// Maximum string length of input xml file, increase it if needed for a large xml file
#define metaDataLength 100000

#define MAX_PATH_LENGTH 256
#define MAX_NUM_OPENFILE 10
#define XSLfileName "docTemplate.xsl"
#define XMLfileName "docMetadata.xml"

ASText XMLfilePath;
bool GetMetadata(char **buf, PDDoc *docs, ASInt32 numDocs);
static bool openFile(char * filetype, char *data, PDDoc *docs, ASInt32 *numDocs, ASFile *asfileP);
static void ReadXMLFile(ASFile *printFile, char *data);
static void WriteXMLFile(ASFile *printFile, char *data);
bool DisplayInIE(ASText file);


/** MergeXMLMetadata
** ----------------------------------------------------------------------
** Process XML matadata strings that are generated from batch processing
** to merge the standard Adobe Metadata schema sections into one single XML file
** Input: a string array
** Output: a string
*/
void MergeXMLMetadata (char * metadataInBufs[], ASInt32 numDocs, char * metadataOutBuf)
{
	// Construct XSL headers
	strcpy (metadataOutBuf,
	"<?xml version='1.0'?> \n \
<?xml-stylesheet type='text/xsl' href='");
	strcat (metadataOutBuf, XSLfileName);
	strcat (metadataOutBuf, "'?> \n <rdf:RDF xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#' \n \
xmlns:iX='http://ns.adobe.com/iX/1.0/'> \n");

	int headerLen = strlen(metadataOutBuf);
	char *pdest1, *pdest2;
	int  strStart, strEnd;
	char str1[30] = "<rdf:Description", str2[30] = "</rdf:Description>";

	// Only process the PDF schema properties
	for (int i = 0; i < numDocs; i++)
	{
		pdest1 = strstr( metadataInBufs[i], str1);
		strStart = pdest1 - metadataInBufs[i] + 1;

		pdest2 = strstr( metadataInBufs[i], str2);
		strEnd = pdest2 - metadataInBufs[i] + 1 + strlen(str2);

		strncat (metadataOutBuf, pdest1, strEnd - strStart);
		strcat (metadataOutBuf, "\n");
	}
	strcat (metadataOutBuf, "</rdf:RDF>");
}


/** ExportXML
** ----------------------------------------------------------------------
** Export the merged XML to the same directory where XSL template exists
*/
void ExportXML(char *dataXML, char *dataXSL, PDDoc *docs)
{
	ASPathName volatile pathName = NULL;
	char * volatile pDIPath = NULL;
	char buf[284];
	ASFile file = NULL, fpm = NULL, fpw = NULL;
	ASText XSLfilePath;

DURING
	// Compose the XML filepath name.
	file = PDDocGetFile (docs[0]);
	pathName = ASFileAcquirePathName (file);

	ASPlatformPath platformPath;
	ASFileSysAcquirePlatformPath(
		ASGetDefaultUnicodeFileSys(), 
		pathName, 
		ASAtomFromString("WinUnicodePath"), 
		&platformPath);

	ASUTF16Val* basePath = (ASUTF16Val*)ASPlatformPathGetCstringPtr(platformPath);
	wchar_t * sepPos = wcsrchr((wchar_t *)basePath, (wchar_t)'\\');
	sepPos[1] = '\0';

	ASText astPathName = ASTextFromUnicode(basePath, kUTF16HostEndian);		

	// Open the XML and XSL files.
	XMLfilePath = ASTextNew();
	ASTextCatMany(XMLfilePath, astPathName, ASTextFromScriptText(XMLfileName, kASRomanScript), NULL);

	const ASUTF16Val* astPath = ASTextGetUnicode(XMLfilePath);

	XSLfilePath = ASTextNew();
	ASTextCatMany(XSLfilePath, astPathName, ASTextFromScriptText(XSLfileName, kASRomanScript), NULL);

	ASFileSys fileSys = ASGetDefaultFileSysForPath(ASAtomFromString("ASTextPath"), astPathName);
	pathName = ASFileSysCreatePathName (fileSys, ASAtomFromString("ASTextPath"), XMLfilePath, 0);
	if (ASFileSysOpenFile (fileSys, pathName, ASFILE_WRITE | ASFILE_CREATE, (ASFile*)&fpm)) {
		sprintf (buf, "Couldn't create xml file %s.", XMLfilePath);
		AVAlert (ALERT_STOP, buf, "OK", NULL, NULL, false);
		ASRaise (GenError(genErrGeneral));
	}

	// Export XML file
	WriteXMLFile (&fpm, dataXML);

	ASFileSysReleasePath (NULL, pathName);
	pathName = NULL;

	pathName = ASFileSysCreatePathName (fileSys, ASAtomFromString("ASTextPath"), XSLfilePath, 0);
	if (ASFileSysOpenFile (fileSys, pathName, ASFILE_WRITE | ASFILE_CREATE, (ASFile*)&fpw)) {
		sprintf (buf, "Couldn't create xsl file %s.", XSLfilePath);
		AVAlert (ALERT_STOP, buf, "OK", NULL, NULL, false);
		ASRaise (GenError(genErrGeneral));
	}

	WriteXMLFile (&fpw, dataXSL);

	ASFileSysReleasePath (NULL, pathName);
	pathName = NULL;

HANDLER
	AVAlertNote ("Error in export XML file");
	return;
END_HANDLER
}

/** bufXSL
** ----------------------------------------------------------------------
** For convenience, XSL template is hard coded here as a constant string
*/
#define bufXSL "<?xml version='1.0'?> \
<xsl:stylesheet xmlns:xsl='http://www.w3.org/TR/WD-xsl'> \
\
<xsl:template match='/'> \
 <HTML> \
 <BODY> \
 <H2 align='center'>Summary of PDF Files </H2> \
 <TABLE BORDER='2'> \
\
 <TR> \
		<TD>Title</TD> \
        <TD>Author</TD> \
        <TD>Producer</TD> \
        <TD>Creator</TD> \
        <TD>Creation Date</TD> \
        <TD>Modify Date</TD> \
		<TD>Key Words</TD> \
        <TD>Subject</TD> \
 </TR> \
\
 <xsl:apply-templates/> \
   </TABLE> \
   </BODY> \
   </HTML> \
 </xsl:template> \
\
 <xsl:template match='rdf:RDF'> \
 <xsl:apply-templates/> \
 </xsl:template> \
\
  <xsl:template match='rdf:Description'> \
  <TR><TD><xsl:value-of select='dc:title'/></TD> \
<TD><xsl:value-of select='dc:creator'/></TD> \
  <TD><xsl:value-of select='@pdf:Producer'/></TD> \
  <TD><xsl:value-of select='@xap:CreatorTool'/></TD> \
  <TD><xsl:value-of select='@xap:CreateDate'/></TD> \
  <TD><xsl:value-of select='@xap:ModifyDate'/></TD> \
  <TD><xsl:value-of select='@pdf:Keywords'/></TD> \
  <TD><xsl:value-of select='dc:description'/></TD> \
  </TR> \
  </xsl:template> \
  </xsl:stylesheet>"

/** - Process XML matadata strings that are generated from batch
	processing to merge the standard Adobe Metadata schema sections into
	one single XML file.
	- Export the generated XML, file path is hard coded.
	- Generate a hard coded XSL transformation template and export it to
	the same directory as the XML output.
	- Tranform XML to XHTML
	- Display the XML file in IE
	@see PDDocGetFile
	@see ASFileAcquirePathName
	@see ASFileSysDIPathFromPath
	@see ASFileSysReleasePath
	@see ASFileSysCreatePathName
	@see ASFileSysOpenFile
	@see PDDocGetXAPMetadata
	@see ASTextGetPDTextCopy
	@see AVAppOpenDialog
	@see ASFileSysGetNameFromPath
	@see PDDocOpen
	@see ASFileSysOpenFile
	@see ASFileSetPos
	@see ASFileWrite
	@see ASFileClose
*/
void TransformMetadataSnip()
{
	char ** buf;
	char * bufXML = new char [metaDataLength];

	PDDoc *docs = (PDDoc *) ASmalloc(MAX_NUM_OPENFILE * sizeof(PDDoc));
	ASFile *fP = (ASFile *) ASmalloc(MAX_NUM_OPENFILE * sizeof(ASFile));
	ASInt32 numDocs;
	if (openFile("PDF", NULL, docs, &numDocs, fP))
	{
		buf = new char *[numDocs];
		for (int i = 0; i < numDocs; i++)
			buf[i] = new char [metaDataLength];

		if (GetMetadata(buf, docs, numDocs)){
			MergeXMLMetadata (buf, numDocs, bufXML);
			ExportXML(bufXML, bufXSL, docs);
		}

		if (!DisplayInIE(XMLfilePath))
			AVAlertNote("Can't display metadata in IE.");
	}
	delete bufXML, buf;
}

/** GetMetadata
** ----------------------------------------------------------------------
** Obtain Adobe metadata from the array of PD Docs passed in as PDDocs *docs.
** Output the number of PDDoc opened and an array string buf to store metadata
*/
bool GetMetadata(char **buf, PDDoc *docs, ASInt32 numDocs)
{
	ASText metaDataAstext;
	for (int i = 0; i < numDocs; i++)
	{
		metaDataAstext = PDDocGetXAPMetadata (docs[i]);

		if (metaDataAstext){
			// convert ASText to a C string
			ASInt32 metaStrLen;
			buf[i] = (char*) ASTextGetPDTextCopy (metaDataAstext, &metaStrLen);
		}
		else
			return false;
	}
	return true;
}

/** openFile
** ----------------------------------------------------------------------
** Open a file by prompting a file open dialog.
** Input Param: filetype - used to specify the file extension
** Output param: data - a buffer that stores the file
** Return: true if successful, false otherwise
*/
static bool openFile(char * filetype, char *data, PDDoc *docs, ASInt32 *numDocs, ASFile *fP)
{
	AVOpenSaveDialogParamsRec dialogParams;
	ASPathName *pathNames;
	ASUns16 numPathnames;
	ASFileSys fileSys = NULL;

	AVFileFilterRec filterRec, *filterRecP;
	AVFileDescRec descRec;

	filterRecP = &filterRec;
DURING
	memset(&dialogParams, 0, sizeof(dialogParams));
	filterRec.filterDescription = ASTextNew();

	ASTextSetEncoded(filterRec.filterDescription, filetype, AVAppGetLanguageEncoding());

	filterRec.fileDescs = &descRec;
	filterRec.numFileDescs = 1;

	strcpy(descRec.extension, filetype);
	descRec.macFileType = NULL;
	descRec.macFileCreator = NULL;

	dialogParams.size = sizeof(dialogParams);
	if (!strcmp(filetype, "PDF"))
		dialogParams.flags = kAVOpenSaveAllowMultiple;
	else
		dialogParams.flags = kAVOpenSaveAllowAllFlag;
	dialogParams.parentWindow = NULL;
	dialogParams.windowTitle = NULL;
	dialogParams.actionButtonTitle = NULL;
	dialogParams.initialFileSys = ASGetDefaultUnicodeFileSys();
	dialogParams.initialPathName = NULL;
	dialogParams.initialFileName = ASTextNew();
	dialogParams.fileFilters = &filterRecP;
	dialogParams.numFileFilters = 1;
	dialogParams.settingsComputeEnabledProc = NULL;
	dialogParams.settingsExecuteProc = NULL;
	dialogParams.settingsProcData = NULL;
	dialogParams.flags |= kAVOpenSaveAllowForeignFileSystems;

	if(!AVAppOpenDialog (&dialogParams,  &fileSys, &pathNames,
									&numPathnames,
									NULL))
		E_RETURN(false);

	ASInt32 errCode;

	for (int i = 0; i <= numPathnames-1; i++)
	{
		if (!strcmp(filetype, "PDF"))
		{
			docs[i] = PDDocOpen( pathNames[i], fileSys, NULL, true );
		}
		else if (!strcmp(filetype, "XML")){
			errCode = ASFileSysOpenFile (fileSys, pathNames[i], ASFILE_READ, &fP[i]);
			if (errCode != 0) ASRaise(errCode);

			ReadXMLFile (&fP[i], data);
		}
		else{
			errCode = ASFileSysOpenFile (fileSys, pathNames[i], ASFILE_READ, &fP[i]);
			if (errCode != 0) ASRaise(errCode);
		}
	}

	*numDocs = numPathnames;

	if (filterRec.filterDescription)
		ASTextDestroy(filterRec.filterDescription);
HANDLER
	char buf[64];
	AVAlertNote(ASGetErrorString(ERRORCODE, buf, 63));
	return false;
END_HANDLER
	return true;
}

/** ReadXMLFile
** ----------------------------------------------------------------------
** Reaf the ASFile into a memeory buffer.
*/
static void ReadXMLFile(ASFile *printFile, char *data)
{
	Int32 nRead;
	char buf[100];
	if (printFile) {
		ASFileSetPos(*printFile, 0);
		strcpy (data, "");
		while (nRead = ASFileRead(*printFile, buf, sizeof(buf)-1)){
			strncat (data, buf, nRead);
		}
	}
	ASFileClose(*printFile);
}

/** Display in IE utilities 
 ---------------------------------------------------------------------
 DisplayInIE.cpp

 - Invokes Internet Explorer by calling Win32 CreateProcess(). To
   display XML with XSL template, IE 5.0 required.

 - Go to http://msdn.microsoft.com/library/default.asp?URL=/library/psdk/xmlsdk
   to get the latest XML or XSL Microsoft Package.
*********************************************************************/

/** DisplayInIE
** ----------------------------------------------------------------------
** Open the constructed XML in Internet Explorer
*/
bool DisplayInIE(ASText file)
{
	HANDLE hfile = NULL;

	TCHAR path[128];
	TCHAR prog[256];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	DWORD nBufferLength = 10000; // size of path buffer
	FILE *input_file;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);

	// Obtain IE application directory
	GetWindowsDirectory(path, 128);
	int len = swprintf_s( 
		prog, 256, 
		L"%s\\%s %s", 
		path, L"explorer.exe",
		ASTextGetUnicode(file));

	// Create a child process
	if(!CreateProcess(NULL, prog, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		AVAlertNote("Can't Launch editor!");
		return false;
	}

	// Synchronize parent-child processes
	if(WaitForSingleObject(pi.hProcess, INFINITE)==0xffffffff)
	{
		AVAlertNote("Synchronization Error with Editor!");
		return false;
	}

	if((input_file = _wfopen((TCHAR *)ASTextGetUnicode(file), L"r, ccs=UNICODE"))== NULL)
	{
		return false;
	}
	fclose (input_file);
	TerminateProcess(pi.hProcess, 1);
	return true;
}

/** WriteXMLFile
** ----------------------------------------------------------------------
** Write the ASFile from a memeory buffer.
*/
static void WriteXMLFile(ASFile *printFile, char *data)
{
	Int32 nWrite = ASFileWrite (*printFile, data, strlen (data));
	ASFileClose(*printFile);
}

/** Register snippet */
SNIPRUN_REGISTER(TransformMetadataSnip, 
				 "PD Layer:Metadata:Transform Metadata",
				 "This snippet demonstrates merging standard Adobe Metadata schema \
sections from input documents. It prompts a file selection dialog \
for selecting input documents, then batch-processes the input documents \
by merging their metadata into an XML file. The XML file is then \
transformed into XHTML for final output in a browser.")


#endif //WIN_PLATFORM