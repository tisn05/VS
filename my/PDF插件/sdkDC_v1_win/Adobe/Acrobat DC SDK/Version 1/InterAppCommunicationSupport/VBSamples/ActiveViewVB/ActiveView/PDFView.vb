Option Strict Off
Option Explicit On
Imports VB = Microsoft.VisualBasic
Friend Class frmPDFView
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
		'This form is an MDI child.
		'This code simulates the VB6 
		' functionality of automatically
		' loading and showing an MDI
		' child's parent.
        Me.MdiParent = frmMDI.DefInstance
        frmMDI.DefInstance.Show()
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
	Public WithEvents mnuRecentFile As Microsoft.VisualBasic.Compatibility.VB6.MenuItemArray
	Public WithEvents mnuFOpen As System.Windows.Forms.MenuItem
	Public WithEvents mnuFClose As System.Windows.Forms.MenuItem
	Public WithEvents mnuFSave As System.Windows.Forms.MenuItem
	Public WithEvents mnuFSaveAs As System.Windows.Forms.MenuItem
	Public WithEvents mnuFPrint As System.Windows.Forms.MenuItem
	Public WithEvents mnuDSep As System.Windows.Forms.MenuItem
	Public WithEvents mnuDocumentInfo As System.Windows.Forms.MenuItem
	Public WithEvents mnuFSep As System.Windows.Forms.MenuItem
	Public WithEvents mnuFExit As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_0 As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_1 As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_2 As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_3 As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_4 As System.Windows.Forms.MenuItem
	Public WithEvents mnuFile As System.Windows.Forms.MenuItem
	Public WithEvents mnuECopy As System.Windows.Forms.MenuItem
	Public WithEvents mnuEdit As System.Windows.Forms.MenuItem
	Public WithEvents mnuOToolbar As System.Windows.Forms.MenuItem
	Public WithEvents mnuOptions As System.Windows.Forms.MenuItem
	Public WithEvents mnuOHand As System.Windows.Forms.MenuItem
	Public WithEvents mnuTSelectText As System.Windows.Forms.MenuItem
	Public WithEvents mnuTSelectGraphic As System.Windows.Forms.MenuItem
	Public WithEvents mnuTHiliteWithList As System.Windows.Forms.MenuItem
	Public WithEvents mnuTHiliteByRect As System.Windows.Forms.MenuItem
	Public WithEvents mnuTAcquireAnnot As System.Windows.Forms.MenuItem
	Public WithEvents mnuTAddTextAnnot As System.Windows.Forms.MenuItem
	Public WithEvents mnuTools As System.Windows.Forms.MenuItem
	Public WithEvents mnuWCascade As System.Windows.Forms.MenuItem
	Public WithEvents mnuWTile As System.Windows.Forms.MenuItem
	Public WithEvents mnuWArrange As System.Windows.Forms.MenuItem
	Public WithEvents mnuWindow As System.Windows.Forms.MenuItem
	Public MainMenu1 As System.Windows.Forms.MainMenu
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmPDFView))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.ToolTip1.Active = True
		Me.mnuRecentFile = New Microsoft.VisualBasic.Compatibility.VB6.MenuItemArray(components)
		Me.MainMenu1 = New System.Windows.Forms.MainMenu
		Me.mnuFile = New System.Windows.Forms.MenuItem
		Me.mnuFOpen = New System.Windows.Forms.MenuItem
		Me.mnuFClose = New System.Windows.Forms.MenuItem
		Me.mnuFSave = New System.Windows.Forms.MenuItem
		Me.mnuFSaveAs = New System.Windows.Forms.MenuItem
		Me.mnuFPrint = New System.Windows.Forms.MenuItem
		Me.mnuDSep = New System.Windows.Forms.MenuItem
		Me.mnuDocumentInfo = New System.Windows.Forms.MenuItem
		Me.mnuFSep = New System.Windows.Forms.MenuItem
		Me.mnuFExit = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_0 = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_1 = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_2 = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_3 = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_4 = New System.Windows.Forms.MenuItem
		Me.mnuEdit = New System.Windows.Forms.MenuItem
		Me.mnuECopy = New System.Windows.Forms.MenuItem
		Me.mnuOptions = New System.Windows.Forms.MenuItem
		Me.mnuOToolbar = New System.Windows.Forms.MenuItem
		Me.mnuTools = New System.Windows.Forms.MenuItem
		Me.mnuOHand = New System.Windows.Forms.MenuItem
		Me.mnuTSelectText = New System.Windows.Forms.MenuItem
		Me.mnuTSelectGraphic = New System.Windows.Forms.MenuItem
		Me.mnuTHiliteWithList = New System.Windows.Forms.MenuItem
		Me.mnuTHiliteByRect = New System.Windows.Forms.MenuItem
		Me.mnuTAcquireAnnot = New System.Windows.Forms.MenuItem
		Me.mnuTAddTextAnnot = New System.Windows.Forms.MenuItem
		Me.mnuWindow = New System.Windows.Forms.MenuItem
		Me.mnuWCascade = New System.Windows.Forms.MenuItem
		Me.mnuWTile = New System.Windows.Forms.MenuItem
		Me.mnuWArrange = New System.Windows.Forms.MenuItem
		CType(Me.mnuRecentFile, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.BackColor = System.Drawing.SystemColors.Window
		Me.Text = "Untitled"
		Me.ClientSize = New System.Drawing.Size(374, 265)
		Me.Location = New System.Drawing.Point(88, 123)
		Me.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Bold Or System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
		Me.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Icon = CType(resources.GetObject("frmPDFView.Icon"), System.Drawing.Icon)
		Me.StartPosition = System.Windows.Forms.FormStartPosition.WindowsDefaultBounds
		Me.Visible = False
		Me.AutoScaleBaseSize = New System.Drawing.Size(6, 13)
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
		Me.Name = "frmPDFView"
		Me.mnuFile.Text = "&File"
		Me.mnuFile.Checked = False
		Me.mnuFile.Enabled = True
		Me.mnuFile.Visible = True
		Me.mnuFile.MDIList = False
		Me.mnuFOpen.Text = "&Open..."
		Me.mnuFOpen.Checked = False
		Me.mnuFOpen.Enabled = True
		Me.mnuFOpen.Visible = True
		Me.mnuFOpen.MDIList = False
		Me.mnuFClose.Text = "&Close"
		Me.mnuFClose.Checked = False
		Me.mnuFClose.Enabled = True
		Me.mnuFClose.Visible = True
		Me.mnuFClose.MDIList = False
		Me.mnuFSave.Text = "&Save"
		Me.mnuFSave.Checked = False
		Me.mnuFSave.Enabled = True
		Me.mnuFSave.Visible = True
		Me.mnuFSave.MDIList = False
		Me.mnuFSaveAs.Text = "Save &As..."
		Me.mnuFSaveAs.Checked = False
		Me.mnuFSaveAs.Enabled = True
		Me.mnuFSaveAs.Visible = True
		Me.mnuFSaveAs.MDIList = False
		Me.mnuFPrint.Text = "&Print"
		Me.mnuFPrint.Checked = False
		Me.mnuFPrint.Enabled = True
		Me.mnuFPrint.Visible = True
		Me.mnuFPrint.MDIList = False
		Me.mnuDSep.Text = "-"
		Me.mnuDSep.Checked = False
		Me.mnuDSep.Enabled = True
		Me.mnuDSep.Visible = True
		Me.mnuDSep.MDIList = False
		Me.mnuDocumentInfo.Text = "Document &Info"
		Me.mnuDocumentInfo.Checked = False
		Me.mnuDocumentInfo.Enabled = True
		Me.mnuDocumentInfo.Visible = True
		Me.mnuDocumentInfo.MDIList = False
		Me.mnuFSep.Text = "-"
		Me.mnuFSep.Checked = False
		Me.mnuFSep.Enabled = True
		Me.mnuFSep.Visible = True
		Me.mnuFSep.MDIList = False
		Me.mnuFExit.Text = "E&xit"
		Me.mnuFExit.Checked = False
		Me.mnuFExit.Enabled = True
		Me.mnuFExit.Visible = True
		Me.mnuFExit.MDIList = False
		Me._mnuRecentFile_0.Text = "-"
		Me._mnuRecentFile_0.Visible = False
		Me._mnuRecentFile_0.Checked = False
		Me._mnuRecentFile_0.Enabled = True
		Me._mnuRecentFile_0.MDIList = False
		Me._mnuRecentFile_1.Text = "RecentFile1"
		Me._mnuRecentFile_1.Visible = False
		Me._mnuRecentFile_1.Checked = False
		Me._mnuRecentFile_1.Enabled = True
		Me._mnuRecentFile_1.MDIList = False
		Me._mnuRecentFile_2.Text = "RecentFile2"
		Me._mnuRecentFile_2.Visible = False
		Me._mnuRecentFile_2.Checked = False
		Me._mnuRecentFile_2.Enabled = True
		Me._mnuRecentFile_2.MDIList = False
		Me._mnuRecentFile_3.Text = "RecentFile3"
		Me._mnuRecentFile_3.Visible = False
		Me._mnuRecentFile_3.Checked = False
		Me._mnuRecentFile_3.Enabled = True
		Me._mnuRecentFile_3.MDIList = False
		Me._mnuRecentFile_4.Text = "RecentFile4"
		Me._mnuRecentFile_4.Visible = False
		Me._mnuRecentFile_4.Checked = False
		Me._mnuRecentFile_4.Enabled = True
		Me._mnuRecentFile_4.MDIList = False
		Me.mnuEdit.Text = "&Edit"
		Me.mnuEdit.Checked = False
		Me.mnuEdit.Enabled = True
		Me.mnuEdit.Visible = True
		Me.mnuEdit.MDIList = False
		Me.mnuECopy.Text = "&Copy"
		Me.mnuECopy.Checked = False
		Me.mnuECopy.Enabled = True
		Me.mnuECopy.Visible = True
		Me.mnuECopy.MDIList = False
		Me.mnuOptions.Text = "&Options"
		Me.mnuOptions.Checked = False
		Me.mnuOptions.Enabled = True
		Me.mnuOptions.Visible = True
		Me.mnuOptions.MDIList = False
		Me.mnuOToolbar.Text = "&Toolbar"
		Me.mnuOToolbar.Checked = False
		Me.mnuOToolbar.Enabled = True
		Me.mnuOToolbar.Visible = True
		Me.mnuOToolbar.MDIList = False
		Me.mnuTools.Text = "&Tools"
		Me.mnuTools.Checked = False
		Me.mnuTools.Enabled = True
		Me.mnuTools.Visible = True
		Me.mnuTools.MDIList = False
		Me.mnuOHand.Text = "&Hand"
		Me.mnuOHand.Checked = False
		Me.mnuOHand.Enabled = True
		Me.mnuOHand.Visible = True
		Me.mnuOHand.MDIList = False
		Me.mnuTSelectText.Text = "&Select Text"
		Me.mnuTSelectText.Checked = False
		Me.mnuTSelectText.Enabled = True
		Me.mnuTSelectText.Visible = True
		Me.mnuTSelectText.MDIList = False
		Me.mnuTSelectGraphic.Text = "Select &Graphic"
		Me.mnuTSelectGraphic.Checked = False
		Me.mnuTSelectGraphic.Enabled = True
		Me.mnuTSelectGraphic.Visible = True
		Me.mnuTSelectGraphic.MDIList = False
		Me.mnuTHiliteWithList.Text = "Hilite With &List"
		Me.mnuTHiliteWithList.Checked = False
		Me.mnuTHiliteWithList.Enabled = True
		Me.mnuTHiliteWithList.Visible = True
		Me.mnuTHiliteWithList.MDIList = False
		Me.mnuTHiliteByRect.Text = "Hilite By &Rect"
		Me.mnuTHiliteByRect.Checked = False
		Me.mnuTHiliteByRect.Enabled = True
		Me.mnuTHiliteByRect.Visible = True
		Me.mnuTHiliteByRect.MDIList = False
		Me.mnuTAcquireAnnot.Text = "Acquire First &Annotation"
		Me.mnuTAcquireAnnot.Checked = False
		Me.mnuTAcquireAnnot.Enabled = True
		Me.mnuTAcquireAnnot.Visible = True
		Me.mnuTAcquireAnnot.MDIList = False
		Me.mnuTAddTextAnnot.Text = "Add  &Text Annotation"
		Me.mnuTAddTextAnnot.Checked = False
		Me.mnuTAddTextAnnot.Enabled = True
		Me.mnuTAddTextAnnot.Visible = True
		Me.mnuTAddTextAnnot.MDIList = False
		Me.mnuWindow.Text = "&Window"
		Me.mnuWindow.MDIList = True
		Me.mnuWindow.Checked = False
		Me.mnuWindow.Enabled = True
		Me.mnuWindow.Visible = True
		Me.mnuWCascade.Text = "&Cascade"
		Me.mnuWCascade.Checked = False
		Me.mnuWCascade.Enabled = True
		Me.mnuWCascade.Visible = True
		Me.mnuWCascade.MDIList = False
		Me.mnuWTile.Text = "&Tile"
		Me.mnuWTile.Checked = False
		Me.mnuWTile.Enabled = True
		Me.mnuWTile.Visible = True
		Me.mnuWTile.MDIList = False
		Me.mnuWArrange.Text = "&Arrange Icons"
		Me.mnuWArrange.Checked = False
		Me.mnuWArrange.Enabled = True
		Me.mnuWArrange.Visible = True
		Me.mnuWArrange.MDIList = False
		Me.mnuRecentFile.SetIndex(_mnuRecentFile_0, CType(0, Short))
		Me.mnuRecentFile.SetIndex(_mnuRecentFile_1, CType(1, Short))
		Me.mnuRecentFile.SetIndex(_mnuRecentFile_2, CType(2, Short))
		Me.mnuRecentFile.SetIndex(_mnuRecentFile_3, CType(3, Short))
		Me.mnuRecentFile.SetIndex(_mnuRecentFile_4, CType(4, Short))
		CType(Me.mnuRecentFile, System.ComponentModel.ISupportInitialize).EndInit()
		Me.mnuFile.Index = 0
		Me.mnuEdit.Index = 1
		Me.mnuOptions.Index = 2
		Me.mnuTools.Index = 3
		Me.mnuWindow.Index = 4
		MainMenu1.MenuItems.AddRange(New System.Windows.Forms.MenuItem(){Me.mnuFile, Me.mnuEdit, Me.mnuOptions, Me.mnuTools, Me.mnuWindow})
		Me.mnuFOpen.Index = 0
		Me.mnuFClose.Index = 1
		Me.mnuFSave.Index = 2
		Me.mnuFSaveAs.Index = 3
		Me.mnuFPrint.Index = 4
		Me.mnuDSep.Index = 5
		Me.mnuDocumentInfo.Index = 6
		Me.mnuFSep.Index = 7
		Me.mnuFExit.Index = 8
		Me._mnuRecentFile_0.Index = 9
		Me._mnuRecentFile_1.Index = 10
		Me._mnuRecentFile_2.Index = 11
		Me._mnuRecentFile_3.Index = 12
		Me._mnuRecentFile_4.Index = 13
		mnuFile.MenuItems.AddRange(New System.Windows.Forms.MenuItem(){Me.mnuFOpen, Me.mnuFClose, Me.mnuFSave, Me.mnuFSaveAs, Me.mnuFPrint, Me.mnuDSep, Me.mnuDocumentInfo, Me.mnuFSep, Me.mnuFExit, Me._mnuRecentFile_0, Me._mnuRecentFile_1, Me._mnuRecentFile_2, Me._mnuRecentFile_3, Me._mnuRecentFile_4})
		Me.mnuECopy.Index = 0
		mnuEdit.MenuItems.AddRange(New System.Windows.Forms.MenuItem(){Me.mnuECopy})
		Me.mnuOToolbar.Index = 0
		mnuOptions.MenuItems.AddRange(New System.Windows.Forms.MenuItem(){Me.mnuOToolbar})
		Me.mnuOHand.Index = 0
		Me.mnuTSelectText.Index = 1
		Me.mnuTSelectGraphic.Index = 2
		Me.mnuTHiliteWithList.Index = 3
		Me.mnuTHiliteByRect.Index = 4
		Me.mnuTAcquireAnnot.Index = 5
		Me.mnuTAddTextAnnot.Index = 6
		mnuTools.MenuItems.AddRange(New System.Windows.Forms.MenuItem(){Me.mnuOHand, Me.mnuTSelectText, Me.mnuTSelectGraphic, Me.mnuTHiliteWithList, Me.mnuTHiliteByRect, Me.mnuTAcquireAnnot, Me.mnuTAddTextAnnot})
		Me.mnuWCascade.Index = 0
		Me.mnuWTile.Index = 1
		Me.mnuWArrange.Index = 2
		mnuWindow.MenuItems.AddRange(New System.Windows.Forms.MenuItem(){Me.mnuWCascade, Me.mnuWTile, Me.mnuWArrange})
		Me.Menu = MainMenu1
	End Sub
#End Region 
#Region "Upgrade Support "
	Private Shared m_vb6FormDefInstance As frmPDFView
	Private Shared m_InitializingDefInstance As Boolean
	Public Shared Property DefInstance() As frmPDFView
		Get
			If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
				m_InitializingDefInstance = True
				m_vb6FormDefInstance = New frmPDFView()
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
	
	Dim AcroExchPDPage As Acrobat.CAcroPDPage
	
	Private Sub frmPDFView_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		frmMDI.DefInstance.imgPageLeft.Visible = True
		frmMDI.DefInstance.imgPageRight.Visible = True
		frmMDI.DefInstance.imgGotoFirstPage.Visible = True
		frmMDI.DefInstance.imgGotoLastPage.Visible = True
		frmMDI.DefInstance.ImageHand.Visible = True
		frmMDI.DefInstance.imgPrint.Visible = True
		
		nOpenWindows = nOpenWindows + 1
	End Sub
	
	Private Sub frmPDFView_Closing(ByVal eventSender As System.Object, ByVal eventArgs As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
        Dim Cancel As Short = False
        Dim Filename, msg As String
        Dim NL As String
		Dim Response As Short
		
		If FState(CInt(Me.Tag)).Dirty Then
			Filename = Me.Text
			NL = Chr(10) & Chr(13)
            msg = "The text in [" & Filename & "] has changed."
			msg = msg & NL
			msg = msg & "Do you want to save the changes?"
			Response = MsgBox(msg, 51, frmMDI.DefInstance.Text)
			Select Case Response
				' User selects Yes
				Case 6
					'Get the filename to save the file
					Filename = GetFileName()
					'If the user did notspecify a file name,
					'cancel the unload; otherwise, save it.
					If Filename = "" Then
						Cancel = True
					Else
						SaveFileAs((Filename))
					End If
					
					' User selects No
					' Ok to unload
				Case 7
					Cancel = False
					' User selects Cancel
					' Cancel the unload
				Case 2
					Cancel = True
			End Select
		End If
		eventArgs.Cancel = Cancel
	End Sub
	
	Private Sub frmPDFView_Resize(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Resize
		Dim pWndChild, rtn As Integer
		Dim r As rect
		pWndChild = GetWindow(Handle.ToInt32, GW_CHILD)
		If (pWndChild) Then
			rtn = GetClientRect(Handle.ToInt32, r)
			rtn = SetWindowPos(pWndChild, 0, 0, 0, r.Right_Renamed - r.Left_Renamed, r.Bottom - r.Top, SWP_NOZORDER Or SWP_NOMOVE)
		End If
	End Sub
	
	Private Sub frmPDFView_Closed(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Closed
        If Me.Tag = Nothing Then
            Exit Sub
        End If
        AcroExchAVDoc(CInt(Me.Tag)).Close(False)

        FState(CInt(Me.Tag)).Deleted = True
        System.Runtime.InteropServices.Marshal.ReleaseComObject(AcroExchAVDoc(CInt(Me.Tag)))
        AcroExchAVDoc(CInt(Me.Tag)) = Nothing

        'Hide toolbar edit buttons if no notepad windows
        If Not AnyPadsLeft() Then
            frmMDI.DefInstance.imgPageLeft.Visible = False
            frmMDI.DefInstance.imgPageRight.Visible = False
            frmMDI.DefInstance.imgGotoFirstPage.Visible = False
            frmMDI.DefInstance.imgGotoLastPage.Visible = False
            frmMDI.DefInstance.ImageHand.Visible = False
            frmMDI.DefInstance.imgPrint.Visible = False
        End If

        nOpenWindows = nOpenWindows - 1
	End Sub
	
	Public Sub mnuDocumentInfo_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuDocumentInfo.Popup
		mnuDocumentInfo_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuDocumentInfo_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuDocumentInfo.Click
        Dim frm As New frmDocumentInfo
        frmDocumentInfo.DefInstance.ShowDialog()
	End Sub
	
	Public Sub mnuECopy_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuECopy.Popup
		mnuECopy_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuECopy_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuECopy.Click
        Dim r As Integer
		r = AcroExchApp.MenuItemExecute("Copy")
	End Sub
	
	Public Sub mnuFClose_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFClose.Popup
		mnuFClose_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuFClose_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFClose.Click
		Me.Close()
	End Sub
	
	Public Sub mnuFExit_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFExit.Popup
		mnuFExit_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuFExit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFExit.Click
		' Unloading the MDI form invokes the QueryUnload event
		' for each child form, then the MDI form - before unloading
		' the MDI form. Setting the Cancel argument to True in any of the
		' QueryUnload events aborts the unload.
		
		frmMDI.DefInstance.Close()
		
		
	End Sub
	
	Public Sub mnuFOpen_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFOpen.Popup
		mnuFOpen_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuFOpen_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFOpen.Click
		FOpenProc()
	End Sub	
	Public Sub mnuFPrint_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFPrint.Popup
		mnuFPrint_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuFPrint_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFPrint.Click
        Dim frm As New frmPrint
        frmPrint.DefInstance.ShowDialog()
	End Sub
	
	Public Sub mnuFSave_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFSave.Popup
		mnuFSave_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuFSave_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFSave.Click
		Dim Filename As String
		
		If VB.Left(Me.Text, 8) = "Untitled" Then
			' The file hasn't been saved yet,
			' get the filename, then call the
			' save procedure
			Filename = GetFileName()
		Else
			' The caption contains the name of the open file
			Filename = Me.Text
		End If
		' call the save procedure, if Filename = Empty then
		' the user selected Cancel in the Save As dialog, otherwise
		' save the file
		If Filename <> "" Then
			SaveFileAs(Filename)
		End If
	End Sub
	
	Public Sub mnuFSaveAs_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFSaveAs.Popup
		mnuFSaveAs_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuFSaveAs_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFSaveAs.Click
		Dim SaveFileName As String
		
		SaveFileName = GetFileName()
		If SaveFileName <> "" Then SaveFileAs((SaveFileName))
		' Update the recent files menu
		UpdateFileMenu((SaveFileName))
	End Sub
	
	Public Sub mnuOHand_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuOHand.Popup
		mnuOHand_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuOHand_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuOHand.Click
        Dim r As Integer
		r = AcroExchApp.SetActiveTool("Hand", True)
	End Sub
	
	Public Sub mnuOptions_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuOptions.Popup
		mnuOptions_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuOptions_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuOptions.Click
		mnuOToolbar.Checked = frmMDI.DefInstance.picToolbar.Visible
	End Sub
	
	Public Sub mnuOToolbar_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuOToolbar.Popup
		mnuOToolbar_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuOToolbar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuOToolbar.Click
        OptionsToolbarProc(Me, mnuOToolbar.Checked)
	End Sub
	
	Public Sub mnuRecentFile_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuRecentFile.Popup
		mnuRecentFile_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuRecentFile_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuRecentFile.Click
		Dim index As Short = mnuRecentFile.GetIndex(eventSender)
		OpenFile((mnuRecentFile(index).Text))
		' Update recent files list for new notepad.
		GetRecentFiles()
	End Sub
	
	Public Sub mnuTAcquireAnnot_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTAcquireAnnot.Popup
		mnuTAcquireAnnot_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuTAcquireAnnot_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTAcquireAnnot.Click
        Dim Title As String
        Dim Style As MsgBoxStyle
		Dim acroPageView As Acrobat.CAcroAVPageView
		Dim acroPDPage As Acrobat.CAcroPDPage
		Dim acroPDAnnot As Acrobat.CAcroPDAnnot
		Dim numAnnots As Integer
		Dim msg, str_Renamed As String
		
		' Acquire the AVPageView and PDPage
		acroPageView = AcroExchAVDoc(CInt(Me.Tag)).GetAVPageView()
		acroPDPage = acroPageView.GetPage()
		
		' Get the number of annots on the page
		numAnnots = acroPDPage.GetNumAnnots()
		If (numAnnots > 0) Then
			acroPDAnnot = acroPDPage.GetAnnot(0)
			If (acroPDAnnot.IsValid) Then
				str_Renamed = "is valid"
			Else
				str_Renamed = "is not valid"
			End If
			
			msg = "Num Annots on Page = " & numAnnots & "; Annot 0 = " & str_Renamed
		Else
			msg = "There are no annots on the page"
		End If
		Style = MsgBoxStyle.OKOnly
		Title = "PDAnnot Demo"
		MsgBox(msg, Style, Title)
	End Sub
	
	Public Sub mnuTAddTextAnnot_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTAddTextAnnot.Popup
		mnuTAddTextAnnot_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuTAddTextAnnot_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTAddTextAnnot.Click
		Dim avPageView As Acrobat.CAcroAVPageView
		Dim pdPage As Acrobat.CAcroPDPage
		Dim annot As Acrobat.CAcroPDAnnot
		Dim rect As Acrobat.CAcroRect
		Dim time As Acrobat.CAcroTime
		
		' Obtain the AVPageView and PDPage
		avPageView = AcroExchAVDoc(CInt(Me.Tag)).GetAVPageView()
		pdPage = avPageView.GetPage()
		
		' Create an AcroExch.Rect object
		rect = CreateObject("AcroExch.Rect")
		' Set values for rect - increases from right to left and bottom to top
		rect.Left = 200
		rect.Right = 400
		rect.Bottom = 200
		rect.Top = 400
		
		' Add a text annotation (a note) before the first annotation on the page
		annot = pdPage.AddNewAnnot(-1, "Text", rect)
		Dim color As Integer
		If (annot.IsValid) Then
			annot.SetTitle("Brand New Text Annotation")
			annot.SetOpen(True)
			annot.SetContents("This is an annotation of type 'Text'")
			
			' Create an AcroExch.Time object and fill it
			
			time = CreateObject("AcroExch.Time")
			time.Year = 1996
			time.Month = 12
			time.Date = 25
			time.Hour = 12
			time.Minute = 1
			time.Second = 59
			time.millisecond = 2
			time.Day = 2
			annot.SetDate(time)
			
			' Set the color for the annot; R is 1st byte, G is 2nd byte, B is 3rd byte
			' 0 is minimum (black); 255 is maximum (full Red, Green, or Blue)
			color = 255 'Red
			'       color = 65280       'Green
			'       color =  16711680   'Blue
			annot.SetColor(color)
		End If
	End Sub
	
	Public Sub mnuTHiliteByRect_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTHiliteByRect.Popup
		mnuTHiliteByRect_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuTHiliteByRect_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTHiliteByRect.Click
        Dim Title As String
        Dim Style As MsgBoxStyle
        Dim msg As String
        Dim avPageView As Acrobat.CAcroAVPageView
		Dim acroRect As Acrobat.CAcroRect
		Dim pdPage As Acrobat.CAcroPDPage
		Dim pdTextSelect As Acrobat.CAcroPDTextSelect
		Dim pdDoc As Acrobat.CAcroPDDoc
		Dim num As Integer
		Dim str_Renamed As String
		
		' Create a rectangle that will indicate the selection
		acroRect = CreateObject("AcroExch.Rect")
		acroRect.Bottom = 100
		acroRect.Top = 400
		acroRect.Left = 100
		acroRect.Right = 400
		
		' Get the current page number and create a text selection based on the page num and rect
		avPageView = AcroExchAVDoc(CInt(Me.Tag)).GetAVPageView()
		pdDoc = AcroExchAVDoc(CInt(Me.Tag)).GetPDDoc()
		pdPage = avPageView.GetPage()
        num = avPageView.GetPageNum()
		pdTextSelect = pdDoc.CreateTextSelect(num, acroRect)
		
		' Set that as the current text selection and show it
		AcroExchAVDoc(CInt(Me.Tag)).SetTextSelection(pdTextSelect)
		AcroExchAVDoc(CInt(Me.Tag)).ShowTextSelect()
		
		' Get the number of words in the text selection and the first word in selection
		num = pdTextSelect.GetNumText()
		If (num > 0) Then
			str_Renamed = pdTextSelect.GetText(0)
			msg = "Num of words in text select=" & num & "; 1st Word='" & str_Renamed & "'"
		Else
			msg = "There are no words in the text selection"
		End If

		Style = MsgBoxStyle.OKOnly
		Title = "PDTextSelect Demo"
		MsgBox(msg, Style, Title)
		
	End Sub
	
	Public Sub mnuTHiliteWithList_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTHiliteWithList.Popup
		mnuTHiliteWithList_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuTHiliteWithList_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTHiliteWithList.Click
        Dim Title As String
        Dim Style As MsgBoxStyle
        Dim msg As String
		Dim hList As Acrobat.CAcroHiliteList
		Dim pdPage As Acrobat.CAcroPDPage
		Dim pdTextSelect As Acrobat.CAcroPDTextSelect
		Dim avPageView As Acrobat.CAcroAVPageView
		Dim num As Integer
		Dim str_Renamed As String
		
		' Create a list object
		hList = CreateObject("AcroExch.HiliteList")
		hList.Add(0, 10)
		
		' Create either a page or word hilite based on the list
		avPageView = AcroExchAVDoc(CInt(Me.Tag)).GetAVPageView()
		pdPage = avPageView.GetPage()
		'    Set pdTextSelect = pdPage.CreatePageHilite(hList)
		pdTextSelect = pdPage.CreateWordHilite(hList)
		
		' Set that as the current text selection and show it
		AcroExchAVDoc(CInt(Me.Tag)).SetTextSelection(pdTextSelect)
		AcroExchAVDoc(CInt(Me.Tag)).ShowTextSelect()
		
		' Get the number of words in the text selection and the first word in selection
		num = pdTextSelect.GetNumText()
		If (num > 0) Then
			str_Renamed = pdTextSelect.GetText(0)
			msg = "Num of words in text select=" & num & "; 1st Word='" & str_Renamed & "'"
		Else
			msg = "There are no words in the text selection"
        End If

        Style = MsgBoxStyle.OKOnly
        Title = "PDTextSelect Demo"
        MsgBox(msg, Style, Title)
		
	End Sub
	
	Public Sub mnuTSelectGraphic_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTSelectGraphic.Popup
		mnuTSelectGraphic_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuTSelectGraphic_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTSelectGraphic.Click
        Dim r As Integer
		r = AcroExchApp.SetActiveTool("SelectGraphics", True)
	End Sub
	
	Public Sub mnuTSelectText_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTSelectText.Popup
		mnuTSelectText_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuTSelectText_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuTSelectText.Click
        Dim r As Integer
		r = AcroExchApp.SetActiveTool("Select", True)
	End Sub
	
	Public Sub mnuWArrange_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWArrange.Popup
		mnuWArrange_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuWArrange_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWArrange.Click
		frmMDI.DefInstance.LayoutMDI(ARRANGE_ICONS)
	End Sub
	
	Public Sub mnuWCascade_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWCascade.Popup
		mnuWCascade_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuWCascade_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWCascade.Click
		frmMDI.DefInstance.LayoutMDI(CASCADE)
	End Sub
	
	Public Sub mnuWTile_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWTile.Popup
		mnuWTile_Click(eventSender, eventArgs)
	End Sub
	Public Sub mnuWTile_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWTile.Click
		frmMDI.DefInstance.LayoutMDI(TILE_HORIZONTAL)
	End Sub
End Class