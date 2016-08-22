Option Strict Off
Option Explicit On 
Imports System
Imports Microsoft.Win32
Friend Class MainFrm
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
	Public WithEvents Quit As System.Windows.Forms.Button
	Public WithEvents DoIndexDialog As System.Windows.Forms.Button
	Public WithEvents DoQueryDialog As System.Windows.Forms.Button
	Public WithEvents Frame1 As System.Windows.Forms.GroupBox
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(MainFrm))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.ToolTip1.Active = True
		Me.Quit = New System.Windows.Forms.Button
		Me.DoIndexDialog = New System.Windows.Forms.Button
		Me.DoQueryDialog = New System.Windows.Forms.Button
		Me.Frame1 = New System.Windows.Forms.GroupBox
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
		Me.Text = "Visual Basic Search Application"
		Me.ClientSize = New System.Drawing.Size(240, 104)
		Me.Location = New System.Drawing.Point(81, 140)
		Me.ForeColor = System.Drawing.SystemColors.WindowText
		Me.MaximizeBox = False
		Me.MinimizeBox = False
		Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
		Me.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.ControlBox = True
		Me.Enabled = True
		Me.KeyPreview = False
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.ShowInTaskbar = True
		Me.HelpButton = False
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "MainFrm"
		Me.Quit.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.Quit.BackColor = System.Drawing.SystemColors.Control
		Me.Quit.Text = "&Quit"
		Me.AcceptButton = Me.Quit
		Me.Quit.Size = New System.Drawing.Size(97, 25)
		Me.Quit.Location = New System.Drawing.Point(72, 72)
		Me.Quit.TabIndex = 2
		Me.Quit.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Quit.CausesValidation = True
		Me.Quit.Enabled = True
		Me.Quit.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Quit.Cursor = System.Windows.Forms.Cursors.Default
		Me.Quit.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Quit.TabStop = True
		Me.Quit.Name = "Quit"
		Me.DoIndexDialog.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.DoIndexDialog.BackColor = System.Drawing.SystemColors.Control
		Me.DoIndexDialog.Text = "Manage &Indices"
		Me.DoIndexDialog.Size = New System.Drawing.Size(97, 25)
		Me.DoIndexDialog.Location = New System.Drawing.Point(128, 32)
		Me.DoIndexDialog.TabIndex = 1
		Me.DoIndexDialog.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.DoIndexDialog.CausesValidation = True
		Me.DoIndexDialog.Enabled = True
		Me.DoIndexDialog.ForeColor = System.Drawing.SystemColors.ControlText
		Me.DoIndexDialog.Cursor = System.Windows.Forms.Cursors.Default
		Me.DoIndexDialog.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.DoIndexDialog.TabStop = True
		Me.DoIndexDialog.Name = "DoIndexDialog"
		Me.DoQueryDialog.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.DoQueryDialog.BackColor = System.Drawing.SystemColors.Control
		Me.DoQueryDialog.Text = "&Perform Query "
		Me.DoQueryDialog.Size = New System.Drawing.Size(97, 25)
		Me.DoQueryDialog.Location = New System.Drawing.Point(16, 32)
		Me.DoQueryDialog.TabIndex = 0
		Me.DoQueryDialog.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.DoQueryDialog.CausesValidation = True
		Me.DoQueryDialog.Enabled = True
		Me.DoQueryDialog.ForeColor = System.Drawing.SystemColors.ControlText
		Me.DoQueryDialog.Cursor = System.Windows.Forms.Cursors.Default
		Me.DoQueryDialog.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.DoQueryDialog.TabStop = True
		Me.DoQueryDialog.Name = "DoQueryDialog"
		Me.Frame1.Text = "Search Options"
		Me.Frame1.Size = New System.Drawing.Size(225, 57)
		Me.Frame1.Location = New System.Drawing.Point(8, 8)
		Me.Frame1.TabIndex = 3
		Me.Frame1.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Frame1.BackColor = System.Drawing.SystemColors.Control
		Me.Frame1.Enabled = True
		Me.Frame1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame1.Visible = True
		Me.Frame1.Name = "Frame1"
		Me.Controls.Add(Quit)
		Me.Controls.Add(DoIndexDialog)
		Me.Controls.Add(DoQueryDialog)
		Me.Controls.Add(Frame1)
	End Sub
#End Region 
#Region "Upgrade Support "
	Private Shared m_vb6FormDefInstance As MainFrm
	Private Shared m_InitializingDefInstance As Boolean
	Public Shared Property DefInstance() As MainFrm
		Get
			If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
				m_InitializingDefInstance = True
				m_vb6FormDefInstance = New MainFrm()
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
	
    Private Const INSTALL_KEY_32 As String = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Acrobat.exe"
    Private Const INSTALL_KEY_64 As String = "SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\App Paths\Acrobat.exe"
	Private Const LAUNCH_ERR As Short = 520
	
	Private Sub DoIndexDialog_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles DoIndexDialog.Click
        Dim myIndexDataForm As New IndexDataForm
        myIndexDataForm.ShowDialog()
	End Sub
	
	Private Sub DoQueryDialog_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles DoQueryDialog.Click
        Dim myQueryDataForm As New QueryDataForm
        myQueryDataForm.ShowDialog()
	End Sub
	
	' Load Acrobat on launch. If this fails, we just bail.
	Private Sub MainFrm_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		buf = New String("0", 255)
		
		On Error GoTo ErrorHandler

        'Determine 32/64bit OS by machine architecture
        Dim arch As Integer = IntPtr.Size

        'Query for the default value stored under the key to install path.
        Dim regKey As RegistryKey
        If arch = 4 Then
            regKey = Registry.LocalMachine.OpenSubKey(INSTALL_KEY_32)
        Else
            regKey = Registry.LocalMachine.OpenSubKey(INSTALL_KEY_64)
        End If

        buf = regKey.GetValue("")
        If buf.Length() = 0 Then
            Err.Raise(LAUNCH_ERR)
        End If

        ' Launch the path that we obtained.
        Shell(buf, AppWinStyle.MinimizedNoFocus)
        Exit Sub

ErrorHandler:
        MsgBox("Failed to launch Acrobat. Aborting...")
        End
    End Sub
	
	' Quit the application
	Private Sub Quit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Quit.Click
		End
	End Sub
End Class