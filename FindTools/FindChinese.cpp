   // FindChinese.cpp : implementation file
//

#include "stdafx.h"
#include "FindTools.h"
#include "FindChinese.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindChinese property page

IMPLEMENT_DYNCREATE(FindChinese, CPropertyPage)

FindChinese::FindChinese() : CPropertyPage(FindChinese::IDD)
{
	//{{AFX_DATA_INIT(FindChinese)
	m_bSubFold = TRUE;
	//}}AFX_DATA_INIT
}

FindChinese::~FindChinese()
{
	ListFuncEx.RemoveAll();
}

void FindChinese::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindChinese)
	DDX_Control(pDX, IDC_BUTTON_SELECTFOLD, m_SelectFold);
	DDX_Control(pDX, IDC_BUTTON_SCANING, m_Button_Scaning);
	DDX_Control(pDX, IDC_BUTTON_EXCEPTION, m_Exception);
	DDX_Control(pDX, IDC_BUTTON_SETFONT, m_Button_Font);
	DDX_Control(pDX, IDC_COMBO_SELECTFOLD, m_Combox_SelectFold);
	DDX_Control(pDX, IDC_PROGRESS_FINDCHINESE, m_Progress);
	DDX_Control(pDX, IDC_LIST_FILELIST, m_ListFile);
	DDX_Control(pDX, IDC_STATIC_RESULT, m_Static_ScanResult);
	DDX_Control(pDX, IDC_RICHEDIT_PREVIEW, m_FilePreview);
	DDX_Control(pDX, IDC_CHECK_SUBFOLD, m_SubFold);
	DDX_Check(pDX, IDC_CHECK_SUBFOLD, m_bSubFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindChinese, CPropertyPage)
	//{{AFX_MSG_MAP(FindChinese)
	ON_BN_CLICKED(IDC_BUTTON_SELECTFOLD, OnButtonSelectfold)
	ON_BN_CLICKED(IDC_BUTTON_SETFONT, OnButtonSetfont)
	ON_BN_CLICKED(IDC_BUTTON_EXCEPTION, OnButtonException)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILELIST, OnClickListFilelist)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FILELIST, OnColumnclickListFilelist)
	ON_BN_CLICKED(IDC_BUTTON_SCANING, OnButtonScaning)
	ON_CBN_DROPDOWN(IDC_COMBO_SELECTFOLD, OnDropdownComboSelectfold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindChinese message handlers

BOOL FindChinese::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	//****************��ʼ��listctrl�ؼ�
	iSortColumn = 0;
	DWORD oldStyle = m_ListFile.GetExtendedStyle();
	m_ListFile.SetExtendedStyle(oldStyle |LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES);
	//�����
	CString strLoadString;
	strLoadString.LoadString(IDS_STRING_FILENAME);
	m_ListFile.InsertColumn(0,strLoadString,LVCFMT_LEFT,500,0);//�ļ���

	strLoadString.LoadString(IDS_STRING_CHINESELINE);
	m_ListFile.InsertColumn(1,strLoadString,LVCFMT_LEFT,75,1);//����������
	
	strLoadString.LoadString(IDS_STRING_CODEPREVIEW);
	m_ListFile.InsertColumn(2,strLoadString,LVCFMT_LEFT,600,2);//����Ԥ��
	//***************��ʼ��imagelist
	imagelist.DeleteImageList();
	imagelist.Create(16, 16, ILC_MASK|ILC_COLORDDB, 1, 100);
	//**************��ʼ��RichEdit�ؼ��������岿��
	strLoadString.LoadString(IDS_STRING_FONT);//����
	memset(&chFormat,0x00,sizeof(CHARFORMAT));
	chFormat.cbSize			= sizeof(CHARFORMAT);
	chFormat.dwMask			= CFM_COLOR|CFM_FACE|CFM_BOLD|CFM_ITALIC|CFM_SIZE|CFM_UNDERLINE|CFM_STRIKEOUT;
	chFormat.dwEffects		&= ~CFE_AUTOCOLOR;
	chFormat.crTextColor	= RGB(255,0,0);
	strcpy(chFormat.szFaceName,strLoadString);//����
	//**************����������Ϣ**************
	logfont.lfHeight	= 24;//64,96
	logfont.lfWeight	= FW_DONTCARE;
	logfont.lfItalic	= FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet	= GB2312_CHARSET;
	strcpy(logfont.lfFaceName,strLoadString);//����
	//***************���ý�����*************
	m_Progress.ShowWindow(SW_HIDE);
	//****************��ȡ�������б�********
	ReadFunctionExList();
	//***************��ȡĿ¼�б�
	ReadFoldList();
    //****************��ʼ��CButtonEx�ؼ�����Ϣ
    m_SubFold.iCheckId = IDC_CHECK_SUBFOLD;
    m_SubFold.iClickId = IDC_CHECK_SUBFOLD;

    m_Button_Font.iCheckId = IDC_BUTTON_SETFONT;
    m_Button_Font.iClickId = IDC_BUTTON_SETFONT;

    m_Exception.iCheckId = IDC_BUTTON_EXCEPTION;
    m_Exception.iClickId = IDC_BUTTON_EXCEPTION;

    m_Button_Scaning.iCheckId = IDC_BUTTON_SCANING;
    m_Button_Scaning.iClickId = IDC_BUTTON_SCANING;

    m_SelectFold.iCheckId = IDC_BUTTON_SELECTFOLD;
    m_SelectFold.iClickId = IDC_BUTTON_SELECTFOLD;

    //*************************
    m_FindingStatus = STOP;
	/*HINSTANCE hInst = ShellExecute(this->GetSafeHwnd(), _T("open"), "hh.exe",
		(CString)cCurrentDir + "\\IVL.CHM", NULL, SW_MAXIMIZE);*/
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FindChinese::OnButtonSelectfold() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi; 
	ITEMIDLIST * pidl; 
	char Dir[MAX_PATH] = "";
	CString csFoldInit;//��ó�ʼĿ¼�Ա�ѡ��
	CString strLoadString;

	m_Combox_SelectFold.GetWindowText(csFoldInit);//��ó�ʼĿ¼�Ա�ѡ��
	strLoadString.LoadString(IDS_STRING_DSELECTFOLD);//ѡ��Ŀ¼

	bi.hwndOwner		= this->GetSafeHwnd();
	bi.pidlRoot			= NULL;
	bi.pszDisplayName	= Dir; 
	bi.lpszTitle		= strLoadString; //ѡ��Ŀ¼
	bi.ulFlags			= BIF_RETURNONLYFSDIRS ; 
	bi.lpfn				= SetSelProc;
	bi.lParam			= (LPARAM)(LPCTSTR)csFoldInit;
	bi.iImage			= 0; 
	
	pidl = SHBrowseForFolder(&bi);

	if(pidl == NULL) 
		memset(Dir,0,sizeof(MAX_PATH));
	if(SHGetPathFromIDList(pidl,Dir))/*Retrieve folder name from ITEMLIST structure.*/ 
	{
		m_Combox_SelectFold.SetWindowText(Dir);
	}
}
//��ʼ���ļ��в��������ַ�
//filename:�ļ���
int FindChinese::FindChineseCode(CString filename)
{
	CStdioFile	file;
	CString		bufRead;
	int			iFileLineNum;
	LPTSTR		lpstr,lpstrNext;
	BOOL		bNotesEnd;		//�Ƿ���"//"�Ժ��һ�д����д���"*/"
	BOOL		bNote;//�Ƿ�ע�Ϳ�ʼ("/*"����ע��)
	BOOL		bS,bD;//�Ƿ��е�����,�Ƿ���˫����
	short		iFileLineCount;//�����ļ�����
	CString		cfile;//�����ļ���,������·��

	if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
	{
		iFileLineCount = 0;//�����ļ�����,Ϊ�������ṩ����
		while (file.ReadString(bufRead))
			iFileLineCount++;

		file.Close();

		m_Progress.ShowWindow(SW_SHOW);
		m_Progress.SetRange(0,iFileLineCount);
		m_Progress.SetStep(1);
		m_Progress.SetProgressFillColor(RGB(255,0,0));
		m_Progress.SetProgressTextColor(RGB(0,0,255));
		m_Progress.ShowPreText(TRUE);
	}

	if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
	{
		CString strLoadString;
		strLoadString.LoadString(IDS_STRING_SCANING);//����ɨ��

		iFileLineNum	= 0;
		bNote			= FALSE;
		
		cfile = file.GetFileName();
		m_Static_ScanResult.SetWindowText(strLoadString + cfile);//����ɨ��
		
		while (file.ReadString(bufRead))//��ʼһ��һ��ɨ�����
		{
			bS = FALSE;		//û�е�����
			bD = FALSE;		//û��˫����
			iFileLineNum++;	//��¼��ǰɨ�������
			bufRead.TrimLeft();
			bufRead.TrimRight();
			
			if (bufRead == "#if 0")
			{
				while (file.ReadString(bufRead))//��Ҫ����#if 0�ⲿ�ִ���
				{
					bufRead.TrimLeft();
					bufRead.TrimRight();
					iFileLineNum++;	//��¼��ǰɨ�������
					m_Progress.SetPos(iFileLineNum);
					if ((bufRead == "#endif") || (bufRead == "#else"))
						break;
				}
			}
			
			lpstr		= NULL;
			lpstrNext	= NULL;
			lpstr		= bufRead.GetBuffer(bufRead.GetLength());

			m_Progress.SetPos(iFileLineNum);

			while (* lpstr != 0)//��һ�д����н���ɨ��
			{
				if (* lpstr == 34)//��ǰ�ַ���˫����
					bD = !bD;
				else if (* lpstr == 39)//��ǰ�ַ�������
					bS = !bS;
				else
				{
					if (* lpstr == 47)//��ǰ�ַ���"/"
					{
						lpstrNext = lpstr;
						lpstrNext++;
						if (* lpstrNext == 0)//������β
							break;

						if (* lpstrNext == 47)//��һ���ַ���"/"
						{
							if ((bS) || (bD))//��"//"֮ǰ�е����Ż���˫���ű�ʾ���"//"����ע��
							{
								lpstr++;
								continue;
							}
							else//��"//"֮ǰû�е�����û��˫���ű�ʾ���"//"��ע��,��ʼɨ����һ��
							{
								//�����������,"//...*/"��ʱ����һ���Ƕ���ע�͵Ľ���
								lpstr += 2;
								bNotesEnd = FALSE;
								while (* lpstr != 0)
								{
									if (* lpstr == 42)//��ǰ�ַ���"*"
									{
										lpstrNext = lpstr;
										lpstrNext++;
										if ((* lpstrNext == 47) && (bNote))//��һ���ַ���"/"�����ж���ע��
										{
											bNote = FALSE;//��ʾ����ע�ͽ���
											bNotesEnd = TRUE;
											lpstr += 2;
											break;
										}
									}
									lpstr++;
								}
								if (bNotesEnd)//����"//"�Ժ��һ�д����д���"*/"��Ҫ����һ���м���ɨ��
									break;
							}
						}
						else if (* lpstrNext == 42)//��һ���ַ���"*"
						{
							if ((bS) || (bD))//��"/*"֮ǰ�е����Ż���˫���ű�ʾ���"/*"����ע��
							{
								lpstr++;
								continue;
							}
							else//��"/*"֮ǰû�е�����û��˫���ű�ʾ���"/*"��ע��
							{
								lpstr++;
								bNote = TRUE;//��ʾ����ע�Ϳ�ʼ
								continue;
							}
						}
					}
					else if (* lpstr == 42)//��ǰ�ַ���"*"
					{
						lpstrNext = lpstr;
						lpstrNext++;
						if (* lpstrNext == 0)//������β
							break;
						if ((* lpstrNext == 47) && (bNote))//��һ���ַ���"/"�����ж���ע��
						{
							bNote = FALSE;//��ʾ����ע�ͽ���
							lpstr += 2;//lpstr++;
							continue;
						}
					}
				}//end if

				if ((unsigned char)(* lpstr) > 0x80)
				{
					if (!bNote)//��ǰ����λ�ò���ע��
					{
						AddFile(filename,iFileLineNum,bufRead);//�ҵ��˴����е�����
						break;//��ʼɨ����һ��
					}
					lpstr += 2;
				}
				else if (* lpstr != 0)
				{
					lpstr++;
				}
			}//end while
            bufRead.ReleaseBuffer();
		}//end while
		file.Close();
	}//end if
	m_Progress.ShowWindow(SW_HIDE);
	return 1;
}
void FindChinese::AddFile(CString filename,int iLineNum,CString code)//���ļ����к���ӵ�listctrl�ؼ���
{
	int iListCount = m_ListFile.GetItemCount();
	long iTextLength;
	SIZE size;
	CString cLineNum,cfile;
	CString	cResult;

	if (IsInFunctionEx(code))
		return;

	cLineNum.Format("%d",iLineNum);
	m_ListFile.InsertItem (iListCount,filename	);	//����б���һ��,�ļ���
	m_ListFile.SetItemText(iListCount,1,cLineNum);	//����к�
	m_ListFile.SetItemText(iListCount,2,code	);	//��Ӵ���Ԥ��
	m_ListFile.SetItemData(iListCount,iListCount);	//��������
	size.cx = 0;
	size.cy = iListCount + 1;
	m_ListFile.Scroll(size);

	cfile = filename.Right(filename.GetLength() - filename.ReverseFind('\\') - 1);

	if (cfile.GetLength() < 8)
		cResult = cfile + "\t\t\t\t" + cLineNum + "\t\t" + code + "\r\n";
	else if (cfile.GetLength() > 15)
		cResult = cfile + "\t\t"	 + cLineNum + "\t\t" + code + "\r\n";
	else
		cResult = cfile + "\t\t\t"	 + cLineNum + "\t\t" + code + "\r\n";

	iTextLength = m_FilePreview.GetWindowTextLength();
	m_FilePreview.SetSel(iTextLength, iTextLength);
	m_FilePreview.ReplaceSel(cResult);
}
//����ļ���,��pLineNum���ص�ǰ��Ŀ�������к�
CString FindChinese::GetFileName(int * pLineNum)
{
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
	CString strFileName = "",strLineNum = "";
	LPTSTR lpstr;
	
	if (-1 == nSelectedItem)
		return strFileName;

	strFileName = m_ListFile.GetItemText(nSelectedItem, 0);//����ļ���
	strLineNum	= m_ListFile.GetItemText(nSelectedItem, 1);//����к�
	//***********���²�����CString����ת����int�ͱ���
	lpstr = strLineNum.GetBuffer(strLineNum.GetLength());

	* pLineNum = atoi(lpstr);//�����к�

    strLineNum.ReleaseBuffer();
	return(strFileName);
}

void FindChinese::PreViewCode()//����Ԥ��
{
	CString		filename,bufRead,bufNew;
	int			iLineNum,iVisibleLine,iLine,i;
	long		nStartChar,nEndChar,iFileLineCount;
	CStdioFile	file;

	if(m_FindingStatus == FINDING)//����ɨ���ʱ��ִ�д���Ԥ��
	{
		return;
	}

	CWaitCursor wait;
	
	filename = GetFileName(&iLineNum);//����ļ���,����Ҫ��������һ��

	if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
	{
		iFileLineCount = 0;//�����ļ�����,Ϊ�������ṩ����
		while (file.ReadString(bufRead))
			iFileLineCount++;

		file.Close();

		m_Progress.ShowWindow(SW_SHOW);
		m_Progress.SetRange(0,iFileLineCount + 1);
		m_Progress.SetStep(1);
	}
	if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
	{
		bufNew = "";
		iLine = 0;//�����к�,���ڴ����ɫ

		while (file.ReadString(bufRead))
		{
			iLine++;
			if (iLine == iLineNum)
				nStartChar = bufNew.GetLength();//��ʼ��ɫ��λ��,��ǰ�е���һ��
			else if (iLine == iLineNum + 1)
				nEndChar = bufNew.GetLength();//��ɫ����λ��,��ǰ�е���һ��

			bufNew = bufNew + bufRead + "\r\n";

			m_Progress.SetPos(iLine + 1);
		}
		file.Close();
		m_FilePreview.SetWindowText(bufNew);//Ԥ������
		i = 0;
		do
		{
			iVisibleLine = m_FilePreview.GetFirstVisibleLine();		//��ÿɼ���
			m_FilePreview.LineScroll(iLineNum - iVisibleLine - 1,0);//����������
			//��ת��������1000�Ļ�����Ϊ��������ѭ��,��Ҫǿ���˳�
			if ((i++ >= 1000) || (iLineNum - iVisibleLine < 78))
				break;
			TRACE("\niVisibleLine = %d\n",iVisibleLine);
		}
		while (iLineNum - iVisibleLine - 1 != 0);

		m_FilePreview.SetSel(nStartChar,nEndChar);//ѡ�з�Χ
		m_FilePreview.SetSelectionCharFormat(chFormat);//��������

		m_Progress.ShowWindow(SW_HIDE);
	}
}

void FindChinese::OnButtonSetfont() 
{
	// TODO: Add your control notification handler code here
	CFontDialog fDialog;
	fDialog.m_cf.rgbColors	= chFormat.crTextColor;
	fDialog.m_cf.lpLogFont	= &logfont;
	fDialog.m_cf.Flags		= fDialog.m_cf.Flags|CF_INITTOLOGFONTSTRUCT;
	
	if (fDialog.DoModal() == IDOK)
	{
		chFormat.crTextColor = fDialog.GetColor();
		strcpy(chFormat.szFaceName,fDialog.GetFaceName());
		chFormat.yHeight = fDialog.GetSize();

		if (fDialog.IsBold())
			chFormat.dwEffects |= CFE_BOLD;
		else
			chFormat.dwEffects &= ~CFE_BOLD;

		if (fDialog.IsItalic())
			chFormat.dwEffects |= CFE_ITALIC;
		else
			chFormat.dwEffects &= ~CFE_ITALIC;

		if (fDialog.IsStrikeOut())
			chFormat.dwEffects |= CFE_STRIKEOUT;
		else
			chFormat.dwEffects &= ~CFE_STRIKEOUT;

		if (fDialog.IsUnderline())
			chFormat.dwEffects |= CFE_UNDERLINE;
		else
			chFormat.dwEffects &= ~CFE_UNDERLINE;
	}
}
//****************************�̲߳�������***************************************
UINT FindChinese::FindingThread(LPVOID lpvData)
{
	FindChinese* pThis = (FindChinese*)lpvData;

	BOOL		bWorking;
	CFileFind	find;
	CString		strFilePath,strPath,strFolder;
	CString		filename,cExpend;
	int			iExpend;
	CStringList listPaths;
	CString		cResult;

	listPaths.RemoveAll();
	strPath = pThis->cPath + "\\*.*";
	do
	{
		bWorking = find.FindFile(strPath);
		while (bWorking && (pThis->m_FindingStatus == FINDING))
		{
			bWorking	= find.FindNextFile();
			strFilePath = find.GetFilePath();
			
			if (find.IsDots())
				continue;
			
			if (find.IsDirectory())
			{
				if ((pThis->m_bSubFold) && (!pThis->IsInFoldEx(strFilePath)))
					listPaths.AddHead(strFilePath);
				continue;
			}
			else
			{
				filename = find.GetFilePath();//����ļ���(����·��)
				iExpend	 = filename.ReverseFind('.');//�����չ����ߵ�"."
				cExpend  = filename.Right(filename.GetLength() - iExpend - 1);//�����չ��
				cExpend.MakeLower();
				if ((cExpend == "c") || (cExpend == "h") || (cExpend == "cpp"))
				{
					if (!pThis->IsInFileEx(filename))
					{
						pThis->FindChineseCode(filename);
						pThis->iFilenum++;
					}
				}
			}
		}
		if(listPaths.IsEmpty())
			break;

		strFolder	= listPaths.RemoveHead();
		strPath		= strFolder + "\\*.*";
		find.Close();
	}while (pThis->m_FindingStatus == FINDING);

	pThis->m_FindingStatus = STOP;//��ǰΪֹͣ״̬

	CString strLoadString;

	strLoadString.LoadString(IDS_STRING_RESULT);//��ǰɨ��%d���ļ�,�����й���%d������
	cResult.Format(strLoadString,pThis->iFilenum,pThis->m_ListFile.GetItemCount());
	pThis->m_Static_ScanResult.SetWindowText(cResult);

	strLoadString.LoadString(IDS_STRING_BEGIN);//��ʼɨ��
	pThis->m_Button_Scaning.SetWindowText(strLoadString);//��ʼɨ��

	pThis->m_Combox_SelectFold.SetWindowText(pThis->cPath);

	pThis->m_Exception.EnableWindow(TRUE);
	pThis->m_Combox_SelectFold.EnableWindow(TRUE);
	pThis->m_SelectFold.EnableWindow(TRUE);
	pThis->m_SubFold.EnableWindow(TRUE);

    sndPlaySound(SOUNDFILENAME,SND_ASYNC);

	return 0;
}

void FindChinese::OnButtonException() 
{
	// TODO: Add your control notification handler code here
	FindChineseEx FCE;

	int nResponse = FCE.DoModal();

	if (nResponse == IDOK)
	{
		ReadFunctionExList();//��ȡ�������б�
	}
}

void FindChinese::ReadFunctionExList(void)//��ȡ�������б�
{
	CStdioFile	file;
	CString		bufRead;
	int			iPre;

	ListFuncEx.RemoveAll();
	ListFileEx.RemoveAll();
	ListFoldEx.RemoveAll();
	//ListFoldExSub.RemoveAll();

	if (file.Open(FINDCHINESEEXFILE,CFile::modeRead,NULL))//�򿪳ɹ�
	{		
		while (file.ReadString(bufRead))
		{
			iPre = bufRead.Find("=");
			if (-1 == iPre)
				break;

			while(bufRead.Right(1) == 13)//ɾ�����Ļس���
				bufRead = bufRead.Left(bufRead.GetLength() - 1);

			if (bufRead.Left(iPre) == FUNCTION_SIGN)
			{
				bufRead = bufRead.Right(bufRead.GetLength() - iPre - 1);
				ListFuncEx.AddHead(bufRead);
			}
			else if (bufRead.Left(iPre) == FILE_SIGN)
			{
				bufRead = bufRead.Right(bufRead.GetLength() - iPre - 1);
				ListFileEx.AddHead(bufRead);
			}
			else if (bufRead.Left(iPre) == FOLD_SIGN)
			{
				bufRead = bufRead.Right(bufRead.GetLength() - iPre - 1);
				ListFoldEx.AddHead(bufRead);
				/*BOOL bFold = FALSE;
				CString cFold = bufRead.Right(2);//��������λ
				if (cFold.Left(1) == FOLD_SING_FOLD)
				{
					bFold = (cFold.Right(1) == "1");//������Ŀ¼
					cFold = bufRead.Left(bufRead.GetLength() - 2);
				}
				else
				{
					cFold = bufRead.Left(bufRead.GetLength());
				}
				ListFoldEx.AddHead(cFold);
				//ListFoldExSub.AddHead(bFold ? "1" : "0");*/
			}
		}
		file.Close();
	}
}

BOOL FindChinese::IsInFunctionEx(CString code)//���д�����������Ƿ�����������
{
	int i,iStringListCount;
	CString csFunctinEx;
	int iFunctinExLen;
	CString csFuncRight;//���������ұ߲���
	POSITION ps;

	iStringListCount = ListFuncEx.GetCount();
	ps = ListFuncEx.GetHeadPosition();

	for (i = 0;i < iStringListCount;i++)
	{
		csFunctinEx		= ListFuncEx.GetNext(ps);//��������е�������
		iFunctinExLen	= csFunctinEx.GetLength();
		
		if (code.Left(iFunctinExLen) == csFunctinEx)//һ�д�����ߵ���������
		{
			csFuncRight = code.Right(code.GetLength() - iFunctinExLen);//��ú��������ұ߲���
			csFuncRight.TrimLeft();
			if (csFuncRight.Left(1) == "(")
				return TRUE;
		}
	}
	return FALSE;
}

BOOL FindChinese::IsInFileEx(CString FileName)//���ļ��Ƿ�����������
{
	int i,iStringListCount;
	CString csFileEx;
	POSITION ps;

	iStringListCount = ListFileEx.GetCount();
	ps = ListFileEx.GetHeadPosition();

	for (i = 0;i < iStringListCount;i++)
	{
		csFileEx = ListFileEx.GetNext(ps);//��������е�������
		csFileEx.MakeLower();
		FileName.MakeLower();
		if (FileName == csFileEx)
			return TRUE;
	}
	return FALSE;
}

BOOL FindChinese::IsInFoldEx(CString FoldName)//��Ŀ¼�Ƿ�����������
{
	int i,iStringListCount;
	CString csFoldEx;
	POSITION ps;

	iStringListCount = ListFoldEx.GetCount();
	ps = ListFoldEx.GetHeadPosition();

	for (i = 0;i < iStringListCount;i++)
	{
		csFoldEx = ListFoldEx.GetNext(ps);//��������е�������
		csFoldEx = csFoldEx.Left(csFoldEx.GetLength() - 2);
		csFoldEx.MakeLower();
		FoldName.MakeLower();
		if (FoldName == csFoldEx)
			return TRUE;
	}
	return FALSE;
}

void FindChinese::OnClickListFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	PreViewCode();
	*pResult = 0;
}

void FindChinese::OnColumnclickListFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iColumn = pNMListView->iSubItem;
	static BOOL bSortArrow = FALSE;//��������¼�ͷ

	if (iColumn == iSortColumn)//����ͬһ��
	{
		m_ListFile.SetHeaderCtrlArrow(iColumn,bSortArrow);
		m_ListFile.m_fAsc = bSortArrow;
		bSortArrow = !bSortArrow;
	}
	else//����ͬ����
	{
		m_ListFile.SetHeaderCtrlArrow(iColumn,FALSE);//��������Ϊ��������
		m_ListFile.m_fAsc = FALSE;
		
		if (!bSortArrow)
			bSortArrow = !bSortArrow;

		iSortColumn = iColumn;
	}
	m_ListFile.m_nSortedCol = iSortColumn;
	m_ListFile.SortItems((PFNLVCOMPARE)CompareFunc,(DWORD)&m_ListFile);

	*pResult = 0;
}
int CALLBACK FindChinese::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortListCtrl* pV = (CSortListCtrl*)lParamSort;

	CString    strItem1 = pV->GetItemText(lParam1, pV->m_nSortedCol);
	CString    strItem2 = pV->GetItemText(lParam2, pV->m_nSortedCol);
	
	/*switch (pV->m_nSortedCol)
	{
	case 0:
	case 2:
		return strcmp(strItem2, strItem1);

		break;
	case 1:
		LPTSTR lpstr1,lpstr2;
		lpstr1 = strItem1.GetBuffer(strItem1.GetLength());
		lpstr2 = strItem2.GetBuffer(strItem2.GetLength());

		int iLine1,iLine2;
		iLine1 = atoi(lpstr1);
		iLine2 = atoi(lpstr2);
		if (iLine1 < iLine2)
			return 1;
		else if (iLine1 > iLine2)
			return -1;
		else
			return 0;

		break;
	}*/
	return strcmp(strItem2, strItem1);
}

BOOL FindChinese::bScaning(void)//�Ƿ�����ɨ�����
{
	return (m_FindingStatus == FINDING);
}

int CALLBACK FindChinese::SetSelProc(HWND hWnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
    ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);

  return 0;
}

void FindChinese::ReadFoldList(void)//��ȡĿ¼�б�
{
	CStdioFile	file;
	CString		bufRead;

	if (file.Open(FINDCHINESELECTFOLD,CFile::modeRead,NULL))//�򿪳ɹ�
	{		
		while (file.ReadString(bufRead))
		{
			while(bufRead.Right(1) == 13)//ɾ�����Ļس���
				bufRead = bufRead.Left(bufRead.GetLength() - 1);

			m_Combox_SelectFold.AddString(bufRead);
		}
		file.Close();
	}
}

BOOL FindChinese::SaveFoldList(CString sFold)//����Ŀ¼�б�
{
	CStdioFile	file;
	int			i,iComboxCount;
	CString		csFoldList,cResult;
	
	iComboxCount = m_Combox_SelectFold.GetCount();
	for (i = iComboxCount - 1;i >= 0;i--)
	{
		m_Combox_SelectFold.GetLBText(i,csFoldList);
		if (sFold == csFoldList)//��Ŀ¼���б�����Ѿ�����
			m_Combox_SelectFold.DeleteString(i);//ɾ����һ��
	}
	m_Combox_SelectFold.InsertString(0,sFold);

	iComboxCount = m_Combox_SelectFold.GetCount();
	if (iComboxCount > FOLDMAXNUM)//ɾ�������һ��
		m_Combox_SelectFold.DeleteString(iComboxCount - 1);

	if (file.Open(FINDCHINESELECTFOLD,CFile::modeCreate | CFile::modeWrite | CFile::typeText,NULL))
	{
		iComboxCount = m_Combox_SelectFold.GetCount();
        cResult      = "";
		for (i = 0;i < iComboxCount;i++)
		{
			if (i > FOLDMAXNUM - 1)
				break;
			m_Combox_SelectFold.GetLBText(i,csFoldList);
			csFoldList  = csFoldList + "\r\n";
            cResult     = cResult + csFoldList;
			//file.WriteString(csFoldList);
		}
        file.WriteString(cResult);
		file.Close();	
	}
	else
	{
		MessageBox("�޷������ļ�.","��ʾ",MB_ICONSTOP|MB_OK);
	}
	return TRUE;
}

void FindChinese::OnButtonScaning() 
{
	// TODO: Add your control notification handler code here
	if(m_FindingStatus == FINDING)
	{
		CString str1, str2,strBegin;

		str1.LoadString(IDS_STRING_STOPCLEW);//��ǰ����ɨ���ļ�,��ȷ��ֹͣ��?
		str2.LoadString(IDS_STRING_CLEW);//��ʾ����
		if(MessageBox(str1, str2, MB_YESNO|MB_ICONQUESTION)==IDNO)
		{
			return ;
		}

		m_FindingStatus = STOP;

		m_Combox_SelectFold.SetWindowText(cPath);

		strBegin.LoadString(IDS_STRING_BEGIN);
		m_Button_Scaning.SetWindowText(strBegin);//��ʼɨ��
		m_Combox_SelectFold.EnableWindow(TRUE);
		m_SelectFold.EnableWindow(TRUE);
		m_SubFold.EnableWindow(TRUE);
		m_Exception.EnableWindow(TRUE);
		return;
	}
	CString csPath;

	m_Combox_SelectFold.GetWindowText(csPath);
	csPath.TrimLeft();
	csPath.TrimRight();
	if (csPath == "")
	{
		MessageBox("���������ѡ��Ŀ¼","��ʾ",MB_ICONASTERISK |MB_OK);
		m_Combox_SelectFold.SetFocus();
		return;
	}

    char cCurDirBak[256] = "";
    GetCurrentDirectory(sizeof(cCurDirBak),cCurDirBak);

	if (!SetCurrentDirectory(csPath))
    {
        MessageBox("�޷��ҵ�Ŀ¼" + csPath,"��ʾ",MB_ICONASTERISK |MB_OK);
		m_Combox_SelectFold.SetFocus();
        SetCurrentDirectory(cCurDirBak);
		return;
    }
    SetCurrentDirectory(cCurDirBak);

	SaveFoldList(csPath);//����Ŀ¼�б�

	m_ListFile.DeleteAllItems();
	m_Combox_SelectFold.SetWindowText(csPath);

	CString strEnd;

	strEnd.LoadString(IDS_STRING_STOP);
	m_Button_Scaning.SetWindowText(strEnd);//
	m_Combox_SelectFold.EnableWindow(FALSE);
	m_SelectFold.EnableWindow(FALSE);
	m_SubFold.EnableWindow(FALSE);
	m_Exception.EnableWindow(FALSE);

	CString strClew;
	strClew.LoadString(IDS_STRING_SCANRESULT);//�ļ���\t\t\t\t������������\t����Ԥ��\r\n
	
	iFilenum		= 0;//�ļ��������ۼ���
	cPath			= csPath;//��¼Ŀ¼
	m_FindingStatus	= FINDING;//��¼��ǰ״̬
	m_bSubFold		= m_SubFold.GetCheck();//�Ƿ������Ŀ¼
	
	CWinThread* pThread = AfxBeginThread(FindingThread,this);//��ʼ�߳�
	
    m_Button_Scaning.SetFocus();
    
	m_FilePreview.SetWindowText(strClew);
}

void FindChinese::OnDropdownComboSelectfold() 
{
	// TODO: Add your control notification handler code here
	int		i,iComboxCount,iMaxLen;
	CString	csFoldList;

    iMaxLen      = 0;
	iComboxCount = m_Combox_SelectFold.GetCount();
	for (i = iComboxCount - 1;i >= 0;i--)
	{
		m_Combox_SelectFold.GetLBText(i,csFoldList);
		if (csFoldList.GetLength() > iMaxLen)
            iMaxLen = csFoldList.GetLength();
	}
    m_Combox_SelectFold.SendMessage(CB_SETDROPPEDWIDTH,iMaxLen * 7,0);
}

BOOL FindChinese::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
    DWORD style = m_Button_Scaning.GetButtonStyle();
    style = style | BS_DEFPUSHBUTTON;
    m_Button_Scaning.SetButtonStyle(style,TRUE);

	return CPropertyPage::OnSetActive();
}

BOOL FindChinese::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN)
        {
            OnButtonScaning();
        }
    }
	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL FindChinese::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == BUTTONEX_KEYDOWNCHAR)
    {
        if (lParam == 'O')//ѡ��F��,����ת��
        {
			m_Combox_SelectFold.SetFocus();
        }
        return TRUE;
    }
    if (wParam == LISTCTRLCHECK_CHECK)
        return TRUE;
    
	return CPropertyPage::OnCommand(wParam, lParam);
}
