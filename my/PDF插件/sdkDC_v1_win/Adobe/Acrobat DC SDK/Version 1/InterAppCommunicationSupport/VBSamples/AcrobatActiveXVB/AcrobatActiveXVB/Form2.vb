Public Class NavigatePanel
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
    Friend WithEvents BtnPrev As System.Windows.Forms.Button
    Friend WithEvents BtnNext As System.Windows.Forms.Button
    Friend WithEvents BtnForward As System.Windows.Forms.Button
    Friend WithEvents BtnBack As System.Windows.Forms.Button
    Friend WithEvents BtnFirst As System.Windows.Forms.Button
    Friend WithEvents BtnLast As System.Windows.Forms.Button
    Friend WithEvents TextPageNumber As System.Windows.Forms.TextBox
    Friend WithEvents BtnPage As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.BtnPrev = New System.Windows.Forms.Button
        Me.BtnNext = New System.Windows.Forms.Button
        Me.BtnForward = New System.Windows.Forms.Button
        Me.BtnBack = New System.Windows.Forms.Button
        Me.BtnFirst = New System.Windows.Forms.Button
        Me.BtnLast = New System.Windows.Forms.Button
        Me.TextPageNumber = New System.Windows.Forms.TextBox
        Me.BtnPage = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'BtnPrev
        '
        Me.BtnPrev.Location = New System.Drawing.Point(16, 8)
        Me.BtnPrev.Name = "BtnPrev"
        Me.BtnPrev.Size = New System.Drawing.Size(72, 24)
        Me.BtnPrev.TabIndex = 0
        Me.BtnPrev.Text = "Previous"
        '
        'BtnNext
        '
        Me.BtnNext.Location = New System.Drawing.Point(96, 8)
        Me.BtnNext.Name = "BtnNext"
        Me.BtnNext.Size = New System.Drawing.Size(72, 24)
        Me.BtnNext.TabIndex = 1
        Me.BtnNext.Text = "Next"
        '
        'BtnForward
        '
        Me.BtnForward.Location = New System.Drawing.Point(96, 104)
        Me.BtnForward.Name = "BtnForward"
        Me.BtnForward.Size = New System.Drawing.Size(72, 24)
        Me.BtnForward.TabIndex = 2
        Me.BtnForward.Text = "Forward"
        '
        'BtnBack
        '
        Me.BtnBack.Location = New System.Drawing.Point(16, 104)
        Me.BtnBack.Name = "BtnBack"
        Me.BtnBack.Size = New System.Drawing.Size(72, 24)
        Me.BtnBack.TabIndex = 3
        Me.BtnBack.Text = "Back"
        '
        'BtnFirst
        '
        Me.BtnFirst.Location = New System.Drawing.Point(16, 40)
        Me.BtnFirst.Name = "BtnFirst"
        Me.BtnFirst.Size = New System.Drawing.Size(72, 24)
        Me.BtnFirst.TabIndex = 4
        Me.BtnFirst.Text = "First"
        '
        'BtnLast
        '
        Me.BtnLast.Location = New System.Drawing.Point(96, 40)
        Me.BtnLast.Name = "BtnLast"
        Me.BtnLast.Size = New System.Drawing.Size(72, 24)
        Me.BtnLast.TabIndex = 5
        Me.BtnLast.Text = "Last"
        '
        'TextPageNumber
        '
        Me.TextPageNumber.Location = New System.Drawing.Point(120, 72)
        Me.TextPageNumber.MaxLength = 5
        Me.TextPageNumber.Name = "TextPageNumber"
        Me.TextPageNumber.Size = New System.Drawing.Size(40, 20)
        Me.TextPageNumber.TabIndex = 7
        Me.TextPageNumber.Text = "1"
        '
        'BtnPage
        '
        Me.BtnPage.Location = New System.Drawing.Point(16, 72)
        Me.BtnPage.Name = "BtnPage"
        Me.BtnPage.Size = New System.Drawing.Size(88, 24)
        Me.BtnPage.TabIndex = 8
        Me.BtnPage.Text = "Go To Page"
        '
        'NavigatePanel
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 13)
        Me.BackColor = System.Drawing.SystemColors.InactiveBorder
        Me.ClientSize = New System.Drawing.Size(188, 133)
        Me.Controls.Add(Me.BtnPage)
        Me.Controls.Add(Me.TextPageNumber)
        Me.Controls.Add(Me.BtnLast)
        Me.Controls.Add(Me.BtnFirst)
        Me.Controls.Add(Me.BtnBack)
        Me.Controls.Add(Me.BtnForward)
        Me.Controls.Add(Me.BtnNext)
        Me.Controls.Add(Me.BtnPrev)
        Me.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.MaximizeBox = False
        Me.MaximumSize = New System.Drawing.Size(196, 167)
        Me.MinimizeBox = False
        Me.MinimumSize = New System.Drawing.Size(196, 167)
        Me.Name = "NavigatePanel"
        Me.Text = "Navigate Pages / Views"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub BtnPrev_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles BtnPrev.Click
        ' go to previous page
        AcrobatPDFWin.AcrobatPDFOCX.gotoPreviousPage()
    End Sub

    Private Sub BtnNext_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles BtnNext.Click
        ' go to next page
        AcrobatPDFWin.AcrobatPDFOCX.gotoNextPage()
    End Sub

    Private Sub BtnForward_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles BtnForward.Click
        ' go forward
        AcrobatPDFWin.AcrobatPDFOCX.goForwardStack()
    End Sub

    Private Sub BtnBack_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles BtnBack.Click
        ' go backward
        AcrobatPDFWin.AcrobatPDFOCX.goBackwardStack()
    End Sub

    Private Sub BtnFirst_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles BtnFirst.Click
        ' go to first page
        AcrobatPDFWin.AcrobatPDFOCX.gotoFirstPage()
    End Sub

    Private Sub BtnLast_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles BtnLast.Click
        ' go to last page
        AcrobatPDFWin.AcrobatPDFOCX.gotoLastPage()
    End Sub

    Private Sub BtnPage_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnPage.Click
        ' go to certain page ( started with 1 )
        Dim n As Integer = 1
        If Not TextPageNumber.Text = "" Then
            n = Convert.ToInt32(TextPageNumber.Text)
        End If
        If n < 1 Then
            TextPageNumber.Text = 1
            n = 1
        End If
        AcrobatPDFWin.AcrobatPDFOCX.setCurrentPage(n)

    End Sub
End Class
