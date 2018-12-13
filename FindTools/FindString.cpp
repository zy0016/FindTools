// FindString.cpp : implementation file
//

#include "stdafx.h"
#include "FindTools.h"
#include "FindString.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindString property page

IMPLEMENT_DYNCREATE(FindString, CPropertyPage)

FindString::FindString() : CPropertyPage(FindString::IDD)
{
	//{{AFX_DATA_INIT(FindString)
	//}}AFX_DATA_INIT
}

FindString::~FindString()
{
}

void FindString::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindString)
	DDX_Control(pDX, IDC_BUTTON_SELECTFOLD, m_Button_SelectFold);
	DDX_Control(pDX, IDC_EDIT_FILES, m_Edit_ListFiles);
	DDX_Control(pDX, IDC_EDIT_STRING, m_ListString);
	DDX_Control(pDX, IDC_CHECK_FAST, m_Check_Fast);
	DDX_Control(pDX, IDC_STATIC_USEDTIMER, m_UsedTimer);
	DDX_Control(pDX, IDC_EDIT_PREFIX, m_Edit_Prefix);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindString, CPropertyPage)
	//{{AFX_MSG_MAP(FindString)
	ON_BN_CLICKED(IDC_BUTTON_SELECTFOLD, OnButtonSelectfold)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_FAST, OnCheckFast)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindString message handlers

BOOL FindString::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_Edit_ListFiles.SetWindowText("请先输入字符串前缀,然后选择目录");

    m_FindingStatus = STOP;
    m_Button_SelectFold.iClickId = IDC_BUTTON_SELECTFOLD;
    m_Button_SelectFold.iCheckId = IDC_BUTTON_SELECTFOLD;
    m_Check_Fast.iCheckId        = IDC_CHECK_FAST;
    m_Check_Fast.iClickId        = IDC_CHECK_FAST;
	// TODO: Add extra initialization here
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FindString::OnButtonSelectfold() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi; 
	ITEMIDLIST * pidl; 
	char Dir[MAX_PATH] = "";
	CString cStrPre;
	
	if(m_FindingStatus == FINDING)
	{
		if(MessageBox("正在扫描字符串,是否停止?", "提示", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDNO)
		{
			return ;
		}
		KillTimer(m_nTimer);
		m_nTimer = NULL;

		m_FindingStatus = STOP;

        CString cSelectFold;
        cSelectFold.LoadString(IDS_STRING_SELECTFOLD);
		m_Button_SelectFold.SetWindowText(cSelectFold);//选择目录...
		m_Check_Fast.EnableWindow(TRUE);
		return;
	}
	m_Edit_Prefix.GetWindowText(cStrPre);//获得字符串前缀
	cStrPre.TrimLeft();
	cStrPre.TrimRight();
	cStrPre.MakeUpper();
	if (cStrPre == "")
	{
		MessageBox("请先输入字符串前缀,字符串前缀形式为STR_...","提示",MB_ICONINFORMATION|MB_OK);
		m_Edit_Prefix.SetFocus();
		return;
	}

	bi.hwndOwner		= this->GetSafeHwnd();; 
	bi.pidlRoot			= NULL; 
	bi.pszDisplayName	= Dir; 
	bi.lpszTitle		= "选择目录并按下[确定]按钮"; 
	bi.ulFlags			= BIF_RETURNONLYFSDIRS; 
	bi.lpfn				= NULL; 
	bi.lParam			= 0; 
	bi.iImage			= 0; 
	
	pidl = SHBrowseForFolder(&bi);

	if(pidl == NULL) 
		memset(Dir,0,sizeof(MAX_PATH));
	if(SHGetPathFromIDList(pidl,Dir))/*Retrieve folder name from ITEMLIST structure.*/ 
	{
		CWaitCursor wait;
		int i;

		CString AllString = "代码中没有用到的字符串";
		for (i = 0;i < 16;i++)
			AllString = AllString + " ";

		iSTRMaxLen	= AllString.GetLength();
		AllString	= AllString + "str_hp.h中的位置(行)\t\t扫描该字符串耗时(秒)\r\n";
		//AllString = "代码中没有用到的字符串\t\t\t\tstr_hp.h中的位置(行)\t\t扫描该字符串耗时(秒)\r\n";
		m_ListString.SetWindowText(AllString);

        CString cStop;
        cStop.LoadString(IDS_STRING_STOP);

		m_Button_SelectFold.SetWindowText(cStop);
		m_Check_Fast.EnableWindow(FALSE);

		cScanPath		= Dir;		//记录目录
		m_FindingStatus	= FINDING;	//记录当前状态
		m_nTimer		= SetTimer(SCANING_TIMER, 1000, NULL);//打开计时器显示时间
		iUsedSecond		= 0;//扫描计时器清零
		iUsedMinute		= 0;
		iUsedHour		= 0;

		iCheckFast		= m_Check_Fast.GetCheck();
		CWinThread* pThread = AfxBeginThread(FindingThread,this);//开始线程

        m_Button_SelectFold.SetFocus();
	}
}
//****************************线程操作部分***************************************
UINT FindString::FindingThread(LPVOID lpvData)
{
	FindString* pThis = (FindString*)lpvData;

	BOOL		bWorking;
	CFileFind	find;
	CString		strFilePath,strPath,strFolder;
	CString		filename,cExpend,cFName;
	int			iExpend;
	CStringList listPaths;

	CStdioFile	cScanFile;		//正在扫描的文件
	CString		bufRead,cStrPre;//一行数据,字符串前缀
	CString		exPath1;		//不扫描的目录
	CString		exPath2;		//不扫描的目录
	short		iStringNum;		//记录扫描的字符串的个数
	short		iFileCurLine;	//文件当前行
	BOOL		bFind;			//是否在代码中找到了该字符串

	filename = pThis->cScanPath + "\\include\\str_plx.h";//字符串文件

	if (!cScanFile.Open(filename,CFile::modeRead,NULL))//打开失败
	{
		pThis->KillTimer(pThis->m_nTimer);
		pThis->m_nTimer = NULL;

		pThis->m_FindingStatus = STOP;

        CString cSelectFold;
        cSelectFold.LoadString(IDS_STRING_SELECTFOLD);
		pThis->m_Button_SelectFold.SetWindowText(cSelectFold);//选择目录...
		pThis->m_Check_Fast.EnableWindow(TRUE);

		AfxMessageBox("找不到" + filename + "文件",MB_ICONINFORMATION|MB_OK);
		return 0;
	}

	pThis->m_Edit_Prefix.GetWindowText(cStrPre);//获得字符串前缀
	cStrPre.TrimLeft();
	cStrPre.TrimRight();
	cStrPre.MakeUpper();

	if (cStrPre.Right(1) != "_")//去掉字符串结尾的"_"
		cStrPre.Left(cStrPre.GetLength() - 1);

	iFileCurLine	= 0;								//文件当前行
	iStringNum		= 0;								//记录扫描的字符串的个数 
	exPath1			= pThis->cScanPath + "\\include";	//记录不需要扫描目录
	exPath2			= pThis->cScanPath + "\\include20";	//记录不需要扫描目录

	while (cScanFile.ReadString(bufRead) && 
			(pThis->m_FindingStatus == FINDING))//扫描每一个字符串数据
	{
		iFileCurLine++;//记录当前字符串在str_hp.h文件中的位置
		bufRead = pThis->GetValidString(bufRead,cStrPre);//返回合法的字符串
		if (bufRead == "")
			continue;

		iStringNum++;	//记录扫描的字符串的个数
		bFind = FALSE;	//是否在代码中找到了该字符串

		pThis->lUsedSecond = 0;//单个字符串的扫描所耗费的时间
		listPaths.RemoveAll();

		strPath = pThis->cScanPath + "\\*.*";
		do
		{
			bWorking = find.FindFile(strPath);
			while (bWorking && (pThis->m_FindingStatus == FINDING))
			{
				bWorking	= find.FindNextFile();
				strFilePath = find.GetFilePath();
				
				if (find.IsDots())
					continue;
				
				if (find.IsDirectory() && 
					(exPath1 != find.GetFilePath()) &&
					(exPath2 != find.GetFilePath()))
				{
					listPaths.AddHead(strFilePath);
					continue;
				}
				else if ((exPath1 != find.GetFilePath()) && (exPath2 != find.GetFilePath()))
				{
                    cFName = find.GetFileName();
                    if ((cFName == "str_plx.h") || (cFName == "str_plx_chn.h") || (cFName == "str_plx_eng.h"))
                        continue;

					filename = find.GetFilePath();			//获得文件名(包括路径)
					iExpend	 = filename.ReverseFind('.');	//获得扩展名左边的"."
					cExpend  = filename.Right(filename.GetLength() - iExpend - 1);//获得扩展名
					cExpend.MakeLower();
					if ((cExpend == "c") || (cExpend == "h") || (cExpend == "cpp"))
					{
						if (1 == pThis->iCheckFast)//快速扫描
						{
							if (1 == pThis->FindStrFast(filename,bufRead))
							{
								bFind = TRUE;
								break;//退出while循环
							}
						}
						else//一般扫描
						{
							if (1 == pThis->FindStr(filename,bufRead))
							{
								bFind = TRUE;
								break;//退出while循环
							}
						}
					}
				}//end if
			}//end while
			if (bFind)//在代码中找到了该字符串,退出 do while循环
				break;

			if(listPaths.IsEmpty())
				break;

			strFolder	= listPaths.RemoveHead();
			strPath		= strFolder + "\\*.*";
			find.Close();
		}while (pThis->m_FindingStatus == FINDING);//end do while

		if (bFind)		//在代码中找到了字符串
			continue;	//继续扫描下一个字符串
		else if (pThis->m_FindingStatus == FINDING) //在代码中没找到该字符串
			pThis->AddFile(bufRead,iFileCurLine);   //记录这个字符串

	}//end while
	cScanFile.Close();
	if(pThis->m_nTimer!=NULL)
	{
		pThis->KillTimer(pThis->m_nTimer);
		pThis->m_nTimer = NULL;
	}
	pThis->m_FindingStatus = STOP;//当前为停止状态
	
	CString cStrResult;
	cStrResult.Format("扫描%d个字符串,代码中共有%d处无用的字符串.",
		iStringNum,pThis->m_ListString.GetLineCount() - 2);

	pThis->m_Edit_ListFiles.SetWindowText(cStrResult);//显示扫描结果

    CString cSelectFold;
    cSelectFold.LoadString(IDS_STRING_SELECTFOLD);
	pThis->m_Button_SelectFold.SetWindowText(cSelectFold);//选择目录...
	pThis->m_Check_Fast.EnableWindow(TRUE);

	sndPlaySound(SOUNDFILENAME,SND_ASYNC);

	return 1;
}
//返回合法的字符串
CString FindString::GetValidString(CString cString,CString cStrPre)
{
	CString cStrResult = "";//合法的字符串

	cString.TrimLeft();
	cString.TrimRight();

	if (cString.Left(cStrPre.GetLength()) != cStrPre)//前缀相等的字符串才进行扫描
		return cStrResult;
	
	cStrResult = cString.Left(cString.Find(","));//获得逗号左边的字符串
	if (cStrResult.Find("=") != -1)
		cStrResult = cStrResult.Left(cStrResult.Find("="));//获得等号左边的字符串
	
	cStrResult.TrimRight();

	return cStrResult;
}
//在一般的一行代码(没有单引号,双引号)中查找字符串,
//返回1表示找到,返回0表示没找到,返回-1表示需要在以下的代码中继续扫描
int FindString::FindStrNoraml(CString bufRead,CString cStr)
{
	int	iNotePlace;			//获得单行注释的位置
	int	iNotesPlaceBegin;	//多行注释/*的位置
	int iNotesPlaceEnd;		//多行注释*/的位置

	iNotesPlaceBegin	= bufRead.Find("/*");
	iNotesPlaceEnd		= bufRead.Find("*/");
	iNotePlace			= bufRead.Find("//");
	//没有单行注释,没有多行注释
	if ((iNotePlace == -1) && (iNotesPlaceBegin == -1) && (iNotesPlaceEnd == -1))
	{
		return ((bufRead.Find(cStr) != -1) ? 1 : 0);
	}
	//没有单行注释,有多行注释
	if ((iNotePlace == -1) && ((iNotesPlaceBegin != -1) || (iNotesPlaceEnd != -1)))
	{
		return -1;
	}
	//有单行注释,没有多行注释
	if ((iNotePlace != -1) && (iNotesPlaceBegin == -1) && (iNotesPlaceEnd == -1))
	{
		bufRead = bufRead.Left(iNotePlace);//获得注释左边的代码
		return ((bufRead.Find(cStr) != -1) ? 1 : 0);
	}
	//有单行注释,有多行注释
	if ((iNotePlace != -1) && ((iNotesPlaceBegin != -1) || (iNotesPlaceEnd != -1)))
	{
		if ((iNotesPlaceBegin != -1) && (iNotesPlaceEnd == -1))//有"/*",没有"*/"
		{
			if (iNotesPlaceBegin < iNotePlace)//多行注释在单行注释的左边,则这实际上是多行注释
				return -1;
			if (iNotesPlaceBegin > iNotesPlaceBegin)//多行注释在单行注释的右边,则这实际上是单行注释
			{
				bufRead = bufRead.Left(iNotePlace);//获得注释左边的代码
				return ((bufRead.Find(cStr) != -1) ? 1 : 0);
			}
		}
		else if ((iNotesPlaceBegin == -1) && (iNotesPlaceEnd != -1))//有"*/",没有"/*"
		{
			//有可能是单行注释,也可能是多行注释的结束
			return -1;
		}
	}
	return -1;
}
//开始在文件中查找字符串,找到返回1,否则返回0
//filename:文件名
int FindString::FindStr(CString filename,CString cStr)
{
	CStdioFile	file;
	CString		bufRead,bufLine;	
	LPTSTR		lptStr,lptStrNext;
	BOOL		bNotesEnd;		//是否在"//"以后的一行代码中存在"*/"
	BOOL		bNote;			//是否注释开始("/*"多行注释)
	BOOL		bS,bD;			//是否有单引号,是否有双引号
	int			iNormal;

	if (file.Open(filename,CFile::modeRead,NULL))//打开成功
	{
		bNote = FALSE;
		m_Edit_ListFiles.SetWindowText("正在扫描" + cStr + " 扫描位置:" + filename);
		
		while (file.ReadString(bufRead))//开始一行一行扫描代码
		{
			bufRead.TrimLeft();
			bufRead.TrimRight();

			if (!bNote)//不在多行注释中
			{			
				if ((bufRead.Find("\'") == -1) && (bufRead.Find("\"") == -1))//没有单引号,没有双引号
				{	//在一般的一行代码(没有单引号,双引号)中查找字符串,
					iNormal = FindStrNoraml(bufRead,cStr);
					if (iNormal == 1)//在这一行中找到了字符串
					{
						file.Close();
						return 1;
					}
					else if (iNormal == 0)//在这一行中没找到字符串,
					{
						continue;
					}
				}
			}
			bS			= FALSE;		//没有单引号
			bD			= FALSE;		//没有双引号
			lptStr		= NULL;
			lptStrNext	= NULL;
			lptStr		= bufRead.GetBuffer(bufRead.GetLength());
			bufLine		= "";//获得一行没有注释的纯代码

			while (* lptStr != 0)//在一行代码中进行扫描
			{
				if (* lptStr == 34)//当前字符是双引号
					bD = !bD;
				else if (* lptStr == 39)//当前字符单引号
					bS = !bS;
				else
				{
					if (* lptStr == 47)//当前字符是"/"
					{
						lptStrNext = lptStr;
						lptStrNext++;
						if (* lptStrNext == 47)//下一个字符是"/"
						{
							if ((!bS) && (!bD))//在"//"之前没有单引号没有双引号表示这个"//"是注释,退出这一行的扫描
							{
								//存在这种情况,"//...*/"这时候这一行是多行注释的结束
								lptStr += 2;
								bNotesEnd = FALSE;
								while (* lptStr != 0)
								{
									if (* lptStr == 42)//当前字符是"*"
									{
										lptStrNext = lptStr;
										lptStrNext++;
										if ((* lptStrNext == 47) && (bNote))//下一个字符是"/"并且有多行注释
										{
											bNote = FALSE;//表示多行注释结束
											bNotesEnd = TRUE;
											lptStr += 2;
											break;
										}
									}
									lptStr++;
								}
								if (bNotesEnd)//是在"//"以后的一行代码中存在"*/"需要在这一行中继续扫描
									break;
							}
						}
						else if (* lptStrNext == 42)//下一个字符是"*"
						{
							if ((!bS) && (!bD))//在"/*"之前没有单引号没有双引号表示这个"/*"是注释
							{
								lptStr++;
								bNote = TRUE;//表示多行注释开始
								continue;
							}							
						}
					}
					else if (* lptStr == 42)//当前字符是"*"
					{
						lptStrNext = lptStr;
						lptStrNext++;
						if ((* lptStrNext == 47) && (bNote))//下一个字符是"/"并且有多行注释
						{
							bNote = FALSE;//表示多行注释结束
							lptStr += 2;
							continue;
						}
					}
				}//end if
				if (* lptStr != 0)
				{
					if ((unsigned char)(* lptStr) > 0x80)
					{
						if (!bNote)
						{
							bufLine += (CString)(* lptStr++);
							bufLine += (CString)(* lptStr++);
						}
						else
						{
							lptStr += 2;
						}
					}
					else
					{
						if (!bNote)
							bufLine += (CString)(* lptStr++);
						else
							lptStr++;
					}
				}
			}//end while(* lptStr != 0)//在一行代码中进行扫描

			if (bufLine.Find(cStr) != -1)//找到了这个字符串
			{
				file.Close();
				return 1;
			}
            bufRead.ReleaseBuffer();
		}//end while
		file.Close();
	}//end if

	return 0;
}

void FindString::AddFile(CString cStr,int iLineNum)//将字符串,行号添加到listctrl控件中
{
	CString cLineNum,cfile,cUsedTime;
	int iLength,i;
	long iTextLength;
	CString cResult;
	
	cLineNum.Format("%d",iLineNum);
	cUsedTime.Format("%d",lUsedSecond);

	lUsedSecond = 0;
	cResult		= cStr;
	iLength		= cStr.GetLength();
	
	for (i = iLength; i < iSTRMaxLen;i++)
	{
		cResult = cResult + " ";
	}
	if (cLineNum.GetLength() == 1)
		cResult = cResult + cLineNum + "\t\t\t\t\t" + cUsedTime + "\r\n";
	else
		cResult = cResult + cLineNum + "\t\t\t\t" + cUsedTime + "\r\n";

	iTextLength = m_ListString.GetWindowTextLength();
	m_ListString.SetSel(iTextLength, iTextLength);
	m_ListString.ReplaceSel(cResult);
}

void FindString::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString cTime;

	if (m_FindingStatus == STOP)
	{
		KillTimer(SCANING_TIMER);
		m_nTimer = NULL;
		return;
	}

	lUsedSecond++;//单个字符串的扫描所耗费的时间

	iUsedSecond++;
	if (iUsedSecond == 60)
	{
		iUsedMinute++;
		if (iUsedMinute == 60)
		{
			iUsedMinute = 0;
			iUsedHour++;
		}
		iUsedSecond = 0;
	}

	cTime.Format("已经扫描了%02d小时%02d分钟%02d秒",iUsedHour,iUsedMinute,iUsedSecond);
	m_UsedTimer.SetWindowText(cTime);
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL FindString::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD style = m_Button_SelectFold.GetButtonStyle();
    style = style | BS_DEFPUSHBUTTON;
    m_Button_SelectFold.SetButtonStyle(style,TRUE);

	return CPropertyPage::OnSetActive();
}

void FindString::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	if (bShow)
		m_Edit_Prefix.SetFocus();
}
//开始在文件中查找字符串,找到返回1,否则返回0
//filename:文件名
int FindString::FindStrFast(CString filename,CString cStr)
{
	CStdioFile	file;
	CString		bufRead;

	if (file.Open(filename,CFile::modeRead,NULL))//打开成功
	{		
		m_Edit_ListFiles.SetWindowText("正在扫描" + cStr + " 扫描位置:" + filename);
		
		while (file.ReadString(bufRead))//开始一行一行扫描代码
		{
			if (bufRead.Find(cStr) != -1)//找到了这个字符串
			{
				file.Close();
				return 1;
			}
		}//end while
		file.Close();
	}//end if

	return 0;
}

void FindString::OnCheckFast() 
{
	// TODO: Add your control notification handler code here
	if (m_Check_Fast.GetCheck() == 1)
	{
		CString clew;
		clew.LoadString(IDS_STRING_CHECKCLEW);
		if (MessageBox(clew,"提示",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
			m_Check_Fast.SetCheck(FALSE);
	}
}
BOOL FindString::bScaning(void)
{
	return (m_FindingStatus == FINDING);
}

BOOL FindString::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN)
        {
            OnButtonSelectfold();
        }
    }
	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL FindString::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == BUTTONEX_KEYDOWNCHAR)
    {
        if (lParam == 'P')//选择F键,焦点转移
        {
            m_Edit_Prefix.SetSel(0,-1);
			m_Edit_Prefix.SetFocus();
        }
        return TRUE;
    }
    if (wParam == LISTCTRLCHECK_CHECK)
        return TRUE;
	return CPropertyPage::OnCommand(wParam, lParam);
}
