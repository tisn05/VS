#include "stdafx.h"
#include "DocumentEventSink.h"

namespace PluginSDK
{

bool DocumentEventSink::Subscribe(VGCore::IVGDocumentPtr doc) {
  if (m_doc) {
    // Unexpected: already subscribed to events.
    return false;
  }

  try {
    m_cookie = doc->AdviseEvents(this);
    m_doc = doc;
    SetHostObject(doc);
  }
  catch (_com_error&) {
    return false;
  }
  return true;
}

bool DocumentEventSink::Unsubscribe() {
  if (!m_doc) {
    // Unexpected: not subscribed to events.
    return false;
  }

  try {
    m_doc->UnadviseEvents(m_cookie);
    m_doc.Release();
    SetHostObject(nullptr);
  }
  catch (_com_error&) {
    return false;
  }
  return true;
}

STDMETHODIMP DocumentEventSink::raw_QueryClose(VARIANT_BOOL* Cancel) {
  bool bCancel = ToBool(*Cancel);
  OnQueryClose(bCancel);
  *Cancel = ToVariantBool(bCancel);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_QuerySave(VARIANT_BOOL* Cancel) {
  bool bCancel = ToBool(*Cancel);
  OnQuerySave(bCancel);
  *Cancel = ToVariantBool(bCancel);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_QueryPrint(VARIANT_BOOL* Cancel) {
  bool bCancel = ToBool(*Cancel);
  OnQueryPrint(bCancel);
  *Cancel = ToVariantBool(bCancel);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_QueryExport(VARIANT_BOOL* Cancel) {
  bool bCancel = ToBool(*Cancel);
  OnQueryExport(bCancel);
  *Cancel = ToVariantBool(bCancel);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_Open() {
  OnOpen();
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_Close() {
  OnClose();
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_BeforeSave(VARIANT_BOOL SaveAs, BSTR FileName) {
  OnBeforeSave(ToBool(SaveAs), FileName);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_AfterSave(VARIANT_BOOL SaveAs, BSTR FileName) {
  OnAfterSave(ToBool(SaveAs), FileName);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_BeforePrint() {
  OnBeforePrint();
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_AfterPrint() {
  OnAfterPrint();
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_BeforeExport(BSTR FileName, VGCore::cdrFilter Filter, VARIANT_BOOL SaveBitmap) {
  OnBeforeExport(FileName, Filter, ToBool(SaveBitmap));
  return S_OK;
}
STDMETHODIMP DocumentEventSink::raw_AfterExport(BSTR FileName, VGCore::cdrFilter Filter, VARIANT_BOOL SaveBitmap) {
  OnAfterExport(FileName, Filter, ToBool(SaveBitmap));
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_LayerCreate(VGCore::IVGLayer* Layer) {
  OnLayerCreate(Layer);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_LayerDelete(long Count) {
  OnLayerDelete(Count);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_LayerActivate(VGCore::IVGLayer* Layer) {
  OnLayerActivate(Layer);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_LayerChange(VGCore::IVGLayer* Layer) {
  OnLayerChange(Layer);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_PageCreate(VGCore::IVGPage* Page) {
  OnPageCreate(Page);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_PageDelete(long Count) {
  OnPageDelete(Count);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_PageActivate(VGCore::IVGPage* Page) {
  OnPageActivate(Page);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_PageChange(VGCore::IVGPage* Page) {
  OnPageChange(Page);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_ShapeCreate(VGCore::IVGShape* Shape) {
  OnShapeCreate(Shape);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_ShapeDelete(long Count) {
  OnShapeDelete(Count);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_ShapeMove(VGCore::IVGShape* Shape) {
  OnShapeMove(Shape);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_ShapeTransform(VGCore::IVGShape* Shape) {
  OnShapeTransform(Shape);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_ShapeDistort(VGCore::IVGShape* Shape) {
  OnShapeDistort(Shape);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_ShapeChange(VGCore::IVGShape* Shape, VGCore::cdrShapeChangeScope Scope) {
  OnShapeChange(Shape, Scope);
  return S_OK;
}

STDMETHODIMP DocumentEventSink::raw_SelectionChange() {
  OnSelectionChange();
  return S_OK;
}

} // namespace PluginSDK
