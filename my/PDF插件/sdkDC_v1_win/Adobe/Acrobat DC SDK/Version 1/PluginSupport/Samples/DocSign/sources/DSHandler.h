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

 DSHandler.h

*********************************************************************/

#ifndef DSHANDLER_H
#define DSHANDLER_H

#include "PubSecHFT.h"

#define DOCSIGN_CURRENT_VERSION	1
#define DOCSIGN_HANDLER_NAME	"Adobe.DocSign"

#define Type_K  ASAtomFromString("Type")
#define XObject_K  ASAtomFromString("XObject")
#define Subtype_K  ASAtomFromString("Subtype")
#define Form_K ASAtomFromString("Form")
#define FormType_K ASAtomFromString("FormType")
#define Matrix_K  ASAtomFromString("Matrix")
#define BBox_K ASAtomFromString("BBox")

#define DocSignFixedMatrixSetIdentity(fm) \
	DocSignFixedMatrixSet(fm, fixedOne, fixedZero, fixedZero, fixedOne, fixedZero, fixedZero)

class DSEngine;

/**
 totally static class to represent a PubSec handler
*/

class DSHandler {

public:


	/** Constructor	*/
	DSHandler() {};
	/** Destructor */
	~DSHandler() {};

	/*
	** Register/Unregister
	*/

	/** Register a handler with PubSecHFT.
	*/
	static bool Register();

	/** Unregisters handler with PubSecHFT (must call at end)
	*/
	static void Unregister();

	/** Return the one and only handler that was registered with PubSec
	*/
	static PubSecHandler Handler() {
		return fbHandlerIsRegistered ? &fHandlerRec : NULL;
	}

	/** Returns the engine that is associated with the handler.
	    This engine is the UI engine.
	*/
	static DSEngine* Engine() {
		return fbHandlerIsRegistered ? (DSEngine*) fHandlerRec.engine : NULL;
	}

	/*
	** PubSecHandler Procs
	** Descriptions of these routines can be found in PubSecHFT.h
	*/

	/** @see PSNewEngineProc */
	static ACCBPROTO1 PubSecEngine ACCBPROTO2 NewEngine();
	/** @see PSDestroyEngineProc */
	static ACCBPROTO1 void ACCBPROTO2 DestroyEngine( PubSecEngine engine );

	/** @see PSGetBoolPropertyProc */
	static ACCBPROTO1 ASBool ACCBPROTO2 GetBoolProperty( PubSecEngine engine, const char* szPropertyName, const ASBool defaultValue );
	/** @see PSGetAtomPropertyProc */
	static ACCBPROTO1 ASAtom ACCBPROTO2 GetAtomProperty( PubSecEngine engine, const char* szPropertyName );
	/** @see PSGetInt32PropertyProc */
	static ACCBPROTO1 ASInt32 ACCBPROTO2 GetInt32Property( PubSecEngine engine, const char* szPropertyName, const ASInt32 defaultValue );
	/** @see PSGetTextPropertyProc */
	static ACCBPROTO1 ASText ACCBPROTO2 GetTextProperty( PubSecEngine engine, const char* szPropertyName, const ASInt32 index );

	/** @see PSSessionAcquireProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 SessionAcquire( PubSecEngine engine, PDDoc pdDoc, PSSessionOpType opType, ASText opText, ASCab cabParams, ASBool bUI );
	/** @see PSSessionReleaseProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 SessionRelease( PubSecEngine engine, PSSessionOpType opType );
	/** @see PSSessionReadyProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 SessionReady( PubSecEngine engine, PSSessionOpType opType );
	/** @see PSPerformOperationProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 PerformOperation( PubSecEngine engine, PSPerformOpType type, const ASCab cabParams, ASBool bUI );

	/** @see PSSigGetSigPropertiesProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 SigGetSigProperties( PubSecEngine engine, PSSigSigPropParams params );
	/** @see PSSigAuthenticateProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 SigAuthenticate( PubSecEngine engine, const PDDoc pdDoc, ASCab inESParams, ASBool bUI );
	/** @see PSSigGetSigValueProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 SigGetSigValue( PubSecEngine engine, PSSigGetSigValueParams inOutParams );
	
	/** @see PSSigCreateAPNXObjProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 SigCreateAPNXObj( PubSecEngine engine, CosObj *pOutXObj, 
							PSSigPDDocParams docParams, PSAPSigType sigType );
	/** @see PSSigValidateProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 SigValidate( PubSecEngine engine, PSSigValidateParams params );
	/** @see PSSigPropDialogProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 SigPropDialog( PubSecEngine engine, 
							PSSigValidateDialogParams valParams );

	/** @see PSGetLogoProc */
	static ACCBPROTO1 void ACCBPROTO2 GetLogo( ASAtom label, const char* *pcLogo, const ASFixedRect* *pRect );

	/** @see PSExportDataProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 ExportData( PubSecEngine engine, PSExportDataType dataType, ASCab outCab, ASBool bUI );

	/** @see PSOpenCMSEnvelopeProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 openCMSEnvelope ( PubSecEngine engine, ASUns8* inCMSEnvelope, ASUns32 inCMSEnvelopeSize, ASUns8** pOutData, ASUns32* pOutDataSize, ASBool inUIAllowed );
	/** @see PSGetImplicitRecipientsProc */
	static ACCBPROTO1 DSRetCode ACCBPROTO2 getImplicitRecipients (PubSecEngine engine, ASCab outCertList, ASBool inUIAllowed );

protected:

	 // Fields
	static bool fbHandlerIsInit;		// true if callbacks created
	static bool fbHandlerIsRegistered;	// true if registered with PubSec
	static ASAtom fHandlerName;
	/** The structure that gets registered with PubSec
	    @see PubSecHandler
	*/
	static PubSecHandlerRec fHandlerRec;

private:
	DSHandler( const DSHandler& rhs );
	DSHandler & operator = ( const DSHandler &rhs );
	static CosObj CreateN2XObject( CosDoc cosDoc, const CosObj sigField, const CosObj sigAnnot, const ASFixedRect* const pBBoxRec );
	static void DocSignFixedRect(ASFixedRect *fr, ASFixed fxLeft, ASFixed fxBottom, ASFixed fxRight, ASFixed fxTop);
	static CosObj DocSignStreamToXObject(CosObj strm, ASFixedMatrixP matrix, const ASFixedRect* const bbox, ASBool bUseOldObj);
	static void DocSignCosPutRect(CosDoc cd, CosObj coParent, ASAtom asaKey, const ASFixedRect *fr);
	static void DocSignCosPutMatrix(CosDoc cd, CosObj coParent, ASAtom asaKey, const ASFixedMatrix *m);
	static void DocSignCosGetMatrix(CosObj coParent, ASAtom asaKey, ASFixedMatrix *m);
	static void DocSignFixedMatrixSet(ASFixedMatrix *fm, ASFixed a, ASFixed b, ASFixed c, ASFixed d, ASFixed h, ASFixed v);
};	// class DSHandler

#endif /* DSHANDLER_H */
