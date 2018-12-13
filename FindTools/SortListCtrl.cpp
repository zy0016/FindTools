// SortListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "findtools.h"
#include "SortListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLTIP_ID 1

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl

CSortListCtrl::CSortListCtrl()
{
	csToolTip = "单击标题栏可以排序(目前还有些问题)";
}

CSortListCtrl::~CSortListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl message handlers

void CSortListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CListCtrl::PreSubclassWindow();

	CRect rect; 
    GetClientRect(rect);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this,csToolTip , rect, TOOLTIP_ID);//"ToolTip"

	my_SortHeaderCtrl.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() ) ;
}
void CSortListCtrl::SetHeaderCtrlArrow(int iSortColumn,BOOL bSortAscending)
{
	my_SortHeaderCtrl.SetSortArrow(iSortColumn,bSortAscending);
}

BOOL CSortListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent(pMsg);
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CSortListCtrl::SetToolTipText(CString cToolTip)
{
	csToolTip = cToolTip;
}
