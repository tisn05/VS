#include "stdafx.h"
#include "AppPlugin.h"
#include "DataSourceFactory.h"

namespace PluginSDK
{

STDMETHODIMP AppPlugin::raw_OnLoad(VGCore::IVGApplication* Application) {
  m_app = Application;
  HRESULT hr = LoadTypeLib(&m_typeLib);
  if (FAILED(hr) && hr != E_NOTIMPL)
    return hr;

  IHostDispatchImpl<VGCore::IVGAppPlugin>::SetHostObject(Application);
  FillToolStates();
  FillDataSources();

  for (const auto& state : m_states) {
    std::get<1>(state)(this, std::get<0>(state));
  }

  if (m_typeLib) {
    VGCore::ICUIApplicationPtr ui_app{m_app};
    for (const auto& p : m_dataSources) {
      if (!m_dataSourceFactory)
        m_dataSourceFactory.Attach(new DataSourceFactory{m_typeLib, m_app, this});
      ui_app->RegisterDataSource(p.first.GetString(), m_dataSourceFactory, L"", VARIANT_FALSE);
    }
  }
  return OnLoad() ? S_OK : E_FAIL;
}

STDMETHODIMP AppPlugin::raw_StartSession() {
  return OnStartSession() ? S_OK : E_FAIL;
}

STDMETHODIMP AppPlugin::raw_StopSession() {
  return OnStopSession() ? S_OK : E_FAIL;
}

STDMETHODIMP AppPlugin::raw_OnUnload() {
  bool bOk = OnUnload();
  for (const auto& state : m_states) {
    std::get<2>(state)(this);
  }
  m_states.clear();

  if (m_typeLib) {
    VGCore::ICUIApplicationPtr ui_app{m_app};
    for (const auto& p : m_dataSources)
      ui_app->UnregisterDataSource(p.first.GetString());
    m_dataSourceFactory.Release();
    m_dataSources.clear();
  }
  IHostDispatchImpl<VGCore::IVGAppPlugin>::SetHostObject(nullptr);
  m_app.Release();
  return bOk ? S_OK : E_FAIL;
}

IDispatch* AppPlugin::GetDataSource(CString name) const {
  if (!m_dataSourceFactory)
    return nullptr;

  return static_cast<DataSourceFactory*>(m_dataSourceFactory.GetInterfacePtr())->GetDataSource(name);
}

} // namespace PluginSDK
