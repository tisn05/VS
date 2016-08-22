#include "stdafx.h"
#include "RulerToolDataSource.h"

void RulerToolDataSource::SetDistance(double distance) {
  m_distance = distance;
  if (m_proxy)
    m_proxy->UpdateListeners("Distance");
}

void RulerToolDataSource::StartMeasuring() {
  m_distance = 0.0;
  m_measuring = true;
  if (m_proxy)
    m_proxy->UpdateListeners("Distance,IsMeasuring");
}

void RulerToolDataSource::StopMeasuring() {
  m_measuring = false;
  if (m_proxy)
    m_proxy->UpdateListeners("IsMeasuring");
}

STDMETHODIMP RulerToolDataSource::get_Distance(double* pVal) {
  *pVal = m_distance;
  return S_OK;
}

STDMETHODIMP RulerToolDataSource::get_IsMeasuring(VARIANT_BOOL* pVal) {
  *pVal = PluginSDK::ToVariantBool(m_measuring);
  return S_OK;
}
