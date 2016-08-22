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

 DSEngine.h

*********************************************************************/

#ifndef DSENGINE_H
#define DSENGINE_H

#include "PubSecHFT.h"
class DSUser;

/*************************************************************************
 * class DSEngine
 ************************************************************************/

class DSEngine
{

public:

	/** Constructor */
	DSEngine(bool bUI = true);
	/** Destructor */
	~DSEngine() {};

	/** Returns a new ASText object containing the value of a particular property (used by escript)
	Gets the indexth string if from a list (else need not specify index)
	*/
	ASText getTextProperty( const char* szPropertyName, const ASInt32 index = 0 );

	/** Returns the value of a particular property (used by escript)
	*/
	bool getBoolProperty( const char* szPropertyName, const bool defaultValue );

	/** Returns the value of a particular property (used by escript)
	*/
	ASInt32 getInt32Property( const char* szPropertyName, const ASInt32 defaultValue );

	/** Perform the operation defined by the cab (usually an escript operation)
	*/
	DSRetCode performOperation( PSPerformOpType type, const ASCab cab, ASBool bUI );

	/* Acquiring and releasing resources */

	/** Acquire any resources that are required before performing operation (eg. logon if necessary)
	    Returns true if session acquisition was done (caller keeps track to know whether to release)
	    If cabParams is specified then this will provide information about the acquisition. The cab is owned by the caller.
	*/
	bool sessionAcquire( const PDDoc& pdDoc, const PSSessionOpType operation, ASText opText, ASCab cabParams, bool bUI );

	/** Acquires any resources that are required for signing (eg. logon if necessary)
	*/
	bool sessionRelease( const PSSessionOpType operation );

	/** Returns true if resources and information are acquired to do operation. 
	    Resources are acquired using dialog
	*/
	bool sessionReady( const PSSessionOpType operation );

	/* Signing */

	/** Properties that the handler specifies that will be included in authenticated/signed attributes
	*/
	void sigGetSigProperties( PSSigSigPropParams params );

	/** If inESParams then try to authenticate user using these params,
	    else bring up UI, if necessary, to authenticate
	    Set bPrompt to true if UI is allowed to come up.
	*/
	bool sigAuthenticate( const PDDoc& pdDoc, ASCab inESParams, bool bUI );

	/** Sign the digest and return signature value, which becomes property of caller.
	    If pOutSigValueData is NULL then return expected size of signature value (only called for PKCS#7)
	*/
	void sigGetSigValue( PSSigGetSigValueParams inOutParams );

	/* Verifying */

	/** Validate a PDDoc signature. State is set in dsSig.
	*/
	void sigValidate( PSSigValidateParams params );

	/* Contacts */

	bool exportData( PSExportDataType dataType, ASCab outCab, bool bUI );

	/* Encryption */

	ASBool openCMSEnvelope (ASUns8* inCMSEnvelope, ASUns32 inCMSEnvelopeSize, ASUns8** pOutData, ASUns32* pOutDataSize, ASBool inUIAllowed);
	void getImplicitRecipients (ASCab outCertList, ASBool inUIAllowed );

	/*
	** DSEngine specific methods
	*/

	/** Set the exception text */
	void putErrorText( ASText text ) { fErrorText = ASTextDup( text ); }

private:

	/**
	This routine aquires a new user using UI and stuffs it into fUser.  Prompts selection of 
	an existing profile and a login or creation of a new user.  inReason is an optional hint 
	as why logging in / new profile creation is needed.
	Returns true if a user was acquired.
	*/
	bool aquireUserWithUI( const ASText& inReason );

	//------------------------------------------------------------------
	// Data members

	/** Contains exception text */
	ASText fErrorText;

	ASCab fCabValidate;

	/** The current user logged in to the engine */
	DSUser *fUser;

	/**
	indicates whether this engine instance is the UI engine (true) or an escript engine (false)
	*/
	bool fUI;
};

/**
 Handy wrapper for acquiring resources so that session is automatically
 released by destructor.
*/
class DSEngineSession {

public:
	DSEngineSession( const PDDoc& pdDoc, DSEngine *pEngine, const PSSessionOpType operation, ASText opText = NULL, bool bUI = true );
	DSEngineSession( DSEngine *pEngine, const PSSessionOpType operation, ASText opText = NULL, bool bUI = true );
	~DSEngineSession();

private:
	DSEngine *fpEngine;
	bool fbSessionAcquired;
	PSSessionOpType fOperation;
};

#endif // DSENGINE_H
