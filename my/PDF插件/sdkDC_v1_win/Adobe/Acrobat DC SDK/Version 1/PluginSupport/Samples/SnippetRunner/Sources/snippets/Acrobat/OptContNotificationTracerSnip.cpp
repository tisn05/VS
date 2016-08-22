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

 OptContNotificationTracerSnip.cpp

*********************************************************************/

#include "SnippetRunnerUtils.h"
#include "SnippetRunner.h"

/* prototype for the functions this snippet depends on */
ACCB1 void ACCB2 optContDidChangeProc(PDDoc pdDoc, PDDocOCChangeType whatHappened, void * object, void * data);
ACCB1 void ACCB2 optContWillChangeProc(PDDoc pdDoc, PDDocOCChangeType whatWillHappen, void * object, void * data);

ACCB1 void ACCB2 optContextWillChangeProc(PDOCContext ocContext, PDOCContextChangeType whatWillHappen, void * objects, void * data);
ACCB1 void ACCB2 optContextDidChangeProc(PDOCContext ocContext, PDOCContextChangeType whatHappened, void * objects, void * data);

/* functions we call for both will/did happen notifications */
void emitOCChangeType(PDDocOCChangeType whatChange);
void emitOContextChangeType(PDOCContextChangeType whatChange);

static string strOutput;
/** example of how to register for the optional content notifcations.
	This allows us to track when optional content groups and optional content contexts will/did change.
	@testfile none
	@requires changing OCGs to fire notifications
	@see AVAppRegisterNotification
	@see PDDocOCWillChange
	@see PDDocOCDidChange
	@see PDOCContextWillChange
	@see PDOCContextDidChange
*/
void OptContNotificationTracerSnip(void){
	if (SnippetRunnerUtils::toggleSnippetCheck()==false){
		AVAppRegisterNotification(PDDocOCWillChangeNSEL, 0, (void*)optContWillChangeProc, NULL);	
		AVAppRegisterNotification(PDDocOCDidChangeNSEL, 0, (void*)optContDidChangeProc, NULL);	
		AVAppRegisterNotification(PDOCContextWillChangeNSEL, 0, (void*)optContextWillChangeProc, NULL);	
		AVAppRegisterNotification(PDOCContextDidChangeNSEL, 0, (void*)optContextDidChangeProc, NULL);	
		Console::displayString("Registered Optional Content notifications - see AVAlert for output.");
	}
	else {
		AVAppUnregisterNotification(PDDocOCWillChangeNSEL, 0, (void*)optContWillChangeProc, NULL);	
		AVAppUnregisterNotification(PDDocOCDidChangeNSEL, 0, (void*)optContDidChangeProc, NULL);	
		AVAppUnregisterNotification(PDOCContextWillChangeNSEL, 0, (void*)optContextWillChangeProc, NULL);	
		AVAppUnregisterNotification(PDOCContextDidChangeNSEL, 0, (void*)optContextDidChangeProc, NULL);	
		Console::displayString("Un-registered Optional Content notifications.");
	}

}

SNIPRUN_REGISTER(OptContNotificationTracerSnip, "PD Layer:Optional Content:Notification tracer","Testing OCG signals")

ACCB1 void ACCB2 optContWillChangeProc(PDDoc pdDoc, PDDocOCChangeType whatWillHappen, void * object, void * data){
	strOutput = "Optional content about to change:"; 
	emitOCChangeType(whatWillHappen);
}
 
ACCB1 void ACCB2 optContDidChangeProc(PDDoc pdDoc, PDDocOCChangeType whatHappened, void * object, void * data){
	strOutput = "Optional content has changed:"; 
	emitOCChangeType(whatHappened);
}

ACCB1 void ACCB2 optContextDidChangeProc(PDOCContext ocContext, PDOCContextChangeType whatHappened, void * objects, void * data){
	strOutput =  "Optional context has changed:"; 
	emitOContextChangeType(whatHappened);
}
ACCB1 void ACCB2 optContextWillChangeProc(PDOCContext ocContext, PDOCContextChangeType whatWillHappen, void * objects, void * data){
	strOutput =  "Optional context about to change:"; 
	emitOContextChangeType(whatWillHappen);
}

void emitOContextChangeType(PDOCContextChangeType whatChange){
	strOutput += "\n";
	switch (whatChange)
	{
		case kPDOCGState:
			strOutput += "OCG's states changing"; break;
		case kPDOCContextDrawEnumType:
			strOutput += "PDOCContext's PDDrawEnumType changing"; break;
		case kPDOCContextNonOCDrawing:
			strOutput += "PDOCContext's non-OC drawing changing"; break;
		case kPDOCContextIntent:
			strOutput += "PDOCContext's intent changing"; break;
		case kPDOCContextInit:
			strOutput += "PDOCContext being reset using PDOCContextInit"; break;
		default:
			strOutput += "Unknown optional context change type";
	}
	AVAlertNote(strOutput.c_str());
}
 
void emitOCChangeType(PDDocOCChangeType whatChange){
	strOutput += "\n";
	switch (whatChange)
	{
		case kPDOCGCreate:
			strOutput += "OCG created"; break;
		case kPDOCGProperties:
			strOutput += "OCG properties changed"; break;
		case kPDOCGReplace:
			strOutput += "OCG replaced by another"; break;
		case kPDOCGDestroy:
			strOutput += "OCG destroyed"; break;
		case kPDOCMDAttach:
			strOutput += "Content made optional"; break;
		case kPDOCMDRemove:
			strOutput += "Content made non-optional"; break;
		case kPDOCConfigCreate:
			strOutput += "OC Configuration created"; break;
		case kPDOCConfigChange:
			strOutput += "OC configuration changed"; break;
		case kPDOCConfigDestroy:
			strOutput += "OC configuration destroyed"; break;
		case kPDDocRemoveOC:
			strOutput += "OC removed from document"; break;
		default:
			strOutput += "Unknown optional content change type";
	}
	AVAlertNote(strOutput.c_str());
}

