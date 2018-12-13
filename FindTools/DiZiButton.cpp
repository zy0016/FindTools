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
    bs      = BUTTONSTATE1;//按钮状态为抬起,并且鼠标没有在按钮之上
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
    MemDC.CreateCompatibleDC(&xdc); ///建立与显示设备兼容的内存设备场境

//    CBitmap* m_oldBitmap;
  //  BITMAP bmSize;

    switch (bs)
    {
    case BUTTONSTATE1://按钮状态为抬起,并且鼠标没有在按钮之上
        DrawBitmap(ButtonState1);
        break;
    case BUTTONSTATE2://按钮状态为抬起,并且鼠标在按钮之上,鼠标没有按下
        DrawBitmap(ButtonState2);
        break;
    case BUTTONSTATE3://按钮状态为按下,并且鼠标没有松开
        DrawBitmap(ButtonState3);
        break;
    case BUTTONSTATE4://按钮为按下状态,鼠标已经抬起,并且不在按钮之上
        DrawBitmap(ButtonState4);
        break;
    case BUTTONSTATE5://按钮为按下状态,鼠标在按钮之上
        DrawBitmap(ButtonState5);
        break;
    }
    /*if (state & ODS_FOCUS)
    {
        ButtonState4.GetBitmap(&bmSize);//得到位图结构中的大小信息 

        m_oldBitmap = MemDC.SelectObject(&ButtonState4);	//将位图选入内存场境
        xdc.BitBlt(0,0,bmSize.bmWidth,bmSize.bmHeight,&MemDC,0,0,SRCCOPY);		//显示它
        MemDC.SelectObject(m_oldBitmap);
	    
	    MemDC.DeleteDC();
    }
    else if (state & ODS_SELECTED)
    {
        ButtonState2.GetBitmap(&bmSize);//得到位图结构中的大小信息 

        m_oldBitmap = MemDC.SelectObject(&ButtonState2);	//将位图选入内存场境
        xdc.BitBlt(0,0,bmSize.bmWidth,bmSize.bmHeight,&MemDC,0,0,SRCCOPY);		//显示它
        MemDC.SelectObject(m_oldBitmap);
	    
	    MemDC.DeleteDC();
    }
    else
    {
        ButtonState1.GetBitmap(&bmSize);//得到位图结构中的大小信息 

        m_oldBitmap = MemDC.SelectObject(&ButtonState1);	//将位图选入内存场境
        xdc.BitBlt(0,0,bmSize.bmWidth,bmSize.bmHeight,&MemDC,0,0,SRCCOPY);		//显示它
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
    if (m_State)//当前按钮已经按下了
    {
    }
    else//当前按钮是抬起状态
    {
        bs = BUTTONSTATE3;//按钮状态为按下,并且鼠标没有松开
        DrawBitmap(ButtonState3);
        SetCheck(TRUE);
    }

	CButton::OnLButtonDown(nFlags, point);
}

void DiZiButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    if (m_State)//当前按钮已经按下了
    {
        DrawBitmap(ButtonState2);
        SetCheck(FALSE);
    }
    else//当前按钮是抬起状态
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

    if (rcItem.PtInRect(pt))//鼠标在按钮区域之内
    {
        if (m_State)//当前按钮按下
        {
            bs = BUTTONSTATE5;//按钮为按下状态,鼠标在按钮之上
            DrawBitmap(ButtonState5);
        }
        else//按钮当前没有按下
        {
            bs = BUTTONSTATE2;//按钮状态为抬起,并且鼠标在按钮之上,鼠标没有按下
            DrawBitmap(ButtonState2);
        }
    }
    else//鼠标不在按钮区域之内
    {
        KillTimer(BUTTONSTATE_TIMER);
        if (m_State)//当前按钮按下
        {
            bs = BUTTONSTATE4;//按钮为按下状态,鼠标已经抬起
            DrawBitmap(ButtonState4);
        }
        else//按钮当前没有按下
        {
            bs = BUTTONSTATE1;//按钮状态为抬起,并且鼠标没有在按钮之上
            DrawBitmap(ButtonState1);
        }
    }
    
	CButton::OnTimer(nIDEvent);
}
//在按钮上贴不同的图片
void DiZiButton::DrawBitmap(CBitmap &DBitmap)
{
    BITMAP bmSize;
    CBitmap* m_oldBitmap;
    CDC * xdc;
    CDC MemDC;

    xdc = GetDC();

    MemDC.CreateCompatibleDC(xdc); //建立与显示设备兼容的内存设备场境

	DBitmap.GetBitmap(&bmSize);//得到位图结构中的大小信息 

    m_oldBitmap = MemDC.SelectObject(&DBitmap);	//将位图选入内存场境
    xdc->BitBlt(5,5,bmSize.bmWidth,bmSize.bmHeight,&MemDC,0,0,SRCCOPY);		//显示它
    MemDC.SelectObject(m_oldBitmap);
    
    MemDC.DeleteDC();
}
