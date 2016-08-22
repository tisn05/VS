/////////////////////////////////////////////////////////////////////
// Module: HostDispatchImpl.h
// Author: Alex Vakulenko, http://www.oberonplace.com/cgs-plugin-sdk
// Copyright 2014 by Alex Vakulenko
// Description:
//  IHostDispatchImpl is a helper class for implementing IDispatch interface
//  for your plugin objects. It requires a type library describing the base
//  interface in order to implement IDispatch correctly. So you need to provide
//  either the type library (ITypeLib) or an object (IDispatch) that already
//  comes from another type library. For implementing interfaces declared in
//  CorelDRAW/Corel DESIGNER type libraries, pass in the application class
//  instance (VGCore::IVGApplication).
/////////////////////////////////////////////////////////////////////

#ifndef __PLUGINSDK_HOST_DISPATCH_IMPL_H
#define __PLUGINSDK_HOST_DISPATCH_IMPL_H
#pragma once

namespace PluginSDK {

template<class BaseInterface, IID const& IID_BaseInterface = __uuidof(BaseInterface)>
class IHostDispatchImpl : public BaseInterface {
public:
  IHostDispatchImpl() {}
  IHostDispatchImpl(IDispatch* hostObj) { SetHostObject(hostObj); }
  IHostDispatchImpl(CComPtr<ITypeLib> typeLib) : m_typeLib{typeLib} {}

  virtual ~IHostDispatchImpl() {}

  HRESULT SetHostObject(IDispatch* hostObj) {
    HRESULT hr = S_OK;
    m_typeLib.Release();
    if (hostObj) {
      CComPtr<ITypeInfo> pHostTypeInfo;
      hr = hostObj->GetTypeInfo(0, LOCALE_NEUTRAL, &pHostTypeInfo);
      if (SUCCEEDED(hr)) {
        UINT uIndex = 0;
        hr = pHostTypeInfo->GetContainingTypeLib(&m_typeLib, &uIndex);
      }
    }
    return hr;
  }

// IUnknown
public:
  STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override {
    HRESULT hr = S_OK;
    AddRef();
    if (riid == IID_IUnknown) {
      *ppvObject = static_cast<IUnknown*>(this);
    } else if (riid == IID_IDispatch) {
      *ppvObject = static_cast<IDispatch*>(this);
    } else if (riid == IID_BaseInterface) {
      *ppvObject = static_cast<BaseInterface*>(this);
    } else {
      Release();
      hr = E_NOINTERFACE;
    }
    return hr;
  }
  STDMETHOD_(ULONG, AddRef)(void) override { return InterlockedIncrement(&m_refCount); }
  STDMETHOD_(ULONG, Release)(void) override {
    ULONG ulCount = InterlockedDecrement(&m_refCount);
    if (ulCount == 0)
      delete this;
    return ulCount;
  }

// IDispatch
protected:
  STDMETHOD(GetTypeInfoCount)(UINT *pctinfo) override {
    if (pctinfo == nullptr)
      return E_POINTER;
    *pctinfo = 1;
    return S_OK;
  }

  STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) override {
    if (iTInfo)
      return DISP_E_BADINDEX;

    HRESULT hr = EnsureTI(lcid);
    if (SUCCEEDED(hr)) {
      *ppTInfo = m_typeInfo;
      (*ppTInfo)->AddRef();
    }
    return hr;
  }

  STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) override {
    HRESULT hr = EnsureTI(lcid);
    if (SUCCEEDED(hr))
      hr = m_typeInfo->GetIDsOfNames(rgszNames, cNames, rgDispId);
    return hr;
  }

  STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) override {
    HRESULT hr = EnsureTI(lcid);
    if (SUCCEEDED(hr))
      hr = m_typeInfo->Invoke(static_cast<IDispatch*>(this), dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
    return hr;
  }

private:
  HRESULT EnsureTI(LCID lcid) {
    if (!m_typeInfo) {
      if (!m_typeLib)
        return E_NOTIMPL;
      HRESULT hr = m_typeLib->GetTypeInfoOfGuid(IID_BaseInterface, &m_typeInfo);
      if (FAILED(hr))
        return hr;
      CComPtr<ITypeInfo2> spTypeInfo2;
      if (SUCCEEDED(m_typeInfo->QueryInterface(&spTypeInfo2)))
        m_typeInfo = spTypeInfo2;
    }
    return S_OK;
  }

  CComPtr<ITypeLib> m_typeLib;
  CComPtr<ITypeInfo> m_typeInfo;
  volatile ULONG m_refCount{1};
};

inline VARIANT_BOOL ToVariantBool(bool bVal) {
  return bVal ? VARIANT_TRUE : VARIANT_FALSE;
}

inline bool ToBool(VARIANT_BOOL bVal) {
  return bVal != VARIANT_FALSE;
}

} // namespace PluginSDK

#endif // __PLUGINSDK_HOST_DISPATCH_IMPL_H
