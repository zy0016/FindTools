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
#include    "hp_dhi.h"//Ӳ������������ļ�
#endif

#ifndef MAIN_LCD
#define MAIN_LCD                1// LCD���
#endif

#define     SIMCLEW1            GetString(STR_SETUP_SIM_CLEW1)//"δ����SIM��"
#define     SIMCLEW3            GetString(STR_SETUP_SIM_CLEW3)//"��ʾ"

extern  int     GetSIMState();
extern  int     GetCurrentLanguage();               //�ӱ����л�õ�ǰ����ID��
extern  int     GetLanguage();                      //���ļ��л�õ�ǰ����ID��(ֻ�ڳ�ʼ������Setup_InitFunction��ִ��һ��)
extern  BOOL    CallAutoCloseClewWindow(void);      //�Զ��ػ�����
extern  void    GetDefaultRing(char * pRingName);   //���Ĭ�ϵ�����
extern  int     GetMaxRingVolume(void);             //��������������
extern  void    App_GetCurPhoneNum(char *pPhoneNum);//��õ�ǰ�绰����
extern  BOOL    APP_PBGetCurRingName(char* cPhoneNum, char* ringname);//���ݵ�ǰ�绰����,��õ�ǰ����

extern  int     GetBg_keyboard();       //��ü��̱������Ϣ������ֵ��ʾ����
extern  int     GetBg_screen();         //�����Ļ�������Ϣ����0��ʾ"�ر�",����Ϊ����
extern  int     GetContrast();          //��öԱȶ�ֵ
extern  int     GetContrastMax();       //��öԱȶ����ֵ
extern  int     GetContrastMin();       //��öԱȶ���Сֵ
extern  void    SetCurrentColorCase();  //���õ�ǰ��ɫ�ʷ���

#if defined(_PHOSPHOR_)
extern  int     GetBglightValue();      //�������
extern  BOOL    Setup_ColorInit();      //��ɫ���ó�ʼ��
        BOOL    CreateWindow_PlayMusic();//����һ�����ش������ڲ�������,�ú�����ϵͳ������ʱ�򴴽�
extern  void    Switch_init(void);      //�л�����
#endif

//////////////////////////���غ���////////////////////////////////////////////////
void    BeginTime(void);//�޶��������ŵ�ʱ��
void    SM_LibrateRingBegin(void);//ʵ������(5��)������Ĺ���
void    SM_LibrateRingEnd(void);//�������񶯺�����Ĺ���,��Ҫ��ɾ����ʱ��
BOOL    CreateWindow_Setup(void);//ϵͳ������ʱ�򴴽�һ�����ش�������ֹͣ��
void    BeginVibration(int iSecond);//ʵ����iSecond��

void    ShowDebugInfo(char * pInfo,char * pOther,long iOther);//��ʾ������Ϣ
/////////////////////////�Զ����к���/////////////////////////////////////////////////
int     AutoNewLine(HDC hdc,char * cSourstr,char * cDesstr,RECT * rect,UINT uFormat);
int     AutoNewLine_chn(HDC hdc,char * cSourstr,char * cDesstr,int iRectWidth);//��������
int     AutoNewLine_eng(HDC hdc,char * cSourstr,char * cDesstr,int iRectWidth);//��������
int     GetCharactorWidth(HDC hdc,char * ch);//��õ����ַ����ߵ������ֵĿ��
////////////////////////���ò��ְ���Ӳ������//////////////////////////////////////////////////
void    SetupHf_LcdBklight(unsigned int uiLcdNo,BOOL bEn);
void    SetupHf_Keylight(BOOL bEn);
void    SetupHf_Contrast(unsigned int uiLcdNo,int icontrast);
void    SetupHf_BgLight(int iBgLight);
void    SetupHf_Vibration(BOOL bEn);
BOOL    SetupHf_Volume(int iVolume);
int     Setup_EndPlayMusic(int uiringselect);
/********************************************************************
* Function   IfFileExist  
* Purpose    ���鵱ǰ�ļ��Ƿ����
* Params     pFileName:�ļ�������·��
* Return     ����ture,������false
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
    if(INVALID_HANDLE_VALUE == hf)//δ�ҵ��ļ�
    {   
        CloseFile(hf);
        return FALSE;
    }
    CloseFile(hf);
    return TRUE;
}
/********************************************************************
* Function   CountControlYaxis  
* Purpose    ����ؼ�������ĺ��� 
* Params     piConY:���ڷ��ؿؼ���������,iControlH:�����ؼ��ĸ�
            iControlNum:�ؼ��ĸ���
* Return     
* Remarks    Ҫ������ؼ��ڽ����ϴ��ϵ�������,���Ҹ����ؼ��ļ����һ�µ�
**********************************************************************/
void    CountControlYaxis(int * piConY,int iControlH,int iControlNum)
{
    int i,iyStep;

    iyStep = (GetSystemUsableWH(SM_CYSCREEN) - iControlNum * iControlH) / (iControlNum + 1);//����ÿ���ؼ��ļ��
    for (i = 0;i < iControlNum;i++)
        * piConY++ = iyStep + i * (iControlH + iyStep);//����ؼ���������
}
/********************************************************************
* Function   FindSIMState  
* Purpose     ���SIM����״̬.
* Params     bShow:�Ƿ���ʾ��ǰ״̬��ʾ
* Return     ��ǰ״̬���
* Remarks      
**********************************************************************/
int     FindSIMState(BOOL bShow)
{
    int istate = 1;
    istate = GetSIMState();
    if (bShow)
    {
        if(!istate)
            MsgBox(NULL,SIMCLEW1,SIMCLEW3,WAITTIMEOUT);//δ����SIM��
    }
    return (istate);
}
//////////////////////////////////////////////////////////////////////////
void    GetSystemUsableXY(int * width,int * height)//�����Ļ�п������õĿ�Ⱥ͸߶�
{
    * width  = GetSystemUsableWH(SM_CXSCREEN);
    * height = GetSystemUsableWH(SM_CYSCREEN);
}
int     GetSystemUsableWH(int iwh)//�����Ļ�п������õĿ�Ȼ��߸߶�
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
* Purpose    ����**P�е�λͼ
* Params     ** pλͼ�ļ�����ָ������
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
* Purpose    ����**P�е�λͼ
* Params     ** pλͼ�ļ�����ָ������
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
* Purpose    ����Ӧ��������ӵ��б����
* Params     bAppend:ͼ���Ƿ��Ǹ�����,TRUE�Ļ�,hIconNormalָ����Ҫ�ƶ�,����hIconNormalָ�벻��
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
* Purpose    ��ָ������**p�е�������ӵ�StrSpinBox��
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
* Purpose    ��ָ������**p�е�������ӵ�ComboBox��
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
* Purpose    ���¼������ݵ�Menulist��
* Params     **P:��Ҫ��ӵ�����,* id:�������ֶ�Ӧ����Ŀ��id,** pic:������Ŀ��Ӧ��ͼ���ļ���
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
* Purpose    ����HMENULIST
* Params     bLostkeyNotify:
            ���ϣ�������softkey��ʱ��,menulist�ղ�����Ϣ,��ô��ȡFALSE
            ���ϣ�������softkey��ʱ���,ֻ����softkey�յ���Ϣ,
            ��menulis�ղ�����Ϣ,��ô��ȡTRUE
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
* Purpose    ���MENULIST�е�ȫ������,*id��Ҫ��-1Ϊ������־
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
* Purpose    ������ת�����ַ�������,�������str��ָ����ַ�����. 
* Params     str:��Ž�����ַ���(Ҫ�㹻��),num:��Ҫת��������(��֧�ָ���),bZero:����֮ǰ�Ƿ����"0"
                bZero = FALSE,����֮ǰ�����"0"
                bZero = TRUE,����֮ǰ���"0"
                izeronum:��ӵ�0�ĸ���,�������Ӧ����������ֵĺ��������������0�ĸ���
                ����:num�ĺ������·�,����,izeronum��Ӧ��Ϊ2.����"02"��,"09"��.����"12"��,"31"��
                num�ĺ�����Сʱ,����,izeronumҲӦ����2,����"03"Сʱ,"05"����,����"14"Сʱ,"58"����
                ������������ֵ�ǰ���Զ�������Ӧ������"0"
* Return     
* Remarks      
**********************************************************************/
void    LongToStrings(char * str,unsigned long num,BOOL bZero,int izeronum)
{
    long    ires,imul;
    int     idigit = 1,i,j,integer;

    ires = num;
    while (ires / 10 > 0)
    {//������ֵ�λ��
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
        
        integer = ires / imul ;//��õ���������
        * str = (char) (integer + 48);
        str++;
        ires = ires - integer * imul;
    }
    * str = (char)0;
}
/********************************************************************
* Function   Setup_Init  
* Purpose    �ṩ�����������,���ڳ�ʼ�����ø�������
* Params     
* Return     
* Remarks    
**********************************************************************/
BOOL    Setup_InitFunction(void)
{
    BOOL        bRes = TRUE;
    SCENEMODE   sm;

    SetCurrentLanguage(GetLanguage());  //���õ�ǰ����
    CallAutoCloseClewWindow();          //�����Զ��ػ�����
    SetUp_SetDialCfg();                 //��������������������
#if defined(_PHOSPHOR_)
    CreateDirectory("FLASH2://public");//������������Ŀ¼
    SetCurrentDirectory("FLASH2://public");
    CreateDirectory("setup");
#endif

///////////////////////////����ɫ�ʷ�������/////////////////////////////////////////
#if defined(CAPITEL)
    SetCurrentColorCase();
#elif defined(_PHOSPHOR_)
    Setup_ColorInit();
#endif

///////////////////////////���ñ����,���̱��ⲿ��///////////////////////////////////////////////
#if defined(CAPITEL)//���ü��̱����,��Ļ�����
    SetupHf_Keylight(GetBg_keyboard());             //���ؼ��̵ı�����
    SetupHf_LcdBklight(MAIN_LCD,GetBg_screen());    //����LCD�ı����
#elif defined(_PHOSPHOR_)
    //���ñ������Ƿ����
    SetupHf_LcdBklight(0,GetBglight()); //����LCD�ı����
#endif

/////////////////////////���öԱȶȲ���/////////////////////////////////////////////////
    SetupHf_Contrast(MAIN_LCD,GetContrast());//���,���öԱȶ�ֵ

#if defined(_PHOSPHOR_)
    SetupHf_BgLight(GetBglightValue());//�ڴ���������
#endif

/////////////////////////�龰ģʽ����/////////////////////////////////////////////////
    CreateWindow_Setup();//����һ�����ش���������һ��ʱ���Զ�ֹͣ

    GetSM(&sm,GetCurSceneMode());//����龰ģʽ��Ϣ
    bRes = SetupHf_Volume(sm.iRingvolume) && bRes;//��������
    if ((sm.iComeview == 1) || (sm.iComeview == 2) || (sm.iComeview == 4))//����ģʽ(1:��;2:������,4:���񶯺�����)
    {
        BeginVibration(1);//��1��
    }
/////////////////////////��ʼ����������/////////////////////////////////////////////////
#if defined(_PHOSPHOR_)
    bRes = CreateWindow_PlayMusic() && bRes;
    Switch_init();//�����豸�л�����
#endif
    return (bRes);
}
////////////////////////////֧���Զ����е���ʾ����//////////////////////////////////////////////
BOOL    ConfirmWin(HWND hwnd,char * szPrompt,char * szTipsPic,char * szCaption,char * szOk,char * szCancel)
{
    HDC     hdc;
    char *  pDesstr = NULL;
    int     ilen;
    BOOL    bres;
    RECT    rect;

#if defined(CAPITEL)//������ʾ����ʾ���ֵ�����
    SetRect(&rect,10,42,166,191);//������ʾ����ʾ���ֵ�����
#elif defined(_PHOSPHOR_)
    SetRect(&rect,10,84,228,229);
#endif
    
    if ((hwnd == NULL) || (strlen(szPrompt) == 0))
    {
        bres = HPConfirmWin(szPrompt,szTipsPic,szCaption,szOk,szCancel);
        return (bres);
    }

    ilen = strlen(szPrompt) * 2 + 1;//���˵����,һ���ַ�(���ϻ��з�)ռһ��

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
//���ֿ����Զ����е���ʾ��
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
    ilen = strlen(szPrompt) * 2 + 1;//���˵����,һ���ַ�(���ϻ��з�)ռһ��

    if ((pDesstr = (char *)malloc(ilen)) == NULL)
    {
        HPTipsWin(szPrompt,szCaption,WAITTIMEOUT);
        return;
    }
    memset(pDesstr,0x00,ilen);
    
#if defined(CAPITEL)//������ʾ����ʾ���ֵ�����
    SetRect(&rect,6,16,90,60);//������ʾ����ʾ���ֵ�����
#elif defined(_PHOSPHOR_)
    SetRect(&rect,6,16,103,60);
#endif

    hdc = GetDC(hwnd);
    AutoNewLine(hdc,szPrompt,pDesstr,&rect,0);
    ReleaseDC(hwnd,hdc);

    HPTipsWin(pDesstr,szCaption,WAITTIMEOUT);
    
    free(pDesstr);
}
///////////////////////////////���²���Ϊ�Զ��������躯��--begin
//��cDesstr==NULLʱ,��Ϊֱ���ں���������ַ���,��ֱ��DrawText,���ؾ���������Ҫ�ĸ߶�
//��cDesstr!=NULLʱ,�������ɵ��ַ���,��ִ��DrawText,�������ؾ���������Ҫ�ĸ߶�
int     AutoNewLine(HDC hdc,char * cSourstr,char * cDesstr,RECT * rect,UINT uFormat)
{
    char *  pDesstr = NULL;
    int     iRowNum,iRectHeight,iRectWidth,ilen,ioldbuttom;
    int     iLanguage = 1;//0:����,1:Ӣ��
    SIZE    size;

    if (strlen(cSourstr) == 0)
    {
        if (cDesstr == NULL)
            DrawText(hdc,cSourstr,strlen(cSourstr),rect,uFormat);

        return 0;
    }
    ilen = strlen(cSourstr) * 2 + 1;//���˵����,һ���ַ�(���ϻ��з�)ռһ��

    if ((pDesstr = (char *)malloc(ilen)) == NULL)
    {
        if (cDesstr == NULL)
            DrawText(hdc,cSourstr,strlen(cSourstr),rect,uFormat);

        return 0;
    }
    memset(pDesstr,0x00,ilen);

    iRectWidth = rect->right - rect->left;//��þ�������Ŀ��

    iLanguage = GetCurrentLanguage();//��õ�ǰ����,0:Ӣ��,1:����

    switch (iLanguage)
    {
    case 0://Ӣ��
        iRowNum = AutoNewLine_eng(hdc,cSourstr,pDesstr,iRectWidth);
        GetTextExtent(hdc,"H",strlen("H"),&size);//���Ӣ���ַ��ĸ߶�
        break;
    case 1://����
        iRowNum = AutoNewLine_chn(hdc,cSourstr,pDesstr,iRectWidth);
        GetTextExtent(hdc,"ʮ",strlen("ʮ"),&size);//��������ַ��ĸ߶�
        break;
    }
    iRectHeight = iRowNum * size.cy + 5;//����������Ҫ�ĸ߶�

    if (cDesstr != NULL)
        strcpy(cDesstr,pDesstr);
    else
    {
        ioldbuttom   = rect->bottom;            //�����������ԭ���ĸ߶�
        rect->bottom = rect->top + iRectHeight; //�޸ľ�������ĸ߶��������
        DrawText(hdc,pDesstr,strlen(pDesstr),rect,uFormat);
        rect->bottom = ioldbuttom;              //�ָ���������ԭ���ĸ߶�
    }
    free(pDesstr);

    return (iRectHeight);
}
int     AutoNewLine_chn(HDC hdc,char * cSourstr,char * cDesstr,int iRectWidth)//��������
{
    int     iCharactorWidth;//һ��Ӣ���ַ�����һ�����ֵĿ��
    int     iCopyNum,iSourlen,iRowNum,iCopyWidth;
    char *  p = NULL;

    p           = cSourstr;
    iSourlen    = strlen(cSourstr);
    iCopyNum    = 0;//�Ѿ��������ַ�����
    iRowNum     = 1;//����
    iCopyWidth  = 0;//һ�����Ѿ��������ַ�һ���ж೤

    while(iCopyNum < iSourlen)
    {
        if ((* p == NULL) || (p == NULL))
            break;
        if ((* p == 0xa) || (* p == 0xd))//����ӻ��з�
        {
            p++;
            iCopyNum++;
            continue;
        }
        iCharactorWidth = GetCharactorWidth(hdc,p);//��õ�ǰ�ַ�(���ߺ���)�ĳ���
        if (iRectWidth - iCopyWidth > iCharactorWidth)//ʣ�೤�ȴ���һ��Ӣ���ַ�����һ�����ֵĿ��
        {
            if ((unsigned char)(* p) > 0x80)//����
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
        else if (iRectWidth - iCopyWidth == iCharactorWidth)//ʣ�೤�ȵ���һ��Ӣ���ַ�����һ�����ֵĿ��
        {
            if ((unsigned char)(* p) > 0x80)//����
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
            iRowNum++;//������һ    
        }
        else if (iRectWidth - iCopyWidth < iCharactorWidth)
        {//ʣ�೤��С��һ��Ӣ���ַ�����һ�����ֵĿ��,��Ҫ��ӻ��з�,������һ
            if (iCopyNum == 0)
            {
                //һ���ַ�Ҳû�п�������ӻ��з�,��������ʵĳ����Ѿ�����
                //�˾�������Ŀ��,ֱ�ӿ���Դ�ַ���,Ȼ�󷵻�
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
int     AutoNewLine_eng(HDC hdc,char * cSourstr,char * cDesstr,int iRectWidth)//��������
{
    int     iCopyNum,iSourlen,iRowNum,iCopyWidth,iWordWidth,iWordLength;
    char *  p = NULL,* pNextSpace = NULL,* pScan = NULL;

    p           = cSourstr;
    iSourlen    = strlen(cSourstr);
    iCopyNum    = 0;//�Ѿ��������ַ�����
    iRowNum     = 1;//����
    iCopyWidth  = 0;//һ�����Ѿ��������ַ�һ���ж೤

    while(iCopyNum < iSourlen)
    {
        if ((* p == NULL) || (p == NULL))
            break;
        if ((* p == 0xa) || (* p == 0xd))//����ӻ��з�
        {
            p++;
            iCopyNum++;
            continue;
        }

        pScan       = p;
        pNextSpace  = strchr(p,' ');//��Ӣ����ĸ

        iWordWidth  = 0;

        if (pNextSpace != NULL)
        {
            iWordLength = abs(pScan - pNextSpace);//������Ӣ�ĵ��ʵ���ĸ����

            while (pScan != pNextSpace)
            {
                iWordWidth += GetCharactorWidth(hdc,pScan);//������Ӣ�ĵ��ʵĿ��
                pScan++;
            }
        }
        else//�����ַ�����β
        {
            iWordLength = iSourlen - iCopyNum;

            while (* pScan != NULL)
            {
                iWordWidth += GetCharactorWidth(hdc,pScan);//������Ӣ�ĵ��ʵĿ��
                pScan++;
            }
        }
        
        if (iRectWidth - iCopyWidth > iWordWidth)//ʣ�೤��>һ��Ӣ�ĵ��ʵĿ��,ֱ�ӿ���
        {
            strncat(cDesstr,p,iWordLength + 1);//��Ҫ���ո�(���߷�Ӣ����ĸ)Ҳ������ȥ
            iCopyNum   += (iWordLength + 1);//���Ѿ��������ַ����������ۼ�,�����ո�(���߷�Ӣ����ĸ)
            iCopyWidth += (iWordWidth + GetCharactorWidth(hdc,pNextSpace));//������ۼ�

            p += (iWordLength + 1);
        }
        else if(iRectWidth - iCopyWidth == iWordWidth)//ʣ�೤�ȵ���һ��Ӣ�ĵ��ʵĿ��,ֱ�ӿ���,Ȼ����ӻ��з�
        {
            strncat(cDesstr,p,iWordLength + 1);//��Ҫ���ո�(���߷�Ӣ����ĸ)Ҳ������ȥ
            strcat(cDesstr,"\n");

            iCopyNum += (iWordLength + 1);//���Ѿ��������ַ����������ۼ�,�����ո�(���߷�Ӣ����ĸ)
            iRowNum++;
            iCopyWidth = 0;

            p += (iWordLength + 1);
        }
        else//ʣ�೤��С��һ��Ӣ�ĵ��ʵĿ��,ֱ����ӻ��з�
        {
            if (iCopyNum == 0)
            {
                //һ���ַ�Ҳû�п�������ӻ��з�,��������ʵĳ����Ѿ�����
                //�˾�������Ŀ��,ֱ�ӿ���Դ�ַ���,Ȼ�󷵻�
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
int     GetCharactorWidth(HDC hdc,char * ch)//��õ����ַ����ߵ������ֵĿ��
{
    SIZE size;

    if ((ch == NULL) || (* ch == NULL))
        return 0;

    if ((unsigned char)(* ch) > 0x80)//��ǰ�ַ��Ǻ���
        GetTextExtent(hdc,ch,2,&size);
    else
        GetTextExtent(hdc,ch,1,&size);

    return (size.cx);
}
///////////////////////////////���ϲ���Ϊ�Զ��������躯��--end
//////////////////////////����Ϊ�������Ų���begin//////////////////////////////////////////
#if defined(CAPITEL)
/******************************************************************
* Function   Setup_MusicPlay  
* Purpose    ���������ļ�
* Params     pFileName  �� ���ŵ��ļ���
              Volume     :  ����ʱ������
              PlayMode   ������ģʽ
              0: ѭ������
              !=0: ���Ŵ��� (<255)             
* Return        ��=0     -���ųɹ���SMAF���о����
                =0     -����ʧ�ܡ�
* Remarks      
**********************************************************************/
int     Setup_MusicPlay(char * pFileName,UINT Volume,int PlayMode)
{
    char        * p = NULL;
    int         iType = 0;
    int         iresult = 1;
    JMUSICINFO  jmusicinfo;
    
    p = strstr(pFileName,".");//û���ҵ�"."���Ƿ����ļ���
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

    SetupHf_Volume(Volume);//��������
#ifndef _EMULATE_
    DHI_PlayMusic(&jmusicinfo);
#endif
    return (iresult);
}
/********************************************************************
* Function   SETUP_PlayMusic  
* Purpose    ��������
* Params     seqHd ��PlayMusic�������ص��ļ����
* Return     
* Remarks      
**********************************************************************/
BOOL    SETUP_PlayMusic(pJSETUPMUSICINFO pSetupMusicInfo)
{
    SCENEMODE           sm;//����龰ģʽ��Ϣ
    RINGSETTING         rs;//���������Ϣ
    JMUSICINFO          jmusicinfo;
    char                ringname[RINGNAMEMAXLEN + 1] = "";//���������ļ���
    char                cPhoneNum[43] = "";//���浱ǰ�绰����
    char                * p = NULL;
    int                 iType = 0,iVolume = GetMaxRingVolume();
    unsigned long       ulPriority;//���������ȼ�
    unsigned short      sPlayModeAndCount;//����ģʽ�Ͳ��Ŵ���
    //������Щ�����Ĳ��Ŵ�������Ϊһ��,���簴����,��ͨ��ʾ��,������Ҫ�ں�����淶���Ŵ���

    memset(&sm          ,0,sizeof(SCENEMODE  ));
    memset(&rs          ,0,sizeof(RINGSETTING));
    memset(&jmusicinfo  ,0,sizeof(JMUSICINFO ));
    
    GetRingSet(&rs);
    GetSM(&sm,GetCurSceneMode());

    sPlayModeAndCount = pSetupMusicInfo->usPlayModeAndCount;

    switch (pSetupMusicInfo->uiRingSelect)
    {
    case RING_PHONE://����
        App_GetCurPhoneNum(cPhoneNum);//��õ�ǰ�绰����
        if (!APP_PBGetCurRingName(cPhoneNum,ringname))//���ݵ�ǰ�绰�����õ�ǰ����
        {//�ú�������FALSE��ʾû���ҵ��õ绰�����Ӧ����,��ʱ��ʹ��һ��Ĭ�ϵ�����
            GetDefaultRing(ringname);
        }
        ulPriority = HIGH;
        switch (sm.iComeview)
        {
        case 0://����
            iVolume = sm.iRingvolume;   //��������
            break;
        case 1://��
            SetupHf_Vibration(TRUE);
            return TRUE;
            break;
        case 2://������
            iVolume = sm.iRingvolume;   //��������
            SetupHf_Vibration(TRUE);
            break;
        case 3://����
            return TRUE;
            break;
        case 4://����(5��)������
            SetupHf_Vibration(TRUE);
            SM_LibrateRingBegin();
            return TRUE;
            break;
        }
        break;
    case RING_SMS://������ʾ��
    case RING_MMS://������ʾ��
        if (sm.iInfoClew == 0)//�ر�
            return TRUE;

        strcpy(ringname,rs.cRingsetSMS);
        sPlayModeAndCount   = COUNT_PLAY | 1;
        iVolume             = sm.iRingvolume;   //��������
        ulPriority          = ABOVENORMAL;
        BeginTime();

        break;
    case RING_POWNON://��������
        strcpy(ringname,rs.cRingsetOpen);
        ulPriority = ABOVENORMAL;
        break;
    case RING_CAM://������ʾ��
        strcpy(ringname,"ROM:snap4.mmf");
        sPlayModeAndCount   = COUNT_PLAY | 1;
        ulPriority          = NORMAL;
        break;
    case RING_KEY://��������ʾ��
    case RING_SCREEN://������
        if (sm.iKeypress == 0)//�ر�
            return TRUE;

        strcpy(ringname,"ROM:ring20.mmf");
        sPlayModeAndCount   = COUNT_PLAY | 1;
        iVolume             = sm.iKeypress;
        ulPriority          = LOW;
        break;
    case RING_TOUCH://��ͨ��ʾ��
        if (sm.iGet == 0)//�ر�
            return TRUE;
        strcpy(ringname,"ROM:ring22.mmf");
        sPlayModeAndCount   = COUNT_PLAY | 1;
        iVolume             = sm.iGet;
        ulPriority          = BELOWNORMAL;
        break;
    case RING_ALARM://�ճ̱�����
        strcpy(ringname,rs.cRingsetAlarm);
        ulPriority = ABOVENORMAL;
        break;
    case RING_CLEW50://50����ʾ��
        strcpy(ringname,"ROM:ring22.mmf");
        sPlayModeAndCount   = COUNT_PLAY | 1;
        ulPriority          = NORMAL;
        break;
    case RING_WARNING://������
        if (sm.iWarning == 0)//�ر�
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
    if (p == NULL)//û���ҵ�"."���Ƿ����ļ���
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

    SetupHf_Volume(iVolume);//��������
#ifndef _EMULATE_
    DHI_PlayMusic(&jmusicinfo);
#endif
    return TRUE;
}
void    TouchAlert ()//������
{
    JSETUPMUSICINFO jsetupmusicinfo;
    jsetupmusicinfo.uiRingSelect        = RING_SCREEN;
    jsetupmusicinfo.usPlayModeAndCount  = 1 | COUNT_PLAY;
    jsetupmusicinfo.pPlayMusicCallBack  = 0;
    jsetupmusicinfo.uiEventMask         = 0;

    SETUP_PlayMusic(&jsetupmusicinfo);
}
#elif defined(_PHOSPHOR_)
extern  int     GetMaxRingVolume();//����������
extern  int     SetupHf_PlayMusicInit(SetupHS_PlayMusic * playmusic);
extern  int     SetupHf_PlayAMR();
extern  int     SetupHf_PlayWAVE();
extern  int     SetupHf_PlayMIDI();//����midi�ļ�
extern  int     SetupHF_EndPlayMusic(void);//ֹͣ��������
/******************************************************************
* Function   SETUP_PlayMusic  
* Purpose    ���������ļ�(���ڲ���,�����)
* Params     
             hWnd:      ���÷��Ĵ��ھ��
             iWM_MMWRITE:���÷��������Ϣ,���÷������ڸ���Ϣ�е���Setup_WriteMusicData����
            �Ƽ���������:Setup_WriteMusicData((LPWAVEHDR)lParam);
            iWM_MMSTOP: ���÷��������Ϣ,����֪ͨ���÷������������,���÷������ڸ���Ϣ�е���Setup_EndPlayMusic()����
            ������������ϵ�ʱ��,iWM_MMSTOP��Ϣ�е�wParam=PLAY_OVER��ʾ���ֲ������,��������������
            pBuffer:��������,
            ulBufferSize:��������ĳ���
* Return        ��=0     -���ųɹ�
                =0     -����ʧ�ܡ�
* Remarks      
**********************************************************************/
int     SETUP_PlayMusic(SetupHS_PlayMusic * hs)
{
    unsigned char *     p = NULL;
    unsigned char       strAudioType[5] = "";//�������ݻ��������������͵��ж�
    SCENEMODE           sm;//����龰ģʽ��Ϣ
    int                 ires = 0,i;
    SetupHS_PlayMusic   setuphs_playmusic;

    memcpy(&setuphs_playmusic,hs,sizeof(SetupHS_PlayMusic));
    
    GetSM(&sm,GetCurSceneMode());
    switch (setuphs_playmusic.uiringselect)
    {
    case RING_PHONE://������ʾ��
        setuphs_playmusic.iVolume       = sm.iRingvolume;//������������
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_HIGH;
        break;
    case RING_POWNON:   //��������
    case RING_POWNOFF:  //�ػ�����
    case RING_SMS:      //����
    case RING_MMS:      //����
        setuphs_playmusic.iVolume       = GetMaxRingVolume();//����Ӧ��ʹ���������
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_ABOVENORMAL;
        break;
    case RING_KEY:      //������
    case RING_TOUCH:    //��ͨ��ʾ��
    case RING_SCREEN:   //������
    case RING_CLEW50:   //50����ʾ��
        setuphs_playmusic.iVolume       = GetMaxRingVolume();//����Ӧ��ʹ���������
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_LOW;//��Щ�������������ȼ�Ӧ��Ϊ���
        break;
    case RING_OTHER:    //������������,�������,������е������ļ�,����Ԥ����һ��Ӧ��
        setuphs_playmusic.iVolume       = GetMaxRingVolume();//����Ӧ��ʹ���������
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_NORMAL;
        break;
    case RING_VOLUME://���������ú�,�����ɲ���iVolume����
        setuphs_playmusic.uiringselect  = WAVEOUT_PRIO_NORMAL;
        break;
    default:
        ShowDebugInfo("!!!�Ƿ�����������,�޷�����","SETUP_PlayMusic",setuphs_playmusic.uiringselect);
        return 0;
        break;
    }

    if ((setuphs_playmusic.pBuffer != NULL) && (setuphs_playmusic.ulBufferSize != 0))//�������ݻ�����������������
    {
        setuphs_playmusic.pFileName = NULL; //���ļ���ָ�븳��

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
            ShowDebugInfo("!!!�Ƿ������ݻ�����,�޷�����","SETUP_PlayMusic",1063);
            return (0);
        }
    }
    else if (strlen(setuphs_playmusic.pFileName))//���ļ����в���
    {
        p = strstr(setuphs_playmusic.pFileName,".");//�����ļ���չ��
        if (p != NULL)
        {
            if (stricmp(p,".amr") == 0)//����ļ�����
                setuphs_playmusic.iType = AMR_TYPE;
            else if (stricmp(p,".wav") == 0)
                setuphs_playmusic.iType = WAVE_TYPE;
            else if ((stricmp(p,".midi") == 0) || (stricmp(p,".mid") == 0))
                setuphs_playmusic.iType = MIDI_TYPE;
            else if (stricmp(p,".mmf") == 0)
                setuphs_playmusic.iType = MMF_TYPE;
            else if (stricmp(p,".tmp") == 0)//�����չ����tmp,�ļ���������߾���
            {}
            else
            {
                ShowDebugInfo("!!!�Ƿ�����������,�޷�����",setuphs_playmusic.pFileName,1084);
                return 0;
            }
        }
        else
        {
            ShowDebugInfo("!!!�Ƿ��������ļ�,�޷�����",setuphs_playmusic.pFileName,1090);
            return 0;
        }
        setuphs_playmusic.pBuffer       = NULL; //�����ݻ�������ָ�����
        setuphs_playmusic.ulBufferSize  = 0;    //�����ݻ������ĳ�������
    }
    else
    {
        ShowDebugInfo("!!!�Ƿ�����������,�޷�����","SETUP_PlayMusic",1084);
        return 0;
    }
    if (!SetupHf_PlayMusicInit(&setuphs_playmusic))
    {
        ShowDebugInfo("!!!��ʼ��ʧ��.","SETUP_PlayMusic",1103);
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
static  char        cRingName[RINGNAMEMAXLEN + 1] = ""; //�����ļ���
static  int         iCallCount;                         //���Ŵ���,0Ϊѭ������,����Ϊ���Ŵ���

#define     CLASSNAME_PLAYMUSIC     "PlayMusicWndClass" //��������
#define     WM_MMWRITE              100
#define     WM_MMSTOP               110
#define     WM_MMGETDATA            115
#define     IDC_PLAYMUSIC           120
/********************************************************************
* Function   Call_PlayMusic  
* Purpose    ���������ӿ�
* Params     uiringselect:�������
            icount:���Ŵ���,0Ϊѭ������,����Ϊ���Ŵ���
* Return     
* Remarks     
**********************************************************************/
BOOL    Call_PlayMusic(int uiringselect,int icount)
{
    SCENEMODE   sm;//����龰ģʽ��Ϣ
    
    GetSM(&sm,GetCurSceneMode());           //����龰ģʽ��Ϣ
    memset(cRingName,0,sizeof(cRingName));

    switch (uiringselect)
    {
    case RING_PHONE://����
        switch (sm.iComeview)
        {
        case 0://����
            strcpy(cRingName,sm.rRingselect.cPhone);
            break;
        case 1://��
            SetupHf_Vibration(TRUE);
            return TRUE;
            break;
        case 2://������
            strcpy(cRingName,sm.rRingselect.cPhone);
            SetupHf_Vibration(TRUE);
            break;
        case 3://����
            return TRUE;
            break;
        default:
            return FALSE;
        }
        break;
    case RING_SMS://������ʾ��
        switch (sm.iComeview)
        {
        case 0://����
            strcpy(cRingName,sm.rRingselect.cShortInfo);
            break;
        case 1://��
            BeginVibration(1);//��һ��
            break;
        case 2://������
            BeginVibration(1);//��һ��
            strcpy(cRingName,sm.rRingselect.cShortInfo);
            break;
        case 3://����
            return TRUE;
            break;
        default:
            return FALSE;
        }
        break;
    case RING_POWNON://��������
        GetOpenRing(cRingName);
        break;
    case RING_MMS://������ʾ��
        strcpy(cRingName,sm.rRingselect.cMMS);
        break;
    case RING_KEY://��������ʾ��
        strcpy(cRingName,sm.cKey);
        break;
    case RING_TOUCH://��ͨ��ʾ��
        strcpy(cRingName,"/rom/prefab/audio/key1.wav");
        break;
    case RING_SCREEN://������
        strcpy(cRingName,sm.cTouch);
        break;
    case RING_POWNOFF://�ػ�����
        GetCloseRing(cRingName);
        break;
    case RING_CLEW50://50����ʾ��
        strcpy(cRingName,"/rom/prefab/audio/key1.wav");
        break;
    default:
        return FALSE;
        break;
    }
    iCallCount = abs(icount);
    //��Ϊ��������Ϊ0Ϊѭ������,����Ϊ���Ŵ���,Ϊ���ڲ����Ŵ��������ķ���
    //���ڲ���ѭ�����ű�־Ϊ-1,������һ�ε�ʱ��iCallCount--,��iCallCount��Ϊ���ʱ��
    //ֹͣ����.
    if (!iCallCount)
        iCallCount = -1;
    else
        iCallCount--;//��ʾ��һ�β���

    PostMessage(hWndApp,IDC_PLAYMUSIC,uiringselect,0);//�����������wParam����

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
static  LRESULT AppWndProc_PlayMusic ( HWND hWnd, UINT wMsgCmd, WPARAM wParam, LPARAM lParam );
BOOL    CreateWindow_PlayMusic()//����һ�����ش������ڲ�������,�ú�����ϵͳ������ʱ�򴴽�
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
    case WM_MMWRITE://����Ϣ���ڲ�������
        Setup_WriteMusicData((LPWAVEHDR)lParam);
        break;
    case WM_MMGETDATA:
        Setup_GetMusicData((LPWAVEHDR)lParam);
        break;
    case WM_MMSTOP://��������ֹͣ��ʾ����ֹͣ����
        Setup_EndPlayMusic(0);
        
        if (iCallCount == -1)//ѭ������
            PostMessage(hWnd,IDC_PLAYMUSIC,hs.uiringselect,0);
        else if (iCallCount > 0)
        {
            PostMessage(hWnd,IDC_PLAYMUSIC,hs.uiringselect,0);
            iCallCount--;//���Ŵ�����һ
        }
        else if (iCallCount == 0)//�ò��Ĵ����Ѿ�������
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
* Purpose    ����ֹͣ�ӿں���,�û�������������ӿ�ֹͣ����
* Params     
* Return     
* Remarks     
**********************************************************************/
BOOL    Call_EndPlayMusic(int uiringselect)
{
    BOOL bres = FALSE;

    bres = Setup_EndPlayMusic(uiringselect);
    SetupHf_Vibration(FALSE);//ֹͣ��

    return (bres);
}
#endif
//////////////////////////////////////////////////////////////////////////
#define     CLASSNAME_SETUP             "CreateWindow_SetupWndClass"    //��������
#define     ID_TIMER_LIMITMUSICPLAY     7
#define     ID_TIMER_LIBRATETHENRING    8//���񶯺������timer
#define     ID_TIMER_VIBRATION          9//�����𶯵ļ�ʱ��
static  HINSTANCE           hInstance;
static  HWND                hWndApp_setup = NULL;

static  LRESULT AppWndProc_Setup( HWND hWnd, UINT wMsgCmd, WPARAM wParam, LPARAM lParam );
BOOL    CreateWindow_Setup(void)//����һ�����ش���������һ��ʱ���Զ�ֹͣ,�ô�����ϵͳ������ʱ�򴴽�
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
        SetupHf_Vibration(FALSE);//ֹͣ��
        return FALSE;
    }
    hwnd = CreateWindow(CLASSNAME_SETUP,"",WS_CAPTION|WS_BORDER |PWS_STATICBAR,
        HP_WIN_POSITION , NULL, NULL, NULL, NULL);

    if (!hwnd)
    {
        SetupHf_Vibration(FALSE);//ֹͣ��
        return FALSE;
    }
    hWndApp_setup = hwnd;
    return (TRUE);
}
static LRESULT AppWndProc_Setup( HWND hWnd, UINT wMsgCmd, WPARAM wParam, LPARAM lParam )
{
    LRESULT     lResult;
    char        cPhoneNum[43] = "";//���浱ǰ�绰����
    char        ringname[RINGNAMEMAXLEN + 1] = "";//���������ļ���
    SCENEMODE   sm;

    lResult = (LRESULT)TRUE;
    
    memset(&sm,0x00,sizeof(SCENEMODE));
    switch ( wMsgCmd )
    {
    case WM_TIMER:
        switch(wParam)
        {
        case ID_TIMER_LIMITMUSICPLAY://�޶��������ŵ�ʱ��
            Setup_EndPlayMusic(0);//ֹͣ����
            KillTimer(hWndApp_setup,ID_TIMER_LIMITMUSICPLAY);
            break;
#if defined(CAPITEL)
        case ID_TIMER_LIBRATETHENRING://��ʼ����(5��)������
            SetupHf_Vibration(FALSE);//ֹͣ��
            KillTimer(hWnd,ID_TIMER_LIBRATETHENRING);
            //��ʼ��������
            App_GetCurPhoneNum(cPhoneNum);//��õ�ǰ�绰����
            if (!APP_PBGetCurRingName(cPhoneNum,ringname))//���ݵ�ǰ�绰�����õ�ǰ����
            {//�ú�������FALSE��ʾû���ҵ��õ绰�����Ӧ����,��ʱ��ʹ��һ��Ĭ�ϵ�����
                GetDefaultRing(ringname);
            }
            GetSM(&sm,GetCurSceneMode());
            Setup_MusicPlay(ringname,sm.iRingvolume,0);
            break;
#endif
        case ID_TIMER_VIBRATION://������һ��ʱ���Զ�ֹͣ
            SetupHf_Vibration(FALSE);//ֹͣ��
            KillTimer(hWnd,ID_TIMER_VIBRATION);
            break;
        }
        break;
    case WM_DESTROY:
        //֪ͨ���������,�˳����˳�;
        UnregisterClass(CLASSNAME_SETUP,NULL);
        DlmNotify ((WPARAM)PES_STCQUIT, (LPARAM)hInstance );
        break;
    default :
        lResult = PDADefWindowProc(hWnd, wMsgCmd, wParam, lParam);
        break;
    }
    return lResult;
}

void    SM_LibrateRingBegin(void)//��ʼ����(5��)������
{
    SetTimer(hWndApp_setup,ID_TIMER_LIBRATETHENRING,5000,NULL);
}

void    SM_LibrateRingEnd(void)//�������񶯺�����Ĺ���,��Ҫ��ɾ����ʱ��,�����������Ҷ�
{
    KillTimer(hWndApp_setup,ID_TIMER_LIBRATETHENRING);
}

void    BeginTime(void)//�޶��������ŵ�ʱ��,������������ʱ��
{
    SetTimer(hWndApp_setup,ID_TIMER_LIMITMUSICPLAY,2000,NULL);
}

void    BeginVibration(int iSecond)//ʵ����iSecond��
{
    SetTimer(hWndApp_setup,ID_TIMER_VIBRATION,iSecond * 1000,NULL);
    SetupHf_Vibration(TRUE);
}
//////////////////////////����Ϊ�������Ų���end//////////////////////////////////////////
//////////////////////////���²���Ϊ���ò��ֶ����Ӳ���ӿں���////////////////////////////////////////////////
//CAPITEL ������Ŀ�������hp_dhi.h
/********************************************************************
* Function   Setup_EndPlayMusic  
* Purpose    ֹͣ��������
* Params     
* Return     
* Remarks      
**********************************************************************/
int     Setup_EndPlayMusic(int uiringselect)
{
    int         ires = 1;
#if defined(CAPITEL)
    SCENEMODE   sm;
    SetupHf_Vibration(FALSE);   //ֹͣ��

    GetSM(&sm,GetCurSceneMode());
    if (sm.iComeview == 4)//����(5��)������
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
* Purpose    ��������
* Params     
* Return     
* Remarks    ��������Ŀ��֧���ڴ���������
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
* Purpose    �߲ʵ���ɫ����
* Params     ColorΪҪ�趨����ɫֵ��0~7��8�֣����ڴ˷�Χ����ֱ�ӷ��ء�
            0Ϊ�صƣ�1Ϊ��ƣ�2�̵ƣ�3���ƣ�4���̵ƣ�5�����ƣ�6�����ƣ�7�������ƣ��׵ƣ�
        
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
* Purpose    ���ؼ��̵ı�����
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
* Params     ������Ļ
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
* Purpose    ����LCD�ı���ƵĿ��ء�
* Params     uiLcdNoָ����LCD��;
            bEn��ΪTRUEʱ������ָ��LCD�ı����ƣ�ΪFALSEʱ���ر�ָ��LCD�ı�����
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
* Purpose    ���öԱȶ�
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
* Purpose    ��������
* Params     
* Return     
* Remarks      
**********************************************************************/
extern  void    SetupHf_BgLight_ioctl(int iBgLight);//����
void    SetupHf_BgLight(int iBgLight)
{
#if defined(_PHOSPHOR_)
    SetupHf_BgLight_ioctl(iBgLight);
#endif
}
/********************************************************************
* Function   SetupHf_Vibration  
* Purpose    ��/�ر���
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
* Params     ˯�߿���
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
* Params     ��Դ�ض�
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
///////////////////////////��ʾ��ʾ��Ϣ///////////////////////////////////////////////
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