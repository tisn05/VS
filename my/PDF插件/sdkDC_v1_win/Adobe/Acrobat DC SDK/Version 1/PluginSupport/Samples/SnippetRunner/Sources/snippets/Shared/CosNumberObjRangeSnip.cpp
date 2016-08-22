/**********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

*/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"
#include "math.h"

/** 
	This snippet demonstrates:
	the new Cos layer APIs for creating CosFloat/CosInteger64 objects and checking
	whether these number objects are within valid ranges so that appropriate API 
	calls can be used to obtain the numbers represented by the correspoding Cos objects.

	@testfile none
	@see CosNumberIsWithinASFixedRange
	@see CosNumberIsWithinASInt32Range
	@see CosNewFloat
	@see CosFloatValue
	@see CosInteger64Value
	@see CosNewInteger64
*/
void CosNumberObjRangeSnip(){
	PDDoc pdDoc = PDDocCreate();
	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);
	
	// Creates some Cos number objects (CosReal deprecates CosFixed)

	CosObj cosReal = CosNewFixed(cosDoc, true, fixedSevenEighths);
	CosObj cosFloat = CosNewFloat(cosDoc, true, (float)0.314159e6);
	CosObj cosInt64 = CosNewInteger64(cosDoc, true, ASMAXInt64);

DURING
	
	//
	// Checks Cos number ranges and retrieves the numbers accordingly
	// Range of ASFixed numbers: [-32768.0, +32768.0)
	// Range of 32-bit integers: [-2147483648, +2147483647]
	//

	ASBool isReal = CosNumberIsWithinASFixedRange(cosReal);
	ASBool isFloat = (CosNumberIsWithinASFixedRange(cosFloat))?false:true;
	ASBool isInt64 = (CosNumberIsWithinASInt32Range(cosInt64))?false:true;

	ASFixed fixed = fixedZero;
	float floating = 0.0;
	ASInt64 int64 = 0;

	if (isReal)
		fixed = CosFixedValue(cosReal);
	
	if (isFloat)
		floating = CosFloatValue(cosFloat);

	if (isInt64)
		int64 = CosInteger64Value(cosInt64);

	char msg[256];
	memset(msg, 0, sizeof(msg));
#ifdef WIN_PLATFORM
	snprintf(msg,sizeof(msg), "cosReal %6.5f %s within ASFixed range\ncosFloat %7.6E %s within ASFixed range\ncosInt64 0x%I64x %s within 32-bit range\n",
		ASFixedToFloat(fixed), (isReal)?"is":"isn't", floating, (isFloat)?"isn't":"is", int64, (isInt64)?"isn't":"is");
#else
	snprintf(msg,sizeof(msg), "cosReal %6.5f %s within ASFixed range\ncosFloat %7.6E %s within ASFixed range\ncosInt64 0x%llx %s within 32-bit range\n",
		ASFixedToFloat(fixed), (isReal)?"is":"isn't", floating, (isFloat)?"isn't":"is", int64, (isInt64)?"isn't":"is");
#endif
	Console::displayString(msg);
	
	PDDocClose(pdDoc);

HANDLER
	char errmsg[256];
	ASGetErrorString(ERRORCODE, errmsg, 255);
	AVAlertNote(errmsg);
END_HANDLER
}

SNIPRUN_REGISTER(CosNumberObjRangeSnip, "COS Layer:Cos Number Range",
	"This snippet demonstrates the new Cos layer APIs for creating CosFloat/CosInteger64 objects and checking \
whether Cos number objects ranges so appropriate API calls can be used to obtain the Cos numbers.\
The execution of this snippet does *not* require an open document.")
