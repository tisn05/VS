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
	Public WithEvents mnuOToolbar As System.Windows.Forms.MenuItem
	Public WithEvents mnuOptions As System.Windows.Forms.MenuItem
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
		Me.mnuDSep = New System.Windows.Forms.MenuItem
		Me.mnuDocumentInfo = New System.Windows.Forms.MenuItem
		Me.mnuFSep = New System.Windows.Forms.MenuItem
		Me.mnuFExit = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_0 = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_1 = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_2 = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_3 = New System.Windows.Forms.MenuItem
		Me._mnuRecentFile_4 = New System.Windows.Forms.MenuItem
		Me.mnuOptions = New System.Windows.Forms.MenuItem
		Me.mnuOToolbar = New System.Windows.Forms.MenuItem
		Me.mnuWindow = New System.Windows.Forms.MenuItem
		Me.mnuWCascade = New System.Windows.Forms.MenuItem
		Me.mnuWTile = New System.Windows.Forms.MenuItem
		Me.mnuWArrange = New System.Windows.Forms.MenuItem
		CType(Me.mnuRecentFile, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.BackColor = System.Drawing.SystemColors.Window
		Me.Text = "Untitled"
		Me.ClientSize = New System.Drawing.Size(374, 265)
		Me.Location = New System.Drawing.Point(77, 135)
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
		Me.mnuOptions.Index = 1
		Me.mnuWindow.Index = 2
		MainMenu1.MenuItems.AddRange(New System.Windows.Forms.MenuItem(){Me.mnuFile, Me.mnuOptions, Me.mnuWindow})
		Me.mnuFOpen.Index = 0
		Me.mnuFClose.Index = 1
		Me.mnuFSave.Index = 2
		Me.mnuFSaveAs.Index = 3
		Me.mnuDSep.Index = 4
		Me.mnuDocumentInfo.Index = 5
		Me.mnuFSep.Index = 6
		Me.mnuFExit.Index = 7
		Me._mnuRecentFile_0.Index = 8
		Me._mnuRecentFile_1.Index = 9
		Me._mnuRecentFile_2.Index = 10
		Me._mnuRecentFile_3.Index = 11
		Me._mnuRecentFile_4.Index = 12
		mnuFile.MenuItems.AddRange(New System.Windows.Forms.MenuItem(){Me.mnuFOpen, Me.mnuFClose, Me.mnuFSave, Me.mnuFSaveAs, Me.mnuDSep, Me.mnuDocumentInfo, Me.mnuFSep, Me.mnuFExit, Me._mnuRecentFile_0, Me._mnuRecentFile_1, Me._mnuRecentFile_2, Me._mnuRecentFile_3, Me._mnuRecentFile_4})
		Me.mnuOToolbar.Index = 0
		mnuOptions.MenuItems.AddRange(New System.Windows.Forms.MenuItem(){Me.mnuOToolbar})
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
	
	Private Sub frmPDFView_Activated(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Activated
		frmMDI.DefInstance.Text1.Text = CStr(FState(CInt(Me.Tag)).nPage + 1)
		If (FState(CInt(Me.Tag)).nScale < 25 Or FState(CInt(Me.Tag)).nScale > 800) Then
			FState(CInt(Me.Tag)).nScale = 100
		End If
		frmMDI.DefInstance.Text2.Text = CStr(FState(CInt(Me.Tag)).nScale)
	End Sub
	
	Private Sub frmPDFView_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		frmMDI.DefInstance.imgPageLeft.Visible = True
		frmMDI.DefInstance.imgPageRight.Visible = True
		
		frmMDI.DefInstance.imgGotoFirstPage.Visible = True
		frmMDI.DefInstance.imgGotoLastPage.Visible = True
		frmMDI.DefInstance.Text1.Visible = True
		
		frmMDI.DefInstance.Label1.Visible = True
		frmMDI.DefInstance.Label2.Visible = True
		frmMDI.DefInstance.Label3.Visible = True
		frmMDI.DefInstance.Text2.Visible = True
	End Sub
	
	Private Sub frmPDFView_Paint(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.PaintEventArgs) Handles MyBase.Paint
        Dim ok As Integer
		Dim acroRect As Acrobat.CAcroRect
        eventArgs.Graphics.Clear(Color.White)
		AcroExchPDPage = AcroExchPDDoc(CInt(Me.Tag)).AcquirePage(FState(CInt(Me.Tag)).nPage)
		acroRect = CreateObject("AcroExch.Rect")
		' UpdateRect is in device space coords.  Origin starts at top left;
		' X increases to right, Y increases down
		acroRect.Top = 792
		acroRect.Bottom = 0
		acroRect.Left = 0
		acroRect.Right = 612
		
        'ok = AcroExchPDPage.DrawEx(Handle.ToInt32, 0, acroRect, 0, 0, FState(CInt(Me.Tag)).nScale)
        ok = AcroExchPDPage.DrawEx(Document(CInt(Me.Tag)).Handle.ToInt32, 0, acroRect, 0, 0, FState(CInt(Me.Tag)).nScale)
        System.Runtime.InteropServices.Marshal.ReleaseComObject(AcroExchPDPage)
		AcroExchPDPage = Nothing
	End Sub
	
    Private Sub frmPDFView_Closing(ByVal eventSender As System.Object, ByVal eventArgs As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
        Dim Cancel As Short = False
        Dim Filename, Msg, NL As String
        Dim Response As Short

        ' see if file needs saving
        Dim PDDocFlags As Integer
        PDDocFlags = AcroExchPDDoc(CInt(frmMDI.DefInstance.ActiveMdiChild.Tag)).GetFlags()

        If (PDDocFlags And 1) = 1 Then
            Filename = Me.Text
            NL = Chr(10) & Chr(13)
            Msg = "The text in [" & Filename & "] has changed."
            Msg = Msg & NL
            Msg = Msg & "Do you want to save the changes?"
            Response = MsgBox(Msg, 51, frmMDI.DefInstance.Text)
            Select Case Response
                ' User selects Yes
                Case 6
                    Dim fsDialog As New SaveFileDialog
                    fsDialog.Filter = "PDF Files (*.pdf)|*.pdf"
                    If fsDialog.ShowDialog = Windows.Forms.DialogResult.OK Then
                        Filename = fsDialog.FileName
                    End If

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

    Private Sub frmPDFView_Closed(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Closed
        FState(CInt(Me.Tag)).Deleted = True
        System.Runtime.InteropServices.Marshal.ReleaseComObject(AcroExchPDDoc(CInt(Me.Tag)))
        AcroExchPDDoc(CInt(Me.Tag)) = Nothing

        'Hide toolbar edit buttons if no notepad windows
        If Not AnyPadsLeft() Then
            frmMDI.DefInstance.imgPageLeft.Visible = False
            frmMDI.DefInstance.imgPageRight.Visible = False

            frmMDI.DefInstance.imgGotoFirstPage.Visible = False
            frmMDI.DefInstance.imgGotoLastPage.Visible = False
            frmMDI.DefInstance.Text1.Visible = False

            frmMDI.DefInstance.Label1.Visible = False
            frmMDI.DefInstance.Label2.Visible = False
            frmMDI.DefInstance.Label3.Visible = False
            frmMDI.DefInstance.Text2.Visible = False
        End If

    End Sub

    Public Sub mnuDocumentInfo_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuDocumentInfo.Popup
        mnuDocumentInfo_Click(eventSender, eventArgs)
    End Sub
    Public Sub mnuDocumentInfo_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuDocumentInfo.Click
        g_nDocumentInfo = Tag
        Dim myfrmDocumentInfo As New frmDocumentInfo
        myfrmDocumentInfo.ShowDialog()
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



    Public Sub mnuFSave_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFSave.Popup
        mnuFSave_Click(eventSender, eventArgs)
    End Sub
    Public Sub mnuFSave_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFSave.Click
        Dim Filename As String

        ' see if file needs saving
        Dim PDDocFlags As Integer
        PDDocFlags = AcroExchPDDoc(CInt(frmMDI.DefInstance.ActiveMdiChild.Tag)).GetFlags()
        If (PDDocFlags And 1) <> 1 Then
            Exit Sub
        End If

        If VB.Left(Me.Text, 8) = "Untitled" Then
            ' The file hasn't been saved yet,
            ' get the filename, then call the
            ' save procedure
            Dim fsDialog As New SaveFileDialog
            fsDialog.Filter = "PDF Files (*.pdf)|*.pdf"
            If fsDialog.ShowDialog = Windows.Forms.DialogResult.OK Then
                Filename = fsDialog.FileName
            Else
                Filename = ""
            End If
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
        Dim fsDialog As New SaveFileDialog
        fsDialog.Filter = "PDF Files (*.pdf)|*.pdf"
        If fsDialog.ShowDialog = Windows.Forms.DialogResult.OK Then
            SaveFileName = fsDialog.FileName
        Else
            SaveFileName = ""
        End If
        If SaveFileName <> "" Then SaveFileAs((SaveFileName))
        ' Update the recent files menu
        UpdateFileMenu((SaveFileName))
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

    Public Sub mnuWArrange_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWArrange.Popup
        mnuWArrange_Click(eventSender, eventArgs)
    End Sub
    Public Sub mnuWArrange_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWArrange.Click
        frmMDI.DefInstance.LayoutMdi(ARRANGE_ICONS)
    End Sub

    Public Sub mnuWCascade_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWCascade.Popup
        mnuWCascade_Click(eventSender, eventArgs)
    End Sub
    Public Sub mnuWCascade_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWCascade.Click
        frmMDI.DefInstance.LayoutMdi(CASCADE)
    End Sub

    Public Sub mnuWTile_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWTile.Popup
        mnuWTile_Click(eventSender, eventArgs)
    End Sub
    Public Sub mnuWTile_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuWTile.Click
        frmMDI.DefInstance.LayoutMdi(TILE_HORIZONTAL)
    End Sub
End Class