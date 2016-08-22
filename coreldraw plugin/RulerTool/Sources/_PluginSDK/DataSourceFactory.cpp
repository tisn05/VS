#include "stdafx.h"
#include "DataSourceFactory.h"

namespace PluginSDK {

DataSourceFactory::DataSourceFactory(CComPtr<ITypeLib> typeLib, VGCore::IVGApplicationPtr app, AppPlugin* appPlugin)
    : IHostDispatchImpl(app), m_typeLib{typeLib}, m_appPlugin{appPlugin} {
}

IDispatch* DataSourceFactory::GetDataSource(CString name) const {
  auto p = m_instances.find(name);
  if (p == m_instances.end())
    return nullptr;
  return p->second.GetInterfacePtr();
}

STDMETHODIMP DataSourceFactory::raw_CreateDataSource(BSTR dataSourceName, VGCore::ICUIDataSourceProxy* proxy, IDispatch** ppVal) {
  CString name{dataSourceName};
  auto p = m_appPlugin->m_dataSources.find(name);
  if (p == m_appPlugin->m_dataSources.end())
    return E_FAIL;

  *ppVal = p->second(m_typeLib, proxy);
  if (*ppVal)
    m_instances.emplace(name, *ppVal);
  return S_OK;
}

} // namespace PluginSDK
