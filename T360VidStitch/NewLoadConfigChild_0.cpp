// NewLoadConfigChild_0.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "NewLoadConfigChild_0.h"
#include "afxdialogex.h"
#include "FolderDlg.h"

// CNewLoadConfigChild_0 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNewLoadConfigChild_0, CDialogEx)

CNewLoadConfigChild_0::CNewLoadConfigChild_0(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CNewLoadConfigChild_0::~CNewLoadConfigChild_0()
{
}

void CNewLoadConfigChild_0::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_BROW, m_btnBrowse);
	DDX_Control(pDX, IDOK, m_btnSave);
	DDX_Control(pDX, IDC_NEWLOAD_EDTPRJPATH, m_txtPrjBasePath);
	DDX_Control(pDX, IDC_NEWLOAD_EDTPRJNAME, m_txtPrjName);
}


BEGIN_MESSAGE_MAP(CNewLoadConfigChild_0, CTaskPanelDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CONFIG_BROW, &CNewLoadConfigChild_0::OnBnClickedBtnConfigBrow)
	ON_BN_CLICKED(IDOK, &CNewLoadConfigChild_0::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CNewLoadConfigChild_0 �޽��� ó�����Դϴ�.
BOOL CNewLoadConfigChild_0::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	resetUI();

	m_btnSave.LoadBitmaps(IDBB_BTN_NEW_NORMAL, IDBB_BTN_NEW_PRESSED, 0, 0);
	m_btnSave.SizeToContent();

	m_btnBrowse.LoadBitmaps(IDBB_BTN_OPENFILE_NORMAL, 0, 0, IDBB_BTN_OPENFILE_DISABLE);
	m_btnBrowse.SizeToContent();

	SetFontObj();

	m_txtPrjName.SetLimitText(30);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CNewLoadConfigChild_0::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CTaskPanelDlg::PreTranslateMessage(pMsg);
}

void CNewLoadConfigChild_0::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.v
	switch (nIDEvent)
	{
	case TIMER_INTO_CENTERSCREEN:
	{
		//if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->m_bFirstTime)
		if (!theApp.m_PrjAllCfg.isQVSEngineInit())
			break;
		KillTimer(TIMER_INTO_CENTERSCREEN);
		theApp.m_ScreenManager.ShowScreens(vdstSourceView);
		((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild)->btnEnable(true);
	}
	break;
	}

	CTaskPanelDlg::OnTimer(nIDEvent);
}

void CNewLoadConfigChild_0::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	CFont* aria11 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(TAria11);
	CFont* aria13 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria13);

	GetDlgItem(IDC_STATIC_NEWLOAD_1)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_NEWLOAD_2)->SetFont(aria12);

	GetDlgItem(IDC_NEWLOAD_EDTPRJPATH)->SetFont(aria12);
	GetDlgItem(IDC_NEWLOAD_EDTPRJNAME)->SetFont(aria12);

}

void CNewLoadConfigChild_0::resetUI()
{
#if(!LICENSE_PERSONAL_KEY && LICENSE_MACADDRESS != 0)
	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);
#else
	TCHAR Path[512] = { 0 };
	SHGetSpecialFolderPath(NULL, Path, CSIDL_MYDOCUMENTS, FALSE);
	TCHAR temp[256] = { '\\', 'V', 'R', 'U', 'E', '3', '6', '0' };//"\\VRUE360";
	_tcscat(Path, temp);
	_tmkdir(Path);
#endif

	m_txtPrjBasePath.SetWindowText(Path);
	m_txtPrjName.SetWindowText(_T(""));
}

void CNewLoadConfigChild_0::OnBnClickedBtnConfigBrow()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString currentPath = _T("");
	LPCTSTR lpszTitle = _T("Select the root folder for the browse dialog:");
	UINT	uFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

	m_txtPrjBasePath.GetWindowText(currentPath);

	CFolderDialog dlgRoot(lpszTitle, currentPath, this, uFlags);

	if (dlgRoot.DoModal() == IDOK)
	{
		//currentPath = dlgRoot.GetFolderPath();
		m_txtPrjBasePath.SetWindowText(dlgRoot.GetFolderPath());
		//UpdateData(FALSE);
	}
}


void CNewLoadConfigChild_0::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CNewLoadView* m_pNewLoadView = (CNewLoadView*)theApp.m_ScreenManager.m_pNewLoadView;
#if LICENSE_PERSONAL_KEY
	if (m_pNewLoadView)
	{
		if (m_pNewLoadView->mQLicense != NULL)
			theApp.m_ScreenManager.plan_type = m_pNewLoadView->mQLicense->checkKeyScope(m_pNewLoadView->HL_key_id, m_pNewLoadView->HL_key_year, m_pNewLoadView->HL_key_month, m_pNewLoadView->HL_key_day);
		else
		{
			AfxMessageBox(_T("License Error1 : We are opening file as demo."), MB_OK, MB_ICONEXCLAMATION);
			theApp.m_ScreenManager.plan_type = QVS_PLANTYPE_DEMO;
		}
	}
#endif

	CString szTemp, szProjectPath, szProjectName;
	m_bPrjNameOverlap = false;

	m_txtPrjBasePath.GetWindowText(szTemp);
	szProjectPath = szTemp;

	m_txtPrjName.GetWindowText(szTemp);

	//������Ʈ �̸� �ߺ� Ȯ��, ���� ��ο� ���� �̸��� ������ �ִ���
	CFileFind finder;
	CString fileName = szTemp + _T(".q360");
	BOOL bWorking = finder.FindFile(szProjectPath + _T("\\*.q360"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString comparePath = finder.GetFilePath();
		CString filePath = szProjectPath + _T("\\") + fileName;

		comparePath.MakeLower();
		filePath.MakeLower();

		if (comparePath == filePath)
			m_bPrjNameOverlap = true;
	}

	//���⿡ ������ �����Ѱ�???Ȯ���ؾ� �ϳ�? KBJ170105 ���߿� Ȯ��
	if (theApp.m_PrjAllCfg.getNewType() == CProjAllConfig::NEWTYPE::None)
	{
		szTemp = _T("Please set project type.");
		AfxMessageBox(szTemp, MB_OK, MB_ICONEXCLAMATION);
		return;
	}

	if (theApp.m_PrjAllCfg.getNewType() == CProjAllConfig::NEWTYPE::Wide_Panorama && 
		//(theApp.m_PrjAllCfg.m_nDstWidth < 1 || theApp.m_PrjAllCfg.m_nDstWidth > 7680 || theApp.m_PrjAllCfg.m_nDstHeight < 1 || theApp.m_PrjAllCfg.m_nDstHeight > 2160))
#if(LICENSE_AVM)
		(theApp.m_PrjAllCfg.m_nDstHeight < 100 || theApp.m_PrjAllCfg.m_nDstHeight > 1920))
	{
		szTemp = _T("Please check value of the Output Size : \n (Height : 100 ~ 1920)");
#else
		(theApp.m_PrjAllCfg.m_nDstHeight < 1920 || theApp.m_PrjAllCfg.m_nDstHeight > 3840))
	{
		szTemp = _T("Please check value of the Output Size : \n (Height : 1920 ~ 3840)");
#endif
		AfxMessageBox(szTemp, MB_OK, MB_ICONEXCLAMATION);
		return;
	}

	if (szTemp.IsEmpty())
	{
		szTemp = _T("Please input project name.");
		AfxMessageBox(szTemp, MB_OK, MB_ICONEXCLAMATION);
		return;
	}
	else if (m_bPrjNameOverlap)
	{
		int result = AfxMessageBox(_T("We can not be case-sensitive.\nThe project name already exists. Do you want to overwrite the file?"), MB_YESNO, MB_ICONEXCLAMATION);
		if (result == IDYES)
		{
			//yes�� ���� ���, �ߺ��� �̸��� ������ ��������鼭 ����
			int pos = szTemp.ReverseFind('.');
			if (pos > 0) //Ȯ���ڰ� ����.
			{
				CString strFileTitleOnly = szTemp.Left(szTemp.ReverseFind('.'));
				szProjectName.Format(_T("%s.q360"), strFileTitleOnly);
			}
			else //Ȯ���� ����.
			{
				szProjectName = szTemp + _T(".q360");
			}
		}
		else
			return;
	}
	else
	{

		TCHAR* tchr;
		CString str = _T("");
		int i;

		bool check = false;

		for (i = 0; i < szTemp.GetLength(); i++) {
			str = szTemp.Mid(i, 1);
			tchr = (TCHAR*)(LPCTSTR)str;

			switch (*tchr)
			{
			case '/':
			case '\\':
			case ':':
			case '*':
			case '?':
			case '"':
			case '<':
			case '>':
			case '|':
				check = true;
				break;
			}
		}
		if (check)
		{
			AfxMessageBox(_T("A file name cannot contain any of the following characters:\n \\ / : *? \" < > |"));
			return;
		}


		m_txtPrjName.SetSel(0, -1);
		m_txtPrjName.SetSel(-1, -1);

		int pos = szTemp.ReverseFind('.');
		if (pos > 0) //Ȯ���ڰ� ����.
		{
			CString strFileTitleOnly = szTemp.Left(szTemp.ReverseFind('.'));
			szProjectName.Format(_T("%s.q360"), strFileTitleOnly);
		}
		else //Ȯ���� ����.
		{
			szProjectName = szTemp + _T(".q360");
		}
	}
	theApp.m_PrjAllCfg.m_szProjectFullPath = szProjectPath;
	theApp.m_PrjAllCfg.m_szProjectName = szProjectName;

	//int type = ((CNewLoadConfigChild*)this->GetParent())->getConfigNewType();
	((CNewLoadConfigChild*)this->GetParent())->setConfigNumOfCam(theApp.m_PrjAllCfg.getNewType());
	((CNewLoadConfigChild*)this->GetParent())->setConfigInputSize(theApp.m_PrjAllCfg.getNewType());
	((CNewLoadConfigChild*)this->GetParent())->setConfigOutputSize(theApp.m_PrjAllCfg.getNewType());
	theApp.m_ScreenManager.isNewProject = true;

	
	if (!theApp.m_PrjAllCfg.m_restart)
	{
		theApp.m_ScreenManager.m_iStartType = 0;
		theApp.m_ScreenManager.CreateAllScreen(vdstCenter);//�ٸ� ȭ���� �����.
		//�ӽ� �Լ�...
		//theApp.m_ScreenManager.parsingPTS(_T("0712.pts"));

		//������Ʈ���� 
		theApp.addToRecentFile(theApp.m_PrjAllCfg.m_szProjectFullPath + _T("\\") + theApp.m_PrjAllCfg.m_szProjectName);
		theApp.m_ScreenManager.SetProjectName();
		theApp.m_ScreenManager.saveProject();

		//���⼭ ��� ȭ���� Config���� �����Ѵ�.
		//theApp.m_ScreenManager.initConfiguration(typeNew);
		//Project�� �����Ѵ�.
		//theApp.m_ScreenManager.ShowScreens(vdstSourceView);
		theApp.m_ScreenManager.PreScreens(vdstSourceView);
		SetTimer(TIMER_INTO_CENTERSCREEN, 100, NULL);
	}
	else
	{
		//������Ʈ���� 

		CCenterView * pCenterView = (CCenterView *)(theApp.m_ScreenManager.m_pCenterView);
		pCenterView->initQVSEngine();

		if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone == FALSE)
			return;

		theApp.addToRecentFile(theApp.m_PrjAllCfg.m_szProjectFullPath + _T("\\") + theApp.m_PrjAllCfg.m_szProjectName);
		theApp.m_ScreenManager.saveProject();

		theApp.m_ScreenManager.m_iStartType = 0;
		theApp.m_ScreenManager.SetProjectName();

		theApp.m_ScreenManager.ShowScreens(vdstSourceView);

		SetTimer(TIMER_INTO_CENTERSCREEN, 100, NULL);
	}

	if (m_pNewLoadView)
		m_pNewLoadView->resetUI();

	/*printf(">>>>> theApp.m_PrjAllCfg.m_newType			 = %d\n", theApp.m_PrjAllCfg.getNewType());
	printf(">>>>> theApp.m_PrjAllCfg.m_szProjectFullPath = %s\n", theApp.m_PrjAllCfg.m_szProjectFullPath);
	printf(">>>>> theApp.m_PrjAllCfg.m_szProjectName	 = %s\n", theApp.m_PrjAllCfg.m_szProjectName);
	printf(">>>>> theApp.m_PrjAllCfg.m_nMaxCamra		 = %d\n", theApp.m_PrjAllCfg.m_nMaxCamra);
	printf(">>>>> theApp.m_PrjAllCfg.m_nWidth			 = %d\n", theApp.m_PrjAllCfg.m_nWidth);
	printf(">>>>> theApp.m_PrjAllCfg.m_nHeight			 = %d\n", theApp.m_PrjAllCfg.m_nHeight);
	printf(">>>>> theApp.m_PrjAllCfg.m_nDstWidth		 = %d\n", theApp.m_PrjAllCfg.m_nDstWidth);
	printf(">>>>> theApp.m_PrjAllCfg.m_nDstHeight		 = %d\n", theApp.m_PrjAllCfg.m_nDstHeight);
	printf(">>>>> theApp.m_PrjAllCfg.m_nStitchingMode	 = %d\n", theApp.m_PrjAllCfg.m_nStitchingMode);*/

	m_txtPrjName.SetWindowTextW(_T(""));
	UpdateData(FALSE);
}


HBRUSH CNewLoadConfigChild_0::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_GRAY_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_NEWLOAD_1:
	case IDC_STATIC_NEWLOAD_2:
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	break;
	case IDC_NEWLOAD_EDTPRJPATH:
		pDC->SetTextColor(COLOR_DIALOG_TEXT);
		pDC->SetBkColor(COLOR_DIALOG_GRAY_BK);
		hbr = hbrNomalBack;
		break;
	case IDC_NEWLOAD_EDTPRJNAME: // Edit box
		pDC->SetTextColor(COLOR_DIALOG_TEXT);
		pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
		hbr = hbrNomalBack;
		break;
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


BOOL CNewLoadConfigChild_0::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, COLOR_DIALOG_BK);

	return TRUE;
	//return CTaskPanelDlg::OnEraseBkgnd(pDC);
}
