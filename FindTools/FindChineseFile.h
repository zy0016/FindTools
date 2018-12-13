#if !defined(AFX_FINDCHINESEFILE_H__0870A86A_7ABC_4841_8FFD_1E9359D91257__INCLUDED_)
#define AFX_FINDCHINESEFILE_H__0870A86A_7ABC_4841_8FFD_1E9359D91257__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindChineseFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FindChineseFile dialog
#include "ListBoxExt.h"
#include "FileInfo.h"

class FindChineseFile : public CPropertyPage
{
	DECLARE_DYNCREATE(FindChineseFile)

// Construction
public:
	FindChineseFile();
	~FindChineseFile();

// Dialog Data
	//{{AFX_DATA(FindChineseFile)
	enum { IDD = IDD_DIALOG_FINDCHINESE_FILE };
	ListBoxExt	m_FindChinese_File;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(FindChineseFile)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
protected:
	// Generated message map functions
	//{{AFX_MSG(FindChineseFile)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDCHINESEFILE_H__0870A86A_7ABC_4841_8FFD_1E9359D91257__INCLUDED_)
