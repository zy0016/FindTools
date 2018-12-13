#if !defined(AFX_FINDCHINESE_H__9698410C_7831_40C7_8757_12FF0EF4CA7D__INCLUDED_)
#define AFX_FINDCHINESE_H__9698410C_7831_40C7_8757_12FF0EF4CA7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindChinese.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// FindChinese dialog
#include "FindChineseEx.h"
#include "SortListCtrl.h"
#include "MyProgressCtrl.h"
#include "FileInfo.h"
#include "ButtonEx.h"
#include "RichEditCtrlExt.h"
class FindChinese : public CPropertyPage
{
	DECLARE_DYNCREATE(FindChinese)
private:
	CString		cPath;
	enum FINDINGSTATUS
	{
		FINDING,
		STOP
	};
	FINDINGSTATUS m_FindingStatus;

	CStringList ListFuncEx;//例外函数列表
	CStringList ListFileEx;//例外文件列表
	CStringList ListFoldEx;//例外目录列表

	void ReadFunctionExList(void);//读取例外项列表
	BOOL IsInFunctionEx(CString code);//该行代码里的中文是否在例外项中
	BOOL IsInFileEx(CString FileName);//该文件是否在例外项中
	BOOL IsInFoldEx(CString FoldName);//该目录是否在例外项中

	void ReadFoldList(void);//读取目录列表
	BOOL SaveFoldList(CString sFold);//保存目录列表
	static int CALLBACK SetSelProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
// Construction

protected:
	HICON		m_hIcon;
	CImageList	imagelist;	//图象列表
	CHARFORMAT	chFormat;	//用于设置RichEdit中的字体格式
	LOGFONT		logfont;	//用于设置字体
	int			iFilenum;	//文件个数,文件个数的累加器

	int			iSortColumn;//保存当前被排序的列
	static	int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	int FindChineseCode(CString filename);
	void AddFile(CString filename,int iLineNum,CString code);
	CString GetFileName(int * pLineNum);
	void PreViewCode();
	static UINT FindingThread(LPVOID lpvData);

public:
	FindChinese();
	~FindChinese();

	BOOL bScaning(void);
// Dialog Data
	//{{AFX_DATA(FindChinese)
	enum { IDD = IDD_DIALOG_FINDCHINESE };
	CButtonEx	m_SelectFold;
	CButtonEx	m_Button_Scaning;
	CButtonEx	m_Exception;
	CButtonEx	m_Button_Font;
	CComboBox	m_Combox_SelectFold;
	CMyProgressCtrl	m_Progress;
	CSortListCtrl	m_ListFile;
	CStatic	m_Static_ScanResult;
	CRichEditCtrlExt	m_FilePreview;
	CButtonEx	m_SubFold;
	BOOL	m_bSubFold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(FindChinese)
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
	//{{AFX_MSG(FindChinese)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelectfold();
	afx_msg void OnButtonSetfont();
	afx_msg void OnButtonException();
	afx_msg void OnClickListFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonScaning();
	afx_msg void OnDropdownComboSelectfold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDCHINESE_H__9698410C_7831_40C7_8757_12FF0EF4CA7D__INCLUDED_)
