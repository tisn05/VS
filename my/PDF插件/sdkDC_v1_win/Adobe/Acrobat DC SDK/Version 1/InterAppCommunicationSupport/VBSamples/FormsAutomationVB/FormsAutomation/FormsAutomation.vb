Option Strict Off
Option Explicit On
Friend Class FormsAutomation
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
	Public WithEvents ExitButton As System.Windows.Forms.Button
	Public WithEvents StatusBox As System.Windows.Forms.TextBox
	Public WithEvents CreateFieldsButton As System.Windows.Forms.Button
	Public WithEvents StatusLabel As System.Windows.Forms.Label
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me.ExitButton = New System.Windows.Forms.Button
        Me.StatusBox = New System.Windows.Forms.TextBox
        Me.CreateFieldsButton = New System.Windows.Forms.Button
        Me.StatusLabel = New System.Windows.Forms.Label
        Me.SuspendLayout()
        '
        'ExitButton
        '
        Me.ExitButton.BackColor = System.Drawing.SystemColors.Control
        Me.ExitButton.Cursor = System.Windows.Forms.Cursors.Default
        Me.ExitButton.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ExitButton.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ExitButton.Location = New System.Drawing.Point(152, 15)
        Me.ExitButton.Name = "ExitButton"
        Me.ExitButton.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ExitButton.Size = New System.Drawing.Size(113, 25)
        Me.ExitButton.TabIndex = 3
        Me.ExitButton.Text = "Exit"
        Me.ExitButton.UseVisualStyleBackColor = False
        '
        'StatusBox
        '
        Me.StatusBox.AcceptsReturn = True
        Me.StatusBox.BackColor = System.Drawing.SystemColors.ButtonFace
        Me.StatusBox.Cursor = System.Windows.Forms.Cursors.IBeam
        Me.StatusBox.Enabled = False
        Me.StatusBox.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.StatusBox.ForeColor = System.Drawing.SystemColors.WindowText
        Me.StatusBox.Location = New System.Drawing.Point(64, 56)
        Me.StatusBox.MaxLength = 0
        Me.StatusBox.Name = "StatusBox"
        Me.StatusBox.ReadOnly = True
        Me.StatusBox.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.StatusBox.Size = New System.Drawing.Size(200, 19)
        Me.StatusBox.TabIndex = 1
        '
        'CreateFieldsButton
        '
        Me.CreateFieldsButton.BackColor = System.Drawing.SystemColors.Control
        Me.CreateFieldsButton.Cursor = System.Windows.Forms.Cursors.Default
        Me.CreateFieldsButton.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CreateFieldsButton.ForeColor = System.Drawing.SystemColors.ControlText
        Me.CreateFieldsButton.Location = New System.Drawing.Point(16, 15)
        Me.CreateFieldsButton.Name = "CreateFieldsButton"
        Me.CreateFieldsButton.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.CreateFieldsButton.Size = New System.Drawing.Size(113, 25)
        Me.CreateFieldsButton.TabIndex = 0
        Me.CreateFieldsButton.Text = "Create Fields"
        Me.CreateFieldsButton.UseVisualStyleBackColor = False
        '
        'StatusLabel
        '
        Me.StatusLabel.BackColor = System.Drawing.SystemColors.Control
        Me.StatusLabel.Cursor = System.Windows.Forms.Cursors.Default
        Me.StatusLabel.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.StatusLabel.ForeColor = System.Drawing.SystemColors.ControlText
        Me.StatusLabel.Location = New System.Drawing.Point(16, 58)
        Me.StatusLabel.Name = "StatusLabel"
        Me.StatusLabel.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.StatusLabel.Size = New System.Drawing.Size(40, 17)
        Me.StatusLabel.TabIndex = 2
        Me.StatusLabel.Text = "Status:"
        '
        'FormsAutomation
        '
        Me.AcceptButton = Me.ExitButton
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.BackColor = System.Drawing.SystemColors.Control
        Me.ClientSize = New System.Drawing.Size(281, 89)
        Me.Controls.Add(Me.ExitButton)
        Me.Controls.Add(Me.StatusBox)
        Me.Controls.Add(Me.CreateFieldsButton)
        Me.Controls.Add(Me.StatusLabel)
        Me.Cursor = System.Windows.Forms.Cursors.Default
        Me.Font = New System.Drawing.Font("Arial", 8.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Location = New System.Drawing.Point(11, 30)
        Me.MaximizeBox = False
        Me.Name = "FormsAutomation"
        Me.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Text = "Acrobat Forms Automation Demo"
        Me.ResumeLayout(False)

    End Sub
#End Region 
#Region "Upgrade Support "
	Private Shared m_vb6FormDefInstance As FormsAutomation
	Private Shared m_InitializingDefInstance As Boolean
	Public Shared Property DefInstance() As FormsAutomation
		Get
			If m_vb6FormDefInstance Is Nothing OrElse m_vb6FormDefInstance.IsDisposed Then
				m_InitializingDefInstance = True
				m_vb6FormDefInstance = New FormsAutomation()
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
	'
	' FormsAutomation.vbp
	'
	' - The application shows how to add Text boxes, combo boxes, radio
	'   buttons and action buttons to a PDF. In this case, the PDF is
	'   empty and we are adding form fields to it.
	'
	' - To run the sample:
	'
	'   Run the sample and click on the Create Fields button. The
    '   sample will open the IAC\TestFiles\FormsAutomation.pdf file 
    '   in Acrobat and add form fields to it.
	
	
	Dim formApp As AFORMAUTLib.AFormApp
	Dim acroForm As AFORMAUTLib.Fields
	Dim field As AFORMAUTLib.Field
	Dim bOK As Boolean
	Dim avDoc As Acrobat.CAcroAVDoc
	Dim pdDoc As Acrobat.CAcroPDDoc
	Dim radiobuttonArray(5) As String
	
	Private Sub CreateFieldsButton_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles CreateFieldsButton.Click
		
		'Set the mouse icon to display an hourglass
		System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.WaitCursor
		
		'Disable the create fields button
		CreateFieldsButton.Enabled = False
		
		'Update the status box
		StatusBox.Text = "Opening PDF Document..."
		
		' Open the sample PDF file
        avDoc = CreateObject("AcroExch.AVDoc")
        Dim file As String = System.Windows.Forms.Application.StartupPath & "\..\..\..\..\TestFiles\FormsAutomation.pdf"

        bOK = avDoc.Open(file, "Forms Automation Demo")
		
		'If everything was OK opening the PDF, we now instantiate the Forms
		'Automation object.
		If (bOK) Then
			formApp = CreateObject("AFormAut.App")
			acroForm = formApp.Fields
        Else
            System.Runtime.InteropServices.Marshal.ReleaseComObject(avDoc)
            avDoc = Nothing
            MsgBox("Failed to open PDF Document. Aborting...")
            End
        End If

        'Update the status box
        StatusBox.Text = "Creating Form Fields..."

        'Add a Text Field to the PDF
        ' Fields.Add takes seven parameters:
        '   FieldName is the The fully-qualified name of the field.
        '   FieldType is the field type for the newly created field. Valid types are:
        '             "text", "button", "combobox", "listbox", "checkbox", "radiobutton".
        '              You must spell the field type names exactly as shown, including
        '              use the quotation marks.
        '   PageNum   is the zero-based page number to add the field to.
        '   Left      A float for the left most position of the rectangle placement.
        '   Top       A float for the top of the rectangle placement.
        '   Right     A float for the right most position of the rectangle placement.
        '   Bottom    A float for the bottom of the rectangle placement.
        field = acroForm.Add("Name", "text", 0, 100, 650, 450, 675)

        'Next use Field.SetBorderColor to set the color of the fields borders in
        'Gray scale, Transparent, RGB or CMYK color. All values are floating point numbers
        'between 0 and 1. In the example below, we are using RGB color space. So the last
        'parameter is set to "0"
        field.SetBorderColor("RGB", 0.7, 0.5, 0.3, 0)

        'Field.TextSize sets the font size of the field. You can also use Field.TextFont
        'to use a specific font. See the Acrobat Forms API Reference for more information
        'on various font names.
        field.TextSize = CShort("12")
        'field.TextFont = "Times-Roman"

        'You can use Field.DefaultValue to set the default value of a field.
        field.DefaultValue = "Enter your full name here"

        'Create a date field formatted to be "mm/dd/yyyy". The SetJavaScriptAction
        'method sets the keystoke and format properties to the proper date format
        'using JavaScript
        field = acroForm.Add("Creation Date", "text", 0, 375, 680, 450, 715)
        field.SetBorderColor("RGB", 0.7, 0.5, 0.3, 0)
        field.TextSize = CShort("12")
        field.DefaultValue = "01/01/2000"
        field.SetJavaScriptAction("keystroke", "AFDate_KeystrokeEx(""mm/dd/yyyy"");")
        field.SetJavaScriptAction("format", "AFDate_FormatEx(""mm/dd/yyyy"");")

        'Now add a few more fields for address, city, state, zip code. The zip code
        'field introduces a new functionality - using JavaScript to format the
        'field values.
        field = acroForm.Add("Address", "text", 0, 100, 620, 450, 645)
        field.SetBorderColor("RGB", 0.7, 0.5, 0.3, 0)
        field.TextSize = CShort("12")
        field.DefaultValue = "Enter your address here"

        field = acroForm.Add("City", "text", 0, 100, 590, 345, 615)
        field.SetBorderColor("RGB", 0.7, 0.5, 0.3, 0)
        field.TextSize = CShort("12")
        field.DefaultValue = "Enter your City"

        field = acroForm.Add("State", "text", 0, 350, 590, 375, 615)
        field.SetBorderColor("RGB", 0.7, 0.5, 0.3, 0)
        field.TextSize = CShort("12")
        field.DefaultValue = "CA"

        field = acroForm.Add("Zip", "text", 0, 385, 590, 450, 615)
        field.SetBorderColor("RGB", 0.7, 0.5, 0.3, 0)
        field.TextSize = CShort("12")
        field.DefaultValue = "99999"
        field.SetJavaScriptAction("keystroke", "AFSpecial_Keystroke(0);")
        field.SetJavaScriptAction("format", "AFSpecial_Format(0);")

        'To create a series of radio buttons. Create the field, then call
        'Fields.Add again to create the children
        field = acroForm.Add("Radio", "radiobutton", 0, 150, 500, 165, 515)
        field.SetBorderColor("RGB", 0.75, 0.55, 0.35, 0)

        field = acroForm.Add("Radio", "radiobutton", 0, 180, 500, 195, 515)
        field.SetBorderColor("RGB", 0.75, 0.55, 0.35, 0)

        field = acroForm.Add("Radio", "radiobutton", 0, 210, 500, 225, 515)
        field.SetBorderColor("RGB", 0.75, 0.55, 0.35, 0)

        field = acroForm.Add("Radio", "radiobutton", 0, 240, 500, 255, 515)
        field.SetBorderColor("RGB", 0.75, 0.55, 0.35, 0)

        field = acroForm.Add("Radio", "radiobutton", 0, 270, 500, 285, 515)
        field.SetBorderColor("RGB", 0.75, 0.55, 0.35, 0)

        'Build an array then call SetExportValues
        radiobuttonArray(0) = "Red"
        radiobuttonArray(1) = "Green"
        radiobuttonArray(2) = "Blue"
        radiobuttonArray(3) = "Yellow"
        radiobuttonArray(4) = "Black"
        radiobuttonArray(5) = "White"
        field.SetExportValues(radiobuttonArray)

        'Create a list box. This section of code will create a list box on the
        'PDF and populate the list box with items.
        field = acroForm.Add("My Combo Box", "combobox", 0, 100, 450, 380, 475)
        field.SetBorderColor("RGB", 0.75, 0.55, 0.35, 0)
        field.TextSize = CShort("12")

        'Now that the list box is complete, let's build two arrays that
        'will be used to populate the list box.
        'First is an array of items that will be displayed to the user
        Dim arrayOfItems(4) As String
        arrayOfItems(0) = "[Choose an item from the list]"
        arrayOfItems(1) = "Bird"
        arrayOfItems(2) = "Cat"
        arrayOfItems(3) = "Dog"
        arrayOfItems(4) = "Gold Fish"

        'Then the exported value that will come back in the FDF
        Dim arrayOfExportValues(4) As String
        arrayOfExportValues(0) = "X"
        arrayOfExportValues(1) = "a"
        arrayOfExportValues(2) = "b"
        arrayOfExportValues(3) = "c"
        arrayOfExportValues(4) = "d"

        'Next, use the PopulateListOrComboBox method to add values into the
        'combo box
        field.PopulateListOrComboBox(arrayOfItems, arrayOfExportValues)

        'Now, create a new submit button for the user to click on. Set the border
        'style, the button caption, the color of the background. Field.Highlight sets
        'the property of the button to look like it was pushed in.
        field = acroForm.Add("Submit Button", "button", 0, 100, 50, 150, 100)
        field.BorderStyle = "beveled"
        field.SetButtonCaption("N", "Submit")
        field.SetBackgroundColor("G", 0.85, 0.85, 0, 0)
        field.Highlight = "push"

        'Set the SubmitForm action to point to a web server.
        field.SetSubmitFormAction("up", "http://your-webserver.com/cgi-bin/myscript", 0)

        'Create a reset button just in case users want to change there minds a
        'reset all the fields on the newly created PDF.
        field = acroForm.Add("Reset Button", "button", 0, 175, 50, 225, 100)
        field.BorderStyle = "beveled"
        field.SetButtonCaption("N", "Reset")
        field.SetBackgroundColor("G", 0.85, 0.85, 0, 0)
        field.Highlight = "push"
        'Set the ResetForm action for the button.
        field.SetResetFormAction("up", 0)

        'Update the Status box.
        StatusBox.Text = "Fields have been added successfully."

        'This section converts a AVDoc to a PDDoc. Then we use PDDoc.Save to
        'save the PDF since there isn't a viewer level method to save an AVDoc.

        'These lines are commented out to provide user feedback
        'If present, no doc is visible and original form is changed
        'so it isn't obvious what the sample did, and it doesn't work
        'twice in a row.
        'It's best to have Acrobat open and visible to see form creation
        '''''''''
        'Set pdDoc = avDoc.GetPDDoc
        'bOK = pdDoc.Save(1, "C:\FormsAutomation.pdf")
        'If bOK = False Then
        'MsgBox "Unable to Save the PDF file"
        'End If

        ' Close the AVDoc
        'avDoc.Close (False)
        '''''''''
        'End provide User Feedback

CleanUp:
        'Change the mouse pointer to display normal cursor and enable the Exit button
        System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.Default

        'Enable the create fields button
        CreateFieldsButton.Enabled = True
	End Sub
	
	Private Sub ExitButton_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ExitButton.Click
		' End the program
		End
		
	End Sub

    Private Sub FormsAutomation_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        StatusBox.Text = "Click Create Fields Button..."
    End Sub
End Class