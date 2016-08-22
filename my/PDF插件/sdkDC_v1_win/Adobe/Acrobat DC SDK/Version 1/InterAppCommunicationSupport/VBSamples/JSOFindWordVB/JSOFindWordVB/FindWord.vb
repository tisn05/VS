Option Strict Off
Option Explicit On
Friend Class FindTextForm
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
    Public WithEvents CloseBtn As System.Windows.Forms.Button
	Public WithEvents FindBtn As System.Windows.Forms.Button
	Public WithEvents InputText As System.Windows.Forms.TextBox
	Public WithEvents OpenFileBtn As System.Windows.Forms.Button
    Public WithEvents LabelInfo As System.Windows.Forms.Label
    Friend WithEvents Label1 As System.Windows.Forms.Label
	Public WithEvents pdfName As System.Windows.Forms.Label
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me.CloseBtn = New System.Windows.Forms.Button
        Me.FindBtn = New System.Windows.Forms.Button
        Me.InputText = New System.Windows.Forms.TextBox
        Me.OpenFileBtn = New System.Windows.Forms.Button
        Me.LabelInfo = New System.Windows.Forms.Label
        Me.pdfName = New System.Windows.Forms.Label
        Me.Label1 = New System.Windows.Forms.Label
        Me.SuspendLayout()
        '
        'CloseBtn
        '
        Me.CloseBtn.BackColor = System.Drawing.SystemColors.Control
        Me.CloseBtn.Cursor = System.Windows.Forms.Cursors.Default
        Me.CloseBtn.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CloseBtn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.CloseBtn.Location = New System.Drawing.Point(243, 86)
        Me.CloseBtn.Name = "CloseBtn"
        Me.CloseBtn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.CloseBtn.Size = New System.Drawing.Size(73, 25)
        Me.CloseBtn.TabIndex = 4
        Me.CloseBtn.Text = "Close"
        Me.CloseBtn.UseVisualStyleBackColor = False
        '
        'FindBtn
        '
        Me.FindBtn.BackColor = System.Drawing.SystemColors.Control
        Me.FindBtn.Cursor = System.Windows.Forms.Cursors.Default
        Me.FindBtn.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FindBtn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.FindBtn.Location = New System.Drawing.Point(219, 55)
        Me.FindBtn.Name = "FindBtn"
        Me.FindBtn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.FindBtn.Size = New System.Drawing.Size(97, 25)
        Me.FindBtn.TabIndex = 3
        Me.FindBtn.Text = "Search"
        Me.FindBtn.UseVisualStyleBackColor = False
        '
        'InputText
        '
        Me.InputText.AcceptsReturn = True
        Me.InputText.BackColor = System.Drawing.SystemColors.Window
        Me.InputText.Cursor = System.Windows.Forms.Cursors.IBeam
        Me.InputText.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.InputText.ForeColor = System.Drawing.Color.Blue
        Me.InputText.Location = New System.Drawing.Point(12, 55)
        Me.InputText.MaxLength = 0
        Me.InputText.Name = "InputText"
        Me.InputText.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.InputText.Size = New System.Drawing.Size(201, 20)
        Me.InputText.TabIndex = 2
        Me.InputText.Text = " "
        '
        'OpenFileBtn
        '
        Me.OpenFileBtn.BackColor = System.Drawing.SystemColors.Control
        Me.OpenFileBtn.Cursor = System.Windows.Forms.Cursors.Default
        Me.OpenFileBtn.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.OpenFileBtn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.OpenFileBtn.Location = New System.Drawing.Point(219, 12)
        Me.OpenFileBtn.Name = "OpenFileBtn"
        Me.OpenFileBtn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.OpenFileBtn.Size = New System.Drawing.Size(97, 25)
        Me.OpenFileBtn.TabIndex = 0
        Me.OpenFileBtn.Text = "Browse..."
        Me.OpenFileBtn.UseVisualStyleBackColor = False
        '
        'LabelInfo
        '
        Me.LabelInfo.BackColor = System.Drawing.Color.FromArgb(CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer))
        Me.LabelInfo.Cursor = System.Windows.Forms.Cursors.Default
        Me.LabelInfo.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LabelInfo.ForeColor = System.Drawing.Color.FromArgb(CType(CType(0, Byte), Integer), CType(CType(128, Byte), Integer), CType(CType(0, Byte), Integer))
        Me.LabelInfo.Location = New System.Drawing.Point(12, 86)
        Me.LabelInfo.Name = "LabelInfo"
        Me.LabelInfo.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.LabelInfo.Size = New System.Drawing.Size(193, 17)
        Me.LabelInfo.TabIndex = 5
        '
        'pdfName
        '
        Me.pdfName.BackColor = System.Drawing.Color.FromArgb(CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer))
        Me.pdfName.Cursor = System.Windows.Forms.Cursors.Default
        Me.pdfName.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.pdfName.ForeColor = System.Drawing.Color.Blue
        Me.pdfName.Location = New System.Drawing.Point(12, 13)
        Me.pdfName.Name = "pdfName"
        Me.pdfName.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.pdfName.Size = New System.Drawing.Size(201, 16)
        Me.pdfName.TabIndex = 1
        Me.pdfName.Text = "No PDF Open"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(12, 37)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(91, 15)
        Me.Label1.TabIndex = 7
        Me.Label1.Text = "Word to Find"
        '
        'FindTextForm
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(8, 14)
        Me.BackColor = System.Drawing.Color.FromArgb(CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer))
        Me.ClientSize = New System.Drawing.Size(324, 121)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.CloseBtn)
        Me.Controls.Add(Me.FindBtn)
        Me.Controls.Add(Me.InputText)
        Me.Controls.Add(Me.OpenFileBtn)
        Me.Controls.Add(Me.LabelInfo)
        Me.Controls.Add(Me.pdfName)
        Me.Cursor = System.Windows.Forms.Cursors.Default
        Me.Font = New System.Drawing.Font("Courier New", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ForeColor = System.Drawing.Color.Blue
        Me.Location = New System.Drawing.Point(4, 23)
        Me.Name = "FindTextForm"
        Me.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.StartPosition = System.Windows.Forms.FormStartPosition.Manual
        Me.Text = "Find Text In PDF"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
#End Region 
#Region "Upgrade Support "
	Private Shared m_vb6FormDefInstance As FindTextForm
	Private Shared m_InitializingDefInstance As Boolean
	Public Shared Property DefInstance() As FindTextForm
		Get
			If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
				m_InitializingDefInstance = True
				m_vb6FormDefInstance = New FindTextForm()
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
	' FindTextDorm.frm
	' VB project "VBjsoFindWord" is a Acrobat VB JavaScript Object sample.
	' There is a dialog for the user to select a PDF and input a word to find.
	' After each occurance is found, a dialog pops up to ask the user if continue the search.
	' Final message will show the result: number of occurances found.
	'
	' The approach is to get the Acrobat JavaScript Object from the PDDoc
	' of a PDF file first, after that you can call most Javascript methods.
	' The search is case sensitive in this code.
	' If Acrobat was already running with PDF opened before, then closing this VB
	' program will not quit Acrobat.
	'
	' Note that this is only a sample to show how to use Acrobat JavaScript Object in VB.
	' It is not supposed to work as a  complete utility tool. Further improvements are needed
	' to make it to be a robust program. Now the sample finds out English words only. If you need
	' it to work with other languages such as Korean, Chinese, or others, you need to localize
	' the VB project to enable write, show, and handle those characters in Unicode.
	'------------------------------------------------------------
	
	'declare global variables
	Dim gApp As Acrobat.CAcroApp
	Dim gPdDoc As Acrobat.CAcroPDDoc
	Dim gAvDoc As Acrobat.CAcroAVDoc
	Dim gPDFPath As String
	
	'flag for Acrobat.
	'true:if Acrobat was already running with PDF opened before. false: otherwise
	Dim gExisting As Boolean
	
	Private Sub FindTextForm_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		
        Dim numOpenPDFs As Short
		
		'Initialize Acrobat by creating App object
		'If Acrobat is running, it will get the existing acrobat object.
		gApp = CreateObject("AcroExch.App")
		
		' show Acrobat
		gApp.Show()
		
		' is a PDF already open?
		numOpenPDFs = gApp.GetNumAVDocs
		
		'if there is a PDF opened, get doc object
		If numOpenPDFs > 0 Then
			gExisting = True
			gAvDoc = gApp.GetActiveDoc
			gPdDoc = gAvDoc.GetPDDoc
			
			'show file name
			pdfName.Text = gAvDoc.GetTitle
		Else
			gExisting = False
		End If
		
		InputText.Text = ""
		LabelInfo.Text = ""
		
	End Sub
	
	
	Private Sub FindTextForm_Closed(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Closed
		
		'End Acrobat only if it was not existing before
		'or it is existing but no PDF file opened.
		If Not gApp Is Nothing Then
			If gExisting = False Then
				gApp.CloseAllDocs()
				gApp.Exit()
			End If
		End If
		
        ' clean
        If Not gApp Is Nothing Then
            System.Runtime.InteropServices.Marshal.ReleaseComObject(gApp)
            gApp = Nothing
        End If
        If Not gPdDoc Is Nothing Then
            System.Runtime.InteropServices.Marshal.ReleaseComObject(gPdDoc)
            gPdDoc = Nothing
        End If
        If Not gAvDoc Is Nothing Then
            System.Runtime.InteropServices.Marshal.ReleaseComObject(gAvDoc)
            gAvDoc = Nothing
        End If

        End
		
	End Sub
	
	Private Sub CloseBtn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CloseBtn.Click
		FindTextForm_Closed(Me, New System.EventArgs())
	End Sub
	
	Private Sub OpenFileBtn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles OpenFileBtn.Click
        Dim foundErr As Boolean
		
		' open file box for user to select a PDF file
        gPDFPath = vbNullString

        Dim CommonDialog1 As New OpenFileDialog

        CommonDialog1.Filter = "PDF Files (*.pdf)|*.pdf"
        If CommonDialog1.ShowDialog = Windows.Forms.DialogResult.OK Then
            gPDFPath = CommonDialog1.FileName
        End If

        'If a file is selected, try to open it
        Dim fname As String
        If Len(gPDFPath) > 0 Then

            'Show acribat if it's hidden
            gApp.Show()

            ' create PDDoc object
            foundErr = False
            gPdDoc = CreateObject("AcroExch.PDDoc")

            ' open the PDF in acrobat
            If gPdDoc.Open(gPDFPath) Then
                fname = gPdDoc.GetFileName
                gPdDoc.OpenAVDoc(fname)
                pdfName.Text = fname
            Else
                MsgBox("Failed to open " & gPDFPath)
            End If
        End If

    End Sub
	
	Private Sub FindBtn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles FindBtn.Click
        Dim inumOpenPDFs As Integer
		
		' is a PDF already open?
		inumOpenPDFs = gApp.GetNumAVDocs
		
		'there must be a PDF file opened
		If gApp.GetNumAVDocs > 0 Then
			
			'get the Doc.
			gAvDoc = gApp.GetActiveDoc
			gPdDoc = gAvDoc.GetPDDoc
			
			'show file name
			pdfName.Text = gAvDoc.GetTitle
			
			'there must be a word input.
			If Len(InputText.Text) = 0 Then
				MsgBox("Please input a word")
			Else
				'OK, go search....
				FindWordJSO()
			End If
		Else
			'warning message
			pdfName.Text = ""
			MsgBox("Please open a PDF file.")
		End If
		
	End Sub
	
	
	Private Sub FindWordJSO()
        Dim gbStop As Boolean
		
		'local variables
		Dim jso As Object
		Dim nCount As Integer
		Dim i, j As Integer
        Dim word As String
        Dim result As Integer
        Dim nPages, nWords As Integer
        Dim rc As Short
		
		' get JavaScript Object
		' note jso is related to PDDoc of a PDF,
		jso = gPdDoc.GetJSObject
		
		' count
		nCount = 0
		gbStop = False
		
		' search for the text
		If Not jso Is Nothing Then
			
			' info
			LabelInfo.Text = "Search ... "
			
			' total number of pages
			nPages = jso.numPages
			
			' Go through pages
			For i = 0 To nPages - 1
				
				' check each word in a page
				nWords = jso.getPageNumWords(i)
				For j = 0 To nWords - 1
					
					' get a word
					word = jso.getPageNthWord(i, j)
					If VarType(word) = VariantType.String Then
						
						' compare the word with what the user wants
                        result = StrComp(word, InputText.Text, CompareMethod.Text)
						
						' if same
						If result = 0 Then
							nCount = nCount + 1
							rc = jso.selectPageNthWord(i, j)
							
							' info
							LabelInfo.Text = "# " & nCount & " found in page " & (i + 1)
							
							If MsgBox("The word is found: Count " & nCount & ".  Continue?", MsgBoxStyle.YesNo) = MsgBoxResult.No Then
								GoTo TheEnd
							End If
							
							' info
							LabelInfo.Text = "Search ... "
							LabelInfo.Refresh()
							
						End If
					End If
				Next j
			Next i
			
TheEnd: 
			' summary
			If nCount > 0 Then
				LabelInfo.Text = "Total found: " & nCount
			Else
				LabelInfo.Text = "Not found in the document"
			End If
			
            System.Runtime.InteropServices.Marshal.ReleaseComObject(jso)
            jso = Nothing
			
		End If
	End Sub
End Class