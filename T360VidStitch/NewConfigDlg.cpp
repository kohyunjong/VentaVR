// NewConfigDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "NewConfigDlg.h"
#include "afxdialogex.h"

#ifndef __FOLDERDLG_H__
#include "src/folderdlg.h"
#endif

#ifndef __FILTERED_FOLDERDLG_H__
#include "src/filteredfolderdlg.h"
#endif

// CNewConfigDlg 대화 상자입니다.
#ifndef CAMERA_MAX
#define CAMERA_MAX MAX_CAMERA
#endif

IMPLEMENT_DYNAMIC(CNewConfigDlg, CDialogEx)

CNewConfigDlg::CNewConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewConfigDlg::IDD, pParent)
	, m_szPrjName(_T(""))
{

}

CNewConfigDlg::~CNewConfigDlg()
{
}

void CNewConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO_IMODULETYPE, m_cboInputMoudleType);
	DDX_Control(pDX, IDC_COMBO_NUMBEROFCAM, m_cboNumofCam);
	DDX_Control(pDX, IDC_COMBO_DSTSIZE, m_cboDstSize);
	DDX_Control(pDX, IDC_NUMBEROFCAM, m_txtNumOfCam);
	DDX_Control(pDX, IDC_EDT_WIDTH, m_txtSourceWidth);
	DDX_Control(pDX, IDC_EDT_HEIGHT, m_txtSourceHeight);
	DDX_Control(pDX, IDC_EDTPRJPATH, m_txtPrjBasePath);
	DDX_Control(pDX, IDC_EDTPRJNAME, m_txtPrjName);
	DDX_Control(pDX, IDC_BTN_CONFIG_BROW, m_btnBrower);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDTPRJNAME, m_szPrjName);
	DDX_Control(pDX, IDC_COMBO_INPUTSIZE, m_cboInputSize);
}


BEGIN_MESSAGE_MAP(CNewConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CONFIG_BROW, &CNewConfigDlg::OnBnClickedBtnbrowose)
	ON_BN_CLICKED(IDOK, &CNewConfigDlg::OnBnClickedOk)
	ON_EN_UPDATE(IDC_EDTPRJNAME, &CNewConfigDlg::OnEnUpdateEdtprjname)
END_MESSAGE_MAP()


// CNewConfigDlg 메시지 처리기입니다.
// 초기 셋팅 시 '경로'를 무조건 나오게 할려면
int CALLBACK BICallBackFunc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		if (lpData)
			::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);      // lpData 가 경로
		break;
	}

	return 0;
}

BOOL CNewConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	printf("bigheadk, OnInitDialog!!\n");

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	/*CXTPSkinManager* pSkinManager = XTPSkinManager();

	CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + SUBSKIN);
	pSkinManager->SetWindowTheme(GetSafeHwnd(), pSchema);
	CMDTARGET_RELEASE(pSchema);*/

	//화면 초기화.
	CString szTemp;
	m_cboInputMoudleType.SetCurSel(0);
	m_cboNumofCam.SetCurSel(5);//Camera 6개
	m_cboInputSize.SetCurSel(4);//1440x1080
	m_cboDstSize.SetCurSel(2);//4K
	m_txtNumOfCam.SetWindowText(_T("6"));
	
	szTemp.Format(_T("%d"), IMG_W);
	m_txtSourceWidth.SetWindowText(szTemp);
	szTemp.Format(_T("%d"), IMG_H);
	m_txtSourceHeight.SetWindowText(szTemp);
	
#if(!LICENSE_PERSONAL_KEY && LICENSE_MACADDRESS != 0)
	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);
#else
	TCHAR Path[512] = { 0 };
	SHGetSpecialFolderPath(NULL, Path, CSIDL_MYDOCUMENTS, FALSE);
	TCHAR temp[256] = {'\\', 'V', 'R', 'U', 'E', '3', '6', '0'};//"\\VRUE360";
	_tcscat(Path, temp);
	_tmkdir(Path);
#endif

	m_txtPrjBasePath.SetWindowText(Path);

	m_txtPrjName.SetWindowText(_T(""));

	m_btnOK.LoadImages(IDB_BTN_SAVE_NORMAL, 0, 0, IDB_BTN_SAVE_PRESSED, 0, 0, _T("PNG"));
	m_btnOK.SizeToContent();

	m_btnBrower.LoadImages(IDB_BTN_OPENFILE_NORMAL, 0, 0, 0, 0, 0, _T("PNG"));
	m_btnBrower.SizeToContent();
	SetFontObj();

	m_txtPrjName.SetLimitText(30);
	m_txtSourceWidth.SetLimitText(5);
	m_txtSourceHeight.SetLimitText(5);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNewConfigDlg::OnBnClickedBtnbrowose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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


void CNewConfigDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szTemp;

	//먼자 parm에 대해 확인하자..
	int nInputType = m_cboInputMoudleType.GetCurSel();
	int nInputSize = m_cboInputSize.GetCurSel();
	//Output Size
	int nDstType = m_cboDstSize.GetCurSel();
	int nMaxCam = (CAMERA_MAX / 2), nWidth, nHeight, nDstWidth, nDstHeight;
	CString szProjectPath, szProjectName;

	nMaxCam = m_cboNumofCam.GetCurSel() + 1;
	m_bPrjNameOverlap = false;

	//m_txtNumOfCam.GetWindowText(szTemp);
	//if (_ttoi(szTemp) <= 0 || nMaxCam < _ttoi(szTemp))
	//{
	//	szTemp.Format(_T("%d"), nMaxCam);
	//	m_txtNumOfCam.SetWindowText(szTemp);	// Set Max Cam
	//	szTemp.Format(_T("Max Camera Num is %d.\n Please Input less than or equl to Max Camera Num."), nMaxCam);
	//	AfxMessageBox(szTemp, MB_OK, MB_ICONEXCLAMATION);
	//	return;
	//}
	//else
	//	nMaxCam = _ttoi(szTemp);

	//m_txtSourceWidth.GetWindowText(szTemp);
	//if (szTemp.GetLength() == 0 || _ttoi(szTemp) <= 0)
	//{
	//	szTemp = _T("Please input width.");
	//	AfxMessageBox(szTemp, MB_OK, MB_ICONEXCLAMATION);
	//	return;
	//}
	//else
	//	nWidth = _ttoi(szTemp);

	//m_txtSourceHeight.GetWindowText(szTemp);
	//if (szTemp.IsEmpty() || _ttoi(szTemp) <= 0)
	//{
	//	szTemp = _T("Please input Height");
	//	AfxMessageBox(szTemp, MB_OK, MB_ICONEXCLAMATION);
	//	return;
	//}
	//else
	//	nHeight = _ttoi(szTemp);

#if LICENSE_PERSONAL_KEY
	if (nDstType == 3 && theApp.m_ScreenManager.plan_type == QVS_PLANTYPE_4K)
	{
		AfxMessageBox(_T("This license can't create 8K project."), MB_OK, MB_ICONEXCLAMATION);
		return;
	}
#endif

	if ((nDstType == 4 || nDstType == 5) && m_cboNumofCam.GetCurSel() % 2 == 0) //selected stereo
	{
		AfxMessageBox(_T("Stereo need pair. Check the Number of Camera and select even."), MB_OK, MB_ICONEXCLAMATION);
		return;
	}
	
	m_txtPrjBasePath.GetWindowText(szTemp);
	//SetCurrentDirectory(szTemp);
	szProjectPath = szTemp;
	
	m_txtPrjName.GetWindowText(szTemp);

	//프로젝트 이름 중복 확인, 같은 경로에 같은 이름의 파일이 있는지
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

	//여기에 생성이 가능한가???확인해야 하나? KBJ170105 나중에 확인
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
			//yes를 누를 경우, 중복된 이름의 파일이 덮어씌워지면서 진행
			int pos = szTemp.ReverseFind('.');
			if (pos > 0) //확장자가 존재.
			{
				CString strFileTitleOnly = szTemp.Left(szTemp.ReverseFind('.'));
				szProjectName.Format(_T("%s.q360"), strFileTitleOnly);
			}
			else //확장자 없음.
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
			//if (*tchr == '/' ||) {
				//check = true;
			//}
		}
		if (check)
		{
			AfxMessageBox(_T("A file name cannot contain any of the following characters:\n \\ / : *? \" < > |"));
			return;
		}


		m_txtPrjName.SetSel(0, -1);
		m_txtPrjName.SetSel(-1, -1);

		int pos = szTemp.ReverseFind('.');
		if (pos > 0) //확장자가 존재.
		{
			CString strFileTitleOnly = szTemp.Left(szTemp.ReverseFind('.'));
			szProjectName.Format(_T("%s.q360"), strFileTitleOnly);
		}
		else //확장자 없음.
		{
			szProjectName = szTemp + _T(".q360");
		}
	}

	//theApp.m_PrjAllCfg 에 설정한다.
	theApp.m_PrjAllCfg.m_ModuleType = (QVS_Engine::_InputModuleType)nInputType;
	theApp.m_PrjAllCfg.m_nMaxCamra = nMaxCam;
	
	switch (nInputSize)
	{
	case 0:
		nWidth = 720;
		nHeight = 486;
		break;
	case 1:
		nWidth = 720;
		nHeight = 576;
		break;
	case 2:
		nWidth = 1280;
		nHeight = 720;
		break;
	case 3:
		nWidth = 1280;
		nHeight = 960;
		break;
	case 4:
		nWidth = 1440;
		nHeight = 1080;
		break;
	case 5:
		nWidth = 1920;
		nHeight = 1080;
		break;
	case 6:
		nWidth = 2048;
		nHeight = 1080;
		break;
	case 7:
		nWidth = 2048;
		nHeight = 1556;
		break;
	case 8:
		nWidth = 3840;
		nHeight = 2160;
		break;
	case 9:
		nWidth = 4096;
		nHeight = 2160;
		break;
	}
	theApp.m_PrjAllCfg.m_nWidth = nWidth;
	theApp.m_PrjAllCfg.m_nHeight = nHeight;
	theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_Normal;

	switch (nDstType)
	{
	case 0:
		theApp.m_PrjAllCfg.m_nDstWidth = DST_1K_WIDTH;
		theApp.m_PrjAllCfg.m_nDstHeight = DST_1K_HEIGHT;
		break;
	case 1:
		theApp.m_PrjAllCfg.m_nDstWidth = DST_2K_WIDTH;
		theApp.m_PrjAllCfg.m_nDstHeight = DST_2K_HEIGHT;
		break;
	case 2:
		theApp.m_PrjAllCfg.m_nDstWidth = DST_4K_WIDTH;
		theApp.m_PrjAllCfg.m_nDstHeight = DST_4K_HEIGHT;
		break;
	case 3:
		theApp.m_PrjAllCfg.m_nDstWidth = DST_8K_WIDTH;
		theApp.m_PrjAllCfg.m_nDstHeight = DST_8K_HEIGHT;
		break;
	case 4: //Stereo Top/Bottom output is 3840x3840
		theApp.m_PrjAllCfg.m_nDstWidth = DST_STEREO_4K_WIDTH;
		theApp.m_PrjAllCfg.m_nDstHeight = DST_STEREO_4K_HEIGHT;
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_Stereo;
		break;
	case 5: //Stereo Left/Right output is 4096x2048
		theApp.m_PrjAllCfg.m_nDstWidth = DST_STEREO_SIDE_WIDTH;
		theApp.m_PrjAllCfg.m_nDstHeight = DST_STEREO_SIDE_HEIGHT;
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_Stereo_Side;
		break;
	case 6: //Stereo Left/Right output is 3840x1920
		theApp.m_PrjAllCfg.m_nDstWidth = DST_2K_WIDTH;
		theApp.m_PrjAllCfg.m_nDstHeight = DST_4K_HEIGHT;
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_Stereo_Side;
		break;
	case 7:
		theApp.m_PrjAllCfg.m_nDstWidth = DST_4K_WIDTH;
		theApp.m_PrjAllCfg.m_nDstHeight = DST_4K_HEIGHT;
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_StitchedInput;
		break;
	}

	
	//theApp.m_PrjAllCfg.m_nFov = QVS_Engine::QVS_getFOV();
	theApp.m_PrjAllCfg.m_szProjectFullPath = szProjectPath;
	theApp.m_PrjAllCfg.m_szProjectName = szProjectName;
	theApp.m_ScreenManager.isNewProject = true;

	CDialogEx::OnOK();
}

void CNewConfigDlg::OnEnUpdateEdtprjname()
{
	/*
	UpdateData(TRUE);

	TCHAR* tchr;
	CString str = _T("");
	int i;

	CString strTest;
	m_txtPrjName.GetWindowText(strTest);

	bool check = false;

	for (i = 0; i < strTest.GetLength(); i++) {
		str = strTest.Mid(i, 1);
		tchr = (TCHAR*)(LPCTSTR)str;

		if (!IsCharAlphaNumericA(*tchr)) {
			check = true;
			strTest.Remove(*tchr);
			m_txtPrjName.SetWindowTextW(strTest);
		}
	}
	if (check)
		AfxMessageBox(_T("Only alphabet or number"));

	UpdateData(FALSE);

	m_txtPrjName.SetSel(0, -1);
	m_txtPrjName.SetSel(-1, -1);
	*/
}


void CNewConfigDlg::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	CFont* aria11 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria11);
	CFont* aria13 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria13);

	GetDlgItem(IDC_STATIC_11_1)->SetFont(aria11);
	GetDlgItem(IDC_STATIC_11_2)->SetFont(aria11);
	GetDlgItem(IDC_STATIC_11_3)->SetFont(aria11);
	GetDlgItem(IDC_STATIC_11_4)->SetFont(aria11);
	GetDlgItem(IDC_STATIC_11_5)->SetFont(aria11);
	GetDlgItem(IDC_STATIC_11_6)->SetFont(aria11);

	GetDlgItem(IDC_NUMBEROFCAM)->SetFont(aria11);
	GetDlgItem(IDC_COMBO_IMODULETYPE)->SetFont(aria11);
	GetDlgItem(IDC_COMBO_DSTSIZE)->SetFont(aria11);
	GetDlgItem(IDC_EDT_WIDTH)->SetFont(aria11);
	GetDlgItem(IDC_EDT_HEIGHT)->SetFont(aria11);
	GetDlgItem(IDC_EDTPRJPATH)->SetFont(aria11);
	GetDlgItem(IDC_EDTPRJNAME)->SetFont(aria11);

}