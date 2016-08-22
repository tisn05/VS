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

 DSSigVal.cpp

*********************************************************************/

#include "DocSign.h"
#include "DSSigVal.h"

/*************************************************************************
 * DSSigVal::getText
 * Maps signature validity to text
 ************************************************************************/

ASText DSSigVal::getText( DSSigValText textType, ASInt32 index )
{
	char *text = NULL;

	switch( textType ) {
	// This string is shown as a separate line in the properties/validate dialogs
	case kDSSigValTextPropIdValidity:
		text = "This is DocSign's kDSSigValTextPropIdValidity string.";
		break;
	// This string is shown as a separate entry in the AVPanel, signature validity details
	case kDSSigValTextDSAVIdValidity:
		text = "This is DocSign's kDSSigValTextDSAVIdValidity string.";
		break;
	// This string is a substring of the tooltip, when requested (usually for invalid sig Id)
	case kDSSigValTextDSTTIdValidity:
		text = "This is DocSign's kDSSigValTextDSTTIdValidity string.";
		break;
	}

	if (text == NULL)
		return NULL;

	return ASTextFromPDText( text );
}

ASAtom DSSigVal::GetAPLabel( ASCab valCab, const ASInt32 layerNum )
{
	// n1 is used for the unknown and valid states
	// n3 is used for the invalid state
	// n3 is blank whenever n1 is in use and vice versa
	ASAtom retVal;
	if ( layerNum == 1 ) {
		ASInt32 sigValState = ASCabGetInt( valCab, PROP_SigVal_Digest, kDSSigValUnknown );
		switch(sigValState) {
			case kDSSigValTrue:
			case kDSSigValJustSigned:
			case kDSSigValUnused:
				retVal = DsSigValValid_K;
				break;
			case kDSSigValUnknown:
			case kDSSigValUnknownTrouble:
			case kDSSigValUnknownBytesNotReady:
				retVal = DsSigValUnknown_K;
				break;
			default:
				retVal = ASAtomNull;
		}
	} else if ( layerNum == 3 ) {
		ASInt32 sigValState = ASCabGetInt( valCab, PROP_SigVal_Digest, kDSSigValUnknown );
		switch(sigValState) {
			case kDSSigValFalse:
			case kDSSigValInvalidTrouble:
				retVal = DsSigValInvalid_K;
				break;
			default:
				retVal = ASAtomNull;
		}
	}
	return retVal;
}
