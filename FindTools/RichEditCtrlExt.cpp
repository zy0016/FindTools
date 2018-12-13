// RichEditCtrlExt.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "RichEditCtrlExt.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlExt

CRichEditCtrlExt::CRichEditCtrlExt()
{
}

CRichEditCtrlExt::~CRichEditCtrlExt()
{
}


BEGIN_MESSAGE_MAP(CRichEditCtrlExt, CRichEditCtrl)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CRichEditCtrlExt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlExt message handlers

void CRichEditCtrlExt::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

        CMenu m_Menu;
        CPoint cPoint = point;
        
        m_Menu.CreatePopupMenu();
        m_Menu.AppendMenu( MF_STRING, IDC_MENU_SETFONT, "ÉèÖÃ×ÖÌå(&F)" );
        //m_Menu.AppendMenu(MF_SEPARATOR,0);
        
        //SetCursor(LoadCursor(AfxGetInstanceHandle(), IDC_ARROW));
        m_Menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, cPoint.x, cPoint.y, this );

		/*CMenu menu;
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_RICH_EDIT_CTRL_EXT));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);*/
	}
}

BOOL CRichEditCtrlExt::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((IDC_MENU_SETFONT == wParam) && (0 == lParam))
    {
        return TRUE;
    }
	return CRichEditCtrl::OnCommand(wParam, lParam);
}
