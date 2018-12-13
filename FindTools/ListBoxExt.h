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
#define	CBUTTONW				30//��ť�ؼ��Ŀ�
#define	CITEMHEIGHT				17//��ť��CEDIT�ؼ�����Ҫ�ĸ�
#define BOX_INIT_WPARAM         1
#define BOX_INIT_LPARAM         2

#define TOOLTIP_ID				1
///////////////menu////////////////////////
#define IDC_MENU_ADDFILES		200
#define IDC_MENU_DELETEFILE		210
/*const MENUITEMTEMPLATE ListBoxExtPopMenu[] = 
{
	{MF_CHECKED,IDC_MENU_ADDFILES,(WCHAR)"��Ӷ���ļ�"},
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

	CString cCurSelFileName;//�����ı����е�����
	CString cClewString;//��ʾ��
	BOOL	bChange;//�ı����е������Ƿ��Ѿ��ı�
    BOOL    bEditing;//��ǰ�Ƿ��Ǳ༭״̬
	BOOL	bValidFile;//�Ƿ��ļ��ĶԻ����Ƿ��Ѿ���ʾ����

	BOOL	bEsc;//�Ƿ�����ESC��

	CToolTipCtrl m_ToolTip; 
	CString csToolTip;

	CString	InvalidFile;//Ϊ�յ�ʱ���ʾû�зǷ��ļ�,�����ʾ�зǷ��ļ�,����Ϊ��ʾ��Ϣ
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
