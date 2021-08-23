; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWsWaveDoc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "winscope.h"
LastPage=0

ClassCount=20
Class1=CFormatDlg
Class2=CLogicLabelView2
Class3=CMainFrame
Class4=CProgressDlg
Class5=CRange32SplitterWnd
Class6=CSamplingDlg
Class7=CScopeDialog
Class8=CSetBuffSizeDlg
Class9=CTestDlg
Class10=CTriggerDlg
Class11=CWinscopeApp
Class12=CAboutDlg
Class13=CWsLogicLabelView
Class14=CWsLogicView
Class15=CWsProbeInfoView
Class16=CWsProbeView
Class17=CWsSplitterWnd
Class18=CWsWaveDataView
Class19=CWsWaveDoc
Class20=CWsWaveFrm

ResourceCount=12
Resource1=IDD_SAMPLE_LOGGING (English (U.S.))
Resource2=IDD_ABOUTBOX (English (U.S.))
Resource3=IDD_PROPPAGE_SMALL (English (U.S.))
Resource4=IDD_SAMPLING (English (U.S.))
Resource5=IDD_PROPPAGE_LARGE (English (U.S.))
Resource6=IDR_MAINFRAME (English (U.S.))
Resource7=IDD_PROPPAGE_MEDIUM (English (U.S.))
Resource8=IDD_TRIGGER (English (U.S.))
Resource9=IDR_WINSCOTYPE (English (U.S.))
Resource10=IDD_FORMAT (English (U.S.))
Resource11=IDD_TEMP_PROGRESS
Resource12=IDD_BUFFER_SETTING

[CLS:CFormatDlg]
Type=0
BaseClass=CPropertyPage
HeaderFile=FormatDlg.h
ImplementationFile=FormatDlg.cpp
Filter=D
VirtualFilter=idWC

[CLS:CLogicLabelView2]
Type=0
BaseClass=CTreeView
HeaderFile=LogicLabelView2.h
ImplementationFile=LogicLabelView2.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame
Filter=T
VirtualFilter=fWC

[CLS:CProgressDlg]
Type=0
BaseClass=CDialog
HeaderFile=ProgressDlg.h
ImplementationFile=ProgressDlg.cpp

[CLS:CRange32SplitterWnd]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=Range32SplitterWnd.h
ImplementationFile=Range32SplitterWnd.cpp

[CLS:CSamplingDlg]
Type=0
BaseClass=CPropertyPage
HeaderFile=SamplingDlg.h
ImplementationFile=SamplingDlg.cpp

[CLS:CScopeDialog]
Type=0
BaseClass=CPropertySheet
HeaderFile=ScopeDialog.h
ImplementationFile=ScopeDialog.cpp

[CLS:CSetBuffSizeDlg]
Type=0
BaseClass=CDialog
HeaderFile=SetBuffSizeDlg.h
ImplementationFile=SetBuffSizeDlg.cpp

[CLS:CTestDlg]
Type=0
BaseClass=CDialog
HeaderFile=TestDlg.h
ImplementationFile=TestDlg.cpp

[CLS:CTriggerDlg]
Type=0
BaseClass=CPropertyPage
HeaderFile=TriggerDlg.h
ImplementationFile=TriggerDlg.cpp

[CLS:CWinscopeApp]
Type=0
BaseClass=CWinApp
HeaderFile=Winscope.h
ImplementationFile=Winscope.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Winscope.cpp
ImplementationFile=Winscope.cpp
LastObject=CAboutDlg

[CLS:CWsLogicLabelView]
Type=0
BaseClass=CScrollView
HeaderFile=WsLogicLabelView.h
ImplementationFile=WsLogicLabelView.cpp

[CLS:CWsLogicView]
Type=0
BaseClass=CWsWaveDataView
HeaderFile=WsLogicView.h
ImplementationFile=WsLogicView.cpp
Filter=C
VirtualFilter=VWC

[CLS:CWsProbeInfoView]
Type=0
BaseClass=CView
HeaderFile=WsProbeInfoView.h
ImplementationFile=WsProbeInfoView.cpp
Filter=C
VirtualFilter=VWC

[CLS:CWsProbeView]
Type=0
BaseClass=CWsWaveDataView
HeaderFile=WsProbeView.h
ImplementationFile=WsProbeView.cpp
Filter=C
VirtualFilter=VWC
LastObject=IDC_VIEW_RADIX

[CLS:CWsSplitterWnd]
Type=0
BaseClass=CSplitterWnd
HeaderFile=WsSplitterWnd.h
ImplementationFile=WsSplitterWnd.cpp
Filter=W
VirtualFilter=pWC
LastObject=CWsSplitterWnd

[CLS:CWsWaveDataView]
Type=0
BaseClass=CScrollView
HeaderFile=WsWaveDataView.h
ImplementationFile=WsWaveDataView.cpp

[CLS:CWsWaveDoc]
Type=0
BaseClass=CDocument
HeaderFile=WsWaveDoc.h
ImplementationFile=WsWaveDoc.cpp
Filter=N
VirtualFilter=DC
LastObject=ID_SCOPE_RUNSCOPE

[CLS:CWsWaveFrm]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=WsWaveFrm.h
ImplementationFile=WsWaveFrm.cpp

[DLG:IDD_FORMAT]
Type=1
Class=CFormatDlg

[DLG:IDD_TEMP_PROGRESS]
Type=1
Class=CProgressDlg
ControlCount=1
Control1=IDC_PROGRESS,msctls_progress32,1350565888

[DLG:IDD_SAMPLING]
Type=1
Class=CSamplingDlg

[DLG:IDD_BUFFER_SETTING]
Type=1
Class=CSetBuffSizeDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUFFER_SIZE,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_TEST_DLG]
Type=1
Class=CTestDlg

[DLG:IDD_TRIGGER]
Type=1
Class=CTriggerDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_SCOPE_CLAIMSCOPEOWNERSHIP
Command9=ID_SCOPE_RUNSCOPE
Command10=ID_SCOPE_STOPSCOPE
Command11=ID_VIEW_ZOOMIN
Command12=ID_VIEW_ZOOMOUT
Command13=ID_VIEW_FITALL
Command14=ID_VIEW_NOZOOM
Command15=ID_SCOPE_DRAWWITHDOTS
Command16=ID_SCOPE_NOERASE
Command17=ID_VIEW_DISPLAYGRID
Command18=ID_VIEW_CURSORX
Command19=ID_VIEW_CURSORY
Command20=ID_VIEW_VOLTAGECURSORA
Command21=ID_VIEW_VOLTAGECURSORB
Command22=ID_APP_ABOUT
CommandCount=22

[TB:IDR_WINSCOTYPE (English (U.S.))]
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

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_HW_SELECTCONFIGURATIONFILE
Command9=ID_HW_CONFIGUREHARDWARE
Command10=ID_HW_GETFWVERSIONS
Command11=ID_HW_STARTINFINITELOOPINTHREAD
Command12=ID_VIEW_WINSCOPE_CONTROL_WINDOW
Command13=ID_APP_ABOUT
CommandCount=13

[MNU:IDR_WINSCOTYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_EXPORTTOCSV
Command7=ID_FILE_EXPORTTOMETAFILE
Command8=ID_FILE_PRINT
Command9=ID_FILE_PRINT_PREVIEW
Command10=ID_FILE_PRINT_SETUP
Command11=ID_FILE_MRU_FILE1
Command12=ID_APP_EXIT
Command13=ID_EDIT_UNDO
Command14=ID_EDIT_CUT
Command15=ID_EDIT_COPY
Command16=ID_EDIT_PASTE
Command17=ID_VIEW_TOOLBAR
Command18=ID_VIEW_STATUS_BAR
Command19=ID_VIEW_ZOOMIN
Command20=ID_VIEW_ZOOMOUT
Command21=ID_VIEW_FITALL
Command22=ID_VIEW_NOZOOM
Command23=ID_VIEW_CURSORX
Command24=ID_VIEW_CURSORY
Command25=ID_VIEW_VOLTAGECURSORA
Command26=ID_VIEW_VOLTAGECURSORB
Command27=ID_SCOPE_DRAWWITHDOTS
Command28=ID_SCOPE_NOERASE
Command29=ID_VIEW_DISPLAYGRID
Command30=ID_SCOPE_CLAIMSCOPEOWNERSHIP
Command31=ID_SCOPE_RUNSCOPE
Command32=ID_SCOPE_STOPSCOPE
Command33=ID_SCOPE_QUERYSWEEPSTATE
Command34=ID_HW_SELECTCONFIGURATIONFILE
Command35=ID_HW_CONFIGUREHARDWARE
Command36=ID_HW_GETFWVERSIONS
Command37=ID_HW_STARTINFINITELOOPINTHREAD
Command38=ID_WINDOW_NEW
Command39=ID_WINDOW_CASCADE
Command40=ID_WINDOW_TILE_HORZ
Command41=ID_WINDOW_ARRANGE
Command42=ID_WINDOW_SPLIT
Command43=ID_VIEW_WINSCOPE_CONTROL_WINDOW
Command44=ID_APP_ABOUT
CommandCount=44

[ACL:IDR_MAINFRAME (English (U.S.))]
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

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_FORMAT (English (U.S.))]
Type=1
Class=CFormatDlg
ControlCount=34
Control1=IDC_LABEL_LIST,SysListView32,1350664329
Control2=IDC_REMOVE_LABEL,button,1342242816
Control3=IDC_LABEL_NAME,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_ADD_LABEL,button,1342242816
Control6=IDC_SYMBIT_P7,button,1342377987
Control7=IDC_SYMBIT_P6,button,1342246915
Control8=IDC_SYMBIT_P5,button,1342246915
Control9=IDC_SYMBIT_P4,button,1342246915
Control10=IDC_SYMBIT_P3,button,1342246915
Control11=IDC_SYMBIT_P2,button,1342246915
Control12=IDC_SYMBIT_P1,button,1342246915
Control13=IDC_SYMBIT_P0,button,1342246915
Control14=IDC_SYMBIT_Q7,button,1342377987
Control15=IDC_SYMBIT_Q6,button,1342246915
Control16=IDC_SYMBIT_Q5,button,1342246915
Control17=IDC_SYMBIT_Q4,button,1342246915
Control18=IDC_SYMBIT_Q3,button,1342246915
Control19=IDC_SYMBIT_Q2,button,1342246915
Control20=IDC_SYMBIT_Q1,button,1342246915
Control21=IDC_SYMBIT_Q0,button,1342246915
Control22=IDC_MODIFY_LABEL,button,1342242816
Control23=IDC_STATIC,button,1342177287
Control24=IDC_STATIC,button,1342177287
Control25=IDC_PRB_A_ACTIVE,button,1342242819
Control26=IDC_PRB_B_ACTIVE,button,1342242819
Control27=IDC_POD_P_ACTIVE,button,1342242819
Control28=IDC_POD_Q_ACTIVE,button,1342242819
Control29=IDC_STATIC,static,1342177287
Control30=IDC_STATIC,static,1342177287
Control31=IDC_STATIC,static,1342308353
Control32=IDC_STATIC,static,1342308353
Control33=IDC_STATIC,static,1342308352
Control34=IDC_VIEW_RADIX,combobox,1344339971

[DLG:IDD_SAMPLE_LOGGING (English (U.S.))]
Type=1
Class=?
ControlCount=14
Control1=65535,button,1476395015
Control2=65535,static,1476526080
Control3=65535,static,1476526080
Control4=IDC_EDIT3,edit,1484851328
Control5=IDC_EDIT4,edit,1484851328
Control6=65535,static,1476526080
Control7=65535,static,1476526080
Control8=IDC_CHECK1,button,1476460547
Control9=65535,static,1476526080
Control10=IDC_EDIT5,edit,1484851328
Control11=IDC_BUTTON4,button,1476460544
Control12=65535,static,1476526080
Control13=IDC_COMBO6,combobox,1478557698
Control14=65535,static,1476526080

[DLG:IDD_SAMPLING (English (U.S.))]
Type=1
Class=?
ControlCount=31
Control1=IDC_A_VPERDIV,combobox,1344339971
Control2=IDC_A_INPUT,button,1342308361
Control3=IDC_A_I_AC,button,1342177289
Control4=IDC_A_I_GND,button,1342177289
Control5=IDC_B_VPERDIV,combobox,1344339971
Control6=IDC_B_INPUT,button,1342308361
Control7=IDC_B_I_AC,button,1342177289
Control8=IDC_B_I_GND,button,1342177289
Control9=IDC_CLOCK_SELECTION,button,1342308361
Control10=IDC_CLOCK_SEL_EXT,button,1342177289
Control11=IDC_TPERDIV,combobox,1344339971
Control12=IDC_SM_NORMAL,button,1342308361
Control13=IDC_SM_AUTO,button,1342177289
Control14=IDC_SWEEP_MODE,button,1342177289
Control15=IDC_TRIGGER_DELAY,edit,1350631552
Control16=IDC_TRIG_DELY_SLIDER,msctls_trackbar32,1342242840
Control17=IDC_BUFFER_SIZE,edit,1484849280
Control18=IDC_BUF_SIZE_SLIDER,msctls_trackbar32,1476460568
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,button,1342177287
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,button,1342177287
Control27=IDC_STATIC,button,1342177287
Control28=IDC_STATIC,button,1342177287
Control29=IDC_STATIC,button,1342177287
Control30=IDC_STATIC,static,1342308352
Control31=IDC_NON_SWEEP_SETTINGS,button,1342242816

[DLG:IDD_TRIGGER (English (U.S.))]
Type=1
Class=?
ControlCount=32
Control1=IDC_STATIC,button,1342177287
Control2=IDC_PRB_TRIGG_SRC,button,1342308361
Control3=IDC_STATIC,button,1342177287
Control4=IDC_TRIGG_FUNCTION,combobox,1344339971
Control5=IDC_PRB_TRIGG_SRC_B,button,1342177289
Control6=IDC_LVL_A_SCROLLBAR,scrollbar,1342177281
Control7=IDC_PRB_TRIGG_SLOPE,button,1342308361
Control8=IDC_PRB_TRIGG_SLOPE_NEG,button,1342177289
Control9=IDC_STATIC,button,1342177287
Control10=IDC_PRB_TRG_LVL_A_VOLT,edit,1350633600
Control11=IDC_LVL_B_SCROLLBAR,scrollbar,1342177281
Control12=IDC_PRB_TRG_LVL_B_VOLT,edit,1350633600
Control13=IDC_STATIC,button,1342177287
Control14=IDC_EXT_PRB_TRIGG_SLOPE,button,1342308361
Control15=IDC_EXT_PRB_TRIGG_SLOPE_NEG,button,1342177289
Control16=IDC_STATIC,button,1342177287
Control17=IDC_PTRN_SYMBOL,combobox,1344340227
Control18=IDC_TRIGG_PTRN,edit,1350631552
Control19=IDC_PTRN_RADIX,combobox,1344339971
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,button,1342177287
Control25=IDC_EDGE_SYMBOL,combobox,1344340227
Control26=IDC_STATIC,static,1342308352
Control27=IDC_SYMBOL_EDGE,button,1342308361
Control28=IDC_SYMBOL_EDGE_NEG,button,1342177289
Control29=IDC_STATIC,button,1342177287
Control30=IDC_STATIC,button,1342177287
Control31=IDC_PRB_TRG_LVL_A,edit,1350631552
Control32=IDC_PRB_TRG_LVL_B,edit,1350631552

[DLG:IDD_PROPPAGE_SMALL (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_PROPPAGE_MEDIUM (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_PROPPAGE_LARGE (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

