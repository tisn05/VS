/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1999-2008 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 BatesNumbering.cpp

*********************************************************************/
// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

#include "PDBatesCalls.h"
#include "SmartPDPage.h"

#include <string>
#include <vector>
using namespace std;

const unsigned int MAX_BUFFER_LENGTH = 2048;
typedef vector<string> VString;

/* ExtractBatesNumbers
** ------------------------------------------------------
**
** Extract Bates number(s) from artifact(s).
*/
void ExtractBatesNumbers(const VString& vArtifact, VString& vBates)
{
	for(VString::size_type i = 0; i < vArtifact.size(); i++){
		string strArtifact = vArtifact[i];

		string::size_type nPrevPos = strArtifact.find("<<");
		string::size_type nPos = strArtifact.find(">>", nPrevPos);
		if(nPrevPos == string::npos || nPos == string::npos || nPrevPos > nPos)
			continue;

		string strArtifactContent = strArtifact.substr(nPrevPos + 2, nPos - nPrevPos - 2);

		// Find /Type
		nPos = strArtifactContent.find("/Type", 0);
		// No "Type" keyword, move on to next artifact
		if(nPos == string::npos)
			continue;

		nPrevPos = strArtifactContent.find('/', ++nPos);
		// Can not locate type, move on to next artifact
		if(nPos == string::npos)
			continue;

		nPos = strArtifactContent.find(' ', nPrevPos);
		string strType;
		if(nPos == string::npos) {
			// At the end of the string, no space found
			strType = strArtifactContent.substr(nPrevPos + 1);
		} else {
			// There is a space after the type
			strType = strArtifactContent.substr(nPrevPos, nPos - nPrevPos);
		}

		// Type is not "Pagination", move on to next artifact
		if(strType.compare("/Pagination"))
			continue;

		// Find /Subtype
		nPos = strArtifactContent.find("/Subtype", 0);
		// No "Subtype" keyword, move on to next artifact
		if(nPos == string::npos)
			continue;

		nPrevPos = strArtifactContent.find('/', ++nPos);
		// Can not locate subtype, move on to next artifact
		if(nPos == string::npos)
			continue;

		nPos = strArtifactContent.find(' ', nPrevPos);
		string strSubtype;
		if(nPos == string::npos) {
			// At the end of the string, no space found
			strSubtype = strArtifactContent.substr(nPrevPos + 1);
		} else {
			// There is a space after subtype
			strSubtype = strArtifactContent.substr(nPrevPos, nPos - nPrevPos);
		}

		// Subtype is not "BatesN", move on to next artifact
		if(strSubtype.compare("/BatesN"))
			continue;

		// Retrieve Bates number
		nPos = strArtifactContent.find("/Contents", 0);
		// No "Contents" keyword, move on to next artifact
		if(nPos == string::npos)
			continue;

		nPrevPos = strArtifactContent.find('(', nPos);
		// Can not locate Bates number, move on to next artifact
		if(nPos == string::npos)
			continue;

		nPos = strArtifactContent.find(')', nPrevPos);
		// Something is not right with this Bates number, move on to next artifact
		if(nPos == string::npos) 
			continue;

		// Found Bates number
		vBates.push_back(strArtifactContent.substr(nPrevPos + 1, nPos - nPrevPos - 1));
	}
}

/* ExtractArtifacts
** ------------------------------------------------------
**
** Extract artifact(s) from the page content.
*/
void ExtractArtifacts(const string& strContents, VString& vArtifact)
{
	string::size_type nPos = 0, nPrevPos = 0;

	// Parse through the "Contents" stream
	while((nPos = strContents.find('\n', nPos)) != string::npos) {
		string strTmp = strContents.substr(nPrevPos, nPos - nPrevPos);

		if(strTmp.length() > 9 && !strTmp.substr(0, 9).compare("/Artifact")) {
			vArtifact.push_back(strTmp);
		}

		// Move on to the next line
		nPrevPos = ++nPos;
	}
}

/* DisplayBates
** ------------------------------------------------------
**
** Display Bates number(s).
*/
void DisplayBates(const VString& vBates)
{
	VString::size_type size = vBates.size();
	char strSize[2048];
	sprintf(strSize, "%d", size);

	string strMsg = "The current page of the front document has ";
	strMsg += strSize;
	strMsg += " Bates ";
	strMsg += (size == 1) ? "number" : "numbers";
	strMsg += " added by Adobe Acrobat 9 or above.\n\nThe Bates";
	strMsg += (size == 1) ? " number is: " : " numbers are: ";

	for(VString::size_type i = 0; i < size; i++) {
		if(i > 0)
			strMsg += ", ";
		strMsg += vBates[i];
	}

	strMsg += ".";

	AVAlertNote(strMsg.c_str());
}

/* ExecuteExtractBates
** ------------------------------------------------------
**
** Called when the user selects Advanced > Acroabt SDK >
** Bates Numbering > Extract...
** 
** Extract Bates Numbering on the current page.
*/
ACCB1 void ACCB2 ExecuteExtractBates (void *data)
{	
	AVDoc avDoc = AVAppGetActiveDoc();
	AVPageView pgView = AVDocGetPageView(avDoc);
	PDPage pdPage = AVPageViewGetPage(pgView);

	CosObj cosPage = PDPageGetCosObj(pdPage);

	CosObj cosContents = CosDictGet(cosPage, ASAtomFromString("Contents"));
	if (CosObjGetType(cosContents) == CosStream) {
		ASStm stm = CosStreamOpenStm(cosContents, cosOpenFiltered);
		char strBuffer[MAX_BUFFER_LENGTH+1];
		ASTCount itemsRead = 0;
		string strContents;
		do {
			itemsRead = ASStmRead(strBuffer, sizeof(char), MAX_BUFFER_LENGTH, stm);
			strBuffer[itemsRead] = 0; // putting the string terminator at the end of the buffer
			strContents += strBuffer;
		} while (itemsRead == MAX_BUFFER_LENGTH);

		ASStmClose(stm);

		VString vArtifact;
		ExtractArtifacts(strContents, vArtifact);
		if(vArtifact.size() > 0) {
			VString vBates;
			ExtractBatesNumbers(vArtifact, vBates);
			
			if(vBates.size() > 0) {
				DisplayBates(vBates);
				return;
			}
		}
	}

	AVAlertNote("There is no Bates number added by Adobe Acrobat 9 and above on the current page of the front document!");	
}

/* ExecuteAddBates
** ------------------------------------------------------
**
** Called when the user selects Advanced > Acroabt SDK >
** Bates Numbering > Add...
**
** Add Bates Numbering to every page of the front document
** with pre-defined configurations.
*/
ACCB1 void ACCB2 ExecuteAddBates (void *data)
{
	AVDoc avDoc = AVAppGetActiveDoc();
	PDDoc pdDoc = AVDocGetPDDoc(avDoc);

	PDDocBatesNumberingParamsRec pdDocBatesNumberingParamsRec;
	memset(&pdDocBatesNumberingParamsRec, 0, sizeof(pdDocBatesNumberingParamsRec));

	pdDocBatesNumberingParamsRec.size = sizeof(PDDocBatesNumberingParamsRec);
	pdDocBatesNumberingParamsRec.start = 1;
	pdDocBatesNumberingParamsRec.nDigits = 6;
	ASText prefix = ASTextFromScriptText("acro", kASRomanScript);
	ASText suffix = ASTextFromScriptText("sdk", kASRomanScript);
	pdDocBatesNumberingParamsRec.prefix = prefix;
	pdDocBatesNumberingParamsRec.suffix = suffix;

	PDDocLayoutParamsRec pdDocLayoutParamsRec;
	memset(&pdDocLayoutParamsRec, 0, sizeof(pdDocLayoutParamsRec));

	pdDocLayoutParamsRec.size = sizeof(PDDocLayoutParamsRec);
	// Add Bates number to every page of the document
	PDPageRange pdPageRange = {0, PDDocGetNumPages(pdDoc) - 1, PDAllPages};
	pdDocLayoutParamsRec.targetRange = pdPageRange;
	ASFixedRect margins = {FloatToASFixed(36), FloatToASFixed(36), FloatToASFixed(36), FloatToASFixed(36)};
	pdDocLayoutParamsRec.margins = margins;
	pdDocLayoutParamsRec.horizAlign = kPDHorizLeft;
	pdDocLayoutParamsRec.vertAlign = kPDVertTop;
	PDColorValueRec pdColorValue = {PDDeviceRGB, fixedZero, fixedZero, fixedZero};
	pdDocLayoutParamsRec.color = pdColorValue;
	pdDocLayoutParamsRec.fontSize = 10.0;
	pdDocLayoutParamsRec.fontName = ASAtomFromString("Courier");
	pdDocLayoutParamsRec.underline = false;

DURING
	ASBool bRet = PDDocAddBatesNumbering(pdDoc, &pdDocBatesNumberingParamsRec, pdDocLayoutParamsRec);
	// Advertise that we changed the contents so the viewer redraws the
	// page and other clients can re-acquire the page contents if needed.
	ASInt32 numPages = PDDocGetNumPages(pdDoc);
	CSmartPDPage pdPage;
	for(ASInt32 i = 0; i < numPages; i++) {
		pdPage.AcquirePage(pdDoc, i);
		PDPageNotifyContentsDidChange(pdPage);
	}
	if(bRet)
		AVAlertNote("Successfully added Bates Numbering to the front document!");
	else
		AVAlertNote("Failed to add Bates Numbering to the front document!");
HANDLER			
	// exception handling
	char strErrMsg[256];
	memset(strErrMsg, 0, 256);
	ASGetErrorString (ASGetExceptionErrorCode(), strErrMsg, 256);
	AVAlertNote (strErrMsg);
END_HANDLER

	ASTextDestroy(prefix);
	ASTextDestroy(suffix);
}

/* ExecuteRemoveBates
** ------------------------------------------------------
**
** Called when the user selects Advanced > Acroabt SDK >
** Bates Numbering > Add...
**
** Remove Bates Numbering from the front document.
*/
ACCB1 void ACCB2 ExecuteRemoveBates (void *data)
{
	AVDoc avDoc = AVAppGetActiveDoc();
	PDDoc pdDoc = AVDocGetPDDoc(avDoc);

DURING
	ASBool bRet = PDDocRemoveBatesNumbering(pdDoc);
	if(bRet)
		AVAlertNote("Successfully removed Bates Numbering from the front document!");
	else
		AVAlertNote("Failed to remove Bates Numbering from the front document!");
HANDLER			
	// exception handling
	char strErrMsg[256];
	memset(strErrMsg, 0, 256);
	ASGetErrorString (ASGetExceptionErrorCode(), strErrMsg, 256);
	AVAlertNote (strErrMsg);
END_HANDLER
}

