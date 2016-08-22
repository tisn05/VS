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

 DSSigVal.h

*********************************************************************/

#ifndef DSSIGVAL_H
#define DSSIGVAL_H

#include "PubSecHFT.h"

/*************************************************************************
 * Class DSSigVal - all static
 ************************************************************************/

class DSSigVal {

protected:
	ASCab fCab;

public:

	/** Enum of handler-specific identity validation values */
	enum IdState 
	{
		/* cert has not yet been validated */
		kIdUnknown = 0,

		// Failure cases

		/** could not determine because of error */
		kIdUnknownTrouble,
		/** a problem with a certificate or the chain */
		kIdInvalid,
		/** a validity period problem with a certificate or the chain */
		kIdNotTimeValid,
		/** certificate has an untrusted root certificate */
		kIdUntrustedRoot,
		/** could not build a certificate chain up to a self-signed root certificate */
		kIdBrokenChain,
		/** Certificate chain has exceeded the specified length restriction. */
		kIdPathLenConstraint,
		/** One of the certificates in the chain has an unrecognized critical extension */
		kIdCriticalExtension,
		               
		// Success cases

		/** Signature JUST applied */
		kIdJustSigned,
		/** Signature is that of currently logged in user */
		kIdIsSelf,
		/** Signature is valid */
		kIdValid,

		kIdStateEnumSize
	};

#define DsSigValValid_K ASAtomFromString("DocSign Signature Valid")
#define DsSigValInvalid_K ASAtomFromString("DocSign Signature Invalid")
#define DsSigValUnknown_K ASAtomFromString("DocSign Signature Unknown")

	/** Maps idState to DSValidState */
	static const DSValidState mapIdToDSValidState[kIdStateEnumSize];


	/* 
	** Constructor/destructor
	*/

	DSSigVal( ASCab valCab ) { fCab = ASCabDup( valCab ); };
	~DSSigVal() { ASCabDestroy( fCab ); };

	/* 
	** Methods 
	*/

	/** Get text describing the validity of the signature. */
	ASText getText( DSSigValText textType, ASInt32 index = 0 );

	void setDocValidity( const DSSigValState state );
	void setSSIdValidity( const IdState state );
	void setIdValidity( const DSValidState state );
	void setIdTrustFlags( const PSSigTrust trust );
	IdState getSSIdValidity();
	DSSigValState getDocValidity();

	/* 
	** Static Procs. 
	** These are exposed directly to PubSecHFT.
	*/

	static ACCBPROTO1 ASText ACCBPROTO2 GetText( ASCab valCab, const DSSigValText textType, const ASInt32 index ) {
		DSSigVal sigVal(valCab);
		return sigVal.getText( textType, index );
	}

	static ACCBPROTO1 ASAtom ACCBPROTO2 GetAPLabel( ASCab valCab, const ASInt32 layerNum);

private:

};	// class DSSigVal

#endif /* DSSIGVAL_H */
