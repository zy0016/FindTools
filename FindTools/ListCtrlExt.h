#if !defined(AFX_LISTCTRLEXT_H__6FE17B30_B5C4_42A4_BE30_4935FFE9A4FB__INCLUDED_)
#define AFX_LISTCTRLEXT_H__6FE17B30_B5C4_42A4_BE30_4935FFE9A4FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlExt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlExt window
#include "ButtonEx.h"
#include "EditExt.h"

#define	IDC_CEDIT_FOLD			100
#define	IDC_CBUTTON_FOLD		110
#define	IDC_CBUTTON_CHECK		120

#define	CBUTTONW				30//按钮控件的宽
#define	CHECKWIDTH				100//复选按钮的宽
#define	CITEMHEIGHT				17//按钮和CEDIT控件所需要的高
#define	HEADERHEIGHT			17//标题栏的高

#define	FOLD_CONTAIN			"是"
#define	FOLD_NOTCONTAIN			"否"

#define LISTCTRL_INIT_WPARAM    3
#define LISTCTRL_INIT_LPARAM    1

#define TOOLTIP_ID				1

class CListCtrlExt : public CListCtrl
{
// Construction
public:
	CListCtrlExt();
	CString GetInvalidFoldName(void);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlExt)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
private:
    int iCurrentSel;//当前选择的行
	CButton CButton_Fold;
	CButtonEx CButton_Check;
	EditExt CEdit_Fold;

	RECT rEdit;
	RECT rButton_Fold;
	RECT rButtonCheck;

	CString cCurSelFoldName;
	CString cClewString;
	int		iCurSelItem;//当前正在操作的项目
	BOOL	bCurCheck;//是否选择子目录
	BOOL	bChange;//文本框中的内容是否已经改变
	BOOL	bValidFold;//非法目录的对话框是否已经显示过了
	BOOL	bEsc;//是否按下了ESC键
	BOOL    bEditing;//当前是否是编辑状态

	CString InvalidFold;//为空的时候表示没有非法目录,否则表示有非法目录,内容为提示信息

	CToolTipCtrl m_ToolTip; 
	CString csToolTip;

	BOOL IfValidFoldName(void);
	void SetData(void);

	static int CALLBACK SetSelProc(HWND hWnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData);
public:
	virtual ~CListCtrlExt();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlExt)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEXT_H__6FE17B30_B5C4_42A4_BE30_4935FFE9A4FB__INCLUDED_)
