/////////////////////////////////////////////////////////////////////
// Module:   ToolStateBase.h
// Author: Alex Vakulenko, http://www.oberonplace.com/cgs-plugin-sdk
// Copyright 2014 by Alex Vakulenko
//
// Description:
//  ToolStateBase is an internal helper base class for implementing tool
//  state objects. You should not need to use this class directly in your
//  plugins.
/////////////////////////////////////////////////////////////////////

#ifndef __PLUGINSDK_TOOL_STATE_BASE_H
#define __PLUGINSDK_TOOL_STATE_BASE_H
#pragma once

#include "AppPlugin.h"
#include "HostDispatchImpl.h"

namespace PluginSDK {

class ToolStateBase : public IHostDispatchImpl<VGCore::IVGToolState> {
public:
  ToolStateBase(AppPlugin* appPlugin);
  VGCore::IVGApplication* GetHostApplication() const;

public:
  virtual void OnStartState() {}
  virtual void OnExitState() {}
  virtual void OnMouseMove(VGCore::IVGPointPtr pt) {}
  virtual void OnLButtonDown(VGCore::IVGPointPtr pt) {}
  virtual void OnLButtonDownLeaveGrace(VGCore::IVGPointPtr pt) {}
  virtual void OnLButtonUp(VGCore::IVGPointPtr pt) {}
  virtual void OnLButtonDblClick(VGCore::IVGPointPtr pt) {}
  virtual bool OnClick(VGCore::IVGPointPtr pt) { return false; }
  virtual bool OnRButtonDown(VGCore::IVGPointPtr pt) { return false; }
  virtual bool OnRButtonUp(VGCore::IVGPointPtr pt) { return false; }
  virtual bool OnKeyDown(long KeyCode) { return false; }
  virtual bool OnKeyUp(long KeyCode) { return false; }
  virtual bool OnDelete() { return false; }
  virtual void OnAbort() {}
  virtual void OnCommit(VGCore::IVGPointPtr pt) {}
  virtual bool OnSnapMouse(VGCore::IVGPointPtr pt) { return false; }
  virtual void OnTimer(long TimerId, long TimeEllapsed) {}
  virtual bool IsDrawing() { return m_drawing; }

#pragma region IVGToolState
protected:
  STDMETHOD(raw_OnStartState)(VGCore::IVGToolStateAttributes* StateAttributes) override;
  STDMETHOD(raw_OnExitState)() override;
  STDMETHOD(raw_OnMouseMove)(VGCore::IVGPoint* pt) override;
  STDMETHOD(raw_OnLButtonDown)(VGCore::IVGPoint* pt) override;
  STDMETHOD(raw_OnLButtonDownLeaveGrace)(VGCore::IVGPoint* pt) override;
  STDMETHOD(raw_OnLButtonUp)(VGCore::IVGPoint* pt) override;
  STDMETHOD(raw_OnLButtonDblClick)(VGCore::IVGPoint* pt) override;
  STDMETHOD(raw_OnClick)(VGCore::IVGPoint* pt, VARIANT_BOOL* Handled) override;
  STDMETHOD(raw_OnRButtonDown)(VGCore::IVGPoint* pt, VARIANT_BOOL* Handled) override;
  STDMETHOD(raw_OnRButtonUp)(VGCore::IVGPoint* pt, VARIANT_BOOL* Handled) override;
  STDMETHOD(raw_OnKeyDown)(long KeyCode, VARIANT_BOOL* Handled) override;
  STDMETHOD(raw_OnKeyUp)(long KeyCode, VARIANT_BOOL* Handled) override;
  STDMETHOD(raw_OnDelete)(VARIANT_BOOL* Handled) override;
  STDMETHOD(raw_OnAbort)() override;
  STDMETHOD(raw_OnCommit)(VGCore::IVGPoint* pt) override;
  STDMETHOD(raw_OnSnapMouse)(VGCore::IVGPoint* pt, VARIANT_BOOL* Handled) override;
  STDMETHOD(raw_OnTimer)(long TimerId, long TimeEllapsed) override;
  STDMETHOD(get_IsDrawing)(VARIANT_BOOL* pVal) override;
#pragma endregion Raw methods

protected:
  AppPlugin* m_appPlugin;
  VGCore::IVGToolStateAttributesPtr m_stateAttributes;
  bool m_drawing{false};

  static void Register(AppPlugin* appPlugin, const GUID& guid, CString name, VGCore::IVGToolState* state);
  static void UnRegister(AppPlugin* appPlugin, const GUID& guid);

private:
  static _bstr_t GuidToString(const GUID& guid);
};

} // namespace PluginSDK

#endif // __PLUGINSDK_TOOL_STATE_BASE_H
