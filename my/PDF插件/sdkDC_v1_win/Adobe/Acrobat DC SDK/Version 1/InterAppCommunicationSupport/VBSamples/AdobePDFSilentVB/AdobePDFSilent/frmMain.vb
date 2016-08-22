' ADOBE SYSTEMS INCORPORATED
' Copyright (C) 1994-2005 Adobe Systems Incorporated
' All rights reserved.
'
' NOTICE: Adobe permits you to use, modify, and distribute this file
' in accordance with the terms of the Adobe license agreement
' accompanying it. If you have received this file from a source other
' than Adobe, then your use, modification, or distribution of it
' requires the prior written permission of Adobe.
Option Explicit On 

Imports System.IO
'Imports System.IO.FileSystemInfo

Friend Class frmMain
    Inherits System.Windows.Forms.Form
#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents TxtToConvertPath As System.Windows.Forms.TextBox
    Friend WithEvents TxtConvertedPath As System.Windows.Forms.TextBox
    Friend WithEvents btnBrowseInput As System.Windows.Forms.Button
    Friend WithEvents btnBrowseOutput As System.Windows.Forms.Button
    Friend WithEvents btnConvert As System.Windows.Forms.Button
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents btnExit As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.Label1 = New System.Windows.Forms.Label
        Me.TxtToConvertPath = New System.Windows.Forms.TextBox
        Me.btnBrowseInput = New System.Windows.Forms.Button
        Me.Label2 = New System.Windows.Forms.Label
        Me.TxtConvertedPath = New System.Windows.Forms.TextBox
        Me.btnBrowseOutput = New System.Windows.Forms.Button
        Me.btnConvert = New System.Windows.Forms.Button
        Me.Label3 = New System.Windows.Forms.Label
        Me.btnExit = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(16, 80)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(304, 23)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Input Directory (files to convert):"
        '
        'TxtToConvertPath
        '
        Me.TxtToConvertPath.Location = New System.Drawing.Point(16, 104)
        Me.TxtToConvertPath.Name = "TxtToConvertPath"
        Me.TxtToConvertPath.Size = New System.Drawing.Size(384, 20)
        Me.TxtToConvertPath.TabIndex = 1
        Me.TxtToConvertPath.Text = ""
        '
        'btnBrowseInput
        '
        Me.btnBrowseInput.Location = New System.Drawing.Point(416, 104)
        Me.btnBrowseInput.Name = "btnBrowseInput"
        Me.btnBrowseInput.Size = New System.Drawing.Size(64, 24)
        Me.btnBrowseInput.TabIndex = 2
        Me.btnBrowseInput.Text = "Browse..."
        '
        'Label2
        '
        Me.Label2.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(16, 160)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(312, 24)
        Me.Label2.TabIndex = 3
        Me.Label2.Text = "Output Directory (for converted files):"
        '
        'TxtConvertedPath
        '
        Me.TxtConvertedPath.Location = New System.Drawing.Point(16, 184)
        Me.TxtConvertedPath.Name = "TxtConvertedPath"
        Me.TxtConvertedPath.Size = New System.Drawing.Size(384, 20)
        Me.TxtConvertedPath.TabIndex = 4
        Me.TxtConvertedPath.Text = ""
        '
        'btnBrowseOutput
        '
        Me.btnBrowseOutput.Location = New System.Drawing.Point(416, 184)
        Me.btnBrowseOutput.Name = "btnBrowseOutput"
        Me.btnBrowseOutput.Size = New System.Drawing.Size(64, 24)
        Me.btnBrowseOutput.TabIndex = 5
        Me.btnBrowseOutput.Text = "Browse..."
        '
        'btnConvert
        '
        Me.btnConvert.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnConvert.Location = New System.Drawing.Point(128, 248)
        Me.btnConvert.Name = "btnConvert"
        Me.btnConvert.Size = New System.Drawing.Size(160, 32)
        Me.btnConvert.TabIndex = 6
        Me.btnConvert.Text = "Convert to PDF"
        '
        'Label3
        '
        Me.Label3.Enabled = False
        Me.Label3.Font = New System.Drawing.Font("Microsoft Sans Serif", 18.0!, CType((System.Drawing.FontStyle.Bold Or System.Drawing.FontStyle.Italic), System.Drawing.FontStyle), System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.Location = New System.Drawing.Point(16, 16)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(456, 32)
        Me.Label3.TabIndex = 7
        Me.Label3.Text = "Welcome to AdobePDF Print Silent"
        Me.Label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'btnExit
        '
        Me.btnExit.Location = New System.Drawing.Point(424, 272)
        Me.btnExit.Name = "btnExit"
        Me.btnExit.Size = New System.Drawing.Size(56, 24)
        Me.btnExit.TabIndex = 8
        Me.btnExit.Text = "E&xit"
        '
        'frmMain
        '
        Me.AcceptButton = Me.btnConvert
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(504, 310)
        Me.Controls.Add(Me.btnExit)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.btnConvert)
        Me.Controls.Add(Me.btnBrowseOutput)
        Me.Controls.Add(Me.TxtConvertedPath)
        Me.Controls.Add(Me.TxtToConvertPath)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.btnBrowseInput)
        Me.Controls.Add(Me.Label1)
        Me.Name = "frmMain"
        Me.Text = "AdobePDF Print Silent"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub frmMain_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        TxtToConvertPath.Text = CurDir() ' start with current directory as input directory
        TxtConvertedPath.Text = CurDir() ' and the output directory
        btnConvert.Focus() 'set Convert button to receive return

        'Support for running from the command line
        ' You must include one or two arguments to run and exit from
        ' the command line.
        ' 1st argument is input directory
        ' 2nd argument is output directory.
        ' If 2nd argument is omitted, then output and input directories are the same

        'get the command line arguments
        Dim CommandLineArgs As String() = Environment.GetCommandLineArgs()

        'get number of args
        'CommandLineArgs(0) is the executable
        Dim iNumArgs As Integer = CommandLineArgs.GetUpperBound(0)

        'if there are two arguments, then set the input and output directories from them
        If iNumArgs >= 2 Then

            TxtToConvertPath.Text = CommandLineArgs(1)
            TxtConvertedPath.Text = CommandLineArgs(2)

            'convert the files and exit
            ConvertAll()
            Application.Exit()

        ElseIf iNumArgs = 1 Then
            ' if one arg, use it for both paths
            TxtToConvertPath.Text = CommandLineArgs(1)
            TxtConvertedPath.Text = CommandLineArgs(1)

            'convert the files and exit
            ConvertAll()
            Application.Exit()

        Else 'no arguments, just open app

        End If

    End Sub

    Private Sub btnBrowseInput_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnBrowseInput.Click

        'Update directory through browse dialog
        Dim strDialogResults As String = SetBrowsePath("Browse for Input Directory")
        If Not (strDialogResults Is Nothing) Then
            TxtToConvertPath.Text = strDialogResults
        End If

    End Sub


    Friend Sub ConvertAll()

        'set string for AdobePDF printer driver
        Const strMyAdobePDFName As String = "Adobe PDF,winspool,Ne01:"

        Dim strFiles() As String 'array of file names in directory to convert one by one
        Dim i As Integer

        ' the following returns an array of FileInfo objects from the input directory
        strFiles = System.IO.Directory.GetFiles(TxtToConvertPath.Text)

        ' make sure there's at least 1 file in the input directory
        If UBound(strFiles) >= 0 Then

            Dim boolDefaultPrinterUpdated As Boolean = False
            Dim strOldPrinterName As String = ""

            'Manage File Types not to convert 
            'Add file extensions you wish to be skipped (not converted to PDF)
            'Must skip PDF or you might never finish processing if 
            ' In and Out directories are the same
            Dim strIgnoreFileTypes() As String = {".exe", ".pdf", ".dll"}

            Array.Sort(strIgnoreFileTypes)      'sort array for use in BinarySearch
            Dim oComp As New CaseInsensitiveComparer
            Dim iNoSkipThisFile As Integer      'compare results
            Dim strFileExt As String        'last split will be the file's extension

            For i = 0 To UBound(strFiles)   'For each file in the directory

                strFileExt = Microsoft.VisualBasic.Strings.Right(strFiles(i), _
                                        Len(strFiles(i)) - InStrRev(strFiles(i), ".") + 1)  ' get file extension
                ' check if this extension in the "ignore" array
                iNoSkipThisFile = Array.BinarySearch(strIgnoreFileTypes, strFileExt, oComp)

                'if file is not an extension to skip and there is an application associated 
                ' with it, then begin processing files
                If (iNoSkipThisFile < 0) Then
                    If Not (GetProgramPath(strFileExt) = "") Then

                        'first time, update the default printer to Adobe PDF and save old printer
                        If Not boolDefaultPrinterUpdated Then
                            boolDefaultPrinterUpdated = True
                            strOldPrinterName = SaveandUpdateDefaultPrinter(strMyAdobePDFName)
                        End If

                        ConvertFile(strFiles(i), TxtConvertedPath.Text)

                    Else
                        'Increment of file skipped for optional status message
                        'iSkipCount = iSkipCount + 1
                    End If
                Else
                    'Increment of files skipped for optional status message
                    'iSkipCount = iSkipCount + 1
                End If

            Next

            'Set default printer back when done
            If (strOldPrinterName <> "" And strOldPrinterName <> strMyAdobePDFName) Then
                strOldPrinterName = SaveandUpdateDefaultPrinter(strOldPrinterName)
            End If
            'Optional status message
            'MsgBox("Conversion complete.  " & i & " file(s) processed. " & iSkipCount & " file(s) skipped.", MsgBoxStyle.OKOnly, "Status")

        Else
            'Optional status message
            'MsgBox("There are no files in the input directory", MsgBoxStyle.RetryCancel, "Convert Files Error")

        End If
    End Sub

    Private Sub btnBrowseOutput_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnBrowseOutput.Click

        'update directory through browse dialog
        Dim strDialogResults As String = SetBrowsePath("Browse for Output Directory")
        If Not (strDialogResults Is Nothing) Then
            TxtConvertedPath.Text = strDialogResults
        End If

    End Sub

    Private Sub Convert_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnConvert.Click

        ConvertAll()

    End Sub

    Private Sub btnExit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnExit.Click
        frmMain.ActiveForm.Close()
    End Sub

End Class
