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

 FormCalculationsSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"


ASCallback gClickCallback = NULL;
ASCallback gKeyCallback = NULL;
double gCalcValue;
AVKeyCode gCurrentOperation;

ACCB1 ASBool ACCB2 FormCalculationClickProc (AVPageView pageView,
AVDevCoord xHit, AVDevCoord yHit, AVFlagBits16 flags, AVTCount clickNo,
void* data);

ACCB1 ASBool ACCB2 FormCalculationKeyDownProc (AVPageView pageView,
							AVKeyCode keyCode, AVFlagBits16 flags, void* data);



void EnableFormCalculations()
{
	if(gClickCallback == NULL)
		gClickCallback =
			(void*)ASCallbackCreateProto(AVPageViewClickProc, FormCalculationClickProc);
	if(gKeyCallback == NULL)
		gKeyCallback =
			(void*)ASCallbackCreateProto(AVPageViewKeyDownProc, FormCalculationKeyDownProc);
	gCalcValue = 0;
	gCurrentOperation = '+';
	AVAlertNote("Total is currently 0");
	AVAppRegisterForPageViewClicks((AVPageViewClickProc)gClickCallback, NULL);
	AVAppRegisterForPageViewKeyDown((AVPageViewKeyDownProc)gKeyCallback, NULL);
	
}

void DisableFormCalculations()
{
	if(gClickCallback)
	{
		AVAppUnregisterForPageViewClicks((AVPageViewClickProc)gClickCallback);
		ASCallbackDestroy(gClickCallback);
		gClickCallback = NULL;
	}

	if(gKeyCallback)
	{
		AVAppUnregisterForPageViewKeyDown((AVPageViewKeyDownProc)gKeyCallback);
		ASCallbackDestroy(gKeyCallback);
		gKeyCallback = NULL;
	}
}

/** Turning on form calculations allows you to use simple math operations
	to combine different form fields. An example of use would be to click
	on a form field, press the '+' key, click on a second form field, press
	the '=', and then click on a third field. This will cause the third field
	to contain the sum of the first two. Feedback is provided in the output window
	which is opened when the snippet is executed.

	The snippet implements two callbacks, one to handle page view mouse key clicks and the other
	to handle key clicks. Both these callbacks are operational when the snippet is executing.

	The snippet detects if the mouse has been clicked over a text form field. If so, the value of the form
	field is used as part of the running calculation (no precedence of operators in this simple calculator).

	If the last key pressed was the '=', the current running total is written to the selected text form field.

	Clicking outside a form field will reset the calculator. Non-calculator keys are ignored.

	This snippet demonstrates how form field values can be read or updated via C based APIs.

	@see AVAppRegisterForPageViewClicks
	@see AVAppRegisterForPageViewKeyDown
	@see AVAppUnregisterForPageViewClicks
	@see AVAppUnregisterForPageViewKeyDown
	@see ASCallbackCreateProto
	@see ASCallbackDestroy
	@see AVPageViewIsAnnotAtPoint
	@see PDAnnotGetSubtype
	@see PDAnnotGetCosObj
	@see CosDictGet
	@see AFPDFieldFromCosObj
	@see AFPDFieldGetValue
	@see CosStringValueSafe
	@see AVPageViewGetPage
	@see PDPageGetDoc
	@see AVPageViewIsFocusAnnot
	@see AVPageViewClearFocusAnnot
	@see CosNewString
	@see AFPDFieldSetValue
	@see AVPageViewSetFocusAnnot
	@see CosCopyNameStringValue
*/
void FormCalculationsSnip()
{
	//returns previous state, so toggle first and ask questions later
	ASBool currentState = SnippetRunnerUtils::toggleSnippetCheck();

	if(currentState == false){
		EnableFormCalculations();
		string strMsg =	 "Click on the form field you want to include in the calculation, followed by the ";
		strMsg += "arithmetic operator (*,+,-,/) then the next form field to be included in the calculation. ";
		strMsg += "When you have finished forming the arithmetic expression, click on the = key before selecting ";
		strMsg += "the form field you want the result to be displayed to. Log:\t0"; 
		AVAlertNote(strMsg.c_str());
	}
	else
		DisableFormCalculations();
}

/* FormCalculatorClickProc
** ------------------------------------------------------
**
** Handles page view clicks when in calc mode
**
** return true if gCalcMode && click is on form field
** else false
*/
ACCB1 ASBool ACCB2 FormCalculationClickProc (AVPageView pageView,
AVDevCoord xHit, AVDevCoord yHit, AVFlagBits16 flags, AVTCount clickNo,
void* data)
{
	PDAnnot hitAnnot;
	if(AVPageViewIsAnnotAtPoint ( pageView, xHit, yHit, &hitAnnot))
	{
		if(PDAnnotGetSubtype (hitAnnot) == ASAtomFromString("Widget"))
		{
			CosObj obj = PDAnnotGetCosObj(hitAnnot);
			CosObj ftKey = CosDictGetKeyString (obj, "FT");

			if (strcmp("Tx", CosCopyNameStringValue(ftKey, NULL))==0) 
			{
				//found one!
				char buffer[100];
				ASInt32 nBytes = 0;
				double newValue = 0;
				PDField pdField = AFPDFieldFromCosObj (obj);
		
				//get the value out if we need to use it
				if(gCurrentOperation != '=')
				{
					CosObj valueObj = AFPDFieldGetValue (pdField);
					
					CosStringValueSafe(valueObj, buffer, 99, &nBytes);
					//make 'C' style string
					buffer[nBytes] = '\0';
					newValue = atof(buffer);
				}
						
				PDPage page = AVPageViewGetPage (pageView);
				PDDoc pdDoc = PDPageGetDoc (page);
				
				switch(gCurrentOperation)
				{
				case '+':
						gCalcValue += newValue;
					break;
				case '-':
						gCalcValue -= newValue;
					break;
				case '*':
						gCalcValue *= newValue;
					break;
				case '/':
						if(newValue != 0)
							gCalcValue = gCalcValue/newValue;
						else
							AVAlertNote("Trying to divide by zero!");
					break;
				case '=':
					
					//need to remove focus before setting value
					ASBool isSelected = false;
					if(AVPageViewIsFocusAnnot (pageView, hitAnnot))
					{
						isSelected = true;

						//this isn't guaranteed, so I might pop up an error message, usually
						AVPageViewClearFocusAnnot (pageView);
					}
					sprintf(buffer, "%f", gCalcValue);
					CosObj val = CosNewString (PDDocGetCosDoc (pdDoc), false, buffer, strlen(buffer));
					AFPDFieldSetValue (pdField, val);
					if(isSelected)
						AVPageViewSetFocusAnnot(pageView, hitAnnot, NULL);
					break;


				}
				if (gCurrentOperation == '='){
					char buffer[200];
					sprintf(buffer,"Total now %f - operator reset to '+'",gCalcValue);
					AVAlertNote(buffer);
					gCurrentOperation = '+';
				}
				else {
					//print current number if still doing arithmetic
					char buff[50];
					if(gCurrentOperation != '=')
						sprintf(buff, "%c\t%f", gCurrentOperation, newValue);
					else
						sprintf(buff, "%c\t%f", gCurrentOperation, gCalcValue);
						
					AVAlertNote(buff); 
				}
				return true;
			}
		}
	}
	// Some other input has occured, reset the current operaition
	gCurrentOperation = '+';
	gCalcValue = 0;
	AVAlertNote("Total reset to 0, operator now '+'"); 
	return false;
}


ACCB1 ASBool ACCB2 FormCalculationKeyDownProc (AVPageView pageView,
							AVKeyCode keyCode, AVFlagBits16 flags, void* data)
{
	switch(keyCode)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '=':
		gCurrentOperation = keyCode;
		return true;
		break;
	default:
		return false;
	}

}

SNIPRUN_REGISTER(FormCalculationsSnip, "AV Layer:Forms:Toggle Calculations", " \
Turning on form calculations allows you to use simple math operations \
to combine different form fields. An example of use would be to click \
on a form field, press the '+' key, click on a second form field, press \
the '=', and then click on a third field. This will cause the third field \
to contain the sum of the first two. Feedback is provided in the output window \
which is opened when the snippet is executed.")


