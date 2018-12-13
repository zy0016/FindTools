#if !defined(AFX_SORTHEADERCTRL_H__6C8C7174_516D_4764_8CEF_E0F9A82EFF08__INCLUDED_)
#define AFX_SORTHEADERCTRL_H__6C8C7174_516D_4764_8CEF_E0F9A82EFF08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortHeaderCtrl window

class CSortHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CSortHeaderCtrl();
	void SetSortArrow( const int iColumn, const BOOL bAscending );
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSortHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSortHeaderCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	int m_iSortColumn;
	BOOL m_bSortAscending;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTHEADERCTRL_H__6C8C7174_516D_4764_8CEF_E0F9A82EFF08__INCLUDED_)
