#if !defined(AFX_FINDCHINESEEX_H__347EA287_E8FE_406B_9BDC_1E43AEC4961A__INCLUDED_)
#define AFX_FINDCHINESEEX_H__347EA287_E8FE_406B_9BDC_1E43AEC4961A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindChineseEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FindChineseEx dialog

#include "FindChinese.h"
#include "FindChineseFunc.h"
#include "FindChineseFile.h"
#include "FindChineseFold.h"
#include "FileInfo.h"

class FindChineseEx : public CDialog
{
// Construction
public:
	FindChineseEx(CWnd* pParent = NULL);   // standard constructor
	~FindChineseEx();
	//FindChineseEx();
// Dialog Data
	//{{AFX_DATA(FindChineseEx)
	enum { IDD = IDD_DIALOG_FINDCHINESEEX };
	CButton	m_OK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FindChineseEx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
private:
	FindChineseFunc FCF;
	FindChineseFile FCFile;
	FindChineseFold	FCFold;

	CPropertySheet FindChineseSheet;

	CStringList FileList;//文件例外项列表
	CStringList FoldList;//目录例外项列表
protected:

	// Generated message map functions
	//{{AFX_MSG(FindChineseEx)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDCHINESEEX_H__347EA287_E8FE_406B_9BDC_1E43AEC4961A__INCLUDED_)
