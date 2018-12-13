#if !defined(AFX_BUTTONEX_H__7FA1A2B1_73F1_4CF6_BCB1_5579D910587E__INCLUDED_)
#define AFX_BUTTONEX_H__7FA1A2B1_73F1_4CF6_BCB1_5579D910587E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonEx.h : header file
//
#define BUTTONEX_KEYDOWNCHAR        0X600

#define LISTCTRLCHECK_CHECK         0x200
#define LISTCTRLCHECK_CHECKED       1
#define LISTCTRLCHECK_UNCHECKED     0
/////////////////////////////////////////////////////////////////////////////
// CButtonEx window
#define TOOLTIP_ID					1
class CButtonEx : public CButton
{
// Construction
public:
	CButtonEx();
    int iCheckId;
    int iClickId;
	//void SetToolTip(const CString cToolTip);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonEx)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONEX_H__7FA1A2B1_73F1_4CF6_BCB1_5579D910587E__INCLUDED_)
