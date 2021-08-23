# Microsoft Developer Studio Project File - Name="Winscope" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Winscope - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Winscope.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Winscope.mak" CFG="Winscope - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Winscope - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Winscope - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Winscope - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 setupapi.lib /nologo /subsystem:windows /incremental:yes /machine:I386

!ELSEIF  "$(CFG)" == "Winscope - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41d /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib setupapi.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Winscope - Win32 Release"
# Name "Winscope - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BitFileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\FormatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PodLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SamplingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScopeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ScopeThread.cpp
# End Source File
# Begin Source File

SOURCE=.\SetBuffSizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TriggerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UsbDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveData.cpp
# End Source File
# Begin Source File

SOURCE=.\Winscope.cpp
# End Source File
# Begin Source File

SOURCE=.\Winscope.rc
# End Source File
# Begin Source File

SOURCE=.\WsLogicLabelView.cpp
# End Source File
# Begin Source File

SOURCE=.\WsLogicView.cpp
# End Source File
# Begin Source File

SOURCE=.\WsProbeInfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\WsProbeView.cpp
# End Source File
# Begin Source File

SOURCE=.\WsSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\WsSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\WsWaveDataView.cpp
# End Source File
# Begin Source File

SOURCE=.\WsWaveDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\WsWaveFrm.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BitFileReader.h
# End Source File
# Begin Source File

SOURCE=.\FormatDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PodLabel.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegisterIF.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SamplingDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScopeDialog.h
# End Source File
# Begin Source File

SOURCE=.\ScopeThread.h
# End Source File
# Begin Source File

SOURCE=.\SetBuffSizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TriggerDlg.h
# End Source File
# Begin Source File

SOURCE=.\UsbDriver.h
# End Source File
# Begin Source File

SOURCE=.\WaveData.h
# End Source File
# Begin Source File

SOURCE=.\Winscope.h
# End Source File
# Begin Source File

SOURCE=.\WsLogicLabelView.h
# End Source File
# Begin Source File

SOURCE=.\WsLogicView.h
# End Source File
# Begin Source File

SOURCE=.\WsProbeInfoView.h
# End Source File
# Begin Source File

SOURCE=.\WsProbeView.h
# End Source File
# Begin Source File

SOURCE=.\WsSetting.h
# End Source File
# Begin Source File

SOURCE=.\WsSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\WsWaveDataView.h
# End Source File
# Begin Source File

SOURCE=.\WsWaveDoc.h
# End Source File
# Begin Source File

SOURCE=.\WsWaveFrm.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Winscope.ico
# End Source File
# Begin Source File

SOURCE=.\res\Winscope.rc2
# End Source File
# Begin Source File

SOURCE=.\res\WsWaveDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Winscope.reg
# End Source File
# End Target
# End Project
