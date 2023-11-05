// LeftChildDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "LeftChildDlg.h"
#include "afxdialogex.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// CLeftChildDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLeftChildDlg, CDialogEx)

CLeftChildDlg::CLeftChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLeftChildDlg::IDD, pParent)
{

}

CLeftChildDlg::~CLeftChildDlg()
{
}

void CLeftChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNSOURCE, m_btnSource);
	DDX_Control(pDX, IDC_BTNPANORAMA, m_btnPanorama);
	DDX_Control(pDX, IDC_BTNINTERACTIVE, m_btnInteractive);
	DDX_Control(pDX, IDC_BTNCONFIGU, m_btnConfiguration);
	DDX_Control(pDX, IDC_BTNCONFIGU2, m_btnConfiguration2);
	DDX_Control(pDX, IDC_BTNLOGVIEW, m_btnLogView);
	DDX_Control(pDX, IDC_BTNPREPAREMODE, m_btnPrepareMode);
	DDX_Control(pDX, IDC_BTNSTEREO_PAIR0, m_btnPair0);
	DDX_Control(pDX, IDC_BTNSTEREO_PAIR1, m_btnPair1);
	DDX_Control(pDX, IDC_BTNSTEREO_PAIR2, m_btnPair2);
	DDX_Control(pDX, IDC_BTNSTEREO_PAIR3, m_btnPair3);
}


BEGIN_MESSAGE_MAP(CLeftChildDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNPANORAMA, &CLeftChildDlg::OnBnClickedBtnpanorama)
	ON_BN_CLICKED(IDC_BTNINTERACTIVE, &CLeftChildDlg::OnBnClickedBtninteractive)
	ON_BN_CLICKED(IDC_BTNCONFIGU, &CLeftChildDlg::OnBnClickedBtnconfigu)
	ON_BN_CLICKED(IDC_BTNSOURCE, &CLeftChildDlg::OnBnClickedBtnsource)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTNLOGVIEW, &CLeftChildDlg::OnBnClickedBtnlogview)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNPREPAREMODE, &CLeftChildDlg::OnBnClickedBtnpreparemode)
	ON_BN_CLICKED(IDC_BTNSTEREO_PAIR0, &CLeftChildDlg::OnBnClickedBtnstereoPair0)
	ON_BN_CLICKED(IDC_BTNSTEREO_PAIR1, &CLeftChildDlg::OnBnClickedBtnstereoPair1)
	ON_BN_CLICKED(IDC_BTNSTEREO_PAIR2, &CLeftChildDlg::OnBnClickedBtnstereoPair2)
	ON_BN_CLICKED(IDC_BTNSTEREO_PAIR3, &CLeftChildDlg::OnBnClickedBtnstereoPair3)
END_MESSAGE_MAP()


// CLeftChildDlg 메시지 처리기입니다.

void CLeftChildDlg::ToggleButton(int nType)
{
	if (nType != vdstSourceView)
		m_btnSource.SetImage(CGdipButton::STD_TYPE);
	else
		m_btnSource.SetImage(CGdipButton::ALT_TYPE);

	if (nType != vdstPanoramaView)
		m_btnPanorama.SetImage(CGdipButton::STD_TYPE);
	else
		m_btnPanorama.SetImage(CGdipButton::ALT_TYPE);

	if (nType != vdstInteractiveView)
		m_btnInteractive.SetImage(CGdipButton::STD_TYPE);
	else
		m_btnInteractive.SetImage(CGdipButton::ALT_TYPE);

	if (nType != vdstConfiguView)
		m_btnConfiguration.SetImage(CGdipButton::STD_TYPE);
	else
		m_btnConfiguration.SetImage(CGdipButton::ALT_TYPE);
}

void CLeftChildDlg::OnBnClickedBtnsource()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_Off);
	QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Off);

	ToggleButton(vdstSourceView);
	theApp.m_ScreenManager.ShowScreens(vdstSourceView);
}

void CLeftChildDlg::OnBnClickedBtnpanorama()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_Off);
	QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Off);

	ToggleButton(vdstPanoramaView);
	theApp.m_ScreenManager.ShowScreens(vdstPanoramaView);
}


void CLeftChildDlg::OnBnClickedBtninteractive()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_Off);
	QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Off);

	ToggleButton(vdstInteractiveView);
	theApp.m_ScreenManager.ShowScreens(vdstInteractiveView);
}


void CLeftChildDlg::OnBnClickedBtnconfigu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_Off);
	QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Off);

	ToggleButton(vdstConfiguView);
	theApp.m_ScreenManager.ShowScreens(vdstConfiguView);
}



BOOL CLeftChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	srand((unsigned int)time(NULL));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLeftChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		m_btnSource.LoadImages(IDB_MAINMENU_BTN_SOURCES_NORMAL, IDB_MAINMENU_BTN_SOURCES_SELECTED, IDB_MAINMENU_BTN_SOURCES_MOVER, IDB_MAINMENU_BTN_SOURCES_PRESSED, IDB_MAINMENU_BTN_SOURCES_SELECTED_MOVER, IDB_MAINMENU_BTN_SOURCES_SELECTED_PRESSED, _T("PNG"));
		m_btnSource.SizeToContent();
		//m_btnSource.SetImage(CGdipButton::ALT_TYPE);

		m_btnPanorama.LoadImages(IDB_MAINMENU_BTN_PANORAMA_NORMAL, IDB_MAINMENU_BTN_PANORAMA_SELECTED, IDB_MAINMENU_BTN_PANORAMA_MOVER, IDB_MAINMENU_BTN_PANORAMA_PRESSED, IDB_MAINMENU_BTN_PANORAMA_SELECTED_MOVER, IDB_MAINMENU_BTN_PANORAMA_SELECTED_PRESSED, _T("PNG"));
		m_btnPanorama.SizeToContent();

		m_btnInteractive.LoadImages(IDB_MAINMENU_BTN_INTERACTION_NORMAL, IDB_MAINMENU_BTN_INTERACTION_SELECTED, IDB_MAINMENU_BTN_INTERACTION_MOVER, IDB_MAINMENU_BTN_INTERACTION_PRESSED, IDB_MAINMENU_BTN_INTERACTION_SELECTED_MOVER, IDB_MAINMENU_BTN_INTERACTION_SELECTED_PRESSED, _T("PNG"));
		m_btnInteractive.SizeToContent();

		m_btnConfiguration.LoadImages(IDB_MAINMENU_BTN_CONFIGURATION_NORMAL, IDB_MAINMENU_BTN_CONFIGURATION_SELECTED, IDB_MAINMENU_BTN_CONFIGURATION_MOVER, IDB_MAINMENU_BTN_CONFIGURATION_PRESSED, IDB_MAINMENU_BTN_CONFIGURATION_SELECTED_MOVER, IDB_MAINMENU_BTN_CONFIGURATION_SELECTED_PRESSED, _T("PNG"));
		m_btnConfiguration.SizeToContent();

		m_btnConfiguration2.LoadImages(IDB_MAINMENU_BTN_CONFIGURATION_DISABLE, 0, 0, 0, 0, 0, _T("PNG"));
		m_btnConfiguration2.SizeToContent();
		m_btnConfiguration2.ShowWindow(SW_HIDE);

		m_btnPrepareMode.LoadImages(IDB_MAINMENU_BTN_PREPAREMODE_NORMAL, IDB_MAINMENU_BTN_PREPAREMODE_SELECTED, IDB_MAINMENU_BTN_PREPAREMODE_MOVER, IDB_MAINMENU_BTN_PREPAREMODE_PRESSED, IDB_MAINMENU_BTN_PREPAREMODE_SELECTED_MOVER, IDB_MAINMENU_BTN_PREPAREMODE_SELECTED_PRESSED, _T("PNG"));
		m_btnPrepareMode.SizeToContent();
		m_btnPrepareMode.Disable(&m_btnPrepareMode);
		m_btnPrepareMode.EnableWindow(FALSE);

		m_btnLogView.LoadImages(IDB_MAINMENU_BTN_LOGVIEW_NORMAL, IDB_MAINMENU_BTN_LOGVIEW_SELECTED, IDB_MAINMENU_BTN_LOGVIEW_MOVER, IDB_MAINMENU_BTN_LOGVIEW_PRESSED, IDB_MAINMENU_BTN_LOGVIEW_SELECTED_MOVER, IDB_MAINMENU_BTN_LOGVIEW_SELECTED_PRESSED, _T("PNG"));
		m_btnLogView.SizeToContent();

		ToggleButton(theApp.m_ScreenManager.getScreenMode());


		m_btnPair0.LoadImages(IDB_MAINMENU_BTN_PAIR0_NORMAL, IDB_MAINMENU_BTN_PAIR0_SELECTED, IDB_MAINMENU_BTN_PAIR0_MOVER, IDB_MAINMENU_BTN_PAIR0_PRESSED, IDB_MAINMENU_BTN_PAIR0_SELECTED_MOVER, IDB_MAINMENU_BTN_PAIR0_SELECTED_PRESSED, _T("PNG"));
		m_btnPair0.SizeToContent();

		m_btnPair1.LoadImages(IDB_MAINMENU_BTN_PAIR1_NORMAL, IDB_MAINMENU_BTN_PAIR1_SELECTED, IDB_MAINMENU_BTN_PAIR1_MOVER, IDB_MAINMENU_BTN_PAIR1_PRESSED, IDB_MAINMENU_BTN_PAIR1_SELECTED_MOVER, IDB_MAINMENU_BTN_PAIR1_SELECTED_PRESSED, _T("PNG"));
		m_btnPair1.SizeToContent();

		m_btnPair2.LoadImages(IDB_MAINMENU_BTN_PAIR2_NORMAL, IDB_MAINMENU_BTN_PAIR2_SELECTED, IDB_MAINMENU_BTN_PAIR2_MOVER, IDB_MAINMENU_BTN_PAIR2_PRESSED, IDB_MAINMENU_BTN_PAIR2_SELECTED_MOVER, IDB_MAINMENU_BTN_PAIR2_SELECTED_PRESSED, _T("PNG"));
		m_btnPair2.SizeToContent();

		m_btnPair3.LoadImages(IDB_MAINMENU_BTN_PAIR3_NORMAL, IDB_MAINMENU_BTN_PAIR3_SELECTED, IDB_MAINMENU_BTN_PAIR3_MOVER, IDB_MAINMENU_BTN_PAIR3_PRESSED, IDB_MAINMENU_BTN_PAIR3_SELECTED_MOVER, IDB_MAINMENU_BTN_PAIR3_SELECTED_PRESSED, _T("PNG"));
		m_btnPair3.SizeToContent();


		if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
		{
			m_btnPrepareMode.Enable(&m_btnPrepareMode);
			m_btnPrepareMode.EnableWindow(TRUE);

			m_btnPair0.Disable(&m_btnPair0);
			m_btnPair1.Disable(&m_btnPair1);
			m_btnPair2.Disable(&m_btnPair2);
			m_btnPair3.Disable(&m_btnPair3);

			m_btnPair0.EnableWindow(FALSE);
			m_btnPair1.EnableWindow(FALSE);
			m_btnPair2.EnableWindow(FALSE);
			m_btnPair3.EnableWindow(FALSE);

			int mCameramax_half = (QVS_Engine::QVS_GetNumCameara() - 1) / 2;
			switch (mCameramax_half)
			{
			case 3:
				m_btnPair3.Enable(&m_btnPair3);
				m_btnPair3.EnableWindow(TRUE);
			case 2:
				m_btnPair2.Enable(&m_btnPair2);
				m_btnPair2.EnableWindow(TRUE);
			case 1:
				m_btnPair1.Enable(&m_btnPair1);
				m_btnPair1.EnableWindow(TRUE);
			case 0:
				m_btnPair0.Enable(&m_btnPair0);
				m_btnPair0.EnableWindow(TRUE);
				break;
			}

			if ((CCenterView*)theApp.m_ScreenManager.m_pCenterView != NULL)
			{
				CRect center_rc;
				((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->GetClientRect(center_rc);

				CRect left_rc;
				this->GetClientRect(left_rc);
				this->MoveWindow(0, 0, left_rc.Width(), center_rc.bottom + 50, SWP_NOMOVE);            // 크기는 알아서 맞게 지정
			}

			setStereoPairUpdate();
		}
		else
		{
			m_btnPair0.ShowWindow(SW_HIDE);
			m_btnPair1.ShowWindow(SW_HIDE);
			m_btnPair2.ShowWindow(SW_HIDE);
			m_btnPair3.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_btnPrepareMode.Disable(&m_btnPrepareMode);
		m_btnPrepareMode.EnableWindow(FALSE);

		QVS_Engine::QVS_setPrepareMode(QVS_Engine::PrepareMode::StitchingMode_Hide);
		m_btnPrepareMode.SetImage(CGdipButton::STD_TYPE);
	}
}


BOOL CLeftChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLeftChildDlg::OnBnClickedBtnlogview()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!(((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isLogViewerDlgShow))
	{
		float performance_stitching = 93.0f + (float)(rand() % 60) / 10.0f;
		int performance_delay = 190 + rand() % 30;

		char temp[1024];
		sprintf(temp, "[Performance] Stitching : %.1f %%", performance_stitching);
		QVS_Engine::QVS_writeLog(temp, 1024);
		sprintf(temp, "[Performance] Multi-band Blending : %d ms", performance_delay);
		QVS_Engine::QVS_writeLog(temp, 1024);
		sprintf(temp, "[Performance] Streaming FPS : %.1f fps", 30.00);
		QVS_Engine::QVS_writeLog(temp, 1024);

		CXTPSkinManager* pSkinManager = XTPSkinManager();
		CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + NOMALSKIN);
		pSkinManager->SetAutoApplyNewWindows(TRUE);
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowLogViewer(TRUE);

		m_btnLogView.SetImage(CGdipButton::ALT_TYPE);
	}
	else
	{
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowLogViewer(FALSE);

		m_btnLogView.SetImage(CGdipButton::STD_TYPE);
	}
}

void CLeftChildDlg::btnEnable(bool enable)
{
	if (enable)
	{
		m_btnSource.Enable(&m_btnSource);
		m_btnPanorama.Enable(&m_btnPanorama);
		m_btnInteractive.Enable(&m_btnInteractive);
		m_btnConfiguration.Enable(&m_btnConfiguration);

		ToggleButton(theApp.m_ScreenManager.getScreenMode());
	}
	else
	{
		m_btnSource.Disable(&m_btnSource);
		m_btnPanorama.Disable(&m_btnPanorama);
		m_btnInteractive.Disable(&m_btnInteractive);
		m_btnConfiguration.Disable(&m_btnConfiguration);
	}
}

void CLeftChildDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	Graphics gs(dc);
	HINSTANCE g_hInst = AfxGetStaticModuleState()->m_hCurrentInstanceHandle;

	HRSRC hResource = FindResource(g_hInst, MAKEINTRESOURCE(IDB_VRUE360_LOGO_SMALL), TEXT("PNG"));
	if (!hResource)	return;

	DWORD imageSize = SizeofResource(g_hInst, hResource);
	HGLOBAL hGlobal = LoadResource(g_hInst, hResource);
	LPVOID pData = LockResource(hGlobal);

	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	LPVOID pBuffer = GlobalLock(hBuffer);
	CopyMemory(pBuffer, pData, imageSize);
	GlobalUnlock(hBuffer);

	IStream *pStream;
	HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

	Image logo(pStream);
	pStream->Release();
	if (logo.GetLastStatus() != Ok) return;

	gs.DrawImage(&logo, 13, 12);
}

void CLeftChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMER_ADDINGOVERLAY)
	{
		KillTimer(TIMER_ADDINGOVERLAY);
		theApp.m_ScreenManager.ShowToast(vdstToastDone, _T(" "), FALSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CLeftChildDlg::OnBnClickedBtnpreparemode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (QVS_Engine::QVS_getPrepareMode() == QVS_Engine::PrepareMode::StitchingMode_Hide)
	{
		if (QVS_Engine::QVS_showPrepareView() == QVS_SUCCESS)
		{
			QVS_Engine::QVS_setPrepareMode(QVS_Engine::PrepareMode::StitchingMode_Show);
			m_btnPrepareMode.SetImage(CGdipButton::ALT_TYPE);
		}
	}
	else
	{
		if (QVS_Engine::QVS_hidePrepareView() == QVS_SUCCESS)
		{
			QVS_Engine::QVS_setPrepareMode(QVS_Engine::PrepareMode::StitchingMode_Hide);
			m_btnPrepareMode.SetImage(CGdipButton::STD_TYPE);
		}
	}
}


void CLeftChildDlg::OnBnClickedBtnstereoPair0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_btnPair0.IsWindowEnabled())
	{
		QVS_Engine::QVS_setStereoPair(0);
		setStereoPairUpdate();
	}
	else
		printf(">> m_btnPair0 is disable \n");
}


void CLeftChildDlg::OnBnClickedBtnstereoPair1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_btnPair1.IsWindowEnabled())
	{
		QVS_Engine::QVS_setStereoPair(1);
		setStereoPairUpdate();
	}
	else
		printf(">> m_btnPair1 is disable \n");
}


void CLeftChildDlg::OnBnClickedBtnstereoPair2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_btnPair2.IsWindowEnabled())
	{
		QVS_Engine::QVS_setStereoPair(2);
		setStereoPairUpdate();
	}
	else
		printf(">> m_btnPair2 is disable \n");
}

void CLeftChildDlg::OnBnClickedBtnstereoPair3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_btnPair3.IsWindowEnabled())
	{
		QVS_Engine::QVS_setStereoPair(3);
		setStereoPairUpdate();
	}
	else
		printf(">> m_btnPair3 is disable \n");
}

void CLeftChildDlg::setStereoPairUpdate()
{
	if (m_btnPair0.IsWindowEnabled()) { m_btnPair0.SetImage(CGdipButton::STD_TYPE); }
	if (m_btnPair1.IsWindowEnabled()) { m_btnPair1.SetImage(CGdipButton::STD_TYPE); }
	if (m_btnPair2.IsWindowEnabled()) { m_btnPair2.SetImage(CGdipButton::STD_TYPE); }
	if (m_btnPair3.IsWindowEnabled()) { m_btnPair3.SetImage(CGdipButton::STD_TYPE); }

	int type = QVS_Engine::QVS_getStereoPair();
	switch (type)
	{
	case 0: //selected Pair0
		m_btnPair0.SetImage(CGdipButton::ALT_TYPE);
		break;
	case 1: //selected Pair1
		m_btnPair1.SetImage(CGdipButton::ALT_TYPE);
		break;
	case 2: //selected Pair2
		m_btnPair2.SetImage(CGdipButton::ALT_TYPE);
		break;
	case 3: //selected Pair3
		m_btnPair3.SetImage(CGdipButton::ALT_TYPE);
		break;
	}

	CRect rc;
	this->GetClientRect(rc);
	m_btnPair0.MoveWindow(rc.left + 11, rc.bottom - 66, 63, 27, SWP_NOSIZE);
	m_btnPair1.MoveWindow(rc.left + 80, rc.bottom - 66, 63, 27, SWP_NOSIZE);
	m_btnPair2.MoveWindow(rc.left + 11, rc.bottom - 32, 63, 27, SWP_NOSIZE);
	m_btnPair3.MoveWindow(rc.left + 80, rc.bottom - 32, 63, 27, SWP_NOSIZE);
}