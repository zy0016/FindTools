// ListCtrlExt.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "ListCtrlExt.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlExt

CListCtrlExt::CListCtrlExt()
{
	cCurSelFoldName = "";
	cClewString		= "˫������ѡ����������ļ���";
	iCurSelItem		= 0;//��ǰ���ڲ�������Ŀ
	bCurCheck		= FALSE;//�Ƿ�ѡ����Ŀ¼
	bChange			= FALSE;//�ı����е������Ƿ��Ѿ��ı�

	bValidFold		= FALSE;//�Ƿ�Ŀ¼�ĶԻ����Ƿ��Ѿ���ʾ����
	InvalidFold		= "";//Ϊ�յ�ʱ���ʾû�зǷ�Ŀ¼,�����ʾ�зǷ�Ŀ¼,����Ϊ��ʾ��Ϣ

    iCurrentSel     = -1;//��ǰѡ�����
	bEsc			= FALSE;//�Ƿ�����ESC��
	bEditing        = FALSE;//��ǰ���Ǳ༭״̬

	csToolTip		= "˫��ĳһ��,ɾ���ı����е�����,�ٵ����б��пհ׵ĵط��Ϳ���ɾ����һ��.";
}

CListCtrlExt::~CListCtrlExt()
{
}


BEGIN_MESSAGE_MAP(CListCtrlExt, CListCtrl)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CListCtrlExt)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_SETFOCUS, OnSetfocus)
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlExt message handlers

void CListCtrlExt::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD oldStyle = GetExtendedStyle();
	SetExtendedStyle(oldStyle |LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES);

	CRect rListCtrl;

	GetClientRect(&rListCtrl);

	rEdit.top			= rListCtrl.top + HEADERHEIGHT;
	rEdit.left			= rListCtrl.left;
	rEdit.right			= rListCtrl.right - CBUTTONW - CHECKWIDTH;
	rEdit.bottom		= rEdit.top + CITEMHEIGHT;

	rButton_Fold.top	= rEdit.top;
	rButton_Fold.left	= rEdit.right;
	rButton_Fold.right	= rButton_Fold.left + CBUTTONW;
	rButton_Fold.bottom	= rEdit.bottom;

	rButtonCheck.top	= rEdit.top;
	rButtonCheck.left	= rButton_Fold.right;
	rButtonCheck.right	= rButtonCheck.left + CHECKWIDTH;
	rButtonCheck.bottom	= rEdit.bottom;

	CEdit_Fold.Create(ES_LEFT|WS_BORDER|WS_VISIBLE|ES_AUTOHSCROLL,rEdit,this,IDC_CEDIT_FOLD);
	CButton_Fold.Create("��",WS_BORDER|WS_VISIBLE|BS_PUSHBUTTON,
		rButton_Fold,this,IDC_CBUTTON_FOLD);

	CButton_Check.Create("������Ŀ¼",
		ES_LEFT|WS_VISIBLE|BS_AUTOCHECKBOX|BS_CHECKBOX,
		rButtonCheck,this,IDC_CBUTTON_CHECK);

	InsertColumn(0,"Ŀ¼�б�",LVCFMT_LEFT,230,0);
	InsertColumn(1,"�Ƿ������Ŀ¼",LVCFMT_LEFT,100,0);
	
	CEdit_Fold.ShowWindow(SW_HIDE);
	CButton_Fold.ShowWindow(SW_HIDE);
	CButton_Check.ShowWindow(SW_HIDE);

	PostMessage(WM_COMMAND,LISTCTRL_INIT_WPARAM,LISTCTRL_INIT_LPARAM);
	CListCtrl::PreSubclassWindow();

	CRect rect; 
    GetClientRect(rect);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this,csToolTip , rect, TOOLTIP_ID);//"ToolTip"
}

BOOL CListCtrlExt::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
    if ((wParam == LISTCTRL_INIT_WPARAM) && (lParam == LISTCTRL_INIT_LPARAM))
	{
		int iListCount = GetItemCount();
		InsertItem(iListCount,cClewString);
		return TRUE;
	}
	if (wParam == EDITEXT)
	{
		bEsc = FALSE;
        switch (lParam)
        {
        case EDITCHANGED://�ı����ݸı�
            CEdit_Fold.GetWindowText(cCurSelFoldName);//�ı���������Ѿ��ı�
		    bChange = TRUE;
            return TRUE;
            
        case EDITRETURN://�յ��س���
            CEdit_Fold.ShowWindow(SW_HIDE);
            return TRUE;

        case EDITESCAPE://�յ�ESC��
			bEsc = TRUE;

			POSITION pos = GetFirstSelectedItemPosition();
			int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
			if (nSelectedItem == GetItemCount() - 1)
			{
				cCurSelFoldName = "";
			}
			CEdit_Fold.SetWindowText("");

            CEdit_Fold.ShowWindow(SW_HIDE);
            CButton_Fold.ShowWindow(SW_HIDE);
            CButton_Check.ShowWindow(SW_HIDE);
			bEditing = FALSE;//��ǰ���Ǳ༭״̬
            return TRUE;
        }
	}
	if (wParam == LISTCTRLCHECK_CHECK)
	{
		POSITION pos = GetFirstSelectedItemPosition();
		int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ

		bCurCheck = lParam;
		SetItemText(iCurSelItem, 1,bCurCheck ? FOLD_CONTAIN : FOLD_NOTCONTAIN);
		return TRUE;
	}
	switch (wParam)
	{
	case IDC_CBUTTON_FOLD:
		// TODO: Add your control notification handler code here
		BROWSEINFO bi; 
		ITEMIDLIST * pidl; 
		char Dir[MAX_PATH] = "";
		CString csFoldInit;//��ó�ʼĿ¼�Ա�ѡ��
		CString strLoadString = "ѡ��Ŀ¼������[ȷ��]��ť";

		CEdit_Fold.GetWindowText(csFoldInit);//��ó�ʼĿ¼�Ա�ѡ��

		bi.hwndOwner		= this->GetSafeHwnd();
		bi.pidlRoot			= NULL;
		bi.pszDisplayName	= Dir; 
		bi.lpszTitle		= strLoadString; //ѡ��Ŀ¼
		bi.ulFlags			= BIF_RETURNONLYFSDIRS ; 
		bi.lpfn				= SetSelProc;
		bi.lParam			= (LPARAM)(LPCTSTR)csFoldInit;
		bi.iImage			= 0; 
		
		pidl = SHBrowseForFolder(&bi);

		if(pidl == NULL) 
			memset(Dir,0,sizeof(MAX_PATH));
		if(SHGetPathFromIDList(pidl,Dir))/*Retrieve folder name from ITEMLIST structure.*/ 
		{
			CEdit_Fold.SetWindowText(Dir);
		}
		break;
	}
	return CListCtrl::OnCommand(wParam, lParam);
}

void CListCtrlExt::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ

	if (nSelectedItem == -1)
	{
		if (!IfValidFoldName())
			return;
	}
	iCurSelItem = nSelectedItem;
    iCurrentSel = nSelectedItem;

	CButton_Fold.ShowWindow(SW_SHOW);
	CButton_Check.ShowWindow(SW_SHOW);
	CEdit_Fold.ShowWindow(SW_SHOW);

	RECT rect;
	POINT point;
	int iItemHeight;

	GetItemRect(nSelectedItem,&rect,LVIR_LABEL);
	GetItemPosition(nSelectedItem,&point);

	iItemHeight			= rect.top;

	rEdit.top			= iItemHeight;
	rEdit.right			= rect.right - CBUTTONW;
	rEdit.bottom		= rEdit.top + CITEMHEIGHT;

	rButton_Fold.top	= rEdit.top;
	rButton_Fold.left	= rEdit.right;
	rButton_Fold.right	= rButton_Fold.left + CBUTTONW;
	rButton_Fold.bottom	= rEdit.bottom;

	rButtonCheck.top	= rEdit.top;
	rButtonCheck.left	= rButton_Fold.right;
	rButtonCheck.right	= rButtonCheck.left + CHECKWIDTH;
	rButtonCheck.bottom	= rEdit.bottom;

	CEdit_Fold.MoveWindow(&rEdit,TRUE);
	CButton_Fold.MoveWindow(&rButton_Fold,TRUE);//���·��ÿؼ���λ��
	CButton_Check.MoveWindow(&rButtonCheck,TRUE);

	if (nSelectedItem == GetItemCount() - 1)//˫�����һ��
	{
		CEdit_Fold.SetWindowText("");
	}
	else
	{
		CString cFile;
		CString cCheck;
		cFile  = GetItemText(nSelectedItem,0);//����б���е�ѡ�����Ŀ
		cCheck = GetItemText(nSelectedItem,1);//����б���е�ѡ�����Ŀ
		if (cCheck == FOLD_CONTAIN)
		{
			CButton_Check.SetCheck(1);
			bCurCheck = TRUE;
		}
		else
		{
			CButton_Check.SetCheck(0);
			bCurCheck = FALSE;
		}
		CEdit_Fold.SetWindowText(cFile);
		CEdit_Fold.SetSel(0,-1);
	}
	CEdit_Fold.SetFocus();
	bEditing = TRUE;//��ǰ�Ǳ༭״̬

	*pResult = 0;
}

void CListCtrlExt::OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetData();
	*pResult = 0;
}

void CListCtrlExt::SetData(void)
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ

	if (-1 == nSelectedItem)
		return;

	if (bEsc)//�յ�ESC��
		return;

	if (!bChange)//�ı����е�����û���Ѿ��ı�
		return;

	if (bValidFold)//�Ƿ�Ŀ¼�ĶԻ����Ѿ���ʾ����
		return;

	bValidFold = FALSE;
	if (!IfValidFoldName())
		return;

	CEdit_Fold.ShowWindow(SW_HIDE);
	CButton_Fold.ShowWindow(SW_HIDE);
	CButton_Check.ShowWindow(SW_HIDE);

	if (bEditing)
	{
		if (iCurSelItem != nSelectedItem)
		{
			nSelectedItem = iCurSelItem;
		}
		if (cCurSelFoldName != "")//�޸Ļ�����������Ŀ
		{
			DeleteItem(nSelectedItem);//ɾ����ǰ��Ŀ,���²���
			InsertItem(nSelectedItem,cCurSelFoldName);
			SetItemText(nSelectedItem,1,bCurCheck ? FOLD_CONTAIN : FOLD_NOTCONTAIN);
			cCurSelFoldName = "";
			
			CString cLastItem;
			cLastItem = GetItemText(GetItemCount() - 1,0);//����б�������һ��
			if (cLastItem != cClewString)//�޸ĵ������һ��
			{
				InsertItem(GetItemCount(),cClewString);
			}
		}
		else//��ʾɾ��
		{
			if (nSelectedItem == GetItemCount() - 1)
			{
				SetItemText(nSelectedItem,1,"");
				return;
			}
			DeleteItem(nSelectedItem);
		}
		bChange = FALSE;
		bEditing = FALSE;//��ǰ���Ǳ༭״̬
	}
}

int CALLBACK CListCtrlExt::SetSelProc(HWND hWnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
    ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);

  return 0;
}

BOOL CListCtrlExt::IfValidFoldName(void)
{
	if ((cCurSelFoldName != "") && (cCurSelFoldName != cClewString))
	{
        char cCurDirBak[256] = "";
        GetCurrentDirectory(sizeof(cCurDirBak),cCurDirBak);
		
        if (!SetCurrentDirectory(cCurSelFoldName))
		{
			if (!bValidFold)
			{
                CEdit_Fold.ShowWindow(SW_SHOW);
                CButton_Fold.ShowWindow(SW_SHOW);
                CButton_Check.ShowWindow(SW_SHOW);

				bValidFold  = TRUE;
				InvalidFold = "�޷��ҵ�Ŀ¼" + cCurSelFoldName;

				MessageBox("�޷��ҵ�Ŀ¼" + cCurSelFoldName,"��ʾ",MB_ICONASTERISK |MB_OK);
				bValidFold = FALSE;
				CEdit_Fold.SetFocus();
                CEdit_Fold.SetSel(0,-1);
			}
            SetCurrentDirectory(cCurDirBak);
			return FALSE;
		}
        else
        {
            int i,iListCount = GetItemCount();
            CString cSList,cFold;

            CEdit_Fold.GetWindowText(cFold);
            for (i = 0;i < iListCount;i++)
            {
                cSList = GetItemText(i,0);
                if ((cSList == cFold) && (i != iCurrentSel) && (iCurrentSel != -1))
                {
                    CEdit_Fold.ShowWindow(SW_SHOW);
                    CButton_Fold.ShowWindow(SW_SHOW);
                    CButton_Check.ShowWindow(SW_SHOW);

                    bValidFold  = TRUE;
                    InvalidFold = "���Ŀ¼�Ѿ���ӹ���";
                    MessageBox("���Ŀ¼�Ѿ���ӹ���","��ʾ",MB_ICONASTERISK |MB_OK);
                    bValidFold = FALSE;
                    CEdit_Fold.SetFocus();
                    CEdit_Fold.SetSel(0,-1);
                    SetCurrentDirectory(cCurDirBak);
			        return FALSE;
                }
            }
        }
        SetCurrentDirectory(cCurDirBak);
	}
	InvalidFold = "";
	return TRUE;
}

CString CListCtrlExt::GetInvalidFoldName(void)
{
	IfValidFoldName();
	return (InvalidFold);
}

void CListCtrlExt::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
//	SetData();
	*pResult = 0;
}

BOOL CListCtrlExt::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);
	return CListCtrl::PreTranslateMessage(pMsg);
}
