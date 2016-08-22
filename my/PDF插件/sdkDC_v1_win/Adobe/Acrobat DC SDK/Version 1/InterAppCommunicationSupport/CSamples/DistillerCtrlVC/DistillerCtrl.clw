; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPdfEventSink
LastTemplate=CCmdTarget
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DistillerCtrl.h"

ClassCount=3
Class1=CDistillerCtrlApp
Class2=CDistillerCtrlDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CPdfEventSink
Resource2=IDD_DISTILLERCTRL_DIALOG

[CLS:CDistillerCtrlApp]
Type=0
HeaderFile=sources\distillerctrl.h
ImplementationFile=sources\distillerctrl.cpp
BaseClass=CWinApp
LastObject=CDistillerCtrlApp

[CLS:CDistillerCtrlDlg]
Type=0
HeaderFile=sources\distillerctrldlg.h
ImplementationFile=sources\distillerctrldlg.cpp
BaseClass=CDialog
LastObject=CDistillerCtrlDlg
Filter=D
VirtualFilter=dWC

[DLG:IDD_DISTILLERCTRL_DIALOG]
Type=1
Class=CDistillerCtrlDlg
ControlCount=10
Control1=IDC_SELECTINPUT,button,1342242816
Control2=IDC_PROCESSFILE,button,1476460544
Control3=IDOK,button,1342242817
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATUS_PROGBAR,msctls_progress32,1350565889
Control7=IDC_STATUS_LABEL,static,1342308352
Control8=IDC_STATUS_PAGECOUNT,static,1342308352
Control9=IDC_STATUS_PERCENT,static,1342308352
Control10=IDC_INPUTFILE,edit,1350568064

[CLS:CPdfEventSink]
Type=0
HeaderFile=sources\pdfeventsink.h
ImplementationFile=sources\pdfeventsink.cpp
BaseClass=CCmdTarget
LastObject=CPdfEventSink
Filter=N
VirtualFilter=C

