Option Strict Off
Option Explicit On
Friend Class QueryDataForm
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
	Public WithEvents SendQuery As System.Windows.Forms.Button
	Public WithEvents SearchString As System.Windows.Forms.TextBox
	Public WithEvents Frame4 As System.Windows.Forms.GroupBox
	Public WithEvents DocumentLimit As System.Windows.Forms.TextBox
	Public WithEvents AcceptDefaultDocLimit As System.Windows.Forms.RadioButton
	Public WithEvents SpecifyDocLimit As System.Windows.Forms.RadioButton
	Public WithEvents Frame3 As System.Windows.Forms.GroupBox
	Public WithEvents QLangType As System.Windows.Forms.ComboBox
	Public WithEvents Frame2 As System.Windows.Forms.GroupBox
	Public WithEvents Refine As System.Windows.Forms.CheckBox
	Public WithEvents Proximity As System.Windows.Forms.CheckBox
	Public WithEvents MatchCase As System.Windows.Forms.CheckBox
	Public WithEvents SoundsLike As System.Windows.Forms.CheckBox
	Public WithEvents Thesaurus As System.Windows.Forms.CheckBox
	Public WithEvents Stemming As System.Windows.Forms.CheckBox
	Public WithEvents CustomWordOptions As System.Windows.Forms.RadioButton
	Public WithEvents DefaultWordOptions As System.Windows.Forms.RadioButton
	Public WithEvents Frame1 As System.Windows.Forms.GroupBox
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me.SendQuery = New System.Windows.Forms.Button
        Me.Frame4 = New System.Windows.Forms.GroupBox
        Me.SearchString = New System.Windows.Forms.TextBox
        Me.Frame3 = New System.Windows.Forms.GroupBox
        Me.DocumentLimit = New System.Windows.Forms.TextBox
        Me.AcceptDefaultDocLimit = New System.Windows.Forms.RadioButton
        Me.SpecifyDocLimit = New System.Windows.Forms.RadioButton
        Me.Frame2 = New System.Windows.Forms.GroupBox
        Me.QLangType = New System.Windows.Forms.ComboBox
        Me.Frame1 = New System.Windows.Forms.GroupBox
        Me.Refine = New System.Windows.Forms.CheckBox
        Me.Proximity = New System.Windows.Forms.CheckBox
        Me.MatchCase = New System.Windows.Forms.CheckBox
        Me.SoundsLike = New System.Windows.Forms.CheckBox
        Me.Thesaurus = New System.Windows.Forms.CheckBox
        Me.Stemming = New System.Windows.Forms.CheckBox
        Me.CustomWordOptions = New System.Windows.Forms.RadioButton
        Me.DefaultWordOptions = New System.Windows.Forms.RadioButton
        Me.Frame4.SuspendLayout()
        Me.Frame3.SuspendLayout()
        Me.Frame2.SuspendLayout()
        Me.Frame1.SuspendLayout()
        Me.SuspendLayout()
        '
        'SendQuery
        '
        Me.SendQuery.BackColor = System.Drawing.SystemColors.Control
        Me.SendQuery.Cursor = System.Windows.Forms.Cursors.Default
        Me.SendQuery.Enabled = False
        Me.SendQuery.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.SendQuery.ForeColor = System.Drawing.SystemColors.ControlText
        Me.SendQuery.Location = New System.Drawing.Point(352, 168)
        Me.SendQuery.Name = "SendQuery"
        Me.SendQuery.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.SendQuery.Size = New System.Drawing.Size(105, 25)
        Me.SendQuery.TabIndex = 13
        Me.SendQuery.Text = "&Send Query"
        '
        'Frame4
        '
        Me.Frame4.BackColor = System.Drawing.SystemColors.Control
        Me.Frame4.Controls.Add(Me.SearchString)
        Me.Frame4.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Frame4.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Frame4.Location = New System.Drawing.Point(200, 8)
        Me.Frame4.Name = "Frame4"
        Me.Frame4.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Frame4.Size = New System.Drawing.Size(257, 49)
        Me.Frame4.TabIndex = 17
        Me.Frame4.TabStop = False
        Me.Frame4.Text = "Search Query"
        '
        'SearchString
        '
        Me.SearchString.AcceptsReturn = True
        Me.SearchString.AutoSize = False
        Me.SearchString.BackColor = System.Drawing.SystemColors.Window
        Me.SearchString.Cursor = System.Windows.Forms.Cursors.IBeam
        Me.SearchString.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.SearchString.ForeColor = System.Drawing.SystemColors.WindowText
        Me.SearchString.Location = New System.Drawing.Point(8, 24)
        Me.SearchString.MaxLength = 0
        Me.SearchString.Name = "SearchString"
        Me.SearchString.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.SearchString.Size = New System.Drawing.Size(241, 19)
        Me.SearchString.TabIndex = 1
        Me.SearchString.Text = ""
        '
        'Frame3
        '
        Me.Frame3.BackColor = System.Drawing.SystemColors.Control
        Me.Frame3.Controls.Add(Me.DocumentLimit)
        Me.Frame3.Controls.Add(Me.AcceptDefaultDocLimit)
        Me.Frame3.Controls.Add(Me.SpecifyDocLimit)
        Me.Frame3.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Frame3.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Frame3.Location = New System.Drawing.Point(304, 64)
        Me.Frame3.Name = "Frame3"
        Me.Frame3.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Frame3.Size = New System.Drawing.Size(153, 97)
        Me.Frame3.TabIndex = 15
        Me.Frame3.TabStop = False
        Me.Frame3.Text = "Max. Number of Docs"
        '
        'DocumentLimit
        '
        Me.DocumentLimit.AcceptsReturn = True
        Me.DocumentLimit.AutoSize = False
        Me.DocumentLimit.BackColor = System.Drawing.SystemColors.Window
        Me.DocumentLimit.Cursor = System.Windows.Forms.Cursors.IBeam
        Me.DocumentLimit.Enabled = False
        Me.DocumentLimit.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.DocumentLimit.ForeColor = System.Drawing.SystemColors.WindowText
        Me.DocumentLimit.Location = New System.Drawing.Point(96, 48)
        Me.DocumentLimit.MaxLength = 0
        Me.DocumentLimit.Name = "DocumentLimit"
        Me.DocumentLimit.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.DocumentLimit.Size = New System.Drawing.Size(49, 19)
        Me.DocumentLimit.TabIndex = 12
        Me.DocumentLimit.Text = "10"
        '
        'AcceptDefaultDocLimit
        '
        Me.AcceptDefaultDocLimit.BackColor = System.Drawing.SystemColors.Control
        Me.AcceptDefaultDocLimit.Checked = True
        Me.AcceptDefaultDocLimit.Cursor = System.Windows.Forms.Cursors.Default
        Me.AcceptDefaultDocLimit.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.AcceptDefaultDocLimit.ForeColor = System.Drawing.SystemColors.ControlText
        Me.AcceptDefaultDocLimit.Location = New System.Drawing.Point(8, 24)
        Me.AcceptDefaultDocLimit.Name = "AcceptDefaultDocLimit"
        Me.AcceptDefaultDocLimit.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.AcceptDefaultDocLimit.Size = New System.Drawing.Size(121, 17)
        Me.AcceptDefaultDocLimit.TabIndex = 10
        Me.AcceptDefaultDocLimit.TabStop = True
        Me.AcceptDefaultDocLimit.Text = "Accept Default Limit"
        '
        'SpecifyDocLimit
        '
        Me.SpecifyDocLimit.BackColor = System.Drawing.SystemColors.Control
        Me.SpecifyDocLimit.Cursor = System.Windows.Forms.Cursors.Default
        Me.SpecifyDocLimit.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.SpecifyDocLimit.ForeColor = System.Drawing.SystemColors.ControlText
        Me.SpecifyDocLimit.Location = New System.Drawing.Point(8, 48)
        Me.SpecifyDocLimit.Name = "SpecifyDocLimit"
        Me.SpecifyDocLimit.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.SpecifyDocLimit.Size = New System.Drawing.Size(89, 17)
        Me.SpecifyDocLimit.TabIndex = 11
        Me.SpecifyDocLimit.TabStop = True
        Me.SpecifyDocLimit.Text = "Specify Limit:"
        '
        'Frame2
        '
        Me.Frame2.BackColor = System.Drawing.SystemColors.Control
        Me.Frame2.Controls.Add(Me.QLangType)
        Me.Frame2.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Frame2.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Frame2.Location = New System.Drawing.Point(8, 8)
        Me.Frame2.Name = "Frame2"
        Me.Frame2.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Frame2.Size = New System.Drawing.Size(185, 49)
        Me.Frame2.TabIndex = 16
        Me.Frame2.TabStop = False
        Me.Frame2.Text = "Query Language Type"
        '
        'QLangType
        '
        Me.QLangType.BackColor = System.Drawing.SystemColors.Window
        Me.QLangType.Cursor = System.Windows.Forms.Cursors.Default
        Me.QLangType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.QLangType.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.QLangType.ForeColor = System.Drawing.SystemColors.WindowText
        Me.QLangType.Location = New System.Drawing.Point(8, 24)
        Me.QLangType.Name = "QLangType"
        Me.QLangType.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.QLangType.Size = New System.Drawing.Size(169, 22)
        Me.QLangType.TabIndex = 0
        '
        'Frame1
        '
        Me.Frame1.BackColor = System.Drawing.SystemColors.Control
        Me.Frame1.Controls.Add(Me.Refine)
        Me.Frame1.Controls.Add(Me.Proximity)
        Me.Frame1.Controls.Add(Me.MatchCase)
        Me.Frame1.Controls.Add(Me.SoundsLike)
        Me.Frame1.Controls.Add(Me.Thesaurus)
        Me.Frame1.Controls.Add(Me.Stemming)
        Me.Frame1.Controls.Add(Me.CustomWordOptions)
        Me.Frame1.Controls.Add(Me.DefaultWordOptions)
        Me.Frame1.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Frame1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Frame1.Location = New System.Drawing.Point(8, 64)
        Me.Frame1.Name = "Frame1"
        Me.Frame1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Frame1.Size = New System.Drawing.Size(289, 97)
        Me.Frame1.TabIndex = 14
        Me.Frame1.TabStop = False
        Me.Frame1.Text = "Word Options"
        '
        'Refine
        '
        Me.Refine.BackColor = System.Drawing.SystemColors.Control
        Me.Refine.Cursor = System.Windows.Forms.Cursors.Default
        Me.Refine.Enabled = False
        Me.Refine.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Refine.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Refine.Location = New System.Drawing.Point(8, 72)
        Me.Refine.Name = "Refine"
        Me.Refine.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Refine.Size = New System.Drawing.Size(97, 17)
        Me.Refine.TabIndex = 6
        Me.Refine.Text = "&Refine Search"
        '
        'Proximity
        '
        Me.Proximity.BackColor = System.Drawing.SystemColors.Control
        Me.Proximity.Cursor = System.Windows.Forms.Cursors.Default
        Me.Proximity.Enabled = False
        Me.Proximity.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Proximity.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Proximity.Location = New System.Drawing.Point(200, 72)
        Me.Proximity.Name = "Proximity"
        Me.Proximity.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Proximity.Size = New System.Drawing.Size(65, 17)
        Me.Proximity.TabIndex = 9
        Me.Proximity.Text = "&Proximity"
        '
        'MatchCase
        '
        Me.MatchCase.BackColor = System.Drawing.SystemColors.Control
        Me.MatchCase.Cursor = System.Windows.Forms.Cursors.Default
        Me.MatchCase.Enabled = False
        Me.MatchCase.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.MatchCase.ForeColor = System.Drawing.SystemColors.ControlText
        Me.MatchCase.Location = New System.Drawing.Point(200, 48)
        Me.MatchCase.Name = "MatchCase"
        Me.MatchCase.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.MatchCase.Size = New System.Drawing.Size(81, 17)
        Me.MatchCase.TabIndex = 8
        Me.MatchCase.Text = "&Match Case"
        '
        'SoundsLike
        '
        Me.SoundsLike.BackColor = System.Drawing.SystemColors.Control
        Me.SoundsLike.Cursor = System.Windows.Forms.Cursors.Default
        Me.SoundsLike.Enabled = False
        Me.SoundsLike.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.SoundsLike.ForeColor = System.Drawing.SystemColors.ControlText
        Me.SoundsLike.Location = New System.Drawing.Point(112, 72)
        Me.SoundsLike.Name = "SoundsLike"
        Me.SoundsLike.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.SoundsLike.Size = New System.Drawing.Size(81, 17)
        Me.SoundsLike.TabIndex = 7
        Me.SoundsLike.Text = "Sounds &Like"
        '
        'Thesaurus
        '
        Me.Thesaurus.BackColor = System.Drawing.SystemColors.Control
        Me.Thesaurus.Cursor = System.Windows.Forms.Cursors.Default
        Me.Thesaurus.Enabled = False
        Me.Thesaurus.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Thesaurus.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Thesaurus.Location = New System.Drawing.Point(112, 48)
        Me.Thesaurus.Name = "Thesaurus"
        Me.Thesaurus.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Thesaurus.Size = New System.Drawing.Size(73, 17)
        Me.Thesaurus.TabIndex = 5
        Me.Thesaurus.Text = "&Thesaurus"
        '
        'Stemming
        '
        Me.Stemming.BackColor = System.Drawing.SystemColors.Control
        Me.Stemming.Cursor = System.Windows.Forms.Cursors.Default
        Me.Stemming.Enabled = False
        Me.Stemming.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Stemming.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Stemming.Location = New System.Drawing.Point(8, 48)
        Me.Stemming.Name = "Stemming"
        Me.Stemming.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Stemming.Size = New System.Drawing.Size(129, 17)
        Me.Stemming.TabIndex = 4
        Me.Stemming.Text = "&Word Stemming"
        '
        'CustomWordOptions
        '
        Me.CustomWordOptions.BackColor = System.Drawing.SystemColors.Control
        Me.CustomWordOptions.Cursor = System.Windows.Forms.Cursors.Default
        Me.CustomWordOptions.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CustomWordOptions.ForeColor = System.Drawing.SystemColors.ControlText
        Me.CustomWordOptions.Location = New System.Drawing.Point(112, 24)
        Me.CustomWordOptions.Name = "CustomWordOptions"
        Me.CustomWordOptions.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.CustomWordOptions.Size = New System.Drawing.Size(97, 17)
        Me.CustomWordOptions.TabIndex = 3
        Me.CustomWordOptions.TabStop = True
        Me.CustomWordOptions.Text = "Specify"
        '
        'DefaultWordOptions
        '
        Me.DefaultWordOptions.BackColor = System.Drawing.SystemColors.Control
        Me.DefaultWordOptions.Checked = True
        Me.DefaultWordOptions.Cursor = System.Windows.Forms.Cursors.Default
        Me.DefaultWordOptions.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.DefaultWordOptions.ForeColor = System.Drawing.SystemColors.ControlText
        Me.DefaultWordOptions.Location = New System.Drawing.Point(8, 24)
        Me.DefaultWordOptions.Name = "DefaultWordOptions"
        Me.DefaultWordOptions.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.DefaultWordOptions.Size = New System.Drawing.Size(97, 17)
        Me.DefaultWordOptions.TabIndex = 2
        Me.DefaultWordOptions.TabStop = True
        Me.DefaultWordOptions.Text = "Accept Defaults"
        '
        'QueryDataForm
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 13)
        Me.BackColor = System.Drawing.SystemColors.Control
        Me.ClientSize = New System.Drawing.Size(465, 201)
        Me.Controls.Add(Me.SendQuery)
        Me.Controls.Add(Me.Frame4)
        Me.Controls.Add(Me.Frame3)
        Me.Controls.Add(Me.Frame2)
        Me.Controls.Add(Me.Frame1)
        Me.Cursor = System.Windows.Forms.Cursors.Default
        Me.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ForeColor = System.Drawing.SystemColors.WindowText
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Location = New System.Drawing.Point(70, 109)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "QueryDataForm"
        Me.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "Query Data Fillout"
        Me.Frame4.ResumeLayout(False)
        Me.Frame3.ResumeLayout(False)
        Me.Frame2.ResumeLayout(False)
        Me.Frame1.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub
#End Region 
#Region "Upgrade Support "
	Private Shared m_vb6FormDefInstance As QueryDataForm
	Private Shared m_InitializingDefInstance As Boolean
	Public Shared Property DefInstance() As QueryDataForm
		Get
			If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
				m_InitializingDefInstance = True
				m_vb6FormDefInstance = New QueryDataForm()
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
	
	Private Sub AcceptDefaultDocLimit_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles AcceptDefaultDocLimit.CheckedChanged
		If eventSender.Checked Then
			DocumentLimit.Enabled = False
		End If
	End Sub
	
	Private Sub SpecifyDocLimit_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles SpecifyDocLimit.CheckedChanged
		If eventSender.Checked Then
			DocumentLimit.Enabled = True
		End If
	End Sub
	
	Private Sub CustomWordOptions_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CustomWordOptions.CheckedChanged
		If eventSender.Checked Then
			
			' Enable the custom word options
			Stemming.Enabled = True
			Thesaurus.Enabled = True
			Refine.Enabled = True
			SoundsLike.Enabled = True
			MatchCase.Enabled = True
			Proximity.Enabled = True
			
		End If
	End Sub
	
	Private Sub DefaultWordOptions_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles DefaultWordOptions.CheckedChanged
		If eventSender.Checked Then
			
			' Disable the custom word options
			Stemming.Enabled = False
			Thesaurus.Enabled = False
			Refine.Enabled = False
			SoundsLike.Enabled = False
			MatchCase.Enabled = False
			Proximity.Enabled = False
		End If
	End Sub
	
	Private Sub QueryDataForm_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		
		' There is a bug with Search when using QLangType_Simple
		' to initiate a search with a QueryData structure - use
		' the Contract example for Simple queries.  The Search plug-in
		' that comes with Acrobat 2.1 has fixed this problem.
		
		QLangType.Items.Insert(0, "Simple Query")
		QLangType.Items.Insert(1, "Support Boolean Logic")
		QLangType.Items.Insert(2, "Verity BooleanPlus")
		QLangType.Text = VB6.GetItemString(QLangType, 0)
		
	End Sub
	
	Private Sub SearchString_TextChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles SearchString.TextChanged
        Dim queryStr As String
		
		' The SendQuery button is only enabled if
		' the user has entered a query string.
		queryStr = SearchString.Text
		If Len(queryStr) <> 0 Then
			SendQuery.Enabled = True
		Else
			SendQuery.Enabled = False
		End If
	End Sub
	
	Private Sub SendQuery_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles SendQuery.Click
        Dim res As Integer
        Dim bOverrideWordOptions As Short
		Dim eQLangTypeSend As Short
        Dim nMaxDocs As Integer
		Dim i As Short
		Dim nWordOptions As Integer
        Dim srchString As String
		
		eQLangTypeSend = QLangType.SelectedIndex
		
		nWordOptions = 0
		bOverrideWordOptions = CustomWordOptions.Checked
		If (bOverrideWordOptions = True) Then
			bOverrideWordOptions = 1
			
			' Compose the word options flags
            If (MatchCase.CheckState = CheckState.Checked) Then
                nWordOptions = nWordOptions + QPON_Case
            End If
            If (Stemming.CheckState = CheckState.Checked) Then
                nWordOptions = nWordOptions + QPON_Stemming
            End If
            If (SoundsLike.CheckState = CheckState.Checked) Then
                nWordOptions = nWordOptions + QPON_SoundsLike
            End If
            If (Thesaurus.CheckState = CheckState.Checked) Then
                nWordOptions = nWordOptions + QPON_Thesaurus
            End If
            If (Proximity.CheckState = CheckState.Checked) Then
                nWordOptions = nWordOptions + QPON_Proximity
            End If
            If (Refine.CheckState = CheckState.Checked) Then
                nWordOptions = nWordOptions + QPON_Refine
            End If
        Else
			bOverrideWordOptions = 0
		End If
		
		' Maximum number of results returned.
		If (SpecifyDocLimit.Checked) Then
            nMaxDocs = CInt(DocumentLimit.Text)
		Else
			nMaxDocs = 0
		End If
		
		srchString = SearchString.Text
		
		On Error GoTo ErrorHandler
		
		' Initialize the DDE Conversation
		SrchDDEInitQuery()
		
		' Set the Query language
		SetQueryLanguageType((eQLangTypeSend))
		
		' Override word options or merge; 0 = merge, 1 = override
		If (bOverrideWordOptions = 1) Then
			
			SetQueryOverideWordOptions()
			' Set the Query Word Options
			SetQueryWordOptions((nWordOptions))
		End If
		
		' Set the maximum number of documents returned.
		If (nMaxDocs > 0) Then
			SetQueryMaxDocs((nMaxDocs))
		End If
		
		' Set the query string
		SetQueryWordTarget((srchString))
		
		' Send the QueryData packet and terminate DDE Conversation
		SrchDDESendQuery()
		Hide()
		res = Shell(buf, AppWinStyle.NormalFocus)
		Exit Sub
		
ErrorHandler: 
		
		' Alert the user of the error. Don't do much else, though.
		MsgBox("The following error occurred:" & vbNewLine & vbNewLine & Err.Description & vbNewLine & vbNewLine & "Make sure that DdeProxy.dll has been configured correctly.")
		Err.Clear()
		
	End Sub
End Class