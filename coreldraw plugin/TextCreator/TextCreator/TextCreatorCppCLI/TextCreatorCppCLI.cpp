// TextCreatorCppCLI.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace Corel::Interop;

void CreateTextInCorelDRAW(const wchar_t* text, const wchar_t* fontName, float fontSize)
{
  Type^ pia_type = Type::GetTypeFromProgID("CorelDRAW.Application.17");
  VGCore::Application^ app = static_cast<VGCore::Application^>(Activator::CreateInstance(pia_type));

  app->Visible = true;

  VGCore::Document^ doc = app->ActiveDocument;
  if (!doc)
    doc = app->CreateDocument();

  VGCore::Shape^ shape = doc->ActiveLayer->CreateArtisticText(
    0.0, 0.0, gcnew String(text), VGCore::cdrTextLanguage::cdrLanguageMixed,
    VGCore::cdrTextCharSet::cdrCharSetMixed, gcnew String(fontName), fontSize,
    VGCore::cdrTriState::cdrUndefined, VGCore::cdrTriState::cdrUndefined,
    VGCore::cdrFontLine::cdrMixedFontLine,
    VGCore::cdrAlignment::cdrLeftAlignment);
}

int main(array<System::String ^> ^args)
{
  try
  {
    CreateTextInCorelDRAW(L"Hello, world", L"Arial", 24.0f);
  }
  catch (Exception^ ex)
  {
    Console::WriteLine(L"Error occurred: {0}", ex->Message);
  }
  return 0;
}
