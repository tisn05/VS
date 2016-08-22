Option Strict Off
Option Explicit On
Module ActiveView
	
	' ADOBE SYSTEMS INCORPORATED
    ' Copyright (C) 1994-2005 Adobe Systems Incorporated
	' All rights reserved.
	'
	' NOTICE: Adobe permits you to use, modify, and distribute this file
	' in accordance with the terms of the Adobe license agreement
	' accompanying it. If you have received this file from a source other
	' than Adobe, then your use, modification, or distribution of it
	' requires the prior written permission of Adobe.
	
	Public Const modal As Short = 1
	Public Const CASCADE As Short = 0
	Public Const TILE_HORIZONTAL As Short = 1
	Public Const TILE_VERTICAL As Short = 2
	Public Const ARRANGE_ICONS As Short = 3
	
	Structure FormState
		Dim Deleted As Short
		Dim Dirty As Short
	End Structure
	
	Public FState() As FormState
    Public Document() As frmPDFView = New frmPDFView() {}
    Public AcroExchAVDoc() As Acrobat.CAcroAVDoc
	Public nOpenWindows As Short
	Public AcroExchApp As Acrobat.CAcroApp
	
	' API functions used to read and write to MDINOTE.INI.
	' Used for handling the recent files list.
	Declare Function GetPrivateProfileStringA Lib "Kernel32" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Short, ByVal lpFileName As String) As Short
	Declare Function WritePrivateProfileStringA Lib "Kernel32" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpString As String, ByVal lplFileName As String) As Short
	
	Declare Function GetWindow Lib "user32" (ByVal hwnd As Integer, ByVal wCmd As Integer) As Integer
	Declare Function SendMessage Lib "user32"  Alias "SendMessageA"(ByVal hwnd As Integer, ByVal wMsg As Integer, ByVal wParam As Integer, ByRef lParam As Integer) As Integer
	
	Public Const WM_KEYDOWN As Short = &H100s
	Public Const WM_KEYUP As Short = &H101s
	Public Const GW_CHILD As Short = 5
	Public Const SWP_NOMOVE As Short = &H2s
	Public Const SWP_NOZORDER As Short = &H4s
	
	' Rect type defined same as standard windows
	' rect.. must be in sync!
	Structure rect
		Dim Left_Renamed As Integer
		Dim Top As Integer
		Dim Right_Renamed As Integer
		Dim Bottom As Integer
	End Structure
	
	Declare Function GetClientRect Lib "user32" (ByVal hwnd As Integer, ByRef lpRect As rect) As Integer
	Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Integer, ByVal hWndInsertAfter As Integer, ByVal x As Integer, ByVal Y As Integer, ByVal cx As Integer, ByVal cy As Integer, ByVal wFlags As Integer) As Integer
	
	Function AnyPadsLeft() As Short
		Dim i As Short
		
		' Cycle throught the document array.
		' Return True if there is at least one
		' open document remaining.
		For i = 1 To UBound(Document)
			If Not FState(i).Deleted Then
				AnyPadsLeft = True
				Exit Function
			End If
		Next 
		
	End Function
	
	Sub CenterForm(ByRef frmParent As System.Windows.Forms.Form, ByRef frmChild As System.Windows.Forms.Form)
		' This procedure centers a child form over a parent form.
		' Calling this routine loads the dialog. Use the Show method
		' to display the dialog after calling this routine ( ie MyFrm.Show 1)
		
        Dim l, t As Integer
		' get left offset
		l = VB6.PixelsToTwipsX(frmParent.Left) + ((VB6.PixelsToTwipsX(frmParent.Width) - VB6.PixelsToTwipsX(frmChild.Width)) / 2)
		If (l + VB6.PixelsToTwipsX(frmChild.Width) > VB6.PixelsToTwipsX(System.Windows.Forms.Screen.PrimaryScreen.Bounds.Width)) Then
			l = VB6.PixelsToTwipsX(System.Windows.Forms.Screen.PrimaryScreen.Bounds.Width) = VB6.PixelsToTwipsX(frmChild.Width)
		End If
		
		' get top offset
		t = VB6.PixelsToTwipsY(frmParent.Top) + ((VB6.PixelsToTwipsY(frmParent.Height) - VB6.PixelsToTwipsY(frmChild.Height)) / 2)
		If (t + VB6.PixelsToTwipsY(frmChild.Height) > VB6.PixelsToTwipsY(System.Windows.Forms.Screen.PrimaryScreen.Bounds.Height)) Then
			t = VB6.PixelsToTwipsY(System.Windows.Forms.Screen.PrimaryScreen.Bounds.Height) - VB6.PixelsToTwipsY(frmChild.Height)
		End If
		
		' center the child formfv
		frmChild.SetBounds(VB6.TwipsToPixelsX(l), VB6.TwipsToPixelsY(t), 0, 0, Windows.Forms.BoundsSpecified.X Or Windows.Forms.BoundsSpecified.Y)
		
	End Sub
	
	Function FindFreeIndex() As Short
		Dim i As Short
		Dim ArrayCount As Short
		
		ArrayCount = UBound(Document)
		
		' Cycle throught the document array. If one of the
		' documents has been deleted, then return that
		' index.
		For i = 1 To ArrayCount
			If FState(i).Deleted Then
				FindFreeIndex = i
                FState(i).Deleted = False
                If Not Document(i) Is Nothing Then
                    Document(i) = Nothing
                End If
                Exit Function
            End If
		Next 
		
		' If none of the elements in the document array have
		' been deleted, then increment the document and the
		' state arrays by one and return the index to the
		' new element.
		
        ReDim Preserve Document(ArrayCount + 1)
		ReDim Preserve FState(ArrayCount + 1)
		ReDim Preserve AcroExchAVDoc(ArrayCount + 1)
		FindFreeIndex = UBound(Document)
	End Function
	
    Sub GetRecentFiles()
        Dim i, j As Short
        Dim key As String
        Dim RetVal As Long
        Dim IniString As String

        ' This variable must be large enough to hold the return string
        ' from the GetPrivateProfileString API.
        IniString = New String(Chr(0), 255)

        ' Get recent file strings from MDINOTE.INI
        For i = 1 To 4
            key = "RecentFile" & i
            RetVal = GetPrivateProfileStringA("Recent Files", key, "Not Used", IniString, Len(IniString), "MDINOTE.INI")
            '(ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long


            If RetVal And Left(IniString, 8) <> "Not Used" Then
                ' Update the MDI form's menu.
                frmMDI.DefInstance.mnuRecentFile(0).Visible = True
                frmMDI.DefInstance.mnuRecentFile(i).Text = IniString
                frmMDI.DefInstance.mnuRecentFile(i).Visible = True

                ' Iterate through all the notepads and update each menu.
                For j = 1 To UBound(Document)
                    If Not FState(j).Deleted Then
                        Document(j).mnuRecentFile(0).Visible = True
                        Document(j).mnuRecentFile(i).Text = IniString
                        Document(j).mnuRecentFile(i).Visible = True
                    End If
                Next j
            End If
        Next i

    End Sub

    Sub OptionsToolbarProc(ByRef CurrentForm As System.Windows.Forms.Form, ByVal inChecked As Boolean)
        If CurrentForm.Name = "frmPDFView" Then
            frmPDFView.DefInstance.mnuOToolbar.Checked = Not inChecked
            frmMDI.DefInstance.mnuOToolbar.Checked = frmPDFView.DefInstance.mnuOToolbar.Checked
        Else
            frmMDI.DefInstance.mnuOToolbar.Checked = Not inChecked
        End If

        If frmMDI.DefInstance.mnuOToolbar.Checked Then
            frmMDI.DefInstance.picToolbar.Visible = True
        Else
            frmMDI.DefInstance.picToolbar.Visible = False
        End If
    End Sub

    Sub WriteRecentFiles(ByRef OpenFileName As String)
        Dim key As String
        Dim RetVal As Long
        Dim i As Integer
        Dim IniString As String
        IniString = New String(Chr(0), 255)

        ' Copy RecentFile1 to RecentFile2, etc.
        For i = 3 To 1 Step -1
            key = "RecentFile" & i
            RetVal = GetPrivateProfileStringA("Recent Files", key, "Not Used", IniString, Len(IniString), "MDINOTE.INI")
            If RetVal And Left(IniString, 8) <> "Not Used" Then
                key = "RecentFile" & (i + 1)
                RetVal = WritePrivateProfileStringA("Recent Files", key, IniString, "MDINOTE.INI")
            End If
        Next i

        ' Write openfile to first Recent File.
        RetVal = WritePrivateProfileStringA("Recent Files", "RecentFile1", OpenFileName, "MDINOTE.INI")

    End Sub
End Module