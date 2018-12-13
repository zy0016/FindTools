// MacroTool.cpp : implementation file
//

#include "stdafx.h"
#include "FindTools.h"
#include "MacroTool.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MacroTool property page

IMPLEMENT_DYNCREATE(MacroTool, CPropertyPage)

MacroTool::MacroTool() : CPropertyPage(MacroTool::IDD)
{
	//{{AFX_DATA_INIT(MacroTool)
	//}}AFX_DATA_INIT
}

MacroTool::~MacroTool()
{
}

void MacroTool::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MacroTool)
	DDX_Control(pDX, IDC_PROGRESS_MACRO, m_Progress_Macro);
	DDX_Control(pDX, IDC_BUTTON_SELECTFILE, m_Button_SelectFile);
	DDX_Control(pDX, IDC_EDIT_MACRO, m_Edit_Macro);
	DDX_Control(pDX, IDC_RICHEDIT_PREVIEW, m_FilePreview);
	DDX_Control(pDX, IDC_LIST_MACRO_LISTFILE, m_ListFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MacroTool, CPropertyPage)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(MacroTool)
	ON_BN_CLICKED(IDC_BUTTON_SELECTFILE, OnButtonSelectfile)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MACRO_LISTFILE, OnClickListMacroListfile)
	ON_BN_CLICKED(IDC_BUTTON_SINGLE, OnButtonSingle)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MACRO_LISTFILE, OnDblclkListMacroListfile)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_SETFONT, OnButtonSetfont)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_MACRO_LISTFILE, OnKeydownListMacroListfile)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_COMMAND(ID_MENU_SINGLEOUT, OnMenuSingleout)
	ON_COMMAND(ID_MENU_DELETE, OnMenuDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MacroTool message handlers

BOOL MacroTool::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	//*****************���ý�����
	m_Progress_Macro.ShowWindow(SW_HIDE);
	//***************************��������
	cClew.LoadString(IDS_STRING_CLEW);
	//*******************����listctrl*******************************
	DWORD oldStyle = m_ListFile.GetExtendedStyle();
	m_ListFile.SetExtendedStyle(oldStyle |LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES);//LVS_EX_TRACKSELECT
	//�����,listctrl������
	m_ListFile.InsertColumn(0,"�ļ���"		,LVCFMT_LEFT,150,0);
	m_ListFile.InsertColumn(1,"�����ļ���"	,LVCFMT_LEFT,300,1);
	//**************����richeditctrl�ؼ���������Ϣ
	memset(&chFormat,0x00,sizeof(CHARFORMAT));
	chFormat.cbSize			= sizeof(CHARFORMAT);
	chFormat.dwMask			= CFM_COLOR|CFM_FACE|CFM_BOLD|CFM_ITALIC|CFM_SIZE|CFM_UNDERLINE|CFM_STRIKEOUT;
	chFormat.dwEffects		&= ~CFE_AUTOCOLOR;
	chFormat.crTextColor	= RGB(255,0,0);
	strcpy(chFormat.szFaceName,"����");
	//*************����RichEdit�ؼ���tab��С,���񲻹���
	PARAFORMAT pf;
	memset(&pf,0x00,sizeof(PARAFORMAT));
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_ALIGNMENT|PFM_TABSTOPS;
	//pf.wNumbering = PFN_BULLET;
	pf.cTabCount = 4;
	m_FilePreview.SetParaFormat(pf);
	//**************��������Ի������Ϣ**************
	logfont.lfHeight	= 24;//64,96
	logfont.lfWeight	= FW_DONTCARE;
	logfont.lfItalic	= FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet	= GB2312_CHARSET;
	strcpy(logfont.lfFaceName,"����");
	//******************�˵�λͼ��Ϣ*************
	cMBSingleOut.LoadBitmap(IDB_BITMAP_SINGLEOUT);
	cMBDelete.LoadBitmap(IDB_BITMAP_DELETE);
	
	imagelist.DeleteImageList();
	imagelist.Create(16, 16, ILC_MASK|ILC_COLORDDB, 1, 100);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MacroTool::OnButtonSelectfile() 
{
	// TODO: Add your control notification handler code here
	CString	pathfilename,filename,pathname;
	char	filetype[] = "VC�ļ�(*.c;*.cpp;*.h)|*.c;*.cpp;*.h|";
	int		iFilenum;//�ļ�����
    char    cCurDir[256] = "";

    GetCurrentDirectory(sizeof(cCurDir),cCurDir);

	CFileDialog open(TRUE,NULL,
		NULL,OFN_ALLOWMULTISELECT|
		OFN_EXTENSIONDIFFERENT|
		OFN_HIDEREADONLY,filetype);
	
	open.m_ofn.lpstrTitle = "ѡ���ļ�";
	open.m_ofn.lpstrInitialDir = cCurDir;
	open.m_ofn.lpstrDefExt = "*.c;*.cpp";
	
	if (open.DoModal() == IDOK)
	{
		POSITION ps = open.GetStartPosition();
		iFilenum	= 0;

		m_ListFile.SetImageList(&imagelist, LVSIL_SMALL);//���б�ؼ�����һ��ͼ���б�

		while (ps != NULL)
		{
			pathfilename = open.GetNextPathName(ps);
			filename	 = open.GetFileName();
			pathname	 = open.GetPathName();

			int pos = pathfilename.ReverseFind('\\');//������ұߵ�"\"��λ��,���ڽ�ȡ�ļ���
						
			SHFILEINFO sfi;
			if (::SHGetFileInfo (pathfilename, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO),SHGFI_USEFILEATTRIBUTES | SHGFI_DISPLAYNAME | SHGFI_TYPENAME |SHGFI_ICON|SHGFI_SMALLICON ))
			{
				imagelist.Add(sfi.hIcon);
				if (pathfilename.Compare(pathname))//�����(ѡ���˶���ļ�)
				{
					int len = pathfilename.GetLength();
					m_ListFile.InsertItem(iFilenum,pathfilename.Right(len - pos - 1),iFilenum);//����б���һ��,�ļ���
					m_ListFile.SetItemText(iFilenum,1,pathname);//���·��
				}
				else//ѡ����һ���ļ�
				{
					m_ListFile.InsertItem(0,filename);//����б���һ��,�ļ���
					m_ListFile.SetItemText(iFilenum,1,pathname.Left(pos));//���·��
				}
				iFilenum++;
			}
		}
	}
}
CString MacroTool::GetMacroReplace(CString filename,CString cmacro)
{
	CStdioFile	file;
	CString		bufRead;//�ļ��е�һ�д���
	CString		bufNew;//ִ��֮��ɨ��֮��Ĵ���
	BOOL		bMacroBegin;
	//�������ڵ�λ�����������ĺ���˵�Ƿ�ǳ���ȷ,���ǳ���ȷ��ΪFALSE��ʾ����Ҫ����.
	//��֮���������ڵ�λ����������˵���Ƿǳ���ȷ,��ΪTRUE,��ʾ��Ҫ����.
	BOOL		bCopy;//�Ƿ�ʼ��������
	int			isMacro;//��ø��д�������
	BOOL		bMacro;//��ʾ�Ƿ���Ҫ��IsMacro�����л����Ϣ,TRUE��,FALSE:��
	BOOL		bifndef;//�Ƿ���"#ifndef"�����
	
	if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
	{
		bMacroBegin = FALSE;
		bMacro		= TRUE;
		bufNew		= "";

		bifndef		= FALSE;
		while (1)
		{
			bCopy = TRUE;

			if (bMacro)
			{
				if (file.ReadString(bufRead))
					isMacro = IsMacro(bufRead,cmacro);//��ø��д�������
				else
					break;
			}
			switch (isMacro)//��������
			{
			case 1://bufRead�а���strMacro����ĺ�
				while (1)
				{
					bMacroBegin = FALSE;//����λ����ȷ
					if (file.ReadString(bufRead))
					{
						int iMacroType = GetMacroType(bufRead);
						if (iMacroType == 5)//#endif
						{
							if (bifndef)//���#endif��#ifndef�Ľ����ĺ�,���д���Ҳ��Ҫ����
							{
								bifndef = FALSE;//��ʾ#ifndef����궨��ķ�Χ�Ѿ�������
								bufNew = bufNew + bufRead + "\n";
							}
							break;//��������
						}
						else if ((iMacroType == 3) || (iMacroType == 4))//#elif,#else
						{
							bCopy = FALSE;//��һ����־,���²��ֵ������궨�岿�ֵĴ��벻���п���
							continue;
						}
						else
						{
							if (iMacroType == 6)//#ifndef
								bifndef = TRUE;//��ʾ#ifndef����궨��ķ�Χ�Ѿ���ʼ��

							if (bCopy)
							{
								bufNew = bufNew + bufRead + "\n";
							}
						}
					}
					else//�Ѿ������ļ���β,����궨���ǲ�������
					{
						file.Close();
						return (bufNew);
					}
				}
				bMacro = TRUE;
				break;
			case 0://bufReadֻ��һ��Ĵ���,���ں궨��֮��
				bufNew = bufNew + bufRead + "\n";
				bMacro = TRUE;
				break;
			case -1://bufRead���������ĺ궨��,������strMacro
				while (1)
				{
					bMacroBegin = TRUE;//������������˵λ�ò���ȷ
					if (file.ReadString(bufRead))
					{
						int iMacroType = GetMacroType(bufRead);
						if (iMacroType == 5)//#endif
						{
							if (bifndef)//���#endif��#ifndef�Ľ����ĺ�
							{
								bifndef = FALSE;
								continue;
							}
							bMacro = TRUE;
							break;
						}
						if ((iMacroType == 3) || (iMacroType == 4))//#elif,#else
						{
							bMacro = FALSE;//�����ĺ궨���Ѿ�������,��Ҫ���»�ô��������
							//����Ҫ��ѭ����ʼ�����ٻ��һ�δ��������
							isMacro = IsMacro(bufRead,cmacro);
							break;//����
						}
						else if (iMacroType == 6)//#ifndef
							bifndef = TRUE;
					}
					else//�Ѿ������ļ���β,����궨���ǲ�������
					{
						file.Close();
						return (bufNew);
					}
				}
				break;
			case -2://�޷�ȷ�����벿���Ƿ�����صĺ궨����
				if (bMacro)//������#else,#elif��һ�д���
				{
					bufNew = bufNew + bufRead + "\n";
				}
				if (bMacroBegin)
				{
					if (file.ReadString(bufRead))
					{
						int iMacroType = GetMacroType(bufRead);
						if ((iMacroType == 3) || (iMacroType == 4) || (iMacroType == 5))//#elif,#else,#endif
						{
							bMacro = TRUE;
							break;//��������
						}
						else
						{
							if (bCopy)
							{
								bufNew = bufNew + bufRead + "\n";
							}
						}
					}
					else//�Ѿ������ļ���β,����궨���ǲ�������
					{
						file.Close();
						return (bufNew);
					}
				}
				break;
			}
		}
		file.Close();
	}
	return (bufNew);
}

CString MacroTool::GetMacroReplace(CString filename,CString cmacro,CRichEditCtrl * pRichEdit)
{
	CStdioFile	file;
	CString		bufRead;//�ļ��е�һ�д���
	CString		bufNew;//ִ��֮��ɨ��֮��Ĵ���
	BOOL		bMacroBegin;
	//�������ڵ�λ�����������ĺ���˵�Ƿ�ǳ���ȷ,���ǳ���ȷ��ΪFALSE��ʾ����Ҫ����.
	//��֮���������ڵ�λ����������˵���Ƿǳ���ȷ,��ΪTRUE,��ʾ��Ҫ����.
	BOOL		bColor;
	int			isMacro;//��ø��д�������
	int			iLineNum = 0;
	BOOL		bMacro;//��ʾ�Ƿ���Ҫ��IsMacro�����л����Ϣ,TRUE��,FALSE:��
	BOOL		bifndef;//�Ƿ���"#ifndef"�����
	long		nStartChar,nEndChar;
	short		iFileLineCount;//�����ļ�����

	if (pRichEdit == NULL)
		return bufNew;

	CWaitCursor wait;

	if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
	{
		bufNew = "";
		iFileLineCount = 0;
		while (file.ReadString(bufRead))
		{
			bufNew = bufNew + bufRead + "\r\n";
			iFileLineCount++;
		}
		file.Close();
		pRichEdit->SetWindowText(bufNew);

		m_Progress_Macro.ShowWindow(SW_SHOW);
		m_Progress_Macro.SetRange(0,iFileLineCount);
		m_Progress_Macro.SetStep(1);
	}
//****************************************************************
	if (file.Open(filename,CFile::modeRead,NULL))//�򿪳ɹ�
	{
		bMacroBegin = FALSE;
		bMacro		= TRUE;
		bColor		= FALSE;//�Ƿ���Ҫ��ʼ��ɫ
		bufNew		= "";

		nStartChar	= 0;
		nEndChar	= 0;
		iLineNum	= 0;
		
		bifndef		= FALSE;

		cSingList.RemoveAll();
		while (1)
		{
			if (bMacro)
			{
				if (file.ReadString(bufRead))
					isMacro = IsMacro(bufRead,cmacro);//��ø��д�������
				else
					break;
			}
			switch (isMacro)
			{
			case 1://bufRead�а���strMacro����ĺ�
				nStartChar = bufNew.GetLength();
				
				if (bMacro)
				{
					bufNew = bufNew + bufRead + "\r\n";
					iLineNum++;
					m_Progress_Macro.SetPos(iLineNum);

					nEndChar = bufNew.GetLength();
					pRichEdit->SetSel(nStartChar,nEndChar);
					pRichEdit->SetSelectionCharFormat(chFormat);//ʹĳһ��(#if...)��ɫ

					CSingList_Add(iLineNum);
				}
				while (1)
				{
					bMacroBegin = FALSE;
					if (file.ReadString(bufRead))
					{
						bufNew = bufNew + bufRead + "\r\n";
						iLineNum++;
						m_Progress_Macro.SetPos(iLineNum);

						int iMacroType = GetMacroType(bufRead);
						if (iMacroType == 5)//#endif
						{
							if (bifndef)//���#endif��#ifndef�Ľ����ĺ�
							{
								bifndef = FALSE;
								continue;
							}
							if (bColor)
								bColor = FALSE;
							else
								nStartChar = bufNew.GetLength() - bufRead.GetLength() - strlen("\r\n");

							nEndChar = bufNew.GetLength();
							pRichEdit->SetSel(nStartChar,nEndChar);
							pRichEdit->SetSelectionCharFormat(chFormat);

							CSingList_Add(iLineNum);
							break;
						}
						else if ((iMacroType == 3) || (iMacroType == 4))//#elif,#else
						{
							if (!bColor)
							{
								bColor = TRUE;//��¼��Ҫ��ɫ��λ��
								nStartChar = bufNew.GetLength() - bufRead.GetLength() - strlen("\r\n");
								
								CSingList_Add(iLineNum);
							}
							continue;
						}
						else if (iMacroType == 6)//#ifndef
							bifndef = TRUE;
					}
					else//�Ѿ������ļ���β,����궨���ǲ�������
					{
						file.Close();
						m_Progress_Macro.ShowWindow(SW_HIDE);
						return bufNew;
					}
				}
				bMacro = TRUE;
				break;
			case 0://bufReadֻ��һ��Ĵ���,���ں궨��֮��
				bufNew = bufNew + bufRead + "\r\n";
				iLineNum++;
				m_Progress_Macro.SetPos(iLineNum);
				bMacro = TRUE;
				break;
			case -1://bufRead���������ĺ궨��,������strMacro
				nStartChar = bufNew.GetLength();

				if (bMacro)
				{
					bufNew = bufNew + bufRead + "\r\n";
					iLineNum++;
					m_Progress_Macro.SetPos(iLineNum);

					nEndChar = bufNew.GetLength();
					pRichEdit->SetSel(nStartChar,nEndChar);
					pRichEdit->SetSelectionCharFormat(chFormat);//ʹĳһ�б�ɫ

					CSingList_Add(iLineNum);
				}
				while (1)
				{
					bMacroBegin = TRUE;
					if (file.ReadString(bufRead))
					{
						bufNew = bufNew + bufRead + "\r\n";
						iLineNum++;
						m_Progress_Macro.SetPos(iLineNum);

						if (!bColor)
						{
							bColor = TRUE;//��¼��Ҫ��ɫ��λ��
							nStartChar = bufNew.GetLength() - bufRead.GetLength() - strlen("\r\n");
						}
						int iMacroType = GetMacroType(bufRead);
						if (iMacroType == 5)//#endif
						{
							if (bifndef)//���#endif��#ifndef�Ľ����ĺ�
							{
								bifndef = FALSE;
								continue;
							}
							bMacro = TRUE;//�����ĺ궨���Ѿ�������,��Ҫ���»�ô��������

							nEndChar = bufNew.GetLength();
							pRichEdit->SetSel(nStartChar,nEndChar);
							pRichEdit->SetSelectionCharFormat(chFormat);
							bColor = FALSE;

							CSingList_Add(iLineNum);
							break;//����
						}
						if ((iMacroType == 3) || (iMacroType == 4))//#elif,#else
						{
							bMacro = FALSE;//�����ĺ궨���Ѿ�������,��Ҫ���»�ô��������
							isMacro = IsMacro(bufRead,cmacro);

							nEndChar = bufNew.GetLength();
							pRichEdit->SetSel(nStartChar,nEndChar);
							pRichEdit->SetSelectionCharFormat(chFormat);
							bColor = FALSE;

							CSingList_Add(iLineNum);
							break;//����
						}
						else if (iMacroType == 6)//#ifndef
							bifndef = TRUE;
					}
					else//�Ѿ������ļ���β,����궨���ǲ�������
					{
						file.Close();
						m_Progress_Macro.ShowWindow(SW_HIDE);
						return bufNew;
					}
				}
				break;
			case -2://�޷�ȷ�����벿���Ƿ�����صĺ궨����,#else
				if (bMacro)
				{
					bufNew = bufNew + bufRead + "\r\n";
					iLineNum++;
					m_Progress_Macro.SetPos(iLineNum);
				}
				if (bMacroBegin)
				{
					if (file.ReadString(bufRead))
					{
						bufNew = bufNew + bufRead + "\r\n";
						iLineNum++;
						m_Progress_Macro.SetPos(iLineNum);

						int iMacroType = GetMacroType(bufRead);
						if ((iMacroType == 3) || (iMacroType == 4) || (iMacroType == 5))//#elif,#else,#endif
						{
							bMacro = TRUE;
							
							nStartChar = bufNew.GetLength() - bufRead.GetLength() - strlen("\r\n");
							nEndChar = bufNew.GetLength();
							pRichEdit->SetSel(nStartChar,nEndChar);
							pRichEdit->SetSelectionCharFormat(chFormat);

							CSingList_Add(iLineNum);
							break;//��������
						}
					}
					else//�Ѿ������ļ���β,����궨���ǲ�������
					{
						file.Close();
						m_Progress_Macro.ShowWindow(SW_HIDE);
						return (bufNew);
					}
				}
				break;
			}
		}
		file.Close();
	}
	m_Progress_Macro.ShowWindow(SW_HIDE);
	return (bufNew);
}

//return��ǰ����Ԥ��������:0:һ�����,1:#if,2:#ifdef,3:#elif,4:#else,5:#endif,6:#ifndef
int MacroTool::GetMacroType(CString bufRead)
{
	int iSpacePlace;
	CString	sLeft3,sLeft5,sLeft6,sLeft7,sLeft;
	//����ÿһ�г��������ߵ�3/5/6���ַ��Ƿ���#if,#elif/#else,#ifdef

	bufRead.TrimLeft();
	bufRead.TrimRight();
	iSpacePlace = bufRead.Find(" ");

	if (iSpacePlace == -1)//û�пո�
		sLeft = bufRead;
	else//�пո�
		sLeft = bufRead.Left(iSpacePlace);

	if (sLeft == "#if")
	{
		if (bufRead.Find("defined") == -1)
			return 0;
		else
			return 1;
	}
	if (sLeft == "#elif")
		return 3;
	if (sLeft == "#else")
		return 4;
	if (sLeft == "#ifdef")
		return 2;
	if (sLeft == "#endif")
		return 5;
	if (sLeft == "#ifndef")
		return 6;

	return 0;
}
//����bufRead�Ƿ����strMacro����ĺ�
//1:bufRead�а���strMacro����ĺ�
//0:bufReadֻ��һ��Ĵ���,û�к궨��
//-1:bufRead���������ĺ궨��,������strMacro
//-2:�޷�ȷ��
int	MacroTool::IsMacro(CString bufRead,const CString strMacro)
{
	CString	sLeft;//����ÿһ�г��������ߵ�1�������Ƿ���#if,#elif,#ifdef,#else
	CString	cStrMacro;

	if (strMacro.GetLength() == 0)//�궨��Ϊ��
		return 0;

	bufRead.TrimLeft();
	bufRead.TrimRight();
	if (bufRead.Find(" ") == -1)//���д�����û�пո��ʱ��,�����ֿ���
	{
		if ((bufRead == "#else") || (bufRead == "#endif"))
			return -2;
		else
			return 0;
	}
	else
	{
		sLeft = bufRead.Left(bufRead.Find(" "));//ȡ���ո���ߵ��ַ���
	}

	if (sLeft == "#if")
	{
		cStrMacro = bufRead.Right(bufRead.GetLength() - 3);//��ȥ#if
		cStrMacro.TrimLeft();
		cStrMacro.TrimRight();
		if (cStrMacro.Find("//") != -1)//������д�����ע��,���ȥע�͵Ĳ���
			cStrMacro = cStrMacro.Left(cStrMacro.Find("//"));

		if (bufRead.Find("defined") == -1)
			return 0;

		if (cStrMacro.Find(strMacro) == -1)//û���ҵ�����ĺ�,�������ĺ�
			return -1;
		else//�ҵ�������궨��,�ⲿ�ֲ�����Ҫϸ��,��ô�����Ͻ�
			return 1;
	}
	if (sLeft == "#elif")
	{
		cStrMacro = bufRead.Right(bufRead.GetLength() - 5);//��ȥ#elif
		cStrMacro.TrimLeft();
		cStrMacro.TrimRight();
		if (cStrMacro.Find("//") != -1)//������д�����ע��,���ȥע�͵Ĳ���
			cStrMacro = cStrMacro.Left(cStrMacro.Find("//"));

		if (cStrMacro.Find(strMacro) == -1)//û���ҵ�����ĺ�,�������ĺ�
			return -1;
		else//�ҵ�������궨��,�ⲿ�ֲ�����Ҫϸ��,��ô�����Ͻ�
			return 1;
	}
	if (sLeft == "#else")//�޷�ȷ��
		return -2;
	if (sLeft == "#ifdef")
	{
		cStrMacro = bufRead.Right(bufRead.GetLength() - 6);//��ȥ#ifdef
		cStrMacro.TrimLeft();
		cStrMacro.TrimRight();
		if (cStrMacro.Find("//") != -1)//������д�����ע��,���ȥע�͵Ĳ���
			cStrMacro = cStrMacro.Left(cStrMacro.Find("//"));

		if (cStrMacro == strMacro)//�ҵ�������궨��
			return 1;
		else
			return -1;
	}
	return 0;
}

void MacroTool::OnClickListMacroListfile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CRichEditCtrl	* pEditNew = ((CRichEditCtrl *)(GetDlgItem(IDC_RICHEDIT_PREVIEW)));
	CString			cMacro;

	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ

	if (-1 == nSelectedItem)
		return;

	CString strFileName = GetFileName();//����ļ���

	m_Edit_Macro.GetWindowText(cMacro);

	cMacro.TrimLeft();
	cMacro.TrimRight();

	GetMacroReplace(strFileName,cMacro,pEditNew);//��ʾ��ɫ֮��Ĵ���
	
	iCurrentLine = 1;//��ʾ��ǰ��Ϊ��һ��
	*pResult = 0;
}

BOOL MacroTool::WriteFile(const CString filename,const CString cStr)
{
	CString sNewFileName;
	CStdioFile	fNewFile;

	sNewFileName = filename + ".bak";//���ļ���
	if (!fNewFile.Open(sNewFileName,CFile::modeCreate | CFile::modeWrite | CFile::typeText,NULL))
	{//�������ļ�
		fNewFile.Close();
		return FALSE;
	}

	fNewFile.WriteString(cStr);
	fNewFile.Close();
	return TRUE;
}

void MacroTool::OnButtonSingle() 
{
	// TODO: Add your control notification handler code here
	CString	cMacro,cNewInfo;
	CString strFileName = GetFileName();
	
	if (m_ListFile.GetItemCount() == 0)
	{
		MessageBox("����ѡ���ļ�",cClew,MB_ICONINFORMATION|MB_OK);
		OnButtonSelectfile();
		return;
	}
	else if (strFileName.GetLength() == 0)
	{
		MessageBox("��ѡ��һ���ļ�",cClew,MB_ICONINFORMATION|MB_OK);
		m_ListFile.SetFocus();
		return;
	}
	m_Edit_Macro.GetWindowText(cMacro);//��ú������

	cMacro.TrimLeft();
	cMacro.TrimRight();
	if (cMacro.GetLength() == 0)
	{
		if(MessageBox("��û������������,�Ƿ����?", cClew, MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;
	}

	cNewInfo = GetMacroReplace(strFileName,cMacro);
	
	WriteFile(strFileName,cNewInfo);
	MessageBox("����ɹ�!�ļ�����:" + strFileName + ".bak",cClew,MB_ICONINFORMATION|MB_OK);
}

void MacroTool::OnDblclkListMacroListfile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonSingle();
	*pResult = 0;
}

void MacroTool::OnButtonAll() 
{
	// TODO: Add your control notification handler code here
	CString		cMacro,cNewInfo;

	m_Edit_Macro.GetWindowText(cMacro);//��ú������

	cMacro.TrimLeft();
	cMacro.TrimRight();
	if (cMacro.GetLength() == 0)
	{
		if(MessageBox("��û������������,�Ƿ����?",cClew, MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;
	}
	CString strFileName,strPathName;

	int icount = m_ListFile.GetItemCount();//��������

	if (icount == 0)
	{
		MessageBox("����ѡ���ļ�",cClew,MB_ICONINFORMATION|MB_OK);
		OnButtonSelectfile();
		return;
	}

	CWaitCursor wait;
	m_Progress_Macro.ShowWindow(SW_SHOW);
	m_Progress_Macro.SetRange(0,icount);
	m_Progress_Macro.SetStep(1);

	for (int i = 0;i < icount;i++)
	{
		strFileName = m_ListFile.GetItemText(i, 0);
		strPathName = m_ListFile.GetItemText(i, 1);
		strFileName = strPathName + "\\" + strFileName;//����ļ���

		cNewInfo = GetMacroReplace(strFileName,cMacro);
	
		WriteFile(strFileName,cNewInfo);

		m_Progress_Macro.SetPos(i + 1);
	}
	m_Progress_Macro.ShowWindow(SW_HIDE);

	MessageBox("����ɹ�!",cClew,MB_ICONINFORMATION|MB_OK);
}

void MacroTool::OnButtonSetfont() 
{
	// TODO: Add your control notification handler code here
	CFontDialog fDialog;
	fDialog.m_cf.rgbColors = chFormat.crTextColor;
	fDialog.m_cf.lpLogFont = &logfont;
	fDialog.m_cf.Flags = fDialog.m_cf.Flags|CF_INITTOLOGFONTSTRUCT;

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

void MacroTool::OnKeydownListMacroListfile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pLVKeyDow->wVKey == VK_DELETE)
	{
		MacroTool::DeleteListCtrl();
	}
	*pResult = 0;
}
BOOL MacroTool::DeleteListCtrl()
{
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
	
	if (nSelectedItem != -1)
	{
		CString str;
		int selectedcount = m_ListFile.GetSelectedCount();
		if (selectedcount > 1)//ѡ���˶���
		{
			if(MessageBox("���б�����ɾ����Щ�ļ�?", cClew, MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
				int icount = m_ListFile.GetItemCount();//��������
				for (int i = icount - 1;i >= 0;i--)
				{
					if (m_ListFile.GetItemState(i,LVIS_SELECTED))
						m_ListFile.DeleteItem(i);
				}
			}
		}
		else//ѡ����һ��
		{
			str = m_ListFile.GetItemText(nSelectedItem,0);//����ļ���
			if(MessageBox("���б�����ɾ���ļ���" + str + "��?", cClew, MB_YESNO|MB_ICONQUESTION)==IDYES)
				m_ListFile.DeleteItem(nSelectedItem);	
		}
	}
	return TRUE;
}

void MacroTool::OnButtonDown() 
{
	// TODO: Add your control notification handler code here
	int iLine = GetMacroPlace(DOWN);

	if (-1 == iLine)
		return;
	m_FilePreview.LineScroll(iLine - iCurrentLine);
	if (iLine == 0)
		iCurrentLine = 1;
	else
		iCurrentLine = iLine;
}
//�����Ҫ��������һ�е�����
//direction:��Ҫ�����ķ���
int MacroTool::GetMacroPlace(DIRECTION direction) 
{
	int iListCount,i,iPosition = 0;

	iListCount = cSingList.GetSize();

	switch (direction)
	{
	case DOWN:
		for (i = 0;i < iListCount;i++)
		{
			iPosition = cSingList.GetAt(i);//�����Ҫ��ɫ���е��к�
			if (iPosition > iCurrentLine)
				return (iPosition);//�ҵ�����һ��
		}
		break;
	case UP:
		int iNearLine = 0;
		for (i = 0;i < iListCount;i++)
		{
			iPosition = cSingList.GetAt(i);
			if (iPosition < iCurrentLine)
				iNearLine = iPosition;
			else 
			{
				if (iNearLine == 0)
					return (cSingList.GetAt(iListCount - 1));
				else
					return (iNearLine);
			}
		}
		break;
	}
	return -1;
}

void MacroTool::OnButtonUp() 
{
	// TODO: Add your control notification handler code here
	int iLine = GetMacroPlace(UP);

	if (-1 == iLine)
		return;
	m_FilePreview.LineScroll(iLine - iCurrentLine);
	if (iLine == 0)
		iCurrentLine = 1;
	else
		iCurrentLine = iLine;
}
CString MacroTool::GetFileName()//����ļ���
{
	POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
	int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);//����б���е�ǰѡ�����Ŀ
	CString strFileName = "",strPathName = "";
	
	if (-1 == nSelectedItem)
		return strFileName;

	strFileName = m_ListFile.GetItemText(nSelectedItem, 0);
	strPathName = m_ListFile.GetItemText(nSelectedItem, 1);

	return(strPathName + "\\" + strFileName);
}

void MacroTool::CSingList_Add(int iLineNum)
{
	cSingList.Add(iLineNum);
}

void MacroTool::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CRect lrect;
		CPoint cPoint = point;

		m_ListFile.GetWindowRect(lrect);
		ScreenToClient(lrect);
		ScreenToClient(&cPoint);//��listctrl�ؼ�����ʾ�˵�

		if ((lrect.left <= cPoint.x) && (cPoint.x <= lrect.left + lrect.right) &&
			(lrect.top <= cPoint.y) && (cPoint.y <= lrect.top + lrect.bottom))
		{
			CMenu menu;
			VERIFY(menu.LoadMenu(CG_IDR_POPUP_MACRO_TOOL));
			
			CMenu* pPopup = menu.GetSubMenu(0);

			pPopup->SetMenuItemBitmaps(ID_MENU_SINGLEOUT,MF_BYCOMMAND,&cMBSingleOut,&cMBSingleOut);
			pPopup->SetMenuItemBitmaps(ID_MENU_DELETE,MF_BYCOMMAND,&cMBDelete,&cMBDelete);

			ASSERT(pPopup != NULL);
			CWnd* pWndPopupOwner = this;
			
			while (pWndPopupOwner->GetStyle() & WS_CHILD)
				pWndPopupOwner = pWndPopupOwner->GetParent();
			
			if (m_ListFile.GetItemCount() == 0)
			{
				pPopup->EnableMenuItem(ID_MENU_SINGLEOUT,MF_GRAYED);
				pPopup->EnableMenuItem(ID_MENU_DELETE,MF_GRAYED);
			}
			else
			{
				POSITION pos = m_ListFile.GetFirstSelectedItemPosition();
				int nSelectedItem = m_ListFile.GetNextSelectedItem(pos);
				if (nSelectedItem == -1)
				{
					pPopup->EnableMenuItem(ID_MENU_SINGLEOUT,MF_GRAYED);
					pPopup->EnableMenuItem(ID_MENU_DELETE,MF_GRAYED);
				}
			}

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
				pWndPopupOwner);
		}
	}
}

void MacroTool::OnMenuSingleout() 
{
	// TODO: Add your command handler code here
	OnButtonSingle();
}

void MacroTool::OnMenuDelete() 
{
	// TODO: Add your command handler code here
	DeleteListCtrl();
}

BOOL MacroTool::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	DWORD style = m_Button_SelectFile.GetButtonStyle();
    style = style | BS_DEFPUSHBUTTON;
    m_Button_SelectFile.SetButtonStyle(style,TRUE);

	return CPropertyPage::OnSetActive();
}
