Option Strict Off
Option Explicit On
Module FileSystemUtils
	
	' ADOBE SYSTEMS INCORPORATED
	' Copyright (C) 1994-2003 Adobe Systems Incorporated
	' All rights reserved.
	'
	' NOTICE: Adobe permits you to use, modify, and distribute this file
	' in accordance with the terms of the Adobe license agreement
	' accompanying it. If you have received this file from a source other
	' than Adobe, then your use, modification, or distribution of it
	' requires the prior written permission of Adobe.
	
	' Constants / Declarations
	Public Const MAX_PATH As Short = 260
	Private Const BIF_RETURNONLYFSDIRS As Short = 1
	Private Const BIF_DONTGOBELOWDOMAIN As Short = 2
	Private Const OFN_FILEMUSTEXIST As Short = &H1000s
	
	Dim gFileSystem As Scripting.FileSystemObject
	
	' Standard Open File Dialog
	'UPGRADE_WARNING: Structure OPENFILENAME may require marshalling attributes to be passed as an argument in this Declare statement. Click for more: 'ms-help://MS.VSCC.2003/commoner/redir/redirect.htm?keyword="vbup1050"'
	Private Declare Function GetOpenFileName Lib "comdlg32"  Alias "GetOpenFileNameA"(ByRef pOpenfilename As OPENFILENAME) As Integer
	
	Private Structure OPENFILENAME
		Dim lStructSize As Integer
		Dim hwndOwner As Integer
		Dim hInstance As Integer
		Dim lpstrFilter As String
		Dim lpstrCustomFilter As String
		Dim nMaxCustFilter As Integer
		Dim nFilterIndex As Integer
		Dim lpstrFile As String
		Dim nMaxFile As Integer
		Dim lpstrFileTitle As String
		Dim nMaxFileTitle As Integer
		Dim lpstrInitialDir As String
		Dim lpstrTitle As String
		Dim flags As Integer
		Dim nFileOffset As Short
		Dim nFileExtension As Short
		Dim lpstrDefExt As String
		Dim lCustData As Integer
		Dim lpfnHook As Integer
		Dim lpTemplateName As String
	End Structure
	
	' FSQueryForFile
	'
	' Displays a standard open file dialog box.
	' Returns the path selected by the user or
	' the empty string if they cancelled.
	
	Public Function FSQueryForFile(ByRef parent As System.Windows.Forms.Form) As String
		
		Dim OpenFile As OPENFILENAME
		Dim lReturn As Integer
		Dim sFilter As String
		Dim sBuffer As String
		
		sFilter = "Catalog Index File (*.pdx)" & Chr(0) & "*.pdx" & Chr(0)
		
		With OpenFile
			.lStructSize = Len(OpenFile)
			.hwndOwner = parent.Handle.ToInt32
			.hInstance = VB6.GetHInstance.ToInt32
			.lpstrFilter = sFilter
			.nFilterIndex = 1
			.lpstrFile = New String(Chr(0), 257)
			.nMaxFile = Len(OpenFile.lpstrFile) - 1
			.lpstrFileTitle = OpenFile.lpstrFile
			.nMaxFileTitle = OpenFile.nMaxFile
			.lpstrInitialDir = "C:\"
			.lpstrTitle = "Select file to process..."
			.flags = OFN_FILEMUSTEXIST
		End With
		
		lReturn = GetOpenFileName(OpenFile)
		If lReturn = 0 Then
			sBuffer = ""
		Else
			sBuffer = Trim(OpenFile.lpstrFile)
		End If
		
		FSQueryForFile = sBuffer
	End Function
	
	' FSPopulateComboWithFilesFromFolder
	'
	' Populates a combobox with the names of
	' the files in the specified folder.
	
	Public Function FSPopulateComboWithFilesFromFolder(ByRef dlgItem As System.Windows.Forms.ComboBox, ByRef folder As String, ByRef extension As String) As Object
		
		If (gFileSystem Is Nothing) Then
			gFileSystem = New Scripting.FileSystemObject
		End If
		
		Dim optionsFile As Scripting.File
		Dim strPos As Short
		If (True = gFileSystem.FolderExists(folder)) Then
			
			
			For	Each optionsFile In gFileSystem.GetFolder(folder).Files
				
				
				strPos = InStr(optionsFile.Name, extension)
				If Not (0 = strPos) Then
					dlgItem.Items.Add(Left(optionsFile.Name, strPos - 1))
				End If
				
			Next optionsFile
		End If ' FolderExists
        Return DBNull.Value
	End Function
End Module