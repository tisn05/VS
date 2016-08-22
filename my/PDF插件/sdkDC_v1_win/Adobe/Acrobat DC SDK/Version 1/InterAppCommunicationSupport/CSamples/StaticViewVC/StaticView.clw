; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "staticview.h"
LastPage=0

ClassCount=8
Class1=CEditBox
Class2=CMainFrame
Class3=CStaticViewApp
Class4=CAboutDlg
Class5=CStaticViewDoc
Class6=CStaticViewVw
Class7=CZoomSet
Class8=ZoomSetOther

ResourceCount=5
Resource1=IDR_STATICVIEWTYPE
Resource2=IDD_ABOUTBOX
Resource3=IDD_ZOOMSET
Resource4=IDD_ZOOMSETOTHER
Resource5=IDR_MAINFRAME

[CLS:CEditBox]
Type=0
BaseClass=CWnd
HeaderFile=sources\EditBox.h
ImplementationFile=sources\EditBox.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=sources\MainFrm.h
ImplementationFile=sources\MainFrm.cpp

[CLS:CStaticViewApp]
Type=0
BaseClass=CWinApp
HeaderFile=sources\StaticView.h
ImplementationFile=sources\StaticView.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=sources\StaticView.cpp
ImplementationFile=sources\StaticView.cpp
LastObject=CAboutDlg

[CLS:CStaticViewDoc]
Type=0
BaseClass=CDocument
HeaderFile=sources\StaticViewDoc.h
ImplementationFile=sources\StaticViewDoc.cpp

[CLS:CStaticViewVw]
Type=0
BaseClass=CScrollView
HeaderFile=sources\StaticViewVw.h
ImplementationFile=sources\StaticViewVw.cpp

[CLS:CZoomSet]
Type=0
BaseClass=CDialog
HeaderFile=sources\ZoomSet.h
ImplementationFile=sources\ZoomSet.cpp

[CLS:ZoomSetOther]
Type=0
BaseClass=CDialog
HeaderFile=sources\ZoomSeto.h
ImplementationFile=sources\ZoomSeto.cpp
LastObject=ID_APP_ABOUT

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_ZOOMSET]
Type=1
Class=CZoomSet
ControlCount=1
Control1=IDC_LIST1,listbox,1342243075

[DLG:IDD_ZOOMSETOTHER]
Type=1
Class=ZoomSetOther
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342373888
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_PRINT_SETUP
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_VIEW_STATUS_BAR
Command7=ID_APP_ABOUT
CommandCount=7

[MNU:IDR_STATICVIEWTYPE]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_CLOSE
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_VIEW_ACTUALSIZE
Command11=ID_VIEW_FITPAGE
Command12=ID_VIEW_FITWIDTH
Command13=ID_VIEW_FIRSTPAGE
Command14=ID_VIEW_PREVPAGE
Command15=ID_VIEW_NEXTPAGE
Command16=ID_VIEW_LASTPAGE
Command17=ID_VIEW_TOOLBAR
Command18=ID_VIEW_STATUS_BAR
Command19=ID_WINDOW_CASCADE
Command20=ID_WINDOW_TILE_HORZ
Command21=ID_WINDOW_ARRANGE
Command22=ID_OTHER_COPYTOCLIPBOARD
Command23=ID_APP_ABOUT
CommandCount=23

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT
Command4=ID_FILE_SAVE
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
CommandCount=6

