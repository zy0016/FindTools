    // ListBoxExt.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "ListBoxExt.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ListBoxExt

ListBoxExt::ListBoxExt()
{
	cCurSelFileName = "";//�����ı����е�����
	cClewString		= "˫������ѡ����������ļ���";
	bChange			= FALSE;//�ı����е������Ƿ��Ѿ��ı�

	bValidFile		= FALSE;//�Ƿ��ļ��ĶԻ����Ƿ��Ѿ���ʾ����
	InvalidFile		= "";//Ϊ�յ�ʱ���ʾû�зǷ��ļ�,�����ʾ�зǷ��ļ�,����Ϊ��ʾ��Ϣ
    iCurrentSel     = 0;
	bEsc			= FALSE;//�Ƿ�����ESC��
    bEditing        = FALSE;//��ǰ���Ǳ༭״̬

	csToolTip		= "˫��ĳһ��,ɾ���ı����е�����,�ٵ����б��пհ׵ĵط��Ϳ���ɾ����һ��.";
}

ListBoxExt::~ListBoxExt()
{
}


BEGIN_MESSAGE_MAP(ListBoxExt, CListBox)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(ListBoxExt)
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnDblclk)
	ON_CONTROL_REFLECT(LBN_SETFOCUS, OnSetfocus)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ListBoxExt message handlers

void ListBoxExt::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect rListBox;

	GetClientRect(&rListBox);

	rEdit.top		= rListBox.top;
	rEdit.left		= rListBox.left;
	rEdit.right		= rListBox.right - CBUTTONW;
	rEdit.bottom	= CITEMHEIGHT;//GetItemHeight(0);

	rButton.top		= rEdit.top;
	rButton.left	= rEdit.right;
	rButton.right	= rListBox.right;
	rButton.bottom	= rEdit.bottom;

	CEdit_File.Create(ES_LEFT|WS_BORDER|WS_VISIBLE|ES_AUTOHSCROLL,rEdit,this,IDC_CEDIT_FILE);
	CButton_File.Create("��",WS_BORDER|WS_VISIBLE|BS_PUSHBUTTON,rButton,this,IDC_CBUTTON_FILE);

	CButton_File.ShowWindow(SW_HIDE);
	CEdit_File.ShowWindow(SW_HIDE);
	
	PostMessage(WM_COMMAND,BOX_INIT_WPARAM,BOX_INIT_LPARAM);

	CListBox::PreSubclassWindow();
	///////////////////////////////////
	CRect rect; 
    GetClientRect(rect);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this,csToolTip , rect, TOOLTIP_ID);//"ToolTip"
}

BOOL ListBoxExt::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
    if ((IDC_MENU_ADDFILES == wParam) && (0 == lParam))//��Ӷ���ļ�
    {
        AddFiles();
        return TRUE;
    }
    if ((IDC_MENU_DELETEFILE == wParam) && (0 == lParam))//ɾ����ǰ�ļ�
    {
        ResetContent();
        AddString(cClewString);
		SetCurSel(0);
        return TRUE;
    }
	if ((wParam == BOX_INIT_WPARAM) && (lParam == BOX_INIT_LPARAM))
	{
		AddString(cClewString);
		SetCurSel(0);
		return TRUE;
	}
    if (wParam == EDITEXT)
	{
		bEsc = FALSE;
        switch (lParam)
        {
        case EDITCHANGED://�ı����ݸı�
            CEdit_File.GetWindowText(cCurSelFileName);//�ı���������Ѿ��ı�
            bChange = TRUE;
            //bEditing = TRUE;//��ǰ�Ǳ༭״̬
            return TRUE;

        case EDITRETURN://�յ��س���
            CEdit_File.ShowWindow(SW_HIDE);
            return TRUE;
            
        case EDITESCAPE://�յ�ESC��
			bEsc = TRUE;
			if (GetCurSel() == GetCount() - 1)
			{
				cCurSelFileName = "";
			}
			CEdit_File.SetWindowText("");

			CButton_File.ShowWindow(SW_HIDE);
            CEdit_File.ShowWindow(SW_HIDE);
            bEditing = FALSE;//��ǰ���Ǳ༭״̬
            return TRUE;
        }
	}	
	switch (wParam)
	{
	case IDC_CBUTTON_FILE:
#if 0
		OPENFILENAME ofn;  // older version
		char	filetype[] = "VC�ļ�(*.c;*.cpp;*.h)\0*.c;*.cpp;*.h\0";
        char    cCurDirBak[256] = "";
        CString FileName = "";
        CString cCurDir;
        GetCurrentDirectory(sizeof(cCurDirBak),cCurDirBak);

        CEdit_File.GetWindowText(FileName);
        if (FileName == "")
        {
            cCurDir = "";
        }
        else
        {
            int iReverseFind = FileName.ReverseFind('\\') ;
            cCurDir = (iReverseFind == -1) ? "" : FileName.Left(iReverseFind);
        }

        SetCurrentDirectory(cCurDir);

		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrTitle = "ѡ���ļ�";
		ofn.lpstrInitialDir = cCurDir;
		ofn.lpstrFilter = filetype;
		ofn.Flags = OFN_HIDEREADONLY;//OFN_ALLOWMULTISELECT|OFN_EXPLORER ;
        ofn.hwndOwner = this->GetSafeHwnd();

		int nResult = ::GetOpenFileName(&ofn);
        if (1 == nResult)
        {
            
        }
        SetCurrentDirectory(cCurDirBak);
#else
		char filetype[] = "VC�ļ�(*.c;*.cpp;*.h)|*.c;*.cpp;*.h|";
        char cCurDirBak[256] = "";
        CString FileName = "";
        CString cCurDir;
        GetCurrentDirectory(sizeof(cCurDirBak),cCurDirBak);

        CEdit_File.GetWindowText(FileName);
        if (FileName == "")
        {
            cCurDir = "";
        }
        else
        {
            int iReverseFind = FileName.ReverseFind('\\') ;
            if (iReverseFind == -1)
                cCurDir = "";
            else
                cCurDir = FileName.Left(iReverseFind);
        }

        SetCurrentDirectory(cCurDir);
		
		CFileDialog open(TRUE,NULL,NULL,OFN_EXTENSIONDIFFERENT|OFN_HIDEREADONLY,filetype);
		
		open.m_ofn.lpstrTitle = "ѡ���ļ�";
		open.m_ofn.lpstrInitialDir = cCurDir;
		open.m_ofn.lpstrDefExt = "*.c;*.cpp";
		if (open.DoModal() == IDOK)
		{
			CString cFileName;
			cFileName = open.GetPathName();
			CEdit_File.SetWindowText(cFileName);
		}
        SetCurrentDirectory(cCurDirBak);
#endif
		break;
	}
	return CListBox::OnCommand(wParam, lParam);
}

void ListBoxExt::OnDblclk() 
{
	// TODO: Add your control notification handler code here
	int isel;
	isel = GetCurSel();

	CButton_File.ShowWindow(SW_SHOW);
	CEdit_File.ShowWindow(SW_SHOW);

	int iItemHeight;
	iItemHeight		= GetItemHeight(isel);

	rEdit.top		= iItemHeight * isel;
	rEdit.bottom	= rEdit.top + CITEMHEIGHT;

	rButton.top		= rEdit.top;
	rButton.bottom	= rEdit.bottom;
	CEdit_File.MoveWindow(&rEdit,TRUE);
	CButton_File.MoveWindow(&rButton,TRUE);//���·��ÿؼ���λ��

	if (isel == GetCount() - 1)//˫�����һ��
	{
		CEdit_File.SetWindowText("");
	}
	else
	{
		CString cFile;
		GetText(isel,cFile);//����б���е�ѡ�����Ŀ
		CEdit_File.SetWindowText(cFile);
		CEdit_File.SetSel(0,-1);
	}
	CEdit_File.SetFocus();
    bEditing = TRUE;//��ǰ�Ǳ༭״̬
}

void ListBoxExt::SetData(void)
{
	int isel;
	isel = GetCurSel();
	if (isel == -1)
		return;

	if (bEsc)//�յ�ESC��
		return;

	if (!bChange)//�ı����е�����û�иı�
		return;

	if (bValidFile)//�Ƿ��ļ��ĶԻ����Ѿ���ʾ����
		return;

	bValidFile = FALSE;
	if (!IfValidFileName())
		return;

	CButton_File.ShowWindow(SW_HIDE);
	CEdit_File.ShowWindow(SW_HIDE);

    if (bEditing)
    {
        if (cCurSelFileName != "")//�޸Ļ�����������Ŀ
        {
            DeleteString(isel);//ɾ����ǰ��Ŀ,���²���
            InsertString(isel,cCurSelFileName);
            cCurSelFileName = "";
            SetCurSel(isel);
            
            CString cLastItem;
            GetText(GetCount() - 1,cLastItem);//����б�������һ��
            if (cLastItem != cClewString)//�޸ĵ������һ��
                AddString(cClewString);
            
            CEdit_File.SetWindowText("");
        }
        else//��ʾɾ��
        {
            int iCount;
            iCount = GetCount();
            if (isel == iCount - 1)
            {
                return;
            }
            DeleteString(isel);//ɾ����ǰ��Ŀ
            SetCurSel(isel == 0 ? isel : isel - 1);
        }
        bChange = FALSE;
        bEditing = FALSE;//��ǰ���Ǳ༭״̬
    }
}
void ListBoxExt::OnSetfocus() 
{
	// TODO: Add your control notification handler code here
	SetData();
}

BOOL ListBoxExt::IfValidFileName(void)
{
	CString		filename;
	CStdioFile	file;

	CEdit_File.GetWindowText(filename);

	if ((filename != "") && (filename != cClewString))
	{
		if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
		{
			file.Close();
            int i,iListCount = GetCount();
            CString cFile1;

            for (i = 0;i < iListCount;i++)
            {
                GetText(i,cFile1);
                if ((cFile1 == filename) && (i != iCurrentSel))
                {
                    CButton_File.ShowWindow(SW_SHOW);
                    CEdit_File.ShowWindow(SW_SHOW);

                    bValidFile  = TRUE;
                    InvalidFile = "����ļ��Ѿ���ӹ���";
                    MessageBox("����ļ��Ѿ���ӹ���.","��ʾ",MB_ICONINFORMATION|MB_OK);
                    bValidFile = FALSE;
                    CEdit_File.SetFocus();
                    CEdit_File.SetSel(0,-1);
                    return FALSE;
                }
                
            }
			return TRUE;
		}
		if (!bValidFile)
		{
            CButton_File.ShowWindow(SW_SHOW);
            CEdit_File.ShowWindow(SW_SHOW);

			bValidFile  = TRUE;
			InvalidFile = "�޷��ҵ��ļ�" + filename;
			MessageBox("�޷��ҵ��ļ�" + filename,"��ʾ",MB_ICONASTERISK |MB_OK);
			bValidFile = FALSE;
			CEdit_File.SetFocus();
            CEdit_File.SetSel(0,-1);
		}
		return FALSE;
	}
	InvalidFile = "";
	return TRUE;
}
CString ListBoxExt::GetInvalidFileName(void)
{
	IfValidFileName();
	return (InvalidFile);
}

void ListBoxExt::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	iCurrentSel = GetCurSel();
}

BOOL ListBoxExt::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);
	return CListBox::PreTranslateMessage(pMsg);
}
////////////////////////////////////////////////////
void ListBoxExt::OnContextMenu(CWnd*, CPoint point)
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

		CPoint cPoint = point;

		{
            CMenu m_Menu;
            CBitmap	cAddFiles,cDelete;

            cAddFiles.LoadBitmap(IDB_BITMAP_NEW);
            cDelete.LoadBitmap(IDB_BITMAP_DEL);
            
            m_Menu.CreatePopupMenu();
            m_Menu.AppendMenu( MF_STRING, IDC_MENU_ADDFILES, "��Ӷ���ļ�(&A)" );
            m_Menu.AppendMenu(MF_SEPARATOR,0);
            m_Menu.AppendMenu( MF_STRING, IDC_MENU_DELETEFILE, "ɾ��ȫ���ļ�(&D)");

            //m_Menu.SetMenuItemBitmaps(IDC_MENU_ADDFILES,MF_BYCOMMAND,&cAddFiles,&cAddFiles);
            m_Menu.SetMenuItemBitmaps(IDC_MENU_DELETEFILE,MF_BYCOMMAND,&cDelete,&cDelete);
            if (GetCount() == 1)
            {
                EnableMenuItem(m_Menu,IDC_MENU_DELETEFILE,MF_GRAYED);
            }
            m_Menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, cPoint.x, cPoint.y, this );

            
			/*CMenu menu;
			VERIFY(menu.LoadMenuIndirect(&ListBoxExtPopMenuTemplate));
            
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
				this->GetParent());*/

			//CMenu* pPopup = menu.GetSubMenu(0);
			//pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			//	this->GetParent());
			/*CMenu* pPopup = menu.GetSubMenu(0);

			ASSERT(pPopup != NULL);
			CWnd* pWndPopupOwner = this;
			
			while (pWndPopupOwner->GetStyle() & WS_CHILD)
				pWndPopupOwner = pWndPopupOwner->GetParent();
			
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
				pWndPopupOwner);*/
		}
	}
}

void ListBoxExt::AddFiles(void)
{
    CString	pathfilename;
	char	filetype[] = "VC�ļ�(*.c;*.cpp;*.h)|*.c;*.cpp;*.h|";
	char	cCurrentDir[256] = "";

	GetCurrentDirectory(sizeof(cCurrentDir),cCurrentDir);
	SetCurrentDirectory(cCurrentDir);

	CFileDialog open(TRUE,NULL,NULL,OFN_ALLOWMULTISELECT|OFN_EXTENSIONDIFFERENT,filetype);
	open.m_ofn.lpstrTitle = "ѡ���ļ�";
	open.m_ofn.lpstrInitialDir = cCurrentDir;
	open.m_ofn.lpstrDefExt = "*.c;*.cpp";
	
	if (open.DoModal() == IDOK)
	{
		POSITION ps = open.GetStartPosition();

		while (ps != NULL)
		{
			pathfilename = open.GetNextPathName(ps);

			SHFILEINFO sfi;
			if (::SHGetFileInfo (pathfilename, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO),SHGFI_USEFILEATTRIBUTES | SHGFI_DISPLAYNAME | SHGFI_TYPENAME |SHGFI_ICON|SHGFI_SMALLICON ))
			{
				int i,icount = GetCount();
				CString cFileName;
                if (icount == 1)
                {
                    InsertString(GetCount() - 1,pathfilename);//����б���һ��,�ļ���
                }
                else
                {
                    for (i = 0;i < icount - 1;i++)
                    {
                        GetText(i,cFileName);
                        if (cFileName == pathfilename)
                        {
                            MessageBox("�ļ���" + pathfilename + "���Ѿ���ӹ���.","��ʾ",MB_ICONINFORMATION|MB_OK);
                            return;
                        }
                    }
                    InsertString(GetCount() - 1,pathfilename);//����б���һ��,�ļ���
                }
			}
		}
	}
}

void ListBoxExt::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	//OnDblclk();
    char cKey[2] = "";

    cKey[0] = nChar;
    //CEdit_File.SetWindowText(cKey);
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}
