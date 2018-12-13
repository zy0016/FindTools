   // FindChinese.cpp : implementation file
//

#include "stdafx.h"
#include "FindTools.h"
#include "FindChinese.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindChinese property page

IMPLEMENT_DYNCREATE(FindChinese, CPropertyPage)

FindChinese::FindChinese() : CPropertyPage(FindChinese::IDD)
{
	//{{AFX_DATA_INIT(FindChinese)
	m_bSubFold = TRUE;
	//}}AFX_DATA_INIT
}

FindChinese::~FindChinese()
{
	ListFuncEx.RemoveAll();
}

void FindChinese::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindChinese)
	DDX_Control(pDX, IDC_BUTTON_SELECTFOLD, m_SelectFold);
	DDX_Control(pDX, IDC_BUTTON_SCANING, m_Button_Scaning);
	DDX_Control(pDX, IDC_BUTTON_EXCEPTION, m_Exception);
	DDX_Control(pDX, IDC_BUTTON_SETFONT, m_Button_Font);
	DDX_Control(pDX, IDC_COMBO_SELECTFOLD, m_Combox_SelectFold);
	DDX_Control(pDX, IDC_PROGRESS_FINDCHINESE, m_Progress);
	DDX_Control(pDX, IDC_LIST_FILELIST, m_ListFile);
	DDX_Control(pDX, IDC_STATIC_RESULT, m_Static_ScanResult);
	DDX_Control(pDX, IDC_RICHEDIT_PREVIEW, m_FilePreview);
	DDX_Control(pDX, IDC_CHECK_SUBFOLD, m_SubFold);
	DDX_Check(pDX, IDC_CHECK_SUBFOLD, m_bSubFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindChinese, CPropertyPage)
	//{{AFX_MSG_MAP(FindChinese)
	ON_BN_CLICKED(IDC_BUTTON_SELECTFOLD, OnButtonSelectfold)
	ON_BN_CLICKED(IDC_BUTTON_SETFONT, OnButtonSetfont)
	ON_BN_CLICKED(IDC_BUTTON_EXCEPTION, OnButtonException)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILELIST, OnClickListFilelist)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FILELIST, OnColumnclickListFilelist)
	ON_BN_CLICKED(IDC_BUTTON_SCANING, OnButtonScaning)
	ON_CBN_DROPDOWN(IDC_COMBO_SELECTFOLD, OnDropdownComboSelectfold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindChinese message handlers

BOOL FindChinese::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	//****************初始化listctrl控件
	iSortColumn = 0;
	DWORD oldStyle = m_ListFile.GetExtendedStyle();
	m_ListFile.SetExtendedStyle(oldStyle |LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES);
	//添加列
	CString strLoadString;
	strLoadString.LoadString(IDS_STRING_FILENAME);
	m_ListFile.InsertColumn(0,strLoadString,LVCFMT_LEFT,500,0);//文件名

	strLoadString.LoadString(IDS_STRING_CHINESELINE);
	m_ListFile.InsertColumn(1,strLoadString,LVCFMT_LEFT,75,1);//中文所在行
	
	strLoadString.LoadString(IDS_STRING_CODEPREVIEW);
	m_ListFile.InsertColumn(2,strLoadString,LVCFMT_LEFT,600,2);//代码预览
	//***************初始化imagelist
	imagelist.DeleteImageList();
	imagelist.Create(16, 16, ILC_MASK|ILC_COLORDDB, 1, 100);
	//**************初始化RichEdit控件所需字体部分
	strLoadString.LoadString(IDS_STRING_FONT);//宋体
	memset(&chFormat,0x00,sizeof(CHARFORMAT));
	chFormat.cbSize			= sizeof(CHARFORMAT);
	chFormat.dwMask			= CFM_COLOR|CFM_FACE|CFM_BOLD|CFM_ITALIC|CFM_SIZE|CFM_UNDERLINE|CFM_STRIKEOUT;
	chFormat.dwEffects		&= ~CFE_AUTOCOLOR;
	chFormat.crTextColor	= RGB(255,0,0);
	strcpy(chFormat.szFaceName,strLoadString);//宋体
	//**************设置字体信息**************
	logfont.lfHeight	= 24;//64,96
	logfont.lfWeight	= FW_DONTCARE;
	logfont.lfItalic	= FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet	= GB2312_CHARSET;
	strcpy(logfont.lfFaceName,strLoadString);//宋体
	//***************设置进度条*************
	m_Progress.ShowWindow(SW_HIDE);
	//****************读取例外项列表********
	ReadFunctionExList();
	//***************读取目录列表
	ReadFoldList();
    //****************初始化CButtonEx控件的消息
    m_SubFold.iCheckId = IDC_CHECK_SUBFOLD;
    m_SubFold.iClickId = IDC_CHECK_SUBFOLD;

    m_Button_Font.iCheckId = IDC_BUTTON_SETFONT;
    m_Button_Font.iClickId = IDC_BUTTON_SETFONT;

    m_Exception.iCheckId = IDC_BUTTON_EXCEPTION;
    m_Exception.iClickId = IDC_BUTTON_EXCEPTION;

    m_Button_Scaning.iCheckId = IDC_BUTTON_SCANING;
    m_Button_Scaning.iClickId = IDC_BUTTON_SCANING;

    m_SelectFold.iCheckId = IDC_BUTTON_SELECTFOLD;
    m_SelectFold.iClickId = IDC_BUTTON_SELECTFOLD;

    //*************************
    m_FindingStatus = STOP;
	/*HINSTANCE hInst = ShellExecute(this->GetSafeHwnd(), _T("open"), "hh.exe",
		(CString)cCurrentDir + "\\IVL.CHM", NULL, SW_MAXIMIZE);*/
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FindChinese::OnButtonSelectfold() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi; 
	ITEMIDLIST * pidl; 
	char Dir[MAX_PATH] = "";
	CString csFoldInit;//获得初始目录以便选择
	CString strLoadString;

	m_Combox_SelectFold.GetWindowText(csFoldInit);//获得初始目录以便选择
	strLoadString.LoadString(IDS_STRING_DSELECTFOLD);//选择目录

	bi.hwndOwner		= this->GetSafeHwnd();
	bi.pidlRoot			= NULL;
	bi.pszDisplayName	= Dir; 
	bi.lpszTitle		= strLoadString; //选择目录
	bi.ulFlags			= BIF_RETURNONLYFSDIRS ; 
	bi.lpfn				= SetSelProc;
	bi.lParam			= (LPARAM)(LPCTSTR)csFoldInit;
	bi.iImage			= 0; 
	
	pidl = SHBrowseForFolder(&bi);

	if(pidl == NULL) 
		memset(Dir,0,sizeof(MAX_PATH));
	if(SHGetPathFromIDList(pidl,Dir))/*Retrieve folder name from ITEMLIST structure.*/ 
	{
		m_Combox_SelectFold.SetWindowText(Dir);
	}
}
//开始在文件中查找中文字符
//filename:文件名
int FindChinese::FindChineseCode(CString filename)
{
	CStdioFile	file;
	CString		bufRead;
	int			iFileLineNum;
	LPTSTR		lpstr,lpstrNext;
	BOOL		bNotesEnd;		//是否在"//"以后的一行代码中存在"*/"
	BOOL		bNote;//是否注释开始("/*"多行注释)
	BOOL		bS,bD;//是否有单引号,是否有双引号
	short		iFileLineCount;//保存文件行数
	CString		cfile;//保存文件名,不包括路径

	if (file.Open(filename,CFile::modeRead,NULL))//打开成功
	{
		iFileLineCount = 0;//计算文件行数,为进度条提供数据
		while (file.ReadString(bufRead))
			iFileLineCount++;

		file.Close();

		m_Progress.ShowWindow(SW_SHOW);
		m_Progress.SetRange(0,iFileLineCount);
		m_Progress.SetStep(1);
		m_Progress.SetProgressFillColor(RGB(255,0,0));
		m_Progress.SetProgressTextColor(RGB(0,0,255));
		m_Progress.ShowPreText(TRUE);
	}

	if (file.Open(filename,CFile::modeRead,NULL))//打开成功
	{
		CString strLoadString;
		strLoadString.LoadString(IDS_STRING_SCANING);//正在扫描

		iFileLineNum	= 0;
		bNote			= FALSE;
		
		cfile = file.GetFileName();
		m_Static_ScanResult.SetWindowText(strLoadString + cfile);//正在扫描
		
		while (file.ReadString(bufRead))//开始一行一行扫描代码
		{
			bS = FALSE;		//没有单引号
			bD = FALSE;		//没有双引号
			iFileLineNum++;	//记录当前扫描的行数
			bufRead.TrimLeft();
			bufRead.TrimRight();
			
			if (bufRead == "#if 0")
			{
				while (file.ReadString(bufRead))//需要跳过#if 0这部分代码
				{
					bufRead.TrimLeft();
					bufRead.TrimRight();
					iFileLineNum++;	//记录当前扫描的行数
					m_Progress.SetPos(iFileLineNum);
					if ((bufRead == "#endif") || (bufRead == "#else"))
						break;
				}
			}
			
			lpstr		= NULL;
			lpstrNext	= NULL;
			lpstr		= bufRead.GetBuffer(bufRead.GetLength());

			m_Progress.SetPos(iFileLineNum);

			while (* lpstr != 0)//在一行代码中进行扫描
			{
				if (* lpstr == 34)//当前字符是双引号
					bD = !bD;
				else if (* lpstr == 39)//当前字符单引号
					bS = !bS;
				else
				{
					if (* lpstr == 47)//当前字符是"/"
					{
						lpstrNext = lpstr;
						lpstrNext++;
						if (* lpstrNext == 0)//到达行尾
							break;

						if (* lpstrNext == 47)//下一个字符是"/"
						{
							if ((bS) || (bD))//在"//"之前有单引号或者双引号表示这个"//"不是注释
							{
								lpstr++;
								continue;
							}
							else//在"//"之前没有单引号没有双引号表示这个"//"是注释,开始扫描下一行
							{
								//存在这种情况,"//...*/"这时候这一行是多行注释的结束
								lpstr += 2;
								bNotesEnd = FALSE;
								while (* lpstr != 0)
								{
									if (* lpstr == 42)//当前字符是"*"
									{
										lpstrNext = lpstr;
										lpstrNext++;
										if ((* lpstrNext == 47) && (bNote))//下一个字符是"/"并且有多行注释
										{
											bNote = FALSE;//表示多行注释结束
											bNotesEnd = TRUE;
											lpstr += 2;
											break;
										}
									}
									lpstr++;
								}
								if (bNotesEnd)//是在"//"以后的一行代码中存在"*/"需要在这一行中继续扫描
									break;
							}
						}
						else if (* lpstrNext == 42)//下一个字符是"*"
						{
							if ((bS) || (bD))//在"/*"之前有单引号或者双引号表示这个"/*"不是注释
							{
								lpstr++;
								continue;
							}
							else//在"/*"之前没有单引号没有双引号表示这个"/*"是注释
							{
								lpstr++;
								bNote = TRUE;//表示多行注释开始
								continue;
							}
						}
					}
					else if (* lpstr == 42)//当前字符是"*"
					{
						lpstrNext = lpstr;
						lpstrNext++;
						if (* lpstrNext == 0)//到达行尾
							break;
						if ((* lpstrNext == 47) && (bNote))//下一个字符是"/"并且有多行注释
						{
							bNote = FALSE;//表示多行注释结束
							lpstr += 2;//lpstr++;
							continue;
						}
					}
				}//end if

				if ((unsigned char)(* lpstr) > 0x80)
				{
					if (!bNote)//当前代码位置不是注释
					{
						AddFile(filename,iFileLineNum,bufRead);//找到了代码中的中文
						break;//开始扫描下一行
					}
					lpstr += 2;
				}
				else if (* lpstr != 0)
				{
					lpstr++;
				}
			}//end while
            bufRead.ReleaseBuffer();
		}//end while
		file.Close();
	}//end if
	m_Progress.ShowWindow(SW_HIDE);
	return 1;
}
void FindChinese::AddFile(CString filename,int iLineNum,CString code)//将文件名行号添加到listctrl控件中
{
	int iListCount = m_ListFile.GetItemCount();
	long iTextLength;
	SIZE size;
	CString cLineNum,cfile;
	CString	cResult;

	if (IsInFunctionEx(code))
		return;

	cLineNum.Format("%d",iLineNum);
	m_ListFile.InsertItem (iListCount,filename	);	//添加列表框第一项,文件名
	m_ListFile.SetItemText(iListCount,1,cLineNum);	//添加行号
	m_ListFile.SetItemText(iListCount,2,code	);	//添加代码预览
	m_ListFile.SetItemData(iListCount,iListCount);	//用于排序
	size.cx = 0;
	size.cy = iListCount + 1;
	m_ListFile.Scroll(size);

	cfile = filename.Right(filename.GetLength() - filename.ReverseFind('\\') - 1);

	if (cfile.GetLength() < 8)
		cResult = cfile + "\t\t\t\t" + cLineNum + "\t\t" + code + "\r\n";
	else if (cfile.GetLength() > 15)
		cResult = cfile + "\t\t"	 + cLineNum + "\t\t" + code + "\r\n";
	else
		cResult = cfile + "\t\t\t"	 + cLineNum + "\t\t" + code + "\r\n";

	iTextLength = m_FilePreview.GetWindowTextLength();
	m_FilePreview.SetSel(iTextLength, iTextLength);
	m_FilePreview.ReplaceSel(cResult);
}
//获得文件名,用pLineNum返回当前项目包含的行号
CString FindChinese::GetFileName(int * pLineNum)
{
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);//获得列表框中当前选择的项目
	CString strFileName = "",strLineNum = "";
	LPTSTR lpstr;
	
	if (-1 == nSelectedItem)
		return strFileName;

	strFileName = m_ListFile.GetItemText(nSelectedItem, 0);//获得文件名
	strLineNum	= m_ListFile.GetItemText(nSelectedItem, 1);//获得行号
	//***********以下操作将CString变量转换成int型变量
	lpstr = strLineNum.GetBuffer(strLineNum.GetLength());

	* pLineNum = atoi(lpstr);//返回行号

    strLineNum.ReleaseBuffer();
	return(strFileName);
}

void FindChinese::PreViewCode()//代码预览
{
	CString		filename,bufRead,bufNew;
	int			iLineNum,iVisibleLine,iLine,i;
	long		nStartChar,nEndChar,iFileLineCount;
	CStdioFile	file;

	if(m_FindingStatus == FINDING)//正在扫描的时候不执行代码预览
	{
		return;
	}

	CWaitCursor wait;
	
	filename = GetFileName(&iLineNum);//获得文件名,和需要滚动到哪一行

	if (file.Open(filename,CFile::modeRead,NULL))//打开成功
	{
		iFileLineCount = 0;//计算文件行数,为进度条提供数据
		while (file.ReadString(bufRead))
			iFileLineCount++;

		file.Close();

		m_Progress.ShowWindow(SW_SHOW);
		m_Progress.SetRange(0,iFileLineCount + 1);
		m_Progress.SetStep(1);
	}
	if (file.Open(filename,CFile::modeRead,NULL))//打开成功
	{
		bufNew = "";
		iLine = 0;//计算行号,用于代码变色

		while (file.ReadString(bufRead))
		{
			iLine++;
			if (iLine == iLineNum)
				nStartChar = bufNew.GetLength();//开始变色的位置,当前行的上一行
			else if (iLine == iLineNum + 1)
				nEndChar = bufNew.GetLength();//变色结束位置,当前行的下一行

			bufNew = bufNew + bufRead + "\r\n";

			m_Progress.SetPos(iLine + 1);
		}
		file.Close();
		m_FilePreview.SetWindowText(bufNew);//预览代码
		i = 0;
		do
		{
			iVisibleLine = m_FilePreview.GetFirstVisibleLine();		//获得可见行
			m_FilePreview.LineScroll(iLineNum - iVisibleLine - 1,0);//滚动到该行
			//跳转次数大于1000的话被认为可能是死循环,需要强行退出
			if ((i++ >= 1000) || (iLineNum - iVisibleLine < 78))
				break;
			TRACE("\niVisibleLine = %d\n",iVisibleLine);
		}
		while (iLineNum - iVisibleLine - 1 != 0);

		m_FilePreview.SetSel(nStartChar,nEndChar);//选中范围
		m_FilePreview.SetSelectionCharFormat(chFormat);//设置字体

		m_Progress.ShowWindow(SW_HIDE);
	}
}

void FindChinese::OnButtonSetfont() 
{
	// TODO: Add your control notification handler code here
	CFontDialog fDialog;
	fDialog.m_cf.rgbColors	= chFormat.crTextColor;
	fDialog.m_cf.lpLogFont	= &logfont;
	fDialog.m_cf.Flags		= fDialog.m_cf.Flags|CF_INITTOLOGFONTSTRUCT;
	
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
//****************************线程操作部分***************************************
UINT FindChinese::FindingThread(LPVOID lpvData)
{
	FindChinese* pThis = (FindChinese*)lpvData;

	BOOL		bWorking;
	CFileFind	find;
	CString		strFilePath,strPath,strFolder;
	CString		filename,cExpend;
	int			iExpend;
	CStringList listPaths;
	CString		cResult;

	listPaths.RemoveAll();
	strPath = pThis->cPath + "\\*.*";
	do
	{
		bWorking = find.FindFile(strPath);
		while (bWorking && (pThis->m_FindingStatus == FINDING))
		{
			bWorking	= find.FindNextFile();
			strFilePath = find.GetFilePath();
			
			if (find.IsDots())
				continue;
			
			if (find.IsDirectory())
			{
				if ((pThis->m_bSubFold) && (!pThis->IsInFoldEx(strFilePath)))
					listPaths.AddHead(strFilePath);
				continue;
			}
			else
			{
				filename = find.GetFilePath();//获得文件名(包括路径)
				iExpend	 = filename.ReverseFind('.');//获得扩展名左边的"."
				cExpend  = filename.Right(filename.GetLength() - iExpend - 1);//获得扩展名
				cExpend.MakeLower();
				if ((cExpend == "c") || (cExpend == "h") || (cExpend == "cpp"))
				{
					if (!pThis->IsInFileEx(filename))
					{
						pThis->FindChineseCode(filename);
						pThis->iFilenum++;
					}
				}
			}
		}
		if(listPaths.IsEmpty())
			break;

		strFolder	= listPaths.RemoveHead();
		strPath		= strFolder + "\\*.*";
		find.Close();
	}while (pThis->m_FindingStatus == FINDING);

	pThis->m_FindingStatus = STOP;//当前为停止状态

	CString strLoadString;

	strLoadString.LoadString(IDS_STRING_RESULT);//当前扫描%d个文件,代码中共有%d处中文
	cResult.Format(strLoadString,pThis->iFilenum,pThis->m_ListFile.GetItemCount());
	pThis->m_Static_ScanResult.SetWindowText(cResult);

	strLoadString.LoadString(IDS_STRING_BEGIN);//开始扫描
	pThis->m_Button_Scaning.SetWindowText(strLoadString);//开始扫描

	pThis->m_Combox_SelectFold.SetWindowText(pThis->cPath);

	pThis->m_Exception.EnableWindow(TRUE);
	pThis->m_Combox_SelectFold.EnableWindow(TRUE);
	pThis->m_SelectFold.EnableWindow(TRUE);
	pThis->m_SubFold.EnableWindow(TRUE);

    sndPlaySound(SOUNDFILENAME,SND_ASYNC);

	return 0;
}

void FindChinese::OnButtonException() 
{
	// TODO: Add your control notification handler code here
	FindChineseEx FCE;

	int nResponse = FCE.DoModal();

	if (nResponse == IDOK)
	{
		ReadFunctionExList();//读取例外项列表
	}
}

void FindChinese::ReadFunctionExList(void)//读取例外项列表
{
	CStdioFile	file;
	CString		bufRead;
	int			iPre;

	ListFuncEx.RemoveAll();
	ListFileEx.RemoveAll();
	ListFoldEx.RemoveAll();
	//ListFoldExSub.RemoveAll();

	if (file.Open(FINDCHINESEEXFILE,CFile::modeRead,NULL))//打开成功
	{		
		while (file.ReadString(bufRead))
		{
			iPre = bufRead.Find("=");
			if (-1 == iPre)
				break;

			while(bufRead.Right(1) == 13)//删除最后的回车符
				bufRead = bufRead.Left(bufRead.GetLength() - 1);

			if (bufRead.Left(iPre) == FUNCTION_SIGN)
			{
				bufRead = bufRead.Right(bufRead.GetLength() - iPre - 1);
				ListFuncEx.AddHead(bufRead);
			}
			else if (bufRead.Left(iPre) == FILE_SIGN)
			{
				bufRead = bufRead.Right(bufRead.GetLength() - iPre - 1);
				ListFileEx.AddHead(bufRead);
			}
			else if (bufRead.Left(iPre) == FOLD_SIGN)
			{
				bufRead = bufRead.Right(bufRead.GetLength() - iPre - 1);
				ListFoldEx.AddHead(bufRead);
				/*BOOL bFold = FALSE;
				CString cFold = bufRead.Right(2);//获得最后两位
				if (cFold.Left(1) == FOLD_SING_FOLD)
				{
					bFold = (cFold.Right(1) == "1");//包含子目录
					cFold = bufRead.Left(bufRead.GetLength() - 2);
				}
				else
				{
					cFold = bufRead.Left(bufRead.GetLength());
				}
				ListFoldEx.AddHead(cFold);
				//ListFoldExSub.AddHead(bFold ? "1" : "0");*/
			}
		}
		file.Close();
	}
}

BOOL FindChinese::IsInFunctionEx(CString code)//该行代码里的中文是否在例外项中
{
	int i,iStringListCount;
	CString csFunctinEx;
	int iFunctinExLen;
	CString csFuncRight;//函数名的右边部分
	POSITION ps;

	iStringListCount = ListFuncEx.GetCount();
	ps = ListFuncEx.GetHeadPosition();

	for (i = 0;i < iStringListCount;i++)
	{
		csFunctinEx		= ListFuncEx.GetNext(ps);//获得链表中的例外项
		iFunctinExLen	= csFunctinEx.GetLength();
		
		if (code.Left(iFunctinExLen) == csFunctinEx)//一行代码左边等于例外项
		{
			csFuncRight = code.Right(code.GetLength() - iFunctinExLen);//获得函数名的右边部分
			csFuncRight.TrimLeft();
			if (csFuncRight.Left(1) == "(")
				return TRUE;
		}
	}
	return FALSE;
}

BOOL FindChinese::IsInFileEx(CString FileName)//该文件是否在例外项中
{
	int i,iStringListCount;
	CString csFileEx;
	POSITION ps;

	iStringListCount = ListFileEx.GetCount();
	ps = ListFileEx.GetHeadPosition();

	for (i = 0;i < iStringListCount;i++)
	{
		csFileEx = ListFileEx.GetNext(ps);//获得链表中的例外项
		csFileEx.MakeLower();
		FileName.MakeLower();
		if (FileName == csFileEx)
			return TRUE;
	}
	return FALSE;
}

BOOL FindChinese::IsInFoldEx(CString FoldName)//该目录是否在例外项中
{
	int i,iStringListCount;
	CString csFoldEx;
	POSITION ps;

	iStringListCount = ListFoldEx.GetCount();
	ps = ListFoldEx.GetHeadPosition();

	for (i = 0;i < iStringListCount;i++)
	{
		csFoldEx = ListFoldEx.GetNext(ps);//获得链表中的例外项
		csFoldEx = csFoldEx.Left(csFoldEx.GetLength() - 2);
		csFoldEx.MakeLower();
		FoldName.MakeLower();
		if (FoldName == csFoldEx)
			return TRUE;
	}
	return FALSE;
}

void FindChinese::OnClickListFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	PreViewCode();
	*pResult = 0;
}

void FindChinese::OnColumnclickListFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iColumn = pNMListView->iSubItem;
	static BOOL bSortArrow = FALSE;//排序的上下箭头

	if (iColumn == iSortColumn)//排序同一列
	{
		m_ListFile.SetHeaderCtrlArrow(iColumn,bSortArrow);
		m_ListFile.m_fAsc = bSortArrow;
		bSortArrow = !bSortArrow;
	}
	else//排序不同的列
	{
		m_ListFile.SetHeaderCtrlArrow(iColumn,FALSE);//首先设置为降序排列
		m_ListFile.m_fAsc = FALSE;
		
		if (!bSortArrow)
			bSortArrow = !bSortArrow;

		iSortColumn = iColumn;
	}
	m_ListFile.m_nSortedCol = iSortColumn;
	m_ListFile.SortItems((PFNLVCOMPARE)CompareFunc,(DWORD)&m_ListFile);

	*pResult = 0;
}
int CALLBACK FindChinese::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortListCtrl* pV = (CSortListCtrl*)lParamSort;

	CString    strItem1 = pV->GetItemText(lParam1, pV->m_nSortedCol);
	CString    strItem2 = pV->GetItemText(lParam2, pV->m_nSortedCol);
	
	/*switch (pV->m_nSortedCol)
	{
	case 0:
	case 2:
		return strcmp(strItem2, strItem1);

		break;
	case 1:
		LPTSTR lpstr1,lpstr2;
		lpstr1 = strItem1.GetBuffer(strItem1.GetLength());
		lpstr2 = strItem2.GetBuffer(strItem2.GetLength());

		int iLine1,iLine2;
		iLine1 = atoi(lpstr1);
		iLine2 = atoi(lpstr2);
		if (iLine1 < iLine2)
			return 1;
		else if (iLine1 > iLine2)
			return -1;
		else
			return 0;

		break;
	}*/
	return strcmp(strItem2, strItem1);
}

BOOL FindChinese::bScaning(void)//是否正在扫描代码
{
	return (m_FindingStatus == FINDING);
}

int CALLBACK FindChinese::SetSelProc(HWND hWnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
    ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);

  return 0;
}

void FindChinese::ReadFoldList(void)//读取目录列表
{
	CStdioFile	file;
	CString		bufRead;

	if (file.Open(FINDCHINESELECTFOLD,CFile::modeRead,NULL))//打开成功
	{		
		while (file.ReadString(bufRead))
		{
			while(bufRead.Right(1) == 13)//删除最后的回车符
				bufRead = bufRead.Left(bufRead.GetLength() - 1);

			m_Combox_SelectFold.AddString(bufRead);
		}
		file.Close();
	}
}

BOOL FindChinese::SaveFoldList(CString sFold)//保存目录列表
{
	CStdioFile	file;
	int			i,iComboxCount;
	CString		csFoldList,cResult;
	
	iComboxCount = m_Combox_SelectFold.GetCount();
	for (i = iComboxCount - 1;i >= 0;i--)
	{
		m_Combox_SelectFold.GetLBText(i,csFoldList);
		if (sFold == csFoldList)//该目录在列表框中已经存在
			m_Combox_SelectFold.DeleteString(i);//删掉这一项
	}
	m_Combox_SelectFold.InsertString(0,sFold);

	iComboxCount = m_Combox_SelectFold.GetCount();
	if (iComboxCount > FOLDMAXNUM)//删掉多余的一项
		m_Combox_SelectFold.DeleteString(iComboxCount - 1);

	if (file.Open(FINDCHINESELECTFOLD,CFile::modeCreate | CFile::modeWrite | CFile::typeText,NULL))
	{
		iComboxCount = m_Combox_SelectFold.GetCount();
        cResult      = "";
		for (i = 0;i < iComboxCount;i++)
		{
			if (i > FOLDMAXNUM - 1)
				break;
			m_Combox_SelectFold.GetLBText(i,csFoldList);
			csFoldList  = csFoldList + "\r\n";
            cResult     = cResult + csFoldList;
			//file.WriteString(csFoldList);
		}
        file.WriteString(cResult);
		file.Close();	
	}
	else
	{
		MessageBox("无法保存文件.","提示",MB_ICONSTOP|MB_OK);
	}
	return TRUE;
}

void FindChinese::OnButtonScaning() 
{
	// TODO: Add your control notification handler code here
	if(m_FindingStatus == FINDING)
	{
		CString str1, str2,strBegin;

		str1.LoadString(IDS_STRING_STOPCLEW);//当前正在扫描文件,您确定停止吗?
		str2.LoadString(IDS_STRING_CLEW);//提示部分
		if(MessageBox(str1, str2, MB_YESNO|MB_ICONQUESTION)==IDNO)
		{
			return ;
		}

		m_FindingStatus = STOP;

		m_Combox_SelectFold.SetWindowText(cPath);

		strBegin.LoadString(IDS_STRING_BEGIN);
		m_Button_Scaning.SetWindowText(strBegin);//开始扫描
		m_Combox_SelectFold.EnableWindow(TRUE);
		m_SelectFold.EnableWindow(TRUE);
		m_SubFold.EnableWindow(TRUE);
		m_Exception.EnableWindow(TRUE);
		return;
	}
	CString csPath;

	m_Combox_SelectFold.GetWindowText(csPath);
	csPath.TrimLeft();
	csPath.TrimRight();
	if (csPath == "")
	{
		MessageBox("请输入或者选择目录","提示",MB_ICONASTERISK |MB_OK);
		m_Combox_SelectFold.SetFocus();
		return;
	}

    char cCurDirBak[256] = "";
    GetCurrentDirectory(sizeof(cCurDirBak),cCurDirBak);

	if (!SetCurrentDirectory(csPath))
    {
        MessageBox("无法找到目录" + csPath,"提示",MB_ICONASTERISK |MB_OK);
		m_Combox_SelectFold.SetFocus();
        SetCurrentDirectory(cCurDirBak);
		return;
    }
    SetCurrentDirectory(cCurDirBak);

	SaveFoldList(csPath);//保存目录列表

	m_ListFile.DeleteAllItems();
	m_Combox_SelectFold.SetWindowText(csPath);

	CString strEnd;

	strEnd.LoadString(IDS_STRING_STOP);
	m_Button_Scaning.SetWindowText(strEnd);//
	m_Combox_SelectFold.EnableWindow(FALSE);
	m_SelectFold.EnableWindow(FALSE);
	m_SubFold.EnableWindow(FALSE);
	m_Exception.EnableWindow(FALSE);

	CString strClew;
	strClew.LoadString(IDS_STRING_SCANRESULT);//文件名\t\t\t\t中文所在行数\t代码预览\r\n
	
	iFilenum		= 0;//文件个数的累加器
	cPath			= csPath;//记录目录
	m_FindingStatus	= FINDING;//记录当前状态
	m_bSubFold		= m_SubFold.GetCheck();//是否包括子目录
	
	CWinThread* pThread = AfxBeginThread(FindingThread,this);//开始线程
	
    m_Button_Scaning.SetFocus();
    
	m_FilePreview.SetWindowText(strClew);
}

void FindChinese::OnDropdownComboSelectfold() 
{
	// TODO: Add your control notification handler code here
	int		i,iComboxCount,iMaxLen;
	CString	csFoldList;

    iMaxLen      = 0;
	iComboxCount = m_Combox_SelectFold.GetCount();
	for (i = iComboxCount - 1;i >= 0;i--)
	{
		m_Combox_SelectFold.GetLBText(i,csFoldList);
		if (csFoldList.GetLength() > iMaxLen)
            iMaxLen = csFoldList.GetLength();
	}
    m_Combox_SelectFold.SendMessage(CB_SETDROPPEDWIDTH,iMaxLen * 7,0);
}

BOOL FindChinese::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
    DWORD style = m_Button_Scaning.GetButtonStyle();
    style = style | BS_DEFPUSHBUTTON;
    m_Button_Scaning.SetButtonStyle(style,TRUE);

	return CPropertyPage::OnSetActive();
}

BOOL FindChinese::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN)
        {
            OnButtonScaning();
        }
    }
	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL FindChinese::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == BUTTONEX_KEYDOWNCHAR)
    {
        if (lParam == 'O')//选择F键,焦点转移
        {
			m_Combox_SelectFold.SetFocus();
        }
        return TRUE;
    }
    if (wParam == LISTCTRLCHECK_CHECK)
        return TRUE;
    
	return CPropertyPage::OnCommand(wParam, lParam);
}
