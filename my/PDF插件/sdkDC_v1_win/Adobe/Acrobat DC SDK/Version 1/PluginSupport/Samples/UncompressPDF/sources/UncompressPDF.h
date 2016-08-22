/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 2001-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/ 
/**
\file UncompressPDF.h

 - Declares the public methods implemented in UncompressPDF.cpp.

*********************************************************************/

#ifndef _UncompressPDF_h
#define _UncompressPDF_h

/* PIInitializeConversionHandler
** ----------------------------------------------------*/ 
/**
 ** Initialize the AVConversionFromPDFHandler and
 ** registers it with Acrobat.
 **
 ** @return true if initialization was successful.
 **/
ASBool PIInitializeConversionHandler ();

/* PITerminateConversionHandler
** ----------------------------------------------------*/ 
/**
 ** Releases resources associated with the
 ** AVConversionFromPDFHandler.
 **/
void PITerminateConversionHandler ();

#endif	// _UncompressPDF_h
