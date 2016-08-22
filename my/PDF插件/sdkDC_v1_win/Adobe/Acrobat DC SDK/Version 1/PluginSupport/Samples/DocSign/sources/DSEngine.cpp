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

#define IDS_ErrNoUser	"No user logged in to DocSign!"

extern ASUns8 hardwiredCert[]; // Really "forward", not extern. See EOF.
extern long hardwiredCertSize; // Really "forward", not extern. See EOF.

/** DSEngine constructor */
DSEngine::DSEngine( bool bUI )
{
	fUI = bUI;
	fErrorText = NULL;
	fCabValidate = NULL;
	fUser = NULL;
}

/** Called when creating a new signature.  For PDDoc signatures this
   call combines SigNew and SigCommit, with the values that PubSec
   writes into the sigDict being returned in sigPropCab.  If bPrompt
   is false then handler should not bring up UI (eg. when
   script-driven).  When script driven then parameters will be passed
   in esSigParamsCab. Parameters can include reason, password, etc..
   If the parameters are from the Acrobat Javascript guide then PubSec
   will transfer them to sigPropCab.  The handler can bring up UI
   during this call. Possible UI is to allow authentication and to
   bring up the signing dialog.  The value of saveOptions determines
   how PubSec will proceed.  PubSec will pop its own signing dialog if
   saveOptions is kPSSaveNone.  PubSec will cancel the signature if
   saveOptions is kPSSaveCancel.  Handler must return cert chain in
   certListCab - with signing cert first, which is used to extract cn,
   dn, etc (keys "0", "1", etc).  Handler must return sigPropCab with
   any common properties.  Handler may return sigAPCab with appearance
   selector information.  sigTypeParams is provided in the event that
   a handler needs this information. */
void DSEngine::sigGetSigProperties( PSSigSigPropParams params )
{
	ASCab certCab = params->outCertListCab;

	void *cert = ASmalloc( hardwiredCertSize );
	if (cert == NULL)
		return;

	// We CHEAT here (no crypto library!) - YOU must not!
	memcpy( cert, hardwiredCert, hardwiredCertSize );
	ASCabPutBinary( certCab, "0", cert, hardwiredCertSize );
	params->outSaveOptions = DSSignSave;
	params->outDialogStatus = kPSSigDialogSign;

	// Indicate we have custom apperances to deal with!	
	static char* docSignAP = "DocSignAP";	
	Uns16 strLen = sizeof(docSignAP) / sizeof(char);	
	char * heapString = static_cast<char *>(ASmalloc(strLen + 1));
	if(heapString != NULL) {
		strncpy(heapString, docSignAP, strLen);
		heapString[strLen] = '\0';
		PSSigPDDocParams docParams = (PSSigPDDocParams)params->docParams;
		ASCabPutString(docParams->sigAPCab, PROP_APCab_Type, heapString);
	}
}

/** Sign the digest and return the signature value, the memory for
   which becomes the property of the caller.  If pOutSigValueData is
   NULL then return expected size of signature value (this is only
   ever called for PKCS#7 signature).  For most values of digestMethod
   the digest will be passed into the handler, and the handler needs
   to sign it. However if a handler specifies that it must do its own
   digest (eg. legacy EntrustFile toolkit) then the digest that is
   passed in will be NULL. The handler can specify this property using
   PROP_PSENG_SignFormatPKCS7Digest. If and only if this property is true,
   then the digest will not be passed to the handler. 
   This proc ideally should not show any user interface and not allow
   a user to cancel the signing operation. */
void DSEngine::sigGetSigValue( PSSigGetSigValueParams params )
{
	// We can only 'sign' PKCS1 signatures with SHA1 hashes...
	if (params->sigMethod != kPSSigMethodPKCS1)
		return;
	if (params->digestMethod != kDSDigestSHA1)
		return;

	// We CHEAT here (no crypto library!) - YOU must not!
	if (ASBoolToBool(params->bGetSigValue) == true) {
		params->outSigValueData = (ASUns8 *)ASmalloc( 128 + 3 + 8 ); 
		memcpy( params->outSigValueData, "\x04\x81\x80", 3 );
		memcpy( params->outSigValueData + 3, params->digestValue, 20 ); // SHA1 is 20 bytes
	}

	
	params->outSigValueSize = 128 + 3 + 8;
}

/** Call handler to validate a signature.  Result is stored in
   params->sigValCab. Refer to PROP_SigVal_* for the list of
   properties in this ASCab.  The handler is required to set the value
   for PROP_SigVal_Id and PROP_SigVal_TrustFlags. The handler may also
   choose to look to see if PROP_SigVal_Doc has already been set, and
   may wish to set this value itself, depending on the format of the
   signature. All other values should be left alone. */
void DSEngine::sigValidate( PSSigValidateParams params )
{
	// We can only 'validate' PKCS1 signatures with SHA1 hashes...
	if (params->sigMethod != kPSSigMethodPKCS1)
		return;
	if (params->digestMethod != kDSDigestSHA1)
		return;

	// Confirm our signature size and the first 3 bytes
	if (params->sigValueSize != 131 + 8) // When we reencode the signing response sometimes some length fields grow
										 // when they are changed from BER encoding to DER encoding.
	{
		return;
	}
	if (memcmp( params->sigValueData, "\x04\x81\x80", 3 ) != 0)
		return;

	// We CHEAT here (no crypto library!) - YOU must not!
	// Confirm our signature is the same as the digest (all SHA1 are 20 bytes)
	if (memcmp( params->sigValueData +3, params->digestValue, 20 ) != 0)
		return;

	ASCab sigValCab = params->sigValCab;
	ASCabPutInt( sigValCab, PROP_SigVal_Digest, kDSSigValTrue );
	ASCabPutInt( sigValCab, PROP_SigVal_TrustFlags, kPSSigTrustAll );
	ASCabPutInt( sigValCab, PROP_SigVal_Id, DSSigDoubleChecked );
}

/** Perform the operation defined by type and with parameters contained
   in the cab. This interface is mainly used by EScript. 
   If cab is NULL then this is a test asking whether this operation is
   supported, so return kDSTrue or kDSFalse. */
DSRetCode DSEngine::performOperation( PSPerformOpType type, const ASCab cab, ASBool bUI )
{
	(void)bUI;  // Avoid warnings
	(void)cab;
	(void)type;
	return kDSFalse;
}

//*************************************************************************
//* DSEngine::getBoolProperty
//* may want to move some of these responses up into DSHandler
//* if we implement lazy loading of engine.
//*************************************************************************
bool DSEngine::getBoolProperty( const char* szPropertyName, const bool defaultValue )
{
	bool bRetVal = defaultValue;
	if( szPropertyName ) {
		if( !strcmp( szPropertyName, PROP_PSENG_IsLoggedOn ) ) {
			bRetVal = (fUser != NULL);
		} else if( !strcmp( szPropertyName, PROP_PSENG_UserInterface ) ) {
			bRetVal = fUI;
        }
	}
	return bRetVal;
}

//*************************************************************************
//* DSEngine::getInt32Property
//*************************************************************************

ASInt32 DSEngine::getInt32Property( const char* szPropertyName, const ASInt32 defaultValue )
{
	ASInt32 retVal = defaultValue;
	if(szPropertyName) 
	{
		if( !strcmp(szPropertyName, PROP_PSENG_ASInt32_AppearanceNum) ) 
		{
			DSAPFileAcquire(false, true);
			retVal = DSAPFileGetCount();
			DSAPFileRelease();
		}
	}
	return retVal;
}

//*************************************************************************
//* DSEngine::getTextProperty
//*************************************************************************

ASText DSEngine::getTextProperty(const char* szPropertyName, const ASInt32 index)
{
	ASText text = NULL;
	if( szPropertyName ) {
		if( !strcmp(szPropertyName, PROP_PSENG_ProfilePath) ) 
		{
			// Pathname of user profile file
		} 
		else if( !strcmp( szPropertyName, PROP_PSENG_CN ) ) 
		{
			// login user ID (Common Name?)
		} 
		else if( !strcmp(szPropertyName, PROP_PSENG_Appearances) ) 
		{
			// Appearance names
			DSAPFileAcquire(false,true);
			ASInt32 count = DSAPFileGetCount();
			if( count && index < count )
				text = DSAPFileGetNewNthName(index);
			DSAPFileRelease();
		} else if( !strcmp( szPropertyName, PROP_PSENG_Exception) ) {
			// Language independent exception description
			text = fErrorText;
			if( ASTextIsEmpty( text ) && DSRetCode(index) == kDSParameterError )
				text = ASTextFromPDText( "A parameter had an incorrect value. " );
		}
	}
	return text;
}

//*************************************************************************
//* DSEngine::sessionAcquire / sessionRelease
//*************************************************************************

bool DSEngine::sessionAcquire(const PDDoc& pdDoc, const PSSessionOpType operation, ASText opText, ASCab cabParams, bool bUI )
{
	bool bAcquired = false;

	// Make sure crypt stuff is init (does nothing if already init)

	switch (operation) {

	case kPSOpTypeNone:
	case kPSOpTypePDDocSigValidate:
	case kPSOpTypeCosDocSigValidate:
	case kPSOpTypeDataSigValidate:
	case kPSOpTypeCertEnum:
	case kPSOpTypeDirEnum:
	case kPSOpTypeDirGet:
	case kPSOpTypeStoreEnum:
	case kPSOpTypeAPRead:
		// DocSign uses PubSec for appearances, and this is always available, so return true.
  	case kPSOpTypeLogin:
	case kPSOpTypeLogout:
		// We are always ready to accept a specific command to login or logout.
		bAcquired = true;
		break;

	case kPSOpTypePDDocSign:
	case kPSOpTypeDataSign:
	case kPSOpTypeCosDocSign:
	case kPSOpTypePDCryptDocOpen:
		// Make sure we have a user
	case kPSOpTypePolicyModify:
		// Used to change password timeout
		// Acquire whatever is necessary to change settings in a PKCS#12 file
	case kPSOpTypeMyContactExport:
		// Acquire whatever is necessary in order to get the certificates that are needed
		// for exporting to a file (eg. FDF).
	case kPSOpTypePDCryptDocCreate:
		// To create an encrypted doc we may want to have an encryption credential to add to the 
		// recipient list as "author".  

		if (fUser == NULL && bUI)
			aquireUserWithUI( opText );	// Set fUser

		// If we still don't have a user it means that either user aquisition was cancelled or
		// we cannot show UI.  In either case throw the 'no-user' message up to PubSec level.
		if(fUser == NULL)
			throw IDS_ErrNoUser;

		// Make sure the user has a signing credential. This may also prompt credential 
		// selection. Will throw if a signing credential is not available or cannot be selected.

		bAcquired = true;
		break;

	case kPSOpTypeContactImport:
		// These are not supported by DocSign.
		bAcquired = false;
		break;

	default :
		// Check if there are any operations we do not know about
		// "Real" signature plug-ins may want to pull out the ACROASSERT() and set bAcquired
		// to true, as new operations are added from time to time. This leads to backward
		// compatibility issues for the code shown here that are probably benign.
		ACROASSERT( false );
		break;
	}

	(void)pdDoc; // Avoid warnings
	(void)cabParams;

	return bAcquired;
}

bool DSEngine::sessionRelease( const PSSessionOpType operation )
{
	switch (operation) {
	case kPSOpTypePDDocSign:
	case kPSOpTypePDDocSigValidate:
	case kPSOpTypeCosDocSign:
	case kPSOpTypeMyContactExport:
	case kPSOpTypePolicyModify:
	case kPSOpTypePDCryptDocCreate:
	case kPSOpTypePDCryptDocOpen:
	case kPSOpTypeCosDocSigValidate:
	case kPSOpTypeDataSign:
	case kPSOpTypeDataSigValidate:
	case kPSOpTypeAPRead:
	case kPSOpTypeContactImport:
	case kPSOpTypeLogin:
	case kPSOpTypeLogout:
	case kPSOpTypeDirEnum:
	case kPSOpTypeDirGet:
		break;
	}

	return true;
}

//*************************************************************************
//* DSEngine::readyForOperation
//* Plug-in specific test that data is consistent to proceed with operation.
//*************************************************************************

bool DSEngine::sessionReady( const PSSessionOpType operation )
{
	bool bRetVal = false;
	
	switch (operation) {
	case kPSOpTypePDDocSign:
	case kPSOpTypePDDocSigValidate:
	case kPSOpTypeCosDocSign:
	case kPSOpTypeMyContactExport:
	case kPSOpTypePolicyModify:
	case kPSOpTypePDCryptDocCreate:
	case kPSOpTypePDCryptDocOpen:
		if (fUser != NULL)
			bRetVal = true;
		break;
	case kPSOpTypeNone:
	case kPSOpTypeCosDocSigValidate:
	case kPSOpTypeDataSign:
	case kPSOpTypeDataSigValidate:
	case kPSOpTypeAPRead:
	case kPSOpTypeLogin:
	case kPSOpTypeLogout:
		bRetVal = true;
		break;
	case kPSOpTypeContactImport:
	case kPSOpTypeDirEnum:
	case kPSOpTypeDirGet:
		bRetVal = false;
		break;
	}
	return bRetVal;
}

// If inESParams then try to authenticate user using these params,
// else bring up UI, if necessary, to authenticate
// Set bPrompt to true if UI is allowed to come up.
bool DSEngine::sigAuthenticate( const PDDoc& pdDoc, ASCab inESParams, bool bUI )
{
	// Check inESParams values
	// If not enough, and UI is allowed, ask for rest
	if (bUI)
		return aquireUserWithUI( NULL );

	(void) pdDoc; // Avoid warnings
	(void) inESParams;
	// No UI allowed, not enough information in ESParams, fail
	return false;
}

/*************************************************************************
 * DSEngine::exportData
 * The entries in this cab are defined in PubSecHFT.h
 ************************************************************************/
bool DSEngine::exportData( PSExportDataType dataType, ASCab outCab, bool bUI )
{
	bool bRetVal = false;
	return bRetVal;
}

/** Open the provided PKCS#7 (CMS) Enveloped Data object and return the data contained in it.
   @param inCMSEnvelope IN specify an ASN1 encoded PKCS#7 Enveloped Data.
   @param inCMSEnvelopeSize IN size of inCMSEnvelope data
   @param pOutData OUT should point to envelope contents (allocated with ASMalloc) if the envelope 
     was opened, and be NULL otherwise.  PubSec will assume ownership of this data and 
     deallocate the memory using ASFree.
   @param pOutDataSize OUT the length of the returned data is returned here
   @param bInUIAllowed IN indicates whether the handler can pop-up any UI to open the envelope. For
     instance, a log-in maybe required to access the user's private key.
   @return kDSTrue, kDSFalse, kDSException, kDSParameterError
   kDSTrue means the envelope was opened
   kDSFalse means the envelope could not be opened
   */
ASBool DSEngine::openCMSEnvelope (ASUns8* inCMSEnvelope, ASUns32 inCMSEnvelopeSize, ASUns8** pOutData, ASUns32* pOutDataSize, ASBool inUIAllowed)
{
	return false;
}

/** PSGetImplicitRecipientsProc
   A new recipient list is being created by PubSec. The handler may
   wish to include some recipients into any recipient list (such as
   the current user).  This call allows the handler to provide
   certificates for such recipients.
   Parameters:
   - outCertList is a list of certificates of implicitly included recipients
     (ASN1-encoded, X509).
   - bInUIAllowed indicates whether the handler can show any UI in this function.
     For instance, a log-in maybe required to access a user's certificate.
   OPTIONAL */
void DSEngine::getImplicitRecipients (ASCab outCertList, ASBool inUIAllowed )
{
}

// This routine aquires a new user using UI and stuffs it into fUser.  Prompts selection of 
// an existing profile and a login or creation of a new user.  inReason is an optional hint 
// as why logging in / new profile creation is needed.
// Returns true if a user was acquired.

bool DSEngine::aquireUserWithUI( const ASText& inReason )
{
	fUser = (DSUser *)1;
	(void)inReason; // Avoid warnings
	return true;
}

ASUns8 hardwiredCert[] = {
0x30, 0x82, 0x02, 0x36, 0x30, 0x82, 0x01, 0xA3, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x10, 0x84,
0x7D, 0xFA, 0xEB, 0x9D, 0xDD, 0x5C, 0x88, 0x40, 0x23, 0xBB, 0x2F, 0x37, 0xF9, 0x59, 0xDD, 0x30,
0x09, 0x06, 0x05, 0x2B, 0x0E, 0x03, 0x02, 0x1D, 0x05, 0x00, 0x30, 0x55, 0x31, 0x11, 0x30, 0x0F,
0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x08, 0x4A, 0x6F, 0x68, 0x6E, 0x20, 0x44, 0x6F, 0x65, 0x31,
0x23, 0x30, 0x21, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x09, 0x01, 0x16, 0x14,
0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x40, 0x6E, 0x6F, 0x64, 0x6F, 0x6D, 0x61, 0x69, 0x6E,
0x2E, 0x6E, 0x6F, 0x6E, 0x31, 0x1B, 0x30, 0x19, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x12, 0x55,
0x53, 0x20, 0x2D, 0x20, 0x55, 0x4E, 0x49, 0x54, 0x45, 0x44, 0x20, 0x53, 0x54, 0x41, 0x54, 0x45,
0x53, 0x30, 0x1E, 0x17, 0x0D, 0x30, 0x33, 0x30, 0x33, 0x30, 0x36, 0x31, 0x38, 0x33, 0x36, 0x30,
0x30, 0x5A, 0x17, 0x0D, 0x30, 0x37, 0x30, 0x33, 0x30, 0x35, 0x31, 0x38, 0x33, 0x36, 0x30, 0x30,
0x5A, 0x30, 0x55, 0x31, 0x11, 0x30, 0x0F, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x08, 0x4A, 0x6F,
0x68, 0x6E, 0x20, 0x44, 0x6F, 0x65, 0x31, 0x23, 0x30, 0x21, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86,
0xF7, 0x0D, 0x01, 0x09, 0x01, 0x16, 0x14, 0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x40, 0x6E,
0x6F, 0x64, 0x6F, 0x6D, 0x61, 0x69, 0x6E, 0x2E, 0x6E, 0x6F, 0x6E, 0x31, 0x1B, 0x30, 0x19, 0x06,
0x03, 0x55, 0x04, 0x06, 0x13, 0x12, 0x55, 0x53, 0x20, 0x2D, 0x20, 0x55, 0x4E, 0x49, 0x54, 0x45,
0x44, 0x20, 0x53, 0x54, 0x41, 0x54, 0x45, 0x53, 0x30, 0x81, 0x9F, 0x30, 0x0D, 0x06, 0x09, 0x2A,
0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x81, 0x8D, 0x00, 0x30, 0x81,
0x89, 0x02, 0x81, 0x81, 0x00, 0xEB, 0x59, 0xFE, 0x11, 0x7B, 0x8A, 0x57, 0x14, 0xBE, 0x27, 0x3B,
0xBC, 0x1E, 0x0B, 0xDA, 0x5D, 0x7B, 0xEA, 0x1F, 0xB7, 0x7D, 0x2F, 0xBC, 0x5A, 0xE3, 0x02, 0x35,
0x83, 0x38, 0x55, 0x0B, 0x17, 0xD1, 0xA0, 0xCA, 0xBE, 0x4A, 0x61, 0x9E, 0x64, 0xDD, 0xBB, 0xFF,
0x36, 0x88, 0x00, 0xB8, 0x8B, 0xA4, 0x07, 0xA6, 0x2F, 0xBA, 0xA2, 0x70, 0xF1, 0xE9, 0x69, 0xA5,
0x16, 0x58, 0x1D, 0xBE, 0x95, 0xB3, 0xF1, 0xCD, 0x9B, 0x28, 0x6E, 0x6D, 0x9E, 0x7A, 0x32, 0xBB,
0x78, 0x2C, 0x4E, 0x17, 0x46, 0x61, 0x3A, 0xC3, 0x9F, 0xBF, 0xE6, 0x6A, 0x24, 0xD1, 0x2C, 0x7D,
0x92, 0x11, 0xD3, 0xC4, 0xC3, 0x72, 0x46, 0xAD, 0xC9, 0x70, 0x8C, 0x44, 0x81, 0x3E, 0xC9, 0xF1,
0xE7, 0x77, 0xF7, 0x3C, 0x42, 0x86, 0x42, 0x3F, 0x04, 0x95, 0xF1, 0xC6, 0x6F, 0xF7, 0xD4, 0x1F,
0x4D, 0xCD, 0xC0, 0xCA, 0x45, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3, 0x0F, 0x30, 0x0D, 0x30, 0x0B,
0x06, 0x03, 0x55, 0x1D, 0x0F, 0x04, 0x04, 0x03, 0x02, 0x04, 0xF0, 0x30, 0x09, 0x06, 0x05, 0x2B,
0x0E, 0x03, 0x02, 0x1D, 0x05, 0x00, 0x03, 0x81, 0x81, 0x00, 0x93, 0x7A, 0xD6, 0x1A, 0x56, 0x9A,
0x6C, 0x35, 0x46, 0x1A, 0x87, 0xE3, 0x91, 0x78, 0xA0, 0x09, 0xA0, 0x8F, 0x25, 0xDB, 0xF2, 0x0B,
0xBE, 0xAB, 0xE6, 0xC4, 0x51, 0x1F, 0x9C, 0xD9, 0x9D, 0x11, 0x3B, 0xE2, 0x18, 0xB4, 0xFA, 0x37,
0x40, 0xD4, 0x6A, 0x0C, 0xBA, 0xFC, 0xBF, 0x53, 0xAE, 0x44, 0x36, 0x6E, 0x77, 0x55, 0x4F, 0xB6,
0xA6, 0x22, 0x83, 0x15, 0xB4, 0x46, 0x9D, 0x2D, 0x52, 0xBE, 0xC9, 0xD5, 0x3F, 0x46, 0x72, 0x22,
0x83, 0x46, 0xC1, 0x91, 0x55, 0x73, 0xCE, 0x90, 0x98, 0x91, 0xDC, 0xEF, 0xD8, 0xCF, 0x49, 0x30,
0x40, 0xDF, 0xB4, 0xA4, 0x7D, 0x99, 0xF1, 0x90, 0xB9, 0x97, 0x94, 0xC4, 0x67, 0xF7, 0x7F, 0x28,
0x95, 0x69, 0x71, 0xF4, 0x3C, 0xFF, 0xF9, 0x55, 0xDC, 0x84, 0x09, 0xF5, 0x5B, 0xF5, 0xFA, 0x65,
0x32, 0x33, 0x69, 0x77, 0x38, 0x3F, 0x7E, 0x48, 0x8A, 0x18 };

long hardwiredCertSize = sizeof( hardwiredCert );
