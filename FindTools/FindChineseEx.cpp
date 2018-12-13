// FindChineseEx.cpp : implementation file
//

#include "stdafx.h"
#include "FindTools.h"
#include "FindChineseEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindChineseEx dialog


FindChineseEx::FindChineseEx(CWnd* pParent /*=NULL*/)
	: CDialog(FindChineseEx::IDD, pParent)
{
	//{{AFX_DATA_INIT(FindChineseEx)
	//}}AFX_DATA_INIT
	FileList.RemoveAll();
	FoldList.RemoveAll();
}
FindChineseEx::~FindChineseEx()
{
	FileList.RemoveAll();
	FoldList.RemoveAll();
}

void FindChineseEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindChineseEx)
	DDX_Control(pDX, IDOK, m_OK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindChineseEx, CDialog)
	//{{AFX_MSG_MAP(FindChineseEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindChineseEx message handlers

void FindChineseEx::OnOK() 
{
	// TODO: Add extra validation here
	CStdioFile	file;
	int			i,iListCount;
	CString		cPre;
	CString		cFunction,cFile,cFold,cSubFold;

	if (FCFile.m_FindChinese_File.GetSafeHwnd() != NULL)
	{
		if (FCFile.m_FindChinese_File.GetInvalidFileName() != "")
			return;
	}
	if (FCFold.m_ListCtrlExt_Fold.GetSafeHwnd() != NULL)
	{
		if (FCFold.m_ListCtrlExt_Fold.GetInvalidFoldName() != "")
			return;
	}

	if (file.Open(FINDCHINESEEXFILE,CFile::modeCreate | CFile::modeWrite | CFile::typeText,NULL))
	{
		//保存例外的函数
		iListCount	= FCF.m_FunctionList.GetCount();
		cPre		= FUNCTION_SIGN;
		for (i = 0;i < iListCount;i++)
		{
			FCF.m_FunctionList.GetText(i,cFunction);
			cFunction = cPre + "=" + cFunction + "\r\n";
			file.WriteString(cFunction);
		}
		//保存例外的文件
		if (FCFile.m_FindChinese_File.GetSafeHwnd() != NULL)
		{
			iListCount	= FCFile.m_FindChinese_File.GetCount();
			cPre		= FILE_SIGN;
			for (i = 0;i < iListCount - 1;i++)
			{
				FCFile.m_FindChinese_File.GetText(i,cFile);
				cFile = cPre + "=" + cFile + "\r\n";
				file.WriteString(cFile);
			}
		}
		else
		{
			iListCount = FileList.GetCount();
			POSITION ps;
			ps = FileList.GetHeadPosition();
			for (i = 0;i < iListCount;i++)
			{
				cFile = FileList.GetNext(ps) + "\r\n";//获得链表中的例外项
				file.WriteString(cFile);
			}
		}
		//保存例外的目录
		if (FCFold.m_ListCtrlExt_Fold.GetSafeHwnd() != NULL)
		{
			iListCount	= FCFold.m_ListCtrlExt_Fold.GetItemCount();
			cPre		= FOLD_SIGN;
			for (i = 0;i < iListCount - 1;i++)
			{
				cFold	 = FCFold.m_ListCtrlExt_Fold.GetItemText(i,0);
				cSubFold = FCFold.m_ListCtrlExt_Fold.GetItemText(i,1);
				if (cSubFold == FOLD_CONTAIN)
					cFold = cPre + "=" + cFold + FOLD_SING_FOLD + "1" + "\r\n";
				else
					cFold = cPre + "=" + cFold + FOLD_SING_FOLD + "0" + "\r\n";

				file.WriteString(cFold);
			}
		}
		else
		{
			iListCount = FoldList.GetCount();
			POSITION ps;
			ps = FoldList.GetHeadPosition();
			for (i = 0;i < iListCount;i++)
			{
				cFile = FoldList.GetNext(ps) + "\r\n";//获得链表中的例外项
				file.WriteString(cFile);
			}
		}
		file.Close();
	}
	else
	{
		MessageBox("无法保存文件.","提示",MB_ICONSTOP|MB_OK);
	}

	CDialog::OnOK();
}

BOOL FindChineseEx::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	FindChineseSheet.AddPage(&FCF);
	FindChineseSheet.AddPage(&FCFile);
	FindChineseSheet.AddPage(&FCFold);

	//用Create来创建一个属性页
	FindChineseSheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);

	//调整属性页的大小和位置
	FindChineseSheet.SetWindowPos(NULL, 0,0,0,0 , SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	///////////////////////////////////////////////////////////////
	FileList.RemoveAll();
	FoldList.RemoveAll();

	CStdioFile	file;
	CString		bufRead;
	int			iPre;

	if (file.Open(FINDCHINESEEXFILE,CFile::modeRead,NULL))//打开成功
	{
		while (file.ReadString(bufRead))
		{
			iPre = bufRead.Find("=");
			if (-1 == iPre)
				break;

			while(bufRead.Right(1) == 13)//删除最后的回车符
				bufRead = bufRead.Left(bufRead.GetLength() - 1);

			if (bufRead.Left(iPre) == FILE_SIGN)
			{
				FileList.AddHead(bufRead);
			}
			else if (bufRead.Left(iPre) == FOLD_SIGN)
			{
				FoldList.AddHead(bufRead);
			}
		}
		file.Close();
	}
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL FindChineseEx::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((wParam == SETDEFBUTTON) && (lParam == SETDEFBUTTON))
    {
        DWORD style = m_OK.GetButtonStyle();
        style = style | BS_DEFPUSHBUTTON;
        m_OK.SetButtonStyle(style,TRUE);
        return TRUE;
    }
	return CDialog::OnCommand(wParam, lParam);
}
