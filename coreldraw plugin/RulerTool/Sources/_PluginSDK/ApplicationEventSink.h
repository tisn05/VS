/////////////////////////////////////////////////////////////////////
// Module: ApplicationEventSink.h
// Author: Alex Vakulenko, http://www.oberonplace.com/cgs-plugin-sdk
// Copyright 2014 by Alex Vakulenko
//
// Description:
//  ApplicationEventSink is a base class that adds simple C++ interface
//  for implementing Application event listeners in plugins.
//  Derive your class from PluginSDK::ApplicationEventSink and override
//  one or more of the event handlers.
//
//  Here is an example of adding OnDocumentNew event handler to your plugin:
//
//    class MyPlugin final : public PluginSDK::AppPlugin, public PluginSDK::ApplicationEventSink
//    {
//    public:
//      bool OnLoad() override { Subscribe(m_app); return true; }
//      bool OnUnload() override { Unsubscribe(); return true; }
//    
//      // Application event handlers
//      void OnDocumentNew(VGCore::IVGDocumentPtr Doc, bool FromTemplate, CString Template, bool IncludeGraphics) override
//      {
//        auto Shape = Doc->ActiveLayer->CreateRectangleRect(Doc->ActivePage->BoundingBox, 0.0, 0.0, 0.0, 0.0);
//        Shape->Fill->ApplyUniformFill(m_app->CreateCMYKColor(0, 0, 100, 0));
//        Shape->Locked = VARIANT_TRUE;
//        Doc->ClearSelection();
//      }
//    };
//    
/////////////////////////////////////////////////////////////////////

#ifndef __PLUGINSDK_APPLICATION_EVENT_SINK_H
#define __PLUGINSDK_APPLICATION_EVENT_SINK_H
#pragma once

#include <vector>

#include "HostDispatchImpl.h"

namespace PluginSDK {

class ApplicationEventSink : public IHostDispatchImpl<VGCore::IVGApplicationEvents> {
public:
  ApplicationEventSink() {}

  bool Subscribe(VGCore::IVGApplicationPtr app);
  bool Unsubscribe();

public:
  virtual void OnQueryDocumentClose(VGCore::IVGDocumentPtr Doc, bool& Cancel) {}
  virtual void OnQueryDocumentSave(VGCore::IVGDocumentPtr Doc, bool& Cancel) {}
  virtual void OnQueryDocumentPrint(VGCore::IVGDocumentPtr Doc, bool& Cancel) {}
  virtual void OnQueryDocumentExport(VGCore::IVGDocumentPtr Doc, bool& Cancel) {}
  virtual void OnQueryQuit(bool& Cancel) {}
  virtual void OnDocumentOpen(VGCore::IVGDocumentPtr Doc, CString FileName) {}
  virtual void OnDocumentNew(VGCore::IVGDocumentPtr Doc, bool FromTemplate, CString Template, bool IncludeGraphics) {}
  virtual void OnDocumentClose(VGCore::IVGDocumentPtr Doc) {}
  virtual void OnDocumentBeforeSave(VGCore::IVGDocumentPtr Doc, bool SaveAs, CString FileName) {}
  virtual void OnDocumentAfterSave(VGCore::IVGDocumentPtr Doc, bool SaveAs, CString FileName) {}
  virtual void OnDocumentBeforePrint(VGCore::IVGDocumentPtr Doc) {}
  virtual void OnDocumentAfterPrint(VGCore::IVGDocumentPtr Doc) {}
  virtual void OnDocumentBeforeExport(VGCore::IVGDocumentPtr Doc, CString FileName, VGCore::cdrFilter Filter, bool SaveBitmap) {}
  virtual void OnDocumentAfterExport(VGCore::IVGDocumentPtr Doc, CString FileName, VGCore::cdrFilter Filter, bool SaveBitmap) {}
  virtual void OnWindowActivate(VGCore::IVGDocumentPtr Doc, VGCore::IVGWindowPtr Window) {}
  virtual void OnWindowDeactivate(VGCore::IVGDocumentPtr Doc, VGCore::IVGWindowPtr Window) {}
  virtual void OnSelectionChange() {}
  virtual void OnStart() {}
  virtual void OnQuit() {}
  virtual void OnPluginCommand(CString CommandID) {}
  virtual void OnUpdatePluginCommand(CString CommandID, bool& Enabled, VGCore::cdrCommandCheckState& Checked) {}
  virtual void OnApplicationEvent(CString EventName, std::vector<const VARIANT> const& Parameters) {}

#pragma region IVGApplicationEvents
protected:
  STDMETHOD(raw_QueryDocumentClose)(VGCore::IVGDocument* Doc, VARIANT_BOOL* Cancel) override;
  STDMETHOD(raw_QueryDocumentSave)(VGCore::IVGDocument* Doc, VARIANT_BOOL* Cancel) override;
  STDMETHOD(raw_QueryDocumentPrint)(VGCore::IVGDocument* Doc, VARIANT_BOOL* Cancel) override;
  STDMETHOD(raw_QueryDocumentExport)(VGCore::IVGDocument* Doc, VARIANT_BOOL* Cancel) override;
  STDMETHOD(raw_QueryQuit)(VARIANT_BOOL* Cancel) override;
  STDMETHOD(raw_DocumentOpen)(VGCore::IVGDocument* Doc, BSTR FileName) override;
  STDMETHOD(raw_DocumentNew)(VGCore::IVGDocument* Doc, VARIANT_BOOL FromTemplate, BSTR Template, VARIANT_BOOL IncludeGraphics) override;
  STDMETHOD(raw_DocumentClose)(VGCore::IVGDocument* Doc) override;
  STDMETHOD(raw_DocumentBeforeSave)(VGCore::IVGDocument* Doc, VARIANT_BOOL SaveAs, BSTR FileName) override;
  STDMETHOD(raw_DocumentAfterSave)(VGCore::IVGDocument* Doc, VARIANT_BOOL SaveAs, BSTR FileName) override;
  STDMETHOD(raw_DocumentBeforePrint)(VGCore::IVGDocument* Doc) override;
  STDMETHOD(raw_DocumentAfterPrint)(VGCore::IVGDocument* Doc) override;
  STDMETHOD(raw_DocumentBeforeExport)(VGCore::IVGDocument* Doc, BSTR FileName, VGCore::cdrFilter Filter, VARIANT_BOOL SaveBitmap) override;
  STDMETHOD(raw_DocumentAfterExport)(VGCore::IVGDocument* Doc, BSTR FileName, VGCore::cdrFilter Filter, VARIANT_BOOL SaveBitmap) override;
  STDMETHOD(raw_WindowActivate)(VGCore::IVGDocument* Doc, VGCore::IVGWindow* Window) override;
  STDMETHOD(raw_WindowDeactivate)(VGCore::IVGDocument* Doc, VGCore::IVGWindow* Window) override;
  STDMETHOD(raw_SelectionChange)() override;
  STDMETHOD(raw_Start)() override;
  STDMETHOD(raw_Quit)() override;
  STDMETHOD(raw_OnPluginCommand)(BSTR CommandID) override;
  STDMETHOD(raw_OnUpdatePluginCommand)(BSTR CommandID, VARIANT_BOOL* Enabled, VGCore::cdrCommandCheckState* Checked) override;
  STDMETHOD(raw_OnApplicationEvent)(BSTR EventName, SAFEARRAY** Parameters) override;
#pragma endregion Raw methods

protected:
  VGCore::IVGApplicationPtr m_app;
  long m_cookie{0};
};

} // namespace PluginSDK

#endif // __APPLICATION_EVENT_SINK_H
