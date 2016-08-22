#ifndef __RULER_TOOL_DATASOURCE_H
#define __RULER_TOOL_DATASOURCE_H
#pragma once

#include "RulerTool_h.h"
#include "_PluginSDK\DataSource.h"

class RulerToolDataSource : public PluginSDK::DataSource<RulerToolDataSource, IRulerDataSource> {
public:
  RulerToolDataSource(CComPtr<ITypeLib> typeLib, VGCore::ICUIDataSourceProxyPtr proxy)
      : DataSource{typeLib, proxy} {}

  void SetDistance(double distance);
  void StartMeasuring();
  void StopMeasuring();

private:
  STDMETHOD(get_Distance)(double* pVal) override;
  STDMETHOD(get_IsMeasuring)(VARIANT_BOOL* pVal) override;

  bool m_measuring{false};
  double m_distance{0.0};
};

#endif // !__RULER_TOOL_DATASOURCE_FACTORY_H
