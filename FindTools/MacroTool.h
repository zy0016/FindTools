#if !defined(AFX_MACROTOOL_H__3A555F52_9266_45FA_A7D4_F2C4EB432359__INCLUDED_)
#define AFX_MACROTOOL_H__3A555F52_9266_45FA_A7D4_F2C4EB432359__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacroTool.h : header file
//
#include "MyProgressCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// MacroTool dialog
class MacroTool : public CPropertyPage
{
	DECLARE_DYNCREATE(MacroTool)
private:
	enum DIRECTION
	{
		UP,
		DOWN
	};
	HICON		m_hIcon;
	CImageList	imagelist;//图象列表
	CHARFORMAT	chFormat;
	LOGFONT		logfont;
	CBitmap		cMBSingleOut;
	CBitmap		cMBDelete;
	CUIntArray	cSingList;//保存变色行位置的列表
	int			iCurrentLine;//保存当前行
	CString		cClew;

	CString GetMacroReplace(CString filename,CString cmacro);
	CString	GetMacroReplace(CString filename,CString cmacro,CRichEditCtrl * pRichEdit);
	int		IsMacro(CString bufRead,CString strMacro);
	int		GetMacroType(CString bufRead);
	BOOL	WriteFile(CString filename,CString cStr);
	BOOL	DeleteListCtrl();
	int		GetMacroPlace(DIRECTION direction) ;
	CString GetFileName();
	void	CSingList_Add(int iLineNum);
// Construction
public:
	MacroTool();
	~MacroTool();

// Dialog Data
	//{{AFX_DATA(MacroTool)
	enum { IDD = IDD_DIALOG_MACROTOOL };
	CMyProgressCtrl	m_Progress_Macro;
	CButton	m_Button_SelectFile;
	CEdit	m_Edit_Macro;
	CRichEditCtrl	m_FilePreview;
	CListCtrl	m_ListFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(MacroTool)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	// Generated message map functions
	//{{AFX_MSG(MacroTool)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelectfile();
	afx_msg void OnClickListMacroListfile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSingle();
	afx_msg void OnDblclkListMacroListfile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonSetfont();
	afx_msg void OnKeydownListMacroListfile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUp();
	afx_msg void OnMenuSingleout();
	afx_msg void OnMenuDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACROTOOL_H__3A555F52_9266_45FA_A7D4_F2C4EB432359__INCLUDED_)
