// MacroTool.cpp : implementation file
//

#include "stdafx.h"
#include "FindTools.h"
#include "MacroTool.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MacroTool property page

IMPLEMENT_DYNCREATE(MacroTool, CPropertyPage)

MacroTool::MacroTool() : CPropertyPage(MacroTool::IDD)
{
	//{{AFX_DATA_INIT(MacroTool)
	//}}AFX_DATA_INIT
}

MacroTool::~MacroTool()
{
}

void MacroTool::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MacroTool)
	DDX_Control(pDX, IDC_PROGRESS_MACRO, m_Progress_Macro);
	DDX_Control(pDX, IDC_BUTTON_SELECTFILE, m_Button_SelectFile);
	DDX_Control(pDX, IDC_EDIT_MACRO, m_Edit_Macro);
	DDX_Control(pDX, IDC_RICHEDIT_PREVIEW, m_FilePreview);
	DDX_Control(pDX, IDC_LIST_MACRO_LISTFILE, m_ListFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MacroTool, CPropertyPage)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(MacroTool)
	ON_BN_CLICKED(IDC_BUTTON_SELECTFILE, OnButtonSelectfile)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MACRO_LISTFILE, OnClickListMacroListfile)
	ON_BN_CLICKED(IDC_BUTTON_SINGLE, OnButtonSingle)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MACRO_LISTFILE, OnDblclkListMacroListfile)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_SETFONT, OnButtonSetfont)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_MACRO_LISTFILE, OnKeydownListMacroListfile)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_COMMAND(ID_MENU_SINGLEOUT, OnMenuSingleout)
	ON_COMMAND(ID_MENU_DELETE, OnMenuDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MacroTool message handlers

BOOL MacroTool::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	//*****************设置进度条
	m_Progress_Macro.ShowWindow(SW_HIDE);
	//***************************设置文字
	cClew.LoadString(IDS_STRING_CLEW);
	//*******************设置listctrl*******************************
	DWORD oldStyle = m_ListFile.GetExtendedStyle();
	m_ListFile.SetExtendedStyle(oldStyle |LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES);//LVS_EX_TRACKSELECT
	//添加列,listctrl各个列
	m_ListFile.InsertColumn(0,"文件名"		,LVCFMT_LEFT,150,0);
	m_ListFile.InsertColumn(1,"所在文件夹"	,LVCFMT_LEFT,300,1);
	//**************设置richeditctrl控件的字体信息
	memset(&chFormat,0x00,sizeof(CHARFORMAT));
	chFormat.cbSize			= sizeof(CHARFORMAT);
	chFormat.dwMask			= CFM_COLOR|CFM_FACE|CFM_BOLD|CFM_ITALIC|CFM_SIZE|CFM_UNDERLINE|CFM_STRIKEOUT;
	chFormat.dwEffects		&= ~CFE_AUTOCOLOR;
	chFormat.crTextColor	= RGB(255,0,0);
	strcpy(chFormat.szFaceName,"宋体");
	//*************设置RichEdit控件的tab大小,好像不管用
	PARAFORMAT pf;
	memset(&pf,0x00,sizeof(PARAFORMAT));
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_ALIGNMENT|PFM_TABSTOPS;
	//pf.wNumbering = PFN_BULLET;
	pf.cTabCount = 4;
	m_FilePreview.SetParaFormat(pf);
	//**************设置字体对话框的信息**************
	logfont.lfHeight	= 24;//64,96
	logfont.lfWeight	= FW_DONTCARE;
	logfont.lfItalic	= FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet	= GB2312_CHARSET;
	strcpy(logfont.lfFaceName,"宋体");
	//******************菜单位图信息*************
	cMBSingleOut.LoadBitmap(IDB_BITMAP_SINGLEOUT);
	cMBDelete.LoadBitmap(IDB_BITMAP_DELETE);
	
	imagelist.DeleteImageList();
	imagelist.Create(16, 16, ILC_MASK|ILC_COLORDDB, 1, 100);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MacroTool::OnButtonSelectfile() 
{
	// TODO: Add your control notification handler code here
	CString	pathfilename,filename,pathname;
	char	filetype[] = "VC文件(*.c;*.cpp;*.h)|*.c;*.cpp;*.h|";
	int		iFilenum;//文件个数
    char    cCurDir[256] = "";

    GetCurrentDirectory(sizeof(cCurDir),cCurDir);

	CFileDialog open(TRUE,NULL,
		NULL,OFN_ALLOWMULTISELECT|
		OFN_EXTENSIONDIFFERENT|
		OFN_HIDEREADONLY,filetype);
	
	open.m_ofn.lpstrTitle = "选择文件";
	open.m_ofn.lpstrInitialDir = cCurDir;
	open.m_ofn.lpstrDefExt = "*.c;*.cpp";
	
	if (open.DoModal() == IDOK)
	{
		POSITION ps = open.GetStartPosition();
		iFilenum	= 0;

		m_ListFile.SetImageList(&imagelist, LVSIL_SMALL);//给列表控件分配一个图象列表

		while (ps != NULL)
		{
			pathfilename = open.GetNextPathName(ps);
			filename	 = open.GetFileName();
			pathname	 = open.GetPathName();

			int pos = pathfilename.ReverseFind('\\');//获得最右边的"\"的位置,用于截取文件名
						
			SHFILEINFO sfi;
			if (::SHGetFileInfo (pathfilename, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO),SHGFI_USEFILEATTRIBUTES | SHGFI_DISPLAYNAME | SHGFI_TYPENAME |SHGFI_ICON|SHGFI_SMALLICON ))
			{
				imagelist.Add(sfi.hIcon);
				if (pathfilename.Compare(pathname))//不相等(选择了多个文件)
				{
					int len = pathfilename.GetLength();
					m_ListFile.InsertItem(iFilenum,pathfilename.Right(len - pos - 1),iFilenum);//添加列表框第一项,文件名
					m_ListFile.SetItemText(iFilenum,1,pathname);//添加路径
				}
				else//选择了一个文件
				{
					m_ListFile.InsertItem(0,filename);//添加列表框第一项,文件名
					m_ListFile.SetItemText(iFilenum,1,pathname.Left(pos));//添加路径
				}
				iFilenum++;
			}
		}
	}
}
CString MacroTool::GetMacroReplace(CString filename,CString cmacro)
{
	CStdioFile	file;
	CString		bufRead;//文件中的一行代码
	CString		bufNew;//执行之后扫描之后的代码
	BOOL		bMacroBegin;
	//代码所在的位置相对于输入的宏来说是否非常明确,若非常明确就为FALSE表示不需要拷贝.
	//反之若代码所在的位置相对宇宏来说不是非常明确,就为TRUE,表示需要拷贝.
	BOOL		bCopy;//是否开始拷贝代码
	int			isMacro;//获得该行代码的类别
	BOOL		bMacro;//表示是否需要从IsMacro函数中获得信息,TRUE是,FALSE:否
	BOOL		bifndef;//是否有"#ifndef"这个宏
	
	if (file.Open(filename,CFile::modeRead,NULL))//打开成功
	{
		bMacroBegin = FALSE;
		bMacro		= TRUE;
		bufNew		= "";

		bifndef		= FALSE;
		while (1)
		{
			bCopy = TRUE;

			if (bMacro)
			{
				if (file.ReadString(bufRead))
					isMacro = IsMacro(bufRead,cmacro);//获得该行代码的类别
				else
					break;
			}
			switch (isMacro)//代码的类别
			{
			case 1://bufRead中包含strMacro定义的宏
				while (1)
				{
					bMacroBegin = FALSE;//代码位置明确
					if (file.ReadString(bufRead))
					{
						int iMacroType = GetMacroType(bufRead);
						if (iMacroType == 5)//#endif
						{
							if (bifndef)//这个#endif是#ifndef的结束的宏,该行代码也需要拷贝
							{
								bifndef = FALSE;//表示#ifndef这个宏定义的范围已经结束了
								bufNew = bufNew + bufRead + "\n";
							}
							break;//拷贝结束
						}
						else if ((iMacroType == 3) || (iMacroType == 4))//#elif,#else
						{
							bCopy = FALSE;//作一个标志,以下部分的其它宏定义部分的代码不进行拷贝
							continue;
						}
						else
						{
							if (iMacroType == 6)//#ifndef
								bifndef = TRUE;//表示#ifndef这个宏定义的范围已经开始了

							if (bCopy)
							{
								bufNew = bufNew + bufRead + "\n";
							}
						}
					}
					else//已经到了文件结尾,这个宏定义是不完整的
					{
						file.Close();
						return (bufNew);
					}
				}
				bMacro = TRUE;
				break;
			case 0://bufRead只是一般的代码,不在宏定义之中
				bufNew = bufNew + bufRead + "\n";
				bMacro = TRUE;
				break;
			case -1://bufRead包含其它的宏定义,不包括strMacro
				while (1)
				{
					bMacroBegin = TRUE;//代码相对宇宏来说位置不明确
					if (file.ReadString(bufRead))
					{
						int iMacroType = GetMacroType(bufRead);
						if (iMacroType == 5)//#endif
						{
							if (bifndef)//这个#endif是#ifndef的结束的宏
							{
								bifndef = FALSE;
								continue;
							}
							bMacro = TRUE;
							break;
						}
						if ((iMacroType == 3) || (iMacroType == 4))//#elif,#else
						{
							bMacro = FALSE;//其它的宏定义已经结束了,需要重新获得代码的类型
							//不需要在循环开始部分再获得一次代码的类型
							isMacro = IsMacro(bufRead,cmacro);
							break;//结束
						}
						else if (iMacroType == 6)//#ifndef
							bifndef = TRUE;
					}
					else//已经到了文件结尾,这个宏定义是不完整的
					{
						file.Close();
						return (bufNew);
					}
				}
				break;
			case -2://无法确定代码部分是否在相关的宏定义中
				if (bMacro)//不拷贝#else,#elif这一行代码
				{
					bufNew = bufNew + bufRead + "\n";
				}
				if (bMacroBegin)
				{
					if (file.ReadString(bufRead))
					{
						int iMacroType = GetMacroType(bufRead);
						if ((iMacroType == 3) || (iMacroType == 4) || (iMacroType == 5))//#elif,#else,#endif
						{
							bMacro = TRUE;
							break;//拷贝结束
						}
						else
						{
							if (bCopy)
							{
								bufNew = bufNew + bufRead + "\n";
							}
						}
					}
					else//已经到了文件结尾,这个宏定义是不完整的
					{
						file.Close();
						return (bufNew);
					}
				}
				break;
			}
		}
		file.Close();
	}
	return (bufNew);
}

CString MacroTool::GetMacroReplace(CString filename,CString cmacro,CRichEditCtrl * pRichEdit)
{
	CStdioFile	file;
	CString		bufRead;//文件中的一行代码
	CString		bufNew;//执行之后扫描之后的代码
	BOOL		bMacroBegin;
	//代码所在的位置相对于输入的宏来说是否非常明确,若非常明确就为FALSE表示不需要拷贝.
	//反之若代码所在的位置相对宇宏来说不是非常明确,就为TRUE,表示需要拷贝.
	BOOL		bColor;
	int			isMacro;//获得该行代码的类别
	int			iLineNum = 0;
	BOOL		bMacro;//表示是否需要从IsMacro函数中获得信息,TRUE是,FALSE:否
	BOOL		bifndef;//是否有"#ifndef"这个宏
	long		nStartChar,nEndChar;
	short		iFileLineCount;//保存文件行数

	if (pRichEdit == NULL)
		return bufNew;

	CWaitCursor wait;

	if (file.Open(filename,CFile::modeRead,NULL))//打开成功
	{
		bufNew = "";
		iFileLineCount = 0;
		while (file.ReadString(bufRead))
		{
			bufNew = bufNew + bufRead + "\r\n";
			iFileLineCount++;
		}
		file.Close();
		pRichEdit->SetWindowText(bufNew);

		m_Progress_Macro.ShowWindow(SW_SHOW);
		m_Progress_Macro.SetRange(0,iFileLineCount);
		m_Progress_Macro.SetStep(1);
	}
//****************************************************************
	if (file.Open(filename,CFile::modeRead,NULL))//打开成功
	{
		bMacroBegin = FALSE;
		bMacro		= TRUE;
		bColor		= FALSE;//是否需要开始变色
		bufNew		= "";

		nStartChar	= 0;
		nEndChar	= 0;
		iLineNum	= 0;
		
		bifndef		= FALSE;

		cSingList.RemoveAll();
		while (1)
		{
			if (bMacro)
			{
				if (file.ReadString(bufRead))
					isMacro = IsMacro(bufRead,cmacro);//获得该行代码的类别
				else
					break;
			}
			switch (isMacro)
			{
			case 1://bufRead中包含strMacro定义的宏
				nStartChar = bufNew.GetLength();
				
				if (bMacro)
				{
					bufNew = bufNew + bufRead + "\r\n";
					iLineNum++;
					m_Progress_Macro.SetPos(iLineNum);

					nEndChar = bufNew.GetLength();
					pRichEdit->SetSel(nStartChar,nEndChar);
					pRichEdit->SetSelectionCharFormat(chFormat);//使某一行(#if...)变色

					CSingList_Add(iLineNum);
				}
				while (1)
				{
					bMacroBegin = FALSE;
					if (file.ReadString(bufRead))
					{
						bufNew = bufNew + bufRead + "\r\n";
						iLineNum++;
						m_Progress_Macro.SetPos(iLineNum);

						int iMacroType = GetMacroType(bufRead);
						if (iMacroType == 5)//#endif
						{
							if (bifndef)//这个#endif是#ifndef的结束的宏
							{
								bifndef = FALSE;
								continue;
							}
							if (bColor)
								bColor = FALSE;
							else
								nStartChar = bufNew.GetLength() - bufRead.GetLength() - strlen("\r\n");

							nEndChar = bufNew.GetLength();
							pRichEdit->SetSel(nStartChar,nEndChar);
							pRichEdit->SetSelectionCharFormat(chFormat);

							CSingList_Add(iLineNum);
							break;
						}
						else if ((iMacroType == 3) || (iMacroType == 4))//#elif,#else
						{
							if (!bColor)
							{
								bColor = TRUE;//记录需要变色的位置
								nStartChar = bufNew.GetLength() - bufRead.GetLength() - strlen("\r\n");
								
								CSingList_Add(iLineNum);
							}
							continue;
						}
						else if (iMacroType == 6)//#ifndef
							bifndef = TRUE;
					}
					else//已经到了文件结尾,这个宏定义是不完整的
					{
						file.Close();
						m_Progress_Macro.ShowWindow(SW_HIDE);
						return bufNew;
					}
				}
				bMacro = TRUE;
				break;
			case 0://bufRead只是一般的代码,不在宏定义之中
				bufNew = bufNew + bufRead + "\r\n";
				iLineNum++;
				m_Progress_Macro.SetPos(iLineNum);
				bMacro = TRUE;
				break;
			case -1://bufRead包含其它的宏定义,不包括strMacro
				nStartChar = bufNew.GetLength();

				if (bMacro)
				{
					bufNew = bufNew + bufRead + "\r\n";
					iLineNum++;
					m_Progress_Macro.SetPos(iLineNum);

					nEndChar = bufNew.GetLength();
					pRichEdit->SetSel(nStartChar,nEndChar);
					pRichEdit->SetSelectionCharFormat(chFormat);//使某一行变色

					CSingList_Add(iLineNum);
				}
				while (1)
				{
					bMacroBegin = TRUE;
					if (file.ReadString(bufRead))
					{
						bufNew = bufNew + bufRead + "\r\n";
						iLineNum++;
						m_Progress_Macro.SetPos(iLineNum);

						if (!bColor)
						{
							bColor = TRUE;//记录需要变色的位置
							nStartChar = bufNew.GetLength() - bufRead.GetLength() - strlen("\r\n");
						}
						int iMacroType = GetMacroType(bufRead);
						if (iMacroType == 5)//#endif
						{
							if (bifndef)//这个#endif是#ifndef的结束的宏
							{
								bifndef = FALSE;
								continue;
							}
							bMacro = TRUE;//其它的宏定义已经结束了,需要重新获得代码的类型

							nEndChar = bufNew.GetLength();
							pRichEdit->SetSel(nStartChar,nEndChar);
							pRichEdit->SetSelectionCharFormat(chFormat);
							bColor = FALSE;

							CSingList_Add(iLineNum);
							break;//结束
						}
						if ((iMacroType == 3) || (iMacroType == 4))//#elif,#else
						{
							bMacro = FALSE;//其它的宏定义已经结束了,需要重新获得代码的类型
							isMacro = IsMacro(bufRead,cmacro);

							nEndChar = bufNew.GetLength();
							pRichEdit->SetSel(nStartChar,nEndChar);
							pRichEdit->SetSelectionCharFormat(chFormat);
							bColor = FALSE;

							CSingList_Add(iLineNum);
							break;//结束
						}
						else if (iMacroType == 6)//#ifndef
							bifndef = TRUE;
					}
					else//已经到了文件结尾,这个宏定义是不完整的
					{
						file.Close();
						m_Progress_Macro.ShowWindow(SW_HIDE);
						return bufNew;
					}
				}
				break;
			case -2://无法确定代码部分是否在相关的宏定义中,#else
				if (bMacro)
				{
					bufNew = bufNew + bufRead + "\r\n";
					iLineNum++;
					m_Progress_Macro.SetPos(iLineNum);
				}
				if (bMacroBegin)
				{
					if (file.ReadString(bufRead))
					{
						bufNew = bufNew + bufRead + "\r\n";
						iLineNum++;
						m_Progress_Macro.SetPos(iLineNum);

						int iMacroType = GetMacroType(bufRead);
						if ((iMacroType == 3) || (iMacroType == 4) || (iMacroType == 5))//#elif,#else,#endif
						{
							bMacro = TRUE;
							
							nStartChar = bufNew.GetLength() - bufRead.GetLength() - strlen("\r\n");
							nEndChar = bufNew.GetLength();
							pRichEdit->SetSel(nStartChar,nEndChar);
							pRichEdit->SetSelectionCharFormat(chFormat);

							CSingList_Add(iLineNum);
							break;//拷贝结束
						}
					}
					else//已经到了文件结尾,这个宏定义是不完整的
					{
						file.Close();
						m_Progress_Macro.ShowWindow(SW_HIDE);
						return (bufNew);
					}
				}
				break;
			}
		}
		file.Close();
	}
	m_Progress_Macro.ShowWindow(SW_HIDE);
	return (bufNew);
}

//return当前代码预编译的类别:0:一般代码,1:#if,2:#ifdef,3:#elif,4:#else,5:#endif,6:#ifndef
int MacroTool::GetMacroType(CString bufRead)
{
	int iSpacePlace;
	CString	sLeft3,sLeft5,sLeft6,sLeft7,sLeft;
	//考查每一行程序的最左边的3/5/6个字符是否是#if,#elif/#else,#ifdef

	bufRead.TrimLeft();
	bufRead.TrimRight();
	iSpacePlace = bufRead.Find(" ");

	if (iSpacePlace == -1)//没有空格
		sLeft = bufRead;
	else//有空格
		sLeft = bufRead.Left(iSpacePlace);

	if (sLeft == "#if")
	{
		if (bufRead.Find("defined") == -1)
			return 0;
		else
			return 1;
	}
	if (sLeft == "#elif")
		return 3;
	if (sLeft == "#else")
		return 4;
	if (sLeft == "#ifdef")
		return 2;
	if (sLeft == "#endif")
		return 5;
	if (sLeft == "#ifndef")
		return 6;

	return 0;
}
//考查bufRead是否包含strMacro定义的宏
//1:bufRead中包含strMacro定义的宏
//0:bufRead只是一般的代码,没有宏定义
//-1:bufRead包含其它的宏定义,不包括strMacro
//-2:无法确定
int	MacroTool::IsMacro(CString bufRead,const CString strMacro)
{
	CString	sLeft;//考查每一行程序的最左边的1个单词是否是#if,#elif,#ifdef,#else
	CString	cStrMacro;

	if (strMacro.GetLength() == 0)//宏定义为空
		return 0;

	bufRead.TrimLeft();
	bufRead.TrimRight();
	if (bufRead.Find(" ") == -1)//该行代码中没有空格的时候,有两种可能
	{
		if ((bufRead == "#else") || (bufRead == "#endif"))
			return -2;
		else
			return 0;
	}
	else
	{
		sLeft = bufRead.Left(bufRead.Find(" "));//取出空格左边的字符串
	}

	if (sLeft == "#if")
	{
		cStrMacro = bufRead.Right(bufRead.GetLength() - 3);//除去#if
		cStrMacro.TrimLeft();
		cStrMacro.TrimRight();
		if (cStrMacro.Find("//") != -1)//如果该行代码有注释,则除去注释的部分
			cStrMacro = cStrMacro.Left(cStrMacro.Find("//"));

		if (bufRead.Find("defined") == -1)
			return 0;

		if (cStrMacro.Find(strMacro) == -1)//没有找到定义的宏,是其它的宏
			return -1;
		else//找到了这个宏定义,这部分操作需要细化,这么做不严谨
			return 1;
	}
	if (sLeft == "#elif")
	{
		cStrMacro = bufRead.Right(bufRead.GetLength() - 5);//除去#elif
		cStrMacro.TrimLeft();
		cStrMacro.TrimRight();
		if (cStrMacro.Find("//") != -1)//如果该行代码有注释,则除去注释的部分
			cStrMacro = cStrMacro.Left(cStrMacro.Find("//"));

		if (cStrMacro.Find(strMacro) == -1)//没有找到定义的宏,是其它的宏
			return -1;
		else//找到了这个宏定义,这部分操作需要细化,这么做不严谨
			return 1;
	}
	if (sLeft == "#else")//无法确定
		return -2;
	if (sLeft == "#ifdef")
	{
		cStrMacro = bufRead.Right(bufRead.GetLength() - 6);//除去#ifdef
		cStrMacro.TrimLeft();
		cStrMacro.TrimRight();
		if (cStrMacro.Find("//") != -1)//如果该行代码有注释,则除去注释的部分
			cStrMacro = cStrMacro.Left(cStrMacro.Find("//"));

		if (cStrMacro == strMacro)//找到了这个宏定义
			return 1;
		else
			return -1;
	}
	return 0;
}

void MacroTool::OnClickListMacroListfile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CRichEditCtrl	* pEditNew = ((CRichEditCtrl *)(GetDlgItem(IDC_RICHEDIT_PREVIEW)));
	CString			cMacro;

	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);//获得列表框中当前选择的项目

	if (-1 == nSelectedItem)
		return;

	CString strFileName = GetFileName();//获得文件名

	m_Edit_Macro.GetWindowText(cMacro);

	cMacro.TrimLeft();
	cMacro.TrimRight();

	GetMacroReplace(strFileName,cMacro,pEditNew);//显示变色之后的代码
	
	iCurrentLine = 1;//表示当前行为第一行
	*pResult = 0;
}

BOOL MacroTool::WriteFile(const CString filename,const CString cStr)
{
	CString sNewFileName;
	CStdioFile	fNewFile;

	sNewFileName = filename + ".bak";//新文件名
	if (!fNewFile.Open(sNewFileName,CFile::modeCreate | CFile::modeWrite | CFile::typeText,NULL))
	{//创建新文件
		fNewFile.Close();
		return FALSE;
	}

	fNewFile.WriteString(cStr);
	fNewFile.Close();
	return TRUE;
}

void MacroTool::OnButtonSingle() 
{
	// TODO: Add your control notification handler code here
	CString	cMacro,cNewInfo;
	CString strFileName = GetFileName();
	
	if (m_ListFile.GetItemCount() == 0)
	{
		MessageBox("请先选择文件",cClew,MB_ICONINFORMATION|MB_OK);
		OnButtonSelectfile();
		return;
	}
	else if (strFileName.GetLength() == 0)
	{
		MessageBox("请选择一个文件",cClew,MB_ICONINFORMATION|MB_OK);
		m_ListFile.SetFocus();
		return;
	}
	m_Edit_Macro.GetWindowText(cMacro);//获得宏的名字

	cMacro.TrimLeft();
	cMacro.TrimRight();
	if (cMacro.GetLength() == 0)
	{
		if(MessageBox("您没有输入宏的名字,是否继续?", cClew, MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;
	}

	cNewInfo = GetMacroReplace(strFileName,cMacro);
	
	WriteFile(strFileName,cNewInfo);
	MessageBox("输出成功!文件名是:" + strFileName + ".bak",cClew,MB_ICONINFORMATION|MB_OK);
}

void MacroTool::OnDblclkListMacroListfile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonSingle();
	*pResult = 0;
}

void MacroTool::OnButtonAll() 
{
	// TODO: Add your control notification handler code here
	CString		cMacro,cNewInfo;

	m_Edit_Macro.GetWindowText(cMacro);//获得宏的名字

	cMacro.TrimLeft();
	cMacro.TrimRight();
	if (cMacro.GetLength() == 0)
	{
		if(MessageBox("您没有输入宏的名字,是否继续?",cClew, MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;
	}
	CString strFileName,strPathName;

	int icount = m_ListFile.GetItemCount();//表项总数

	if (icount == 0)
	{
		MessageBox("请先选择文件",cClew,MB_ICONINFORMATION|MB_OK);
		OnButtonSelectfile();
		return;
	}

	CWaitCursor wait;
	m_Progress_Macro.ShowWindow(SW_SHOW);
	m_Progress_Macro.SetRange(0,icount);
	m_Progress_Macro.SetStep(1);

	for (int i = 0;i < icount;i++)
	{
		strFileName = m_ListFile.GetItemText(i, 0);
		strPathName = m_ListFile.GetItemText(i, 1);
		strFileName = strPathName + "\\" + strFileName;//获得文件名

		cNewInfo = GetMacroReplace(strFileName,cMacro);
	
		WriteFile(strFileName,cNewInfo);

		m_Progress_Macro.SetPos(i + 1);
	}
	m_Progress_Macro.ShowWindow(SW_HIDE);

	MessageBox("输出成功!",cClew,MB_ICONINFORMATION|MB_OK);
}

void MacroTool::OnButtonSetfont() 
{
	// TODO: Add your control notification handler code here
	CFontDialog fDialog;
	fDialog.m_cf.rgbColors = chFormat.crTextColor;
	fDialog.m_cf.lpLogFont = &logfont;
	fDialog.m_cf.Flags = fDialog.m_cf.Flags|CF_INITTOLOGFONTSTRUCT;

	if (fDialog.DoModal() == IDOK)
	{
		chFormat.crTextColor = fDialog.GetColor();
		strcpy(chFormat.szFaceName,fDialog.GetFaceName());
		chFormat.yHeight = fDialog.GetSize();

		if (fDialog.IsBold())
			chFormat.dwEffects |= CFE_BOLD;
		else
			chFormat.dwEffects &= ~CFE_BOLD;

		if (fDialog.IsItalic())
			chFormat.dwEffects |= CFE_ITALIC;
		else
			chFormat.dwEffects &= ~CFE_ITALIC;

		if (fDialog.IsStrikeOut())
			chFormat.dwEffects |= CFE_STRIKEOUT;
		else
			chFormat.dwEffects &= ~CFE_STRIKEOUT;

		if (fDialog.IsUnderline())
			chFormat.dwEffects |= CFE_UNDERLINE;
		else
			chFormat.dwEffects &= ~CFE_UNDERLINE;
	}
}

void MacroTool::OnKeydownListMacroListfile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pLVKeyDow->wVKey == VK_DELETE)
	{
		MacroTool::DeleteListCtrl();
	}
	*pResult = 0;
}
BOOL MacroTool::DeleteListCtrl()
{
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);//获得列表框中当前选择的项目
	
	if (nSelectedItem != -1)
	{
		CString str;
		int selectedcount = m_ListFile.GetSelectedCount();
		if (selectedcount > 1)//选择了多项
		{
			if(MessageBox("从列表项中删除这些文件?", cClew, MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
				int icount = m_ListFile.GetItemCount();//表项总数
				for (int i = icount - 1;i >= 0;i--)
				{
					if (m_ListFile.GetItemState(i,LVIS_SELECTED))
						m_ListFile.DeleteItem(i);
				}
			}
		}
		else//选择了一项
		{
			str = m_ListFile.GetItemText(nSelectedItem,0);//获得文件名
			if(MessageBox("从列表项中删除文件“" + str + "”?", cClew, MB_YESNO|MB_ICONQUESTION)==IDYES)
				m_ListFile.DeleteItem(nSelectedItem);	
		}
	}
	return TRUE;
}

void MacroTool::OnButtonDown() 
{
	// TODO: Add your control notification handler code here
	int iLine = GetMacroPlace(DOWN);

	if (-1 == iLine)
		return;
	m_FilePreview.LineScroll(iLine - iCurrentLine);
	if (iLine == 0)
		iCurrentLine = 1;
	else
		iCurrentLine = iLine;
}
//获得需要滚动到哪一行的行数
//direction:需要滚动的方向
int MacroTool::GetMacroPlace(DIRECTION direction) 
{
	int iListCount,i,iPosition = 0;

	iListCount = cSingList.GetSize();

	switch (direction)
	{
	case DOWN:
		for (i = 0;i < iListCount;i++)
		{
			iPosition = cSingList.GetAt(i);//获得需要变色的行的行号
			if (iPosition > iCurrentLine)
				return (iPosition);//找到了下一行
		}
		break;
	case UP:
		int iNearLine = 0;
		for (i = 0;i < iListCount;i++)
		{
			iPosition = cSingList.GetAt(i);
			if (iPosition < iCurrentLine)
				iNearLine = iPosition;
			else 
			{
				if (iNearLine == 0)
					return (cSingList.GetAt(iListCount - 1));
				else
					return (iNearLine);
			}
		}
		break;
	}
	return -1;
}

void MacroTool::OnButtonUp() 
{
	// TODO: Add your control notification handler code here
	int iLine = GetMacroPlace(UP);

	if (-1 == iLine)
		return;
	m_FilePreview.LineScroll(iLine - iCurrentLine);
	if (iLine == 0)
		iCurrentLine = 1;
	else
		iCurrentLine = iLine;
}
CString MacroTool::GetFileName()//获得文件名
{
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);//获得列表框中当前选择的项目
	CString strFileName = "",strPathName = "";
	
	if (-1 == nSelectedItem)
		return strFileName;

	strFileName = m_ListFile.GetItemText(nSelectedItem, 0);
	strPathName = m_ListFile.GetItemText(nSelectedItem, 1);

	return(strPathName + "\\" + strFileName);
}

void MacroTool::CSingList_Add(int iLineNum)
{
	cSingList.Add(iLineNum);
}

void MacroTool::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CRect lrect;
		CPoint cPoint = point;

		m_ListFile.GetWindowRect(lrect);
		ScreenToClient(lrect);
		ScreenToClient(&cPoint);//在listctrl控件上显示菜单

		if ((lrect.left <= cPoint.x) && (cPoint.x <= lrect.left + lrect.right) &&
			(lrect.top <= cPoint.y) && (cPoint.y <= lrect.top + lrect.bottom))
		{
			CMenu menu;
			VERIFY(menu.LoadMenu(CG_IDR_POPUP_MACRO_TOOL));
			
			CMenu* pPopup = menu.GetSubMenu(0);

			pPopup->SetMenuItemBitmaps(ID_MENU_SINGLEOUT,MF_BYCOMMAND,&cMBSingleOut,&cMBSingleOut);
			pPopup->SetMenuItemBitmaps(ID_MENU_DELETE,MF_BYCOMMAND,&cMBDelete,&cMBDelete);

			ASSERT(pPopup != NULL);
			CWnd* pWndPopupOwner = this;
			
			while (pWndPopupOwner->GetStyle() & WS_CHILD)
				pWndPopupOwner = pWndPopupOwner->GetParent();
			
			if (m_ListFile.GetItemCount() == 0)
			{
				pPopup->EnableMenuItem(ID_MENU_SINGLEOUT,MF_GRAYED);
				pPopup->EnableMenuItem(ID_MENU_DELETE,MF_GRAYED);
			}
			else
			{
				POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
				int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);
				if (nSelectedItem == -1)
				{
					pPopup->EnableMenuItem(ID_MENU_SINGLEOUT,MF_GRAYED);
					pPopup->EnableMenuItem(ID_MENU_DELETE,MF_GRAYED);
				}
			}

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
				pWndPopupOwner);
		}
	}
}

void MacroTool::OnMenuSingleout() 
{
	// TODO: Add your command handler code here
	OnButtonSingle();
}

void MacroTool::OnMenuDelete() 
{
	// TODO: Add your command handler code here
	DeleteListCtrl();
}

BOOL MacroTool::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD style = m_Button_SelectFile.GetButtonStyle();
    style = style | BS_DEFPUSHBUTTON;
    m_Button_SelectFile.SetButtonStyle(style,TRUE);

	return CPropertyPage::OnSetActive();
}
