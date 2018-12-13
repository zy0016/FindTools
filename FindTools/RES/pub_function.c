//fdsfs
//fdsfs
//fdsfs
//fdsfs


/*
//fdddddddddddd
//
//fdfafd
//dfaas2005-7-17*/



//fdsfs
//fdsfs
//fdsfs
//fdsfs

#include    "stdio.h"
#include    "string.h"
#include    "stdlib.h"
#include    "str_hp.h"
#include    "str_public.h"
#include    "hp_pdaex.h"
#include    "pubapp.h"
#include    "setting.h"
#include    "fapi.h"
#include    "winpda.h"
#include    "hpimage.h"
#if defined(CAPITEL)
#include    "hp_dhi.h"//硬件函数所需的文件
#endif

#ifndef MAIN_LCD
#define MAIN_LCD                1// LCD标号
#endif

#define     SIMCLEW1            GetString(STR_SETUP_SIM_CLEW1)//"未插入SIM卡"
#define     SIMCLEW3            GetString(STR_SETUP_SIM_CLEW3)//"提示"

extern  int     GetSIMState();
extern  int     GetCurrentLanguage();               //从变量中获得当前语言ID号
extern  int     GetLanguage();                      //从文件中获得当前语言ID号(只在初始化函数Setup_InitFunction中执行一次)
extern  BOOL    CallAutoCloseClewWindow(void);      //自动关机界面
extern  void    GetDefaultRing(char * pRingName);   //获得默认的铃声
extern  int     GetMaxRingVolume(void);             //获得最大铃声音量
extern  void    App_GetCurPhoneNum(char *pPhoneNum);//获得当前电话号码
extern  BOOL    APP_PBGetCurRingName(char* cPhoneNum, char* ringname);//根据当前电话号码,获得当前铃声

extern  int     GetBg_keyboard();       //获得键盘背光灯信息返回数值表示秒数
extern  int     GetBg_screen();         //获得屏幕背光灯信息返回0表示"关闭",非零为秒数
extern  int     GetContrast();          //获得对比度值
extern  int     GetContrastMax();       //获得对比度最大值
extern  int     GetContrastMin();       //获得对比度最小值
extern  void    SetCurrentColorCase();  //设置当前的色彩方案

#if defined(_PHOSPHOR_)
extern  int     GetBglightValue();      //获得亮度
extern  BOOL    Setup_ColorInit();      //颜色设置初始化
        BOOL    CreateWindow_PlayMusic();//创建一个隐藏窗口用于播放声音,该函数是系统启动的时候创建
extern  void    Switch_init(void);      //切换设置
#endif

//////////////////////////本地函数////////////////////////////////////////////////
void    BeginTime(void);//限定声音播放的时间
void    SM_LibrateRingBegin(void);//实现先振动(5秒)后响铃的功能
void    SM_LibrateRingEnd(void);//结束先振动后响铃的功能,主要是删除定时器
BOOL    CreateWindow_Setup(void);//系统启动的时候创建一个隐藏窗口用于停止震动
void    BeginVibration(int iSecond);//实现振动iSecond秒

void    ShowDebugInfo(char * pInfo,char * pOther,long iOther);//显示调试信息
/////////////////////////自动换行函数/////////////////////////////////////////////////
int     AutoNewLine(HDC hdc,char * cSourstr,char * cDesstr,RECT * rect,UINT uFormat);
int     AutoNewLine_chn(HDC hdc,char * cSourstr,char * cDesstr,int iRectWidth);//返回行数
int     AutoNewLine_eng(HDC hdc,char * cSourstr,char * cDesstr,int iRectWidth);//返回行数
int     GetCharactorWidth(HDC hdc,char * ch);//获得单个字符或者单个汉字的宽度
////////////////////////设置部分包的硬件函数//////////////////////////////////////////////////
void    SetupHf_LcdBklight(unsigned int uiLcdNo,BOOL bEn);
void    SetupHf_Keylight(BOOL bEn);
void    SetupHf_Contrast(unsigned int uiLcdNo,int icontrast);
void    SetupHf_BgLight(int iBgLight);
void    SetupHf_Vibration(BOOL bEn);
BOOL    SetupHf_Volume(int iVolume);
int     Setup_EndPlayMusic(int uiringselect);
/********************************************************************
* Function   IfFileExist  
* Purpose    考查当前文件是否存在
* Params     pFileName:文件名包括路径
* Return     存在ture,不存在false
* Remarks      
**********************************************************************/
BOOL    IfFileExist(char * pFileName)
{
    HANDLE  hf;
#if defined(CAPITEL)
    hf = CreateFile(pFileName,ACCESS_READ,0);
#elif defined(_PHOSPHOR_)
    char * p = NULL;
    if (strncmp(pFileName,"/rom",4) == 0)
    {
        p = (char *)malloc(strlen(pFileName) + 1);
        if (p == NULL)
            hf = CreateFile(pFileName,ACCESS_READ,0);
        else
        {
            strcpy(p,"rom:");
            strcat(p,pFileName + 4);
            hf = CreateFile(p,ACCESS_READ,0);
            free(p);
        }
    }
    else
        hf = CreateFile(pFileName,ACCESS_READ,0);
#endif
    if(INVALID_HANDLE_VALUE == hf)//未找到文件
    {   
        CloseFile(hf);
        return FALSE;
    }
    CloseFile(hf);
    return TRUE;
}
/********************************************************************
* Function   CountControlYaxis  
* Purpose    计算控件纵坐标的函数 
* Params     piConY:用于返回控件的纵坐标,iControlH:各个控件的高
            iControlNum:控件的个数
* Return     
* Remarks    要求各个控件在界面上从上到下排列,并且各个控件的间隔是一致的
**********************************************************************/
void    CountControlYaxis(int * piConY,int iControlH,int iControlNum)
{
    int i,iyStep;

    iyStep = (GetSystemUsableWH(SM_CYSCREEN) - iControlNum * iControlH) / (iControlNum + 1);//计算每个控件的间隔
    for (i = 0;i < iControlNum;i++)
        * piConY++ = iyStep + i * (iControlH + iyStep);//计算控件的纵坐标
}
/********************************************************************
* Function   FindSIMState  
* Purpose     获得SIM卡的状态.
* Params     bShow:是否显示当前状态提示
* Return     当前状态编号
* Remarks      
**********************************************************************/
int     FindSIMState(BOOL bShow)
{
    int istate = 1;
    istate = GetSIMState();
    if (bShow)
    {
        if(!istate)
            MsgBox(NULL,SIMCLEW1,SIMCLEW3,WAITTIMEOUT);//未插入SIM卡
    }
    return (istate);
}
//////////////////////////////////////////////////////////////////////////
void    GetSystemUsableXY(int * width,int * height)//获得屏幕中可以利用的宽度和高度
{
    * width  = GetSystemUsableWH(SM_CXSCREEN);
    * height = GetSystemUsableWH(SM_CYSCREEN);
}
int     GetSystemUsableWH(int iwh)//获得屏幕中可以利用的宽度或者高度
{
    int iresult = 0;
    switch (iwh)
    {
    case SM_CXSCREEN:
        iresult = GetSystemMetrics(SM_CXSCREEN);
        break;
    case SM_CYSCREEN:
#if defined(CAPITEL)
        iresult = 180;
#elif defined(_PHOSPHOR_)
        iresult = 240;
#endif
        break;
    }
    return(iresult);
}
/********************************************************************
* Function   LoadHbitmaps  
* Purpose    加载**P中的位图
* Params     ** p位图文件名的指针数组
* Return     
* Remarks    
**********************************************************************/
void    LoadHbitmaps(HBITMAP * hbitmap,char ** p)
{
    while (** p)
        * hbitmap++ = LoadImage(NULL,* p++,IMAGE_BITMAP,0, 0, LR_LOADFROMFILE);
}
/********************************************************************
* Function   LoadHbitmaps_Gif  
* Purpose    加载**P中的位图
* Params     ** p位图文件名的指针数组
* Return     
* Remarks    
**********************************************************************/
void    LoadHbitmaps_Gif(HWND hWnd,HBITMAP * hbitmap,char ** p)
{
    COLORREF    Color;
    BOOL        bGif;
    HDC         hdc;

    hdc = GetDC(hWnd);
    while (** p)
        * hbitmap++ = CreateBitmapFromImageFile(hdc,* p++,&Color,&bGif);

    ReleaseDC(hWnd,hdc);
}
/********************************************************************
* Function   List_AddString  
* Purpose    将相应的文字添加到列表框中
* Params     bAppend:图标是否是个数组,TRUE的话,hIconNormal指针需要移动,否则hIconNormal指针不动
* Return     
* Remarks    
**********************************************************************/
void    List_AddString(HWND hList,HBITMAP * hIconNormal,char ** p,BOOL bAppend)
{
    DWORD   dWord;
    int     i = 0;

    while (** p)
    {
        SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)* p++);

        if (hIconNormal != NULL)
        {
            dWord = MAKEWPARAM((WORD)IMAGE_BITMAP, (WORD)i++);
            SendMessage(hList, LB_SETIMAGE, (WPARAM)dWord, (LPARAM)* hIconNormal);
        }
        if ((bAppend) && (hIconNormal != NULL))
            hIconNormal++;
    }
    SendMessage(hList , LB_SETCURSEL, 0, 0 );
}
/********************************************************************
* Function   AddListString  
* Purpose    将指针数组**p中的文字添加到StrSpinBox中
* Params     
* Return     
* Remarks    
**********************************************************************/
void    StrSpinBox_AddString(HWND hStrSpinBox,char ** p)
{
    while (** p)
        SendMessage(hStrSpinBox, SSBM_ADDSTRING, 0, (LPARAM)* p++);

    SendMessage(hStrSpinBox,SSBM_SETCURSEL,0,0);
}
/********************************************************************
* Function   ComboBox_AddString  
* Purpose    将指针数组**p中的文字添加到ComboBox中
* Params     
* Return     
* Remarks    
**********************************************************************/
void    ComboBox_AddString(HWND hComboBox,char ** p)
{
    while (** p)
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)* p++);

    SendMessage(hComboBox,CB_SETCURSEL,0,0);
}
/********************************************************************
* Function   MenuList_AddString  
* Purpose    重新加载数据到Menulist中
* Params     **P:需要添加的文字,* id:各个文字对应的项目的id,** pic:各个项目对应的图标文件名
* Return     
* Remarks      
**********************************************************************/
void    MenuList_AddString(HWND hMenuList,char ** p,int * id,char ** pic)
{
    MENULISTITEMTEMPLATE    tmpValue;
    int                     nMenuTemplateSize;
    nMenuTemplateSize = sizeof(MENULISTITEMTEMPLATE);
    
    while(** p)
    {
        memset(&tmpValue,0x00,nMenuTemplateSize);
        if (pic != NULL)
            tmpValue.lpszIconName = * pic++;
        else
            tmpValue.lpszIconName = NULL;

        tmpValue.dwFlags        = MLI_LEFT | MLI_NOTIFY;
        tmpValue.nPicAreaWidth  = 20;
        tmpValue.wID            = * id++;
        tmpValue.lpszItemName   = * p++;
        SendMessage(hMenuList, ML_ADDITEM, 0, (LPARAM)&tmpValue);
    }
}
/********************************************************************
* Function   LoadMenuList  
* Purpose    创建HMENULIST
* Params     bLostkeyNotify:
            如果希望点击左softkey的时候,menulist收不到消息,那么请取FALSE
            如果希望点击左softkey的时候后,只有左softkey收到消息,
            而menulis收不到消息,那么请取TRUE
* Return     
* Remarks      
**********************************************************************/
HMENULIST   LoadMenuList(MENULISTTEMPLATE * MenuListElement,BOOL bLostkeyNotify)
{
    int isw,ish;
    
    GetSystemUsableXY(&isw,&ish);

    MenuListElement->cmlit          = 0;    
    MenuListElement->lpszBgPic      = NULL;
    MenuListElement->nItemWidth     = isw;
    MenuListElement->nItemHeight    = 22;
    MenuListElement->nTopLeftX      = 0;
    MenuListElement->nTopLeftY      = 0;
    MenuListElement->nLeftGap       = 5;

#if defined(_PHOSPHOR_)
    MenuListElement->nUArrowX       = isw - 20;
    MenuListElement->nUArrowY       = 5;    
    MenuListElement->nUArrowHeight  = 10;
    MenuListElement->nUArrowWidth   = 10;
    
    MenuListElement->lpszUAPic      = NULL;
    MenuListElement->nDArrowX       = isw - 20;
    MenuListElement->nDArrowY       = ish - 10;
    MenuListElement->nDArrowHeight  = 10;
    MenuListElement->nDArrowWidth   = 10;
    MenuListElement->lpszDAPic      = NULL;
    MenuListElement->pmlItems       = 0;

    MenuListElement->dwStyle        = MLI_LEFT | MLI_NOTIFY;    
#elif defined(CAPITEL)
    MenuListElement->colIconBk      = RGB(131,220,255);
    MenuListElement->colSelIconBk   = GetColorCaseValue(SN_GETSELICOBK);
    MenuListElement->colSelItem     = GetColorCaseValue(SN_GETSELITEM);
    MenuListElement->colSepLine     = RGB(3,99,239);
    
    if (bLostkeyNotify)
        MenuListElement->dwStyle    = MLI_SETICONBK | MLI_SETSELCOL | MLI_NO3DSELITEM | MLI_SETSEPLNCOL|MLI_LSOFTKEYNOTIFY;
    else
        MenuListElement->dwStyle    = MLI_SETICONBK | MLI_SETSELCOL | MLI_NO3DSELITEM | MLI_SETSEPLNCOL;

#endif
    return(LoadMenuListIndirect((const MENULISTTEMPLATE *)MenuListElement));
}
/********************************************************************
* Function   ResetMenuList  
* Purpose    清除MENULIST中的全部数据,*id需要以-1为结束标志
* Params     
* Return     
* Remarks      
**********************************************************************/
BOOL    ResetMenuList(HWND hMenuList,int * id)
{
    while (* id > 0)
        SendMessage(hMenuList,ML_DELITEM,* id++,0);
    
    return TRUE;
}
/********************************************************************
* Function   LongToStrings  
* Purpose    将数字转化成字符串数组,结果放在str所指向的字符串中. 
* Params     str:存放结果的字符串(要足够长),num:需要转换的数字(不支持负数),bZero:数字之前是否添加"0"
                bZero = FALSE,数字之前不添加"0"
                bZero = TRUE,数字之前添加"0"
                izeronum:添加的0的个数,这个个数应该是这个数字的含义所允许的最多的0的个数
                比如:num的含义是月份,日期,izeronum就应该为2.返回"02"月,"09"日.或者"12"月,"31"日
                num的含义是小时,分钟,izeronum也应该是2,返回"03"小时,"05"分钟,或者"14"小时,"58"分钟
                即程序会在数字的前方自动补足相应数量的"0"
* Return     
* Remarks      
**********************************************************************/
void    LongToStrings(char * str,unsigned long num,BOOL bZero,int izeronum)
{
    long    ires,imul;
    int     idigit = 1,i,j,integer;

    ires = num;
    while (ires / 10 > 0)
    {//获得数字的位数
        ires = ires / 10;
        idigit++;
    }
    if (bZero)
    {
        for (i = 0;i < izeronum - idigit;i++)
        {
            * str = (char)48;
            str++;
        }
    }

    ires = num;
    for (i = 0;i < idigit;i++)
    {
        imul = 1;
        for (j = 1 ;j < idigit - i;j++)
            imul = imul * 10;
        
        integer = ires / imul ;//获得单个的数字
        * str = (char) (integer + 48);
        str++;
        ires = ires - integer * imul;
    }
    * str = (char)0;
}
/********************************************************************
* Function   Setup_Init  
* Purpose    提供给程序管理器,用于初始化设置各个参数
* Params     
* Return     
* Remarks    
**********************************************************************/
BOOL    Setup_InitFunction(void)
{
    BOOL        bRes = TRUE;
    SCENEMODE   sm;

    SetCurrentLanguage(GetLanguage());  //设置当前语言
    CallAutoCloseClewWindow();          //创建自动关机界面
    SetUp_SetDialCfg();                 //创建拨号设置所需数据
#if defined(_PHOSPHOR_)
    CreateDirectory("FLASH2://public");//创建程序所需目录
    SetCurrentDirectory("FLASH2://public");
    CreateDirectory("setup");
#endif

///////////////////////////设置色彩方案部分/////////////////////////////////////////
#if defined(CAPITEL)
    SetCurrentColorCase();
#elif defined(_PHOSPHOR_)
    Setup_ColorInit();
#endif

///////////////////////////设置背光灯,键盘背光部分///////////////////////////////////////////////
#if defined(CAPITEL)//设置键盘背光灯,屏幕背光灯
    SetupHf_Keylight(GetBg_keyboard());             //开关键盘的背景灯
    SetupHf_LcdBklight(MAIN_LCD,GetBg_screen());    //开关LCD的背光灯
#elif defined(_PHOSPHOR_)
    //设置背景灯是否点亮
    SetupHf_LcdBklight(0,GetBglight()); //开关LCD的背光灯
#endif

/////////////////////////设置对比度部分/////////////////////////////////////////////////
    SetupHf_Contrast(MAIN_LCD,GetContrast());//获得,设置对比度值

#if defined(_PHOSPHOR_)
    SetupHf_BgLight(GetBglightValue());//在此设置亮度
#endif

/////////////////////////情景模式部分/////////////////////////////////////////////////
    CreateWindow_Setup();//创建一个隐藏窗口用于震动一定时间自动停止

    GetSM(&sm,GetCurSceneMode());//获得情景模式信息
    bRes = SetupHf_Volume(sm.iRingvolume) && bRes;//设置音量
    if ((sm.iComeview == 1) || (sm.iComeview == 2) || (sm.iComeview == 4))//振铃模式(1:振动;2:振动铃声,4:先振动后响铃)
    {
        BeginVibration(1);//震动1秒
    }
/////////////////////////初始化声音播放/////////////////////////////////////////////////
#if defined(_PHOSPHOR_)
    bRes = CreateWindow_PlayMusic() && bRes;
    Switch_init();//声道设备切换设置
#endif
    return (bRes);
}
////////////////////////////支持自动换行的提示窗口//////////////////////////////////////////////
BOOL    ConfirmWin(HWND hwnd,char * szPrompt,char * szTipsPic,char * szCaption,char * szOk,char * szCancel)
{
    HDC     hdc;
    char *  pDesstr = NULL;
    int     ilen;
    BOOL    bres;
    RECT    rect;

#if defined(CAPITEL)//定义提示框显示文字的区域
    SetRect(&rect,10,42,166,191);//定义提示框显示文字的区域
#elif defined(_PHOSPHOR_)
    SetRect(&rect,10,84,228,229);
#endif
    
    if ((hwnd == NULL) || (strlen(szPrompt) == 0))
    {
        bres = HPConfirmWin(szPrompt,szTipsPic,szCaption,szOk,szCancel);
        return (bres);
    }

    ilen = strlen(szPrompt) * 2 + 1;//极端的情况,一个字符(加上换行符)占一行

    if ((pDesstr = (char *)malloc(ilen)) == NULL)
    {
        bres = HPConfirmWin(szPrompt,szTipsPic,szCaption,szOk,szCancel);
        return (bres);
    }
    memset(pDesstr,0x00,ilen);

    hdc = GetDC(hwnd);
    AutoNewLine(hdc,szPrompt,pDesstr,&rect,0);
    ReleaseDC(hwnd,hdc);

    bres = HPConfirmWin(pDesstr,szTipsPic,szCaption,szOk,szCancel);
    
    free(pDesstr);

    return(bres);
}
//文字可以自动换行的提示框
void    MsgBox(HWND hwnd,char * szPrompt,char * szCaption,int nTimeout)
{
    HDC     hdc;
    char *  pDesstr = NULL;
    int     ilen;
    RECT    rect;

    if ((hwnd == NULL) || (strlen(szPrompt) == 0))
    {
        HPTipsWin(szPrompt,szCaption,WAITTIMEOUT);
        return;
    }
    ilen = strlen(szPrompt) * 2 + 1;//极端的情况,一个字符(加上换行符)占一行

    if ((pDesstr = (char *)malloc(ilen)) == NULL)
    {
        HPTipsWin(szPrompt,szCaption,WAITTIMEOUT);
        return;
    }
    memset(pDesstr,0x00,ilen);
    
#if defined(CAPITEL)//定义提示框显示文字的区域
    SetRect(&rect,6,16,90,60);//定义提示框显示文字的区域
#elif defined(_PHOSPHOR_)
    SetRect(&rect,6,16,103,60);
#endif

    hdc = GetDC(hwnd);
    AutoNewLine(hdc,szPrompt,pDesstr,&rect,0);
    ReleaseDC(hwnd,hdc);

    HPTipsWin(pDesstr,szCaption,WAITTIMEOUT);
    
    free(pDesstr);
}
///////////////////////////////以下部分为自动换行所需函数--begin
//当cDesstr==NULL时,认为直接在函数中输出字符串,即直接DrawText,返回矩形区域需要的高度
//当cDesstr!=NULL时,返回生成的字符串,不执行DrawText,函数返回矩形区域需要的高度
int     AutoNewLine(HDC hdc,char * cSourstr,char * cDesstr,RECT * rect,UINT uFormat)
{
    char *  pDesstr = NULL;
    int     iRowNum,iRectHeight,iRectWidth,ilen,ioldbuttom;
    int     iLanguage = 1;//0:中文,1:英文
    SIZE    size;

    if (strlen(cSourstr) == 0)
    {
        if (cDesstr == NULL)
            DrawText(hdc,cSourstr,strlen(cSourstr),rect,uFormat);

        return 0;
    }
    ilen = strlen(cSourstr) * 2 + 1;//极端的情况,一个字符(加上换行符)占一行

    if ((pDesstr = (char *)malloc(ilen)) == NULL)
    {
        if (cDesstr == NULL)
            DrawText(hdc,cSourstr,strlen(cSourstr),rect,uFormat);

        return 0;
    }
    memset(pDesstr,0x00,ilen);

    iRectWidth = rect->right - rect->left;//获得矩形区域的宽度

    iLanguage = GetCurrentLanguage();//获得当前语言,0:英文,1:中文

    switch (iLanguage)
    {
    case 0://英文
        iRowNum = AutoNewLine_eng(hdc,cSourstr,pDesstr,iRectWidth);
        GetTextExtent(hdc,"H",strlen("H"),&size);//获得英文字符的高度
        break;
    case 1://中文
        iRowNum = AutoNewLine_chn(hdc,cSourstr,pDesstr,iRectWidth);
        GetTextExtent(hdc,"十",strlen("十"),&size);//获得中文字符的高度
        break;
    }
    iRectHeight = iRowNum * size.cy + 5;//矩形区域需要的高度

    if (cDesstr != NULL)
        strcpy(cDesstr,pDesstr);
    else
    {
        ioldbuttom   = rect->bottom;            //保存矩形区域原来的高度
        rect->bottom = rect->top + iRectHeight; //修改矩形区域的高度用于输出
        DrawText(hdc,pDesstr,strlen(pDesstr),rect,uFormat);
        rect->bottom = ioldbuttom;              //恢复矩形区域原来的高度
    }
    free(pDesstr);

    return (iRectHeight);
}
int     AutoNewLine_chn(HDC hdc,char * cSourstr,char * cDesstr,int iRectWidth)//返回行数
{
    int     iCharactorWidth;//一个英文字符或者一个汉字的宽度
    int     iCopyNum,iSourlen,iRowNum,iCopyWidth;
    char *  p = NULL;

    p           = cSourstr;
    iSourlen    = strlen(cSourstr);
    iCopyNum    = 0;//已经拷贝的字符个数
    iRowNum     = 1;//行数
    iCopyWidth  = 0;//一行中已经拷贝的字符一共有多长

    while(iCopyNum < iSourlen)
    {
        if ((* p == NULL) || (p == NULL))
            break;
        if ((* p == 0xa) || (* p == 0xd))//不添加换行符
        {
            p++;
            iCopyNum++;
            continue;
        }
        iCharactorWidth = GetCharactorWidth(hdc,p);//获得当前字符(或者汉字)的长度
        if (iRectWidth - iCopyWidth > iCharactorWidth)//剩余长度大于一个英文字符或者一个汉字的宽度
        {
            if ((unsigned char)(* p) > 0x80)//中文
            {
                strncat(cDesstr,p,2);
                iCopyNum += 2;
                p += 2;
            }
            else
            {
                strncat(cDesstr,p,1);
                iCopyNum++;
                p++;
            }
            iCopyWidth += iCharactorWidth;
        }
        else if (iRectWidth - iCopyWidth == iCharactorWidth)//剩余长度等于一个英文字符或者一个汉字的宽度
        {
            if ((unsigned char)(* p) > 0x80)//中文
            {
                strncat(cDesstr,p,2);
                iCopyNum += 2;
                p += 2;
            }
            else
            {
                strncat(cDesstr,p,1);
                iCopyNum++;
                p++;
            }
            strcat (cDesstr,"\n");
            iCopyWidth = 0;
            iRowNum++;//行数加一    
        }
        else if (iRectWidth - iCopyWidth < iCharactorWidth)
        {//剩余长度小于一个英文字符或者一个汉字的宽度,需要添加换行符,行数加一
            if (iCopyNum == 0)
            {
                //一个字符也没有拷贝就添加换行符,则这个单词的长度已经超过
                //了矩形区域的宽度,直接拷贝源字符串,然后返回
                strcpy(cDesstr,cSourstr);
                return (strlen(cSourstr));
            }
            strcat(cDesstr,"\n");
            iCopyWidth = 0;
            iRowNum++;
        }
    }
    strcat(cDesstr,"\0");

    return (iRowNum);
}
int     AutoNewLine_eng(HDC hdc,char * cSourstr,char * cDesstr,int iRectWidth)//返回行数
{
    int     iCopyNum,iSourlen,iRowNum,iCopyWidth,iWordWidth,iWordLength;
    char *  p = NULL,* pNextSpace = NULL,* pScan = NULL;

    p           = cSourstr;
    iSourlen    = strlen(cSourstr);
    iCopyNum    = 0;//已经拷贝的字符个数
    iRowNum     = 1;//行数
    iCopyWidth  = 0;//一行中已经拷贝的字符一共有多长

    while(iCopyNum < iSourlen)
    {
        if ((* p == NULL) || (p == NULL))
            break;
        if ((* p == 0xa) || (* p == 0xd))//不添加换行符
        {
            p++;
            iCopyNum++;
            continue;
        }

        pScan       = p;
        pNextSpace  = strchr(p,' ');//非英文字母

        iWordWidth  = 0;

        if (pNextSpace != NULL)
        {
            iWordLength = abs(pScan - pNextSpace);//获得这个英文单词的字母个数

            while (pScan != pNextSpace)
            {
                iWordWidth += GetCharactorWidth(hdc,pScan);//获得这个英文单词的宽度
                pScan++;
            }
        }
        else//到了字符串结尾
        {
            iWordLength = iSourlen - iCopyNum;

            while (* pScan != NULL)
            {
                iWordWidth += GetCharactorWidth(hdc,pScan);//获得这个英文单词的宽度
                pScan++;
            }
        }
        
        if (iRectWidth - iCopyWidth > iWordWidth)//剩余长度>一个英文单词的宽度,直接拷贝
        {
            strncat(cDesstr,p,iWordLength + 1);//需要将空格(或者非英文字母)也拷贝进去
            iCopyNum   += (iWordLength + 1);//将已经拷贝的字符个数进行累加,包括空格(或者非英文字母)
            iCopyWidth += (iWordWidth + GetCharactorWidth(hdc,pNextSpace));//将宽度累加

            p += (iWordLength + 1);
        }
        else if(iRectWidth - iCopyWidth == iWordWidth)//剩余长度等于一个英文单词的宽度,直接拷贝,然后添加换行符
        {
            strncat(cDesstr,p,iWordLength + 1);//需要将空格(或者非英文字母)也拷贝进去
            strcat(cDesstr,"\n");

            iCopyNum += (iWordLength + 1);//将已经拷贝的字符个数进行累加,包括空格(或者非英文字母)
            iRowNum++;
            iCopyWidth = 0;

            p += (iWordLength + 1);
        }
        else//剩余长度小于一个英文单词的宽度,直接添加换行符
        {
            if (iCopyNum == 0)
            {
                //一个字符也没有拷贝就添加换行符,则这个单词的长度已经超过
                //了矩形区域的宽度,直接拷贝源字符串,然后返回
                strcpy(cDesstr,cSourstr);
                return (strlen(cSourstr));
            }
            strcat(cDesstr,"\n");
            iRowNum++;
            iCopyWidth = 0;
        }
    }
    strcat(cDesstr,"\0");

    return (iRowNum);
}
int     GetCharactorWidth(HDC hdc,char * ch)//获得单个字符或者单个汉字的宽度
{
    SIZE size;

    if ((ch == NULL) || (* ch == NULL))
        return 0;

    if ((unsigned char)(* ch) > 0x80)//当前字符是汉字
        GetTextExtent(hdc,ch,2,&size);
    else
        GetTextExtent(hdc,ch,1,&size);

    return (size.cx);
}
///////////////////////////////以上部分为自动换行所需函数--end
//////////////////////////以下为声音播放部分begin//////////////////////////////////////////
#if defined(CAPITEL)
/******************************************************************
* Function   Setup_MusicPlay  
* Purpose    播放音乐文件
* Params     pFileName  ： 播放的文件名
              Volume     :  播放时的音量
              PlayMode   ：播放模式
              0: 循环播放
              !=0: 播放次数 (<255)             
* Return        ！=0     -播放成功，SMAF序列句柄。
                =0     -播放失败。
* Remarks      
**********************************************************************/
int     Setup_MusicPlay(char * pFileName,UINT Volume,int PlayMode)
{
    char        * p = NULL;
    int         iType = 0;
    int         iresult = 1;
    JMUSICINFO  jmusicinfo;
    
    p = strstr(pFileName,".");//没有找到"."即非法的文件名
    if (p == NULL)
        return 0;

    if (stricmp(p,".MIDI") == 0)
        iType = MIDI_TYPE;
    else if (stricmp(p,".WAV") == 0)
        iType = WAVE_TYPE;
    else if (stricmp(p,".MMF") == 0)
        iType = MMF_TYPE;
    else
        return 0;

    jmusicinfo.pBuffer              = 0;
    jmusicinfo.ulBufferSize         = 0;
    jmusicinfo.usMusicType          = iType;
    jmusicinfo.usPlayModeAndCount   = ((!PlayMode) ? LOOP_PLAY : COUNT_PLAY) | PlayMode;
    jmusicinfo.uiEventMask          = 0;
    jmusicinfo.pPlayMusicCallBack   = 0;
    jmusicinfo.ulPriority           = BELOWNORMAL;
    jmusicinfo.pFileName            = pFileName;

    SetupHf_Volume(Volume);//设置音量
#ifndef _EMULATE_
    DHI_PlayMusic(&jmusicinfo);
#endif
    return (iresult);
}
/********************************************************************
* Function   SETUP_PlayMusic  
* Purpose    播放音乐
* Params     seqHd 由PlayMusic函数返回的文件句柄
* Return     
* Remarks      
**********************************************************************/
BOOL    SETUP_PlayMusic(pJSETUPMUSICINFO pSetupMusicInfo)
{
    SCENEMODE           sm;//获得情景模式信息
    RINGSETTING         rs;//获得铃声信息
    JMUSICINFO          jmusicinfo;
    char                ringname[RINGNAMEMAXLEN + 1] = "";//保存铃声文件名
    char                cPhoneNum[43] = "";//保存当前电话号码
    char                * p = NULL;
    int                 iType = 0,iVolume = GetMaxRingVolume();
    unsigned long       ulPriority;//声音的优先级
    unsigned short      sPlayModeAndCount;//播放模式和播放次数
    //由于有些铃声的播放次数必须为一次,比如按键音,接通提示音,所以需要在函数里规范播放次数

    memset(&sm          ,0,sizeof(SCENEMODE  ));
    memset(&rs          ,0,sizeof(RINGSETTING));
    memset(&jmusicinfo  ,0,sizeof(JMUSICINFO ));
    
    GetRingSet(&rs);
    GetSM(&sm,GetCurSceneMode());

    sPlayModeAndCount = pSetupMusicInfo->usPlayModeAndCount;

    switch (pSetupMusicInfo->uiRingSelect)
    {
    case RING_PHONE://来电
        App_GetCurPhoneNum(cPhoneNum);//获得当前电话号码
        if (!APP_PBGetCurRingName(cPhoneNum,ringname))//根据当前电话号码获得当前铃声
        {//该函数返回FALSE表示没有找到该电话号码对应的组,这时候使用一个默认的铃声
            GetDefaultRing(ringname);
        }
        ulPriority = HIGH;
        switch (sm.iComeview)
        {
        case 0://铃声
            iVolume = sm.iRingvolume;   //设置音量
            break;
        case 1://振动
            SetupHf_Vibration(TRUE);
            return TRUE;
            break;
        case 2://振动铃声
            iVolume = sm.iRingvolume;   //设置音量
            SetupHf_Vibration(TRUE);
            break;
        case 3://静音
            return TRUE;
            break;
        case 4://先振动(5秒)后响铃
            SetupHf_Vibration(TRUE);
            SM_LibrateRingBegin();
            return TRUE;
            break;
        }
        break;
    case RING_SMS://短信提示音
    case RING_MMS://彩信提示音
        if (sm.iInfoClew == 0)//关闭
            return TRUE;

        strcpy(ringname,rs.cRingsetSMS);
        sPlayModeAndCount   = COUNT_PLAY | 1;
        iVolume             = sm.iRingvolume;   //设置音量
        ulPriority          = ABOVENORMAL;
        BeginTime();

        break;
    case RING_POWNON://开机铃声
        strcpy(ringname,rs.cRingsetOpen);
        ulPriority = ABOVENORMAL;
        break;
    case RING_CAM://照相提示音
        strcpy(ringname,"ROM:snap4.mmf");
        sPlayModeAndCount   = COUNT_PLAY | 1;
        ulPriority          = NORMAL;
        break;
    case RING_KEY://按键音提示音
    case RING_SCREEN://触屏音
        if (sm.iKeypress == 0)//关闭
            return TRUE;

        strcpy(ringname,"ROM:ring20.mmf");
        sPlayModeAndCount   = COUNT_PLAY | 1;
        iVolume             = sm.iKeypress;
        ulPriority          = LOW;
        break;
    case RING_TOUCH://接通提示音
        if (sm.iGet == 0)//关闭
            return TRUE;
        strcpy(ringname,"ROM:ring22.mmf");
        sPlayModeAndCount   = COUNT_PLAY | 1;
        iVolume             = sm.iGet;
        ulPriority          = BELOWNORMAL;
        break;
    case RING_ALARM://日程表闹铃
        strcpy(ringname,rs.cRingsetAlarm);
        ulPriority = ABOVENORMAL;
        break;
    case RING_CLEW50://50秒提示音
        strcpy(ringname,"ROM:ring22.mmf");
        sPlayModeAndCount   = COUNT_PLAY | 1;
        ulPriority          = NORMAL;
        break;
    case RING_WARNING://警告音
        if (sm.iWarning == 0)//关闭
            return TRUE;

        strcpy(ringname,"ROM:ring22.mmf");
        sPlayModeAndCount   = COUNT_PLAY | 1;
        ulPriority          = BELOWNORMAL;
        break;
    default:
        return FALSE;
        break;
    }
    
    p = strstr(ringname,".");
    if (p == NULL)//没有找到"."即非法的文件名
        return FALSE;

    if (stricmp(p,".MIDI") == 0)
        iType = MIDI_TYPE;
    else if (stricmp(p,".WAV") == 0)
        iType = WAVE_TYPE;
    else if (stricmp(p,".MMF") == 0)
        iType = MMF_TYPE;
    else
        return FALSE;

    jmusicinfo.pBuffer              = 0;
    jmusicinfo.ulBufferSize         = 0;
    jmusicinfo.usMusicType          = iType;
    jmusicinfo.uiEventMask          = pSetupMusicInfo->uiEventMask;
    jmusicinfo.usPlayModeAndCount   = sPlayModeAndCount;
    jmusicinfo.ulPriority           = ulPriority;
    jmusicinfo.pPlayMusicCallBack   = pSetupMusicInfo->pPlayMusicCallBack;
    jmusicinfo.pFileName            = ringname;

    SetupHf_Volume(iVolume);//设置音量
#ifndef _EMULATE_
    DHI_PlayMusic(&jmusicinfo);
#endif
    return TRUE;
}
void    TouchAlert ()//触屏音
{
    JSETUPMUSICINFO jsetupmusicinfo;
    jsetupmusicinfo.uiRingSelect        = RING_SCREEN;
    jsetupmusicinfo.usPlayModeAndCount  = 1 | COUNT_PLAY;
    jsetupmusicinfo.pPlayMusicCallBack  = 0;
    jsetupmusicinfo.uiEventMask         = 0;

    SETUP_PlayMusic(&jsetupmusicinfo);
}
#elif defined(_PHOSPHOR_)
extern  int     GetMaxRingVolume();//获得最大音量
extern  int     SetupHf_PlayMusicInit(SetupHS_PlayMusic * playmusic);
extern  int     SetupHf_PlayAMR();
extern  int     SetupHf_PlayWAVE();
extern  int     SetupHf_PlayMIDI();//播放midi文件
extern  int     SetupHF_EndPlayMusic(void);//停止播放音乐
/******************************************************************
* Function   SETUP_PlayMusic  
* Purpose    播放音乐文件(用于彩信,浏览器)
* Params     
             hWnd:      调用方的窗口句柄
             iWM_MMWRITE:调用方定义的消息,调用方必须在该消息中调用Setup_WriteMusicData函数
            推荐这样调用:Setup_WriteMusicData((LPWAVEHDR)lParam);
            iWM_MMSTOP: 调用方定义的消息,用于通知调用方铃声播放完毕,调用方必须在该消息中调用Setup_EndPlayMusic()函数
            当铃声播放完毕的时候,iWM_MMSTOP消息中的wParam=PLAY_OVER表示音乐播放完毕,而且是正常结束
            pBuffer:数据区域,
            ulBufferSize:数据区域的长度
* Return        ！=0     -播放成功
                =0     -播放失败。
* Remarks      
**********************************************************************/
int     SETUP_PlayMusic(SetupHS_PlayMusic * hs)
{
    unsigned char *     p = NULL;
    unsigned char       strAudioType[5] = "";//用于数据缓冲区中数据类型的判断
    SCENEMODE           sm;//获得情景模式信息
    int                 ires = 0,i;
    SetupHS_PlayMusic   setuphs_playmusic;

    memcpy(&setuphs_playmusic,hs,sizeof(SetupHS_PlayMusic));
    
    GetSM(&sm,GetCurSceneMode());
    switch (setuphs_playmusic.uiringselect)
    {
    case RING_PHONE://来电提示音
        setuphs_playmusic.iVolume       = sm.iRingvolume;//设置铃声音量
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_HIGH;
        break;
    case RING_POWNON:   //开机铃声
    case RING_POWNOFF:  //关机音乐
    case RING_SMS:      //短信
    case RING_MMS:      //彩信
        setuphs_playmusic.iVolume       = GetMaxRingVolume();//其他应用使用最大音量
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_ABOVENORMAL;
        break;
    case RING_KEY:      //按键音
    case RING_TOUCH:    //接通提示音
    case RING_SCREEN:   //触屏音
    case RING_CLEW50:   //50秒提示音
        setuphs_playmusic.iVolume       = GetMaxRingVolume();//其他应用使用最大音量
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_LOW;//这些类别的铃声的优先级应该为最低
        break;
    case RING_OTHER:    //播放其它声音,比如彩信,浏览器中的声音文件,铃声预览等一般应用
        setuphs_playmusic.iVolume       = GetMaxRingVolume();//其他应用使用最大音量
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_NORMAL;
        break;
    case RING_VOLUME://测试音量用宏,音量由参数iVolume决定
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_NORMAL;
        break;
    default:
        ShowDebugInfo("!!!非法的声音类型,无法播放","SETUP_PlayMusic",setuphs_playmusic.uiringselect);
        return 0;
        break;
    }

    if ((setuphs_playmusic.pBuffer != NULL) && (setuphs_playmusic.ulBufferSize != 0))//根据数据缓冲区进行声音播放
    {
        setuphs_playmusic.pFileName = NULL; //将文件名指针赋空

        p = setuphs_playmusic.pBuffer;
        for (i = 0;i < 5;i++)
        {
            strAudioType[i] = * p++;
        }
        if ((strAudioType[0] == 0x23) && (strAudioType[1] == 0x21) && 
            (strAudioType[2] == 'A' ) && (strAudioType[3] == 'M' ) &&
            (strAudioType[4] == 'R'))
            setuphs_playmusic.iType = AMR_TYPE;
        else if ((strAudioType[0] == 'R') && (strAudioType[1] == 'I') && 
                 (strAudioType[2] == 'F') && (strAudioType[3] == 'F'))
            setuphs_playmusic.iType = WAVE_TYPE;
        else if ((strAudioType[0] == 'M') && (strAudioType[1] == 'T') && 
                 (strAudioType[2] == 'h') && (strAudioType[3] == 'd'))
            setuphs_playmusic.iType = MIDI_TYPE;
        else if ((strAudioType[0] == 'M') && (strAudioType[1] == 'M') &&
                 (strAudioType[2] == 'M') && (strAudioType[3] == 'D'))
            setuphs_playmusic.iType = MMF_TYPE;
        else
        {
            ShowDebugInfo("!!!非法的数据缓冲区,无法播放","SETUP_PlayMusic",1063);
            return (0);
        }
    }
    else if (strlen(setuphs_playmusic.pFileName))//按文件进行播放
    {
        p = strstr(setuphs_playmusic.pFileName,".");//考察文件扩展名
        if (p != NULL)
        {
            if (stricmp(p,".amr") == 0)//获得文件类型
                setuphs_playmusic.iType = AMR_TYPE;
            else if (stricmp(p,".wav") == 0)
                setuphs_playmusic.iType = WAVE_TYPE;
            else if ((stricmp(p,".midi") == 0) || (stricmp(p,".mid") == 0))
                setuphs_playmusic.iType = MIDI_TYPE;
            else if (stricmp(p,".mmf") == 0)
                setuphs_playmusic.iType = MMF_TYPE;
            else if (stricmp(p,".tmp") == 0)//如果扩展名是tmp,文件类型由外边决定
            {}
            else
            {
                ShowDebugInfo("!!!非法的声音类型,无法播放",setuphs_playmusic.pFileName,1084);
                return 0;
            }
        }
        else
        {
            ShowDebugInfo("!!!非法的声音文件,无法播放",setuphs_playmusic.pFileName,1090);
            return 0;
        }
        setuphs_playmusic.pBuffer       = NULL; //将数据缓冲区的指针清空
        setuphs_playmusic.ulBufferSize  = 0;    //将数据缓冲区的长度清零
    }
    else
    {
        ShowDebugInfo("!!!非法的声音参数,无法播放","SETUP_PlayMusic",1084);
        return 0;
    }
    if (!SetupHf_PlayMusicInit(&setuphs_playmusic))
    {
        ShowDebugInfo("!!!初始化失败.","SETUP_PlayMusic",1103);
        return 0;
    }
    switch (setuphs_playmusic.iType)
    {
    case AMR_TYPE:
        ires = SetupHf_PlayAMR();
        break;
    case WAVE_TYPE:
        ires = SetupHf_PlayWAVE();
        break;
    case MIDI_TYPE:
        ires = SetupHf_PlayMIDI();
        break;
    default:
        return (ires);
    }
    return (ires);
}
static  HWND        hWndApp;
static  char        cRingName[RINGNAMEMAXLEN + 1] = ""; //铃声文件名
static  int         iCallCount;                         //播放次数,0为循环播放,非零为播放次数

#define     CLASSNAME_PLAYMUSIC     "PlayMusicWndClass" //窗口类名
#define     WM_MMWRITE              100
#define     WM_MMSTOP               110
#define     WM_MMGETDATA            115
#define     IDC_PLAYMUSIC           120
/********************************************************************
* Function   Call_PlayMusic  
* Purpose    播放声音接口
* Params     uiringselect:播放类别
            icount:播放次数,0为循环播放,非零为播放次数
* Return     
* Remarks     
**********************************************************************/
BOOL    Call_PlayMusic(int uiringselect,int icount)
{
    SCENEMODE   sm;//获得情景模式信息
    
    GetSM(&sm,GetCurSceneMode());           //获得情景模式信息
    memset(cRingName,0,sizeof(cRingName));

    switch (uiringselect)
    {
    case RING_PHONE://来电
        switch (sm.iComeview)
        {
        case 0://铃声
            strcpy(cRingName,sm.rRingselect.cPhone);
            break;
        case 1://振动
            SetupHf_Vibration(TRUE);
            return TRUE;
            break;
        case 2://振动铃声
            strcpy(cRingName,sm.rRingselect.cPhone);
            SetupHf_Vibration(TRUE);
            break;
        case 3://静音
            return TRUE;
            break;
        default:
            return FALSE;
        }
        break;
    case RING_SMS://短信提示音
        switch (sm.iComeview)
        {
        case 0://铃声
            strcpy(cRingName,sm.rRingselect.cShortInfo);
            break;
        case 1://振动
            BeginVibration(1);//振动一秒
            break;
        case 2://振动铃声
            BeginVibration(1);//振动一秒
            strcpy(cRingName,sm.rRingselect.cShortInfo);
            break;
        case 3://静音
            return TRUE;
            break;
        default:
            return FALSE;
        }
        break;
    case RING_POWNON://开机铃声
        GetOpenRing(cRingName);
        break;
    case RING_MMS://彩信提示音
        strcpy(cRingName,sm.rRingselect.cMMS);
        break;
    case RING_KEY://按键音提示音
        strcpy(cRingName,sm.cKey);
        break;
    case RING_TOUCH://接通提示音
        strcpy(cRingName,"/rom/prefab/audio/key1.wav");
        break;
    case RING_SCREEN://触屏音
        strcpy(cRingName,sm.cTouch);
        break;
    case RING_POWNOFF://关机音乐
        GetCloseRing(cRingName);
        break;
    case RING_CLEW50://50秒提示音
        strcpy(cRingName,"/rom/prefab/audio/key1.wav");
        break;
    default:
        return FALSE;
        break;
    }
    iCallCount = abs(icount);
    //因为参数意义为0为循环播放,非零为播放次数,为了内部播放次数计数的方便
    //在内部将循环播放标志为-1,当播放一次的时候iCallCount--,当iCallCount减为零的时候
    //停止播放.
    if (!iCallCount)
        iCallCount = -1;
    else
        iCallCount--;//表示第一次播放

    PostMessage(hWndApp,IDC_PLAYMUSIC,uiringselect,0);//将声音类别用wParam传入

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
static  LRESULT AppWndProc_PlayMusic ( HWND hWnd, UINT wMsgCmd, WPARAM wParam, LPARAM lParam );
BOOL    CreateWindow_PlayMusic()//创建一个隐藏窗口用于播放声音,该函数是系统启动的时候创建
{
    HWND        hwnd;
    WNDCLASS    wc;

    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = AppWndProc_PlayMusic;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = NULL;
    wc.hIcon         = NULL;
    wc.hCursor       = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = CLASSNAME_PLAYMUSIC;

    if( !RegisterClass(&wc) )
    {
        UnregisterClass(CLASSNAME_PLAYMUSIC,NULL);
        return FALSE;
    }
    hwnd = CreateWindow(CLASSNAME_PLAYMUSIC,"",WS_CAPTION|WS_BORDER |PWS_STATICBAR, 
        HP_WIN_POSITION , NULL, NULL, NULL, NULL);

    if (!hwnd)
        return FALSE;

    hWndApp = hwnd;

    return (TRUE);
}
static LRESULT AppWndProc_PlayMusic( HWND hWnd, UINT wMsgCmd, WPARAM wParam, LPARAM lParam )
{
            LRESULT             lResult;
    static  SetupHS_PlayMusic   hs;

    lResult = (LRESULT)TRUE;
    switch ( wMsgCmd )
    {
    case WM_CREATE :
        hs.hWnd             = hWnd;
        hs.iWM_MMWRITE      = WM_MMWRITE;
        hs.iWM_MMSTOP       = WM_MMSTOP;
        hs.iWM_MMGETDATA    = WM_MMGETDATA;
        hs.iVolume          = 0;
        hs.iType            = 0;
        hs.uiringselect     = RING_WITHOUT;
        hs.pFileName        = cRingName;
        hs.pBuffer          = NULL;
        hs.ulBufferSize     = 0;
        break;
    case WM_MMWRITE://该消息用于播放声音
        Setup_WriteMusicData((LPWAVEHDR)lParam);
        break;
    case WM_MMGETDATA:
        Setup_GetMusicData((LPWAVEHDR)lParam);
        break;
    case WM_MMSTOP://铃声播放停止表示主动停止铃声
        Setup_EndPlayMusic(0);
        
        if (iCallCount == -1)//循环播放
            PostMessage(hWnd,IDC_PLAYMUSIC,hs.uiringselect,0);
        else if (iCallCount > 0)
        {
            PostMessage(hWnd,IDC_PLAYMUSIC,hs.uiringselect,0);
            iCallCount--;//播放次数减一
        }
        else if (iCallCount == 0)//该播的次数已经播完了
            return TRUE;

        break; 
    case IDC_PLAYMUSIC:
        hs.uiringselect = wParam;
        SETUP_PlayMusic(&hs);
        break;
    default :
        lResult = PDADefWindowProc(hWnd, wMsgCmd, wParam, lParam);
        break;
    }
    return lResult;
}
/********************************************************************
* Function   Call_EndPlayMusic  
* Purpose    声音停止接口函数,用户主动调用这个接口停止音乐
* Params     
* Return     
* Remarks     
**********************************************************************/
BOOL    Call_EndPlayMusic(int uiringselect)
{
    BOOL bres = FALSE;

    bres = Setup_EndPlayMusic(uiringselect);
    SetupHf_Vibration(FALSE);//停止震动

    return (bres);
}
#endif
//////////////////////////////////////////////////////////////////////////
#define     CLASSNAME_SETUP             "CreateWindow_SetupWndClass"    //窗口类名
#define     ID_TIMER_LIMITMUSICPLAY     7
#define     ID_TIMER_LIBRATETHENRING    8//先振动后响铃的timer
#define     ID_TIMER_VIBRATION          9//用于震动的计时器
static  HINSTANCE           hInstance;
static  HWND                hWndApp_setup = NULL;

static  LRESULT AppWndProc_Setup( HWND hWnd, UINT wMsgCmd, WPARAM wParam, LPARAM lParam );
BOOL    CreateWindow_Setup(void)//创建一个隐藏窗口用于震动一定时间自动停止,该窗口是系统启动的时候创建
{
    HWND        hwnd;
    WNDCLASS    wc;

    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = AppWndProc_Setup;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = NULL;
    wc.hIcon         = NULL;
    wc.hCursor       = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = CLASSNAME_SETUP;

    if( !RegisterClass(&wc) )
    {
        UnregisterClass(CLASSNAME_SETUP,NULL);
        SetupHf_Vibration(FALSE);//停止震动
        return FALSE;
    }
    hwnd = CreateWindow(CLASSNAME_SETUP,"",WS_CAPTION|WS_BORDER |PWS_STATICBAR,
        HP_WIN_POSITION , NULL, NULL, NULL, NULL);

    if (!hwnd)
    {
        SetupHf_Vibration(FALSE);//停止震动
        return FALSE;
    }
    hWndApp_setup = hwnd;
    return (TRUE);
}
static LRESULT AppWndProc_Setup( HWND hWnd, UINT wMsgCmd, WPARAM wParam, LPARAM lParam )
{
    LRESULT     lResult;
    char        cPhoneNum[43] = "";//保存当前电话号码
    char        ringname[RINGNAMEMAXLEN + 1] = "";//保存铃声文件名
    SCENEMODE   sm;

    lResult = (LRESULT)TRUE;
    
    memset(&sm,0x00,sizeof(SCENEMODE));
    switch ( wMsgCmd )
    {
    case WM_TIMER:
        switch(wParam)
        {
        case ID_TIMER_LIMITMUSICPLAY://限定声音播放的时间
            Setup_EndPlayMusic(0);//停止音乐
            KillTimer(hWndApp_setup,ID_TIMER_LIMITMUSICPLAY);
            break;
#if defined(CAPITEL)
        case ID_TIMER_LIBRATETHENRING://开始先振动(5秒)后响铃
            SetupHf_Vibration(FALSE);//停止震动
            KillTimer(hWnd,ID_TIMER_LIBRATETHENRING);
            //开始播放音乐
            App_GetCurPhoneNum(cPhoneNum);//获得当前电话号码
            if (!APP_PBGetCurRingName(cPhoneNum,ringname))//根据当前电话号码获得当前铃声
            {//该函数返回FALSE表示没有找到该电话号码对应的组,这时候使用一个默认的铃声
                GetDefaultRing(ringname);
            }
            GetSM(&sm,GetCurSceneMode());
            Setup_MusicPlay(ringname,sm.iRingvolume,0);
            break;
#endif
        case ID_TIMER_VIBRATION://用于震动一定时间自动停止
            SetupHf_Vibration(FALSE);//停止震动
            KillTimer(hWnd,ID_TIMER_VIBRATION);
            break;
        }
        break;
    case WM_DESTROY:
        //通知程序管理器,此程序退出;
        UnregisterClass(CLASSNAME_SETUP,NULL);
        DlmNotify ((WPARAM)PES_STCQUIT, (LPARAM)hInstance );
        break;
    default :
        lResult = PDADefWindowProc(hWnd, wMsgCmd, wParam, lParam);
        break;
    }
    return lResult;
}

void    SM_LibrateRingBegin(void)//开始先振动(5秒)后响铃
{
    SetTimer(hWndApp_setup,ID_TIMER_LIBRATETHENRING,5000,NULL);
}

void    SM_LibrateRingEnd(void)//结束先振动后响铃的功能,主要是删除定时器,适用于主动挂断
{
    KillTimer(hWndApp_setup,ID_TIMER_LIBRATETHENRING);
}

void    BeginTime(void)//限定声音播放的时间,在这里启动计时器
{
    SetTimer(hWndApp_setup,ID_TIMER_LIMITMUSICPLAY,2000,NULL);
}

void    BeginVibration(int iSecond)//实现振动iSecond秒
{
    SetTimer(hWndApp_setup,ID_TIMER_VIBRATION,iSecond * 1000,NULL);
    SetupHf_Vibration(TRUE);
}
//////////////////////////以上为声音播放部分end//////////////////////////////////////////
//////////////////////////以下部分为设置部分定义的硬件接口函数////////////////////////////////////////////////
//CAPITEL 首信项目部分详见hp_dhi.h
/********************************************************************
* Function   Setup_EndPlayMusic  
* Purpose    停止播放音乐
* Params     
* Return     
* Remarks      
**********************************************************************/
int     Setup_EndPlayMusic(int uiringselect)
{
    int         ires = 1;
#if defined(CAPITEL)
    SCENEMODE   sm;
    SetupHf_Vibration(FALSE);   //停止震动

    GetSM(&sm,GetCurSceneMode());
    if (sm.iComeview == 4)//先振动(5秒)后响铃
        SM_LibrateRingEnd();

#ifndef _EMULATE_
    DHI_StopMusic();
#endif
#elif defined(_PHOSPHOR_)
    ires = SetupHF_EndPlayMusic();
#endif
    return(ires);
}
/********************************************************************
* Function   SetupHf_Volume  
* Purpose    设置音量
* Params     
* Return     
* Remarks    启明星项目不支持在此设置音量
**********************************************************************/
BOOL    SetupHf_Volume(int iVolume)
{
    BOOL res = TRUE;
#ifndef _EMULATE_
#if defined(CAPITEL)
    res = DHI_SetMasterVolume(iVolume);
#elif defined(_PHOSPHOR_)
    res = FALSE;
#endif
#endif
     return (res);
}
/********************************************************************
* Function   SetupHf_Ledlight  
* Purpose    七彩灯颜色控制
* Params     Color为要设定的颜色值，0~7共8种，不在此范围内则直接返回。
            0为关灯，1为红灯，2绿灯，3蓝灯，4红绿灯，5绿蓝灯，6红蓝灯，7红绿蓝灯（白灯）
        
* Return     
* Remarks      
**********************************************************************/
void    SetupHf_Ledlight(unsigned int color)
{
#ifndef _EMULATE_
#if defined(CAPITEL)
    DHI_SetLedColor(color);
#endif
#endif
}
/********************************************************************
* Function   SetupHf_Keylight  
* Purpose    开关键盘的背景灯
* Params     
* Return     
* Remarks      
**********************************************************************/
void    SetupHf_Keylight(BOOL bEn)
{
#ifndef _EMULATE_
#if defined(CAPITEL)
    DHI_EnableKeyBkLight(bEn);
#endif
#endif
}
/********************************************************************
* Function   SetupHf_EnabledLcd  
* Purpose    
* Params     开关屏幕
* Return     
* Remarks      
**********************************************************************/
void    SetupHf_EnabledLcd(unsigned int uiLcdNo,BOOL bEn)
{
#ifndef _EMULATE_
#if defined(CAPITEL)
    DHI_EnableLcd(uiLcdNo,bEn);
#endif
#endif
}
/********************************************************************
* Function   SetupHf_LcdBklight  
* Purpose    控制LCD的背光灯的开关。
* Params     uiLcdNo指出了LCD号;
            bEn：为TRUE时，点亮指定LCD的背景灯，为FALSE时，关闭指定LCD的背景灯
* Return     
* Remarks      
**********************************************************************/
extern  void    SetupHf_LcdBklight_ioctl(BOOL bEn);
void    SetupHf_LcdBklight(unsigned int uiLcdNo,BOOL bEn)
{
#ifndef _EMULATE_
#if defined(CAPITEL)
    DHI_EnableLcdBkLight(uiLcdNo,bEn);
#elif defined(_PHOSPHOR_)
    SetupHf_LcdBklight_ioctl(bEn);
#endif
#endif
}
/********************************************************************
* Function   SetupHf_Contrast  
* Purpose    设置对比度
* Params     
* Return     
* Remarks      
**********************************************************************/
void    SetupHf_Contrast(unsigned int uiLcdNo,int icontrast)
{
    int iMin = GetContrastMin(),iMax = GetContrastMax();
    if ((icontrast < iMin) || (icontrast > iMax))
        icontrast = (iMax - iMin + 1) / 2;
#ifndef _EMULATE_
#if defined(CAPITEL)
    DHI_SetLcdContrast(uiLcdNo,icontrast - 1);//0-7
#elif defined(_PHOSPHOR_)
    
#endif
#endif
}
/********************************************************************
* Function   SetupHf_BgLight  
* Purpose    设置亮度
* Params     
* Return     
* Remarks      
**********************************************************************/
extern  void    SetupHf_BgLight_ioctl(int iBgLight);//亮度
void    SetupHf_BgLight(int iBgLight)
{
#if defined(_PHOSPHOR_)
    SetupHf_BgLight_ioctl(iBgLight);
#endif
}
/********************************************************************
* Function   SetupHf_Vibration  
* Purpose    打开/关闭震动
* Params     
* Return     
* Remarks      
**********************************************************************/
extern  void    SetupHf_Vibration_ioctl(BOOL bEn);
void    SetupHf_Vibration(BOOL bEn)
{
#ifndef _EMULATE_
#if defined(CAPITEL)
    DHI_EnableVibration(bEn);
#elif defined(_PHOSPHOR_)
    SetupHf_Vibration_ioctl(bEn);
#endif
#endif
}
//////////////////////////////////////////////////////////////////////////
/********************************************************************
* Function   SetupHf_Sleep  
* Purpose    
* Params     睡眠控制
* Return     
* Remarks      
**********************************************************************/
void    SetupHf_Sleep(BOOL bSleep)
{
#ifndef _EMULATE_
#if defined(CAPITEL)
    //DHI_Sleep(bSleep);
#elif defined(_PHOSPHOR_)

#endif
#endif
}
/********************************************************************
* Function   SetupHf_PowerDown  
* Purpose    
* Params     电源关断
* Return     
* Remarks      
**********************************************************************/
void    SetupHf_PowerDown()
{
#ifndef _EMULATE_
#if defined(CAPITEL)
    //DHI_PowerDown();
#elif defined(_PHOSPHOR_)

#endif
#endif
}
///////////////////////////显示提示信息///////////////////////////////////////////////
extern  void    printf(const char *, ...);
void    ShowDebugInfo(char * pInfo,char * pOther,long iOther)
{
#if defined(_DEBUG)
    printf("%s\n",pInfo);
    if (pOther != NULL)
        printf("%s\n",pOther);
    if (iOther != 0)
        printf("%d\n",iOther);
#endif
}