' ADOBE SYSTEMS INCORPORATED
' Copyright (C) 1994-2005 Adobe Systems Incorporated
' All rights reserved.
'
' NOTICE: Adobe permits you to use, modify, and distribute this file
' in accordance with the terms of the Adobe license agreement
' accompanying it. If you have received this file from a source other
' than Adobe, then your use, modification, or distribution of it
' requires the prior written permission of Adobe.

Option Strict Off
Option Explicit On 

Imports System.IO
Imports System.Diagnostics.Process

Module AdobePDFSilent

    'get the path from the browse dialog
    Friend Function SetBrowsePath(ByVal DescrText As String) As String

        Dim FBDialog As New FolderBrowserDialog

        FBDialog.Description = DescrText
        FBDialog.ShowNewFolderButton = True

        If (FBDialog.ShowDialog() = DialogResult.OK) Then
            Return FBDialog.SelectedPath
        Else
            Return ""
        End If

    End Function

    Friend Function GetProgramPath(ByVal strExtension As String) As String
        ' Return the program path (executable) associated with the file extension
        ' If there is no associated application, then return ""

        'This function attempts to get the application associated with an extension
        ' defined by the extension's subkey under the HKEY_CLASSES_ROOT key.
        ' If the extension does not have such a key or subkey, then the function 
        ' returns "" and the file is not converted.

        ' You may, of course, associate your file extensions and application 
        ' as you wish. This is the place to do so if you do not wish to rely on
        ' system registry entries. Just return the application path from this function
        ' for conversion.

        Dim objExtensionRegKey As Microsoft.Win32.RegistryKey = _
             Microsoft.Win32.Registry.ClassesRoot
        Dim objAppRegKey As Microsoft.Win32.RegistryKey = _
            Microsoft.Win32.Registry.ClassesRoot
        Dim objAppSubKey As Microsoft.Win32.RegistryKey

        Dim strExtensionValue As String

        ' Open and get registry key for this extension
        objExtensionRegKey = objExtensionRegKey.OpenSubKey(strExtension.Trim)

        ' If the registry key exists 
        If Not objExtensionRegKey Is Nothing Then

            'Then Check that the shell\open\command subkey exists
            strExtensionValue = objExtensionRegKey.GetValue("")
            objAppSubKey = objAppRegKey.OpenSubKey(strExtensionValue & "\shell\open\command")

            'And if it does exist
            If Not objAppSubKey Is Nothing Then 'if is has a value

                ' Begin processing the key value to get the actual program path
                ' get rid of any quotes and leading/trailing spaces in the program path
                ' extensions without an application may have a key but return "%1", 
                ' check for that and strip it out, too

                Dim SplitArray() As String
                SplitArray = Split(objAppSubKey.GetValue(Nothing), """")

                If SplitArray(0).Trim.Length > 0 Then
                    Return SplitArray(0).Replace(" %1", "")
                Else
                    Return SplitArray(1).Replace(" %1", "")
                End If
            Else
                Return "" 'subkey value is nothing
            End If
        Else
            Return ""   'subkey for the extension does not exist
        End If

    End Function

    Friend Function SaveandUpdateDefaultPrinter(ByVal strNewPrinter As String) As String

        'Update printer reg key for default printer
        'Return previous printer's name

        ' For Win XP, update this subkey to change printers: 
        '[HKEY_CURRENT_USER\Software\Microsoft\Windows NT\Current Version\Windows\Device]

        'Disclaimer: The registry contains system-related information that is 
        'critical to your computer and applications. Before modifying the registry, 
        'be sure to make a backup copy of it. Adobe doesn't provide support for problems 
        'that arise from improperly modifying the registry. Adobe strongly recommends that 
        'you have experience editing system files before you modify the registry. 
        ' Use the information provided at your own risk. 

        ' As an alternative you may wish to update your printer with the Microsoft 
        ' utility RUNDLL32.
        ' Please see MSDN: http://support.microsoft.com/default.aspx?scid=kb;en-us;314486

        Dim strSavedprinter As String   'old printer value

        Dim objPrinterRegKey As Microsoft.Win32.RegistryKey = Microsoft.Win32.Registry.ClassesRoot

        ' registry key for Win XP 
        Const strMyPrinterKey As String = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows"
        ' registry subkey for default printer
        Const strMyPrinterSubKey As String = "Device"

        'open subkey for writing
        objPrinterRegKey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(strMyPrinterKey, True)

        'Update Printer Registry Key 
        If objPrinterRegKey Is Nothing Then   'if key is not there, create it
            objPrinterRegKey.CreateSubKey(strMyPrinterSubKey)
            strSavedprinter = ""
        Else
            'If key is there then save old setting to restore
            strSavedprinter = objPrinterRegKey.GetValue(strMyPrinterSubKey)
        End If

        'Update subkey
        If Not (strSavedprinter = strNewPrinter) Then
            objPrinterRegKey.SetValue(strMyPrinterSubKey, strNewPrinter)
        End If

        objPrinterRegKey.Close()
        Return strSavedprinter

    End Function

    Friend Sub ConvertFile(ByVal strFilePath As String, ByVal strOutputDir As String)

        'Two parts to converting a file:
        'a) update registry for silent conversion of strFilePath
        'b) perform the print command with the associated application - PrintToAdobePDF()

        Dim objThisFile As New FileInfo(strFilePath)   'get FileInfo object for file string
        Dim strFileExt As String = objThisFile.Extension   'get file extension

        'Documentation example for what we're going to do in the registry:
        'Set the key for the output file name to turn off prompting
        ' [HKEY_CURRENT_USER\Software\Adobe\Acrobat Distiller\PrinterJobControl] 
        ' "C:\\Program Files\\Windows NT\\Accessories\\wordpad.exe" =
        ' "c:\\MyPDFoutputFileName.pdf"
        ' removed once the StartDoc(HDC hdc, CONST DOCINFO* lpdi) has successfully completed

        'Get application for current file
        Dim strAppPath As String = GetProgramPath(strFileExt)

        'If there is an app for the file, then convert
        If Not strAppPath = "" Then

            'Get file name without extension
            Dim strFilePrefix As String = Path.GetFileNameWithoutExtension(strFilePath)

            'Create output file path
            Dim strOutputFile As String = strOutputDir & "\" & strFilePrefix & ".pdf"
            ' Registry Updates
            Dim objDistillerRegKey As Microsoft.Win32.RegistryKey = Microsoft.Win32.Registry.ClassesRoot
            Dim strDistillerSubKey As String = "SOFTWARE\\Adobe\\Acrobat Distiller\\PrinterJobControl"

            'Open Current User's Distiller Subkey for writing
            objDistillerRegKey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(strDistillerSubKey, True)

            Dim strSaveOutputPath As String = ""

            'Distiller Registry Key 
            If objDistillerRegKey Is Nothing Then   'if key is not there, create it
                objDistillerRegKey = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(strDistillerSubKey)
            End If

            If (Not objDistillerRegKey Is Nothing) Then     'set reg key value for this app and file
                objDistillerRegKey.SetValue(strAppPath, strOutputFile)
                objDistillerRegKey.Close()
            End If

            PrintToAdobePDF(strFilePath)
            'if there's an app for this file

            ' Else
            '  Microsoft.VisualBasic.Interaction.MsgBox("No app for this file " & strFilePath)

        End If


    End Sub

    Private Sub PrintToAdobePDF(ByVal InputfilePath As String)

        'Prints InputFilePath to the AdobePDF printer. 
        'Since we just gathered all this info programmatically, 
        'this function assumes the file is present, that it has an 
        'associated application and that the current user has print privileges.

        'Define properties for the print process
        Dim pProcInfo As New ProcessStartInfo

        pProcInfo.FileName = InputfilePath
        pProcInfo.Verb = "Print"

        'Make process invisible
        pProcInfo.CreateNoWindow = True
        pProcInfo.WindowStyle = ProcessWindowStyle.Hidden

        'start print process
        Dim pMyProc As Process = Process.Start(pProcInfo)
        pMyProc.WaitForExit()

    End Sub

End Module

