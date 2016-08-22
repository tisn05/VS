; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRemoteControlAcrobatApp
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "remotecontrolacrobat.h"
LastPage=0

ClassCount=7
Class1=FindTextDlg
Class2=CMainFrame
Class3=PageNumDlg
Class4=CRemoteControlAcrobatApp
Class5=CAboutDlg
Class6=CRemoteControlAcrobatDoc
Class7=CRemoteControlAcrobatView

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDD_DIALOG1
Resource3=IDR_MAINFRAME
Resource4=IDD_DIALOG2

[CLS:FindTextDlg]
Type=0
BaseClass=CDialog
HeaderFile=sources\FindTextDlg.h
ImplementationFile=sources\FindTextDlg.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=sources\MainFrm.h
ImplementationFile=sources\MainFrm.cpp

[CLS:PageNumDlg]
Type=0
BaseClass=CDialog
HeaderFile=sources\PageNumDlg.h
ImplementationFile=sources\PageNumDlg.cpp

[CLS:CRemoteControlAcrobatApp]
Type=0
BaseClass=CWinApp
HeaderFile=sources\RemoteControlAcrobat.h
ImplementationFile=sources\RemoteControlAcrobat.cpp
LastObject=CRemoteControlAcrobatApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=sources\RemoteControlAcrobat.cpp
ImplementationFile=sources\RemoteControlAcrobat.cpp
LastObject=ID_ACROBAT_AVDOC_CLOSE

[CLS:CRemoteControlAcrobatDoc]
Type=0
BaseClass=CDocument
HeaderFile=sources\RemoteControlAcrobatDoc.h
ImplementationFile=sources\RemoteControlAcrobatDoc.cpp

[CLS:CRemoteControlAcrobatView]
Type=0
BaseClass=CView
HeaderFile=sources\RemoteControlAcrobatView.h
ImplementationFile=sources\RemoteControlAcrobatView.cpp

[DLG:IDD_DIALOG1]
Type=1
Class=FindTextDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_WORD,edit,1350631552
Control4=IDC_STATIC_LABEL,static,1342308352
Control5=IDC_STATIC_FILENAME,static,1342308352

[DLG:IDD_DIALOG2]
Type=1
Class=PageNumDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_NUMBER,edit,1350639744
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC_FILENAME,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
Command17=ID_ACROBAT_LAUNCH
Command18=ID_ACROBAT_APP_SHOW
Command19=ID_ACROBAT_APP_HIDE
Command20=ID_ACROBAT_AVDOC_OPEN
Command21=ID_ACROBAT_AVDOC_PRINT
Command22=ID_ACROBAT_AVDOC_FINDTEXT
Command23=ID_ACROBAT_AVDOC_CLOSE
Command24=ID_ACROBAT_AVPAGEVIEW_GOTOPAGE
Command25=ID_ACROBAT_AVPAGEVIEW_PAGENUMBER
Command26=ID_ACROBAT_AVPAGEVIEW_GOFORWARD
Command27=ID_ACROBAT_AVPAGEVIEW_GOBACK
Command28=ID_ACROBAT_QUIT
CommandCount=28

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

