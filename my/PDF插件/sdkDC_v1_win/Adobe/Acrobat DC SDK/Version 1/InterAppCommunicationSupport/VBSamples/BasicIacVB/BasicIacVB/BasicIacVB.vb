Option Strict Off
Option Explicit On
Friend Class Form1
	Inherits System.Windows.Forms.Form
#Region "Windows Form Designer generated code "
	Public Sub New()
		MyBase.New()
		If m_vb6FormDefInstance Is Nothing Then
			If m_InitializingDefInstance Then
				m_vb6FormDefInstance = Me
			Else
				Try 
					'For the start-up form, the first instance created is the default instance.
					If System.Reflection.Assembly.GetExecutingAssembly.EntryPoint.DeclaringType Is Me.GetType Then
						m_vb6FormDefInstance = Me
					End If
				Catch
				End Try
			End If
		End If
		'This call is required by the Windows Form Designer.
		InitializeComponent()
	End Sub
	'Form overrides dispose to clean up the component list.
	Protected Overloads Overrides Sub Dispose(ByVal Disposing As Boolean)
		If Disposing Then
			If Not components Is Nothing Then
				components.Dispose()
			End If
		End If
		MyBase.Dispose(Disposing)
	End Sub
	'Required by the Windows Form Designer
	Private components As System.ComponentModel.IContainer
	Public ToolTip1 As System.Windows.Forms.ToolTip
	Public WithEvents Label2 As System.Windows.Forms.Label
	Public WithEvents Label1 As System.Windows.Forms.Label
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(Form1))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.ToolTip1.Active = True
		Me.Label2 = New System.Windows.Forms.Label
		Me.Label1 = New System.Windows.Forms.Label
		Me.Text = "Form1"
		Me.ClientSize = New System.Drawing.Size(476, 202)
		Me.Location = New System.Drawing.Point(4, 23)
		Me.StartPosition = System.Windows.Forms.FormStartPosition.WindowsDefaultLocation
		Me.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable
		Me.ControlBox = True
		Me.Enabled = True
		Me.KeyPreview = False
		Me.MaximizeBox = True
		Me.MinimizeBox = True
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.ShowInTaskbar = True
		Me.HelpButton = False
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "Form1"
		Me.Label2.Text = "Program is over."
		Me.Label2.Font = New System.Drawing.Font("Arial", 12!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label2.ForeColor = System.Drawing.Color.FromARGB(0, 128, 0)
		Me.Label2.Size = New System.Drawing.Size(441, 137)
		Me.Label2.Location = New System.Drawing.Point(16, 48)
		Me.Label2.TabIndex = 1
		Me.Label2.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.Label2.BackColor = System.Drawing.SystemColors.Control
		Me.Label2.Enabled = True
		Me.Label2.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label2.UseMnemonic = True
		Me.Label2.Visible = True
		Me.Label2.AutoSize = False
		Me.Label2.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label2.Name = "Label2"
		Me.Label1.Text = "Sample : BasicIacVB"
		Me.Label1.Font = New System.Drawing.Font("Arial", 18!, System.Drawing.FontStyle.Bold Or System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Label1.Size = New System.Drawing.Size(273, 33)
		Me.Label1.Location = New System.Drawing.Point(104, 8)
		Me.Label1.TabIndex = 0
		Me.Label1.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.Label1.BackColor = System.Drawing.SystemColors.Control
		Me.Label1.Enabled = True
		Me.Label1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label1.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label1.UseMnemonic = True
		Me.Label1.Visible = True
		Me.Label1.AutoSize = False
		Me.Label1.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label1.Name = "Label1"
		Me.Controls.Add(Label2)
		Me.Controls.Add(Label1)
	End Sub
#End Region 
#Region "Upgrade Support "
	Private Shared m_vb6FormDefInstance As Form1
	Private Shared m_InitializingDefInstance As Boolean
	Public Shared Property DefInstance() As Form1
		Get
			If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
				m_InitializingDefInstance = True
				m_vb6FormDefInstance = New Form1()
				m_InitializingDefInstance = False
			End If
			DefInstance = m_vb6FormDefInstance
		End Get
		Set
			m_vb6FormDefInstance = Value
		End Set
	End Property
#End Region 
	' ADOBE SYSTEMS INCORPORATED
	' Copyright (C) 1994-2003 Adobe Systems Incorporated
	' All rights reserved.
	'
	' NOTICE: Adobe permits you to use, modify, and distribute this file
	' in accordance with the terms of the Adobe license agreement
	' accompanying it. If you have received this file from a source other
	' than Adobe, then your use, modification, or distribution of it
	' requires the prior written permission of Adobe.
	'------------------------------------------------------------
	'BasicIacVB
	'- This is a simple Acrobat IAC VB sample. It includes the code
    'to launch Acrobat Viewer, open a PDF file ( IAC\TestFiles\sample.pdf ), and get
	'simple information ( number of pages ). The purpose of the sample is
	'to provide a minimum code for VB IAC developers to get started.
	'It should be easy to add more code to improve and extend the capability.
	'- The way to set up using Acrobat IAC in the project is from the menu
	' Project -> Preferences ... to select Acrobat.tlb.
	'- To run the sample, Acrobat Viewer should not be launched, or it is launched but
	'no any file is loaded. If Acrobat Viewer has launched with open files,
	'Acrobat is locked, and some IAC methods in the code won't work.
	'------------------------------------------------------------
	
	'Acrobat Application project as a global variable
	Dim gApp As Acrobat.CAcroApp
	Private Sub Form1_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		StartAcrobatIac()
	End Sub
	Private Sub StartAcrobatIac()
		
		'IAC objects
		Dim gPdDoc As Acrobat.CAcroPDDoc
		Dim gAvDoc As Acrobat.CAcroAVDoc
		
		'variables
		Dim gPDFPath As String
        Dim sStr As String
		Dim sName As String
		Dim lNum As Integer
		
        'hard-coded for the PDF to open, it can be changed when needed.
        gPDFPath = System.Windows.Forms.Application.StartupPath + "\..\..\..\..\TestFiles\simple.pdf"
		
		'Initialize Acrobat by creating App object
		gApp = CreateObject("AcroExch.App")
		
		' show Acrobat
		gApp.Show()
		
		'Set AVDoc object
        'gAvDoc = gApp.GetActiveDoc
        gAvDoc = CreateObject("AcroExch.AVDoc")
		
		' open the PDF
		If gAvDoc.Open(gPDFPath, "") Then
			
			'Set PDDoc object and get some data
			gPdDoc = gAvDoc.GetPDDoc()
			lNum = gPdDoc.GetNumPages()
			sName = gPdDoc.GetFileName()
			
			'compose a message
            sStr = "PDF file " & sName & " is loaded in Acrobat through IAC program." & vbCrLf & "The PDF document has " & lNum
            If lNum > 1 Then
                sStr += " pages."
            Else
                sStr += " page."
            End If

            Label2.Text = sStr
            ' if failed, show error message
        Else
            Label2.Text = "Cannot open " & gPDFPath & vbCrLf
        End If

    End Sub
	Private Sub Form1_Closed(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Closed
		' finished, quit Acrobat
		If Not gApp Is Nothing Then
			gApp.CloseAllDocs()
			gApp.Exit()
		End If
	End Sub
End Class