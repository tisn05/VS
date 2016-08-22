Option Strict Off
Option Explicit On 

Friend Class frmMDI
    Inherits System.Windows.Forms.Form
    Dim printfrm As frmPrint = Nothing
#Region "Windows Form Designer generated code "
	Public Sub New()
		MyBase.New()
		If m_vb6FormDefInstance Is Nothing Then
			m_vb6FormDefInstance = Me
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
	Public WithEvents imgPrintButtonUp As System.Windows.Forms.PictureBox
	Public WithEvents imgPrintButtonDown As System.Windows.Forms.PictureBox
	Public WithEvents ImageHandUp As System.Windows.Forms.PictureBox
	Public WithEvents ImageHandDn As System.Windows.Forms.PictureBox
	Public WithEvents ImageHand As System.Windows.Forms.PictureBox
	Public WithEvents imgGotoLastPageUp As System.Windows.Forms.PictureBox
	Public WithEvents imgGotoFirstPageUp As System.Windows.Forms.PictureBox
	Public WithEvents imgGotoLastPageDn As System.Windows.Forms.PictureBox
	Public WithEvents imgGotoFirstPageDn As System.Windows.Forms.PictureBox
	Public WithEvents imgGotoLastPage As System.Windows.Forms.PictureBox
	Public WithEvents imgGotoFirstPage As System.Windows.Forms.PictureBox
	Public WithEvents imgPageRightUp As System.Windows.Forms.PictureBox
	Public WithEvents imgPageRight As System.Windows.Forms.PictureBox
	Public WithEvents imgPageRightDn As System.Windows.Forms.PictureBox
	Public WithEvents imgPageLeftUp As System.Windows.Forms.PictureBox
	Public WithEvents ImgPageLeftDn As System.Windows.Forms.PictureBox
	Public WithEvents imgPageLeft As System.Windows.Forms.PictureBox
	Public WithEvents imgFileOpenButtonDn As System.Windows.Forms.PictureBox
	Public WithEvents imgFileOpenButtonUp As System.Windows.Forms.PictureBox
	Public WithEvents imgFileOpenButton As System.Windows.Forms.PictureBox
	Public WithEvents imgPrint As System.Windows.Forms.PictureBox
	Public WithEvents picToolbar As System.Windows.Forms.Panel
	Public WithEvents mnuRecentFile As Microsoft.VisualBasic.Compatibility.VB6.MenuItemArray
	Public WithEvents mnuFOpen As System.Windows.Forms.MenuItem
	Public WithEvents mnuFExit As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_0 As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_1 As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_2 As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_3 As System.Windows.Forms.MenuItem
	Public WithEvents _mnuRecentFile_4 As System.Windows.Forms.MenuItem
	Public WithEvents mnuFile As System.Windows.Forms.MenuItem
	Public WithEvents mnuOToolbar As System.Windows.Forms.MenuItem
	Public WithEvents mnuOptions As System.Windows.Forms.MenuItem
	Public MainMenu1 As System.Windows.Forms.MainMenu
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmMDI))
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me.picToolbar = New System.Windows.Forms.Panel
        Me.imgPrintButtonUp = New System.Windows.Forms.PictureBox
        Me.imgPrintButtonDown = New System.Windows.Forms.PictureBox
        Me.ImageHandUp = New System.Windows.Forms.PictureBox
        Me.ImageHandDn = New System.Windows.Forms.PictureBox
        Me.ImageHand = New System.Windows.Forms.PictureBox
        Me.imgGotoLastPageUp = New System.Windows.Forms.PictureBox
        Me.imgGotoFirstPageUp = New System.Windows.Forms.PictureBox
        Me.imgGotoLastPageDn = New System.Windows.Forms.PictureBox
        Me.imgGotoFirstPageDn = New System.Windows.Forms.PictureBox
        Me.imgGotoLastPage = New System.Windows.Forms.PictureBox
        Me.imgGotoFirstPage = New System.Windows.Forms.PictureBox
        Me.imgPageRightUp = New System.Windows.Forms.PictureBox
        Me.imgPageRight = New System.Windows.Forms.PictureBox
        Me.imgPageRightDn = New System.Windows.Forms.PictureBox
        Me.imgPageLeftUp = New System.Windows.Forms.PictureBox
        Me.ImgPageLeftDn = New System.Windows.Forms.PictureBox
        Me.imgPageLeft = New System.Windows.Forms.PictureBox
        Me.imgFileOpenButtonDn = New System.Windows.Forms.PictureBox
        Me.imgFileOpenButtonUp = New System.Windows.Forms.PictureBox
        Me.imgFileOpenButton = New System.Windows.Forms.PictureBox
        Me.imgPrint = New System.Windows.Forms.PictureBox
        Me.mnuRecentFile = New Microsoft.VisualBasic.Compatibility.VB6.MenuItemArray(Me.components)
        Me._mnuRecentFile_0 = New System.Windows.Forms.MenuItem
        Me._mnuRecentFile_1 = New System.Windows.Forms.MenuItem
        Me._mnuRecentFile_2 = New System.Windows.Forms.MenuItem
        Me._mnuRecentFile_3 = New System.Windows.Forms.MenuItem
        Me._mnuRecentFile_4 = New System.Windows.Forms.MenuItem
        Me.MainMenu1 = New System.Windows.Forms.MainMenu
        Me.mnuFile = New System.Windows.Forms.MenuItem
        Me.mnuFOpen = New System.Windows.Forms.MenuItem
        Me.mnuFExit = New System.Windows.Forms.MenuItem
        Me.mnuOptions = New System.Windows.Forms.MenuItem
        Me.mnuOToolbar = New System.Windows.Forms.MenuItem
        Me.picToolbar.SuspendLayout()
        CType(Me.mnuRecentFile, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'picToolbar
        '
        Me.picToolbar.BackColor = System.Drawing.SystemColors.Control
        Me.picToolbar.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.picToolbar.Controls.Add(Me.imgPrintButtonUp)
        Me.picToolbar.Controls.Add(Me.imgPrintButtonDown)
        Me.picToolbar.Controls.Add(Me.ImageHandUp)
        Me.picToolbar.Controls.Add(Me.ImageHandDn)
        Me.picToolbar.Controls.Add(Me.ImageHand)
        Me.picToolbar.Controls.Add(Me.imgGotoLastPageUp)
        Me.picToolbar.Controls.Add(Me.imgGotoFirstPageUp)
        Me.picToolbar.Controls.Add(Me.imgGotoLastPageDn)
        Me.picToolbar.Controls.Add(Me.imgGotoFirstPageDn)
        Me.picToolbar.Controls.Add(Me.imgGotoLastPage)
        Me.picToolbar.Controls.Add(Me.imgGotoFirstPage)
        Me.picToolbar.Controls.Add(Me.imgPageRightUp)
        Me.picToolbar.Controls.Add(Me.imgPageRight)
        Me.picToolbar.Controls.Add(Me.imgPageRightDn)
        Me.picToolbar.Controls.Add(Me.imgPageLeftUp)
        Me.picToolbar.Controls.Add(Me.ImgPageLeftDn)
        Me.picToolbar.Controls.Add(Me.imgPageLeft)
        Me.picToolbar.Controls.Add(Me.imgFileOpenButtonDn)
        Me.picToolbar.Controls.Add(Me.imgFileOpenButtonUp)
        Me.picToolbar.Controls.Add(Me.imgFileOpenButton)
        Me.picToolbar.Controls.Add(Me.imgPrint)
        Me.picToolbar.Cursor = System.Windows.Forms.Cursors.Default
        Me.picToolbar.Dock = System.Windows.Forms.DockStyle.Top
        Me.picToolbar.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.picToolbar.ForeColor = System.Drawing.SystemColors.WindowText
        Me.picToolbar.Location = New System.Drawing.Point(0, 0)
        Me.picToolbar.Name = "picToolbar"
        Me.picToolbar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.picToolbar.Size = New System.Drawing.Size(656, 25)
        Me.picToolbar.TabIndex = 0
        Me.picToolbar.TabStop = True
        '
        'imgPrintButtonUp
        '
        Me.imgPrintButtonUp.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgPrintButtonUp.Image = CType(resources.GetObject("imgPrintButtonUp.Image"), System.Drawing.Image)
        Me.imgPrintButtonUp.Location = New System.Drawing.Point(329, 0)
        Me.imgPrintButtonUp.Name = "imgPrintButtonUp"
        Me.imgPrintButtonUp.Size = New System.Drawing.Size(24, 22)
        Me.imgPrintButtonUp.TabIndex = 0
        Me.imgPrintButtonUp.TabStop = False
        Me.imgPrintButtonUp.Visible = False
        '
        'imgPrintButtonDown
        '
        Me.imgPrintButtonDown.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgPrintButtonDown.Image = CType(resources.GetObject("imgPrintButtonDown.Image"), System.Drawing.Image)
        Me.imgPrintButtonDown.Location = New System.Drawing.Point(305, 0)
        Me.imgPrintButtonDown.Name = "imgPrintButtonDown"
        Me.imgPrintButtonDown.Size = New System.Drawing.Size(24, 22)
        Me.imgPrintButtonDown.TabIndex = 1
        Me.imgPrintButtonDown.TabStop = False
        Me.imgPrintButtonDown.Visible = False
        '
        'ImageHandUp
        '
        Me.ImageHandUp.Cursor = System.Windows.Forms.Cursors.Default
        Me.ImageHandUp.Image = CType(resources.GetObject("ImageHandUp.Image"), System.Drawing.Image)
        Me.ImageHandUp.Location = New System.Drawing.Point(544, 0)
        Me.ImageHandUp.Name = "ImageHandUp"
        Me.ImageHandUp.Size = New System.Drawing.Size(24, 22)
        Me.ImageHandUp.TabIndex = 2
        Me.ImageHandUp.TabStop = False
        Me.ImageHandUp.Visible = False
        '
        'ImageHandDn
        '
        Me.ImageHandDn.Cursor = System.Windows.Forms.Cursors.Default
        Me.ImageHandDn.Image = CType(resources.GetObject("ImageHandDn.Image"), System.Drawing.Image)
        Me.ImageHandDn.Location = New System.Drawing.Point(568, 0)
        Me.ImageHandDn.Name = "ImageHandDn"
        Me.ImageHandDn.Size = New System.Drawing.Size(24, 22)
        Me.ImageHandDn.TabIndex = 3
        Me.ImageHandDn.TabStop = False
        Me.ImageHandDn.Visible = False
        '
        'ImageHand
        '
        Me.ImageHand.Cursor = System.Windows.Forms.Cursors.Default
        Me.ImageHand.Image = CType(resources.GetObject("ImageHand.Image"), System.Drawing.Image)
        Me.ImageHand.Location = New System.Drawing.Point(160, 0)
        Me.ImageHand.Name = "ImageHand"
        Me.ImageHand.Size = New System.Drawing.Size(24, 22)
        Me.ImageHand.TabIndex = 4
        Me.ImageHand.TabStop = False
        Me.ImageHand.Visible = False
        '
        'imgGotoLastPageUp
        '
        Me.imgGotoLastPageUp.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgGotoLastPageUp.Image = CType(resources.GetObject("imgGotoLastPageUp.Image"), System.Drawing.Image)
        Me.imgGotoLastPageUp.Location = New System.Drawing.Point(520, 0)
        Me.imgGotoLastPageUp.Name = "imgGotoLastPageUp"
        Me.imgGotoLastPageUp.Size = New System.Drawing.Size(24, 22)
        Me.imgGotoLastPageUp.TabIndex = 5
        Me.imgGotoLastPageUp.TabStop = False
        Me.imgGotoLastPageUp.Visible = False
        '
        'imgGotoFirstPageUp
        '
        Me.imgGotoFirstPageUp.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgGotoFirstPageUp.Image = CType(resources.GetObject("imgGotoFirstPageUp.Image"), System.Drawing.Image)
        Me.imgGotoFirstPageUp.Location = New System.Drawing.Point(472, 0)
        Me.imgGotoFirstPageUp.Name = "imgGotoFirstPageUp"
        Me.imgGotoFirstPageUp.Size = New System.Drawing.Size(24, 22)
        Me.imgGotoFirstPageUp.TabIndex = 6
        Me.imgGotoFirstPageUp.TabStop = False
        Me.imgGotoFirstPageUp.Visible = False
        '
        'imgGotoLastPageDn
        '
        Me.imgGotoLastPageDn.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgGotoLastPageDn.Image = CType(resources.GetObject("imgGotoLastPageDn.Image"), System.Drawing.Image)
        Me.imgGotoLastPageDn.Location = New System.Drawing.Point(496, 0)
        Me.imgGotoLastPageDn.Name = "imgGotoLastPageDn"
        Me.imgGotoLastPageDn.Size = New System.Drawing.Size(24, 22)
        Me.imgGotoLastPageDn.TabIndex = 7
        Me.imgGotoLastPageDn.TabStop = False
        Me.imgGotoLastPageDn.Visible = False
        '
        'imgGotoFirstPageDn
        '
        Me.imgGotoFirstPageDn.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgGotoFirstPageDn.Image = CType(resources.GetObject("imgGotoFirstPageDn.Image"), System.Drawing.Image)
        Me.imgGotoFirstPageDn.Location = New System.Drawing.Point(448, 0)
        Me.imgGotoFirstPageDn.Name = "imgGotoFirstPageDn"
        Me.imgGotoFirstPageDn.Size = New System.Drawing.Size(24, 22)
        Me.imgGotoFirstPageDn.TabIndex = 8
        Me.imgGotoFirstPageDn.TabStop = False
        Me.imgGotoFirstPageDn.Visible = False
        '
        'imgGotoLastPage
        '
        Me.imgGotoLastPage.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgGotoLastPage.Image = CType(resources.GetObject("imgGotoLastPage.Image"), System.Drawing.Image)
        Me.imgGotoLastPage.Location = New System.Drawing.Point(128, 0)
        Me.imgGotoLastPage.Name = "imgGotoLastPage"
        Me.imgGotoLastPage.Size = New System.Drawing.Size(24, 22)
        Me.imgGotoLastPage.TabIndex = 9
        Me.imgGotoLastPage.TabStop = False
        Me.imgGotoLastPage.Visible = False
        '
        'imgGotoFirstPage
        '
        Me.imgGotoFirstPage.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgGotoFirstPage.Image = CType(resources.GetObject("imgGotoFirstPage.Image"), System.Drawing.Image)
        Me.imgGotoFirstPage.Location = New System.Drawing.Point(56, 0)
        Me.imgGotoFirstPage.Name = "imgGotoFirstPage"
        Me.imgGotoFirstPage.Size = New System.Drawing.Size(24, 22)
        Me.imgGotoFirstPage.TabIndex = 10
        Me.imgGotoFirstPage.TabStop = False
        Me.imgGotoFirstPage.Visible = False
        '
        'imgPageRightUp
        '
        Me.imgPageRightUp.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgPageRightUp.Image = CType(resources.GetObject("imgPageRightUp.Image"), System.Drawing.Image)
        Me.imgPageRightUp.Location = New System.Drawing.Point(280, 0)
        Me.imgPageRightUp.Name = "imgPageRightUp"
        Me.imgPageRightUp.Size = New System.Drawing.Size(24, 22)
        Me.imgPageRightUp.TabIndex = 11
        Me.imgPageRightUp.TabStop = False
        Me.imgPageRightUp.Visible = False
        '
        'imgPageRight
        '
        Me.imgPageRight.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgPageRight.Image = CType(resources.GetObject("imgPageRight.Image"), System.Drawing.Image)
        Me.imgPageRight.Location = New System.Drawing.Point(104, 0)
        Me.imgPageRight.Name = "imgPageRight"
        Me.imgPageRight.Size = New System.Drawing.Size(24, 22)
        Me.imgPageRight.TabIndex = 12
        Me.imgPageRight.TabStop = False
        Me.imgPageRight.Visible = False
        '
        'imgPageRightDn
        '
        Me.imgPageRightDn.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgPageRightDn.Image = CType(resources.GetObject("imgPageRightDn.Image"), System.Drawing.Image)
        Me.imgPageRightDn.Location = New System.Drawing.Point(256, 0)
        Me.imgPageRightDn.Name = "imgPageRightDn"
        Me.imgPageRightDn.Size = New System.Drawing.Size(24, 22)
        Me.imgPageRightDn.TabIndex = 13
        Me.imgPageRightDn.TabStop = False
        Me.imgPageRightDn.Visible = False
        '
        'imgPageLeftUp
        '
        Me.imgPageLeftUp.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgPageLeftUp.Image = CType(resources.GetObject("imgPageLeftUp.Image"), System.Drawing.Image)
        Me.imgPageLeftUp.Location = New System.Drawing.Point(230, 0)
        Me.imgPageLeftUp.Name = "imgPageLeftUp"
        Me.imgPageLeftUp.Size = New System.Drawing.Size(24, 22)
        Me.imgPageLeftUp.TabIndex = 14
        Me.imgPageLeftUp.TabStop = False
        Me.imgPageLeftUp.Visible = False
        '
        'ImgPageLeftDn
        '
        Me.ImgPageLeftDn.Cursor = System.Windows.Forms.Cursors.Default
        Me.ImgPageLeftDn.Image = CType(resources.GetObject("ImgPageLeftDn.Image"), System.Drawing.Image)
        Me.ImgPageLeftDn.Location = New System.Drawing.Point(207, 0)
        Me.ImgPageLeftDn.Name = "ImgPageLeftDn"
        Me.ImgPageLeftDn.Size = New System.Drawing.Size(24, 22)
        Me.ImgPageLeftDn.TabIndex = 15
        Me.ImgPageLeftDn.TabStop = False
        Me.ImgPageLeftDn.Visible = False
        '
        'imgPageLeft
        '
        Me.imgPageLeft.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgPageLeft.Image = CType(resources.GetObject("imgPageLeft.Image"), System.Drawing.Image)
        Me.imgPageLeft.Location = New System.Drawing.Point(80, 0)
        Me.imgPageLeft.Name = "imgPageLeft"
        Me.imgPageLeft.Size = New System.Drawing.Size(24, 22)
        Me.imgPageLeft.TabIndex = 16
        Me.imgPageLeft.TabStop = False
        Me.imgPageLeft.Visible = False
        '
        'imgFileOpenButtonDn
        '
        Me.imgFileOpenButtonDn.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgFileOpenButtonDn.Image = CType(resources.GetObject("imgFileOpenButtonDn.Image"), System.Drawing.Image)
        Me.imgFileOpenButtonDn.Location = New System.Drawing.Point(392, 0)
        Me.imgFileOpenButtonDn.Name = "imgFileOpenButtonDn"
        Me.imgFileOpenButtonDn.Size = New System.Drawing.Size(24, 22)
        Me.imgFileOpenButtonDn.TabIndex = 17
        Me.imgFileOpenButtonDn.TabStop = False
        Me.imgFileOpenButtonDn.Visible = False
        '
        'imgFileOpenButtonUp
        '
        Me.imgFileOpenButtonUp.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgFileOpenButtonUp.Image = CType(resources.GetObject("imgFileOpenButtonUp.Image"), System.Drawing.Image)
        Me.imgFileOpenButtonUp.Location = New System.Drawing.Point(416, 0)
        Me.imgFileOpenButtonUp.Name = "imgFileOpenButtonUp"
        Me.imgFileOpenButtonUp.Size = New System.Drawing.Size(24, 22)
        Me.imgFileOpenButtonUp.TabIndex = 18
        Me.imgFileOpenButtonUp.TabStop = False
        Me.imgFileOpenButtonUp.Visible = False
        '
        'imgFileOpenButton
        '
        Me.imgFileOpenButton.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgFileOpenButton.Image = CType(resources.GetObject("imgFileOpenButton.Image"), System.Drawing.Image)
        Me.imgFileOpenButton.Location = New System.Drawing.Point(0, 0)
        Me.imgFileOpenButton.Name = "imgFileOpenButton"
        Me.imgFileOpenButton.Size = New System.Drawing.Size(24, 22)
        Me.imgFileOpenButton.TabIndex = 19
        Me.imgFileOpenButton.TabStop = False
        '
        'imgPrint
        '
        Me.imgPrint.Cursor = System.Windows.Forms.Cursors.Default
        Me.imgPrint.Image = CType(resources.GetObject("imgPrint.Image"), System.Drawing.Image)
        Me.imgPrint.Location = New System.Drawing.Point(24, 0)
        Me.imgPrint.Name = "imgPrint"
        Me.imgPrint.Size = New System.Drawing.Size(24, 22)
        Me.imgPrint.TabIndex = 20
        Me.imgPrint.TabStop = False
        Me.imgPrint.Visible = False
        '
        'mnuRecentFile
        '
        '
        '_mnuRecentFile_0
        '
        Me.mnuRecentFile.SetIndex(Me._mnuRecentFile_0, CType(0, Short))
        Me._mnuRecentFile_0.Index = 2
        Me._mnuRecentFile_0.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me._mnuRecentFile_0.Text = "-"
        Me._mnuRecentFile_0.Visible = False
        '
        '_mnuRecentFile_1
        '
        Me.mnuRecentFile.SetIndex(Me._mnuRecentFile_1, CType(1, Short))
        Me._mnuRecentFile_1.Index = 3
        Me._mnuRecentFile_1.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me._mnuRecentFile_1.Text = "RecentFile1"
        Me._mnuRecentFile_1.Visible = False
        '
        '_mnuRecentFile_2
        '
        Me.mnuRecentFile.SetIndex(Me._mnuRecentFile_2, CType(2, Short))
        Me._mnuRecentFile_2.Index = 4
        Me._mnuRecentFile_2.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me._mnuRecentFile_2.Text = "RecentFile2"
        Me._mnuRecentFile_2.Visible = False
        '
        '_mnuRecentFile_3
        '
        Me.mnuRecentFile.SetIndex(Me._mnuRecentFile_3, CType(3, Short))
        Me._mnuRecentFile_3.Index = 5
        Me._mnuRecentFile_3.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me._mnuRecentFile_3.Text = "RecentFile3"
        Me._mnuRecentFile_3.Visible = False
        '
        '_mnuRecentFile_4
        '
        Me.mnuRecentFile.SetIndex(Me._mnuRecentFile_4, CType(4, Short))
        Me._mnuRecentFile_4.Index = 6
        Me._mnuRecentFile_4.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me._mnuRecentFile_4.Text = "RecentFile4"
        Me._mnuRecentFile_4.Visible = False
        '
        'MainMenu1
        '
        Me.MainMenu1.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuFile, Me.mnuOptions})
        '
        'mnuFile
        '
        Me.mnuFile.Index = 0
        Me.mnuFile.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuFOpen, Me.mnuFExit, Me._mnuRecentFile_0, Me._mnuRecentFile_1, Me._mnuRecentFile_2, Me._mnuRecentFile_3, Me._mnuRecentFile_4})
        Me.mnuFile.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me.mnuFile.Text = "&File"
        '
        'mnuFOpen
        '
        Me.mnuFOpen.Index = 0
        Me.mnuFOpen.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me.mnuFOpen.Text = "&Open"
        '
        'mnuFExit
        '
        Me.mnuFExit.Index = 1
        Me.mnuFExit.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me.mnuFExit.Text = "E&xit"
        '
        'mnuOptions
        '
        Me.mnuOptions.Index = 1
        Me.mnuOptions.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuOToolbar})
        Me.mnuOptions.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me.mnuOptions.Text = "&Options"
        '
        'mnuOToolbar
        '
        Me.mnuOToolbar.Index = 0
        Me.mnuOToolbar.MergeType = System.Windows.Forms.MenuMerge.Remove
        Me.mnuOToolbar.Text = "&Toolbar"
        '
        'frmMDI
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.BackColor = System.Drawing.SystemColors.AppWorkspace
        Me.ClientSize = New System.Drawing.Size(656, 512)
        Me.Controls.Add(Me.picToolbar)
        Me.Cursor = System.Windows.Forms.Cursors.Default
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.IsMdiContainer = True
        Me.Location = New System.Drawing.Point(75, 108)
        Me.Menu = Me.MainMenu1
        Me.Name = "frmMDI"
        Me.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.StartPosition = System.Windows.Forms.FormStartPosition.Manual
        Me.Text = "Active PDF View"
        Me.picToolbar.ResumeLayout(False)
        CType(Me.mnuRecentFile, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub
#End Region 
#Region "Upgrade Support "
	Private Shared m_vb6FormDefInstance As frmMDI
	Private Shared m_InitializingDefInstance As Boolean
	Public Shared Property DefInstance() As frmMDI
		Get
			If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
				m_InitializingDefInstance = True
				m_vb6FormDefInstance = New frmMDI()
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
	
	Private Sub ImageHand_MouseDown(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles ImageHand.MouseDown
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		ImageHand.Image = ImageHandDn.Image
	End Sub
	
	Private Sub ImageHand_MouseMove(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles ImageHand.MouseMove
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		' If the button is pressed, display the up bitmap if the
		' mouse is dragged outside the button's area, otherwise
		' display the up bitmap
		Select Case Button
			Case 1
				If x <= 0 Or x > VB6.PixelsToTwipsX(ImageHand.Width) Or Y < 0 Or Y > VB6.PixelsToTwipsY(ImageHand.Height) Then
					ImageHand.Image = ImageHandUp.Image
				Else
					ImageHand.Image = ImageHandDn.Image
				End If
		End Select
	End Sub
	
	Private Sub ImageHand_MouseUp(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles ImageHand.MouseUp
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
        Dim r As Integer
		ImageHand.Image = ImageHandUp.Image
		r = AcroExchApp.SetActiveTool("Hand", True)
	End Sub
	
	Private Sub imgFileOpenButton_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles imgFileOpenButton.Click
		imgFileOpenButton.Refresh()
		FOpenProc()
	End Sub
	
	Private Sub imgFileOpenButton_MouseDown(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgFileOpenButton.MouseDown
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgFileOpenButton.Image = imgFileOpenButtonDn.Image
	End Sub
	
	Private Sub imgFileOpenButton_MouseMove(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgFileOpenButton.MouseMove
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		' If the button is pressed, display the up bitmap if the
		' mouse is dragged outside the button's area, otherwise
		' display the up bitmap
		Select Case Button
			Case 1
				If x <= 0 Or x > VB6.PixelsToTwipsX(imgFileOpenButton.Width) Or Y < 0 Or Y > VB6.PixelsToTwipsY(imgFileOpenButton.Height) Then
					imgFileOpenButton.Image = imgFileOpenButtonUp.Image
				Else
					imgFileOpenButton.Image = imgFileOpenButtonDn.Image
				End If
		End Select
	End Sub
	
	Private Sub imgFileOpenButton_MouseUp(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgFileOpenButton.MouseUp
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgFileOpenButton.Image = imgFileOpenButtonUp.Image
	End Sub
	
	Private Sub imgGotoFirstPage_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles imgGotoFirstPage.Click
        Dim r As Integer
		imgGotoFirstPage.Refresh()
        Dim nForm, nCurPage As Short
		nForm = CShort(frmMDI.DefInstance.ActiveMDIChild.Tag)
		
		Dim AcroAVPageView As Acrobat.CAcroAVPageView
		AcroAVPageView = AcroExchAVDoc(nForm).GetAVPageView()
		
		nCurPage = AcroAVPageView.GetPageNum()
		If (nCurPage = 0) Then
			Exit Sub
		End If
		
		r = AcroAVPageView.GoTo(0)
	End Sub
	
	Private Sub imgGotoFirstPage_MouseDown(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgGotoFirstPage.MouseDown
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgGotoFirstPage.Image = imgGotoFirstPageDn.Image
	End Sub
	
	Private Sub imgGotoFirstPage_MouseMove(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgGotoFirstPage.MouseMove
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		' If the button is pressed, display the up bitmap if the
		' mouse is dragged outside the button's area, otherwise
		' display the up bitmap
		Select Case Button
			Case 1
				If x <= 0 Or x > VB6.PixelsToTwipsX(imgGotoFirstPage.Width) Or Y < 0 Or Y > VB6.PixelsToTwipsY(imgGotoFirstPage.Height) Then
					imgGotoFirstPage.Image = imgGotoFirstPageUp.Image
				Else
					imgGotoFirstPage.Image = imgGotoFirstPageDn.Image
				End If
		End Select
	End Sub
	
	Private Sub imgGotoFirstPage_MouseUp(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgGotoFirstPage.MouseUp
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgGotoFirstPage.Image = imgGotoFirstPageUp.Image
	End Sub
	
	Private Sub imgGotoLastPage_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles imgGotoLastPage.Click
        Dim r As Integer
		imgGotoLastPage.Refresh()
		
		Dim nMaxPage, nForm, nCurPage As Short
		nForm = CShort(frmMDI.DefInstance.ActiveMDIChild.Tag)
		
		Dim AcroAVPageView As Acrobat.CAcroAVPageView
		Dim AcroPDDoc As Acrobat.CAcroPDDoc
		AcroAVPageView = AcroExchAVDoc(nForm).GetAVPageView()
		AcroPDDoc = AcroExchAVDoc(nForm).GetPDDoc()
		
		nMaxPage = AcroPDDoc.GetNumPages()
		nCurPage = AcroAVPageView.GetPageNum()
		If (nCurPage = nMaxPage - 1) Then
			Exit Sub
		End If
		
		r = AcroAVPageView.GoTo(nMaxPage - 1)
	End Sub
	
	Private Sub imgGotoLastPage_MouseDown(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgGotoLastPage.MouseDown
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgGotoLastPage.Image = imgGotoLastPageDn.Image
	End Sub
	
	Private Sub imgGotoLastPage_MouseMove(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgGotoLastPage.MouseMove
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		' If the button is pressed, display the up bitmap if the
		' mouse is dragged outside the button's area, otherwise
		' display the up bitmap
		Select Case Button
			Case 1
				If x <= 0 Or x > VB6.PixelsToTwipsX(imgGotoLastPage.Width) Or Y < 0 Or Y > VB6.PixelsToTwipsY(imgGotoLastPage.Height) Then
					imgGotoLastPage.Image = imgGotoLastPageUp.Image
				Else
					imgGotoLastPage.Image = imgGotoLastPageDn.Image
				End If
		End Select
	End Sub
	
	Private Sub imgGotoLastPage_MouseUp(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgGotoLastPage.MouseUp
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgGotoLastPage.Image = imgGotoLastPageUp.Image
	End Sub
	
	Private Sub imgPageLeft_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles imgPageLeft.Click
        Dim r As Integer
		imgPageLeft.Refresh()
        Dim nForm, nCurPage As Short
		
		nForm = CShort(frmMDI.DefInstance.ActiveMDIChild.Tag)
		
		Dim AcroAVPageView As Acrobat.CAcroAVPageView
		AcroAVPageView = AcroExchAVDoc(nForm).GetAVPageView()
		
		nCurPage = AcroAVPageView.GetPageNum()
		If (nCurPage = 0) Then
			Exit Sub
		End If
		
		r = AcroAVPageView.GoTo(nCurPage - 1)
	End Sub
	
	Private Sub imgPageLeft_MouseDown(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgPageLeft.MouseDown
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgPageLeft.Image = ImgPageLeftDn.Image
	End Sub
	
	Private Sub imgPageLeft_MouseMove(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgPageLeft.MouseMove
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		' If the button is pressed, display the up bitmap if the
		' mouse is dragged outside the button's area, otherwise
		' display the up bitmap
		Select Case Button
			Case 1
				If x <= 0 Or x > VB6.PixelsToTwipsX(imgPageLeft.Width) Or Y < 0 Or Y > VB6.PixelsToTwipsY(imgPageLeft.Height) Then
					imgPageLeft.Image = imgPageLeftUp.Image
				Else
					imgPageLeft.Image = ImgPageLeftDn.Image
				End If
		End Select
		
	End Sub
	
	Private Sub imgPageLeft_MouseUp(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgPageLeft.MouseUp
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgPageLeft.Image = imgPageLeftUp.Image
	End Sub
	
	Private Sub imgPageRight_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles imgPageRight.Click
        Dim r As Integer
		imgPageRight.Refresh()
		
		Dim nMaxPage, nForm, nCurPage As Short
		nForm = CShort(frmMDI.DefInstance.ActiveMDIChild.Tag)
		
		Dim AcroAVPageView As Acrobat.CAcroAVPageView
		Dim AcroPDDoc As Acrobat.CAcroPDDoc
		AcroAVPageView = AcroExchAVDoc(nForm).GetAVPageView()
		AcroPDDoc = AcroExchAVDoc(nForm).GetPDDoc()
		
		nMaxPage = AcroPDDoc.GetNumPages()
		nCurPage = AcroAVPageView.GetPageNum()
		If (nCurPage = nMaxPage - 1) Then
			Exit Sub
		End If
		
		r = AcroAVPageView.GoTo(nCurPage + 1)
	End Sub
	
	Private Sub imgPageRight_MouseDown(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgPageRight.MouseDown
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgPageRight.Image = imgPageRightDn.Image
	End Sub
	
	Private Sub imgPageRight_MouseMove(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgPageRight.MouseMove
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		' If the button is pressed, display the up bitmap if the
		' mouse is dragged outside the button's area, otherwise
		' display the up bitmap
		Select Case Button
			Case 1
				If x <= 0 Or x > VB6.PixelsToTwipsX(imgPageRight.Width) Or Y < 0 Or Y > VB6.PixelsToTwipsY(imgPageRight.Height) Then
					imgPageRight.Image = imgPageRightUp.Image
				Else
					imgPageRight.Image = imgPageRightDn.Image
				End If
		End Select
	End Sub
	
	Private Sub imgPageRight_MouseUp(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgPageRight.MouseUp
		Dim Button As Short = eventArgs.Button \ &H100000
		Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
		Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
		Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
		imgPageRight.Image = imgPageRightUp.Image
	End Sub
	
	Private Sub imgPrint_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles imgPrint.Click
		imgPrint.Refresh()
        Dim printfrm As New frmPrint
        frmPrint.DefInstance.ShowDialog()
    End Sub

    Private Sub imgPrint_MouseDown(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgPrint.MouseDown
        Dim Button As Short = eventArgs.Button \ &H100000
        Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
        Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
        Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
        imgPrint.Image = imgPrintButtonDown.Image
    End Sub

    Private Sub imgPrint_MouseMove(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgPrint.MouseMove
        Dim Button As Short = eventArgs.Button \ &H100000
        Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
        Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
        Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
        ' If the button is pressed, display the up bitmap if the
        ' mouse is dragged outside the button's area, otherwise
        ' display the up bitmap
        Select Case Button
            Case 1
                If x <= 0 Or x > VB6.PixelsToTwipsX(imgPrint.Width) Or Y < 0 Or Y > VB6.PixelsToTwipsY(imgPrint.Height) Then
                    imgPrint.Image = imgPrintButtonUp.Image
                Else
                    imgPrint.Image = imgPrintButtonDown.Image
                End If
        End Select
    End Sub

    Private Sub imgPrint_MouseUp(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.MouseEventArgs) Handles imgPrint.MouseUp
        Dim Button As Short = eventArgs.Button \ &H100000
        Dim Shift As Short = System.Windows.Forms.Control.ModifierKeys \ &H10000
        Dim x As Single = VB6.PixelsToTwipsX(eventArgs.X)
        Dim Y As Single = VB6.PixelsToTwipsY(eventArgs.Y)
        imgPrint.Image = imgPrintButtonUp.Image
    End Sub

    Private Sub imgPrintButton_Click()

    End Sub

    Private Sub frmMDI_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
        Dim r As Integer
        ' Application starts here (Load event of Startup form).
        Show()

        AcroExchApp = CreateObject("AcroExch.App")
        r = AcroExchApp.Lock("ActiveViewVB")
        If Not r Then
            System.Runtime.InteropServices.Marshal.ReleaseComObject(AcroExchApp)
            AcroExchApp = Nothing
            MsgBox("ActiveViewVB cannot run at this time.  The viewer is in use.")
            End
        End If

        ' Always set working directory to directory containing the application.
        ChDir(Application.StartupPath())

        'Initialize document form arrays, and show first document.
        ReDim Document(1)
        ReDim FState(1)
        ReDim AcroExchAVDoc(1)
        FState(1).Deleted = True
        nOpenWindows = 0

        ' Read MDINOTE.INI and set recent file menu items appropriately
        GetRecentFiles()
    End Sub

    Private Sub frmMDI_Closed(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Closed
        ' If the Unload was not canceled (in the QueryUnload events for the Notepad forms)
        ' there will be no document windows left, so go ahead and end the application.

        If Not AnyPadsLeft() Then
            AcroExchApp.Unlock()
            End
        End If

    End Sub

    Public Sub mnuFExit_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFExit.Popup
        mnuFExit_Click(eventSender, eventArgs)
    End Sub
    Public Sub mnuFExit_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFExit.Click
        End
    End Sub

    Public Sub mnuFOpen_Popup(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFOpen.Popup
        mnuFOpen_Click(eventSender, eventArgs)
    End Sub
    Public Sub mnuFOpen_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles mnuFOpen.Click
        FOpenProc()
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
        ' Update recent files list.
        GetRecentFiles()
    End Sub
End Class