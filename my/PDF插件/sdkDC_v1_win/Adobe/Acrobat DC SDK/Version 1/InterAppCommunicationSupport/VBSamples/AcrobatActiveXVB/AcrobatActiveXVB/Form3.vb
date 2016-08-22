Public Class PrintingPanel
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
    Friend WithEvents BtnPrint As System.Windows.Forms.Button
    Friend WithEvents BtnPrintPages As System.Windows.Forms.Button
    Friend WithEvents BtnPrintAll As System.Windows.Forms.Button
    Friend WithEvents TextToPage As System.Windows.Forms.TextBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents TextFromPage As System.Windows.Forms.TextBox
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.BtnPrint = New System.Windows.Forms.Button
        Me.BtnPrintPages = New System.Windows.Forms.Button
        Me.BtnPrintAll = New System.Windows.Forms.Button
        Me.TextFromPage = New System.Windows.Forms.TextBox
        Me.TextToPage = New System.Windows.Forms.TextBox
        Me.Label1 = New System.Windows.Forms.Label
        Me.Label2 = New System.Windows.Forms.Label
        Me.SuspendLayout()
        '
        'BtnPrint
        '
        Me.BtnPrint.Location = New System.Drawing.Point(8, 8)
        Me.BtnPrint.Name = "BtnPrint"
        Me.BtnPrint.Size = New System.Drawing.Size(88, 24)
        Me.BtnPrint.TabIndex = 0
        Me.BtnPrint.Text = "Print"
        '
        'BtnPrintPages
        '
        Me.BtnPrintPages.Location = New System.Drawing.Point(8, 48)
        Me.BtnPrintPages.Name = "BtnPrintPages"
        Me.BtnPrintPages.Size = New System.Drawing.Size(80, 24)
        Me.BtnPrintPages.TabIndex = 1
        Me.BtnPrintPages.Text = "Print Pages"
        '
        'BtnPrintAll
        '
        Me.BtnPrintAll.Location = New System.Drawing.Point(136, 8)
        Me.BtnPrintAll.Name = "BtnPrintAll"
        Me.BtnPrintAll.Size = New System.Drawing.Size(80, 24)
        Me.BtnPrintAll.TabIndex = 2
        Me.BtnPrintAll.Text = "Print All"
        '
        'TextFromPage
        '
        Me.TextFromPage.Location = New System.Drawing.Point(128, 48)
        Me.TextFromPage.Name = "TextFromPage"
        Me.TextFromPage.Size = New System.Drawing.Size(32, 20)
        Me.TextFromPage.TabIndex = 4
        Me.TextFromPage.Text = "1"
        '
        'TextToPage
        '
        Me.TextToPage.Location = New System.Drawing.Point(192, 48)
        Me.TextToPage.Name = "TextToPage"
        Me.TextToPage.Size = New System.Drawing.Size(32, 20)
        Me.TextToPage.TabIndex = 5
        Me.TextToPage.Text = "1"
        '
        'Label1
        '
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(88, 48)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(32, 16)
        Me.Label1.TabIndex = 6
        Me.Label1.Text = "from"
        '
        'Label2
        '
        Me.Label2.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(168, 48)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(16, 16)
        Me.Label2.TabIndex = 7
        Me.Label2.Text = "to"
        '
        'PrintingPanel
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 13)
        Me.BackColor = System.Drawing.SystemColors.InactiveBorder
        Me.ClientSize = New System.Drawing.Size(232, 78)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.TextToPage)
        Me.Controls.Add(Me.TextFromPage)
        Me.Controls.Add(Me.BtnPrintAll)
        Me.Controls.Add(Me.BtnPrintPages)
        Me.Controls.Add(Me.BtnPrint)
        Me.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.MaximizeBox = False
        Me.MaximumSize = New System.Drawing.Size(240, 112)
        Me.MinimumSize = New System.Drawing.Size(240, 112)
        Me.Name = "PrintingPanel"
        Me.Text = "Printing"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub BtnPrint_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnPrint.Click
        ' print with dialog
        AcrobatPDFWin.AcrobatPDFOCX.printWithDialog()
    End Sub

    Private Sub BtnPrintAll_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnPrintAll.Click
        ' print all
        AcrobatPDFWin.AcrobatPDFOCX.printAll()
    End Sub

    Private Sub BtnPrintPages_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnPrintPages.Click
        ' print certain pages
        Dim m As Integer = 1
        If Not TextFromPage.Text = "" Then
            m = Convert.ToInt32(TextFromPage.Text)
        End If
        Dim n As Integer = 1
        If Not TextToPage.Text = "" Then
            n = Convert.ToInt32(TextToPage.Text)
            AcrobatPDFWin.AcrobatPDFOCX.printPages(m, n)
        End If
    End Sub
End Class
