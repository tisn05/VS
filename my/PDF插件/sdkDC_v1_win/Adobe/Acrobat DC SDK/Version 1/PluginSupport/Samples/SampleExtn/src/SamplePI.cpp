/*************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2011 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains
* the property of Adobe Systems Incorporated and its suppliers,
* if any.  The intellectual and technical concepts contained
* herein are proprietary to Adobe Systems Incorporated and its
* suppliers and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe Systems Incorporated.
**************************************************************************/
/** 
\file SamplePI.cpp

  - This file implements the functionality of the SamplePI.
*********************************************************************/


// Acrobat Headers.
#include "PIHeaders.h"

//SandboxSDK Header
#include "SandboxPIClientDefs.h"
//Header for broker APIs implemented by this pug-in.
#include "SamplePICrossCalls.h"

/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/
// This plug-in's name, you should specify your own unique name here.
const char* MyPluginExtensionName = "ADBE:SampleExtn";

/* A convenient function to add a menu item for your plugin.
*/
ACCB1 ASBool ACCB2 AddPluginMenuItems();

/*-------------------------------------------------------
	Functions
-------------------------------------------------------*/

/**		SamplePI project is a Reader plugin sample with the minimum code 
	to provide an environment for plugin developers to get started quickly
	on implementing Sandbox Reader Extensibility for their plugin.
	
	This implements the functionality when the "Display Message In Broker" menu item is clicked.
	
	The menu item when clicked will do the following to demonstrate broker extensibility at work:
	1. It will construct a message with some simple information about the plugin.
	2. It will make a call in sandboxpisdk to do the setup for IPC with SamplePIBroker process.
	3. If 2 succeeds, it will make a cross call to SamplePIBroker process which will display 
	the message constructed in step 1. in a OK/CANCEL message box.
	4. The cross call returns another message back stating whether OK/CANCEL was clicked by the user.
	5. The plugin displays a message box reporting the user's action.
	
	Users can follow these coding steps to implement IPC between their own Plug-in and Plug-in Broker.

	DisplayMessageInBrokerCommand is the function to be called when executing the "Display Message In Broker" menu.
	This is the entry point for user's code, just add your code inside.
*/ 
ACCB1 void ACCB2 DisplayMessageInBrokerCommand(void *clientData)
{
	// get this plugin's name for display
	ASAtom NameAtom = ASExtensionGetRegisteredName (gExtensionID);
	const char * name = ASAtomGetString(NameAtom);

	//construct a message with some simple information about the plugin
	char str[256];
	int j = 0;
	j = sprintf_s(str, 256, "This message is displayed by the Broker.\n\n");
	j += sprintf_s(str + j, 256 - j, "Press OK or CANCEL Button.\n\nYour action will be reported back to the Plugin.\n");

	//Setup IPC with SamplePIBroker process.
	bool sampleipcisset = sandboxpisdk::InitSandboxPIBrokerIPCChannel(L"SamplePIBroker.exe");
	if(sampleipcisset)
	{
		//Make a cross call to SamplePIBroker process which will display the message and return the user action.
		std::string outString;
		AVAppBeginModal(NULL);
		if(sampleipc_remote::SamplePIBrokerDisplayMessage(WinAppGetModalParent(AVAppGetActiveDoc()), str, outString))
		{
			//Display a message box from this plugin reporting the user's action.
			j = sprintf_s(str, 256, "This message is displayed by the Plugin.\n\n");
			j += sprintf_s(str + j, 256 - j, "Response from Broker: %s.\n\n", outString.c_str());
			MessageBoxA(WinAppGetModalParent(AVAppGetActiveDoc()), str, "SamplePI Plugin", MB_OK);
		}
		AVAppEndModal();
	}

	return;
}

/**		SamplePI project is a Reader plugin sample with the minimum code 
	to provide an environment for plugin developers to get started quickly
	on implementing Sandbox Reader Extensibility for their plugin.
	
	This implements the functionality when the "Launch Calc.exe" menu item is clicked.
	
	The menu item when clicked will do the following to demonstrate broker extensibility at work:
	1. It will make a call in sandboxpisdk to do the setup for IPC with SamplePIBroker process.
	2. If 2 succeeds, it will make a cross call to SamplePIBroker process which will display 
	the message box from broker to get user consent before launching calc.exe.
	
	Users can follow these coding steps to implement IPC between their own Plug-in and Plug-in Broker.

	DoSomeNastyStuffCommand is the function to be called when executing the "Launch Calc.exe" menu.
	This is the entry point for user's code, just add your code inside.
*/ 
ACCB1 void ACCB2 DoSomeNastyStuffCommand(void *clientData)
{
	//Setup IPC with SamplePIBroker process.
	bool sampleipcisset = sandboxpisdk::InitSandboxPIBrokerIPCChannel(L"SamplePIBroker.exe");
	if(sampleipcisset)
	{
		//Make a cross call to SamplePIBroker process to launch calc.exe
		sampleipc_remote::SamplePIBrokerDoSomeNastyStuff(); //will launch calc.exe
	}
	return;
}

/* MyPluginIsEnabled
** ------------------------------------------------------
** Function to control if a menu item should be enabled.
** Return true to enable it, false not to enable it.
*/
ACCB1 ASBool ACCB2 MyPluginIsEnabled(void *clientData)
{
	// always enabled.
	return true;
}

