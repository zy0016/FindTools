// DiZiButton.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "DiZiButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DiZiButton

DiZiButton::DiZiButton()
{
    bs      = BUTTONSTATE1;//��ť״̬Ϊ̧��,�������û���ڰ�ť֮��
    m_State = FALSE;

    ButtonState1.LoadBitmap(IDB_BITMAP_TD1);
    ButtonState2.LoadBitmap(IDB_BITMAP_TD2);
    ButtonState3.LoadBitmap(IDB_BITMAP_TD3);
    ButtonState4.LoadBitmap(IDB_BITMAP_TD5);
    ButtonState5.LoadBitmap(IDB_BITMAP_TD5);
}

DiZiButton::~DiZiButton()
{
}


BEGIN_MESSAGE_MAP(DiZiButton, CButton)
	//{{AFX_MSG_MAP(DiZiButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DiZiButton message handlers


void DiZiButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void DiZiButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	UINT state = lpDrawItemStruct->itemState ;
    
    CDC xdc;
    CDC MemDC;
    xdc.Attach( lpDrawItemStruct->hDC );
    MemDC.CreateCompatibleDC(&xdc); ///��������ʾ�豸���ݵ��ڴ��豸����

//    CBitmap* m_oldBitmap;
  //  BITMAP bmSize;

    switch (bs)
    {
    case BUTTONSTATE1://��ť״̬Ϊ̧��,�������û���ڰ�ť֮��
        DrawBitmap(ButtonState1);
        break;
    case BUTTONSTATE2://��ť״̬Ϊ̧��,��������ڰ�ť֮��,���û�а���
        DrawBitmap(ButtonState2);
        break;
    case BUTTONSTATE3://��ť״̬Ϊ����,�������û���ɿ�
        DrawBitmap(ButtonState3);
        break;
    case BUTTONSTATE4://��ťΪ����״̬,����Ѿ�̧��,���Ҳ��ڰ�ť֮��
        DrawBitmap(ButtonState4);
        break;
    case BUTTONSTATE5://��ťΪ����״̬,����ڰ�ť֮��
        DrawBitmap(ButtonState5);
        break;
    }
    /*if (state & ODS_FOCUS)
    {
        ButtonState4.GetBitmap(&bmSize);//�õ�λͼ�ṹ�еĴ�С��Ϣ 

        m_oldBitmap = MemDC.SelectObject(&ButtonState4);	//��λͼѡ���ڴ泡��
        xdc.BitBlt(0,0,bmSize.bmWidth,bmSize.bmHeight,&MemDC,0,0,SRCCOPY);		//��ʾ��
        MemDC.SelectObject(m_oldBitmap);
	    
	    MemDC.DeleteDC();
    }
    else if (state & ODS_SELECTED)
    {
        ButtonState2.GetBitmap(&bmSize);//�õ�λͼ�ṹ�еĴ�С��Ϣ 

        m_oldBitmap = MemDC.SelectObject(&ButtonState2);	//��λͼѡ���ڴ泡��
        xdc.BitBlt(0,0,bmSize.bmWidth,bmSize.bmHeight,&MemDC,0,0,SRCCOPY);		//��ʾ��
        MemDC.SelectObject(m_oldBitmap);
	    
	    MemDC.DeleteDC();
    }
    else
    {
        ButtonState1.GetBitmap(&bmSize);//�õ�λͼ�ṹ�еĴ�С��Ϣ 

        m_oldBitmap = MemDC.SelectObject(&ButtonState1);	//��λͼѡ���ڴ泡��
        xdc.BitBlt(0,0,bmSize.bmWidth,bmSize.bmHeight,&MemDC,0,0,SRCCOPY);		//��ʾ��
        MemDC.SelectObject(m_oldBitmap);
	    
	    MemDC.DeleteDC();
    }*/
}

void DiZiButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    SetTimer(BUTTONSTATE_TIMER,10,NULL);
    
	CButton::OnMouseMove(nFlags, point);
}

void DiZiButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    if (m_State)//��ǰ��ť�Ѿ�������
    {
    }
    else//��ǰ��ť��̧��״̬
    {
        bs = BUTTONSTATE3;//��ť״̬Ϊ����,�������û���ɿ�
        DrawBitmap(ButtonState3);
        SetCheck(TRUE);
    }

	CButton::OnLButtonDown(nFlags, point);
}

void DiZiButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    if (m_State)//��ǰ��ť�Ѿ�������
    {
        DrawBitmap(ButtonState2);
        SetCheck(FALSE);
    }
    else//��ǰ��ť��̧��״̬
    {
        DrawBitmap(ButtonState5);
        SetCheck(TRUE);
    }

    m_State = !m_State;

	CButton::OnLButtonUp(nFlags, point);
}

void DiZiButton::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

    if (rcItem.PtInRect(pt))//����ڰ�ť����֮��
    {
        if (m_State)//��ǰ��ť����
        {
            bs = BUTTONSTATE5;//��ťΪ����״̬,����ڰ�ť֮��
            DrawBitmap(ButtonState5);
        }
        else//��ť��ǰû�а���
        {
            bs = BUTTONSTATE2;//��ť״̬Ϊ̧��,��������ڰ�ť֮��,���û�а���
            DrawBitmap(ButtonState2);
        }
    }
    else//��겻�ڰ�ť����֮��
    {
        KillTimer(BUTTONSTATE_TIMER);
        if (m_State)//��ǰ��ť����
        {
            bs = BUTTONSTATE4;//��ťΪ����״̬,����Ѿ�̧��
            DrawBitmap(ButtonState4);
        }
        else//��ť��ǰû�а���
        {
            bs = BUTTONSTATE1;//��ť״̬Ϊ̧��,�������û���ڰ�ť֮��
            DrawBitmap(ButtonState1);
        }
    }
    
	CButton::OnTimer(nIDEvent);
}
//�ڰ�ť������ͬ��ͼƬ
void DiZiButton::DrawBitmap(CBitmap &DBitmap)
{
    BITMAP bmSize;
    CBitmap* m_oldBitmap;
    CDC * xdc;
    CDC MemDC;

    xdc = GetDC();

    MemDC.CreateCompatibleDC(xdc); //��������ʾ�豸���ݵ��ڴ��豸����

	DBitmap.GetBitmap(&bmSize);//�õ�λͼ�ṹ�еĴ�С��Ϣ 

    m_oldBitmap = MemDC.SelectObject(&DBitmap);	//��λͼѡ���ڴ泡��
    xdc->BitBlt(5,5,bmSize.bmWidth,bmSize.bmHeight,&MemDC,0,0,SRCCOPY);		//��ʾ��
    MemDC.SelectObject(m_oldBitmap);
    
    MemDC.DeleteDC();
}
