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

 ASGetConfigurationSnip.cpp

*********************************************************************/

#include "SnippetRunner.h"
#include "SnippetRunnerUtils.h"

/*-------------------------------------------------------
	Implementation
-------------------------------------------------------*/

/**  This plugin checks for the current configuration of Acrobat, and whether you
	can edit PDFs using this product, and displays an alert with the the information.

	The return values for the configuration check is one of:
		"Reader"
		"Exchange"
		"Exchange-Pro"
		"Adobe PDF Library"

	The CanEdit check returns true or false.

	@testfile none
	@see ASGetConfiguration
*/
void ASGetConfigurationSnip()
{
	char* prodName =  (char*)ASGetConfiguration(ASAtomFromString("Product"));
	ASBool canEdit = (ASGetConfiguration(ASAtomFromString("CanEdit")) != NULL);
	if(prodName != UNDEFINED_CONFIGURATION_SELECTOR)
	{
		char buff[100];
		if(canEdit)
			snprintf(buff,sizeof(buff), "This product is %s. You can edit PDFs using this product.", prodName);
		else
			snprintf(buff,sizeof(buff), "This product is %s. You cannot edit PDFs using this product.", prodName);
				
		Console::displayString(buff);
	}
	else
		AVAlertNote("Undefined Configuration!");

}

SNIPRUN_REGISTER(ASGetConfigurationSnip, "AS Layer:Get Configuration", "Gets the current Acrobat configuration.")

