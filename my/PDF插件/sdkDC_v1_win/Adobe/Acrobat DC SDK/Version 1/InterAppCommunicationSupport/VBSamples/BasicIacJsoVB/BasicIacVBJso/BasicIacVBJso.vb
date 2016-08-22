Option Strict Off
Option Explicit On
Module EntryPoint
	
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
	'BasicIacVBJso
	'- This simple VB sample demonstrates how to use Acrobat IAC JavaScript Object
	'in VB applications. It includes the minimum code to create IAC object CAcroPDDoc
	'and AFormApp, and use their properties and methods. The program simply opens a
    'PDF file ( IAC\TestFiles\TestForm.pdf ), and get some information about the document
	'( number of pages, number of words, and number of form fields ) as well as Acrobat Viewer.
    '----------------------------------------------------------------------
    'The purpose of the sample is to provide simple code for VB developers
	'to get started with Acrobat JavaScript Programming in VB. Based on this project, you
	'should be easily add more code to improve and extend the capability, or to use
	'other properties and methods of the Acrobat IAC objects.
	'----------------------------------------------------------------------
	'The way to set up using Acrobat IAC in the project is from the menu
	' Project -> Preferences ... to select Acrobat and AcroFormAut type libraries.
	'------------------------------------------------------------------------
	
	' global variables
	Public jsObj As Object


    Public Sub Main()

        ' Create a PDDoc IAC object.
        ' Since we don't need to launch Acrobat Viewer on the screen, we don't have
        ' to create Acrobat App object.
        Dim pdDoc As Acrobat.CAcroPDDoc
        pdDoc = CreateObject("AcroExch.PDDoc")
        If pdDoc Is Nothing Then
            MsgBox("Failed to create Acrobat PDDoc object.")
            End
        End If

        ' Open the source PDF document
        Dim SampleFilePath As String = System.Windows.Forms.Application.StartupPath & "\..\..\..\..\TestFiles\TestForm.pdf"

        Dim rc As Integer
        rc = pdDoc.Open(SampleFilePath)
        If rc <> -1 Then
            MsgBox("Failed to open PDF document " & SampleFilePath)
            End
        End If

        ' Acquire the Acrobat JavaScript Object interface from the PDDoc object
        jsObj = pdDoc.GetJSObject
        Dim n As Short
        n = jsObj.numPages

        'Get some data from SampleFilePath
        Dim str_Renamed As String
        str_Renamed = "A PDF document is opened using Acrobat by the BasicIacVBJso program." & vbCrLf
        str_Renamed = str_Renamed & "Path of the PDF document : " & SampleFilePath & vbCrLf
        str_Renamed = str_Renamed & "Number of pages in the PDF document : " & jsObj.numPages & vbCrLf
        str_Renamed = str_Renamed & "Number of words on the first page : " & jsObj.getPageNumWords(0) & vbCrLf
        MsgBox(str_Renamed)

        ' Open AVDoc of the source PDF document in order to use AcroForm functionality.
        Dim avDoc As Acrobat.CAcroAVDoc
        avDoc = pdDoc.OpenAVDoc("Sample PDF File")

        ' get acrobat form object
        Dim formApp As AFORMAUTLib.AFormApp
        formApp = CreateObject("AFormAut.App")

        ' access some object property in objects inside AcroForm.
        Dim fields As AFORMAUTLib.Fields
        fields = formApp.Fields
        str_Renamed = "Number of form fields in the document : " & fields.Count & vbCrLf
        MsgBox(str_Renamed)

        ' another way to use a JavaScript code in VB is through fields' method ExecuteThisJavascript.
        Dim nVersion As Short
        nVersion = CShort(fields.ExecuteThisJavascript("event.value = app.viewerVersion;"))
        MsgBox("The Acrobat Viewer Version is " & nVersion)

        'finish
        avDoc.Close(True)

        avDoc = Nothing
        pdDoc = Nothing
        jsObj = Nothing
        formApp = Nothing
        fields = Nothing

    End Sub
End Module