# Microsoft Developer Studio Project File - Name="FindTools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FindTools - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FindTools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FindTools.mak" CFG="FindTools - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FindTools - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FindTools - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FindTools - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"

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
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FindTools - Win32 Release"
# Name "FindTools - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ButtonEx.cpp
# End Source File
# Begin Source File

SOURCE=.\CreditStatic.CPP
# End Source File
# Begin Source File

SOURCE=.\DiZiButton.cpp
# End Source File
# Begin Source File

SOURCE=.\EditExt.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChinese.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChineseEx.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChineseFile.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChineseFold.cpp
# End Source File
# Begin Source File

SOURCE=.\FindChineseFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\FindHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\FindString.cpp
# End Source File
# Begin Source File

SOURCE=.\FindTools.cpp
# End Source File
# Begin Source File

SOURCE=.\FindTools.rc
# End Source File
# Begin Source File

SOURCE=.\FindToolsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBoxExt.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlExt.cpp
# End Source File
# Begin Source File

SOURCE=.\MacroTool.cpp
# End Source File
# Begin Source File

SOURCE=.\MyProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditCtrlExt.cpp
# End Source File
# Begin Source File

SOURCE=.\SortHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ButtonEx.h
# End Source File
# Begin Source File

SOURCE=.\CreditStatic.H
# End Source File
# Begin Source File

SOURCE=.\DiZiButton.h
# End Source File
# Begin Source File

SOURCE=.\EditExt.h
# End Source File
# Begin Source File

SOURCE=.\FileInfo.h
# End Source File
# Begin Source File

SOURCE=.\FindChinese.h
# End Source File
# Begin Source File

SOURCE=.\FindChineseEx.h
# End Source File
# Begin Source File

SOURCE=.\FindChineseFile.h
# End Source File
# Begin Source File

SOURCE=.\FindChineseFold.h
# End Source File
# Begin Source File

SOURCE=.\FindChineseFunc.h
# End Source File
# Begin Source File

SOURCE=.\FindHelp.h
# End Source File
# Begin Source File

SOURCE=.\FindString.h
# End Source File
# Begin Source File

SOURCE=.\FindTools.h
# End Source File
# Begin Source File

SOURCE=.\FindToolsDlg.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\ListBoxExt.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlExt.h
# End Source File
# Begin Source File

SOURCE=.\MacroTool.h
# End Source File
# Begin Source File

SOURCE=.\MyProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RichEditCtrlExt.h
# End Source File
# Begin Source File

SOURCE=.\SortHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\del.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FILENEW.BMP
# End Source File
# Begin Source File

SOURCE=.\res\Find.ico
# End Source File
# Begin Source File

SOURCE=.\res\FindTools.ico
# End Source File
# Begin Source File

SOURCE=.\res\FindTools.rc2
# End Source File
# Begin Source File

SOURCE=.\res\g1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\g1.JPG
# End Source File
# Begin Source File

SOURCE=.\res\g2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\g3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\g4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\new.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pin1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pin2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pin3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pin4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pin5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ZOOM.BMP
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\about.htm
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\WINMM.LIB
# End Source File
# End Target
# End Project
# Section FindTools : {72ADFD6C-2C39-11D0-9903-00A0C91BC942}
# 	1:26:CG_IDR_POPUP_LIST_CTRL_EXT:117
# 	1:25:CG_IDR_POPUP_LIST_BOX_EXT:116
# 	1:23:CG_IDR_POPUP_MACRO_TOOL:114
# 	1:31:CG_IDR_POPUP_RICH_EDIT_CTRL_EXT:119
# End Section
