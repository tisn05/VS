#include "stdafx.h"
#include "RulerToolState.h"
#include "RulerToolDataSource.h"

RulerToolState::RulerToolState(PluginSDK::AppPlugin* appPlugin) : PluginSDK::ToolState<RulerToolState>{appPlugin} {
}

RulerToolState::~RulerToolState() {
}

void RulerToolState::OnStartState() {
  m_stateAttributes->SetCursorGuid("a5bdc561-e254-a088-4c94-ed1eb6f8e320");
  m_stateAttributes->AllowTempPickState = VARIANT_FALSE;
  m_stateAttributes->PropertyBarGuid = "07da4f59-2cfd-4d8a-49bc-6d121ebcd47b";
  m_Preview = GetHostApplication()->CreateOnScreenCurve();
  m_PreviewText = GetHostApplication()->CreateOnScreenText();
}

const wchar_t* GetUnitName(VGCore::cdrUnit unit) {
  switch (unit) {
    case VGCore::cdrInch:
      return L"in";
    case VGCore::cdrFoot:
      return L"ft";
    case VGCore::cdrMillimeter:
      return L"mm";
    case VGCore::cdrCentimeter:
      return L"cm";
    case VGCore::cdrPixel:
      return L"px";
    case VGCore::cdrMile:
      return L"mi";
    case VGCore::cdrMeter:
      return L"m";
    case VGCore::cdrKilometer:
      return L"km";
    case VGCore::cdrDidots:
      return L"dd";
    case VGCore::cdrAgate:
      return L"ag";
    case VGCore::cdrYard:
      return L"yd";
    case VGCore::cdrPica:
      return L"pi";
    case VGCore::cdrCicero:
      return L"cc";
    case VGCore::cdrPoint:
      return L"pt";
    case VGCore::cdrUnitQ:
      return L"Q";
    case VGCore::cdrUnitH:
      return L"H";
  }
  return L"";
}

void RulerToolState::OnMouseMove(VGCore::IVGPointPtr pt) {
  if (m_drawing) {
    m_Preview->SetLine(m_StartPoint->x, m_StartPoint->y, pt->x, pt->y);
    auto doc = GetHostApplication()->ActiveDocument;
    double length = m_StartPoint->DistanceTo(pt);
    double ui_distance = doc->FromUnits(length, doc->Rulers->HUnits);
    double internal_distance = doc->FromUnits(length, VGCore::cdrTenthMicron);
    auto pos = pt;
    double dx = pt->x - m_StartPoint->x;
    double dy = pt->y - m_StartPoint->y;
    auto vec = doc->Math->CreateVector(dy, -dx);
    auto align = VGCore::cdrOnScreenTextAlign::cdrLeftOnScreenText;
    dx = 1.0;
    if (dy > 0) {
      align = VGCore::cdrOnScreenTextAlign::cdrRightOnScreenText;
      dx = 0.0;
    }
    vec->Length = doc->ActiveWindow->ScreenDistanceToDocumentDistance(10);
    pos->Add(vec);
    CStringW text;
    text.Format(L"%.3f %s", ui_distance, GetUnitName(doc->Rulers->HUnits));
    m_PreviewText->SetTextAndPosition(text.GetString(), pos->x, pos->y, align, dx, 0);
    m_Preview->Show();
    m_PreviewText->Show();
    auto ds = m_appPlugin->GetDataSource<RulerToolDataSource>();
    if (ds)
      ds->SetDistance(internal_distance);
  }
}

void RulerToolState::OnLButtonDownLeaveGrace(VGCore::IVGPointPtr pt) {
  m_StartPoint = pt;
  m_drawing = true;
  auto ds = m_appPlugin->GetDataSource<RulerToolDataSource>();
  if (ds)
    ds->StartMeasuring();
}

void RulerToolState::OnLButtonUp(VGCore::IVGPointPtr pt) {
  OnCommit(pt);
}

void RulerToolState::OnAbort() {
  m_Preview->Hide();
  m_PreviewText->Hide();
  m_drawing = false;
  auto ds = m_appPlugin->GetDataSource<RulerToolDataSource>();
  if (ds)
    ds->StopMeasuring();
}

void RulerToolState::OnCommit(VGCore::IVGPointPtr pt) {
  if (m_drawing) {
    OnAbort();
  }
}

bool RulerToolState::OnSnapMouse(VGCore::IVGPointPtr pt) {
  if (m_drawing) {
    m_stateAttributes->AnchoredSnapMouse(pt, m_StartPoint);
    m_stateAttributes->ConstrainMouse(pt, m_StartPoint);
    return true;
  }
  return false;
}
