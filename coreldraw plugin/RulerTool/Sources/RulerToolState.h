#ifndef __RULER_TOOL_STATE_H
#define __RULER_TOOL_STATE_H
#pragma once

#include "_PluginSDK\ToolState.h"

class __declspec(uuid("a5bdc561-e254-a088-4c94-ed1eb6f8e320")) RulerToolState
    : public PluginSDK::ToolState<RulerToolState> {
public:
  RulerToolState(PluginSDK::AppPlugin* appPlugin);
  ~RulerToolState();

  void OnStartState() override;
  void OnMouseMove(VGCore::IVGPointPtr pt) override;
  void OnLButtonDownLeaveGrace(VGCore::IVGPointPtr pt) override;
  void OnLButtonUp(VGCore::IVGPointPtr pt) override;
  void OnAbort() override;
  void OnCommit(VGCore::IVGPointPtr pt) override;
  bool OnSnapMouse(VGCore::IVGPointPtr pt) override;

private:
  VGCore::IVGPointPtr m_StartPoint;
  VGCore::IVGOnScreenCurvePtr m_Preview;
  VGCore::IVGOnScreenTextPtr m_PreviewText;
};

#endif // !__RULER_TOOL_STATE_H
