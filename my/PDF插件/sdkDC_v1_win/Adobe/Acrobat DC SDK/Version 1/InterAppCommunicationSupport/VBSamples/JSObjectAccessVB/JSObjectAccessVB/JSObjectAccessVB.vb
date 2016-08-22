Option Strict Off
Option Explicit On
Module EntryPointJSObjectAccessVB

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
    ' For more information on accessing the JavaScript DOM through
    ' Acrobat's automation interface, please see the document
    ' "Programming Acrobat JavaScript Using Visual Basic" 
    ' (VBJavaScript.pdf) located in the SDK's Documentation folder.

    Public jsObj As Object
    Private Const NUM_ROWS As Short = 7

    Public Sub Main()
        Dim SOURCE_DOCUMENT As String = System.Windows.Forms.Application.StartupPath + "\..\..\..\..\TestFiles\FormSample.pdf"
        Dim DATA_FILE As String = System.Windows.Forms.Application.StartupPath + "\..\..\..\..\TestFiles\data.txt"

        ' create at root level of sample
        Dim OUTPUT_FOLDER As String = System.Windows.Forms.Application.StartupPath + "/../../outputfolder/"
        System.IO.Directory.CreateDirectory(OUTPUT_FOLDER)
        Dim outputString As String
        Dim i As Short

        ' We gain access to the JavaScript DOM through the GetJSObject method
        ' of the CAcroPDDoc object.

        On Error GoTo ErrorHandler

        Dim retVal As Integer
        Dim count As Integer
        Dim srcDoc As Acrobat.CAcroPDDoc

        ' Open the source PDF document

        srcDoc = CreateObject("AcroExch.PDDoc")
        retVal = srcDoc.Open(SOURCE_DOCUMENT)
        If retVal <> -1 Then
            Err.Description = "Failed to open source document. Make sure that SOURCE_DOCUMENT has been configured."
            GoTo ErrorHandler
        End If

        ' Acquire the JSObject interface

        jsObj = srcDoc.GetJSObject

        ' For each row in data.txt, as defined by the NUM_ROWS constant, we:
        '
        ' 1. Spawn the form template.
        ' 2. Populate the form with the data from data.txt.
        ' 3. Save the PDF.

        Dim newDoc As Object
        Dim template As Object
        For i = 1 To NUM_ROWS


            ' Acquire the form template and spawn a new page
            template = jsObj.getTemplate("Templates:1")
            If template Is Nothing Then
                Err.Description = "Failed to acquire form template. Aborting..."
                GoTo ErrorHandler
            End If
            template.spawn(1, False, False)

            ' Extract the page and populate the form with the data
            newDoc = jsObj.extractPages(1)
            newDoc.importTextData(DATA_FILE, i - 1)

            ' Save the new PDF to the output folder
            outputString = OUTPUT_FOLDER & "Output" & i & ".pdf"
            newDoc.saveAs(outputString)
            newDoc.closeDoc()
        Next

EndSub:
        System.Runtime.InteropServices.Marshal.ReleaseComObject(jsObj)
        jsObj = Nothing
        If Not srcDoc Is Nothing Then
            srcDoc.Close()
            System.Runtime.InteropServices.Marshal.ReleaseComObject(srcDoc)
            srcDoc = Nothing
        End If
        Exit Sub

ErrorHandler:

        MsgBox("Error Occured: " & Err.Description)
        Resume EndSub

    End Sub
End Module