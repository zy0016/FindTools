// FindChineseFile.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "FindChineseFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindChineseFile property page

IMPLEMENT_DYNCREATE(FindChineseFile, CPropertyPage)

FindChineseFile::FindChineseFile() : CPropertyPage(FindChineseFile::IDD)
{
	//{{AFX_DATA_INIT(FindChineseFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

FindChineseFile::~FindChineseFile()
{
}

void FindChineseFile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindChineseFile)
	DDX_Control(pDX, IDC_LIST_FINDCHINESEEXT_FILE, m_FindChinese_File);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindChineseFile, CPropertyPage)
	//{{AFX_MSG_MAP(FindChineseFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindChineseFile message handlers

BOOL FindChineseFile::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CStdioFile	file;
	CString		bufRead;
	LPTSTR		lptstr;
	int			iPre;

	if (file.Open(FINDCHINESEEXFILE,CFile::modeRead,NULL))//打开成功
	{		
		while (file.ReadString(bufRead))
		{
			iPre = bufRead.Find("=");
			if (-1 == iPre)
				break;

			if (bufRead.Left(iPre) != FILE_SIGN)
				continue;

			bufRead = bufRead.Right(bufRead.GetLength() - iPre - 1);

			while(bufRead.Right(1) == 13)//删除最后的回车符
				bufRead = bufRead.Left(bufRead.GetLength() - 1);

			lptstr = bufRead.GetBuffer(bufRead.GetLength());
			m_FindChinese_File.AddString(lptstr);
            bufRead.ReleaseBuffer();
		}
		file.Close();
	}
	m_FindChinese_File.SendMessage(LB_SETHORIZONTALEXTENT,800,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL FindChineseFile::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString InvalidFileName = m_FindChinese_File.GetInvalidFileName();
	if (InvalidFileName != "")
	{
		return FALSE;
	}
	return CPropertyPage::OnKillActive();
}

BOOL FindChineseFile::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd* pcwnd;
    pcwnd = this->GetParentOwner();

    if (pcwnd->GetSafeHwnd() != NULL)
	    pcwnd->PostMessage(WM_COMMAND,SETDEFBUTTON,SETDEFBUTTON);//通知父窗口选项卡激活了
    
	return CPropertyPage::OnSetActive();
}
