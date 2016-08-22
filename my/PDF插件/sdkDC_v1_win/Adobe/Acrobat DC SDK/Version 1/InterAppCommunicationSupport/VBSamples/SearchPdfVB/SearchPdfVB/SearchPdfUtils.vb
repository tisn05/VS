Option Strict Off
Option Explicit On
Module SearchPdfUtils
	
	' ADOBE SYSTEMS INCORPORATED
	' Copyright (C) 1994-2003 Adobe Systems Incorporated
	' All rights reserved.
	'
	' NOTICE: Adobe permits you to use, modify, and distribute this file
	' in accordance with the terms of the Adobe license agreement
	' accompanying it. If you have received this file from a source other
	' than Adobe, then your use, modification, or distribution of it
	' requires the prior written permission of Adobe.
	'
	' -------------------------------------------------------------------
	'
	' SearchPDFUtils.bas
	'
	' - When declaring DLL procedures in VB you must either specify the 
	'   entire path to the DLL or it must be located along the standard
	'   VB search path. This means that you must do one of the following
	'   before using this sample:
	'
	'   1. move ddeproxy.dll (to the windows system directory)
	'   2. add the full path to this file
	'   3. add the path to ddeproxy.dll to the PATH environment variable.
	
	'"D:\path\to\AcrobatSDK\InterAppSupport\Win\Visual Basic\SearchPDF\DdeProxy\Release\ddeproxy.dll"
	
	'Used for storing the path to the executeable
	Public buf As String
	
	' Set the Query language
	Declare Sub SetQueryLanguageType Lib "DdeProxy.dll"  Alias "_SetQueryLanguageType@4"(ByVal language As Short)
	' Set the MaxDocs
	Declare Sub SetQueryMaxDocs Lib "DdeProxy.dll"  Alias "_SetQueryMaxDocs@4"(ByVal maxDocs As Short)
	' Override the Query word options
	Declare Sub SetQueryOverideWordOptions Lib "DdeProxy.dll"  Alias "_SetQueryOverrideWordOptions@0"()
	' Set the Query word options
	Declare Sub SetQueryWordOptions Lib "DdeProxy.dll"  Alias "_SetQueryWordOptions@4"(ByVal flags As Integer)
	' Set the Query target word
	Declare Sub SetQueryWordTarget Lib "DdeProxy.dll"  Alias "_SetQueryWordTarget@4"(ByVal target As String)
	' Set the Sort option with boolean for ascending (0=descend, 1=ascend)
	Declare Sub SetQuerySortOption Lib "DdeProxy.dll"  Alias "_SetQuerySortOption@8"(ByVal field As String, ByVal ascending As Short)
	
	' Set the Index Action Type
	Declare Sub SetIndexAction Lib "DdeProxy.dll"  Alias "_SetIndexAction@4"(ByVal action As Short)
	' Set the Index name
	Declare Sub SetIndexName Lib "DdeProxy.dll"  Alias "_SetIndexName@4"(ByVal indexName As String)
	' Set the Index temporary name
	Declare Sub SetIndexTempName Lib "DdeProxy.dll"  Alias "_SetIndexTempName@4"(ByVal TempIndexName As String)
	
	' Initialize the Query DDE Conversation
	Declare Sub SrchDDEInitQuery Lib "DdeProxy.dll"  Alias "_SrchDDEInitQuery@0"()
	' Initialize the Index DDE Conversation
	Declare Sub SrchDDEInitIndex Lib "DdeProxy.dll"  Alias "_SrchDDEInitIndex@0"()
	' Send the QueryData packet and terminate DDE Conversation
	Declare Sub SrchDDESendQuery Lib "DdeProxy.dll"  Alias "_SrchDDESendQuery@0"()
	' Send the IndexData packet and terminate DDE Conversation
	Declare Sub SrchDDESendIndex Lib "DdeProxy.dll"  Alias "_SrchDDESendIndex@0"()
	
	
	' * Function Prototypes and datatypes for accessing the Windows Registry
	Public Const HKEY_LOCAL_MACHINE As Integer = &H80000002
	Public Const ERROR_SUCCESS As Short = 0
	Public Const SYNCHRONIZE As Integer = &H100000
	Public Const STANDARD_RIGHTS_READ As Integer = &H20000
	Public Const KEY_QUERY_VALUE As Short = &H1s
	Public Const KEY_ENUMERATE_SUB_KEYS As Short = &H8s
	Public Const KEY_NOTIFY As Short = &H10s
	Public Const KEY_READ As Boolean = ((STANDARD_RIGHTS_READ Or KEY_QUERY_VALUE Or KEY_ENUMERATE_SUB_KEYS Or KEY_NOTIFY) And (Not SYNCHRONIZE))
	
	Public Const QPON_Case As Short = &H1s
	Public Const QPON_Stemming As Short = &H2s
	Public Const QPON_SoundsLike As Short = &H4s
	Public Const QPON_Thesaurus As Short = &H8s
	Public Const QPON_Proximity As Short = &H10s
	Public Const QPON_Refine As Short = &H20s
End Module