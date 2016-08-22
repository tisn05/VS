/////////////////////////////////////////////////////////////////////
// Module: DataSourceFactory.h
// Author: Alex Vakulenko, http://www.oberonplace.com/cgs-plugin-sdk
// Copyright 2014 by Alex Vakulenko
//
// Description:
//  DataSourceFactory is an internal helper class for implementing custom
//  datasources. You should not use this class directly in your plugins.
/////////////////////////////////////////////////////////////////////

#ifndef __PLUGINSDK_DATASOURCE_FACTORY_H
#define __PLUGINSDK_DATASOURCE_FACTORY_H
#pragma once

#include "AppPlugin.h"
#include "HostDispatchImpl.h"

namespace PluginSDK {

class DataSourceFactory : public PluginSDK::IHostDispatchImpl<VGCore::ICUIDataSourceFactory> {
public:
  DataSourceFactory(CComPtr<ITypeLib> typeLib, VGCore::IVGApplicationPtr app, AppPlugin* appPlugin);
  IDispatch* GetDataSource(CString name) const;

private:
  STDMETHOD(raw_CreateDataSource)(BSTR DataSourceName, VGCore::ICUIDataSourceProxy* Proxy, IDispatch** ppVal) override;

  CComPtr<ITypeLib> m_typeLib;
  AppPlugin* m_appPlugin;
  std::map<CString, IDispatchPtr> m_instances;
};

} // namespace PluginSDK

#endif // __PLUGINSDK_DATASOURCE_FACTORY_H
