Option Strict Off
Option Explicit On
Friend Class IndexDataForm
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
	Public WithEvents Close_Renamed As System.Windows.Forms.Button
	Public WithEvents IndexAction As System.Windows.Forms.ComboBox
	Public WithEvents ExecOp As System.Windows.Forms.Button
	Public WithEvents TempIndexName As System.Windows.Forms.TextBox
	Public WithEvents Frame3 As System.Windows.Forms.GroupBox
	Public WithEvents SelectIndex As System.Windows.Forms.Button
	Public WithEvents IndexPath As System.Windows.Forms.TextBox
	Public WithEvents Frame2 As System.Windows.Forms.GroupBox
	Public WithEvents Frame1 As System.Windows.Forms.GroupBox
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(IndexDataForm))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.ToolTip1.Active = True
		Me.Close_Renamed = New System.Windows.Forms.Button
		Me.IndexAction = New System.Windows.Forms.ComboBox
		Me.ExecOp = New System.Windows.Forms.Button
		Me.Frame3 = New System.Windows.Forms.GroupBox
		Me.TempIndexName = New System.Windows.Forms.TextBox
		Me.Frame2 = New System.Windows.Forms.GroupBox
		Me.SelectIndex = New System.Windows.Forms.Button
		Me.IndexPath = New System.Windows.Forms.TextBox
		Me.Frame1 = New System.Windows.Forms.GroupBox
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
		Me.Text = "Index Data Fillout"
		Me.ClientSize = New System.Drawing.Size(376, 184)
		Me.Location = New System.Drawing.Point(111, 101)
		Me.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Bold Or System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.ForeColor = System.Drawing.SystemColors.WindowText
		Me.MaximizeBox = False
		Me.MinimizeBox = False
		Me.ShowInTaskbar = False
		Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
		Me.AutoScaleBaseSize = New System.Drawing.Size(6, 13)
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.ControlBox = True
		Me.Enabled = True
		Me.KeyPreview = False
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.HelpButton = False
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "IndexDataForm"
		Me.Close_Renamed.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.Close_Renamed.Text = "&Close"
		Me.Close_Renamed.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Close_Renamed.Size = New System.Drawing.Size(86, 25)
		Me.Close_Renamed.Location = New System.Drawing.Point(283, 152)
		Me.Close_Renamed.TabIndex = 8
		Me.Close_Renamed.BackColor = System.Drawing.SystemColors.Control
		Me.Close_Renamed.CausesValidation = True
		Me.Close_Renamed.Enabled = True
		Me.Close_Renamed.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Close_Renamed.Cursor = System.Windows.Forms.Cursors.Default
		Me.Close_Renamed.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Close_Renamed.TabStop = True
		Me.Close_Renamed.Name = "Close_Renamed"
		Me.IndexAction.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.IndexAction.Size = New System.Drawing.Size(161, 21)
		Me.IndexAction.Location = New System.Drawing.Point(16, 120)
		Me.IndexAction.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
		Me.IndexAction.TabIndex = 1
		Me.IndexAction.BackColor = System.Drawing.SystemColors.Window
		Me.IndexAction.CausesValidation = True
		Me.IndexAction.Enabled = True
		Me.IndexAction.ForeColor = System.Drawing.SystemColors.WindowText
		Me.IndexAction.IntegralHeight = True
		Me.IndexAction.Cursor = System.Windows.Forms.Cursors.Default
		Me.IndexAction.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.IndexAction.Sorted = False
		Me.IndexAction.TabStop = True
		Me.IndexAction.Visible = True
		Me.IndexAction.Name = "IndexAction"
		Me.ExecOp.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.ExecOp.BackColor = System.Drawing.SystemColors.Control
		Me.ExecOp.Text = "&Perform Operation"
		Me.ExecOp.Enabled = False
		Me.ExecOp.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.ExecOp.Size = New System.Drawing.Size(113, 25)
		Me.ExecOp.Location = New System.Drawing.Point(152, 152)
		Me.ExecOp.TabIndex = 3
		Me.ExecOp.CausesValidation = True
		Me.ExecOp.ForeColor = System.Drawing.SystemColors.ControlText
		Me.ExecOp.Cursor = System.Windows.Forms.Cursors.Default
		Me.ExecOp.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.ExecOp.TabStop = True
		Me.ExecOp.Name = "ExecOp"
		Me.Frame3.Text = "Temporary Name"
		Me.Frame3.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Frame3.Size = New System.Drawing.Size(177, 49)
		Me.Frame3.Location = New System.Drawing.Point(192, 96)
		Me.Frame3.TabIndex = 7
		Me.Frame3.BackColor = System.Drawing.SystemColors.Control
		Me.Frame3.Enabled = True
		Me.Frame3.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame3.Visible = True
		Me.Frame3.Name = "Frame3"
		Me.TempIndexName.AutoSize = False
		Me.TempIndexName.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.TempIndexName.Size = New System.Drawing.Size(161, 19)
		Me.TempIndexName.Location = New System.Drawing.Point(8, 24)
		Me.TempIndexName.TabIndex = 2
		Me.TempIndexName.Text = "IndexName"
		Me.TempIndexName.AcceptsReturn = True
		Me.TempIndexName.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.TempIndexName.BackColor = System.Drawing.SystemColors.Window
		Me.TempIndexName.CausesValidation = True
		Me.TempIndexName.Enabled = True
		Me.TempIndexName.ForeColor = System.Drawing.SystemColors.WindowText
		Me.TempIndexName.HideSelection = True
		Me.TempIndexName.ReadOnly = False
		Me.TempIndexName.Maxlength = 0
		Me.TempIndexName.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.TempIndexName.MultiLine = False
		Me.TempIndexName.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.TempIndexName.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.TempIndexName.TabStop = True
		Me.TempIndexName.Visible = True
		Me.TempIndexName.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.TempIndexName.Name = "TempIndexName"
		Me.Frame2.Text = "Index Path"
		Me.Frame2.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Frame2.Size = New System.Drawing.Size(361, 81)
		Me.Frame2.Location = New System.Drawing.Point(8, 8)
		Me.Frame2.TabIndex = 6
		Me.Frame2.BackColor = System.Drawing.SystemColors.Control
		Me.Frame2.Enabled = True
		Me.Frame2.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame2.Visible = True
		Me.Frame2.Name = "Frame2"
		Me.SelectIndex.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.SelectIndex.Text = "Select &Index"
		Me.SelectIndex.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.SelectIndex.Size = New System.Drawing.Size(97, 25)
		Me.SelectIndex.Location = New System.Drawing.Point(8, 48)
		Me.SelectIndex.TabIndex = 0
		Me.SelectIndex.BackColor = System.Drawing.SystemColors.Control
		Me.SelectIndex.CausesValidation = True
		Me.SelectIndex.Enabled = True
		Me.SelectIndex.ForeColor = System.Drawing.SystemColors.ControlText
		Me.SelectIndex.Cursor = System.Windows.Forms.Cursors.Default
		Me.SelectIndex.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.SelectIndex.TabStop = True
		Me.SelectIndex.Name = "SelectIndex"
		Me.IndexPath.AutoSize = False
		Me.IndexPath.BackColor = System.Drawing.SystemColors.Control
		Me.IndexPath.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.IndexPath.Size = New System.Drawing.Size(345, 19)
		Me.IndexPath.Location = New System.Drawing.Point(8, 24)
		Me.IndexPath.ReadOnly = True
		Me.IndexPath.TabIndex = 4
		Me.IndexPath.TabStop = False
		Me.IndexPath.Text = "Index file not yet selected."
		Me.IndexPath.AcceptsReturn = True
		Me.IndexPath.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.IndexPath.CausesValidation = True
		Me.IndexPath.Enabled = True
		Me.IndexPath.ForeColor = System.Drawing.SystemColors.WindowText
		Me.IndexPath.HideSelection = True
		Me.IndexPath.Maxlength = 0
		Me.IndexPath.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.IndexPath.MultiLine = False
		Me.IndexPath.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.IndexPath.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.IndexPath.Visible = True
		Me.IndexPath.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.IndexPath.Name = "IndexPath"
		Me.Frame1.Text = "Operation"
		Me.Frame1.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Frame1.Size = New System.Drawing.Size(177, 49)
		Me.Frame1.Location = New System.Drawing.Point(8, 96)
		Me.Frame1.TabIndex = 5
		Me.Frame1.BackColor = System.Drawing.SystemColors.Control
		Me.Frame1.Enabled = True
		Me.Frame1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame1.Visible = True
		Me.Frame1.Name = "Frame1"
		Me.Controls.Add(Close_Renamed)
		Me.Controls.Add(IndexAction)
		Me.Controls.Add(ExecOp)
		Me.Controls.Add(Frame3)
		Me.Controls.Add(Frame2)
		Me.Controls.Add(Frame1)
		Me.Frame3.Controls.Add(TempIndexName)
		Me.Frame2.Controls.Add(SelectIndex)
		Me.Frame2.Controls.Add(IndexPath)
	End Sub
#End Region 
#Region "Upgrade Support "
	Private Shared m_vb6FormDefInstance As IndexDataForm
	Private Shared m_InitializingDefInstance As Boolean
	Public Shared Property DefInstance() As IndexDataForm
		Get
			If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
				m_InitializingDefInstance = True
				m_vb6FormDefInstance = New IndexDataForm()
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
	
	Private Sub Command1_Click()
		
	End Sub
	
	Private Sub Close_Renamed_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Close_Renamed.Click
		Hide()
	End Sub
	
	Private Sub ExecOp_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ExecOp.Click
		
		On Error GoTo ErrorHandler
		
		' Initialize the DDE operation.
		SrchDDEInitIndex()
		
		' Set the Index Action type.
		SetIndexAction((IndexAction.SelectedIndex))
		
		' Set the Index pathname.
		SetIndexName((IndexPath.Text))
		
		' Set the Index temporary name.
		SetIndexTempName((TempIndexName.Text))
		
		' Send the IndexData packet and terminate DDE operation.
		SrchDDESendIndex()
		
		MsgBox("Operation performed successfully.")
		Exit Sub
		
ErrorHandler: 
		
		' Alert the user of the error. Don't do much else, though.
		MsgBox("The following error occurred:" & vbNewLine & vbNewLine & Err.Description & vbNewLine & vbNewLine & "Make sure that DdeProxy.dll has been configured correctly.")
		
	End Sub
	
	Private Sub IndexDataForm_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		
		' Populate the action list.
		IndexAction.Items.Insert(0, "Add")
		IndexAction.Items.Insert(1, "Remove")
		IndexAction.Items.Insert(2, "Enable")
		IndexAction.Items.Insert(3, "Disable")
		
		IndexAction.Text = VB6.GetItemString(IndexAction, 0)
		
	End Sub
	
	Private Sub SelectIndex_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles SelectIndex.Click
		
		' Allow user to select an index file.
		Dim path As String
		
		' Obtain input path from user.
		path = FSQueryForFile(Me)
		If Not path = "" Then
			IndexPath.Text = path
			ExecOp.Enabled = True
		End If
	End Sub
End Class