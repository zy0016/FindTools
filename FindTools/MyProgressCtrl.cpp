// MyProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyProgressCtrl

CMyProgressCtrl::CMyProgressCtrl()
{
	iPos        = -1;
	iStep       = 1;
	crFillColor = RGB(255,0,0);
	crTextColor = RGB(0,0,255);
	bShowPre    = TRUE;
}

CMyProgressCtrl::~CMyProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CMyProgressCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyProgressCtrl message handlers

void CMyProgressCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CString str;

	if (iPos == -1)
		return;

	/*CRgn rgn;
	VERIFY( rgn.CreateRectRgnIndirect( &rc ) );
	(void)dc.SelectObject( &rgn );
	VERIFY( rgn.DeleteObject() );*/

	CPen pen(PS_NULL,0,crFillColor);
	CBrush brush(crFillColor);//RGB(255,0,0)
	CBrush * oldBrush;

	dc.SelectObject(&pen);
	oldBrush = dc.SelectObject(&brush);
	dc.Rectangle(&rFillin);
	iRectFillRight = rFillin.right;//保存已经刷新的区域的right
	
	brush.DeleteObject();
    dc.SelectObject(oldBrush);
    
	if (bShowPre)
	{
		str.Format("%d%%",iPos * 100 / (iUpper - iLower));
		dc.SetBkMode(TRANSPARENT);
		dc.SetBkColor(RGB(0,255,0));
		dc.SetTextColor(crTextColor);
		(void)dc.DrawText(str, -1, rctxt, DT_CENTER |DT_VCENTER);//hditem.fmt
	}
}

int CMyProgressCtrl::SetPos( int nPos )
{
	int oldpos = iPos;

	iPos = nPos;

	if(iPos != oldpos)
	{
		GetClientRect(&rFresh);
		GetClientRect(&rFillin);

		rFresh.right  = rFresh.left + iPos * (rFresh.right - rFresh.left) / (iUpper - iLower);
		rFillin.right = rFresh.right;

		if (rFresh.right > rFresh.left)
		{
			rFresh.left = iRectFillRight - 1;

			InvalidateRect(&rFresh,TRUE);
			UpdateWindow();

			if (bShowPre)
			{
				if (iOldPre < (iPos * 100 / (iUpper - iLower)))//当百分比改变的时候再刷新区域
				{
					iOldPre = iPos * 100 / (iUpper - iLower);
					InvalidateRect(&rctxt,TRUE);
					//UpdateWindow();
				}
			}
			//CWnd * cwnd = this->GetParent( ) ;
		}
	}
	return oldpos;
}

void CMyProgressCtrl::SetRange( int nLower, int nUpper )
{	
	iLower			= nLower;
	iUpper			= nUpper;

	iRectFillRight	= 0;//刷新后的区域的right
	iOldPre			= 0;//保存原来的百分比数字,如果没有更新的话就不重新显示

	GetClientRect(&rFillin);
	GetClientRect(&rctxt);

	InvalidateRect(&rFillin,TRUE);
	UpdateWindow();
	
	int iRectWidth	= 35;//显示百分比的区域的宽
	int iRectHeight = 15;//显示百分比的区域的高

	rctxt.top		= (rctxt.bottom - rctxt.top - iRectHeight) / 2;
	rctxt.bottom	= rctxt.top + iRectHeight;
	rctxt.left		= (rctxt.right - rctxt.left - iRectWidth) / 2;
	rctxt.right		= rctxt.left + iRectWidth;
}

int CMyProgressCtrl::SetStep( int nStep )
{
	int oldstep = iStep;
	iStep = nStep;
	return (oldstep);
}

void CMyProgressCtrl::SetProgressFillColor(COLORREF crColor)
{
	crFillColor = crColor;
}

void CMyProgressCtrl::SetProgressTextColor(COLORREF crColor)
{
	crTextColor = crColor;
}

void CMyProgressCtrl::ShowPreText(BOOL bShow)
{
	bShowPre = bShow;
}
