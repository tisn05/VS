// TextCreatorCppSmartPtr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// The following #ifdef is a hack around Visual Studio's IntelliSense not
// parsing the generated header files from the type library. #import directive
// produces VGCoreAuto.tlh in the output directory and then compiled, however
// IntelliSense doesn't know anything about any of those types and highlight
// every VGCore interface in this file as an error. This hack is to include
// a pre-generated header file for IntelliSense only.
#ifndef __INTELLISENSE__
#import "libid:95E23C91-BC5A-49F3-8CD1-1FC515597048" version("11.5") \
      rename("GetCommandLine", "VGGetCommandLine") \
      rename("CopyFile", "VGCopyFile") \
      rename("FindWindow", "VGFindWindow")
#else
// If you are not building 32-bit Debug target, update this include path, so
// the IntelliSense manages to load the generated header file for the type library.
#include "Debug/VGCoreAuto.tlh"
#endif

void CreateTextInCorelDRAW(const wchar_t* text, const wchar_t* fontName, float fontSize)
{
  VGCore::IVGApplicationPtr app(L"CorelDRAW.Application.17");
  app->Visible = VARIANT_TRUE;

  VGCore::IVGDocumentPtr doc = app->ActiveDocument;
  if (!doc)
    doc = app->CreateDocument();

  VGCore::IVGShapePtr shape = doc->ActiveLayer->CreateArtisticText(
    0.0, 0.0, text, VGCore::cdrTextLanguage::cdrLanguageMixed,
    VGCore::cdrTextCharSet::cdrCharSetMixed, fontName, fontSize,
    VGCore::cdrTriState::cdrUndefined, VGCore::cdrTriState::cdrUndefined,
    VGCore::cdrFontLine::cdrMixedFontLine,
    VGCore::cdrAlignment::cdrLeftAlignment);
}

int wmain(int argc, wchar_t* argv[])
{
  HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  if (SUCCEEDED(hr))
  {
    try
    {
      CreateTextInCorelDRAW(L"Hello, world", L"Arial", 24.0f);
    }
    catch (_com_error& ex)
    {
      wprintf(L"Error occurred: 0x%08X (%s)\n", ex.Error(), ex.ErrorMessage());
    }
    CoUninitialize();
  }
  return 0;
}

