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
"��Ȩ����(c) 2004��3��\r|"
"��������\t|��������\r|"
"E-mail: zyabc12345@163.com\r||"
//"��ҳ: http://xiaogi.diy.163.com\r||"
"IDB_BITMAP5^|"

"�� ��FindTools��ʹ��˵�� ��\t|"
//"���¸������أ�http://xiaogi.diy.163.com\r||"

"���˵����������������������������������\t||"

"FindChinese:\r|"
"ʹ��FindChinese���߿��Բ��ҵ�C/C++�����е������ַ������ȵ���\r|"
"��ť��ѡ��Ŀ¼�����ڳ��ֵ�Ŀ¼ѡ�����ȷ����ϣ��ɨ���λ�ã�\r|"
"���߾ͻῪʼɨ�裬ɨ��Ľ����ʵʱ��ʾ���б����ɨ�������\r|"
"������ѡ��ֹͣ����ť������ǰɨ�裬��������б���ѡ��һ��֮\r|"
"���б��·�������������Զ���ʾ��ǰ�����ַ����ڵ��ļ��ʹ���\r|"
"�����ĵ�λ�ã���ǰ���������л��Զ��ı���ɫ�����塣�����ϣ��\r|"
"ѡ�����������壬������ѡ��ť���������塱��                \r|"
"															 \r|"
"FindString:\r|"
"��HOPEN��Ŀ�����У�includeĿ¼���и�str_hp.h������ļ��а���\r|"
"�ŵ�ǰ�������õ������������ַ�������Ӧ��ö��ֵ��������һЩö\r|"
"��ֵ�ڵ�ǰ�����в�û���õ���ʹ��FindString���Խ���Щû���õ�\r|"
"��ö��ֵɨ��������������ı�����������ϣ��ɨ���ö���ַ�����\r|"
"ǰ׺��Ȼ�������ѡ��ť��ѡ��Ŀ¼����ѡ��ǰ�������ڵ�Ŀ¼\r|"
"��ѡ��Ŀ¼֮�󣬹��߻��Զ���ȡ��ǰ������includeĿ¼�µ�     \r|"
"str_hp.h�ļ���ȡ���ļ�������ͬǰ׺��ö���ַ����������ڵ�ǰ��\r|"
"���е�����.c�ļ���.h�ļ��н���ɨ�衣���ĳ���ַ���������.c��\r|"
".h�ļ��ж�û�г��ֹ�����ô����Ϊ���ö���ַ����ڵ�ǰ�����в�\r|"
"û��ʹ�ã�������ַ�����ʾ���·����ı����С���ɨ��Ĺ�������\r|"
"����ѡ��ֹͣ����ť������ɨ�衣                            \r|"
"															 \r|"
"MacroTool:\r|"
"���ǵ���Ŀ�����������������һ�״������ںü�����Ŀ�������м�\r|"
"�Ĳ����ĳ����Ŀ�ĺ궨�������֡�ʱ��һ�ã�����ͱ��궨�廮��\r|"
"��֧�����飬����ĳ����Ŀ��˵Ҫ��ɾ��������������Ŀ����Ĵ���\r|"
"����һ������ͷ�۵����顣����ʹ��MacroTool�Ϳ��Խ�������   \r|"
"�⡣�������ı��������뵱ǰ������Ŀ�ĺ궨�壬Ȼ��ѡ��ť��ѡ\r|"
"���ļ�...��������ϣ��������ļ������Զ�ѡ��ѡ����ļ����� \r|"
"ʾ���б���У���ʱ�������ѡ���б���е�һ����Ŀ����ǰ�ļ���\r|"
"�������ͻ���ʾ���·����ı����У������Է���������Ŀ�궨����\r|"
"�Ĵ����Ѿ����ı����������ɫ�ʹ�С����ǰ��Ŀ����Ĵ��������\r|"
"��������ɫȴû�иı䣬������һĿ��Ȼ�����и�����Ŀ����Ĳ���\r|"
"����ʱ��������ѡ���ϡ������¡���ť�鿴��������һ������һ��\r|"
"�궨�Ĵ��롣������Դ���������Ļ���������ѡ��ť�������\r|"
"���ļ������������б���е�������Ҽ���ѡ��˵��е���Ŀ������\r|"
"�ɴ�˫���б���еĵ�ǰ��Ŀ��MacroTool�Ὣ���ڵ�ǰ��Ŀ�궨�� \r|"
"���ֵĴ��������һ���ļ��У��ļ���Ϊ��ǰ������ļ�������.bak\r|"
"�������ϣ��һ�δ������ļ�����ô����ѡ��ť��ȫ���������\r|"
"                                                            \r|"
"                                                            \r|"
"����������\r|"
"                                                            \r|"
"�����Ϊ���������Դ������ȫ�����������������Ҫ���������Ƶ�\r|"
"���ã������뱣�����ߵİ�Ȩ��Ϣ��������ñ���������а�������\r|"
"��Ҫ�����в��ֵĴ����õ��Լ�������У�����������Ÿ�֪��    \r|"
"�����Ա�����кõĽ����bug ���֣�Ҳ�����Ÿ�֪��          \r|"
"                                                            \r|"
"                                                            \r|"
"�ҵ�Email��zyabc12345@163.com\r|"
"��������\r|"
"2004��3��\r|||"
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
#if 0//������Դ֮��֪�����ʾλͼ,����ʱע�͵�
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
        HANDLE h = AfxGetResourceHandle();//���ر������е���Դ
		//�����Ǽ��ض�̬���ӿ��е���Դ
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
#if 0//�޷����ذ汾��Ϣ
		HRSRC hrsrc;

		hrsrc = FindResource(hinstance,MAKEINTRESOURCE("VS_VERSION_INFO"),RT_VERSION);//""
		hrsrc = FindResourceEx(hinstance,RT_STRING,MAKEINTRESOURCE(2106),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
#endif
		int i;
		for (i = 0;i < 4;i++)
			cbitmap[i] = CBitmap::FromHandle(hbitmap[i]);

		cbitmap[0]->GetBitmap(&bm);//�õ�λͼ�ṹ�еĴ�С��Ϣ 

		iPicW = bm.bmWidth;
		iPicH = bm.bmHeight;
		SetRect(&rect,PICX,PICY,iPicW,iPicH);

		FreeLibrary(hinstance);
    }
    //���ضԻ�����Դ
	hinstDLL = LoadLibrary((LPCTSTR)"FindToolsDll.dll");
    if (hinstDLL != NULL)
    {
		HRSRC hrsrc;
		hrsrc = FindResource(hinstDLL,MAKEINTRESOURCE(1000),RT_DIALOG);
	}
	// TODO: Add extra initialization here
#if 0//ԭ��ͼƬ�ڱ�������,���Ǽ���λͼ�Ĵ���
	cBitmap[0].LoadBitmap(IDB_BITMAP1);
	cBitmap[1].LoadBitmap(IDB_BITMAP2);
	cBitmap[2].LoadBitmap(IDB_BITMAP3);
	cBitmap[3].LoadBitmap(IDB_BITMAP4);
	
	cBitmap[0].GetBitmap(&bm);//�õ�λͼ�ṹ�еĴ�С��Ϣ 
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
    MemDC.CreateCompatibleDC(&dc); ///��������ʾ�豸���ݵ��ڴ��豸����

#if 0
	//CBitmap bitmap;
    CBitmap* pOldBitmap;

    //bitmap.LoadBitmap(IDB_BITMAP1); ///ȡ��λͼ��Դ
	
    //pOldBitmap=MemDC.SelectObject(&bitmap);		//��λͼѡ���ڴ泡��
	pOldBitmap = MemDC.SelectObject(&cBitmap[iPic]);	//��λͼѡ���ڴ泡��
    dc.BitBlt(PICX,PICY,iPicW,iPicH,&MemDC,0,0,SRCCOPY);		//��ʾ��
#else
    CBitmap* pOldBitmap,*cbitmap;

    cbitmap = CBitmap::FromHandle(hbitmap[iPic]);
	pOldBitmap = MemDC.SelectObject(cbitmap);	//��λͼѡ���ڴ泡��
    //dc.StretchBlt(PICX,PICY,iPicW,iPicH,&MemDC,0,0,iPicW,iPicH,SRCCOPY);//�����ж�����
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
