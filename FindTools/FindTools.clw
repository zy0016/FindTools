; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRichEditCtrlExt
LastTemplate=CButton
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "findtools.h"
LastPage=0

ClassCount=22
Class1=CButtonEx
Class2=CCreditStatic
Class3=EditExt
Class4=FindChinese
Class5=FindChineseEx
Class6=FindChineseFile
Class7=FindChineseFold
Class8=FindChineseFunc
Class9=FindHelp
Class10=FindString
Class11=CFindToolsApp
Class12=CAboutDlg
Class13=CFindToolsDlg
Class14=CHyperLink
Class15=ListBoxExt
Class16=CListCtrlExt
Class17=MacroTool
Class18=CMyProgressCtrl
Class19=CSortHeaderCtrl
Class20=CSortListCtrl

ResourceCount=11
Resource1=IDD_FINDTOOLS_DIALOG
Resource2=IDD_DIALOG_MACROTOOL
Resource3=IDD_ABOUTBOX
Resource4=IDD_DIALOG_HELP
Resource5=IDD_DIALOG_FINDSTRING
Resource6=IDD_DIALOG_FINDCHINESE_FILE
Resource7=IDD_DIALOG_FINDCHINESE
Resource8=IDD_DIALOG_FINDCHINESE_FUNC
Resource9=IDD_DIALOG_FINDCHINESEEX
Resource10=IDD_DIALOG_FINDCHINESE_FOLD
Class21=CRichEditCtrlExt
Class22=DiZiButton
Resource11=CG_IDR_POPUP_MACRO_TOOL

[CLS:CButtonEx]
Type=0
BaseClass=CButton
HeaderFile=ButtonEx.h
ImplementationFile=ButtonEx.cpp
LastObject=CButtonEx
Filter=W
VirtualFilter=BWC

[CLS:CCreditStatic]
Type=0
BaseClass=CStatic
HeaderFile=CreditStatic.H
ImplementationFile=CreditStatic.CPP

[CLS:EditExt]
Type=0
BaseClass=CEdit
HeaderFile=EditExt.h
ImplementationFile=EditExt.cpp

[CLS:FindChinese]
Type=0
BaseClass=CPropertyPage
HeaderFile=FindChinese.h
ImplementationFile=FindChinese.cpp
LastObject=IDC_COMBO_SELECTFOLD
Filter=D
VirtualFilter=idWC

[CLS:FindChineseEx]
Type=0
BaseClass=CDialog
HeaderFile=FindChineseEx.h
ImplementationFile=FindChineseEx.cpp
LastObject=FindChineseEx

[CLS:FindChineseFile]
Type=0
BaseClass=CPropertyPage
HeaderFile=FindChineseFile.h
ImplementationFile=FindChineseFile.cpp
LastObject=FindChineseFile

[CLS:FindChineseFold]
Type=0
BaseClass=CPropertyPage
HeaderFile=FindChineseFold.h
ImplementationFile=FindChineseFold.cpp
LastObject=FindChineseFold
Filter=D
VirtualFilter=idWC

[CLS:FindChineseFunc]
Type=0
BaseClass=CPropertyPage
HeaderFile=FindChineseFunc.h
ImplementationFile=FindChineseFunc.cpp
LastObject=IDC_BUTTON_FINDCHINESEFUNC_ADD

[CLS:FindHelp]
Type=0
BaseClass=CPropertyPage
HeaderFile=FindHelp.h
ImplementationFile=FindHelp.cpp
LastObject=FindHelp

[CLS:FindString]
Type=0
BaseClass=CPropertyPage
HeaderFile=FindString.h
ImplementationFile=FindString.cpp
Filter=D
VirtualFilter=idWC
LastObject=FindString

[CLS:CFindToolsApp]
Type=0
BaseClass=CWinApp
HeaderFile=FindTools.h
ImplementationFile=FindTools.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=FindToolsDlg.cpp
ImplementationFile=FindToolsDlg.cpp
LastObject=CAboutDlg

[CLS:CFindToolsDlg]
Type=0
BaseClass=CDialog
HeaderFile=FindToolsDlg.h
ImplementationFile=FindToolsDlg.cpp
LastObject=IDC_CHECK_FRONT
Filter=D
VirtualFilter=dWC

[CLS:CHyperLink]
Type=0
BaseClass=CStatic
HeaderFile=HyperLink.h
ImplementationFile=HyperLink.cpp

[CLS:ListBoxExt]
Type=0
BaseClass=CListBox
HeaderFile=ListBoxExt.h
ImplementationFile=ListBoxExt.cpp
Filter=W
LastObject=ID_MENU_DELETE
VirtualFilter=bWC

[CLS:CListCtrlExt]
Type=0
BaseClass=CListCtrl
HeaderFile=ListCtrlExt.h
ImplementationFile=ListCtrlExt.cpp
LastObject=CListCtrlExt
Filter=W
VirtualFilter=FWC

[CLS:MacroTool]
Type=0
BaseClass=CPropertyPage
HeaderFile=MacroTool.h
ImplementationFile=MacroTool.cpp
Filter=D
VirtualFilter=idWC
LastObject=ID_MENU_DELETE

[CLS:CMyProgressCtrl]
Type=0
BaseClass=CProgressCtrl
HeaderFile=MyProgressCtrl.h
ImplementationFile=MyProgressCtrl.cpp

[CLS:CSortHeaderCtrl]
Type=0
BaseClass=CHeaderCtrl
HeaderFile=SortHeaderCtrl.h
ImplementationFile=SortHeaderCtrl.cpp

[CLS:CSortListCtrl]
Type=0
BaseClass=CListCtrl
HeaderFile=SortListCtrl.h
ImplementationFile=SortListCtrl.cpp

[DLG:IDD_DIALOG_FINDCHINESE]
Type=1
Class=FindChinese
ControlCount=13
Control1=IDC_COMBO_SELECTFOLD,combobox,1344340034
Control2=IDC_BUTTON_SELECTFOLD,button,1342242816
Control3=IDC_CHECK_SUBFOLD,button,1342242819
Control4=IDC_BUTTON_SCANING,button,1342242817
Control5=IDC_BUTTON_SETFONT,button,1342242816
Control6=IDC_BUTTON_EXCEPTION,button,1342242816
Control7=IDC_LIST_FILELIST,SysListView32,1350631425
Control8=IDC_RICHEDIT_PREVIEW,RICHEDIT,1353777348
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC_RESULT,static,1342308352
Control11=IDC_STATIC_CLEW,static,1342308352
Control12=IDC_PROGRESS_FINDCHINESE,msctls_progress32,1350565889
Control13=IDC_STATIC_SEARCHFOLD,static,1342308352

[DLG:IDD_DIALOG_FINDCHINESEEX]
Type=1
Class=FindChineseEx
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DIALOG_FINDCHINESE_FILE]
Type=1
Class=FindChineseFile
ControlCount=2
Control1=IDC_LIST_FINDCHINESEEXT_FILE,listbox,1353777409
Control2=IDC_STATIC,static,1342308353

[DLG:IDD_DIALOG_FINDCHINESE_FOLD]
Type=1
Class=FindChineseFold
ControlCount=2
Control1=IDC_STATIC,static,1342308353
Control2=IDC_LIST_FINDCHINESE_FOLD,SysListView32,1350664197

[DLG:IDD_DIALOG_FINDCHINESE_FUNC]
Type=1
Class=FindChineseFunc
ControlCount=6
Control1=IDC_EDIT_FINDCHINESEFUNC,edit,1350631552
Control2=IDC_BUTTON_FINDCHINESEFUNC_ADD,button,1342242816
Control3=IDC_BUTTON_FINDCHINESEFUNC_DEL,button,1342242816
Control4=IDC_LIST_FINDCHINESEFUNC,listbox,1353777409
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_HELP]
Type=1
Class=FindHelp
ControlCount=3
Control1=IDC_STATIC_EMAIL,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC_HELP,static,1342312448

[DLG:IDD_DIALOG_FINDSTRING]
Type=1
Class=FindString
ControlCount=8
Control1=IDC_EDIT_PREFIX,edit,1350631552
Control2=IDC_BUTTON_SELECTFOLD,button,1342242817
Control3=IDC_CHECK_FAST,button,1342242819
Control4=IDC_EDIT_STRING,edit,1352732740
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC_USEDTIMER,static,1342308352
Control8=IDC_EDIT_FILES,edit,1342244868

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_FINDTOOLS_DIALOG]
Type=1
Class=CFindToolsDlg
ControlCount=4
Control1=IDC_CHECK_FRONT,button,1342247043
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON_TUDING,button,1342242816
Control4=IDC_CHECK_TUDING,button,1342247043

[DLG:IDD_DIALOG_MACROTOOL]
Type=1
Class=MacroTool
ControlCount=13
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,button,1342177287
Control3=IDC_EDIT_MACRO,edit,1350631552
Control4=IDC_BUTTON_SELECTFILE,button,1342242816
Control5=IDC_BUTTON_SETFONT,button,1342242816
Control6=IDC_LIST_MACRO_LISTFILE,SysListView32,1350631425
Control7=IDC_STATIC,static,1342308352
Control8=IDC_RICHEDIT_PREVIEW,RICHEDIT,1353777348
Control9=IDC_BUTTON_SINGLE,button,1342242816
Control10=IDC_BUTTON_ALL,button,1342242816
Control11=IDC_BUTTON_UP,button,1342242816
Control12=IDC_BUTTON_DOWN,button,1342242816
Control13=IDC_PROGRESS_MACRO,msctls_progress32,1350565889

[MNU:CG_IDR_POPUP_MACRO_TOOL]
Type=1
Class=?
Command1=ID_MENU_SINGLEOUT
Command2=ID_MENU_DELETE
CommandCount=2

[CLS:CRichEditCtrlExt]
Type=0
HeaderFile=RichEditCtrlExt.h
ImplementationFile=RichEditCtrlExt.cpp
BaseClass=CRichEditCtrl
Filter=W
LastObject=CRichEditCtrlExt
VirtualFilter=WC

[CLS:DiZiButton]
Type=0
HeaderFile=DiZiButton.h
ImplementationFile=DiZiButton.cpp
BaseClass=CButton
Filter=W
LastObject=DiZiButton
VirtualFilter=BWC

