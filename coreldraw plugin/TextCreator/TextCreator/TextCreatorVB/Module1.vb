Imports Corel.Interop.VGCore

Module Module1

  Sub CreateTextInCorelDRAW(text As String, fontName As String, fontSize As Single)
    Dim pia_type As Type = Type.GetTypeFromProgID("CorelDRAW.Application.17")
    Dim app As Application = Activator.CreateInstance(pia_type)

    app.Visible = True

    Dim doc As Document = app.ActiveDocument
    If doc Is Nothing Then doc = app.CreateDocument()

    Dim shape As Shape = doc.ActiveLayer.CreateArtisticText(
      0.0, 0.0, text, cdrTextLanguage.cdrLanguageMixed,
      cdrTextCharSet.cdrCharSetMixed, fontName, fontSize,
      cdrTriState.cdrUndefined, cdrTriState.cdrUndefined,
      cdrFontLine.cdrMixedFontLine, cdrAlignment.cdrLeftAlignment)
  End Sub

  Sub Main()
    Try
      CreateTextInCorelDRAW("Hello, world", "Arial", 24.0F)
    Catch ex As Exception
      Console.WriteLine("Error occurred: {0}", ex.Message)
    End Try
  End Sub

End Module
