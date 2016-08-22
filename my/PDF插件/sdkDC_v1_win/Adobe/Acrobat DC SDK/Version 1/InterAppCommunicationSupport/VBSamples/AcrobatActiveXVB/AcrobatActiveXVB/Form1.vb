'*********************************************************************
'
' ADOBE SYSTEMS INCORPORATED
' Copyright (C) 1994-2004 Adobe Systems Incorporated
' All rights reserved.
'
' NOTICE: Adobe permits you to use, modify, and distribute this file
' in accordance with the terms of the Adobe license agreement
' accompanying it. If you have received this file from a source other
' than Adobe, then your use, modification, or distribution of it
' requires the prior written permission of Adobe.
'
' ---------------------------------------------------------------------
'
' AcrobatOCXSample ( VB.NET Project ) 
'   - Acrobat provided a COM-based AcroPDF.dll, and you can add it to your 
'  project's references to have such a new control in your toolbox. Drag-
'  Drop the ActiveX control to a window in your application, then you can
'  get a PDF document window with toolbars. Besides, you can use the APIs 
'  provided by the ActiveX control to programmatically manage the window, 
'  such as opening a PDF file from a URL or a local disk, navigation, 
'  setting display mode or style, hiding toolbars, etc.
'   - To add Acrobat ActiveX to your project, go through the menu "Project - 
'  Add References ... - COM" select "Adobe Acrobat 7.0 Browswer Control Type 
'  Library 1.0" which points to AcroPDF.dll under Acrobat. Then, add the Acrobat 
'  ActiveX control to the "Toolbox" so that you can use. 
'   - The four Form vb files and UI resources in this VB.NET project were 
'  created by Acrobat SDK team, while other files were machine created. 
'
'*********************************************************************/

Public Class AcrobatPDFWin
    Inherits System.Windows.Forms.Form
    Public Shared AcrobatPDFOCX As AxAcroPDFLib.AxAcroPDF

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
    Friend WithEvents AxAcroPDF1 As AxAcroPDFLib.AxAcroPDF
    Friend WithEvents BtnNavigation As System.Windows.Forms.Button
    Friend WithEvents BtnPrinting As System.Windows.Forms.Button
    Friend WithEvents BtnWindow As System.Windows.Forms.Button
    Friend WithEvents TextAddress As System.Windows.Forms.TextBox
    Friend WithEvents BtnFile As System.Windows.Forms.Button
    Friend WithEvents BtnAbout As System.Windows.Forms.Button
    Friend WithEvents ButtonGoToURL As System.Windows.Forms.Button
    Friend WithEvents ButtonVersion As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(AcrobatPDFWin))
        Me.Label1 = New System.Windows.Forms.Label
        Me.TextAddress = New System.Windows.Forms.TextBox
        Me.ButtonGoToURL = New System.Windows.Forms.Button
        Me.BtnNavigation = New System.Windows.Forms.Button
        Me.AxAcroPDF1 = New AxAcroPDFLib.AxAcroPDF
        Me.BtnAbout = New System.Windows.Forms.Button
        Me.BtnPrinting = New System.Windows.Forms.Button
        Me.BtnWindow = New System.Windows.Forms.Button
        Me.BtnFile = New System.Windows.Forms.Button
        Me.ButtonVersion = New System.Windows.Forms.Button
        CType(Me.AxAcroPDF1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(8, 8)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(56, 24)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Address"
        '
        'TextAddress
        '
        Me.TextAddress.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TextAddress.Location = New System.Drawing.Point(72, 8)
        Me.TextAddress.Name = "TextAddress"
        Me.TextAddress.Size = New System.Drawing.Size(440, 22)
        Me.TextAddress.TabIndex = 1
        Me.TextAddress.Text = ""
        '
        'ButtonGoToURL
        '
        Me.ButtonGoToURL.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ButtonGoToURL.Location = New System.Drawing.Point(520, 8)
        Me.ButtonGoToURL.Name = "ButtonGoToURL"
        Me.ButtonGoToURL.Size = New System.Drawing.Size(88, 24)
        Me.ButtonGoToURL.TabIndex = 2
        Me.ButtonGoToURL.Text = "Go To URL"
        '
        'BtnNavigation
        '
        Me.BtnNavigation.Location = New System.Drawing.Point(112, 40)
        Me.BtnNavigation.Name = "BtnNavigation"
        Me.BtnNavigation.Size = New System.Drawing.Size(96, 24)
        Me.BtnNavigation.TabIndex = 4
        Me.BtnNavigation.Text = "Navigation"
        '
        'AxAcroPDF1
        '
        Me.AxAcroPDF1.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
                    Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.AxAcroPDF1.Enabled = True
        Me.AxAcroPDF1.Location = New System.Drawing.Point(8, 72)
        Me.AxAcroPDF1.Name = "AxAcroPDF1"
        Me.AxAcroPDF1.OcxState = CType(resources.GetObject("AxAcroPDF1.OcxState"), System.Windows.Forms.AxHost.State)
        Me.AxAcroPDF1.Size = New System.Drawing.Size(600, 376)
        Me.AxAcroPDF1.TabIndex = 5
        '
        'BtnAbout
        '
        Me.BtnAbout.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
                    Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.BtnAbout.Location = New System.Drawing.Point(528, 40)
        Me.BtnAbout.Name = "BtnAbout"
        Me.BtnAbout.Size = New System.Drawing.Size(80, 24)
        Me.BtnAbout.TabIndex = 6
        Me.BtnAbout.Text = "About"
        '
        'BtnPrinting
        '
        Me.BtnPrinting.Location = New System.Drawing.Point(328, 40)
        Me.BtnPrinting.Name = "BtnPrinting"
        Me.BtnPrinting.Size = New System.Drawing.Size(96, 24)
        Me.BtnPrinting.TabIndex = 7
        Me.BtnPrinting.Text = "Printing"
        '
        'BtnWindow
        '
        Me.BtnWindow.Location = New System.Drawing.Point(216, 40)
        Me.BtnWindow.Name = "BtnWindow"
        Me.BtnWindow.Size = New System.Drawing.Size(104, 24)
        Me.BtnWindow.TabIndex = 8
        Me.BtnWindow.Text = "Window Style"
        '
        'BtnFile
        '
        Me.BtnFile.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.BtnFile.Location = New System.Drawing.Point(8, 40)
        Me.BtnFile.Name = "BtnFile"
        Me.BtnFile.Size = New System.Drawing.Size(96, 24)
        Me.BtnFile.TabIndex = 3
        Me.BtnFile.Text = "Load File"
        '
        'ButtonVersion
        '
        Me.ButtonVersion.Location = New System.Drawing.Point(432, 40)
        Me.ButtonVersion.Name = "ButtonVersion"
        Me.ButtonVersion.Size = New System.Drawing.Size(88, 24)
        Me.ButtonVersion.TabIndex = 9
        Me.ButtonVersion.Text = "Versions"
        '
        'AcrobatPDFWin
        '
        Me.AccessibleRole = System.Windows.Forms.AccessibleRole.Window
        Me.AutoScaleBaseSize = New System.Drawing.Size(7, 15)
        Me.BackColor = System.Drawing.SystemColors.InactiveCaptionText
        Me.ClientSize = New System.Drawing.Size(616, 454)
        Me.Controls.Add(Me.ButtonVersion)
        Me.Controls.Add(Me.BtnWindow)
        Me.Controls.Add(Me.BtnPrinting)
        Me.Controls.Add(Me.BtnAbout)
        Me.Controls.Add(Me.AxAcroPDF1)
        Me.Controls.Add(Me.BtnNavigation)
        Me.Controls.Add(Me.BtnFile)
        Me.Controls.Add(Me.ButtonGoToURL)
        Me.Controls.Add(Me.TextAddress)
        Me.Controls.Add(Me.Label1)
        Me.DockPadding.All = 5
        Me.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.MinimumSize = New System.Drawing.Size(624, 300)
        Me.Name = "AcrobatPDFWin"
        Me.Text = "Acrobat PDF Document Window"
        CType(Me.AxAcroPDF1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub AcrobatPDFWin_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        ' sample wide public variable 
        AcrobatPDFOCX = AxAcroPDF1

        ' initial URL 
        Dim InitURL As String
        InitURL = "http://www.adobe.com/epaper/pdfs/elib_swedish_libraries.pdf"
        Me.TextAddress.Text = InitURL

    End Sub

    Private Sub ButtonGoToURL_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonGoToURL.Click
        ' set AxAcroPDF.src to a URL to open a PDF on internet 
        ' it can be used to open a local file, too.
        AxAcroPDF1.src = Me.TextAddress.Text

    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnFile.Click
        ' get user selected local PDF file 
        Dim FileBox As OpenFileDialog = New OpenFileDialog
        Dim Result As DialogResult = FileBox.ShowDialog()
        Dim FileName As String
        If Result = Windows.Forms.DialogResult.Cancel Then
            Return
        End If
        FileName = FileBox.FileName
        If (FileName = "" OrElse FileName = Nothing) Then
            MessageBox.Show("Invalid file name")
        ElseIf (FileName.EndsWith(".pdf")) Then
            ' call AxAcroPDF.LoadFile to open a PDF in local disk 
            AxAcroPDF1.LoadFile(FileName)
            TextAddress.Text = FileName
        Else
            MessageBox.Show("Invalid PDF file name")
        End If



    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnNavigation.Click
        ' pop up our navigation panel 
        Dim NavPan As NavigatePanel
        NavPan = New NavigatePanel
        NavPan.Show()
    End Sub

    Private Sub BtnPrinting_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles BtnPrinting.Click
        ' pop up our printing panel 
        Dim PrintPan As PrintingPanel
        PrintPan = New PrintingPanel
        PrintPan.Show()
    End Sub

    Private Sub BtnWindow_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles BtnWindow.Click
        ' pop up our window style panel 
        Dim SaylePan As StylePanel
        SaylePan = New StylePanel
        SaylePan.Show()
    End Sub

    Private Sub BtnAbout_Click_1(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnAbout.Click
        ' pop up our about message 
        Dim msg As String
        msg = "AcrobatActiveXVB.NET demonstrates how to use the Acrobat ActiveX control to embed a PDF document window with toolbars in a VB.NET application. It also shows how to implement user graphical interfaces using the ActiveX APIs, so the PDF window can be externally controlled. It was created by the Adobe Acrobat SDK team."
        MsgBox(msg, vbOKOnly, "Acrobat ActiveX Sample")
    End Sub

    Private Sub ButtonVersion_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles ButtonVersion.Click
        Dim msg As String
        msg = AxAcroPDF1.GetVersions()
        MsgBox(msg, vbOKOnly, "Acrobat Versions")
    End Sub
End Class
 