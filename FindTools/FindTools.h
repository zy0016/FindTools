// FindTools.h : main header file for the FINDTOOLS application
//

#if !defined(AFX_FINDTOOLS_H__F8DB3CCB_AD57_4F5C_8B23_173E6971E837__INCLUDED_)
#define AFX_FINDTOOLS_H__F8DB3CCB_AD57_4F5C_8B23_173E6971E837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFindToolsApp:
// See FindTools.cpp for the implementation of this class
//

class CFindToolsApp : public CWinApp
{
public:
	CFindToolsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindToolsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFindToolsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDTOOLS_H__F8DB3CCB_AD57_4F5C_8B23_173E6971E837__INCLUDED_)
