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
	
	Public Const MAX_PATH As Short = 260
	Private Const BIF_RETURNONLYFSDIRS As Short = 1
	Private Const BIF_DONTGOBELOWDOMAIN As Short = 2
	
	Dim gFileSystem As Scripting.FileSystemObject
	
	'UPGRADE_WARNING: Structure BrowseInfo may require marshalling attributes to be passed as an argument in this Declare statement. Click for more: 'ms-help://MS.VSCC.2003/commoner/redir/redirect.htm?keyword="vbup1050"'
	Private Declare Function SHBrowseForFolder Lib "shell32" (ByRef lpbi As BrowseInfo) As Integer
	Private Declare Function SHGetPathFromIDList Lib "shell32" (ByVal pidList As Integer, ByVal lpBuffer As String) As Integer
	Private Declare Function lstrcat Lib "kernel32"  Alias "lstrcatA"(ByVal lpString1 As String, ByVal lpString2 As String) As Integer

	Private Structure BrowseInfo
		Dim hwndOwner As Integer
		Dim pIDLRoot As Integer
		Dim pszDisplayName As Integer
		Dim lpszTitle As Integer
		Dim ulFlags As Integer
		Dim lpfnCallback As Integer
		Dim lParam As Integer
		Dim iImage As Integer
	End Structure
	
	
	Public Function FSQueryForFolder() As String
		
		'Open a Browse Folders dialog that displays directories
		
		Dim lpIDList As Integer
		Dim sBuffer As String
		Dim szTitle As String
		Dim tBrowseInfo As BrowseInfo
        sBuffer = ""
		szTitle = "Select folder to process..."
		
		With tBrowseInfo
			.hwndOwner = MainFrm.DefInstance.Handle.ToInt32
			.lpszTitle = lstrcat(szTitle, "")
			.ulFlags = BIF_RETURNONLYFSDIRS + BIF_DONTGOBELOWDOMAIN
		End With
		
		lpIDList = SHBrowseForFolder(tBrowseInfo)
		
		' Append NULL character to string
		If Not (0 = lpIDList) Then
			sBuffer = New String(Chr(0), MAX_PATH)
			SHGetPathFromIDList(lpIDList, sBuffer)
			sBuffer = Left(sBuffer, InStr(sBuffer, vbNullChar) - 1)
		End If
		
		FSQueryForFolder = sBuffer
	End Function
	
	Public Function FSQueryForFile() As String

		Dim sBuffer As String
        Dim OpenFileName As String
        Dim ofDialog As New OpenFileDialog

        ofDialog.Filter = "PostScript Files (*.ps)|*.ps"
        If ofDialog.ShowDialog = DialogResult.OK Then
            OpenFileName = ofDialog.FileName
            sBuffer = Trim(OpenFileName)
        Else
            sBuffer = ""
        End If


		FSQueryForFile = sBuffer
	End Function
	
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
	
	
	Public Function FSValidatePath(ByRef path As String, ByRef mustBeFile As Boolean) As Boolean
		
		If (gFileSystem Is Nothing) Then
			gFileSystem = New Scripting.FileSystemObject
		End If
		
		If (mustBeFile) Then
			FSValidatePath = gFileSystem.FileExists(path)
		Else
			FSValidatePath = gFileSystem.FolderExists(path)
		End If
	End Function
End Module