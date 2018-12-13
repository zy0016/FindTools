#if !defined(AFX_FINDHELP_H__EB401D10_7648_4053_BCF0_D6AE1E057F27__INCLUDED_)
#define AFX_FINDHELP_H__EB401D10_7648_4053_BCF0_D6AE1E057F27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindHelp.h : header file
//
#include "HyperLink.h"
#include "CreditStatic.h"

/////////////////////////////////////////////////////////////////////////////
// FindHelp dialog
#define SCANING_TIMER		1001
#define	PICX				20
#define PICY				20

#define ABOUT_CREDIT_TIMER	1003
class FindHelp : public CPropertyPage
{
	DECLARE_DYNCREATE(FindHelp)
private:
	int m_nTimer;
#if 0
	CBitmap cBitmap[4];
	//以下部分用于动态链接库
    HGLOBAL hGlobal[4];
    LPVOID  lpvoid[4];
#endif
    CBitmap *cbitmap[4];
    HBITMAP hbitmap[4];
    
	int iPic;
	int iPicW,iPicH;
	RECT rect;

	UINT m_nCreditTimer;
	CCreditStatic m_static;
// Construction
public:
	FindHelp();
	~FindHelp();

// Dialog Data
	//{{AFX_DATA(FindHelp)
	enum { IDD = IDD_DIALOG_HELP };
	CHyperLink	m_ctrlMyEmail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(FindHelp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(FindHelp)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDHELP_H__EB401D10_7648_4053_BCF0_D6AE1E057F27__INCLUDED_)
