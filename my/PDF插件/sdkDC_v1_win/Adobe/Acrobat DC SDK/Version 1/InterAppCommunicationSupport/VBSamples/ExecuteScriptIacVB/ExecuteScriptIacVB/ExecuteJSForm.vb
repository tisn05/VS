Option Strict Off
Option Explicit On
Friend Class ExecuteJSForm
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
    Public WithEvents HelpBtn As System.Windows.Forms.Button
	Public WithEvents CloseBtn As System.Windows.Forms.Button
	Public WithEvents ExecuteBtn As System.Windows.Forms.Button
	Public WithEvents ReturnString As System.Windows.Forms.TextBox
	Public WithEvents ResetBtn As System.Windows.Forms.Button
	Public WithEvents ClearBtn As System.Windows.Forms.Button
	Public WithEvents JScode As System.Windows.Forms.TextBox
	Public WithEvents OpenDocBtn As System.Windows.Forms.Button
	Public WithEvents Label4 As System.Windows.Forms.Label
	Public WithEvents Label3 As System.Windows.Forms.Label
	Public WithEvents PDFDocName As System.Windows.Forms.Label
	Public WithEvents Label1 As System.Windows.Forms.Label
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(ExecuteJSForm))
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me.HelpBtn = New System.Windows.Forms.Button
        Me.CloseBtn = New System.Windows.Forms.Button
        Me.ExecuteBtn = New System.Windows.Forms.Button
        Me.ReturnString = New System.Windows.Forms.TextBox
        Me.ResetBtn = New System.Windows.Forms.Button
        Me.ClearBtn = New System.Windows.Forms.Button
        Me.JScode = New System.Windows.Forms.TextBox
        Me.OpenDocBtn = New System.Windows.Forms.Button
        Me.Label4 = New System.Windows.Forms.Label
        Me.Label3 = New System.Windows.Forms.Label
        Me.PDFDocName = New System.Windows.Forms.Label
        Me.Label1 = New System.Windows.Forms.Label
        Me.SuspendLayout()
        '
        'HelpBtn
        '
        Me.HelpBtn.BackColor = System.Drawing.SystemColors.Control
        Me.HelpBtn.Cursor = System.Windows.Forms.Cursors.Default
        Me.HelpBtn.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.HelpBtn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.HelpBtn.Location = New System.Drawing.Point(144, 360)
        Me.HelpBtn.Name = "HelpBtn"
        Me.HelpBtn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.HelpBtn.Size = New System.Drawing.Size(89, 25)
        Me.HelpBtn.TabIndex = 11
        Me.HelpBtn.Text = "Help"
        '
        'CloseBtn
        '
        Me.CloseBtn.BackColor = System.Drawing.SystemColors.Control
        Me.CloseBtn.Cursor = System.Windows.Forms.Cursors.Default
        Me.CloseBtn.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CloseBtn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.CloseBtn.Location = New System.Drawing.Point(288, 360)
        Me.CloseBtn.Name = "CloseBtn"
        Me.CloseBtn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.CloseBtn.Size = New System.Drawing.Size(89, 25)
        Me.CloseBtn.TabIndex = 10
        Me.CloseBtn.Text = "Close"
        '
        'ExecuteBtn
        '
        Me.ExecuteBtn.BackColor = System.Drawing.SystemColors.Control
        Me.ExecuteBtn.Cursor = System.Windows.Forms.Cursors.Default
        Me.ExecuteBtn.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ExecuteBtn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ExecuteBtn.Location = New System.Drawing.Point(8, 360)
        Me.ExecuteBtn.Name = "ExecuteBtn"
        Me.ExecuteBtn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ExecuteBtn.Size = New System.Drawing.Size(81, 25)
        Me.ExecuteBtn.TabIndex = 9
        Me.ExecuteBtn.Text = "Execute"
        '
        'ReturnString
        '
        Me.ReturnString.AcceptsReturn = True
        Me.ReturnString.AutoSize = False
        Me.ReturnString.BackColor = System.Drawing.SystemColors.Window
        Me.ReturnString.Cursor = System.Windows.Forms.Cursors.IBeam
        Me.ReturnString.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ReturnString.ForeColor = System.Drawing.SystemColors.WindowText
        Me.ReturnString.Location = New System.Drawing.Point(8, 312)
        Me.ReturnString.MaxLength = 0
        Me.ReturnString.Multiline = True
        Me.ReturnString.Name = "ReturnString"
        Me.ReturnString.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ReturnString.Size = New System.Drawing.Size(369, 33)
        Me.ReturnString.TabIndex = 8
        Me.ReturnString.Text = ""
        '
        'ResetBtn
        '
        Me.ResetBtn.BackColor = System.Drawing.SystemColors.Control
        Me.ResetBtn.Cursor = System.Windows.Forms.Cursors.Default
        Me.ResetBtn.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ResetBtn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ResetBtn.Location = New System.Drawing.Point(288, 56)
        Me.ResetBtn.Name = "ResetBtn"
        Me.ResetBtn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ResetBtn.Size = New System.Drawing.Size(81, 25)
        Me.ResetBtn.TabIndex = 6
        Me.ResetBtn.Text = "Reset"
        '
        'ClearBtn
        '
        Me.ClearBtn.BackColor = System.Drawing.SystemColors.Control
        Me.ClearBtn.Cursor = System.Windows.Forms.Cursors.Default
        Me.ClearBtn.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ClearBtn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ClearBtn.Location = New System.Drawing.Point(192, 56)
        Me.ClearBtn.Name = "ClearBtn"
        Me.ClearBtn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ClearBtn.Size = New System.Drawing.Size(81, 25)
        Me.ClearBtn.TabIndex = 5
        Me.ClearBtn.Text = "Clear"
        '
        'JScode
        '
        Me.JScode.AcceptsReturn = True
        Me.JScode.AutoSize = False
        Me.JScode.BackColor = System.Drawing.SystemColors.Window
        Me.JScode.Cursor = System.Windows.Forms.Cursors.IBeam
        Me.JScode.Font = New System.Drawing.Font("Arial", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.JScode.ForeColor = System.Drawing.SystemColors.Desktop
        Me.JScode.Location = New System.Drawing.Point(8, 96)
        Me.JScode.MaxLength = 10000
        Me.JScode.Multiline = True
        Me.JScode.Name = "JScode"
        Me.JScode.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.JScode.ScrollBars = System.Windows.Forms.ScrollBars.Both
        Me.JScode.Size = New System.Drawing.Size(369, 201)
        Me.JScode.TabIndex = 1
        Me.JScode.Text = "// open a new report" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "var rep = new Report();" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "// write titile" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.size = 3.0;" & _
        "" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.color = color.blue;" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.style = ""NoteTitle"";" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.writeText(this.document" & _
        "FileName);" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.writeText("" "");" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.divide();" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.writeText("" "");" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "// write do" & _
        "cument info." & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.size = 2.0;" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.color = color.black;" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.style = ""DefaultNot" & _
        "eText"";" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.indent(36);" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.writeText(""Title:    "" + this.title);" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.writeTe" & _
        "xt(""Author:   "" + this.author);" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.writeText(""Date:     "" + util.printd(""HH:MM" & _
        ":SS mmmm dd, yyyy"", this.modDate));" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "rep.writeText(""Pages:    "" + this.numPages)" & _
        ";" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "//open the report" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "var thePath = this.path;" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "repPath = thePath.replace("".pd" & _
        "f"", ""_report.pdf"");" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "var docRep = rep.open(repPath);" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "// return string" & Microsoft.VisualBasic.ChrW(13) & Microsoft.VisualBasic.ChrW(10) & "event.va" & _
        "lue = ""ThePDF file information is saved into a new file that is open in Acrobat " & _
        "now."";"
        Me.JScode.WordWrap = False
        '
        'OpenDocBtn
        '
        Me.OpenDocBtn.BackColor = System.Drawing.SystemColors.Control
        Me.OpenDocBtn.Cursor = System.Windows.Forms.Cursors.Default
        Me.OpenDocBtn.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.OpenDocBtn.ForeColor = System.Drawing.SystemColors.ControlText
        Me.OpenDocBtn.Location = New System.Drawing.Point(8, 8)
        Me.OpenDocBtn.Name = "OpenDocBtn"
        Me.OpenDocBtn.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.OpenDocBtn.Size = New System.Drawing.Size(97, 25)
        Me.OpenDocBtn.TabIndex = 0
        Me.OpenDocBtn.Text = "PDF document"
        '
        'Label4
        '
        Me.Label4.BackColor = System.Drawing.SystemColors.Control
        Me.Label4.Cursor = System.Windows.Forms.Cursors.Default
        Me.Label4.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Label4.Location = New System.Drawing.Point(8, 296)
        Me.Label4.Name = "Label4"
        Me.Label4.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Label4.Size = New System.Drawing.Size(145, 25)
        Me.Label4.TabIndex = 7
        Me.Label4.Text = "Return string by event.value"
        '
        'Label3
        '
        Me.Label3.BackColor = System.Drawing.SystemColors.Control
        Me.Label3.Cursor = System.Windows.Forms.Cursors.Default
        Me.Label3.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Label3.Location = New System.Drawing.Point(16, 56)
        Me.Label3.Name = "Label3"
        Me.Label3.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Label3.Size = New System.Drawing.Size(89, 17)
        Me.Label3.TabIndex = 4
        Me.Label3.Text = "JavaScript Code"
        '
        'PDFDocName
        '
        Me.PDFDocName.BackColor = System.Drawing.SystemColors.Control
        Me.PDFDocName.Cursor = System.Windows.Forms.Cursors.Default
        Me.PDFDocName.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.PDFDocName.ForeColor = System.Drawing.SystemColors.ControlText
        Me.PDFDocName.Location = New System.Drawing.Point(120, 8)
        Me.PDFDocName.Name = "PDFDocName"
        Me.PDFDocName.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.PDFDocName.Size = New System.Drawing.Size(257, 41)
        Me.PDFDocName.TabIndex = 3
        Me.PDFDocName.Text = "None"
        '
        'Label1
        '
        Me.Label1.BackColor = System.Drawing.SystemColors.Control
        Me.Label1.Cursor = System.Windows.Forms.Cursors.Default
        Me.Label1.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.ForeColor = System.Drawing.SystemColors.ControlText
        Me.Label1.Location = New System.Drawing.Point(8, 40)
        Me.Label1.Name = "Label1"
        Me.Label1.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Label1.Size = New System.Drawing.Size(1, 17)
        Me.Label1.TabIndex = 2
        Me.Label1.Text = "Label1"
        '
        'ExecuteJSForm
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.BackColor = System.Drawing.SystemColors.Control
        Me.ClientSize = New System.Drawing.Size(392, 397)
        Me.Controls.Add(Me.HelpBtn)
        Me.Controls.Add(Me.CloseBtn)
        Me.Controls.Add(Me.ExecuteBtn)
        Me.Controls.Add(Me.ReturnString)
        Me.Controls.Add(Me.JScode)
        Me.Controls.Add(Me.ResetBtn)
        Me.Controls.Add(Me.ClearBtn)
        Me.Controls.Add(Me.OpenDocBtn)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.PDFDocName)
        Me.Controls.Add(Me.Label1)
        Me.Cursor = System.Windows.Forms.Cursors.Default
        Me.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Location = New System.Drawing.Point(4, 23)
        Me.Name = "ExecuteJSForm"
        Me.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Text = "Execute Acrobat JavaScript"
        Me.ResumeLayout(False)

    End Sub
#End Region 
#Region "Upgrade Support "
	Private Shared m_vb6FormDefInstance As ExecuteJSForm
	Private Shared m_InitializingDefInstance As Boolean
	Public Shared Property DefInstance() As ExecuteJSForm
		Get
			If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
				m_InitializingDefInstance = True
				m_vb6FormDefInstance = New ExecuteJSForm()
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
	'
	' -------------------------------------------------------------------
	'ExecuteScriptIacVB
	'- This VB sample demonstrates how to execute Acrobat JavaScript code by calling
	'OLE object AcroForm's method ExecuteThisScript() in VB applications. The default
	'code is to write brief information of the active PDF to a new PDF report file and
	'open the report. The program provides a dialog for the user to modify, rewrite,
	'and execute their own JavaScript code.
	'- To work with the The dialog is quite simple, and the Help button provides
	'more information.
	'- Limitations:
	' The JavaScript code is executed from the active PDDoc, not from console. see
	' Acrobat JavaScript reference for functionality restrictions.
	' This is only a sample for the user to exercise simple Acrobat JavaScript.
	' It is not supposed to work as a real utility tool.
	' Error handling is not implemented in the sample, since error messages generally
	' cannot be passed back when calling ExecuteThisScript(). If anything inside the
	' input JavaScript code is wrong, the return string would be uncertain. i.e.
	' it may be empty, 0, or some error message. The user can open the JavaScript
	' Debugger in Acrobat to see messages.
	' ----------------------------------------------------------------------
	' OLE method of CAcroForm object Fields:
	'       CString ExecuteThisJavascript (LPCTSTR bstrTheScript);
	' Description -
	'       Executes the specified JavaScript script.
	' Parameters -
	'       bstrTheScript is a string containing a JavaScript script, which is executed by
	'     Acrobat in the context of the currently active document.
	' Return Value -
	'       Returns a result by assigning it to event value.
	' See SDK document "Acrobat Forms API Reference" for more information.
	'------------------------------------------------------------------------
	'The way to set up using Acrobat IAC in the project is from the menu
	' Project -> Preferences ... to select Acrobat and AcroFormAut type libraries.
	'------------------------------------------------------------------------
	
	'declare global variables
	Dim gApp As Acrobat.CAcroApp
	Dim gPdDoc As Acrobat.CAcroPDDoc
	Dim gAvDoc As Acrobat.CAcroAVDoc
	Dim gPDFPath As String
	'flag for Acrobat. true:if Acrobat was already running with PDF opened before. false: otherwise
	Dim gExisting As Boolean
	Dim gDefaultScript As String
	
	
	Private Sub ExecuteJSForm_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		
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
			PDFDocName.Text = gAvDoc.GetTitle
		Else
			gExisting = False
		End If
		
		gDefaultScript = JScode.Text
		
	End Sub
	
	Private Sub OpenDocBtn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles OpenDocBtn.Click
        Dim foundErr As Boolean
		
		' open file box for user to select a PDF file
        gPDFPath = vbNullString

        Dim CommonDialog1 As New OpenFileDialog

        CommonDialog1.Filter = "PDF Files (*.pdf)|*.pdf"
        If CommonDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then
            gPDFPath = CommonDialog1.FileName
        End If

		'If a file is selected, try to open it
		Dim fname As String
		If Len(gPDFPath) > 0 Then
			
			'Show acrobat if it's hidden
			gApp.Show()
			
			' create PDDoc object
            foundErr = False
			gPdDoc = CreateObject("AcroExch.PDDoc")
			
			' open the PDF in acrobat
			If gPdDoc.Open(gPDFPath) Then
				fname = gPdDoc.GetFileName
				gPdDoc.OpenAVDoc(fname)
				PDFDocName.Text = fname
			Else
				MsgBox("Failed to open " & gPDFPath)
			End If
		End If
	End Sub
	Private Sub CloseBtn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CloseBtn.Click
		ExecuteJSForm_Closed(Me, New System.EventArgs())
	End Sub
	
	Private Sub ExecuteJSForm_Closed(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Closed
		
		'End Acrobat only if it was not existing before
		'or it is existing but no PDF file opened.
		If Not gApp Is Nothing Then
			If gExisting = False Then
				gApp.CloseAllDocs()
				gApp.Exit()
			End If
		End If
		
        ' clean
        System.Runtime.InteropServices.Marshal.ReleaseComObject(gApp)
        gApp = Nothing
        System.Runtime.InteropServices.Marshal.ReleaseComObject(gPdDoc)
        gPdDoc = Nothing
        System.Runtime.InteropServices.Marshal.ReleaseComObject(gAvDoc)
		gAvDoc = Nothing
		End
		
	End Sub
	
	Private Sub ExecuteBtn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ExecuteBtn.Click
        Dim inumOpenPDFs As Integer
		' check if there are open PDFs
		inumOpenPDFs = gApp.GetNumAVDocs
		
		'Yes, we may execute JS code.
		If gApp.GetNumAVDocs > 0 Then
			
			'get the Doc.
			gAvDoc = gApp.GetActiveDoc
			gPdDoc = gAvDoc.GetPDDoc
			
			'show file name
			PDFDocName.Text = gAvDoc.GetTitle
			
			'there must be a JavaScript code input.
			If Len(JScode.Text) = 0 Then
				MsgBox("Please input JavaScript code")
			Else
				'OK, go.
				ExecuteJavaScript()
			End If
		Else
			'No. Warning message
			PDFDocName.Text = ""
			MsgBox("Please open a PDF file.")
		End If
		
		
	End Sub
	
	Private Sub ExecuteJavaScript()
		
		' get acrobat form object
		Dim formApp As AFORMAUTLib.AFormApp
		formApp = CreateObject("AFormAut.App")
		
		' access fields object inside AcroForm.
		Dim fields As AFORMAUTLib.Fields
		fields = formApp.fields
		
		' call the method to execute JavaScript
		ReturnString.Text = fields.ExecuteThisJavascript(JScode.Text)
		
	End Sub
	
	Private Sub ResetBtn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ResetBtn.Click
		JScode.Text = gDefaultScript
	End Sub
	
	Private Sub ClearBtn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ClearBtn.Click
		JScode.Text = ""
	End Sub
	
	Private Sub HelpBtn_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles HelpBtn.Click
		Dim HelpText As String
		HelpText = "1. Click PDF Document button to open or change the active PDF file open in Acrobat." & vbCrLf
		HelpText = HelpText & "2. Click Clear button to clear the text window, then input your JavaScript." & vbCrLf
		HelpText = HelpText & "3. Click Reset button to restore the default JavaScript code." & vbCrLf
		HelpText = HelpText & "4. Click Execute button to execute the JavaScript code. The return string of the execution is the value assigned to event.value." & vbCrLf
		HelpText = HelpText & "5. Click Close button to quit the program. Acrobat viewer would also quit if it is opened by this program or it has no PDF files opened." & vbCrLf
		MsgBox(HelpText)
	End Sub

    Private Sub JScode_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles JScode.TextChanged

    End Sub
End Class