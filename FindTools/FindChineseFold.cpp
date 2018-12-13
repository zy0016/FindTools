// FindChineseFold.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "FindChineseFold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindChineseFold property page

IMPLEMENT_DYNCREATE(FindChineseFold, CPropertyPage)

FindChineseFold::FindChineseFold() : CPropertyPage(FindChineseFold::IDD)
{
	//{{AFX_DATA_INIT(FindChineseFold)
	//}}AFX_DATA_INIT
}

FindChineseFold::~FindChineseFold()
{
}

void FindChineseFold::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindChineseFold)
	DDX_Control(pDX, IDC_LIST_FINDCHINESE_FOLD, m_ListCtrlExt_Fold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindChineseFold, CPropertyPage)
	//{{AFX_MSG_MAP(FindChineseFold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindChineseFold message handlers

BOOL FindChineseFold::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CStdioFile	file;
	CString		bufRead;
	int			iPre;
	int			iCount;
	CString		cFold;//����Ƿ������Ŀ¼����Ϣ
	BOOL		bFold;

	if (file.Open(FINDCHINESEEXFILE,CFile::modeRead,NULL))//�򿪳ɹ�
	{		
		while (file.ReadString(bufRead))
		{
			iPre = bufRead.Find("=");
			if (-1 == iPre)
				break;

			if (bufRead.Left(iPre) != FOLD_SIGN)
				continue;

			bufRead = bufRead.Right(bufRead.GetLength() - iPre - 1);

			while(bufRead.Right(1) == 13)//ɾ�����Ļس���
				bufRead = bufRead.Left(bufRead.GetLength() - 1);

			bFold = FALSE;
			cFold = bufRead.Right(2);//��������λ
			if (cFold.Left(1) == FOLD_SING_FOLD)
			{
				bFold = (cFold.Right(1) == "1");//������Ŀ¼
				cFold = bufRead.Left(bufRead.GetLength() - 2);
			}
			else
			{
				cFold = bufRead.Left(bufRead.GetLength());
			}
			iCount = m_ListCtrlExt_Fold.GetItemCount();
			m_ListCtrlExt_Fold.InsertItem(iCount,cFold);
			m_ListCtrlExt_Fold.SetItemText(iCount,1,bFold ? FOLD_CONTAIN : FOLD_NOTCONTAIN);
		}
		file.Close();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL FindChineseFold::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString InvalidFoldName = m_ListCtrlExt_Fold.GetInvalidFoldName();
	if (InvalidFoldName != "")
	{
		return FALSE;
	}
	return CPropertyPage::OnKillActive();
}

BOOL FindChineseFold::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd* pcwnd;
    pcwnd = this->GetParentOwner();

    if (pcwnd->GetSafeHwnd() != NULL)
	    pcwnd->PostMessage(WM_COMMAND,SETDEFBUTTON,SETDEFBUTTON);//֪ͨ������ѡ�������
    
	return CPropertyPage::OnSetActive();
}
