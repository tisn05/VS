/////////////////////////////////////////////////////////////////////
// Module: DataSource.h
// Author: Alex Vakulenko, http://www.oberonplace.com/cgs-plugin-sdk
// Copyright 2014 by Alex Vakulenko
//
// Description:
//  DataSource is a base class to help with implementing datasources in C++.
//  Derive your datasource class from PluginSDK::DataSource and implement your
//  COM-based datasource interface (IMyDataSource):
//
//    [object, uuid(9B079F0F-C96C-8CAD-43D1-76350029B1A4), dual]
//    interface IMyDataSource : IDispatch {
//      [propget, id(1)] HRESULT MyIntegerProperty([out, retval] long* pVal);
//      [id(2)] HRESULT DoSomething();
//    };

//    class MyDataSource : public PluginSDK::DataSource<MyDataSource, IMyDataSource> {
//    public:
//      MyDataSource(CComPtr<ITypeLib> typeLib, VGCore::ICUIDataSourceProxyPtr proxy)
//          : DataSource{typeLib, proxy} {}
//    private:
//      // IMyDataSource properties and methods
//      STDMETHOD(get_MyIntegerProperty)(long* pVal) override;
//      STDMETHOD(DoSomething)() override;
//    };
/////////////////////////////////////////////////////////////////////
#ifndef __PLUGINSDK_DATASOURCE_H
#define __PLUGINSDK_DATASOURCE_H
#pragma once

#include "HostDispatchImpl.h"

namespace PluginSDK {

template<class Derived, class BaseInterface, IID const& IID_BaseInterface = __uuidof(BaseInterface)>
class DataSource : public IHostDispatchImpl<BaseInterface, IID_BaseInterface> {
public:
  DataSource(CComPtr<ITypeLib> typeLib, VGCore::ICUIDataSourceProxyPtr proxy) : IHostDispatchImpl{typeLib}, m_proxy{proxy} {}

  static IDispatch* CreateInstance(CComPtr<ITypeLib> typeLib, VGCore::ICUIDataSourceProxyPtr proxy) {
    return new Derived{typeLib, proxy};
  }

protected:
  CString m_name;
  VGCore::ICUIDataSourceProxyPtr m_proxy;
  DataSource* m_instance;
};

} // namespace PluginSDK

#endif // __PLUGINSDK_DATASOURCE_H
