// FindString.cpp : implementation file
//

#include "stdafx.h"
#include "FindTools.h"
#include "FindString.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindString property page

IMPLEMENT_DYNCREATE(FindString, CPropertyPage)

FindString::FindString() : CPropertyPage(FindString::IDD)
{
	//{{AFX_DATA_INIT(FindString)
	//}}AFX_DATA_INIT
}

FindString::~FindString()
{
}

void FindString::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindString)
	DDX_Control(pDX, IDC_BUTTON_SELECTFOLD, m_Button_SelectFold);
	DDX_Control(pDX, IDC_EDIT_FILES, m_Edit_ListFiles);
	DDX_Control(pDX, IDC_EDIT_STRING, m_ListString);
	DDX_Control(pDX, IDC_CHECK_FAST, m_Check_Fast);
	DDX_Control(pDX, IDC_STATIC_USEDTIMER, m_UsedTimer);
	DDX_Control(pDX, IDC_EDIT_PREFIX, m_Edit_Prefix);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindString, CPropertyPage)
	//{{AFX_MSG_MAP(FindString)
	ON_BN_CLICKED(IDC_BUTTON_SELECTFOLD, OnButtonSelectfold)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_FAST, OnCheckFast)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindString message handlers

BOOL FindString::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_Edit_ListFiles.SetWindowText("���������ַ���ǰ׺,Ȼ��ѡ��Ŀ¼");

    m_FindingStatus = STOP;
    m_Button_SelectFold.iClickId = IDC_BUTTON_SELECTFOLD;
    m_Button_SelectFold.iCheckId = IDC_BUTTON_SELECTFOLD;
    m_Check_Fast.iCheckId        = IDC_CHECK_FAST;
    m_Check_Fast.iClickId        = IDC_CHECK_FAST;
	// TODO: Add extra initialization here
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FindString::OnButtonSelectfold() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi; 
	ITEMIDLIST * pidl; 
	char Dir[MAX_PATH] = "";
	CString cStrPre;
	
	if(m_FindingStatus == FINDING)
	{
		if(MessageBox("����ɨ���ַ���,�Ƿ�ֹͣ?", "��ʾ", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDNO)
		{
			return ;
		}
		KillTimer(m_nTimer);
		m_nTimer = NULL;

		m_FindingStatus = STOP;

        CString cSelectFold;
        cSelectFold.LoadString(IDS_STRING_SELECTFOLD);
		m_Button_SelectFold.SetWindowText(cSelectFold);//ѡ��Ŀ¼...
		m_Check_Fast.EnableWindow(TRUE);
		return;
	}
	m_Edit_Prefix.GetWindowText(cStrPre);//����ַ���ǰ׺
	cStrPre.TrimLeft();
	cStrPre.TrimRight();
	cStrPre.MakeUpper();
	if (cStrPre == "")
	{
		MessageBox("���������ַ���ǰ׺,�ַ���ǰ׺��ʽΪSTR_...","��ʾ",MB_ICONINFORMATION|MB_OK);
		m_Edit_Prefix.SetFocus();
		return;
	}

	bi.hwndOwner		= this->GetSafeHwnd();; 
	bi.pidlRoot			= NULL; 
	bi.pszDisplayName	= Dir; 
	bi.lpszTitle		= "ѡ��Ŀ¼������[ȷ��]��ť"; 
	bi.ulFlags			= BIF_RETURNONLYFSDIRS; 
	bi.lpfn				= NULL; 
	bi.lParam			= 0; 
	bi.iImage			= 0; 
	
	pidl = SHBrowseForFolder(&bi);

	if(pidl == NULL) 
		memset(Dir,0,sizeof(MAX_PATH));
	if(SHGetPathFromIDList(pidl,Dir))/*Retrieve folder name from ITEMLIST structure.*/ 
	{
		CWaitCursor wait;
		int i;

		CString AllString = "������û���õ����ַ���";
		for (i = 0;i < 16;i++)
			AllString = AllString + " ";

		iSTRMaxLen	= AllString.GetLength();
		AllString	= AllString + "str_hp.h�е�λ��(��)\t\tɨ����ַ�����ʱ(��)\r\n";
		//AllString = "������û���õ����ַ���\t\t\t\tstr_hp.h�е�λ��(��)\t\tɨ����ַ�����ʱ(��)\r\n";
		m_ListString.SetWindowText(AllString);

        CString cStop;
        cStop.LoadString(IDS_STRING_STOP);

		m_Button_SelectFold.SetWindowText(cStop);
		m_Check_Fast.EnableWindow(FALSE);

		cScanPath		= Dir;		//��¼Ŀ¼
		m_FindingStatus	= FINDING;	//��¼��ǰ״̬
		m_nTimer		= SetTimer(SCANING_TIMER, 1000, NULL);//�򿪼�ʱ����ʾʱ��
		iUsedSecond		= 0;//ɨ���ʱ������
		iUsedMinute		= 0;
		iUsedHour		= 0;

		iCheckFast		= m_Check_Fast.GetCheck();
		CWinThread* pThread = AfxBeginThread(FindingThread,this);//��ʼ�߳�

        m_Button_SelectFold.SetFocus();
	}
}
//****************************�̲߳�������***************************************
UINT FindString::FindingThread(LPVOID lpvData)
{
	FindString* pThis = (FindString*)lpvData;

	BOOL		bWorking;
	CFileFind	find;
	CString		strFilePath,strPath,strFolder;
	CString		filename,cExpend,cFName;
	int			iExpend;
	CStringList listPaths;

	CStdioFile	cScanFile;		//����ɨ����ļ�
	CString		bufRead,cStrPre;//һ������,�ַ���ǰ׺
	CString		exPath1;		//��ɨ���Ŀ¼
	CString		exPath2;		//��ɨ���Ŀ¼
	short		iStringNum;		//��¼ɨ����ַ����ĸ���
	short		iFileCurLine;	//�ļ���ǰ��
	BOOL		bFind;			//�Ƿ��ڴ������ҵ��˸��ַ���

	filename = pThis->cScanPath + "\\include\\str_plx.h";//�ַ����ļ�

	if (!cScanFile.Open(filename,CFile::modeRead,NULL))//��ʧ��
	{
		pThis->KillTimer(pThis->m_nTimer);
		pThis->m_nTimer = NULL;

		pThis->m_FindingStatus = STOP;

        CString cSelectFold;
        cSelectFold.LoadString(IDS_STRING_SELECTFOLD);
		pThis->m_Button_SelectFold.SetWindowText(cSelectFold);//ѡ��Ŀ¼...
		pThis->m_Check_Fast.EnableWindow(TRUE);

		AfxMessageBox("�Ҳ���" + filename + "�ļ�",MB_ICONINFORMATION|MB_OK);
		return 0;
	}

	pThis->m_Edit_Prefix.GetWindowText(cStrPre);//����ַ���ǰ׺
	cStrPre.TrimLeft();
	cStrPre.TrimRight();
	cStrPre.MakeUpper();

	if (cStrPre.Right(1) != "_")//ȥ���ַ�����β��"_"
		cStrPre.Left(cStrPre.GetLength() - 1);

	iFileCurLine	= 0;								//�ļ���ǰ��
	iStringNum		= 0;								//��¼ɨ����ַ����ĸ��� 
	exPath1			= pThis->cScanPath + "\\include";	//��¼����Ҫɨ��Ŀ¼
	exPath2			= pThis->cScanPath + "\\include20";	//��¼����Ҫɨ��Ŀ¼

	while (cScanFile.ReadString(bufRead) && 
			(pThis->m_FindingStatus == FINDING))//ɨ��ÿһ���ַ�������
	{
		iFileCurLine++;//��¼��ǰ�ַ�����str_hp.h�ļ��е�λ��
		bufRead = pThis->GetValidString(bufRead,cStrPre);//���غϷ����ַ���
		if (bufRead == "")
			continue;

		iStringNum++;	//��¼ɨ����ַ����ĸ���
		bFind = FALSE;	//�Ƿ��ڴ������ҵ��˸��ַ���

		pThis->lUsedSecond = 0;//�����ַ�����ɨ�����ķѵ�ʱ��
		listPaths.RemoveAll();

		strPath = pThis->cScanPath + "\\*.*";
		do
		{
			bWorking = find.FindFile(strPath);
			while (bWorking && (pThis->m_FindingStatus == FINDING))
			{
				bWorking	= find.FindNextFile();
				strFilePath = find.GetFilePath();
				
				if (find.IsDots())
					continue;
				
				if (find.IsDirectory() && 
					(exPath1 != find.GetFilePath()) &&
					(exPath2 != find.GetFilePath()))
				{
					listPaths.AddHead(strFilePath);
					continue;
				}
				else if ((exPath1 != find.GetFilePath()) && (exPath2 != find.GetFilePath()))
				{
                    cFName = find.GetFileName();
                    if ((cFName == "str_plx.h") || (cFName == "str_plx_chn.h") || (cFName == "str_plx_eng.h"))
                        continue;

					filename = find.GetFilePath();			//����ļ���(����·��)
					iExpend	 = filename.ReverseFind('.');	//�����չ����ߵ�"."
					cExpend  = filename.Right(filename.GetLength() - iExpend - 1);//�����չ��
					cExpend.MakeLower();
					if ((cExpend == "c") || (cExpend == "h") || (cExpend == "cpp"))
					{
						if (1 == pThis->iCheckFast)//����ɨ��
						{
							if (1 == pThis->FindStrFast(filename,bufRead))
							{
								bFind = TRUE;
								break;//�˳�whileѭ��
							}
						}
						else//һ��ɨ��
						{
							if (1 == pThis->FindStr(filename,bufRead))
							{
								bFind = TRUE;
								break;//�˳�whileѭ��
							}
						}
					}
				}//end if
			}//end while
			if (bFind)//�ڴ������ҵ��˸��ַ���,�˳� do whileѭ��
				break;

			if(listPaths.IsEmpty())
				break;

			strFolder	= listPaths.RemoveHead();
			strPath		= strFolder + "\\*.*";
			find.Close();
		}while (pThis->m_FindingStatus == FINDING);//end do while

		if (bFind)		//�ڴ������ҵ����ַ���
			continue;	//����ɨ����һ���ַ���
		else if (pThis->m_FindingStatus == FINDING) //�ڴ�����û�ҵ����ַ���
			pThis->AddFile(bufRead,iFileCurLine);   //��¼����ַ���

	}//end while
	cScanFile.Close();
	if(pThis->m_nTimer!=NULL)
	{
		pThis->KillTimer(pThis->m_nTimer);
		pThis->m_nTimer = NULL;
	}
	pThis->m_FindingStatus = STOP;//��ǰΪֹͣ״̬
	
	CString cStrResult;
	cStrResult.Format("ɨ��%d���ַ���,�����й���%d�����õ��ַ���.",
		iStringNum,pThis->m_ListString.GetLineCount() - 2);

	pThis->m_Edit_ListFiles.SetWindowText(cStrResult);//��ʾɨ����

    CString cSelectFold;
    cSelectFold.LoadString(IDS_STRING_SELECTFOLD);
	pThis->m_Button_SelectFold.SetWindowText(cSelectFold);//ѡ��Ŀ¼...
	pThis->m_Check_Fast.EnableWindow(TRUE);

	sndPlaySound(SOUNDFILENAME,SND_ASYNC);

	return 1;
}
//���غϷ����ַ���
CString FindString::GetValidString(CString cString,CString cStrPre)
{
	CString cStrResult = "";//�Ϸ����ַ���

	cString.TrimLeft();
	cString.TrimRight();

	if (cString.Left(cStrPre.GetLength()) != cStrPre)//ǰ׺��ȵ��ַ����Ž���ɨ��
		return cStrResult;
	
	cStrResult = cString.Left(cString.Find(","));//��ö�����ߵ��ַ���
	if (cStrResult.Find("=") != -1)
		cStrResult = cStrResult.Left(cStrResult.Find("="));//��õȺ���ߵ��ַ���
	
	cStrResult.TrimRight();

	return cStrResult;
}
//��һ���һ�д���(û�е�����,˫����)�в����ַ���,
//����1��ʾ�ҵ�,����0��ʾû�ҵ�,����-1��ʾ��Ҫ�����µĴ����м���ɨ��
int FindString::FindStrNoraml(CString bufRead,CString cStr)
{
	int	iNotePlace;			//��õ���ע�͵�λ��
	int	iNotesPlaceBegin;	//����ע��/*��λ��
	int iNotesPlaceEnd;		//����ע��*/��λ��

	iNotesPlaceBegin	= bufRead.Find("/*");
	iNotesPlaceEnd		= bufRead.Find("*/");
	iNotePlace			= bufRead.Find("//");
	//û�е���ע��,û�ж���ע��
	if ((iNotePlace == -1) && (iNotesPlaceBegin == -1) && (iNotesPlaceEnd == -1))
	{
		return ((bufRead.Find(cStr) != -1) ? 1 : 0);
	}
	//û�е���ע��,�ж���ע��
	if ((iNotePlace == -1) && ((iNotesPlaceBegin != -1) || (iNotesPlaceEnd != -1)))
	{
		return -1;
	}
	//�е���ע��,û�ж���ע��
	if ((iNotePlace != -1) && (iNotesPlaceBegin == -1) && (iNotesPlaceEnd == -1))
	{
		bufRead = bufRead.Left(iNotePlace);//���ע����ߵĴ���
		return ((bufRead.Find(cStr) != -1) ? 1 : 0);
	}
	//�е���ע��,�ж���ע��
	if ((iNotePlace != -1) && ((iNotesPlaceBegin != -1) || (iNotesPlaceEnd != -1)))
	{
		if ((iNotesPlaceBegin != -1) && (iNotesPlaceEnd == -1))//��"/*",û��"*/"
		{
			if (iNotesPlaceBegin < iNotePlace)//����ע���ڵ���ע�͵����,����ʵ�����Ƕ���ע��
				return -1;
			if (iNotesPlaceBegin > iNotesPlaceBegin)//����ע���ڵ���ע�͵��ұ�,����ʵ�����ǵ���ע��
			{
				bufRead = bufRead.Left(iNotePlace);//���ע����ߵĴ���
				return ((bufRead.Find(cStr) != -1) ? 1 : 0);
			}
		}
		else if ((iNotesPlaceBegin == -1) && (iNotesPlaceEnd != -1))//��"*/",û��"/*"
		{
			//�п����ǵ���ע��,Ҳ�����Ƕ���ע�͵Ľ���
			return -1;
		}
	}
	return -1;
}
//��ʼ���ļ��в����ַ���,�ҵ�����1,���򷵻�0
//filename:�ļ���
int FindString::FindStr(CString filename,CString cStr)
{
	CStdioFile	file;
	CString		bufRead,bufLine;	
	LPTSTR		lptStr,lptStrNext;
	BOOL		bNotesEnd;		//�Ƿ���"//"�Ժ��һ�д����д���"*/"
	BOOL		bNote;			//�Ƿ�ע�Ϳ�ʼ("/*"����ע��)
	BOOL		bS,bD;			//�Ƿ��е�����,�Ƿ���˫����
	int			iNormal;

	if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
	{
		bNote = FALSE;
		m_Edit_ListFiles.SetWindowText("����ɨ��" + cStr + " ɨ��λ��:" + filename);
		
		while (file.ReadString(bufRead))//��ʼһ��һ��ɨ�����
		{
			bufRead.TrimLeft();
			bufRead.TrimRight();

			if (!bNote)//���ڶ���ע����
			{			
				if ((bufRead.Find("\'") == -1) && (bufRead.Find("\"") == -1))//û�е�����,û��˫����
				{	//��һ���һ�д���(û�е�����,˫����)�в����ַ���,
					iNormal = FindStrNoraml(bufRead,cStr);
					if (iNormal == 1)//����һ�����ҵ����ַ���
					{
						file.Close();
						return 1;
					}
					else if (iNormal == 0)//����һ����û�ҵ��ַ���,
					{
						continue;
					}
				}
			}
			bS			= FALSE;		//û�е�����
			bD			= FALSE;		//û��˫����
			lptStr		= NULL;
			lptStrNext	= NULL;
			lptStr		= bufRead.GetBuffer(bufRead.GetLength());
			bufLine		= "";//���һ��û��ע�͵Ĵ�����

			while (* lptStr != 0)//��һ�д����н���ɨ��
			{
				if (* lptStr == 34)//��ǰ�ַ���˫����
					bD = !bD;
				else if (* lptStr == 39)//��ǰ�ַ�������
					bS = !bS;
				else
				{
					if (* lptStr == 47)//��ǰ�ַ���"/"
					{
						lptStrNext = lptStr;
						lptStrNext++;
						if (* lptStrNext == 47)//��һ���ַ���"/"
						{
							if ((!bS) && (!bD))//��"//"֮ǰû�е�����û��˫���ű�ʾ���"//"��ע��,�˳���һ�е�ɨ��
							{
								//�����������,"//...*/"��ʱ����һ���Ƕ���ע�͵Ľ���
								lptStr += 2;
								bNotesEnd = FALSE;
								while (* lptStr != 0)
								{
									if (* lptStr == 42)//��ǰ�ַ���"*"
									{
										lptStrNext = lptStr;
										lptStrNext++;
										if ((* lptStrNext == 47) && (bNote))//��һ���ַ���"/"�����ж���ע��
										{
											bNote = FALSE;//��ʾ����ע�ͽ���
											bNotesEnd = TRUE;
											lptStr += 2;
											break;
										}
									}
									lptStr++;
								}
								if (bNotesEnd)//����"//"�Ժ��һ�д����д���"*/"��Ҫ����һ���м���ɨ��
									break;
							}
						}
						else if (* lptStrNext == 42)//��һ���ַ���"*"
						{
							if ((!bS) && (!bD))//��"/*"֮ǰû�е�����û��˫���ű�ʾ���"/*"��ע��
							{
								lptStr++;
								bNote = TRUE;//��ʾ����ע�Ϳ�ʼ
								continue;
							}							
						}
					}
					else if (* lptStr == 42)//��ǰ�ַ���"*"
					{
						lptStrNext = lptStr;
						lptStrNext++;
						if ((* lptStrNext == 47) && (bNote))//��һ���ַ���"/"�����ж���ע��
						{
							bNote = FALSE;//��ʾ����ע�ͽ���
							lptStr += 2;
							continue;
						}
					}
				}//end if
				if (* lptStr != 0)
				{
					if ((unsigned char)(* lptStr) > 0x80)
					{
						if (!bNote)
						{
							bufLine += (CString)(* lptStr++);
							bufLine += (CString)(* lptStr++);
						}
						else
						{
							lptStr += 2;
						}
					}
					else
					{
						if (!bNote)
							bufLine += (CString)(* lptStr++);
						else
							lptStr++;
					}
				}
			}//end while(* lptStr != 0)//��һ�д����н���ɨ��

			if (bufLine.Find(cStr) != -1)//�ҵ�������ַ���
			{
				file.Close();
				return 1;
			}
            bufRead.ReleaseBuffer();
		}//end while
		file.Close();
	}//end if

	return 0;
}

void FindString::AddFile(CString cStr,int iLineNum)//���ַ���,�к���ӵ�listctrl�ؼ���
{
	CString cLineNum,cfile,cUsedTime;
	int iLength,i;
	long iTextLength;
	CString cResult;
	
	cLineNum.Format("%d",iLineNum);
	cUsedTime.Format("%d",lUsedSecond);

	lUsedSecond = 0;
	cResult		= cStr;
	iLength		= cStr.GetLength();
	
	for (i = iLength; i < iSTRMaxLen;i++)
	{
		cResult = cResult + " ";
	}
	if (cLineNum.GetLength() == 1)
		cResult = cResult + cLineNum + "\t\t\t\t\t" + cUsedTime + "\r\n";
	else
		cResult = cResult + cLineNum + "\t\t\t\t" + cUsedTime + "\r\n";

	iTextLength = m_ListString.GetWindowTextLength();
	m_ListString.SetSel(iTextLength, iTextLength);
	m_ListString.ReplaceSel(cResult);
}

void FindString::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString cTime;

	if (m_FindingStatus == STOP)
	{
		KillTimer(SCANING_TIMER);
		m_nTimer = NULL;
		return;
	}

	lUsedSecond++;//�����ַ�����ɨ�����ķѵ�ʱ��

	iUsedSecond++;
	if (iUsedSecond == 60)
	{
		iUsedMinute++;
		if (iUsedMinute == 60)
		{
			iUsedMinute = 0;
			iUsedHour++;
		}
		iUsedSecond = 0;
	}

	cTime.Format("�Ѿ�ɨ����%02dСʱ%02d����%02d��",iUsedHour,iUsedMinute,iUsedSecond);
	m_UsedTimer.SetWindowText(cTime);
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL FindString::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD style = m_Button_SelectFold.GetButtonStyle();
    style = style | BS_DEFPUSHBUTTON;
    m_Button_SelectFold.SetButtonStyle(style,TRUE);

	return CPropertyPage::OnSetActive();
}

void FindString::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	if (bShow)
		m_Edit_Prefix.SetFocus();
}
//��ʼ���ļ��в����ַ���,�ҵ�����1,���򷵻�0
//filename:�ļ���
int FindString::FindStrFast(CString filename,CString cStr)
{
	CStdioFile	file;
	CString		bufRead;

	if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
	{		
		m_Edit_ListFiles.SetWindowText("����ɨ��" + cStr + " ɨ��λ��:" + filename);
		
		while (file.ReadString(bufRead))//��ʼһ��һ��ɨ�����
		{
			if (bufRead.Find(cStr) != -1)//�ҵ�������ַ���
			{
				file.Close();
				return 1;
			}
		}//end while
		file.Close();
	}//end if

	return 0;
}

void FindString::OnCheckFast() 
{
	// TODO: Add your control notification handler code here
	if (m_Check_Fast.GetCheck() == 1)
	{
		CString clew;
		clew.LoadString(IDS_STRING_CHECKCLEW);
		if (MessageBox(clew,"��ʾ",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
			m_Check_Fast.SetCheck(FALSE);
	}
}
BOOL FindString::bScaning(void)
{
	return (m_FindingStatus == FINDING);
}

BOOL FindString::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN)
        {
            OnButtonSelectfold();
        }
    }
	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL FindString::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == BUTTONEX_KEYDOWNCHAR)
    {
        if (lParam == 'P')//ѡ��F��,����ת��
        {
            m_Edit_Prefix.SetSel(0,-1);
			m_Edit_Prefix.SetFocus();
        }
        return TRUE;
    }
    if (wParam == LISTCTRLCHECK_CHECK)
        return TRUE;
	return CPropertyPage::OnCommand(wParam, lParam);
}
