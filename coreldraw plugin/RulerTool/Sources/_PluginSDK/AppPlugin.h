/////////////////////////////////////////////////////////////////////
// Module: AppPlugin.h
// Author: Alex Vakulenko, http://www.oberonplace.com/cgs-plugin-sdk
// Copyright 2014 by Alex Vakulenko
//
// Description:
//  DocumentEventSink is a base class that adds simple C++ interface
//  for implementing Document event listeners in plugins.
//  Derive your class from PluginSDK::DocumentEventSink and override
//  one or more of the event handlers.
//
//  Here is an example of implementing a simple application plugin class that
//  declares one custom tool state and one datasource:
//
//    extern HMODULE g_hModule;  // Declared in dllmain.cpp
//    
//    class MyPlugin final : public PluginSDK::AppPlugin {
//    public:
//      // Tool state registration
//      BEGIN_TOOLSTATE_LIST()
//        TOOLSTATE_LIST_ENTRY(MyToolState)
//      END_TOOLSTATE_LIST()
//    
//      // Datasources
//      BEGIN_DATASOURCE_LIST()
//        DATASOURCE_LIST_ENTRY(MyToolDataSource)
//      END_DATASOURCE_LIST()
//    
//    private:
//      HRESULT LoadTypeLib(ITypeLib** pTypeLib) override {
//        CStringW strFileNameW;
//        GetModuleFileNameW(g_hModule, strFileNameW.GetBuffer(MAX_PATH), MAX_PATH);
//        strFileNameW.ReleaseBuffer();
//        strFileNameW.AppendFormat(L"\\%u", IDT_MYTOOL_TYPELIB);
//        return LoadTypeLibEx(strFileNameW, REGKIND_NONE, pTypeLib);
//      }
//    };
//    
//    DECLARE_APP_PLUGIN(MyPlugin)
/////////////////////////////////////////////////////////////////////

#ifndef __PLUGINSDK_APP_PLUGIN_H
#define __PLUGINSDK_APP_PLUGIN_H
#pragma once

#include <map>
#include <tuple>
#include <vector>

#include "HostDispatchImpl.h"

namespace PluginSDK {

class DataSourceFactory;

class AppPlugin : public IHostDispatchImpl<VGCore::IVGAppPlugin> {
public:
  AppPlugin() {}

public:
  VGCore::IVGApplication* GetHostApplication() { return m_app; }

  template<class DataSource>
  DataSource* GetDataSource() {
    for (const auto& p : m_dataSources) {
      if (p.second == &DataSource::CreateInstance) {
        return static_cast<DataSource*>(GetDataSource(p.first));
      }
    }
    return nullptr;
  }

  virtual bool OnLoad() { return true; }
  virtual bool OnStartSession() { return true; }
  virtual bool OnStopSession() { return true; }
  virtual bool OnUnload() { return true; }

  virtual HRESULT LoadTypeLib(ITypeLib** pTypeLib) { return E_NOTIMPL; }

#pragma region IVGAppPlugin
protected:
  STDMETHOD(raw_OnLoad)(VGCore::IVGApplication* Application) override;
  STDMETHOD(raw_StartSession)() override;
  STDMETHOD(raw_StopSession)() override;
  STDMETHOD(raw_OnUnload)() override;
#pragma endregion Raw methods

protected:
  friend class DataSourceFactory;
  virtual void FillToolStates() {}
  virtual void FillDataSources() {}
  IDispatch* GetDataSource(CString name) const;

  VGCore::IVGApplicationPtr m_app;
  std::vector<std::tuple<CString, void(*)(AppPlugin*, CString), void(*)(AppPlugin*)>> m_states;
  std::map<CString, IDispatch*(*)(CComPtr<ITypeLib>, VGCore::ICUIDataSourceProxyPtr)> m_dataSources;
  VGCore::ICUIDataSourceFactoryPtr m_dataSourceFactory;
  CComPtr<ITypeLib> m_typeLib;
};

} // namespace PluginSDK

#define DECLARE_APP_PLUGIN(ClassName) \
  extern "C" __declspec(dllexport) DWORD APIENTRY AttachPlugin(VGCore::IVGAppPlugin** ppIPlugin) { \
    *ppIPlugin = new ClassName; \
    return 0x100; \
  }

#define BEGIN_TOOLSTATE_LIST() \
  virtual void FillToolStates() override {

#define TOOLSTATE_LIST_ENTRY(state) \
  m_states.push_back(std::make_tuple(CString(#state), &state::Register, &state::UnRegister));

#define END_TOOLSTATE_LIST() \
  }

#define BEGIN_DATASOURCE_LIST() \
  virtual void FillDataSources() override {

#define DATASOURCE_LIST_ENTRY2(name, datasource) \
  m_dataSources.emplace(CString(name), &datasource::CreateInstance);

#define DATASOURCE_LIST_ENTRY(datasource) \
  DATASOURCE_LIST_ENTRY2(#datasource, datasource);

#define END_DATASOURCE_LIST() \
  }

#endif // __PLUGINSDK_APP_PLUGIN_H
