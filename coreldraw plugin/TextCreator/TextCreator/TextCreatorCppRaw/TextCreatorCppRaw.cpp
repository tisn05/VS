// TextCreatorCppRaw.cpp : Defines the entry point for the console application.
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
      raw_interfaces_only no_smart_pointers \
      rename("GetCommandLine", "VGGetCommandLine") \
      rename("CopyFile", "VGCopyFile") \
      rename("FindWindow", "VGFindWindow")
#else
// If you are not building 32-bit Debug target, update this include path, so
// the IntelliSense manages to load the generated header file for the type library.
#include "Debug/VGCoreAuto.tlh"
#endif

HRESULT CreateText(VGCore::IVGApplication* app, const wchar_t* text,
                   const wchar_t* fontName, float fontSize)
{
  HRESULT hr = app->put_Visible(VARIANT_TRUE);
  if (FAILED(hr))
    return hr;

  VGCore::IVGDocument* doc = nullptr;
  hr = app->get_ActiveDocument(&doc);
  if (FAILED(hr))
    return hr;

  if (doc == nullptr)
  {
    hr = app->CreateDocument(&doc);
    if (FAILED(hr))
      return hr;
  }

  VGCore::IVGLayer* active_layer = nullptr;
  hr = doc->get_ActiveLayer(&active_layer);
  if (SUCCEEDED(hr))
  {
    BSTR bstr_text = SysAllocString(text);
    BSTR bstr_font = SysAllocString(fontName);
    VGCore::IVGShape* shape = nullptr;

    hr = active_layer->CreateArtisticText(
      0.0, 0.0, bstr_text, VGCore::cdrTextLanguage::cdrLanguageMixed,
      VGCore::cdrTextCharSet::cdrCharSetMixed, bstr_font, fontSize,
      VGCore::cdrTriState::cdrUndefined, VGCore::cdrTriState::cdrUndefined,
      VGCore::cdrFontLine::cdrMixedFontLine,
      VGCore::cdrAlignment::cdrLeftAlignment, &shape);

    SysFreeString(bstr_font);
    SysFreeString(bstr_text);
    if (SUCCEEDED(hr))
      shape->Release();
  }
  active_layer->Release();
  return hr;
}

HRESULT CreateTextInCorelDRAW(const wchar_t* text, const wchar_t* fontName,
                              float fontSize)
{
  CLSID clsid;
  HRESULT hr = CLSIDFromProgID(L"CorelDRAW.Application.17", &clsid);
  if (FAILED(hr))
    return hr;

  VGCore::IVGApplication* app = nullptr;
  hr = CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER,
                        __uuidof(VGCore::IVGApplication),
                        reinterpret_cast<void**>(&app));
  if (FAILED(hr))
    return hr;

  hr = CreateText(app, text, fontName, fontSize);
  app->Release();
  return hr;
}

int wmain(int argc, wchar_t* argv[])
{
  HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  if (SUCCEEDED(hr))
  {
    hr = CreateTextInCorelDRAW(L"Hello, world", L"Arial", 24.0f);
    if (FAILED(hr))
    {
      wprintf(L"Error occurred: 0x%08X\n", hr);
    }
    CoUninitialize();
  }
  return 0;
}

