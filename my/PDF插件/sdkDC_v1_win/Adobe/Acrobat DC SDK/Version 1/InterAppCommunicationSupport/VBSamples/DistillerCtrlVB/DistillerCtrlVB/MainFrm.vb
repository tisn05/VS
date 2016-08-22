Option Strict Off
Option Explicit On
Imports VB = Microsoft.VisualBasic
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
	Public WithEvents JobOptsList As System.Windows.Forms.ComboBox
	Public WithEvents OtherJobOpts As System.Windows.Forms.RadioButton
	Public WithEvents DefJobOpts As System.Windows.Forms.RadioButton
	Public WithEvents JobOptions As System.Windows.Forms.GroupBox
	Public WithEvents StatusPercent As System.Windows.Forms.Label
	Public WithEvents StatusPageCount As System.Windows.Forms.Label
	Public WithEvents StatusLabel As System.Windows.Forms.Label
	Public WithEvents Progress As System.Windows.Forms.GroupBox
	Public WithEvents SelectInput As System.Windows.Forms.Button
	Public WithEvents InputFile As System.Windows.Forms.TextBox
	Public WithEvents InptFile As System.Windows.Forms.GroupBox
	Public WithEvents ProcessFile As System.Windows.Forms.Button
	Public WithEvents Quit As System.Windows.Forms.Button
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(MainFrm))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.ToolTip1.Active = True
		Me.JobOptions = New System.Windows.Forms.GroupBox
		Me.JobOptsList = New System.Windows.Forms.ComboBox
		Me.OtherJobOpts = New System.Windows.Forms.RadioButton
		Me.DefJobOpts = New System.Windows.Forms.RadioButton
		Me.Progress = New System.Windows.Forms.GroupBox
		Me.StatusPercent = New System.Windows.Forms.Label
		Me.StatusPageCount = New System.Windows.Forms.Label
		Me.StatusLabel = New System.Windows.Forms.Label
		Me.InptFile = New System.Windows.Forms.GroupBox
		Me.SelectInput = New System.Windows.Forms.Button
		Me.InputFile = New System.Windows.Forms.TextBox
		Me.ProcessFile = New System.Windows.Forms.Button
		Me.Quit = New System.Windows.Forms.Button
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
		Me.Text = "DistillerCtrl"
		Me.ClientSize = New System.Drawing.Size(299, 280)
		Me.Location = New System.Drawing.Point(3, 23)
		Me.ControlBox = False
		Me.Icon = CType(resources.GetObject("MainFrm.Icon"), System.Drawing.Icon)
		Me.MaximizeBox = False
		Me.MinimizeBox = False
		Me.StartPosition = System.Windows.Forms.FormStartPosition.WindowsDefaultLocation
		Me.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.Enabled = True
		Me.KeyPreview = False
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.ShowInTaskbar = True
		Me.HelpButton = False
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "MainFrm"
		Me.JobOptions.Text = " Job Options "
		Me.JobOptions.Size = New System.Drawing.Size(281, 77)
		Me.JobOptions.Location = New System.Drawing.Point(8, 104)
		Me.JobOptions.TabIndex = 9
		Me.JobOptions.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.JobOptions.BackColor = System.Drawing.SystemColors.Control
		Me.JobOptions.Enabled = True
		Me.JobOptions.ForeColor = System.Drawing.SystemColors.ControlText
		Me.JobOptions.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.JobOptions.Visible = True
		Me.JobOptions.Name = "JobOptions"
		Me.JobOptsList.Enabled = False
		Me.JobOptsList.Size = New System.Drawing.Size(127, 21)
		Me.JobOptsList.Location = New System.Drawing.Point(133, 40)
		Me.JobOptsList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
		Me.JobOptsList.TabIndex = 12
		Me.JobOptsList.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.JobOptsList.BackColor = System.Drawing.SystemColors.Window
		Me.JobOptsList.CausesValidation = True
		Me.JobOptsList.ForeColor = System.Drawing.SystemColors.WindowText
		Me.JobOptsList.IntegralHeight = True
		Me.JobOptsList.Cursor = System.Windows.Forms.Cursors.Default
		Me.JobOptsList.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.JobOptsList.Sorted = False
		Me.JobOptsList.TabStop = True
		Me.JobOptsList.Visible = True
		Me.JobOptsList.Name = "JobOptsList"
		Me.OtherJobOpts.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me.OtherJobOpts.Text = "Specify job options:"
		Me.OtherJobOpts.Size = New System.Drawing.Size(113, 17)
		Me.OtherJobOpts.Location = New System.Drawing.Point(16, 42)
		Me.OtherJobOpts.TabIndex = 11
		Me.OtherJobOpts.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.OtherJobOpts.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me.OtherJobOpts.BackColor = System.Drawing.SystemColors.Control
		Me.OtherJobOpts.CausesValidation = True
		Me.OtherJobOpts.Enabled = True
		Me.OtherJobOpts.ForeColor = System.Drawing.SystemColors.ControlText
		Me.OtherJobOpts.Cursor = System.Windows.Forms.Cursors.Default
		Me.OtherJobOpts.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.OtherJobOpts.Appearance = System.Windows.Forms.Appearance.Normal
		Me.OtherJobOpts.TabStop = True
		Me.OtherJobOpts.Checked = False
		Me.OtherJobOpts.Visible = True
		Me.OtherJobOpts.Name = "OtherJobOpts"
		Me.DefJobOpts.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me.DefJobOpts.Text = "Use default job options"
		Me.DefJobOpts.Size = New System.Drawing.Size(129, 17)
		Me.DefJobOpts.Location = New System.Drawing.Point(16, 18)
		Me.DefJobOpts.TabIndex = 10
		Me.DefJobOpts.Checked = True
		Me.DefJobOpts.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.DefJobOpts.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me.DefJobOpts.BackColor = System.Drawing.SystemColors.Control
		Me.DefJobOpts.CausesValidation = True
		Me.DefJobOpts.Enabled = True
		Me.DefJobOpts.ForeColor = System.Drawing.SystemColors.ControlText
		Me.DefJobOpts.Cursor = System.Windows.Forms.Cursors.Default
		Me.DefJobOpts.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.DefJobOpts.Appearance = System.Windows.Forms.Appearance.Normal
		Me.DefJobOpts.TabStop = True
		Me.DefJobOpts.Visible = True
		Me.DefJobOpts.Name = "DefJobOpts"
		Me.Progress.Text = " Progress "
		Me.Progress.Size = New System.Drawing.Size(281, 50)
		Me.Progress.Location = New System.Drawing.Point(8, 190)
		Me.Progress.TabIndex = 4
		Me.Progress.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Progress.BackColor = System.Drawing.SystemColors.Control
		Me.Progress.Enabled = True
		Me.Progress.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Progress.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Progress.Visible = True
		Me.Progress.Name = "Progress"
		Me.StatusPercent.Size = New System.Drawing.Size(41, 17)
		Me.StatusPercent.Location = New System.Drawing.Point(232, 24)
		Me.StatusPercent.TabIndex = 8
		Me.StatusPercent.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.StatusPercent.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.StatusPercent.BackColor = System.Drawing.SystemColors.Control
		Me.StatusPercent.Enabled = True
		Me.StatusPercent.ForeColor = System.Drawing.SystemColors.ControlText
		Me.StatusPercent.Cursor = System.Windows.Forms.Cursors.Default
		Me.StatusPercent.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.StatusPercent.UseMnemonic = True
		Me.StatusPercent.Visible = True
		Me.StatusPercent.AutoSize = False
		Me.StatusPercent.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.StatusPercent.Name = "StatusPercent"
		Me.StatusPageCount.Size = New System.Drawing.Size(65, 17)
		Me.StatusPageCount.Location = New System.Drawing.Point(92, 24)
		Me.StatusPageCount.TabIndex = 7
		Me.StatusPageCount.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.StatusPageCount.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.StatusPageCount.BackColor = System.Drawing.SystemColors.Control
		Me.StatusPageCount.Enabled = True
		Me.StatusPageCount.ForeColor = System.Drawing.SystemColors.ControlText
		Me.StatusPageCount.Cursor = System.Windows.Forms.Cursors.Default
		Me.StatusPageCount.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.StatusPageCount.UseMnemonic = True
		Me.StatusPageCount.Visible = True
		Me.StatusPageCount.AutoSize = False
		Me.StatusPageCount.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.StatusPageCount.Name = "StatusPageCount"
		Me.StatusLabel.Text = "Idle"
		Me.StatusLabel.Size = New System.Drawing.Size(81, 17)
		Me.StatusLabel.Location = New System.Drawing.Point(8, 24)
		Me.StatusLabel.TabIndex = 6
		Me.StatusLabel.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.StatusLabel.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.StatusLabel.BackColor = System.Drawing.SystemColors.Control
		Me.StatusLabel.Enabled = True
		Me.StatusLabel.ForeColor = System.Drawing.SystemColors.ControlText
		Me.StatusLabel.Cursor = System.Windows.Forms.Cursors.Default
		Me.StatusLabel.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.StatusLabel.UseMnemonic = True
		Me.StatusLabel.Visible = True
		Me.StatusLabel.AutoSize = False
		Me.StatusLabel.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.StatusLabel.Name = "StatusLabel"
		Me.InptFile.Text = " Input File "
		Me.InptFile.Size = New System.Drawing.Size(281, 89)
		Me.InptFile.Location = New System.Drawing.Point(8, 8)
		Me.InptFile.TabIndex = 2
		Me.InptFile.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.InptFile.BackColor = System.Drawing.SystemColors.Control
		Me.InptFile.Enabled = True
		Me.InptFile.ForeColor = System.Drawing.SystemColors.ControlText
		Me.InptFile.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.InptFile.Visible = True
		Me.InptFile.Name = "InptFile"
		Me.SelectInput.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.SelectInput.Text = "&Select Input File"
		Me.AcceptButton = Me.SelectInput
		Me.SelectInput.Size = New System.Drawing.Size(105, 25)
		Me.SelectInput.Location = New System.Drawing.Point(8, 53)
		Me.SelectInput.TabIndex = 0
		Me.SelectInput.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.SelectInput.BackColor = System.Drawing.SystemColors.Control
		Me.SelectInput.CausesValidation = True
		Me.SelectInput.Enabled = True
		Me.SelectInput.ForeColor = System.Drawing.SystemColors.ControlText
		Me.SelectInput.Cursor = System.Windows.Forms.Cursors.Default
		Me.SelectInput.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.SelectInput.TabStop = True
		Me.SelectInput.Name = "SelectInput"
		Me.InputFile.AutoSize = False
		Me.InputFile.BackColor = System.Drawing.SystemColors.InactiveBorder
		Me.InputFile.Size = New System.Drawing.Size(265, 19)
		Me.InputFile.Location = New System.Drawing.Point(8, 24)
		Me.InputFile.ReadOnly = True
		Me.InputFile.TabIndex = 5
		Me.InputFile.TabStop = False
		Me.InputFile.Text = "File not yet selected."
		Me.InputFile.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.InputFile.AcceptsReturn = True
		Me.InputFile.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.InputFile.CausesValidation = True
		Me.InputFile.Enabled = True
		Me.InputFile.ForeColor = System.Drawing.SystemColors.WindowText
		Me.InputFile.HideSelection = True
		Me.InputFile.Maxlength = 0
		Me.InputFile.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.InputFile.MultiLine = False
		Me.InputFile.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.InputFile.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.InputFile.Visible = True
		Me.InputFile.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.InputFile.Name = "InputFile"
		Me.ProcessFile.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.ProcessFile.Text = "&Process File"
		Me.ProcessFile.Enabled = False
		Me.ProcessFile.Size = New System.Drawing.Size(73, 25)
		Me.ProcessFile.Location = New System.Drawing.Point(137, 249)
		Me.ProcessFile.TabIndex = 1
		Me.ProcessFile.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.ProcessFile.BackColor = System.Drawing.SystemColors.Control
		Me.ProcessFile.CausesValidation = True
		Me.ProcessFile.ForeColor = System.Drawing.SystemColors.ControlText
		Me.ProcessFile.Cursor = System.Windows.Forms.Cursors.Default
		Me.ProcessFile.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.ProcessFile.TabStop = True
		Me.ProcessFile.Name = "ProcessFile"
		Me.Quit.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.Quit.Text = "&Quit"
		Me.Quit.Size = New System.Drawing.Size(73, 25)
		Me.Quit.Location = New System.Drawing.Point(216, 249)
		Me.Quit.TabIndex = 3
		Me.Quit.Font = New System.Drawing.Font("Arial", 8!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.Quit.BackColor = System.Drawing.SystemColors.Control
		Me.Quit.CausesValidation = True
		Me.Quit.Enabled = True
		Me.Quit.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Quit.Cursor = System.Windows.Forms.Cursors.Default
		Me.Quit.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Quit.TabStop = True
		Me.Quit.Name = "Quit"
		Me.Controls.Add(JobOptions)
		Me.Controls.Add(Progress)
		Me.Controls.Add(InptFile)
		Me.Controls.Add(ProcessFile)
		Me.Controls.Add(Quit)
		Me.JobOptions.Controls.Add(JobOptsList)
		Me.JobOptions.Controls.Add(OtherJobOpts)
		Me.JobOptions.Controls.Add(DefJobOpts)
		Me.Progress.Controls.Add(StatusPercent)
		Me.Progress.Controls.Add(StatusPageCount)
		Me.Progress.Controls.Add(StatusLabel)
		Me.InptFile.Controls.Add(SelectInput)
		Me.InptFile.Controls.Add(InputFile)
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
	
    Private Const ACROBAT_APP As String = "Software\Adobe\Adobe Acrobat\8.0\InstallPath"
	Private Const OPTIONS_EXT As String = ".joboptions"
	Private WithEvents pdfDist As ACRODISTXLib.PdfDistiller
	Private bWorking As Boolean
	
	Private Sub MainFrm_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		
		' Populate jobOptions combobox with available .jobOptions files.
		' If we can't find any, don't allow selection.
		
		PopulateJobOptionsList()
		If (0 = JobOptsList.Items.Count) Then
			OtherJobOpts.Enabled = False
		Else
			JobOptsList.Text = VB6.GetItemString(JobOptsList, 0)
		End If
		
	End Sub
	
	Private Sub PopulateJobOptionsList()
		
		Dim installPath As String
		Dim jobOptionsPath As String
		
		' Get the Acrobat install path and edit to
		' obtain the Distillr\Settings folder
		installPath = RegistryQueryStringValue(HKEY_LOCAL_MACHINE, ACROBAT_APP)
        jobOptionsPath = VB.Left(installPath, InStrRev(installPath, "\")) & "Acrobat\Settings"
		
		' Parse files in \Settings folder and populate
		' JobOptsList with all the .joboptions file names
		FSPopulateComboWithFilesFromFolder(JobOptsList, jobOptionsPath, OPTIONS_EXT)
		
	End Sub
	
	Private Sub DefJobOpts_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles DefJobOpts.CheckedChanged
		If eventSender.Checked Then
			JobOptsList.Enabled = False
		End If
	End Sub
	
	Private Sub OtherJobOpts_CheckedChanged(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles OtherJobOpts.CheckedChanged
		If eventSender.Checked Then
			JobOptsList.Enabled = True
		End If
	End Sub
	
	Private Sub SelectInput_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles SelectInput.Click
		
		' Allow user to select the input file
		Dim path As String

		' Obtain input path from user
		path = FSQueryForFile()
		If Not path = "" Then
			InputFile.Text = path
			ProcessFile.Enabled = True
		End If
		
	End Sub
	
	Private Sub ProcessFile_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ProcessFile.Click
        Dim optionsFile As String
		
		On Error GoTo ErrHandler
		
		' Initialize the automation server
		If (pdfDist Is Nothing) Then
			pdfDist = New ACRODISTXLib.PdfDistiller
			pdfDist.bSpoolJobs = True
		End If
		
		' Prepare to process the file
		StatusLabel.Text = "Processing page"
		StatusPercent.Text = "0%"
		StatusPageCount.Text = "0"
		bWorking = False
		optionsFile = JobOptsList.Text & ".joboptions"
		
		' Here we go...
		pdfDist.FileToPDF(InputFile.Text, "", optionsFile)
		
ErrReturn: 
		Exit Sub
		
ErrHandler: 
		' Doesn't do anything fancy. Just alert user and release server object
        MsgBox("Error occurred while processing file.")
        System.Runtime.InteropServices.Marshal.ReleaseComObject(pdfDist)
		pdfDist = Nothing
		Resume ErrReturn
		
	End Sub
	
	Private Sub Quit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles Quit.Click
		Me.Close()
	End Sub
	
	''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
	' pdfDistiller Event callbacks
	''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
	
	Private Sub pdfDist_OnJobDone(ByVal strInputPostScript As String, ByVal strOutputPDF As String) Handles pdfDist.OnJobDone
        MsgBox(strOutputPDF & " successfully generated.")
    End Sub
	
	Private Sub pdfDist_OnJobFail(ByVal strInputPostScript As String, ByVal strOutputPDF As String) Handles pdfDist.OnJobFail
        MsgBox("Failed to generate " & strOutputPDF)
    End Sub
	
	Private Sub pdfDist_OnPercentDone(ByVal nPercentDone As Integer) Handles pdfDist.OnPercentDone
        Dim percentLabel As String
		
		If nPercentDone > 0 Then
			bWorking = True
		End If
		
		If bWorking = True Then
			
			If (nPercentDone = 0) Then
				' Reset the UI labels
				StatusLabel.Text = "Idle"
				StatusPercent.Text = ""
				StatusPageCount.Text = ""
				bWorking = False
			Else
				
				' Update status label
				percentLabel = "" & nPercentDone & "%"
                StatusPercent.Text = percentLabel
			End If
		End If
		
	End Sub
	
	Private Sub pdfDist_OnPageNumber(ByVal nPageNumber As Integer) Handles pdfDist.OnPageNumber
		
		If bWorking = True Then
			' Update status label
			StatusPageCount.Text = CStr(nPageNumber)
		End If
		
	End Sub
End Class