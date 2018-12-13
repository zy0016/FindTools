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

#define	CBUTTONW				30//��ť�ؼ��Ŀ�
#define	CHECKWIDTH				100//��ѡ��ť�Ŀ�
#define	CITEMHEIGHT				17//��ť��CEDIT�ؼ�����Ҫ�ĸ�
#define	HEADERHEIGHT			17//�������ĸ�

#define	FOLD_CONTAIN			"��"
#define	FOLD_NOTCONTAIN			"��"

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
    int iCurrentSel;//��ǰѡ�����
	CButton CButton_Fold;
	CButtonEx CButton_Check;
	EditExt CEdit_Fold;

	RECT rEdit;
	RECT rButton_Fold;
	RECT rButtonCheck;

	CString cCurSelFoldName;
	CString cClewString;
	int		iCurSelItem;//��ǰ���ڲ�������Ŀ
	BOOL	bCurCheck;//�Ƿ�ѡ����Ŀ¼
	BOOL	bChange;//�ı����е������Ƿ��Ѿ��ı�
	BOOL	bValidFold;//�Ƿ�Ŀ¼�ĶԻ����Ƿ��Ѿ���ʾ����
	BOOL	bEsc;//�Ƿ�����ESC��
	BOOL    bEditing;//��ǰ�Ƿ��Ǳ༭״̬

	CString InvalidFold;//Ϊ�յ�ʱ���ʾû�зǷ�Ŀ¼,�����ʾ�зǷ�Ŀ¼,����Ϊ��ʾ��Ϣ

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
