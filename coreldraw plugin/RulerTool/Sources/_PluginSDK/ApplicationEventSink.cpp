#include "stdafx.h"
#include "ApplicationEventSink.h"

namespace PluginSDK
{

bool ApplicationEventSink::Subscribe(VGCore::IVGApplicationPtr app) {
  if (m_app) {
    // Unexpected: already subscribed to events.
    return false;
  }

  try {
    m_cookie = app->AdviseEvents(this);
    m_app = app;
    SetHostObject(app);
  }
  catch (_com_error&) {
    return false;
  }
  return true;
}

bool ApplicationEventSink::Unsubscribe() {
  if (!m_app) {
    // Unexpected: not subscribed to events.
    return false;
  }

  try {
    m_app->UnadviseEvents(m_cookie);
    m_app.Release();
    SetHostObject(nullptr);
  }
  catch (_com_error&) {
    return false;
  }
  return true;
}

STDMETHODIMP ApplicationEventSink::raw_QueryDocumentClose(VGCore::IVGDocument* Doc, VARIANT_BOOL* Cancel) {
  bool bCancel = ToBool(*Cancel);
  OnQueryDocumentClose(Doc, bCancel);
  *Cancel = ToVariantBool(bCancel);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_QueryDocumentSave(VGCore::IVGDocument* Doc, VARIANT_BOOL* Cancel) {
  bool bCancel = ToBool(*Cancel);
  OnQueryDocumentSave(Doc, bCancel);
  *Cancel = ToVariantBool(bCancel);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_QueryDocumentPrint(VGCore::IVGDocument* Doc, VARIANT_BOOL* Cancel) {
  bool bCancel = ToBool(*Cancel);
  OnQueryDocumentPrint(Doc, bCancel);
  *Cancel = ToVariantBool(bCancel);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_QueryDocumentExport(VGCore::IVGDocument* Doc, VARIANT_BOOL* Cancel) {
  bool bCancel = ToBool(*Cancel);
  OnQueryDocumentExport(Doc, bCancel);
  *Cancel = ToVariantBool(bCancel);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_QueryQuit(VARIANT_BOOL* Cancel) {
  bool bCancel = ToBool(*Cancel);
  OnQueryQuit(bCancel);
  *Cancel = ToVariantBool(bCancel);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_DocumentOpen(VGCore::IVGDocument* Doc, BSTR FileName) {
  OnDocumentOpen(Doc, FileName);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_DocumentNew(VGCore::IVGDocument* Doc, VARIANT_BOOL FromTemplate, BSTR Template, VARIANT_BOOL IncludeGraphics) {
  OnDocumentNew(Doc, ToBool(FromTemplate), Template, ToBool(IncludeGraphics));
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_DocumentClose(VGCore::IVGDocument* Doc) {
  OnDocumentClose(Doc);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_DocumentBeforeSave(VGCore::IVGDocument* Doc, VARIANT_BOOL SaveAs, BSTR FileName) {
  OnDocumentBeforeSave(Doc, ToBool(SaveAs), FileName);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_DocumentAfterSave(VGCore::IVGDocument* Doc, VARIANT_BOOL SaveAs, BSTR FileName) {
  OnDocumentAfterSave(Doc, ToBool(SaveAs), FileName);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_DocumentBeforePrint(VGCore::IVGDocument* Doc) {
  OnDocumentBeforePrint(Doc);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_DocumentAfterPrint(VGCore::IVGDocument* Doc) {
  OnDocumentAfterPrint(Doc);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_DocumentBeforeExport(VGCore::IVGDocument* Doc, BSTR FileName, VGCore::cdrFilter Filter, VARIANT_BOOL SaveBitmap) {
  OnDocumentBeforeExport(Doc, FileName, Filter, ToBool(SaveBitmap));
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_DocumentAfterExport(VGCore::IVGDocument* Doc, BSTR FileName, VGCore::cdrFilter Filter, VARIANT_BOOL SaveBitmap) {
  OnDocumentAfterExport(Doc, FileName, Filter, ToBool(SaveBitmap));
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_WindowActivate(VGCore::IVGDocument* Doc, VGCore::IVGWindow* Window) {
  OnWindowActivate(Doc, Window);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_WindowDeactivate(VGCore::IVGDocument* Doc, VGCore::IVGWindow* Window) {
  OnWindowDeactivate(Doc, Window);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_SelectionChange() {
  OnSelectionChange();
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_Start() {
  OnStart();
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_Quit() {
  OnQuit();
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_OnPluginCommand(BSTR CommandID) {
  OnPluginCommand(CommandID);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_OnUpdatePluginCommand(BSTR CommandID, VARIANT_BOOL* Enabled, VGCore::cdrCommandCheckState* Checked) {
  bool bEnabled = ToBool(*Enabled);
  OnUpdatePluginCommand(CommandID, bEnabled, *Checked);
  *Enabled = ToVariantBool(bEnabled);
  return S_OK;
}

STDMETHODIMP ApplicationEventSink::raw_OnApplicationEvent(BSTR EventName, SAFEARRAY** Parameters) {
  if (SafeArrayGetDim(*Parameters) != 1)
    return E_INVALIDARG;

  long from = 0, to = 0;
  if (FAILED(SafeArrayGetLBound(*Parameters, 1, &from)) || FAILED(SafeArrayGetUBound(*Parameters, 1, &to)))
    return E_INVALIDARG;

  VARTYPE vt = VT_EMPTY;
  if (FAILED(SafeArrayGetVartype(*Parameters, &vt)) || vt != VT_VARIANT)
    return E_INVALIDARG;

  std::vector<const VARIANT> params;
  params.reserve(to - from + 1);
  VARIANT const* ptr = nullptr;
  if (FAILED(SafeArrayAccessData(*Parameters, (void**)&ptr)))
    return E_INVALIDARG;
  for (long i = from; i <= to; i++) {
    params.emplace_back(*ptr++);
  }
  SafeArrayUnaccessData(*Parameters);
  OnApplicationEvent(EventName, params);
  return S_OK;
}

} // namespace PluginSDK
