#if !defined(AFX_DIZIBUTTON_H__6232FB4A_08E7_4D9F_87AF_A4B095B69315__INCLUDED_)
#define AFX_DIZIBUTTON_H__6232FB4A_08E7_4D9F_87AF_A4B095B69315__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiZiButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DiZiButton window
#define BUTTONSTATE_TIMER	1003

class DiZiButton : public CButton
{
// Construction
public:
	DiZiButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DiZiButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DiZiButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(DiZiButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
private:
    CBitmap ButtonState1;//按钮状态为抬起,并且鼠标没有在按钮之上
    CBitmap ButtonState2;//按钮状态为抬起,并且鼠标在按钮之上,鼠标没有按下
    CBitmap ButtonState3;//按钮状态为按下,并且鼠标没有松开
    CBitmap ButtonState4;//按钮为按下状态,鼠标已经抬起,并且不在按钮之上
    CBitmap ButtonState5;//按钮为按下状态,鼠标在按钮之上

    typedef enum
    {
        BUTTONSTATE1,//按钮状态为抬起,并且鼠标没有在按钮之上
        BUTTONSTATE2,//按钮状态为抬起,并且鼠标在按钮之上,鼠标没有按下
        BUTTONSTATE3,//按钮状态为按下,并且鼠标没有松开
        BUTTONSTATE4,//按钮为按下状态,鼠标已经抬起,并且不在按钮之上
        BUTTONSTATE5 //按钮为按下状态,鼠标在按钮之上
    }BUTTONSTATE;

    BUTTONSTATE bs;
    BOOL m_State;//按钮当前是否按下,TRUE:按下,FALSE:没有按下

    void DrawBitmap(CBitmap &DBitmap);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIZIBUTTON_H__6232FB4A_08E7_4D9F_87AF_A4B095B69315__INCLUDED_)
