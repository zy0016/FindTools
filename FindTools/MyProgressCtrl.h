#if !defined(AFX_MYPROGRESSCTRL_H__E59ED7C7_B805_4C50_929C_47E5EF0A67ED__INCLUDED_)
#define AFX_MYPROGRESSCTRL_H__E59ED7C7_B805_4C50_929C_47E5EF0A67ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyProgressCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyProgressCtrl window

class CMyProgressCtrl : public CProgressCtrl
{
private:
	int	iPos;	// The current position
	int iLower;
	int iUpper;
	int iStep;

	CRect rFresh;//需要刷新的区域
	CRect rFillin;//填充进度条的区域
	CRect rctxt;//显示文字的区域

	int iRectFillRight;//刷新后的区域的right
	int iOldPre;//保存原来的百分比数字,如果没有更新的话就不重新显示
	COLORREF crFillColor;
	COLORREF crTextColor;
	BOOL bShowPre;//是否显示百分比
// Construction
public:
	CMyProgressCtrl();

// Attributes
public:
	int SetPos( int nPos );
	void SetRange( int nLower, int nUpper );
	int SetStep( int nStep );
	void SetProgressFillColor(COLORREF crColor);
	void SetProgressTextColor(COLORREF crColor);
	void ShowPreText(BOOL bShow);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyProgressCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyProgressCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyProgressCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROGRESSCTRL_H__E59ED7C7_B805_4C50_929C_47E5EF0A67ED__INCLUDED_)
