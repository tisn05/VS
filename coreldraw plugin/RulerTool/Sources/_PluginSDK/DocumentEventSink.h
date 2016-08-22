/////////////////////////////////////////////////////////////////////
// Module: DocumentEventSink.h
// Author: Alex Vakulenko, http://www.oberonplace.com/cgs-plugin-sdk
// Copyright 2014 by Alex Vakulenko
//
// Description:
//  DocumentEventSink is a base class that adds simple C++ interface
//  for implementing Document event listeners in plugins.
//  Derive your class from PluginSDK::DocumentEventSink and override
//  one or more of the event handlers.
//
//  Here is an example of adding OnDocumentNew event handler to your plugin:
//
//    class MyDocumentEvents final : public PluginSDK::DocumentEventSink
//    {
//    public:
//      // Document event handlers
//      void OnShapeCreate(VGCore::IVGShapePtr Shape) override
//      {
//        Shape->Fill->ApplyUniformFill(m_doc->Application->CreateCMYKColor(0, 0, 100, 0));
//      }
//    };
//    
/////////////////////////////////////////////////////////////////////

#ifndef __PLUGINSDK_DOCUMENT_EVENT_SINK_H
#define __PLUGINSDK_DOCUMENT_EVENT_SINK_H
#pragma once

#include "HostDispatchImpl.h"

namespace PluginSDK {

class DocumentEventSink : public IHostDispatchImpl<VGCore::IVGDocumentEvents> {
public:
  DocumentEventSink() {}

  bool Subscribe(VGCore::IVGDocumentPtr doc);
  bool Unsubscribe();

protected:
  virtual void OnQueryClose(bool& Cancel) {}
  virtual void OnQuerySave(bool& Cancel) {}
  virtual void OnQueryPrint(bool& Cancel) {}
  virtual void OnQueryExport(bool& Cancel) {}
  virtual void OnOpen() {}
  virtual void OnClose() {}
  virtual void OnBeforeSave(bool SaveAs, CString FileName) {}
  virtual void OnAfterSave(bool SaveAs, CString FileName) {}
  virtual void OnBeforePrint() {}
  virtual void OnAfterPrint() {}
  virtual void OnBeforeExport(CString FileName, VGCore::cdrFilter Filter, bool SaveBitmap) {}
  virtual void OnAfterExport(CString FileName, VGCore::cdrFilter Filter, bool SaveBitmap) {}
  virtual void OnLayerCreate(VGCore::IVGLayerPtr Layer) {}
  virtual void OnLayerDelete(long Count) {}
  virtual void OnLayerActivate(VGCore::IVGLayerPtr Layer) {}
  virtual void OnLayerChange(VGCore::IVGLayerPtr Layer) {}
  virtual void OnPageCreate(VGCore::IVGPagePtr Page) {}
  virtual void OnPageDelete(long Count) {}
  virtual void OnPageActivate(VGCore::IVGPagePtr Page) {}
  virtual void OnPageChange(VGCore::IVGPagePtr Page) {}
  virtual void OnShapeCreate(VGCore::IVGShapePtr Shape) {}
  virtual void OnShapeDelete(long Count) {}
  virtual void OnShapeMove(VGCore::IVGShapePtr Shape) {}
  virtual void OnShapeTransform(VGCore::IVGShapePtr Shape) {}
  virtual void OnShapeDistort(VGCore::IVGShapePtr Shape) {}
  virtual void OnShapeChange(VGCore::IVGShapePtr Shape, VGCore::cdrShapeChangeScope Scope) {}
  virtual void OnSelectionChange() {}

#pragma region IVGDocumentEvents
protected:
  STDMETHOD(raw_QueryClose)(VARIANT_BOOL* Cancel) override;
  STDMETHOD(raw_QuerySave)(VARIANT_BOOL* Cancel) override;
  STDMETHOD(raw_QueryPrint)(VARIANT_BOOL* Cancel) override;
  STDMETHOD(raw_QueryExport)(VARIANT_BOOL* Cancel) override;
  STDMETHOD(raw_Open)() override;
  STDMETHOD(raw_Close)() override;
  STDMETHOD(raw_BeforeSave)(VARIANT_BOOL SaveAs, BSTR FileName) override;
  STDMETHOD(raw_AfterSave)(VARIANT_BOOL SaveAs, BSTR FileName) override;
  STDMETHOD(raw_BeforePrint)() override;
  STDMETHOD(raw_AfterPrint)() override;
  STDMETHOD(raw_BeforeExport)(BSTR FileName, VGCore::cdrFilter Filter, VARIANT_BOOL SaveBitmap) override;
  STDMETHOD(raw_AfterExport)(BSTR FileName, VGCore::cdrFilter Filter, VARIANT_BOOL SaveBitmap) override;
  STDMETHOD(raw_LayerCreate)(VGCore::IVGLayer* Layer) override;
  STDMETHOD(raw_LayerDelete)(long Count) override;
  STDMETHOD(raw_LayerActivate)(VGCore::IVGLayer* Layer) override;
  STDMETHOD(raw_LayerChange)(VGCore::IVGLayer* Layer) override;
  STDMETHOD(raw_PageCreate)(VGCore::IVGPage* Page) override;
  STDMETHOD(raw_PageDelete)(long Count) override;
  STDMETHOD(raw_PageActivate)(VGCore::IVGPage* Page) override;
  STDMETHOD(raw_PageChange)(VGCore::IVGPage* Page) override;
  STDMETHOD(raw_ShapeCreate)(VGCore::IVGShape* Shape) override;
  STDMETHOD(raw_ShapeDelete)(long Count) override;
  STDMETHOD(raw_ShapeMove)(VGCore::IVGShape* Shape) override;
  STDMETHOD(raw_ShapeTransform)(VGCore::IVGShape* Shape) override;
  STDMETHOD(raw_ShapeDistort)(VGCore::IVGShape* Shape) override;
  STDMETHOD(raw_ShapeChange)(VGCore::IVGShape* Shape, VGCore::cdrShapeChangeScope Scope) override;
  STDMETHOD(raw_SelectionChange)() override;
#pragma endregion Raw methods

protected:
  VGCore::IVGDocumentPtr m_doc;
  long m_cookie = 0;
};

} // namespace PluginSDK

#endif // __PLUGINSDK_DOCUMENT_EVENT_SINK_H
