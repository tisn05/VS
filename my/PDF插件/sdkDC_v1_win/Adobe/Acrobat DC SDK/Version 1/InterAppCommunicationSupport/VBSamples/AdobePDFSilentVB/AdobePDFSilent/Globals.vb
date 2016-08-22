Module globals
    Public defaultfrmMain As frmMain

    Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" _
           (ByVal hwnd As Long, ByVal lpszOp As String, _
           ByVal lpszFile As String, ByVal lpszParams As String, _
          ByVal LpszDir As String, ByVal FsShowCmd As Long) _
        As Long

    Friend Declare Function GetDesktopWindow Lib "user32" () As Long


    Public Sub Main()
        defaultfrmMain = New frmMain
        Application.Run(defaultfrmMain)

    End Sub

End Module