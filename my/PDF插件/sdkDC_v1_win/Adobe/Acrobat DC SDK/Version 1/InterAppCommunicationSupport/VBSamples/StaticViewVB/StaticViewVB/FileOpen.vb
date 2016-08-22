Option Strict Off
Option Explicit On
Module FILOPEN
	
	' ADOBE SYSTEMS INCORPORATED
	' Copyright (C) 1994-2003 Adobe Systems Incorporated
	' All rights reserved.
	'
	' NOTICE: Adobe permits you to use, modify, and distribute this file
	' in accordance with the terms of the Adobe license agreement
	' accompanying it. If you have received this file from a source other
	' than Adobe, then your use, modification, or distribution of it
	' requires the prior written permission of Adobe.
	
	Sub FOpenProc()
        On Error Resume Next
        Dim OpenFileName As String
        Dim ofDialog As New OpenFileDialog

        ofDialog.Filter = "PDF Files (*.pdf)|*.pdf"
        If ofDialog.ShowDialog = DialogResult.OK Then
            OpenFileName = ofDialog.FileName
            OpenFile((OpenFileName))
            UpdateFileMenu((OpenFileName))
        End If
    End Sub
	
    Function OnRecentFilesList(ByRef Filename As String) As Boolean
        Dim i As Short

        For i = 1 To 4
            If frmMDI.DefInstance.mnuRecentFile(i).Text = Filename Then
                OnRecentFilesList = True
                Exit Function
            End If
        Next i
        OnRecentFilesList = False
    End Function

    Sub OpenFile(ByRef Filename As String)
        Dim ok As Integer
        Dim fIndex As Short

        ' change mousepointer to an hourglass
        System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.WaitCursor

        ' change form's caption and display new text
        fIndex = FindFreeIndex()
        Document(fIndex) = New frmPDFView
        Document(fIndex).Tag = fIndex
        Document(fIndex).Text = UCase(Filename)

        ' open the selected file
        AcroExchPDDoc(fIndex) = CreateObject("AcroExch.PDDoc")
        ok = AcroExchPDDoc(fIndex).Open(Filename)
        If Not ok Then
            MsgBox("Can't open file: " + Filename)
            Document(fIndex).Close()
            System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.Default
            Exit Sub
        End If


        FState(fIndex).Dirty = False
        FState(fIndex).nPage = 0
        Document(fIndex).Show()

        ' reset mouse pointer
        System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.Default
    End Sub

    Sub SaveFileAs(ByRef Filename As String)
        Dim ok As Integer
        On Error Resume Next
        ' display hourglass
        System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.WaitCursor

        ok = AcroExchPDDoc(CInt(frmMDI.DefInstance.ActiveMdiChild.Tag)).Save(PDSaveFull, Filename)

        If Not ok Then
            MsgBox("Can't Save As file: " + Filename)
            System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.Default
            Exit Sub
        End If

        ' reset the mousepointer
        System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.Default

        ' set the caption
        frmMDI.DefInstance.ActiveMdiChild.Text = UCase(Filename)

        ' reset the dirty flag
        FState(CInt(frmMDI.DefInstance.ActiveMdiChild.Tag)).Dirty = False

    End Sub

    Sub UpdateFileMenu(ByRef Filename As String)
        Dim RetVal As Boolean
        ' Check if OpenFileName is already on MRU list.
        RetVal = OnRecentFilesList(Filename)
        If Not RetVal Then
            ' Write OpenFileName to MDINOTEPAD.INI
            WriteRecentFiles((Filename))
        End If
        ' Update menus for most recent file list.
        GetRecentFiles()
    End Sub
End Module