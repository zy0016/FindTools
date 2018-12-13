#if !defined(AFX_EDITEXT_H__0D639C3E_EE65_45E4_8803_7C4423B9DBF2__INCLUDED_)
#define AFX_EDITEXT_H__0D639C3E_EE65_45E4_8803_7C4423B9DBF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditExt.h : header file
//
#define EDITEXT             0x100
#define EDITCHANGED         0x101
#define EDITRETURN          0x102
#define EDITESCAPE          0X103
/////////////////////////////////////////////////////////////////////////////
// EditExt window

class EditExt : public CEdit
{
// Construction
public:
	EditExt();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditExt)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~EditExt();

	// Generated message map functions
protected:
	//{{AFX_MSG(EditExt)
	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITEXT_H__0D639C3E_EE65_45E4_8803_7C4423B9DBF2__INCLUDED_)
