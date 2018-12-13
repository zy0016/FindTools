// FindToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FindTools.h"
#include "FindToolsDlg.h"
#include "FindChinese.h"
#include "FindString.h"
#include "MacroTool.h"
#include "FIndHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
// 最小化窗口的参数
#define  WM_NC  1001
#define  IDC_NC 1002

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindToolsDlg dialog

CFindToolsDlg::CFindToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindToolsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindToolsDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(FINDTOOLS_ICON);
}

void CFindToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindToolsDlg)
	DDX_Control(pDX, IDC_CHECK_TUDING, m__TD);
	DDX_Control(pDX, IDC_BUTTON_TUDING, m_TuDing);
	DDX_Control(pDX, IDC_CHECK_FRONT, m_Check_Front);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindToolsDlg, CDialog)
	//{{AFX_MSG_MAP(CFindToolsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_FRONT, OnCheckFront)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_NC,OnNc)  //有这句才能响应鼠标消息
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindToolsDlg message handlers

BOOL CFindToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
#if 0
	FindToolSheet.m_psh.pszCaption = "FindTools";
	FindToolSheet.m_psh.dwFlags = FindToolSheet.m_psh.dwFlags|PSH_NOAPPLYNOW;

	//CWnd *pWnd = GetDlgItem (IDHELP);
	//pWnd->ShowWindow(SW_HIDE);
	
	FindToolSheet.AddPage(&FCpage);
	FindToolSheet.AddPage(&FSpage);
	FindToolSheet.AddPage(&MTpage);
	FindToolSheet.AddPage(&FHpage);
	FindToolSheet.DoModal();
	//pWnd ->SetWindowPos(pWnd-> GetStyle( ) &WS_EX_TOPMOST)? &wndNoTopMOST: &wndTopMost,0,0,0,0,SSP_NOSIZE|WSP_NOMOVE);
	
	CDialog::OnCancel();
#else

	FindToolSheet.AddPage(&FCpage);
	FindToolSheet.AddPage(&FSpage);
	FindToolSheet.AddPage(&MTpage);
	FindToolSheet.AddPage(&FHpage);
	//用Create来创建一个属性页
	FindToolSheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);

	//调整属性页的大小和位置
	FindToolSheet.SetWindowPos(NULL, 0,0,0,0 , SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    {
        //HINSTANCE hins;
        CString msg;
        char szBuf[256];
        char *p;
        //AFX_MANAGE_STATE(AfxGetStaticModuleState());
        //hins=AfxGetInstanceHandle();
        GetCurrentDirectory(sizeof(szBuf),szBuf);
        GetModuleFileName(AfxGetInstanceHandle( ),szBuf,sizeof(szBuf));
        //GetModuleFileName(NULL,szBuf,sizeof(szBuf));//也可以
        p = szBuf;
        while(strchr(p,'\\')) 
        { 
            p = strchr(p,'\\'); 
            p++; 
        }
        *p = '\0'; 		
        msg=szBuf;
    }

    HBITMAP hBitmap;

    hBitmap = (HBITMAP)::LoadImage(AfxGetResourceHandle(),"res\\pin4.bmp",
        IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
    m_Check_Front.SetBitmap(hBitmap);

	return FALSE;
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFindToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
        if(nID == SC_MINIMIZE)  //如果鼠标点击最小化按钮
        {
            NOTIFYICONDATA nc;
            nc.cbSize           = sizeof(NOTIFYICONDATA);
            nc.hIcon            = AfxGetApp()->LoadIcon(FINDTOOLS_ICON);
            nc.hWnd             = m_hWnd;
            nc.uCallbackMessage = WM_NC;
            nc.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            nc.uID              = IDC_NC;
            lstrcpy(nc.szTip,"FindTools");
            Shell_NotifyIcon(NIM_ADD,&nc);
            ShowWindow(SW_HIDE);	
            SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | WS_EX_TOPMOST);
        } 
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFindToolsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFindToolsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFindToolsDlg::OnCheckFront() 
{
	// TODO: Add your control notification handler code here
    HBITMAP hBitmap;
	if (m_Check_Front.GetCheck() == 1)
    {
        SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);//永远在前

        hBitmap = (HBITMAP)::LoadImage(AfxGetResourceHandle(),"res\\pin2.bmp",
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE); 
    }
	else
    {
        SetWindowPos(&this->wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
        
        hBitmap = (HBITMAP)::LoadImage(AfxGetResourceHandle(),"res\\pin4.bmp",
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE);         
    }
    m_Check_Front.SetBitmap(hBitmap);
}

void CFindToolsDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CString str1;
	str1.LoadString(IDS_STRING_CLEW);//提示部分

	if (FCpage.bScaning())
	{
		if(MessageBox("FindChinese工具当前正在扫描文件,您确定停止吗?", str1, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
			return ;
	}
	if (FSpage.bScaning())
	{
		if(MessageBox("FindString工具当前正在扫描文件,您确定停止吗?", str1, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
			return ;
	}

	CDialog::OnCancel();
}

void CFindToolsDlg::OnNc(WPARAM wParam,LPARAM IParam)
{   
    NOTIFYICONDATA nc;
	if ((IParam == WM_LBUTTONDOWN) || (IParam == WM_RBUTTONDOWN))
    {
		ShowWindow(SW_SHOWNORMAL);
		nc.cbSize           = sizeof(NOTIFYICONDATA);
		nc.hWnd             = m_hWnd;
		nc.uCallbackMessage = WM_NC;
		nc.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nc.uID              = IDC_NC;
		Shell_NotifyIcon(NIM_DELETE,&nc);
        OnCheckFront();
		//::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
    else
    {
        nc.cbSize           = sizeof(NOTIFYICONDATA);
        nc.hIcon            = AfxGetApp()->LoadIcon(FINDTOOLS_ICON);
        nc.hWnd             = m_hWnd;
        nc.uCallbackMessage = WM_NC;
        nc.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nc.uID              = IDC_NC;
        if ((FCpage.bScaning()) && (FSpage.bScaning()))
            lstrcpy(nc.szTip,"FindChinese和FindString工具正在扫描文件.");
        else if (FCpage.bScaning())
            lstrcpy(nc.szTip,"FindChinese工具正在扫描文件.");
        else if (FSpage.bScaning())
            lstrcpy(nc.szTip,"FindString工具正在扫描文件.");
        else
            lstrcpy(nc.szTip,"这就是那传说中的查找工具.");

        Shell_NotifyIcon(NIM_MODIFY ,&nc);
    }
	/*if(IParam==WM_RBUTTONDOWN)
	{
		CMenu* menu;
		menu = new CMenu();
		menu->LoadMenu(CG_IDR_POPUP_MACRO_TOOL);
		CMenu* pPopup=menu->GetSubMenu(0);
		::SetMenuDefaultItem(pPopup->m_hMenu,0,TRUE); //设置粗体字
		CPoint Point;
		GetCursorPos(&Point);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN,Point.x,Point.y,AfxGetMainWnd(),NULL);
	}*/
}
