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

 DSHandler.cpp

 - Main Digital Signature callbacks and related routines.

*********************************************************************/
#include "DocSign.h"
#include "DSHandler.h"
#include "DSEngine.h"
#include "DSSigVal.h"

ASAtom DSHandler::fHandlerName = ASAtomNull;
PubSecHandlerRec DSHandler::fHandlerRec;
bool DSHandler::fbHandlerIsInit = false;
bool DSHandler::fbHandlerIsRegistered = false;

// Precise bbox of Acrobat logo stream stmLogoLogo
extern const ASFixedRect stmLogoBBox;

// Stream data for Acrobat Logo
extern const char* const stmLogoData;

/*************************************************************************
 * Big red X
 ************************************************************************/
static ASFixedRect stmInvalidBBox = { fixedZero,fixedZero,0x00630000,0x00630000 };
static const char* const stmInvalidData = 
	"% DSInvalid\n"
	"q\n"
	"0.1 0 0 0.1 5 0 cm\n"
	"0 J 0 j 4 M []0 d\n"
	"1 i \n"
	"1 G\n"
	"0 g\n"
	"69 181 m\n"
	"92 205 310 447 343 486 c\n"
	"291 542 126 730 99 763 c\n"
	"242 894 l\n"
	"282 842 417 668 468 607 c\n"
	"501 646 668 845 708 895 c\n"
	"843 768 l\n"
	"810 735 604 527 576 492 c\n"
	"614 451 822 221 851 187 c\n"
	"710 52 l\n"
	"677 92 486 330 451 370 c\n"
	"431 347 217 75 197 49 c\n"
	"69 181 l\n"
	"f\n"
	"0.44 G\n"
	"1 0 0 rg\n"
	"1.2 w\n"
	"50 232 m\n"
	"73 257 292 498 325 537 c\n"
	"273 594 108 782 80 814 c\n"
	"224 945 l\n"
	"264 893 398 719 450 658 c\n"
	"483 697 650 896 690 946 c\n"
	"825 820 l\n"
	"791 786 585 578 557 544 c\n"
	"595 502 803 272 832 239 c\n"
	"691 103 l\n"
	"659 143 467 381 433 421 c\n"
	"413 398 198 126 178 101 c\n"
	"50 232 l\n"
	"B\n"
	"Q";

/*************************************************************************
 * Yellow question mark
 ************************************************************************/
static ASFixedRect stmUnknownBBox = { fixedZero,fixedZero,0x00630000,0x00630000 };
static const char* const stmUnknownData = 
	"% DSUnknown\n"
	"q\n"
	"1 G\n"
	"1 g\n"
	"0.1 0 0 0.1 9 0 cm\n"
	"0 J 0 j 4 M []0 d\n"
	"1 i \n"
	"0 g\n"
	"313 292 m\n"
	"313 404 325 453 432 529 c\n"
	"478 561 504 597 504 645 c\n"
	"504 736 440 760 391 760 c\n"
	"286 760 271 681 265 626 c\n"
	"265 625 l\n"
	"100 625 l\n"
	"100 828 253 898 381 898 c\n"
	"451 898 679 878 679 650 c\n"
	"679 555 628 499 538 435 c\n"
	"488 399 467 376 467 292 c\n"
	"313 292 l\n"
	"h\n"
	"308 214 170 -164 re\n"
	"f\n"
	"0.44 G\n"
	"1.2 w\n"
	"1 1 0.4 rg\n"
	"287 318 m\n"
	"287 430 299 479 406 555 c\n"
	"451 587 478 623 478 671 c\n"
	"478 762 414 786 365 786 c\n"
	"260 786 245 707 239 652 c\n"
	"239 651 l\n"
	"74 651 l\n"
	"74 854 227 924 355 924 c\n"
	"425 924 653 904 653 676 c\n"
	"653 581 602 525 512 461 c\n"
	"462 425 441 402 441 318 c\n"
	"287 318 l\n"
	"h\n"
	"282 240 170 -164 re\n"
	"B\n"
	"Q";

/*************************************************************************
 * Green check
 ************************************************************************/
static ASFixedRect stmDoubleValidBBox = { 0x00000000,0x00000000,0x03C39459,0x03E80000 };
static const char* const stmDoubleValidData = 
	"% DSDoubleValid\n"
	"q\n"
	"0 J 0 j 4 M []0 d\n"
	"1.5 i \n"
	"1 g\n"
	"370 37 m\n"
	"0 401 l\n"
	"126 574 l\n"
	"365 289 l\n"
	"779 1000 l\n"
	"944 876 l\n"
	"h f\n"
	"0 g\n"
	"406 0 m\n"
	"55 349 l\n"
	"157 485 l\n"
	"396 200 l\n"
	"817 916 l\n"
	"963 808 l\n"
	"h f\n"
	"0 G\n"
	"0 0.7 0.23 rg\n"
	"15 w\n"
	"377 50 m\n"
	"25 399 l\n"
	"128 536 l\n"
	"367 251 l\n"
	"787 967 l\n"
	"934 859 l\n"
	"b\n"
	"Q";

/*************************************************************************
 * DSHandler::Register
 * Register this handler with PubSec
 ************************************************************************/

bool DSHandler::Register()
{
	fHandlerName = ASAtomFromString( DOCSIGN_HANDLER_NAME );

	ACROASSERT( !fbHandlerIsInit && !fbHandlerIsRegistered );

	if( gPubSecHFT == NULL || gDigSigHFT == NULL || gAcroFormHFT == NULL || fbHandlerIsRegistered )
		return false;

	if( !fbHandlerIsInit ) {

		memset( &fHandlerRec, 0, sizeof(fHandlerRec) );

		DSEngine *psEngine = new DSEngine;
		if( !psEngine )
			return false;

		fHandlerRec.size = sizeof(PubSecHandlerRec);
		fHandlerRec.engine = (PubSecEngine) psEngine;

		fHandlerRec.getBoolProperty = ASCallbackCreateProto(PSGetBoolPropertyProc, DSHandler::GetBoolProperty );
		fHandlerRec.getAtomProperty = ASCallbackCreateProto(PSGetAtomPropertyProc, DSHandler::GetAtomProperty );
		fHandlerRec.getTextProperty = ASCallbackCreateProto(PSGetTextPropertyProc, DSHandler::GetTextProperty );
		fHandlerRec.getInt32Property = ASCallbackCreateProto(PSGetInt32PropertyProc, DSHandler::GetInt32Property );

		fHandlerRec.newEngine = ASCallbackCreateProto(PSNewEngineProc, DSHandler::NewEngine );
		fHandlerRec.destroyEngine = ASCallbackCreateProto(PSDestroyEngineProc, DSHandler::DestroyEngine );

		fHandlerRec.sessionAcquire = ASCallbackCreateProto(PSSessionAcquireProc, DSHandler::SessionAcquire );
		fHandlerRec.sessionRelease = ASCallbackCreateProto(PSSessionReleaseProc, DSHandler::SessionRelease );
		fHandlerRec.sessionReady = ASCallbackCreateProto(PSSessionReadyProc, DSHandler::SessionReady );
		fHandlerRec.performOperation = ASCallbackCreateProto(PSPerformOperationProc, DSHandler::PerformOperation );


		fHandlerRec.sigGetSigProperties = ASCallbackCreateProto(PSSigGetSigPropertiesProc, DSHandler::SigGetSigProperties );
		fHandlerRec.sigAuthenticate = ASCallbackCreateProto(PSSigAuthenticateProc, DSHandler::SigAuthenticate );
		fHandlerRec.sigGetSigValue = ASCallbackCreateProto(PSSigGetSigValueProc, DSHandler::SigGetSigValue );

		// Set up this callback if you want to have custom appearance
		fHandlerRec.sigCreateAPNXObj = ASCallbackCreateProto(PSSigCreateAPNXObjProc, DSHandler::SigCreateAPNXObj );
		fHandlerRec.sigValidate = ASCallbackCreateProto(PSSigValidateProc, DSHandler::SigValidate );
		fHandlerRec.sigValidateDialog = NULL;
		fHandlerRec.sigPropDialog = NULL;

		fHandlerRec.getLogo = ASCallbackCreateProto(PSGetLogoProc, DSHandler::GetLogo );

		// SigVal methods
		fHandlerRec.sigValGetText = ASCallbackCreateProto(PSSigValGetTextProc, DSSigVal::GetText );
		// Once you set up the PSSigCreateAPNXObjProc callback, you must set up this callback
		// in order to have the PSGetLogoProc callback invoked
		fHandlerRec.sigValGetAPLabel = ASCallbackCreateProto(PSSigValGetAPLabelProc, DSSigVal::GetAPLabel );

		// Cert exchange methods
		fHandlerRec.exportData = ASCallbackCreateProto(PSExportDataProc, DSHandler::ExportData );
		fHandlerRec.importData = NULL;
		
		// Encryption methods
		fHandlerRec.cryptOpenCMSEnvelope = ASCallbackCreateProto(PSOpenCMSEnvelopeProc, DSHandler::openCMSEnvelope);
		fHandlerRec.cryptGetImplicitRecipients = ASCallbackCreateProto(PSGetImplicitRecipientsProc, DSHandler::getImplicitRecipients);

		fbHandlerIsInit = true;
	}

	/* Register security handler. 
	   Note that ownership of this struct is retained by this plug-in */

	ASBool bOk = PSRegisterHandler( gExtensionID, &fHandlerRec );
	if( !bOk ) {
		// Destroy fHandlerRec
		Unregister();
	}

	fbHandlerIsRegistered = true;

	return true;
}

// Will remove struct from PubSecHFTs list of handlers
void DSHandler::Unregister()
{
	if( fbHandlerIsInit ) {

		if( fbHandlerIsRegistered )
			PSUnregisterHandler( &fHandlerRec );
		fbHandlerIsRegistered = false;

		if( fHandlerRec.engine ) {
			DSEngine* psEngine = (DSEngine*) fHandlerRec.engine;
			delete psEngine;
			fHandlerRec.engine = NULL;
		}

		/* Destroy callbacks */

		ASCallbackDestroy ( (void *)fHandlerRec.getBoolProperty );
		ASCallbackDestroy ( (void *)fHandlerRec.getAtomProperty );
		ASCallbackDestroy ( (void *)fHandlerRec.getTextProperty );
		ASCallbackDestroy ( (void *)fHandlerRec.getInt32Property );
		ASCallbackDestroy ( (void *)fHandlerRec.newEngine );
		ASCallbackDestroy ( (void *)fHandlerRec.destroyEngine );
		ASCallbackDestroy ( (void *)fHandlerRec.sessionAcquire );
		ASCallbackDestroy ( (void *)fHandlerRec.sessionRelease );
		ASCallbackDestroy ( (void *)fHandlerRec.sessionReady );
		ASCallbackDestroy ( (void *)fHandlerRec.performOperation );

		ASCallbackDestroy ( (void *)fHandlerRec.sigAuthenticate );
		ASCallbackDestroy ( (void *)fHandlerRec.sigGetSigProperties );
		ASCallbackDestroy ( (void *)fHandlerRec.sigGetSigValue );

		ASCallbackDestroy ( (void *)fHandlerRec.sigValidate );
		ASCallbackDestroy ( (void *)fHandlerRec.sigValidateDialog );
		ASCallbackDestroy ( (void *)fHandlerRec.sigCreateAPNXObj );
		ASCallbackDestroy ( (void *)fHandlerRec.getLogo );
		ASCallbackDestroy ( (void *)fHandlerRec.sigValGetAPLabel);
		ASCallbackDestroy ( (void *)fHandlerRec.sigValGetText );
		ASCallbackDestroy ( (void *)fHandlerRec.exportData );

		ASCallbackDestroy ( (void *)fHandlerRec.cryptOpenCMSEnvelope );
		ASCallbackDestroy ( (void *)fHandlerRec.cryptGetImplicitRecipients);

		memset( &fHandlerRec, 0, sizeof(fHandlerRec) );
	}
	fbHandlerIsInit = false;
}

/*************************************************************************
 * Common wrapper for engine calls that throw exceptions
 ************************************************************************/
#define ErrorASText(a,b)	ASTextFromPDText( "Error at File: " a " , Line: " #b )
#define PSTRY(myEng,inEng)	DSRetCode retCode = kDSParameterError; \
							DSEngine* myEng = (DSEngine*)inEng; \
							if( !myEng ) myEng = (DSEngine*)fHandlerRec.engine; \
							if( !myEng ) return retCode; \
							try {
#define PSCATCH				} catch(...) { \
								psEngine->putErrorText( ErrorASText( __FILE__, __LINE__ ) ); \
							} return retCode;

/*************************************************************************
 * Class DSHandler basic methods
 ************************************************************************/

ACCB1 DSRetCode ACCB2 DSHandler::SessionAcquire( PubSecEngine engine, PDDoc pdDoc, PSSessionOpType opType, ASText opText, ASCab cabParams, ASBool bUI )
{
	PSTRY(psEngine,engine) {
		retCode = psEngine->sessionAcquire( pdDoc, opType, opText, cabParams, bUI ? true : false );
	} PSCATCH;
}

ACCB1 DSRetCode ACCB2 DSHandler::SessionRelease( PubSecEngine engine, PSSessionOpType opType )
{
	PSTRY(psEngine,engine) {
		retCode = psEngine->sessionRelease( opType );
	} PSCATCH;
}

ACCB1 DSRetCode ACCB2 DSHandler::SessionReady( PubSecEngine engine, PSSessionOpType opType )
{
	PSTRY(psEngine,engine) {
		retCode = psEngine->sessionReady( opType );
	} PSCATCH;
}

ACCB1 DSRetCode ACCB2 DSHandler::PerformOperation( PubSecEngine engine, PSPerformOpType type, const ASCab cabParams, ASBool bUI )
{
	DSRetCode retCode = kDSException;
	DSEngine* psEngine = (DSEngine*)engine;
	if( !psEngine ) 
		psEngine = (DSEngine*)fHandlerRec.engine;
	if( !psEngine ) 
		return retCode;
	try {
		retCode = psEngine->performOperation( type, cabParams, bUI );
		if( ( retCode == kDSParameterError ) && ( cabParams == NULL ) )
			retCode = kDSFalse;
	} catch(...) { 
		psEngine->putErrorText( ErrorASText( __FILE__, __LINE__ ) );
	} 
	return retCode;
}

ACCB1 PubSecEngine ACCB2 DSHandler::NewEngine() 
{
	DSEngine* engine = new DSEngine;
	return (PubSecEngine) engine;
}

ACCB1 void ACCB2 DSHandler::DestroyEngine( PubSecEngine engine ) 
{
	if( engine ) {
		DSEngine* psEngine = (DSEngine*) engine;
		delete psEngine;
	}
}

ACCB1 ASBool ACCB2 DSHandler::GetBoolProperty( PubSecEngine engine, const char* szPropertyName, const ASBool defaultValue ) 
{
	// If engine ever put in DLL then move static responses here
	// (for example, response to PROP_PSENG_SignInvisible)

	bool retVal = defaultValue ? true : false;
	DSEngine* psEngine = (DSEngine*)engine;
	if( !psEngine ) psEngine = (DSEngine*)fHandlerRec.engine;
	if( psEngine && szPropertyName ) {
		try {
			if( szPropertyName ) {
				if( !strcmp( szPropertyName, PROP_PSENG_PDSignVisible ) ) {
					retVal = true;
				} else if( !strcmp( szPropertyName, PROP_PSENG_PDSignInvisible ) ) {
					retVal = true;
				} else if( !strcmp( szPropertyName, PROP_PSENG_PDSignAuthor ) ) {
					retVal = true;
				} else if( !strcmp( szPropertyName, PROP_PSENG_PDEncrypt ) ) {
					retVal = false;
				} else if( !strcmp( szPropertyName, PROP_PSENG_PDDecrypt ) ) {
					retVal = false;
				} else if( !strcmp( szPropertyName, PROP_PSENG_ExportContact ) ) {
					retVal = true;
				} else if( !strcmp( szPropertyName, PROP_PSENG_SignFormatPKCS1 ) ) {
					retVal = true;
				} else if( !strcmp( szPropertyName, PROP_PSENG_SignFormatPKCS7 ) ) {
					retVal = false;
				} else if( !strcmp( szPropertyName, PROP_PSENG_SignFormatPKCS1Digest ) ) {
					retVal = false;
				} else if( !strcmp( szPropertyName, PROP_PSENG_SignFormatPKCS7DetachedDigest ) ) {
					retVal = false;
				} else if( !strcmp( szPropertyName, PROP_PSENG_CosSign ) ) {
					retVal = true;
				} else if( !strcmp (szPropertyName, PROP_PSENG_PDSignCustomAP )) {
					retVal = true;
				} else {
					retVal = ((DSEngine*)psEngine)->getBoolProperty( szPropertyName, defaultValue ? true : false );
				}
			}	
		} catch (...) { };
	}
	return retVal ? true : false;
}

ACCB1 ASAtom ACCB2 DSHandler::GetAtomProperty( PubSecEngine engine, const char* szPropertyName ) 
{
	// Note that we do not call engine unless we have to. 
	// This allows possibility for engine code to be put in DLL that gets loaded later

	ASAtom retVal = ASAtomNull;
	if( szPropertyName ) {
		if( !strcmp( szPropertyName, PROP_PSENG_ASAtom_DigSigHandlerName ) )
			retVal = fHandlerName;
		else if( !strcmp( szPropertyName, PROP_PSENG_ASAtom_PubSecHandlerName ) )
			retVal = fHandlerName;
	}
	(void)engine;	// Deliberately unused parameter engine.
	return retVal;
}

ACCB1 ASInt32 ACCB2 DSHandler::GetInt32Property( PubSecEngine engine, const char* szPropertyName, const ASInt32 defaultValue ) 
{
	ASInt32 retVal = defaultValue;
	if( szPropertyName ) {

		// First deal with handler generic properties, then call 
		// engine for engine specific properities

		if( !strcmp( szPropertyName, PROP_PSENG_ASInt32_HandlerVersion ) ) {
			retVal = DOCSIGN_CURRENT_VERSION;
		} else {		
			DSEngine* psEngine = (DSEngine*)engine;
			if( !psEngine ) psEngine = (DSEngine*)fHandlerRec.engine;
			if( psEngine ) {
				try {
					retVal = ((DSEngine*)psEngine)->getInt32Property( szPropertyName, defaultValue );
				} catch (...) { };
			}
		}
	}
	return retVal;
}

ACCB1 ASText ACCB2 DSHandler::GetTextProperty( PubSecEngine engine, const char* szPropertyName, const ASInt32 index ) 
{
	ASText retVal = NULL;
	if (!strcmp( szPropertyName, PROP_PSENG_ASAtom_PubSecHandlerName) ) {
		// Language independent Handler name
		retVal = ASTextFromPDText( DOCSIGN_HANDLER_NAME );
	} else if (!strcmp( szPropertyName, PROP_PSENG_ASAtom_DigSigHandlerName) ) {
		retVal = ASTextFromPDText( DOCSIGN_HANDLER_NAME );
	} else if (!strcmp( szPropertyName, PROP_PSENG_HandlerUIName) ) {
		// Language DEpendent handler name
		retVal = ASTextFromPDText( DOCSIGN_HANDLER_NAME );
	} else {
		DSEngine* psEngine = (DSEngine*)engine;
		if( !psEngine ) psEngine = (DSEngine*)fHandlerRec.engine;
		if( psEngine ) {
			try {
				retVal = ((DSEngine*)psEngine)->getTextProperty( szPropertyName, index );
			} catch (...) { };
		}
	}

	return retVal;
}

/*************************************************************************
 * Encryption support routines
 ************************************************************************/

ACCB1 DSRetCode ACCB2 DSHandler::openCMSEnvelope ( PubSecEngine engine, ASUns8* inCMSEnvelope, ASUns32 inCMSEnvelopeSize, ASUns8** pOutData, ASUns32* pOutDataSize, ASBool inUIAllowed )
{
	PSTRY(psEngine, engine) {
		if(inCMSEnvelope != NULL && pOutData != NULL && pOutDataSize != NULL)
			retCode = psEngine->openCMSEnvelope(inCMSEnvelope, inCMSEnvelopeSize, pOutData, pOutDataSize, inUIAllowed);
	} PSCATCH;
}

ACCB1 DSRetCode ACCB2 DSHandler::getImplicitRecipients (PubSecEngine engine, ASCab outCertList, ASBool inUIAllowed )
{
	PSTRY(psEngine, engine) {
		if(outCertList != NULL)
		{
			psEngine->getImplicitRecipients(outCertList, inUIAllowed);
			retCode = kDSOk;
		}
	} PSCATCH;
}

/*************************************************************************
 * DigSigHFT routines
 ************************************************************************/

ACCB1 DSRetCode ACCB2 DSHandler::SigGetSigProperties( PubSecEngine engine, PSSigSigPropParams params ) 
{
	PSTRY(psEngine,engine) {
		if( params ) {
 			psEngine->sigGetSigProperties( params);
			retCode = kDSOk;
		}
	} PSCATCH;
}

ACCB1 DSRetCode ACCB2 DSHandler::SigAuthenticate( PubSecEngine engine, const PDDoc pdDoc, ASCab inESParams, ASBool bUI ) 
{
	PSTRY(psEngine,engine) {
		retCode = psEngine->sigAuthenticate( pdDoc, inESParams, bUI ? true : false );
	} PSCATCH;
}

ACCB1 DSRetCode ACCB2 DSHandler::SigGetSigValue( PubSecEngine engine, PSSigGetSigValueParams inOutParams ) 
{
	PSTRY(psEngine,engine) {
		if( inOutParams ) {
 			psEngine->sigGetSigValue( inOutParams);
			retCode = kDSOk;
		}
	} PSCATCH;
}

ACCB1 DSRetCode ACCB2 DSHandler::SigValidate( PubSecEngine engine, PSSigValidateParams params ) 
{
	PSTRY(psEngine,engine) {
		if( params ) {
			psEngine->sigValidate( params );
			retCode = kDSOk;
		}
	} PSCATCH;
}


/*************************************************************************
 * DSHandler::ExportData
 ************************************************************************/

ACCB1 DSRetCode ACCB2 DSHandler::ExportData( PubSecEngine engine, PSExportDataType dataType, ASCab outCab, ASBool bUI ) 
{
	PSTRY(psEngine,engine) {
		if( outCab ) {
			psEngine->exportData( dataType, outCab, bUI ? true : false );
			retCode = kDSOk;
		}
	} PSCATCH;
}

/*************************************************************************
 * DSHander::SigCreateAPNXObj
 * Returns a signature appearance that is put into the /AP dictionry
 * /N entry.
 ************************************************************************/
ACCB1 DSRetCode ACCB2 DSHandler::SigCreateAPNXObj( PubSecEngine engine, CosObj *pOutXObj, 
							PSSigPDDocParams docParams, PSAPSigType sigType )
{
	CosObj xObjArray[4];			// XObject Array for the apprearance layers
	ASFixedMatrix Matrix[4];		// XObject positions
	ASBool layerFlags[4];			// flags indicating whether the corresponding layers will be displayed

	PDDoc pdDoc = docParams->pdDoc;
	CosDoc cosDoc = PDDocGetCosDoc(pdDoc);
	PDAnnot pdAnnot = PDAnnotFromCosObj(docParams->sigAnnot);

	// set up AP
	// get border and background colors
	PDColorValueRec cBorder, cBgrd;
	AFPDWidgetGetAreaColors(pdAnnot, &cBorder, &cBgrd);

	// get widget border
	AFPDWidgetBorderRec border;
	AFPDWidgetGetBorder(pdAnnot, &border);

	// determine bbox dependent on rotation
	ASFixedRect annotRectRec;
	PDAnnotGetRect ( pdAnnot, &annotRectRec );

	// get bbox width and height
	ASFixed width = annotRectRec.right - annotRectRec.left;
	ASFixed height = annotRectRec.top - annotRectRec.bottom;

	// get rotation
	AVDoc avDoc = AVDocFromPDDoc(pdDoc);
	AVPageView pgView = AVDocGetPageView(avDoc);
	PDPage pdPage = AVPageViewGetPage(pgView);
	PDRotate pdRotate = PDPageGetRotate(pdPage);

	// Compensate for rotation
	ASFixedRect bboxRec;
	switch(pdRotate) {
		case pdRotate90:
		case pdRotate270:
			DocSignFixedRect(&bboxRec, 0, 0, height, width);
			break;
		default:
			DocSignFixedRect(&bboxRec, 0, 0, width, height);
			break;
	}

	// layout individual layers (n0 is created from acroforms background info)
	// All but n2 layer can be NULL, which will cause defaults to be used.
	xObjArray[0] = CosNewNull(); // for n1
	xObjArray[1] = CreateN2XObject(cosDoc, docParams->sigField, docParams->sigAnnot, &bboxRec);	// for n2
	xObjArray[2] = CosNewNull();	// for n3
	xObjArray[3] = CosNewNull();	// for n4

	//set ASFixedMatrixP of xObjects  
	for(int i = 0; i < 4; i++) 
		DocSignFixedMatrixSetIdentity( &Matrix[i] );

	// set flag for each XObject
	// The flags indicates whether any of the layers are optional. 
	// You must have n1 and n3 generated if you want your PSSigValGetAPLabelProc callback invoked
	layerFlags[0] = false; 
	layerFlags[1] = false;	
	layerFlags[2] = false;	
	layerFlags[3] = true;

	// Create multi layered XObject
	DSAPCreateLayeredStreamExParamsRec params;
	memset(&params, 0x00, sizeof(DSAPCreateLayeredStreamExParamsRec));
	params.XObjects = xObjArray;
	params.cosDoc = cosDoc;
	params.layerMatrices = Matrix;
	params.border = &border;
	params.cBorder = &cBorder;
	params.cBackGnd = &cBgrd;
	params.width = width;
	params.height = height;
	params.pdr = pdRotate;
	params.layerFlags = layerFlags;
	params.layerNNum = 4;

	CosObj tmpObj = DigSigAPCreateLayeredStreamEx(&params);

	*pOutXObj = DocSignStreamToXObject(tmpObj, NULL, NULL, true);

	return kDSOk;
}

/*************************************************************************
 * DSHandler::GetLogo
 * Returns string that is used to create signature background
 ************************************************************************/

ACCB1 void ACCB2 DSHandler::GetLogo( ASAtom label, const char* *pcLogo, const ASFixedRect* *pRect ) 
{
	if( pRect )
		*pRect = NULL;
	if( pcLogo )
		*pcLogo = NULL;

	if( label == DsSigValValid_K) {
		if( pRect )
			*pRect = &stmDoubleValidBBox;
		if( pcLogo ) 
			*pcLogo = stmDoubleValidData;
	} else if( label ==  DsSigValUnknown_K) {
		if( pRect)
			*pRect = &stmUnknownBBox;
		if( pcLogo )
			*pcLogo = stmUnknownData;
	} else if ( label == DsSigValInvalid_K) {
		if( pRect)
			*pRect = &stmInvalidBBox;
		if( pcLogo )
			*pcLogo = stmInvalidData;
	}
}

/*************************************************************************
 * Acrobat Logo
 ************************************************************************/

// Precise bbox of Acrobat logo stream stmLogoLogo
const ASFixedRect stmLogoBBox = { 0, 0x00630000, 0x00630000, 0 };	// 0,0,99,99
// Stream data for Acrobat Logo
const char* const stmLogoData =
	"% Acrobat Logo\n"
	"q\n"
    "0 J 0 j 4 M []0 d\n"
	"1 0 0 1 0 -1 cm\n"
	"1 0.85 0.85 rg\n"				/* faded red */
	"0 i\n"
	"96.1 24.7 m\n"
	"96.4 24.7 l\n"
	"96.8 24.7 97 24.6 97 24.3 c\n"
	"97 24 96.7 23.9 96.4 23.9 c\n"
	"96.1 23.9 l\n"
	"96.1 24.7 l\n"
	"94.1 23.7 m\n"
	"94.1 22.4 95.2 21.4 96.5 21.4 c\n"
	"98 21.4 99 22.4 99 23.7 c\n"
	"99 25.1 98 26.1 96.5 26.1 c\n"
	"95.2 26.1 94.1 25.1 94.1 23.7 c\n"
	"96.1 23.5 m\n"
	"96.4 23.5 l\n"
	"96.7 23.5 96.9 23.2 97 22.9 c\n"
	"97.1 22.4 l\n"
	"97.6 22.4 l\n"
	"97.5 22.9 l\n"
	"97.5 23.3 97.3 23.6 97 23.7 c\n"
	"97.3 23.8 97.6 23.9 97.6 24.3 c\n"
	"97.5 24.9 97.2 25.3 96.4 25.3 c\n"
	"95.6 25.3 l\n"
	"95.6 22.4 l\n"
	"96.1 22.4 l\n"
	"96.1 23.5 l\n"
	"94.6 23.7 m\n"
	"94.6 24.8 95.5 25.6 96.5 25.6 c\n"
	"97.7 25.6 98.4 24.8 98.4 23.7 c\n"
	"98.4 22.7 97.7 21.9 96.5 21.9 c\n"
	"95.5 21.9 94.6 22.7 94.6 23.7 c\n"
	"98.4 30.4 m\n"
	"98.7 31.1 99 31.4 99 32.1 c\n"
	"99 36 92.2 38 82.3 38 c\n"
	"78.9 38 74.8 37.6 70.3 37.3 c\n"
	"67.2 39 64.1 41 61.3 43.1 c\n"
	"54.2 49 49 58.5 45.9 68.1 c\n"
	"47.3 76 47.3 82.8 47.6 90.3 c\n"
	"46.9 86.9 46.2 81.4 44.6 73.2 c\n"
	"42.8 79.1 42.1 84.9 42.1 89.4 c\n"
	"42.1 90.3 42.1 96.5 44.2 97.9 c\n"
	"45.6 97.2 47.3 95.8 47.6 92.1 c\n"
	"49 98.5 46.6 98.5 42.8 98.5 c\n"
	"39.4 98.5 39.4 90.3 39.4 88.3 c\n"
	"39.4 85.5 39.8 82.1 40.4 78.6 c\n"
	"41 75.2 41.8 71.4 42.8 67.8 c\n"
	"42.8 62.3 16.8 1.2 2.7 1.2 c\n"
	"2.1 4.7 8.9 14.3 18.5 21.6 c\n"
	"4.8 14.3 0.8 6.1 0.8 2.6 c\n"
	"0.8 0.6 4.8 -0.1 5.1 -0.1 c\n"
	"9.9 -0.1 17.4 7.1 28.1 25.6 c\n"
	"39.8 29.7 55.1 32.7 68.8 34.1 c\n"
	"77.4 29.7 87 27 92.8 27 c\n"
	"95.9 27 97.6 27.7 98 29.3 c\n"
	"97.3 29 96.2 28.6 94.9 28.6 c\n"
	"90.4 28.6 82.5 31.1 74.7 34.8 c\n"
	"81.9 35.3 98.7 36 98.4 30.4 c\n"
	"28.1 25.9 m\n"
	"39.4 45.5 42.8 55.4 44.6 62.3 c\n"
	"51.4 45.2 60.3 39.4 65.1 36.6 c\n"
	"53.5 34.6 40.1 31.1 28.1 25.9 c\n"
	"h f\n"
	"Q";

/*************************************************************************
 * DSHander::CreateN2XObject
 * Returns an XObject for n2
 ************************************************************************/
CosObj DSHandler::CreateN2XObject( CosDoc cosDoc, const CosObj sigField, const CosObj sigAnnot, const ASFixedRect* const pBBoxRec )
{
	const char * strText = "DocSign custom appearance text.";
	DSAPTextEntryRec inText;
	memset(&inText, 0x00, sizeof(DSAPTextEntryRec));
	inText.next = NULL;
	inText.heightRatio = 0x00010000; // 100%
	inText.text = ASTextFromScriptText(strText, kASRomanScript);

	CosObj cosObj = DigSigAPCreateCompositeTextXObj(cosDoc, &inText, pBBoxRec, sigField, sigAnnot);
	return cosObj;
}

/*************************************************************************
 * DocSign support functions
 ************************************************************************/
void DSHandler::DocSignFixedRect(ASFixedRect *fr, ASFixed fxLeft, ASFixed fxBottom, ASFixed fxRight, ASFixed fxTop)
{
	fr->left = fxLeft;
	fr->top = fxTop;
	fr->right = fxRight;
	fr->bottom = fxBottom;
}

CosObj DSHandler::DocSignStreamToXObject(CosObj strm, ASFixedMatrixP matrix, const ASFixedRect* const bbox, ASBool bUseOldObj)
{
	CosDoc cosDoc;
	CosObj co;

	if(CosObjGetType(strm) != CosNull) {
		if(!bUseOldObj)
			strm = CosObjCopy(strm, CosObjGetDoc(strm), true);

		cosDoc = CosObjGetDoc(strm);
		co = CosStreamDict(strm);

		CosDictPut(co, Type_K, CosNewName(cosDoc, false, XObject_K));
		CosDictPut(co, Subtype_K, CosNewName(cosDoc, false, Form_K));
		CosDictPut(co, FormType_K, CosNewInteger(cosDoc, false, 1));

		if(bbox)
			DocSignCosPutRect(cosDoc, co, BBox_K, bbox);

		if(!CosDictKnown(co, Matrix_K) || matrix) {		
			if(matrix){
				ASFixedMatrix fm;
				if(CosDictKnown(co, Matrix_K)) {
					ASFixedMatrix om;

					DocSignCosGetMatrix(co, Matrix_K, &om);
					ASFixedMatrixConcat(&fm, matrix, &om);
				} else 
					fm = *matrix;

				DocSignCosPutMatrix(cosDoc, co, Matrix_K, &fm);
			} else {
				ASFixedMatrix fmId;
				DocSignFixedMatrixSetIdentity(&fmId);
				DocSignCosPutMatrix(cosDoc, co, Matrix_K, &fmId);
			}
		}
	}
	return strm;
}

void DSHandler::DocSignCosPutRect(CosDoc cd, CosObj coParent, ASAtom asaKey, const ASFixedRect *fr)
{
	CosObj coRect;
	
	ACROASSERT(fr);

	coRect = CosNewArray(cd, false, 4);

	CosArrayPut(coRect, 0, CosNewFixed(cd, false, fr->left));
	CosArrayPut(coRect, 1, CosNewFixed(cd, false, fr->bottom));
	CosArrayPut(coRect, 2, CosNewFixed(cd, false, fr->right));
	CosArrayPut(coRect, 3, CosNewFixed(cd, false, fr->top));

	CosDictPut(coParent, asaKey, coRect);
}

void DSHandler::DocSignCosPutMatrix(CosDoc cd, CosObj coParent, ASAtom asaKey, const ASFixedMatrix *m)
{
	CosObj coMatrix;
	
	ACROASSERT(m);

	coMatrix = CosNewArray(cd, false, 6);

	CosArrayPut(coMatrix, 0, CosNewFixed(cd, false, m->a));
	CosArrayPut(coMatrix, 1, CosNewFixed(cd, false, m->b));
	CosArrayPut(coMatrix, 2, CosNewFixed(cd, false, m->c));
	CosArrayPut(coMatrix, 3, CosNewFixed(cd, false, m->d));
	CosArrayPut(coMatrix, 4, CosNewFixed(cd, false, m->h));
	CosArrayPut(coMatrix, 5, CosNewFixed(cd, false, m->v));
	CosDictPut(coParent, asaKey, coMatrix);
}

void DSHandler::DocSignCosGetMatrix(CosObj coParent, ASAtom asaKey, ASFixedMatrix *m)
{
	CosObj coMatrix;

	ACROASSERT(m);
	
	/* Set to identity matrix. */
	DocSignFixedMatrixSetIdentity(m);

	coMatrix = CosDictGet(coParent, asaKey);
	if (CosObjGetType(coMatrix) == CosArray) {
		m->a = CosFixedValue(CosArrayGet(coMatrix, 0));
		m->b = CosFixedValue(CosArrayGet(coMatrix, 1));
		m->c = CosFixedValue(CosArrayGet(coMatrix, 2));
		m->d = CosFixedValue(CosArrayGet(coMatrix, 3));
		m->h = CosFixedValue(CosArrayGet(coMatrix, 4));
		m->v = CosFixedValue(CosArrayGet(coMatrix, 5));
	}
}

void DSHandler::DocSignFixedMatrixSet(ASFixedMatrix *fm, ASFixed a, ASFixed b, ASFixed c, ASFixed d, 
	ASFixed h, ASFixed v)
{
	if( fm ) {
		fm->a = a;
		fm->b = b;
		fm->c = c;
		fm->d = d;
		fm->h = h;
		fm->v = v;
	}
}
