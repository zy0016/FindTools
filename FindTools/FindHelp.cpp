// FindHelp.cpp : implementation file
//

#include "stdafx.h"
#include "FindTools.h"
#include "FindHelp.h"
//#include "urlmon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef HRESULT STDAPICALLTYPE SHOWHTMLDIALOGFN (HWND hwndParent, IMoniker
		*pmk, VARIANT *pvarArgIn, TCHAR* pchOptions,VARIANT *pvArgOut);

/////////////////////////////////////////////////////////////////////////////
// FindHelp property page

IMPLEMENT_DYNCREATE(FindHelp, CPropertyPage)

FindHelp::FindHelp() : CPropertyPage(FindHelp::IDD)
{
	//{{AFX_DATA_INIT(FindHelp)
	//}}AFX_DATA_INIT
	iPicW = 0;
	iPicH = 0;
	int i;
	for (i = 0;i < 4;i++)
	{
		cbitmap[i] = NULL;
		hbitmap[i] = NULL;
	}
}

FindHelp::~FindHelp()
{
}

void FindHelp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindHelp)
	DDX_Control(pDX, IDC_STATIC_EMAIL, m_ctrlMyEmail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindHelp, CPropertyPage)
	//{{AFX_MSG_MAP(FindHelp)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

char *pArrCredit = {
"IDB_BITMAP5^|"
"FindTools1.0\t|"
"版权所有(c) 2004年3月\r|"
"软件设计者\t|聪明的猪\r|"
"E-mail: zyabc12345@163.com\r||"
//"主页: http://xiaogi.diy.163.com\r||"
"IDB_BITMAP5^|"

"★ “FindTools”使用说明 ★\t|"
//"最新更新下载：http://xiaogi.diy.163.com\r||"

"软件说明：　　　　　　　　　　　　　　　\t||"

"FindChinese:\r|"
"使用FindChinese工具可以查找到C/C++代码中的中文字符。首先单击\r|"
"按钮“选择目录”，在出现的目录选择框中确定您希望扫描的位置，\r|"
"工具就会开始扫描，扫描的结果会实时显示在列表里，在扫描过程中\r|"
"您可以选择“停止”按钮结束当前扫描，用鼠标在列表中选择一行之\r|"
"后，列表下方的文字区域会自动显示当前中文字符所在的文件和代码\r|"
"里中文的位置，当前中文所在行会自动改变颜色和字体。如果您希望\r|"
"选择其它的字体，您可以选择按钮“设置字体”。                \r|"
"															 \r|"
"FindString:\r|"
"在HOPEN项目工程中，include目录下有个str_hp.h，这个文件中包含\r|"
"着当前工程中用到的所有中文字符串所对应的枚举值，其中有一些枚\r|"
"举值在当前工程中并没有用到，使用FindString可以将这些没有用到\r|"
"的枚举值扫描出来。首先在文本框中输入您希望扫描的枚举字符串的\r|"
"前缀，然后用鼠标选择按钮“选择目录”，选择当前工程所在的目录\r|"
"，选择目录之后，工具会自动读取当前工程中include目录下的     \r|"
"str_hp.h文件，取出文件中有相同前缀的枚举字符串，并且在当前工\r|"
"程中的所有.c文件，.h文件中进行扫描。如果某个字符串在所有.c和\r|"
".h文件中都没有出现过，那么就认为这个枚举字符串在当前工程中并\r|"
"没有使用，将这个字符串显示在下方的文本框中。在扫描的过程中您\r|"
"可以选择“停止”按钮来结束扫描。                            \r|"
"															 \r|"
"MacroTool:\r|"
"我们的项目常常有这样的情况：一套代码用于好几个项目，代码中间\r|"
"的差别用某个项目的宏定义来区分。时间一久，代码就被宏定义划分\r|"
"得支离破碎，对于某个项目来说要想删掉代码中其它项目定义的代码\r|"
"就是一件令人头疼的事情。现在使用MacroTool就可以解决这个问   \r|"
"题。首先在文本框中输入当前工程项目的宏定义，然后选择按钮“选\r|"
"择文件...”，打开您希望处理的文件，可以多选，选择的文件会显 \r|"
"示在列表框中，这时候用鼠标选择列表框中的一个项目，当前文件的\r|"
"处理结果就会显示在下方的文本框中，您可以发现其它项目宏定义所\r|"
"的代码已经被改变了字体的颜色和大小，当前项目定义的代码的字体\r|"
"包含和颜色却没有改变，您可以一目了然代码中各个项目定义的部分\r|"
"。这时候您可以选择“上”，“下”按钮查看代码中上一处和下一处\r|"
"宏定的代码。如果您对处理结果满意的话，您可以选择按钮“输出单\r|"
"个文件”，或者在列表框中单击鼠标右键，选择菜单中的项目，或者\r|"
"干脆双击列表框中的当前项目，MacroTool会将属于当前项目宏定义 \r|"
"部分的代码输出到一个文件中，文件名为当前代码的文件名加上.bak\r|"
"。如果您希望一次处理多个文件，那么可以选择按钮“全部输出”。\r|"
"                                                            \r|"
"                                                            \r|"
"作者声明：\r|"
"                                                            \r|"
"本软件为自由软件，源代码完全公开。你可以来信索要并不受限制地\r|"
"利用，但务请保留作者的版权信息。若你觉得本软件对你有帮助或你\r|"
"需要将其中部分的代码用到自己的软件中，请给作者来信告知。    \r|"
"如果你对本软件有好的建议或bug 发现，也请来信告知。          \r|"
"                                                            \r|"
"                                                            \r|"
"我的Email：zyabc12345@163.com\r|"
"聪明的猪\r|"
"2004年3月\r|||"
};

/////////////////////////////////////////////////////////////////////////////
// FindHelp message handlers
///////////////////////////////////////////////////////////////////////////////
void FindHelp::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	iPic = ++iPic % 4;
	
	InvalidateRect(&rect,TRUE);
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL FindHelp::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    static HINSTANCE hinstDLL; 
    typedef BOOL (CALLBACK *inshook)(); 
    //inshook instkbhook;
    HINSTANCE hinstance;
	BITMAP bm;

    hinstDLL = LoadLibrary((LPCTSTR)"FindToolsRsDll.dll");
    if (hinstDLL != NULL)
    {
        hinstance = LoadLibraryEx("FindToolsRsDll.dll",NULL,LOAD_LIBRARY_AS_DATAFILE);        
#if 0//加载资源之后不知如何显示位图,故暂时注释掉
		HRSRC hrsrc;

		hrsrc = FindResource(hinstance,MAKEINTRESOURCE(2101),RT_BITMAP);
        hGlobal[0] = LoadResource(hinstDLL,hrsrc);

        hrsrc = FindResource(hinstance,MAKEINTRESOURCE(2102),RT_BITMAP);
        hGlobal[1] = LoadResource(hinstDLL,hrsrc);

        hrsrc = FindResource(hinstance,MAKEINTRESOURCE(2103),RT_BITMAP);
        hGlobal[2] = LoadResource(hinstDLL,hrsrc);

        hrsrc = FindResource(hinstance,MAKEINTRESOURCE(2104),RT_BITMAP);
        hGlobal[3] = LoadResource(hinstDLL,hrsrc);

        lpvoid[0] = LockResource(hGlobal[0]);
        lpvoid[1] = LockResource(hGlobal[1]);
        lpvoid[2] = LockResource(hGlobal[2]);
        lpvoid[3] = LockResource(hGlobal[3]);
#endif
        HANDLE h = AfxGetResourceHandle();//加载本工程中的资源
		//以下是加载动态链接库中的资源
		char cWidth[10] = "",cHeight[10] = "";
		int iWidth = 0,iHeight = 0;
		LoadString(hinstance,2105,cWidth,sizeof(cWidth));
		LoadString(hinstance,2106,cHeight,sizeof(cHeight));

		iWidth  = atoi(cWidth);
		iHeight = atoi(cHeight);

        hbitmap[0] = (HBITMAP)::LoadImage(hinstance,
            MAKEINTRESOURCE(2101),IMAGE_BITMAP,iWidth,iHeight,LR_CREATEDIBSECTION);//LR_LOADFROMFILE
        hbitmap[1] = (HBITMAP)::LoadImage(hinstance,
            MAKEINTRESOURCE(2102),IMAGE_BITMAP,iWidth,iHeight,LR_CREATEDIBSECTION);//LR_LOADFROMFILE
        hbitmap[2] = (HBITMAP)::LoadImage(hinstance,
            MAKEINTRESOURCE(2103),IMAGE_BITMAP,iWidth,iHeight,LR_CREATEDIBSECTION);//LR_LOADFROMFILE
        hbitmap[3] = (HBITMAP)::LoadImage(hinstance,
            MAKEINTRESOURCE(2104),IMAGE_BITMAP,iWidth,iHeight,LR_CREATEDIBSECTION);//LR_LOADFROMFILE
#if 0//无法加载版本信息
		HRSRC hrsrc;

		hrsrc = FindResource(hinstance,MAKEINTRESOURCE("VS_VERSION_INFO"),RT_VERSION);//""
		hrsrc = FindResourceEx(hinstance,RT_STRING,MAKEINTRESOURCE(2106),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
#endif
		int i;
		for (i = 0;i < 4;i++)
			cbitmap[i] = CBitmap::FromHandle(hbitmap[i]);

		cbitmap[0]->GetBitmap(&bm);//得到位图结构中的大小信息 

		iPicW = bm.bmWidth;
		iPicH = bm.bmHeight;
		SetRect(&rect,PICX,PICY,iPicW,iPicH);

		FreeLibrary(hinstance);
    }
    //加载对话框资源
	hinstDLL = LoadLibrary((LPCTSTR)"FindToolsDll.dll");
    if (hinstDLL != NULL)
    {
		HRSRC hrsrc;
		hrsrc = FindResource(hinstDLL,MAKEINTRESOURCE(1000),RT_DIALOG);
	}
	// TODO: Add extra initialization here
#if 0//原来图片在本工程中,这是加载位图的代码
	cBitmap[0].LoadBitmap(IDB_BITMAP1);
	cBitmap[1].LoadBitmap(IDB_BITMAP2);
	cBitmap[2].LoadBitmap(IDB_BITMAP3);
	cBitmap[3].LoadBitmap(IDB_BITMAP4);
	
	cBitmap[0].GetBitmap(&bm);//得到位图结构中的大小信息 
#endif

    //////////////////////////////////////////////////////////////////////////
	//m_ctrlMyHomepage.SetURL("http://xiaogi.diy.163.com");
	m_ctrlMyEmail.SetURL("mailto:yongzhao");
	m_ctrlMyEmail.SetUnderline(FALSE);

	m_static.SubclassDlgItem(IDC_STATIC_HELP,this);
	m_static.SetCredits(pArrCredit,'|');
	m_static.SetColor(BACKGROUND_COLOR, RGB(0,0,255));
	m_static.SetTransparent();
	m_static.StartScrolling();
	m_nCreditTimer = SetTimer(ABOUT_CREDIT_TIMER,5000,NULL);
	ASSERT(m_nCreditTimer != 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FindHelp::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
    CDC MemDC;
    MemDC.CreateCompatibleDC(&dc); ///建立与显示设备兼容的内存设备场境

#if 0
	//CBitmap bitmap;
    CBitmap* pOldBitmap;

    //bitmap.LoadBitmap(IDB_BITMAP1); ///取出位图资源
	
    //pOldBitmap=MemDC.SelectObject(&bitmap);		//将位图选入内存场境
	pOldBitmap = MemDC.SelectObject(&cBitmap[iPic]);	//将位图选入内存场境
    dc.BitBlt(PICX,PICY,iPicW,iPicH,&MemDC,0,0,SRCCOPY);		//显示它
#else
    CBitmap* pOldBitmap,*cbitmap;

    cbitmap = CBitmap::FromHandle(hbitmap[iPic]);
	pOldBitmap = MemDC.SelectObject(cbitmap);	//将位图选入内存场境
    //dc.StretchBlt(PICX,PICY,iPicW,iPicH,&MemDC,0,0,iPicW,iPicH,SRCCOPY);//这两行都可以
    dc.BitBlt(PICX,PICY,iPicW,iPicH,&MemDC,0,0,SRCCOPY);

    {
        /*CRect rc;        
        GetClientRect(rc);
        CDC memdc;
        memdc.CreateCompatibleDC(&dc);
        dc.BitBlt(0,0,rc.Width(),rc.Height(),&memdc,0,0,SRCCOPY);*/
        /*CRect rc;        
        GetClientRect(rc);  
        CDC memdc;
        memdc.CreateCompatibleDC(&dc);
        CBitmap bmp;
        bmp.CreateCompatibleBitmap (&dc,rc.Width(),rc.Height());
        CBitmap *oldbmp=memdc.SelectObject(&bmp);
        static int curframe=0;
        //m_myimglist.Draw(&memdc,curframe,Cpoint(0,0),ILD_TRANSPARENT);
        //curframe++;
        //if(curframe > m_myimglist.GetImageCount())
          //  curframe=0;
        if(oldbmp)
            memdc.SelectObject(oldbmp);

        dc.BitBlt(0,0,rc.Width(),rc.Height(),&memdc,0,0,SRCCOPY);*/
    }
#endif

    MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void FindHelp::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (bShow)
	{
		iPic = 0;
		m_nTimer = SetTimer(SCANING_TIMER, 500, NULL);
	}
	else
	{
		KillTimer(m_nTimer);
		m_nTimer = 0;
	}
}
