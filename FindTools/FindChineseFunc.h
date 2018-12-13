#if !defined(AFX_FINDCHINESEFUNC_H__4224379D_2818_41EA_BA31_380C2285F84F__INCLUDED_)
#define AFX_FINDCHINESEFUNC_H__4224379D_2818_41EA_BA31_380C2285F84F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindChineseFunc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FindChineseFunc dialog
#include "FileInfo.h"
#include "ButtonEx.h"

class FindChineseFunc : public CPropertyPage
{
	DECLARE_DYNCREATE(FindChineseFunc)

// Construction
public:
	FindChineseFunc();
	~FindChineseFunc();

// Dialog Data
	//{{AFX_DATA(FindChineseFunc)
	enum { IDD = IDD_DIALOG_FINDCHINESE_FUNC };
	CButtonEx	m_Function_Del;
	CButtonEx	m_FunctionAdd;
	CEdit	m_Function;
	CListBox	m_FunctionList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(FindChineseFunc)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
private:
protected:
	// Generated message map functions
	//{{AFX_MSG(FindChineseFunc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFindchinesefuncAdd();
	afx_msg void OnButtonFindchinesefuncDel();
	afx_msg void OnChangeEditFindchinesefunc();
	afx_msg void OnSetfocusListFindchinesefunc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDCHINESEFUNC_H__4224379D_2818_41EA_BA31_380C2285F84F__INCLUDED_)
