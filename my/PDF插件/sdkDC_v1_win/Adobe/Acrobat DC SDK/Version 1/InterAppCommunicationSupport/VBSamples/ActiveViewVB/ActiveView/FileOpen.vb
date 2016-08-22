Option Strict Off
Option Explicit On
Module FileOpen_Renamed
	
	' ADOBE SYSTEMS INCORPORATED
    ' Copyright (C) 1994-2005 Adobe Systems Incorporated
	' All rights reserved.
	'
	' NOTICE: Adobe permits you to use, modify, and distribute this file
	' in accordance with the terms of the Adobe license agreement
	' accompanying it. If you have received this file from a source other
	' than Adobe, then your use, modification, or distribution of it
	' requires the prior written permission of Adobe.
	
	Sub FOpenProc()
        'Dim RetVal As Object
		On Error Resume Next
        Dim OpenFileName As String
        Dim OpenDlg As New OpenFileDialog

        OpenDlg.FileName = ""
        OpenDlg.DefaultExt = "pdf"
        OpenDlg.Filter = "PDF Files (*.pdf)|*.pdf"

        If OpenDlg.ShowDialog() = DialogResult.OK Then
            OpenFileName = OpenDlg.FileName
            OpenFile(OpenFileName)
            UpdateFileMenu(OpenFileName)
        End If
	End Sub
	
    Function GetFileName() As String
        'Displays a Save As dialog and returns a file name
        'or an empty string if the user cancels
        On Error Resume Next

        Dim SaveDialog As New SaveFileDialog
        SaveDialog.OverwritePrompt = True
        SaveDialog.FileName = ""
        SaveDialog.DefaultExt = "pdf"
        SaveDialog.Filter = "PDF Files (*.pdf)|*.pdf"

        If SaveDialog.ShowDialog() = DialogResult.OK Then
            GetFileName = SaveDialog.FileName

        Else
            GetFileName = ""
        End If
    End Function

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
        Document(fIndex).Show()
        Document(fIndex).Tag = fIndex
        Document(fIndex).Text = UCase(Filename)

        ' open the selected file
        AcroExchAVDoc(fIndex) = CreateObject("AcroExch.AVDoc")
        ok = AcroExchAVDoc(fIndex).OpenInWindowEx(Filename, Document(fIndex).Handle.ToInt32, AV_DOC_VIEW, True, 0, PDUseBookmarks, AVZoomFitWidth, 0, 0, 0)

        ' See IAC.bas and the Acrobat Viewer IAC Overview for information on the
        ' OpenInWindowEx open modes

        If Not ok Then
            MsgBox("Can't open file: " + Filename)
            Document(fIndex).Close()
            System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.Default
            Exit Sub
        End If


        FState(fIndex).Dirty = False
        Document(fIndex).Show()

        ' reset mouse pointer
        System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.Default
    End Sub

    Sub SaveFileAs(ByRef Filename As String)
        Dim ok As Integer
        On Error Resume Next
        ' display hourglass
        System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.WaitCursor

        Dim nForm As Short
        nForm = CShort(frmMDI.DefInstance.ActiveMdiChild.Tag)

        Dim AcroPDDoc As Acrobat.CAcroPDDoc
        AcroPDDoc = AcroExchAVDoc(nForm).GetPDDoc()
        ok = AcroPDDoc.Save(PDSaveFull + PDSaveCollectGarbage + PDSaveLinearized, Filename)

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
        FState(nForm).Dirty = False

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