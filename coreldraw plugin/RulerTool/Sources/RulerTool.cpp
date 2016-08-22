// RulerTool.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "_PluginSDK\ApplicationEventSink.h"
#include "_PluginSDK\AppPlugin.h"

#include "Resource.h"
#include "RulerToolState.h"
#include "RulerToolDataSource.h"

extern HMODULE g_hModule;  // Declared in dllmain.cpp

class MyPlugin final : public PluginSDK::AppPlugin {
public:
  // Tool state registration
  BEGIN_TOOLSTATE_LIST()
    TOOLSTATE_LIST_ENTRY(RulerToolState)
  END_TOOLSTATE_LIST()

  // Datasources
  BEGIN_DATASOURCE_LIST()
    DATASOURCE_LIST_ENTRY2("RulerToolDS", RulerToolDataSource)
  END_DATASOURCE_LIST()

private:
  HRESULT LoadTypeLib(ITypeLib** pTypeLib) override {
    CStringW strFileNameW;
    GetModuleFileNameW(g_hModule, strFileNameW.GetBuffer(MAX_PATH), MAX_PATH);
    strFileNameW.ReleaseBuffer();
    const UINT unResID = IDT_RULERTOOL_TYPELIB;
    if (unResID > 0) {
      strFileNameW.AppendFormat(L"\\%u", unResID);
    }
    return LoadTypeLibEx(strFileNameW, REGKIND_NONE, pTypeLib);
  }
};

DECLARE_APP_PLUGIN(MyPlugin)
