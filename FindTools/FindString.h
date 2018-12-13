#if !defined(AFX_FINDSTRING_H__5B948F74_9374_4CA0_89BA_4FE6CC0657C4__INCLUDED_)
#define AFX_FINDSTRING_H__5B948F74_9374_4CA0_89BA_4FE6CC0657C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindString.h : header file
//
#include "FileInfo.h"
#include "ButtonEx.h"
/////////////////////////////////////////////////////////////////////////////
// FindString dialog
#define SCANING_TIMER		1001
class FindString : public CPropertyPage
{
	DECLARE_DYNCREATE(FindString)
private:
	CString	cScanPath;
	
	enum FINDINGSTATUS
	{
		FINDING,
		STOP
	};
	FINDINGSTATUS m_FindingStatus;

	int iCheckFast;//是否是快速扫描
	int iSTRMaxLen;

	int m_nTimer;
	int iUsedSecond;
	int iUsedMinute;
	int iUsedHour;

	long lUsedSecond;//单个字符串的扫描所耗费的时间

	static UINT FindingThread(LPVOID lpvData);
// Construction
protected:	
	CString GetValidString(CString cString,CString cStrPre);
	void AddFile(CString cStr,int iLineNum);
	int FindStr(CString filename,CString cStr);
	int FindStrNoraml(CString bufRead,CString cStr);

	int FindStrFast(CString filename,CString cStr);
public:
	FindString();
	~FindString();

	BOOL bScaning(void);
// Dialog Data
	//{{AFX_DATA(FindString)
	enum { IDD = IDD_DIALOG_FINDSTRING };
	CButtonEx	m_Button_SelectFold;
	CEdit	m_Edit_ListFiles;
	CEdit	m_ListString;
	CButtonEx	m_Check_Fast;
	CStatic	m_UsedTimer;
	CRichEditCtrl	m_RichEdit_ListString;
	CEdit	m_Edit_Prefix;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(FindString)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(FindString)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelectfold();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCheckFast();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDSTRING_H__5B948F74_9374_4CA0_89BA_4FE6CC0657C4__INCLUDED_)
