// ColorSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ColorSettingDlg.h"
#include "afxdialogex.h"


// CColorSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CColorSettingDlg, CDialogEx)

CColorSettingDlg::CColorSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CColorSettingDlg::IDD, pParent)
	, m_szRedValue(_T("0"))
	, m_szGreenValue(_T("0"))
	, m_szBlueValue(_T("0"))
{
	m_nActiveIdx = -1;
}

CColorSettingDlg::~CColorSettingDlg()
{
}

void CColorSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_RED, m_sliderRed);
	DDX_Control(pDX, IDC_SLIDER_GREEN, m_sliderGreen);
	DDX_Control(pDX, IDC_SLIDER_BLUE, m_sliderBlue);
	DDX_Text(pDX, IDC_EDIT_RED, m_szRedValue);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_szGreenValue);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_szBlueValue);
	
	DDX_Control(pDX, IDC_BTN_COLOR_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_1, m_btnThumb1);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_2, m_btnThumb2);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_3, m_btnThumb3);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_4, m_btnThumb4);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_5, m_btnThumb5);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_6, m_btnThumb6);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_7, m_btnThumb7);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_8, m_btnThumb8);
}


BEGIN_MESSAGE_MAP(CColorSettingDlg, CDialogEx)
	
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_COLOR_SAVE, &CColorSettingDlg::OnBnClickedBtnColorSave)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_1, &CColorSettingDlg::OnBnClickedButtonImage1)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_2, &CColorSettingDlg::OnBnClickedButtonImage2)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_3, &CColorSettingDlg::OnBnClickedButtonImage3)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_4, &CColorSettingDlg::OnBnClickedButtonImage4)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_5, &CColorSettingDlg::OnBnClickedButtonImage5)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_6, &CColorSettingDlg::OnBnClickedButtonImage6)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_7, &CColorSettingDlg::OnBnClickedButtonImage7)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_8, &CColorSettingDlg::OnBnClickedButtonImage8)
END_MESSAGE_MAP()


// CColorSettingDlg 메시지 처리기입니다.
BOOL CColorSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CXTPSkinManager* pSkinManager = XTPSkinManager();

	CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + SUBSKIN);
	pSkinManager->SetWindowTheme(GetSafeHwnd(), pSchema);
	CMDTARGET_RELEASE(pSchema);

	// 최소 값 설정.
	m_sliderRed.SetRangeMin(0);
	m_sliderGreen.SetRangeMin(0);
	m_sliderBlue.SetRangeMin(0);
	// 최대 값을 설정.
	m_sliderRed.SetRangeMax(40);
	m_sliderGreen.SetRangeMax(40);
	m_sliderBlue.SetRangeMax(40);

	m_sliderRed.SetLineSize(1);
	m_sliderGreen.SetLineSize(1);
	m_sliderBlue.SetLineSize(1);

	m_sliderRed.SetPos(20);
	m_sliderGreen.SetPos(20);
	m_sliderBlue.SetPos(20);



	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CColorSettingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	if (m_nActiveIdx != -1)
	{
		m_szRedValue.Format(_T("%d"), m_sliderRed.GetPos() - 20);
		m_szGreenValue.Format(_T("%d"), m_sliderGreen.GetPos() - 20);
		m_szBlueValue.Format(_T("%d"), m_sliderBlue.GetPos() - 20);
	}

	UpdateData(FALSE);
}


void CColorSettingDlg::OnBnClickedBtnColorSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_nActiveIdx == -1)
		return;
	QVS_Engine::QVS_SetManualRGB(m_nActiveIdx, _ttoi(m_szRedValue), _ttoi(m_szGreenValue), _ttoi(m_szBlueValue));
}


void CColorSettingDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//이미지를 가져오자.
	if (bShow)
	{
		m_btnSave.LoadImages(IDB_BTN_SAVE_NORMAL, 0, 0, IDB_BTN_SAVE_PRESSED, 0, 0, _T("PNG"));
		m_btnSave.SizeToContent();
	}

	LoadButtonImage(bShow);
}

void CColorSettingDlg::LoadButtonImage(BOOL bShow)
{
	CString temp, szTempPath, strFilePath;
	//GetTempPath(_MAX_PATH, temp.GetBuffer(MAX_PATH));
	temp = theApp.GetAppDir();	//임시 폴더 변경...프로그램...디렉토리하위로..
	szTempPath.Format(_T("%s"), temp);

	// Temp폴더를 다른곳에?? 나중에 생각 해보자.
	szTempPath.Format(_T("%s"), szTempPath.Right(1));
	if (szTempPath.CompareNoCase(_T("\\")) != 0)
		szTempPath.Format(_T("%s%s"), temp, _T("\\_Q360_\\"));
	else
		szTempPath.Format(_T("%s%s"), temp, _T("_Q360_\\"));

	if (bShow)
	{
		CGdipButtonExt* pGdiBtn = NULL;
		m_nActiveIdx = -1; //재초기화
		
		m_btnThumb1.ShowWindow(SW_HIDE);
		m_btnThumb2.ShowWindow(SW_HIDE);
		m_btnThumb3.ShowWindow(SW_HIDE);
		m_btnThumb4.ShowWindow(SW_HIDE);
		m_btnThumb5.ShowWindow(SW_HIDE);
		m_btnThumb6.ShowWindow(SW_HIDE);
		m_btnThumb7.ShowWindow(SW_HIDE);
		m_btnThumb8.ShowWindow(SW_HIDE);

		while (theApp.m_ScreenManager.GetSnapShotDone())
			((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);

		for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
		{
			switch (i)
			{
			case 0: pGdiBtn = &m_btnThumb1; break;
			case 1: pGdiBtn = &m_btnThumb2; break;
			case 2: pGdiBtn = &m_btnThumb3; break;
			case 3: pGdiBtn = &m_btnThumb4; break;
			case 4: pGdiBtn = &m_btnThumb5; break;
			case 5: pGdiBtn = &m_btnThumb6; break;
			case 6: pGdiBtn = &m_btnThumb7; break;
			case 7: pGdiBtn = &m_btnThumb8; break;
			}
			strFilePath.Format(_T("snapshot_%d.jpg"), i);
			pGdiBtn->LoadImages(szTempPath + strFilePath, IDB_POPUP_SRCTHB1 + i, 0, 0, _T("PNG"));
			pGdiBtn->ShowWindow(SW_SHOW);
			pGdiBtn->EnableButton(TRUE);
			pGdiBtn->SetImage(CGdipButtonExt::STD_TYPE);
		}
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);
		
		m_sliderRed.SetPos(20);
		m_sliderGreen.SetPos(20);
		m_sliderBlue.SetPos(20);
		m_szRedValue.Format(_T("%d"), m_sliderRed.GetPos() - 20);
		m_szGreenValue.Format(_T("%d"), m_sliderGreen.GetPos() - 20);
		m_szBlueValue.Format(_T("%d"), m_sliderBlue.GetPos() - 20);
		m_sliderRed.EnableWindow(FALSE);
		m_sliderGreen.EnableWindow(FALSE);
		m_sliderBlue.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else
	{
		m_btnThumb1.LoadImages(IDB_POPUP_SRCTHB1, IDB_POPUP_SRCTHB1, 0, 0, _T("PNG"));
		m_btnThumb2.LoadImages(IDB_POPUP_SRCTHB2, IDB_POPUP_SRCTHB2, 0, 0, _T("PNG"));
		m_btnThumb3.LoadImages(IDB_POPUP_SRCTHB3, IDB_POPUP_SRCTHB3, 0, 0, _T("PNG"));
		m_btnThumb4.LoadImages(IDB_POPUP_SRCTHB4, IDB_POPUP_SRCTHB4, 0, 0, _T("PNG"));
		m_btnThumb5.LoadImages(IDB_POPUP_SRCTHB5, IDB_POPUP_SRCTHB5, 0, 0, _T("PNG"));
		m_btnThumb6.LoadImages(IDB_POPUP_SRCTHB6, IDB_POPUP_SRCTHB6, 0, 0, _T("PNG"));
		m_btnThumb7.LoadImages(IDB_POPUP_SRCTHB7, IDB_POPUP_SRCTHB7, 0, 0, _T("PNG"));
		m_btnThumb8.LoadImages(IDB_POPUP_SRCTHB8, IDB_POPUP_SRCTHB8, 0, 0, _T("PNG"));
	}

}

void CColorSettingDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	LoadButtonImage(FALSE);
//	ShowWindow(SW_HIDE);

	theApp.m_ScreenManager.m_pBottomPanoChild->PostMessage(WM_CLOSECOLORDLG, 2, 0);
	CDialogEx::OnClose();
}

void CColorSettingDlg::ClickButtons(int i)
{
	m_sliderRed.EnableWindow(TRUE);
	m_sliderGreen.EnableWindow(TRUE);
	m_sliderBlue.EnableWindow(TRUE);

	if (i == IDC_BUTTON_IMAGE_1)
		m_btnThumb1.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb1.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_2)
		m_btnThumb2.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb2.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_3)
		m_btnThumb3.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb3.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_4)
		m_btnThumb4.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb4.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_5)
		m_btnThumb5.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb5.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_6)
		m_btnThumb6.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb6.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_7)
		m_btnThumb7.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb7.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_8)
		m_btnThumb8.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb8.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_1)
		m_btnThumb1.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb1.SetImage(CGdipButtonExt::STD_TYPE);

	m_nActiveIdx = i - (IDC_BUTTON_IMAGE_1);
}

void CColorSettingDlg::SetControlValue()
{
	if (m_nActiveIdx != -1)
	{
		QVS_Engine::QVS_SetRGBIndex(m_nActiveIdx);
		int nIdx = QVS_Engine::QVS_GetRGBIndex();
		if (nIdx == m_nActiveIdx)
		{
			m_szRedValue.Format(_T("%d"), (int)QVS_Engine::QVS_GetRed());
			m_sliderRed.SetPos(20 + _ttoi(m_szRedValue));
			m_szBlueValue.Format(_T("%d"), (int)QVS_Engine::QVS_GetBlue());
			m_sliderBlue.SetPos(20 + _ttoi(m_szBlueValue));
			m_szGreenValue.Format(_T("%d"), (int)QVS_Engine::QVS_GetGreen());
			m_sliderGreen.SetPos(20 + _ttoi(m_szGreenValue));
		}
	}
}


void CColorSettingDlg::OnBnClickedButtonImage1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClickButtons(IDC_BUTTON_IMAGE_1);
	SetControlValue();
	UpdateData(FALSE);
}


void CColorSettingDlg::OnBnClickedButtonImage2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClickButtons(IDC_BUTTON_IMAGE_2);
	SetControlValue();
	UpdateData(FALSE);
}


void CColorSettingDlg::OnBnClickedButtonImage3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClickButtons(IDC_BUTTON_IMAGE_3);
	SetControlValue();
	UpdateData(FALSE);
}


void CColorSettingDlg::OnBnClickedButtonImage4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClickButtons(IDC_BUTTON_IMAGE_4);
	SetControlValue();
	UpdateData(FALSE);
}


void CColorSettingDlg::OnBnClickedButtonImage5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClickButtons(IDC_BUTTON_IMAGE_5);
	SetControlValue();
	UpdateData(FALSE);
}


void CColorSettingDlg::OnBnClickedButtonImage6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClickButtons(IDC_BUTTON_IMAGE_6);
	SetControlValue();
	UpdateData(FALSE);
}


void CColorSettingDlg::OnBnClickedButtonImage7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClickButtons(IDC_BUTTON_IMAGE_7);
	SetControlValue();
	UpdateData(FALSE);
}


void CColorSettingDlg::OnBnClickedButtonImage8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClickButtons(IDC_BUTTON_IMAGE_8);
	SetControlValue();
	UpdateData(FALSE);
}


BOOL CColorSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		OnClose();
	case VK_RETURN:
		return TRUE;
	
	default:
		break;
	}	
	return CDialogEx::PreTranslateMessage(pMsg);
}


