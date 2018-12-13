#if !defined(AFX_SORTLISTCTRL_H__949025EA_50EA_4FEF_BC97_649F3A7A6217__INCLUDED_)
#define AFX_SORTLISTCTRL_H__949025EA_50EA_4FEF_BC97_649F3A7A6217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl window
#include "SortHeaderCtrl.h"

class CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();

	BOOL m_fAsc;//是否升序排列
	int m_nSortedCol;//当前排序的列
	void SetHeaderCtrlArrow(int iSortColumn,BOOL bSortAscending);
	void SetToolTipText(CString cToolTip);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSortListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSortListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	CSortHeaderCtrl my_SortHeaderCtrl;
	CToolTipCtrl m_ToolTip; 
	CString csToolTip;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTLISTCTRL_H__949025EA_50EA_4FEF_BC97_649F3A7A6217__INCLUDED_)
