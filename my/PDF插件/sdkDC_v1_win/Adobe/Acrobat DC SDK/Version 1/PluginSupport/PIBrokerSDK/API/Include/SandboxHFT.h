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
* suppliers and may be covered by U.S. and Foreign Patents,
* patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe Systems Incorporated.
**************************************************************************/

#ifndef SANDBOXHFT_H
#define SANDBOXHFT_H

//#define MAX_PIPENAME_LEN 240
#include "SandboxExpT.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Enumerate the selectors */
#define NPROC(returnType, name, params)			\
	name##SEL,
enum {
	SandboxBAD_SELECTOR,
	#include "SandboxProcs.h"
	SandboxNUMSELECTORSplusOne
};
#define SandboxNUMSELECTORS (SandboxNUMSELECTORSplusOne - 1)
#undef NPROC

/* Declare a global sandbox HFT object */
extern HFT gSandboxHFT;
extern ASVersion gSandboxVersion;

/* Define function prototypes */
#define NPROC(returnType, name, params)		\
	typedef ACCBPROTO1 returnType (ACCBPROTO2 *name##SELPROTO)params;
	#include "SandboxProcs.h"
#undef NPROC

void SetUpSandboxHFTServer(void);

#ifdef __cplusplus
}
#endif

#endif /* _SANDBOXHFT_H_ */
