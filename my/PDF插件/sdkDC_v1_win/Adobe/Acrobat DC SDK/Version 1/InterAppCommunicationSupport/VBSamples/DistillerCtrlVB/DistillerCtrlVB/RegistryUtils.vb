Option Strict Off
Option Explicit On
Module RegistryUtils
	
	' ADOBE SYSTEMS INCORPORATED
	' Copyright (C) 1994-2003 Adobe Systems Incorporated
	' All rights reserved.
	'
	' NOTICE: Adobe permits you to use, modify, and distribute this file
	' in accordance with the terms of the Adobe license agreement
	' accompanying it. If you have received this file from a source other
	' than Adobe, then your use, modification, or distribution of it
	' requires the prior written permission of Adobe.
	
	Private Const REG_SZ As Integer = 1
	Private Const REG_DWORD As Integer = 4
	
	Public Const HKEY_CLASSES_ROOT As Integer = &H80000000
	Public Const HKEY_CURRENT_USER As Integer = &H80000001
	Public Const HKEY_LOCAL_MACHINE As Integer = &H80000002
	Public Const HKEY_USERS As Integer = &H80000003
	
	Private Const ERROR_NONE As Short = 0
	
	Private Const KEY_QUERY_VALUE As Short = 1
	
	Declare Function RegCloseKey Lib "advapi32.dll" (ByVal hKey As Integer) As Integer
	Declare Function RegOpenKeyEx Lib "advapi32.dll"  Alias "RegOpenKeyExA"(ByVal hKey As Integer, ByVal lpSubKey As String, ByVal ulOptions As Integer, ByVal samDesired As Integer, ByRef phkResult As Integer) As Integer
	Declare Function RegQueryValueExString Lib "advapi32.dll"  Alias "RegQueryValueExA"(ByVal hKey As Integer, ByVal lpValueName As String, ByVal lpReserved As Integer, ByRef lpType As Integer, ByVal lpData As String, ByRef lpcbData As Integer) As Integer
	
	Public Function RegistryQueryStringValue(ByVal hive As Integer, ByRef keyName As String) As String
		
		Dim cch As Integer
		Dim lRetVal As Integer
		Dim lType As Integer
		Dim hKey As Integer
		Dim sValue As String
		
		On Error GoTo RegistryQueryStringValueError
		
		' Open the key
		lRetVal = RegOpenKeyEx(hive, keyName, 0, KEY_QUERY_VALUE, hKey)
		If lRetVal <> ERROR_NONE Then Error(5)
		
		' Read the string value
		sValue = New String(Chr(0), MAX_PATH)
		cch = MAX_PATH - 1
		lRetVal = RegQueryValueExString(hKey, "", 0, lType, sValue, cch)
		If lRetVal = ERROR_NONE Then
			sValue = Left(sValue, cch - 1)
		Else
			Error(5)
		End If
		
		' Close the key
		RegCloseKey(hKey)
		
RegistryQueryStringValueExit: 
		RegistryQueryStringValue = sValue
		Exit Function
RegistryQueryStringValueError: 
		sValue = ""
		Resume RegistryQueryStringValueExit
	End Function
End Module