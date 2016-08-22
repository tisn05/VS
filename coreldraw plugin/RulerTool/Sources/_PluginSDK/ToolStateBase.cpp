#include "stdafx.h"
#include "ToolStateBase.h"

#pragma comment(linker, "/DEFAULTLIB:Rpcrt4.lib")

namespace PluginSDK
{
ToolStateBase::ToolStateBase(AppPlugin* appPlugin)
    : IHostDispatchImpl{appPlugin->GetHostApplication()},
      m_appPlugin{appPlugin} {
}

VGCore::IVGApplication* ToolStateBase::GetHostApplication() const {
  return m_appPlugin ? m_appPlugin->GetHostApplication() : nullptr;
}

STDMETHODIMP ToolStateBase::raw_OnStartState(VGCore::IVGToolStateAttributes* StateAttributes) {
  m_drawing = false;
  m_stateAttributes = StateAttributes;
  OnStartState();
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnExitState() {
  OnExitState();
  m_stateAttributes.Release();
  m_drawing = false;
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnMouseMove(VGCore::IVGPoint* pt) {
  OnMouseMove(pt);
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnLButtonDown(VGCore::IVGPoint* pt) {
  OnLButtonDown(pt);
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnLButtonDownLeaveGrace(VGCore::IVGPoint* pt) {
  OnLButtonDownLeaveGrace(pt);
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnLButtonUp(VGCore::IVGPoint* pt) {
  OnLButtonUp(pt);
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnLButtonDblClick(VGCore::IVGPoint* pt) {
  OnLButtonDblClick(pt);
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnClick(VGCore::IVGPoint* pt, VARIANT_BOOL* Handled) {
  *Handled = ToVariantBool(OnClick(pt));
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnRButtonDown(VGCore::IVGPoint* pt, VARIANT_BOOL* Handled) {
  *Handled = ToVariantBool(OnRButtonDown(pt));
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnRButtonUp(VGCore::IVGPoint* pt, VARIANT_BOOL* Handled) {
  *Handled = ToVariantBool(OnRButtonUp(pt));
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnKeyDown(long KeyCode, VARIANT_BOOL* Handled) {
  *Handled = ToVariantBool(OnKeyDown(KeyCode));
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnKeyUp(long KeyCode, VARIANT_BOOL* Handled) {
  *Handled = ToVariantBool(OnKeyUp(KeyCode));
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnDelete(VARIANT_BOOL* Handled) {
  *Handled = ToVariantBool(OnDelete());
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnAbort() {
  OnAbort();
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnCommit(VGCore::IVGPoint* pt) {
  OnCommit(pt);
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnSnapMouse(VGCore::IVGPoint* pt, VARIANT_BOOL* Handled) {
  *Handled = ToVariantBool(OnSnapMouse(pt));
  return S_OK;
}

STDMETHODIMP ToolStateBase::raw_OnTimer(long TimerId, long TimeEllapsed) {
  OnTimer(TimerId, TimeEllapsed);
  return S_OK;
}

STDMETHODIMP ToolStateBase::get_IsDrawing(VARIANT_BOOL* pVal) {
  *pVal = ToVariantBool(IsDrawing());
  return S_OK;
}

void ToolStateBase::Register(AppPlugin* appPlugin, const GUID& guid, CString name, VGCore::IVGToolState* state) {
  appPlugin->GetHostApplication()->RegisterToolState(GuidToString(guid), name.GetString(), state);
}

void ToolStateBase::UnRegister(AppPlugin* appPlugin, const GUID& guid) {
  appPlugin->GetHostApplication()->UnregisterToolState(GuidToString(guid));
}

_bstr_t ToolStateBase::GuidToString(const GUID& guid) {
  _bstr_t guidString;
  RPC_WSTR pStr = nullptr;
  if (UuidToStringW(&guid, &pStr) == RPC_S_OK) {
    guidString = reinterpret_cast<LPCWSTR>(pStr);
    RpcStringFreeW(&pStr);
  }
  return guidString;
}

} // namespace PluginSDK
