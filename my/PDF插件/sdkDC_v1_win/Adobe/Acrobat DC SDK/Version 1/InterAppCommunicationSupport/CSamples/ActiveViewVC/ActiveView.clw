; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "activeview.h"
LastPage=0

ClassCount=6
Class1=CActiveViewApp
Class2=CAboutDlg
Class3=CActiveViewDoc
Class4=CActiveViewVw
Class5=CCPrintDialog
Class6=CMainFrame

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_ACTIVEVIEWTYPE
Resource3=IDR_MAINFRAME
Resource4=IDD_PRINTDIALOG

[CLS:CActiveViewApp]
Type=0
BaseClass=CWinApp
HeaderFile=sources\ActiveView.h
ImplementationFile=sources\ActiveView.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=sources\ActiveView.cpp
ImplementationFile=sources\ActiveView.cpp
LastObject=CAboutDlg

[CLS:CActiveViewDoc]
Type=0
BaseClass=CDocument
HeaderFile=sources\ActiveViewDoc.h
ImplementationFile=sources\ActiveViewDoc.cpp

[CLS:CActiveViewVw]
Type=0
BaseClass=CView
HeaderFile=sources\ActiveViewVw.h
ImplementationFile=sources\ActiveViewVw.cpp

[CLS:CCPrintDialog]
Type=0
BaseClass=CDialog
HeaderFile=sources\ActiveViewVw.h
ImplementationFile=sources\ActiveViewVw.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=sources\MainFrm.h
ImplementationFile=sources\MainFrm.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PRINTDIALOG]
Type=1
Class=CCPrintDialog
ControlCount=11
Control1=IDC_EDIT1,edit,1350762624
Control2=IDC_EDIT2,edit,1350762624
Control3=IDC_RADIO1,button,1342373897
Control4=IDC_RADIO2,button,1342242825
Control5=IDC_CHECK1,button,1342373891
Control6=IDC_CHECK2,button,1342373891
Control7=IDOK,button,1342373889
Control8=IDCANCEL,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342177280
Control11=IDC_STATIC,static,1342308352

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

[MNU:IDR_ACTIVEVIEWTYPE]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_CLOSE
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_SETUP
Command7=ID_FILE_MRU_FILE1
Command8=ID_APP_EXIT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_SELECTALL
Command11=ID_VIEW_ACTUALSIZE
Command12=ID_VIEW_FITPAGE
Command13=ID_VIEW_FITWIDTH
Command14=ID_VIEW_FIRSTPAGE
Command15=ID_VIEW_PREVPAGE
Command16=ID_VIEW_NEXTPAGE
Command17=ID_VIEW_LASTPAGE
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_TOOLS_HAND
Command21=ID_EDIT_SELECTTEXT
Command22=ID_EDIT_SELECTGRAPHIC
Command23=ID_TOOLS_HILITEWORDS
Command24=ID_TOOLS_HILITEBYRECT
Command25=ID_TOOLS_ACQUIREBOOKMARK
Command26=ID_TOOLS_ACQUIREANNOT
Command27=ID_TOOLS_ADDTEXTANNOT
Command28=ID_TOOLS_DISPLAYAPERTURE
Command29=ID_WINDOW_CASCADE
Command30=ID_WINDOW_TILE_HORZ
Command31=ID_WINDOW_ARRANGE
Command32=ID_APP_ABOUT
CommandCount=32

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

