#if !defined(AFX_RICHEDITCTRLEXT_H__54DDB9AB_04F0_487B_8305_A4C228733D54__INCLUDED_)
#define AFX_RICHEDITCTRLEXT_H__54DDB9AB_04F0_487B_8305_A4C228733D54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RichEditCtrlExt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlExt window
#define IDC_MENU_SETFONT		200
//#define IDC_MENU_DELETEFILE		210

class CRichEditCtrlExt : public CRichEditCtrl
{
// Construction
public:
	CRichEditCtrlExt();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditCtrlExt)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRichEditCtrlExt();

	// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CRichEditCtrlExt)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHEDITCTRLEXT_H__54DDB9AB_04F0_487B_8305_A4C228733D54__INCLUDED_)
