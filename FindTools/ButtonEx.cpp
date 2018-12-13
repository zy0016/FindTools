// ButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "ButtonEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonEx

CButtonEx::CButtonEx()
{
    iCheckId = 0;
    iClickId = 0;
}

CButtonEx::~CButtonEx()
{
}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	//{{AFX_MSG_MAP(CButtonEx)
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonEx message handlers

void CButtonEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd* pcwnd;
	pcwnd = this->GetParent();

	pcwnd->PostMessage(WM_COMMAND,BUTTONEX_KEYDOWNCHAR,nChar);//通知父窗口

	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CButtonEx::OnClicked() 
{
	// TODO: Add your control notification handler code here
	CWnd* pcwnd;
	pcwnd = this->GetParent();

    UINT Style;

    Style = GetButtonStyle();
    if (Style & BS_AUTOCHECKBOX)
    {
        if (GetCheck() == 1)
            pcwnd->PostMessage(WM_COMMAND,LISTCTRLCHECK_CHECK,LISTCTRLCHECK_CHECKED);//通知父窗口
        else
            pcwnd->PostMessage(WM_COMMAND,LISTCTRLCHECK_CHECK,LISTCTRLCHECK_UNCHECKED);//通知父窗口

        pcwnd->PostMessage(WM_COMMAND,iCheckId,BN_CLICKED);
    }
	else
    {
        pcwnd->PostMessage(WM_COMMAND,iClickId,BN_CLICKED);
    }
}
