#if !defined(AFX_FINDCHINESEFOLD_H__678AECB9_CF0A_4FB3_BB65_038F27C75B33__INCLUDED_)
#define AFX_FINDCHINESEFOLD_H__678AECB9_CF0A_4FB3_BB65_038F27C75B33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindChineseFold.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FindChineseFold dialog
#include "ListCtrlExt.h"
#include "FileInfo.h"

class FindChineseFold : public CPropertyPage
{
	DECLARE_DYNCREATE(FindChineseFold)

// Construction
public:
	FindChineseFold();
	~FindChineseFold();

// Dialog Data
	//{{AFX_DATA(FindChineseFold)
	enum { IDD = IDD_DIALOG_FINDCHINESE_FOLD };
	CListCtrlExt	m_ListCtrlExt_Fold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(FindChineseFold)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
protected:
	// Generated message map functions
	//{{AFX_MSG(FindChineseFold)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDCHINESEFOLD_H__678AECB9_CF0A_4FB3_BB65_038F27C75B33__INCLUDED_)
