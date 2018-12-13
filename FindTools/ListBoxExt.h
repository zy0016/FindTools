#if !defined(AFX_LISTBOXEXT_H__F03D723C_33D2_4A41_B7CD_7A784755AF0B__INCLUDED_)
#define AFX_LISTBOXEXT_H__F03D723C_33D2_4A41_B7CD_7A784755AF0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListBoxExt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ListBoxExt window
#include "EditExt.h"

#define	IDC_CEDIT_FILE			100
#define	IDC_CBUTTON_FILE		110
#define	CBUTTONW				30//按钮控件的宽
#define	CITEMHEIGHT				17//按钮和CEDIT控件所需要的高
#define BOX_INIT_WPARAM         1
#define BOX_INIT_LPARAM         2

#define TOOLTIP_ID				1
///////////////menu////////////////////////
#define IDC_MENU_ADDFILES		200
#define IDC_MENU_DELETEFILE		210
/*const MENUITEMTEMPLATE ListBoxExtPopMenu[] = 
{
	{MF_CHECKED,IDC_MENU_ADDFILES,(WCHAR)"添加多个文件"},
	{MF_END,0,NULL}
};
const MENUITEMTEMPLATEHEADER ListBoxExtPopMenuTemplate =
{
    0,
    (WORD)ListBoxExtPopMenu
};*/
/////////////////////////////////////////////
class ListBoxExt : public CListBox
{
// Construction
public:
	ListBoxExt();
	CString GetInvalidFileName(void);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ListBoxExt)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
private:
    int iCurrentSel;
	CButton CButton_File;
	EditExt CEdit_File;

	RECT rEdit;
	RECT rButton;

	CString cCurSelFileName;//保存文本框中的内容
	CString cClewString;//提示语
	BOOL	bChange;//文本框中的内容是否已经改变
    BOOL    bEditing;//当前是否是编辑状态
	BOOL	bValidFile;//非法文件的对话框是否已经显示过了

	BOOL	bEsc;//是否按下了ESC键

	CToolTipCtrl m_ToolTip; 
	CString csToolTip;

	CString	InvalidFile;//为空的时候表示没有非法文件,否则表示有非法文件,内容为提示信息
	void SetData(void);
	BOOL IfValidFileName(void);
    void AddFiles(void);
public:
	virtual ~ListBoxExt();

	// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);

	//{{AFX_MSG(ListBoxExt)
	afx_msg void OnDblclk();
	afx_msg void OnSetfocus();
	afx_msg void OnSelchange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXEXT_H__F03D723C_33D2_4A41_B7CD_7A784755AF0B__INCLUDED_)
