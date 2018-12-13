# Microsoft Developer Studio Generated NMAKE File, Based on FindTools.dsp
!IF "$(CFG)" == ""
CFG=FindTools - Win32 Debug
!MESSAGE No configuration specified. Defaulting to FindTools - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "FindTools - Win32 Release" && "$(CFG)" != "FindTools - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "FindTools - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\FindTools.exe"

!ELSE 

ALL : "FindToolsRsDll - Win32 Release" "FindToolsDll - Win32 Release" "$(OUTDIR)\FindTools.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"FindToolsDll - Win32 ReleaseCLEAN" "FindToolsRsDll - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ButtonEx.obj"
	-@erase "$(INTDIR)\CreditStatic.obj"
	-@erase "$(INTDIR)\EditExt.obj"
	-@erase "$(INTDIR)\FindChinese.obj"
	-@erase "$(INTDIR)\FindChineseEx.obj"
	-@erase "$(INTDIR)\FindChineseFile.obj"
	-@erase "$(INTDIR)\FindChineseFold.obj"
	-@erase "$(INTDIR)\FindChineseFunc.obj"
	-@erase "$(INTDIR)\FindHelp.obj"
	-@erase "$(INTDIR)\FindString.obj"
	-@erase "$(INTDIR)\FindTools.obj"
	-@erase "$(INTDIR)\FindTools.pch"
	-@erase "$(INTDIR)\FindTools.res"
	-@erase "$(INTDIR)\FindToolsDlg.obj"
	-@erase "$(INTDIR)\HyperLink.obj"
	-@erase "$(INTDIR)\ListBoxExt.obj"
	-@erase "$(INTDIR)\ListCtrlExt.obj"
	-@erase "$(INTDIR)\MacroTool.obj"
	-@erase "$(INTDIR)\MyProgressCtrl.obj"
	-@erase "$(INTDIR)\SortHeaderCtrl.obj"
	-@erase "$(INTDIR)\SortListCtrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\FindTools.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\FindTools.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\FindTools.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FindTools.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\FindTools.pdb" /machine:I386 /out:"$(OUTDIR)\FindTools.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ButtonEx.obj" \
	"$(INTDIR)\CreditStatic.obj" \
	"$(INTDIR)\EditExt.obj" \
	"$(INTDIR)\FindChinese.obj" \
	"$(INTDIR)\FindChineseEx.obj" \
	"$(INTDIR)\FindChineseFile.obj" \
	"$(INTDIR)\FindChineseFold.obj" \
	"$(INTDIR)\FindChineseFunc.obj" \
	"$(INTDIR)\FindHelp.obj" \
	"$(INTDIR)\FindString.obj" \
	"$(INTDIR)\FindTools.obj" \
	"$(INTDIR)\FindToolsDlg.obj" \
	"$(INTDIR)\HyperLink.obj" \
	"$(INTDIR)\ListBoxExt.obj" \
	"$(INTDIR)\ListCtrlExt.obj" \
	"$(INTDIR)\MacroTool.obj" \
	"$(INTDIR)\MyProgressCtrl.obj" \
	"$(INTDIR)\SortHeaderCtrl.obj" \
	"$(INTDIR)\SortListCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\FindTools.res" \
	".\WINMM.LIB" \
	"..\FindToolsDll\Release\FindToolsDll.lib" \
	"..\FindToolsRsDll\Release\FindToolsRsDll.lib"

"$(OUTDIR)\FindTools.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\FindTools.exe" "$(OUTDIR)\FindTools.bsc"

!ELSE 

ALL : "FindToolsRsDll - Win32 Debug" "FindToolsDll - Win32 Debug" "$(OUTDIR)\FindTools.exe" "$(OUTDIR)\FindTools.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"FindToolsDll - Win32 DebugCLEAN" "FindToolsRsDll - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ButtonEx.obj"
	-@erase "$(INTDIR)\ButtonEx.sbr"
	-@erase "$(INTDIR)\CreditStatic.obj"
	-@erase "$(INTDIR)\CreditStatic.sbr"
	-@erase "$(INTDIR)\EditExt.obj"
	-@erase "$(INTDIR)\EditExt.sbr"
	-@erase "$(INTDIR)\FindChinese.obj"
	-@erase "$(INTDIR)\FindChinese.sbr"
	-@erase "$(INTDIR)\FindChineseEx.obj"
	-@erase "$(INTDIR)\FindChineseEx.sbr"
	-@erase "$(INTDIR)\FindChineseFile.obj"
	-@erase "$(INTDIR)\FindChineseFile.sbr"
	-@erase "$(INTDIR)\FindChineseFold.obj"
	-@erase "$(INTDIR)\FindChineseFold.sbr"
	-@erase "$(INTDIR)\FindChineseFunc.obj"
	-@erase "$(INTDIR)\FindChineseFunc.sbr"
	-@erase "$(INTDIR)\FindHelp.obj"
	-@erase "$(INTDIR)\FindHelp.sbr"
	-@erase "$(INTDIR)\FindString.obj"
	-@erase "$(INTDIR)\FindString.sbr"
	-@erase "$(INTDIR)\FindTools.obj"
	-@erase "$(INTDIR)\FindTools.pch"
	-@erase "$(INTDIR)\FindTools.res"
	-@erase "$(INTDIR)\FindTools.sbr"
	-@erase "$(INTDIR)\FindToolsDlg.obj"
	-@erase "$(INTDIR)\FindToolsDlg.sbr"
	-@erase "$(INTDIR)\HyperLink.obj"
	-@erase "$(INTDIR)\HyperLink.sbr"
	-@erase "$(INTDIR)\ListBoxExt.obj"
	-@erase "$(INTDIR)\ListBoxExt.sbr"
	-@erase "$(INTDIR)\ListCtrlExt.obj"
	-@erase "$(INTDIR)\ListCtrlExt.sbr"
	-@erase "$(INTDIR)\MacroTool.obj"
	-@erase "$(INTDIR)\MacroTool.sbr"
	-@erase "$(INTDIR)\MyProgressCtrl.obj"
	-@erase "$(INTDIR)\MyProgressCtrl.sbr"
	-@erase "$(INTDIR)\SortHeaderCtrl.obj"
	-@erase "$(INTDIR)\SortHeaderCtrl.sbr"
	-@erase "$(INTDIR)\SortListCtrl.obj"
	-@erase "$(INTDIR)\SortListCtrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\FindTools.bsc"
	-@erase "$(OUTDIR)\FindTools.exe"
	-@erase "$(OUTDIR)\FindTools.ilk"
	-@erase "$(OUTDIR)\FindTools.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FindTools.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\FindTools.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FindTools.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ButtonEx.sbr" \
	"$(INTDIR)\CreditStatic.sbr" \
	"$(INTDIR)\EditExt.sbr" \
	"$(INTDIR)\FindChinese.sbr" \
	"$(INTDIR)\FindChineseEx.sbr" \
	"$(INTDIR)\FindChineseFile.sbr" \
	"$(INTDIR)\FindChineseFold.sbr" \
	"$(INTDIR)\FindChineseFunc.sbr" \
	"$(INTDIR)\FindHelp.sbr" \
	"$(INTDIR)\FindString.sbr" \
	"$(INTDIR)\FindTools.sbr" \
	"$(INTDIR)\FindToolsDlg.sbr" \
	"$(INTDIR)\HyperLink.sbr" \
	"$(INTDIR)\ListBoxExt.sbr" \
	"$(INTDIR)\ListCtrlExt.sbr" \
	"$(INTDIR)\MacroTool.sbr" \
	"$(INTDIR)\MyProgressCtrl.sbr" \
	"$(INTDIR)\SortHeaderCtrl.sbr" \
	"$(INTDIR)\SortListCtrl.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\FindTools.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\FindTools.pdb" /debug /machine:I386 /out:"$(OUTDIR)\FindTools.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ButtonEx.obj" \
	"$(INTDIR)\CreditStatic.obj" \
	"$(INTDIR)\EditExt.obj" \
	"$(INTDIR)\FindChinese.obj" \
	"$(INTDIR)\FindChineseEx.obj" \
	"$(INTDIR)\FindChineseFile.obj" \
	"$(INTDIR)\FindChineseFold.obj" \
	"$(INTDIR)\FindChineseFunc.obj" \
	"$(INTDIR)\FindHelp.obj" \
	"$(INTDIR)\FindString.obj" \
	"$(INTDIR)\FindTools.obj" \
	"$(INTDIR)\FindToolsDlg.obj" \
	"$(INTDIR)\HyperLink.obj" \
	"$(INTDIR)\ListBoxExt.obj" \
	"$(INTDIR)\ListCtrlExt.obj" \
	"$(INTDIR)\MacroTool.obj" \
	"$(INTDIR)\MyProgressCtrl.obj" \
	"$(INTDIR)\SortHeaderCtrl.obj" \
	"$(INTDIR)\SortListCtrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\FindTools.res" \
	".\WINMM.LIB" \
	"..\FindToolsDll\Debug\FindToolsDll.lib" \
	"..\FindToolsRsDll\Debug\FindToolsRsDll.lib"

"$(OUTDIR)\FindTools.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("FindTools.dep")
!INCLUDE "FindTools.dep"
!ELSE 
!MESSAGE Warning: cannot find "FindTools.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "FindTools - Win32 Release" || "$(CFG)" == "FindTools - Win32 Debug"
SOURCE=.\ButtonEx.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\ButtonEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\ButtonEx.obj"	"$(INTDIR)\ButtonEx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\CreditStatic.CPP

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\CreditStatic.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\CreditStatic.obj"	"$(INTDIR)\CreditStatic.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\EditExt.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\EditExt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\EditExt.obj"	"$(INTDIR)\EditExt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\FindChinese.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\FindChinese.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\FindChinese.obj"	"$(INTDIR)\FindChinese.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\FindChineseEx.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\FindChineseEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\FindChineseEx.obj"	"$(INTDIR)\FindChineseEx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\FindChineseFile.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\FindChineseFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\FindChineseFile.obj"	"$(INTDIR)\FindChineseFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\FindChineseFold.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\FindChineseFold.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\FindChineseFold.obj"	"$(INTDIR)\FindChineseFold.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\FindChineseFunc.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\FindChineseFunc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\FindChineseFunc.obj"	"$(INTDIR)\FindChineseFunc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\FindHelp.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\FindHelp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\FindHelp.obj"	"$(INTDIR)\FindHelp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\FindString.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\FindString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\FindString.obj"	"$(INTDIR)\FindString.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\FindTools.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\FindTools.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\FindTools.obj"	"$(INTDIR)\FindTools.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\FindTools.rc

"$(INTDIR)\FindTools.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\FindToolsDlg.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\FindToolsDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\FindToolsDlg.obj"	"$(INTDIR)\FindToolsDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\HyperLink.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\HyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\HyperLink.obj"	"$(INTDIR)\HyperLink.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\ListBoxExt.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\ListBoxExt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\ListBoxExt.obj"	"$(INTDIR)\ListBoxExt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\ListCtrlExt.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\ListCtrlExt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\ListCtrlExt.obj"	"$(INTDIR)\ListCtrlExt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\MacroTool.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\MacroTool.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\MacroTool.obj"	"$(INTDIR)\MacroTool.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\MyProgressCtrl.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\MyProgressCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\MyProgressCtrl.obj"	"$(INTDIR)\MyProgressCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\SortHeaderCtrl.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\SortHeaderCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\SortHeaderCtrl.obj"	"$(INTDIR)\SortHeaderCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\SortListCtrl.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"


"$(INTDIR)\SortListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"


"$(INTDIR)\SortListCtrl.obj"	"$(INTDIR)\SortListCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FindTools.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "FindTools - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\FindTools.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\FindTools.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FindTools.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\FindTools.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "FindTools - Win32 Release"

"FindToolsDll - Win32 Release" : 
   cd "\computer file\C++\FindTool\FindToolsDll"
   $(MAKE) /$(MAKEFLAGS) /F ".\FindToolsDll.mak" CFG="FindToolsDll - Win32 Release" 
   cd "..\FindTools"

"FindToolsDll - Win32 ReleaseCLEAN" : 
   cd "\computer file\C++\FindTool\FindToolsDll"
   $(MAKE) /$(MAKEFLAGS) /F ".\FindToolsDll.mak" CFG="FindToolsDll - Win32 Release" RECURSE=1 CLEAN 
   cd "..\FindTools"

!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"

"FindToolsDll - Win32 Debug" : 
   cd "\computer file\C++\FindTool\FindToolsDll"
   $(MAKE) /$(MAKEFLAGS) /F ".\FindToolsDll.mak" CFG="FindToolsDll - Win32 Debug" 
   cd "..\FindTools"

"FindToolsDll - Win32 DebugCLEAN" : 
   cd "\computer file\C++\FindTool\FindToolsDll"
   $(MAKE) /$(MAKEFLAGS) /F ".\FindToolsDll.mak" CFG="FindToolsDll - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\FindTools"

!ENDIF 

!IF  "$(CFG)" == "FindTools - Win32 Release"

"FindToolsRsDll - Win32 Release" : 
   cd "\computer file\C++\FindTool\FindToolsRsDll"
   $(MAKE) /$(MAKEFLAGS) /F ".\FindToolsRsDll.mak" CFG="FindToolsRsDll - Win32 Release" 
   cd "..\FindTools"

"FindToolsRsDll - Win32 ReleaseCLEAN" : 
   cd "\computer file\C++\FindTool\FindToolsRsDll"
   $(MAKE) /$(MAKEFLAGS) /F ".\FindToolsRsDll.mak" CFG="FindToolsRsDll - Win32 Release" RECURSE=1 CLEAN 
   cd "..\FindTools"

!ELSEIF  "$(CFG)" == "FindTools - Win32 Debug"

"FindToolsRsDll - Win32 Debug" : 
   cd "\computer file\C++\FindTool\FindToolsRsDll"
   $(MAKE) /$(MAKEFLAGS) /F ".\FindToolsRsDll.mak" CFG="FindToolsRsDll - Win32 Debug" 
   cd "..\FindTools"

"FindToolsRsDll - Win32 DebugCLEAN" : 
   cd "\computer file\C++\FindTool\FindToolsRsDll"
   $(MAKE) /$(MAKEFLAGS) /F ".\FindToolsRsDll.mak" CFG="FindToolsRsDll - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\FindTools"

!ENDIF 


!ENDIF 

