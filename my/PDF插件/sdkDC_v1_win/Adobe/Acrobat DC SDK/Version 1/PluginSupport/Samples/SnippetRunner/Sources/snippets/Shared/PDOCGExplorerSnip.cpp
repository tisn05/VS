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

 PDOCGExplorerSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/* prototype for the actual callback function */
static ACCB1 ASBool ACCB2 OCGExplorerCallback(PDOCG theOCG, void * clientData);


/** registers the optional content group callback proc with the application. The callback 
	displays information about each OCG (name, visibility state - relative 
	to the front document's context, intent and usage). 

	@testfile OCGSample1.pdf
	@requires pdf with OCGs
	@see PDDocGetOCContext
	@see PDOCGGetName
	@see PDDocEnumOCGs
	@see PDOCGEnumProc
	@see PDOCGGetCurrentState
	@see PDOCGHasUsageInfo
	@see PDOCGGetUsageEntry
	@see PDOCGGetIntent
	@see PDOCGGetLocked
*/
void PDOCGExplorerSnip(){
	CDocument document;
	PDDoc theDoc = (PDDoc)document;
	if (theDoc!=NULL){
		// ennumerate the OCGs
		PDOCGEnumProc cbOCGs = ASCallbackCreateProto(PDOCGEnumProc, OCGExplorerCallback);
		PDDocEnumOCGs(theDoc, cbOCGs, NULL);
		ASCallbackDestroy( (void*)cbOCGs );
	}
}

SNIPRUN_REGISTER(PDOCGExplorerSnip, "PD Layer:Optional Content:Explore Optional Content",
				 "dumps diagnostinc information around optional content groups within the \
document")

/* called once for each OCG (in this case in the document, although it could also be 
   page based) */
ACCB1 ASBool ACCB2 OCGExplorerCallback(PDOCG theOCG, void *)
{
	// make sure the document is still valid
	CDocument document;
	PDDoc frontDoc = (PDDoc)document;
	if (frontDoc==NULL){
		return false;
	}
	
	// get the name
	ASText theName = PDOCGGetName(theOCG);
	const char* name = ASTextGetScriptText(theName, kASRomanScript);
	char buff[600];

	// get the OCGs current visibility state
	PDOCContext pdContext = PDDocGetOCContext(frontDoc); 
	char * state = "OFF";
	if(ASBoolToBool(PDOCGGetCurrentState(theOCG,pdContext)) == true){
		state = "ON";
	}

	// retrieve any usage information
	char myBuff [500] = "";
#define NUM_USAGE_NAMES  8
#define MAX_ENTRY_NAMES  3
	const char* usageNames[NUM_USAGE_NAMES][MAX_ENTRY_NAMES] = 
		{
			{"CreatorInfo", "Creator", "Subtype"},
			{"Language", "Lang", "Preferred"},
			{"Export", "ExportState", ""},
			{"Zoom", "min", "max"},
			{"Print", "Subtype", "PrintState"},
			{"View", "ViewState", ""},
			{"User", "Type", "Name"},
			{"PageElement" "Subtype", ""}
		};
		
	//check for CreatorInfo Language Export Zoom Print View User PageElement
	if (PDOCGHasUsageInfo(theOCG)){
		for(int i = 0; i<NUM_USAGE_NAMES; i++)
		{
			ASAtom usageEntry = ASAtomFromString(usageNames[i][0]);
			CosObj usageValue = PDOCGGetUsageEntry(theOCG,usageEntry);
			if(CosObjGetType(usageValue) != CosNull)
			{
				strcat(myBuff,"\n");
				strcat(myBuff,usageNames[i][0]);
				
				for(int subEntry = 1; subEntry < MAX_ENTRY_NAMES; subEntry++)
				{
					if(usageNames[i][subEntry][0] != '\0')
					{
						CosObj cosValue = CosDictGetKeyString(usageValue, usageNames[i][subEntry]);
						int type = CosObjGetType(cosValue);
						if(type != CosNull)
						{
							strcat(myBuff,"\n   ");
							strcat(myBuff,usageNames[i][subEntry]);
							strcat(myBuff," = ");
						
							if(type == CosName)
								strcat(myBuff,ASAtomGetString(CosNameValue(cosValue)));
							else if(type == CosString)
							{
								ASInt32 strSize;
								char* strValue = CosStringValue(cosValue, &strSize);
								strncat(myBuff, strValue, strSize);
							}
							else	
							{
								strcat(myBuff,"unexpected object type");
							}
						}
						
						
						
					}
				}
				
			}		
		}
	}
	else 
	{
		strcpy(myBuff,"No associated usage");
	}

	// get the intent associated with the OCG
	const char * intentBuff = "";
	ASAtom *intent = PDOCGGetIntent(theOCG);
	intentBuff = ASAtomGetString(*intent);

	/* get the locked state of the OCG in the built-in default optional-content configuration 
       for the document */
	PDOCConfig defaultDocConfig = PDDocGetOCConfig(frontDoc);
	char * whetherLocked = "is not locked";
		if(ASBoolToBool(PDOCGGetLocked(theOCG, defaultDocConfig)) == true) {
			whetherLocked = "is locked";
		}
	
	// format and display the information
	sprintf(buff,"Name is \"%s\" \nState is \"%s\"\n",name,state);
	Console::displayString(buff);
	sprintf(buff,"Usage Entries: %s \nIntent is \"%s\"\n",myBuff,intentBuff);
	Console::displayString(buff);
	sprintf(buff,"It %s\n\n", whetherLocked);
	Console::displayString(buff);
	ASTextDestroy(theName);
	return true;
}


