Public Class StylePanel
    Inherits System.Windows.Forms.Form


#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents BtnSetZoom As System.Windows.Forms.Button
    Friend WithEvents TextZoom As System.Windows.Forms.TextBox
    Friend WithEvents ComboPageMode As System.Windows.Forms.ComboBox
    Friend WithEvents CheckBoxToolbar As System.Windows.Forms.CheckBox
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents ComboBoxLayout As System.Windows.Forms.ComboBox
    Friend WithEvents Label4 As System.Windows.Forms.Label
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.Label1 = New System.Windows.Forms.Label
        Me.Label2 = New System.Windows.Forms.Label
        Me.BtnSetZoom = New System.Windows.Forms.Button
        Me.TextZoom = New System.Windows.Forms.TextBox
        Me.ComboPageMode = New System.Windows.Forms.ComboBox
        Me.CheckBoxToolbar = New System.Windows.Forms.CheckBox
        Me.Label3 = New System.Windows.Forms.Label
        Me.ComboBoxLayout = New System.Windows.Forms.ComboBox
        Me.Label4 = New System.Windows.Forms.Label
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(8, 32)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(72, 16)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Page Mode"
        '
        'Label2
        '
        Me.Label2.Location = New System.Drawing.Point(48, 8)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(128, 16)
        Me.Label2.TabIndex = 1
        Me.Label2.Text = "Show Toolbar"
        '
        'BtnSetZoom
        '
        Me.BtnSetZoom.Location = New System.Drawing.Point(8, 96)
        Me.BtnSetZoom.Name = "BtnSetZoom"
        Me.BtnSetZoom.Size = New System.Drawing.Size(72, 24)
        Me.BtnSetZoom.TabIndex = 3
        Me.BtnSetZoom.Text = "Set Zoom"
        '
        'TextZoom
        '
        Me.TextZoom.Location = New System.Drawing.Point(96, 96)
        Me.TextZoom.Name = "TextZoom"
        Me.TextZoom.Size = New System.Drawing.Size(48, 20)
        Me.TextZoom.TabIndex = 5
        Me.TextZoom.Text = "60"
        '
        'ComboPageMode
        '
        Me.ComboPageMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.ComboPageMode.Items.AddRange(New Object() {"None", "Bookmarks", "Thumbs"})
        Me.ComboPageMode.Location = New System.Drawing.Point(72, 32)
        Me.ComboPageMode.Name = "ComboPageMode"
        Me.ComboPageMode.Size = New System.Drawing.Size(96, 21)
        Me.ComboPageMode.TabIndex = 6
        '
        'CheckBoxToolbar
        '
        Me.CheckBoxToolbar.Checked = True
        Me.CheckBoxToolbar.CheckState = System.Windows.Forms.CheckState.Checked
        Me.CheckBoxToolbar.Location = New System.Drawing.Point(16, 8)
        Me.CheckBoxToolbar.Name = "CheckBoxToolbar"
        Me.CheckBoxToolbar.Size = New System.Drawing.Size(16, 16)
        Me.CheckBoxToolbar.TabIndex = 7
        '
        'Label3
        '
        Me.Label3.Location = New System.Drawing.Point(8, 64)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(40, 16)
        Me.Label3.TabIndex = 8
        Me.Label3.Text = "Layout"
        '
        'ComboBoxLayout
        '
        Me.ComboBoxLayout.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.ComboBoxLayout.Items.AddRange(New Object() {"Don't Care", "Single Page", "One Column", "Two Column Left", "Two Column Right"})
        Me.ComboBoxLayout.Location = New System.Drawing.Point(72, 64)
        Me.ComboBoxLayout.Name = "ComboBoxLayout"
        Me.ComboBoxLayout.Size = New System.Drawing.Size(96, 21)
        Me.ComboBoxLayout.TabIndex = 9
        '
        'Label4
        '
        Me.Label4.Location = New System.Drawing.Point(152, 96)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(16, 16)
        Me.Label4.TabIndex = 10
        Me.Label4.Text = "%"
        '
        'StylePanel
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(184, 126)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.ComboBoxLayout)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.CheckBoxToolbar)
        Me.Controls.Add(Me.ComboPageMode)
        Me.Controls.Add(Me.TextZoom)
        Me.Controls.Add(Me.BtnSetZoom)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.MaximumSize = New System.Drawing.Size(192, 160)
        Me.MinimumSize = New System.Drawing.Size(192, 160)
        Me.Name = "StylePanel"
        Me.Text = "Window Style"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

#End Region

    Private Sub ComboPageMode_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboPageMode.SelectedIndexChanged
        ' set page mode: none, bookmark, thumbnail
        AcrobatPDFWin.AcrobatPDFOCX.setPageMode(ComboPageMode.Text.ToLower())
    End Sub

    Private Sub BtnSetZoom_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles BtnSetZoom.Click
        ' set zoom: n per cent 
        AcrobatPDFWin.AcrobatPDFOCX.setZoom(TextZoom.Text)
    End Sub
    Private Sub CheckBoxToolbar_CheckedChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles CheckBoxToolbar.CheckedChanged
        ' show / hide toolbar 
        AcrobatPDFWin.AcrobatPDFOCX.setShowToolbar(CheckBoxToolbar.Checked)
    End Sub
    Private Sub ComboBoxLayout_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBoxLayout.SelectedIndexChanged
        ' set layout: one, two column, ... 
        Dim layout As String

        If (String.Compare(ComboBoxLayout.Text, "Single Page") = 0) Then
            layout = "singlepage"
        ElseIf (String.Compare(ComboBoxLayout.Text, "One Column") = 0) Then
            layout = "onecolumn"
        ElseIf (String.Compare(ComboBoxLayout.Text, "Two Column Left") = 0) Then
            layout = "twocolumnleft"
        ElseIf (String.Compare(ComboBoxLayout.Text, "Two Column Right") = 0) Then
            layout = "twocolumnright"
        Else
            layout = "dontcare"
        End If
        AcrobatPDFWin.AcrobatPDFOCX.setLayoutMode(layout)
    End Sub
End Class
