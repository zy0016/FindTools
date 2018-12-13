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
    CBitmap ButtonState1;//��ť״̬Ϊ̧��,�������û���ڰ�ť֮��
    CBitmap ButtonState2;//��ť״̬Ϊ̧��,��������ڰ�ť֮��,���û�а���
    CBitmap ButtonState3;//��ť״̬Ϊ����,�������û���ɿ�
    CBitmap ButtonState4;//��ťΪ����״̬,����Ѿ�̧��,���Ҳ��ڰ�ť֮��
    CBitmap ButtonState5;//��ťΪ����״̬,����ڰ�ť֮��

    typedef enum
    {
        BUTTONSTATE1,//��ť״̬Ϊ̧��,�������û���ڰ�ť֮��
        BUTTONSTATE2,//��ť״̬Ϊ̧��,��������ڰ�ť֮��,���û�а���
        BUTTONSTATE3,//��ť״̬Ϊ����,�������û���ɿ�
        BUTTONSTATE4,//��ťΪ����״̬,����Ѿ�̧��,���Ҳ��ڰ�ť֮��
        BUTTONSTATE5 //��ťΪ����״̬,����ڰ�ť֮��
    }BUTTONSTATE;

    BUTTONSTATE bs;
    BOOL m_State;//��ť��ǰ�Ƿ���,TRUE:����,FALSE:û�а���

    void DrawBitmap(CBitmap &DBitmap);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIZIBUTTON_H__6232FB4A_08E7_4D9F_87AF_A4B095B69315__INCLUDED_)
