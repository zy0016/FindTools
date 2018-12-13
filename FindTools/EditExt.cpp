// EditExt.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "EditExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditExt

EditExt::EditExt()
{
}

EditExt::~EditExt()
{
}


BEGIN_MESSAGE_MAP(EditExt, CEdit)
	//{{AFX_MSG_MAP(EditExt)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditExt message handlers

void EditExt::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CWnd* pcwnd;
	pcwnd = this->GetParent();

	pcwnd->PostMessage(WM_COMMAND,EDITEXT,EDITCHANGED);//通知父窗口文本内容改变
}

BOOL EditExt::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
        CWnd* pcwnd;
        pcwnd = this->GetParent();
		if(pMsg->wParam == VK_RETURN)
        {
            pcwnd->PostMessage(WM_COMMAND,EDITEXT,EDITRETURN);//通知父窗口收到回车键
            return TRUE;
        }
        if (pMsg->wParam == VK_ESCAPE)
        {
            pcwnd->PostMessage(WM_COMMAND,EDITEXT,EDITESCAPE);//通知父窗口收到ESC键
            return TRUE;
        }
	}
	return CEdit::PreTranslateMessage(pMsg);
}
