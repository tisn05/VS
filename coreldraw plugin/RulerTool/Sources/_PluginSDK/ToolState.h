/////////////////////////////////////////////////////////////////////
// Module: ToolState.h
// Author: Alex Vakulenko, http://www.oberonplace.com/cgs-plugin-sdk
// Copyright 2014 by Alex Vakulenko
//
// Description:
//  ToolState is a base class to help with implementing state objects for custom
//  tools in C++.
//  Derive your tool state class from PluginSDK::ToolState and overload state
//  methods to handle mouse and keyboard input from the document.
//
//  Here is an example of implementing a simple application plugin class that
//  declares one custom tool state and one datasource:
//
//    class __declspec(uuid("a08b6b5c-e344-10aa-4e59-60b338d36cde")) MyToolState
//        : public PluginSDK::ToolState<MyToolState> {
//    public:
//      MyToolState(PluginSDK::AppPlugin* appPlugin);
//      ~MyToolState();
//    
//      void OnStartState() override;
//      void OnMouseMove(VGCore::IVGPointPtr pt) override;
//      void OnLButtonDownLeaveGrace(VGCore::IVGPointPtr pt) override;
//      void OnLButtonUp(VGCore::IVGPointPtr pt) override;
//      void OnAbort() override;
//      void OnCommit(VGCore::IVGPointPtr pt) override;
//      bool OnSnapMouse(VGCore::IVGPointPtr pt) override;
//    };
/////////////////////////////////////////////////////////////////////

#ifndef __PLUGINSDK_TOOL_STATE_H
#define __PLUGINSDK_TOOL_STATE_H
#pragma once

#include "ToolStateBase.h"

namespace PluginSDK {

template<class State>
class ToolState : public ToolStateBase {
public:
  ToolState(AppPlugin* appPlugin) : ToolStateBase{appPlugin} {}

  static void Register(AppPlugin* appPlugin, CString name) {
    CComPtr<State> state;
    state.Attach(new State(appPlugin));
    ToolStateBase::Register(appPlugin, __uuidof(State), name, state);
  }

  static void UnRegister(AppPlugin* appPlugin) {
    ToolStateBase::UnRegister(appPlugin, __uuidof(State));
  }
};

} // namespace PluginSDK

#endif // __PLUGINSDK_TOOL_STATE_BASE_H
