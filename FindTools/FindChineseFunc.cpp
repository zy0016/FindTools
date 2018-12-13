// FindChineseFunc.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "FindChineseFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindChineseFunc property page

IMPLEMENT_DYNCREATE(FindChineseFunc, CPropertyPage)

FindChineseFunc::FindChineseFunc() : CPropertyPage(FindChineseFunc::IDD)
{
	//{{AFX_DATA_INIT(FindChineseFunc)
	//}}AFX_DATA_INIT
}

FindChineseFunc::~FindChineseFunc()
{
}

void FindChineseFunc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindChineseFunc)
	DDX_Control(pDX, IDC_BUTTON_FINDCHINESEFUNC_DEL, m_Function_Del);
	DDX_Control(pDX, IDC_BUTTON_FINDCHINESEFUNC_ADD, m_FunctionAdd);
	DDX_Control(pDX, IDC_EDIT_FINDCHINESEFUNC, m_Function);
	DDX_Control(pDX, IDC_LIST_FINDCHINESEFUNC, m_FunctionList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindChineseFunc, CPropertyPage)
	//{{AFX_MSG_MAP(FindChineseFunc)
	ON_BN_CLICKED(IDC_BUTTON_FINDCHINESEFUNC_ADD, OnButtonFindchinesefuncAdd)
	ON_BN_CLICKED(IDC_BUTTON_FINDCHINESEFUNC_DEL, OnButtonFindchinesefuncDel)
	ON_EN_CHANGE(IDC_EDIT_FINDCHINESEFUNC, OnChangeEditFindchinesefunc)
	ON_LBN_SETFOCUS(IDC_LIST_FINDCHINESEFUNC, OnSetfocusListFindchinesefunc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindChineseFunc message handlers

BOOL FindChineseFunc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	//��ȡ�����б�
	CStdioFile	file;
	CString		bufRead;
	LPTSTR		lptstr;
	int			iPre;

	if (file.Open(FINDCHINESEEXFILE,CFile::modeRead,NULL))//�򿪳ɹ�
	{		
		while (file.ReadString(bufRead))
		{
			iPre = bufRead.Find("=");
			if (-1 == iPre)
				break;

			if (bufRead.Left(iPre) != FUNCTION_SIGN)
				continue;

			bufRead = bufRead.Right(bufRead.GetLength() - iPre - 1);

			while(bufRead.Right(1) == 13)//ɾ�����Ļس���
				bufRead = bufRead.Left(bufRead.GetLength() - 1);

			lptstr = bufRead.GetBuffer(bufRead.GetLength());
			m_FunctionList.AddString(lptstr);
            bufRead.ReleaseBuffer();
		}
		file.Close();
	}
	//m_FunctionList.SendMessage(LB_SETHORIZONTALEXTENT,300,0);

    m_FunctionAdd.EnableWindow(FALSE);
    m_Function_Del.EnableWindow(FALSE);

	m_FunctionAdd.iCheckId = IDC_BUTTON_FINDCHINESEFUNC_ADD;
	m_FunctionAdd.iClickId = IDC_BUTTON_FINDCHINESEFUNC_ADD;

	m_Function_Del.iCheckId = IDC_BUTTON_FINDCHINESEFUNC_DEL;
	m_Function_Del.iClickId = IDC_BUTTON_FINDCHINESEFUNC_DEL;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FindChineseFunc::OnButtonFindchinesefuncAdd() 
{
	// TODO: Add your control notification handler code here
	CString cSFunctin;
	int i,iListCount;
	CString cSList;

	m_Function.GetWindowText(cSFunctin);
	cSFunctin.TrimLeft();
	cSFunctin.TrimRight();

	if (cSFunctin == "")
	{
		MessageBox("�����뺯����.","��ʾ",MB_ICONINFORMATION|MB_OK);
		m_Function.SetFocus();
		return;
	}
	iListCount = m_FunctionList.GetCount();
	for (i = 0;i < iListCount;i++)
	{
		m_FunctionList.GetText(i,cSList);
		if (cSList == cSFunctin)
		{
			MessageBox("��������Ѿ���ӹ���.","��ʾ",MB_ICONINFORMATION|MB_OK);
			m_Function.SetSel(0,-1);
			m_Function.SetFocus();
			return;
		}
	}
	m_FunctionList.AddString(cSFunctin.GetBuffer(cSFunctin.GetLength()));
    cSFunctin.ReleaseBuffer();
	m_Function.SetWindowText("");
	m_Function.SetFocus();
}

void FindChineseFunc::OnButtonFindchinesefuncDel() 
{
	// TODO: Add your control notification handler code here
	CString cSFunctin;
	int i,iListCount;

	iListCount = m_FunctionList.GetCount();
	for (i = 0;i < iListCount;i++)
	{
		if (m_FunctionList.GetSel(i))
		{
			m_FunctionList.GetText(i,cSFunctin);
			if (MessageBox("���б�����ɾ��������" + cSFunctin + "��?", "��ʾ", MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
				m_FunctionList.DeleteString(i);
				m_FunctionList.SetCurSel((i == iListCount - 1) ? i - 1 : i);
				break;
			}
		}
	}
    OnSetfocusListFindchinesefunc();
}

void FindChineseFunc::OnChangeEditFindchinesefunc() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString cFunction;

    m_Function.GetWindowText(cFunction);
    if (cFunction != "")
        m_FunctionAdd.EnableWindow(TRUE);
    else
        m_FunctionAdd.EnableWindow(FALSE);
}

void FindChineseFunc::OnSetfocusListFindchinesefunc() 
{
	// TODO: Add your control notification handler code here
	if (0 == m_FunctionList.GetCount())
        m_Function_Del.EnableWindow(FALSE);
    else
        m_Function_Del.EnableWindow(TRUE);
}

BOOL FindChineseFunc::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd* pcwnd;
    pcwnd = this->GetParentOwner();

    if (pcwnd->GetSafeHwnd() != NULL)
	    pcwnd->PostMessage(WM_COMMAND,SETDEFBUTTON,SETDEFBUTTON);//֪ͨ������ѡ�������

	return CPropertyPage::OnSetActive();
}

BOOL FindChineseFunc::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * pFocus;
    pFocus = GetFocus();

    if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN) && 
        (pFocus->m_hWnd == m_Function.GetSafeHwnd()))//��ǰ�������ı��������յ��س���Ϣ
    {
        OnButtonFindchinesefuncAdd();
        return TRUE;
    }
	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL FindChineseFunc::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == BUTTONEX_KEYDOWNCHAR)
    {
        if (lParam == 'F')//ѡ��F��,����ת��
        {
            m_Function.SetSel(0,-1);
			m_Function.SetFocus();
        }
        return TRUE;
    }
	return CPropertyPage::OnCommand(wParam, lParam);
}
