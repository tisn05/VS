' ADOBE SYSTEMS INCORPORATED
' Copyright (C) 1994-2004 Adobe Systems Incorporated
' All rights reserved.
'
' NOTICE: Adobe permits you to use, modify, and distribute this file
' in accordance with the terms of the Adobe license agreement
' accompanying it. If you have received this file from a source other
' than Adobe, then your use, modification, or distribution of it
' requires the prior written permission of Adobe.
'
' -------------------------------------------------------------------
'JSOWatermarkVB
'
'- This VB sample demonstrates how to add watermarks to a PDF file programmatically.
'  Acrobat IAC JavaScript Object is used in the VB application. 
'- Two methods, addWatermarkFromFile and addWatermarkFromText, are used to add a "Received"
'  stamp with date and time to the left-top corner of the first page in the PDF document. 
'- The program will open IAC\TestFiles\SamplePDF01.pdf and add IAC\TestFiles\Stamp.pdf 
'   as a water mark. It will save the resulting file in the root directory of the sample.  
' 
'------------------------------------------------------------------------

Option Strict Off
Option Explicit On
Module EntryPoint
	
    ' global variables
    Public jsObj As Object

    Public Sub Main()

        Dim SamplePDFFilePath As String = System.Windows.Forms.Application.StartupPath + "\..\..\..\..\TestFiles\SamplePDF01.pdf"
        Dim SampleImageFilePath As String = System.Windows.Forms.Application.StartupPath + "\..\..\..\..\TestFiles\Stamp.pdf"
        Dim OutputFilePath As String = System.Windows.Forms.Application.StartupPath + "\..\..\WatermarkOutput01.pdf"

        ' Create a PDDoc IAC object.
        Dim pdDoc As Acrobat.CAcroPDDoc
        pdDoc = CreateObject("AcroExch.PDDoc")
        If pdDoc Is Nothing Then
            MsgBox("Failed to create Acrobat PDDoc object.")
            End
        End If

        ' Open the source PDF document
        Dim rc As Integer
        rc = pdDoc.Open(SamplePDFFilePath)
        If rc <> -1 Then
            MsgBox("Failed to open PDF document " & SamplePDFFilePath)
            End
        End If

        ' Acquire the Acrobat JavaScript Object interface from the PDDoc object
        jsObj = pdDoc.GetJSObject

        ' Add a watermark from a file.
        ' function prototype:
        '   addWatermarkFromFile(cDIPath, nSourcePage, nStart, nEnd, bOnTop, bOnScreen, bOnPrint, nHorizAlign, nVertAlign, nHorizValue, nVertValue, bPercentage, nScale, bFixedPrint, nRotation, nOpacity)
        jsObj.addWatermarkFromFile(SampleImageFilePath, 0, 0, 0, True, True, True, 0, 3, 10, -10, False, 0.4, False, 0, 0.7)

        ' get current time and make a string from it
        Dim currentTime As String
        currentTime = Now.ToString

        ' make a color object
        Dim oColor As Object
        oColor = jsObj.color.blue()

        ' Add a text watermark.
        ' function prototype:
        '   addWatermarkFromText(cText, nTextAlign, cFont, nFontSize, oColor, nStart, nEnd, bOnTop, bOnScreen, bOnPrint, nHorizAlign, nVertAlign, nHorizValue, nVertValue, bPercentage, nScale, bFixedPrint, nRotation, nOpacity)
        jsObj.addWatermarkFromText(currentTime, 1, "Helvetica", 10, oColor, 0, 0, True, True, True, 0, 3, 20, -45, False, 1.0, False, 0, 0.7)

        ' save the PDF with watermarks to a new document.
        rc = pdDoc.Save(1, OutputFilePath)  ' full save

        ' close original PDF.
        pdDoc.Close()
        pdDoc = Nothing
        jsObj = Nothing

        ' to clean up, get the Acrobat application,  
        ' close Acrobat if there are no open documents.
        Dim gApp As Acrobat.CAcroApp
        gApp = CreateObject("AcroExch.App")
        If gApp.GetNumAVDocs = 0 Then
            gApp.CloseAllDocs()
            gApp.Exit()
        End If

        ' show message. You can comment it out if you really need a quiet operation.
        If rc = True Then
            MsgBox("PDF with the watermark was saved to " & OutputFilePath)
        Else
            MsgBox("Operation was failed.")
        End If

    End Sub
End Module