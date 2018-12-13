// FindToolsDlg.h : header file
//

#if !defined(AFX_FINDTOOLSDLG_H__41336782_8E9B_4D61_ACFA_1925127AC36C__INCLUDED_)
#define AFX_FINDTOOLSDLG_H__41336782_8E9B_4D61_ACFA_1925127AC36C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFindToolsDlg dialog
#include "FindChinese.h"
#include "FindString.h"
#include "MacroTool.h"
#include "FindHelp.h"
#include "DiZiButton.h"

class CFindToolsDlg : public CDialog
{
// Construction
public:
	FindChinese FCpage;
	FindString	FSpage;
	MacroTool	MTpage;
	FindHelp	FHpage;

	CPropertySheet FindToolSheet;

	CFindToolsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFindToolsDlg)
	enum { IDD = IDD_FINDTOOLS_DIALOG };
	DiZiButton	m__TD;
	DiZiButton	m_TuDing;
	CButton	m_Check_Front;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindToolsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFindToolsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCheckFront();
	virtual void OnCancel();
	//}}AFX_MSG
    afx_msg void OnNc(WPARAM wParam,LPARAM IParam);  // 最小化窗口的参数
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDTOOLSDLG_H__41336782_8E9B_4D61_ACFA_1925127AC36C__INCLUDED_)
