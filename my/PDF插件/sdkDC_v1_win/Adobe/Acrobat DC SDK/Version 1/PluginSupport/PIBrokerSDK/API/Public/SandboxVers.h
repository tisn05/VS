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

/* for Adobe use only */
#define _SandboxHFT_LATEST_VERSION 0x00020000
#define _SandboxHFT_LAST_BETA_COMPATIBLE_VERSION 0x00010000
#define _SandboxHFT_IS_BETA 0

/* for public use */
#define SandboxHFT_LATEST_VERSION (_SandboxHFT_IS_BETA ? (kHFT_IN_BETA_FLAG | _SandboxHFT_LATEST_VERSION) : _SandboxHFT_LATEST_VERSION)

#define SandboxHFT_VERSION_1	0x00010000
#define SandboxHFT_VERSION_2	SandboxHFT_LATEST_VERSION