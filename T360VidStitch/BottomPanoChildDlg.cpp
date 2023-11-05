// BottonPanoChildDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "BottomPanoChildDlg.h"
#include "afxdialogex.h"


// CBottomPanoChildDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBottomPanoChildDlg, CDialogEx)

CBottomPanoChildDlg::CBottomPanoChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBottomPanoChildDlg::IDD, pParent)
	, m_nSeam(0)
	, m_nBlending(0)
	, m_nAGainEffect(0)
{
	m_bisShowPanoPreview = FALSE;
	m_bisCapConf = FALSE;
	m_bGainActive = FALSE;
	m_oldAudioPos = 50;
	m_oldVudioPos = 60;
	m_pCurveControlDlg = NULL;
	m_pColorSetlDlg = NULL;
}

CBottomPanoChildDlg::~CBottomPanoChildDlg()
{
	DeleteTempShapShot();
}

void CBottomPanoChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BTMTOPBAR, m_ctlTopBar);
	DDX_Control(pDX, IDC_BTN_CURVES, m_btnCurves1);
	DDX_Control(pDX, IDC_BTN_COLORSET, m_btnColor);
	DDX_Control(pDX, IDC_BTN_NADIR_SET, m_btnNadirOnOff);
	DDX_Control(pDX, IDC_BTN_GAIN, m_btnGainOnOff);
	DDX_Control(pDX, IDC_BTN_FILE_BROW, m_btnBrow);
	DDX_Control(pDX, IDC_COMBO_NADIRPOS, m_cboNadirPos);
	DDX_Control(pDX, IDC_SLD_NADIRHEIGHT, m_sldNadirHeight);
	DDX_Radio(pDX, IDC_RADIO_SEAM1, m_nSeam);
	DDX_Radio(pDX, IDC_RADIO_BLENDING1, m_nBlending);
	DDX_Control(pDX, IDC_COMBO_AGAINMIDE, m_cboAGainMode);
	DDX_Radio(pDX, IDC_RADIO_AGAINEFF1, m_nAGainEffect);
	DDX_Control(pDX, IDC_SLD_DS_AUDIO, m_sldDsAudio);
	DDX_Control(pDX, IDC_SLD_DS_VIDEO, m_sldDsVideo);
	DDX_Control(pDX, IDC_BTN_CURVES2, m_btnCurves2);
	DDX_Control(pDX, IDC_BTN_GRAYSCALE_SET, m_btnGrayOnOff);
	DDX_Control(pDX, IDC_SLD_BRIGHTNESS, m_sldBrightness);
	DDX_Control(pDX, IDC_SLD_CONTRAST, m_sldContrast);
}


BEGIN_MESSAGE_MAP(CBottomPanoChildDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_CURVES, &CBottomPanoChildDlg::OnBnClickedBtnCurves)
	ON_BN_CLICKED(IDC_BTN_COLORSET, &CBottomPanoChildDlg::OnBnClickedBtnColorset)
	ON_BN_CLICKED(IDC_BTN_GAIN, &CBottomPanoChildDlg::OnBnClickedBtnGain)
	ON_BN_CLICKED(IDC_BTN_FILE_BROW, &CBottomPanoChildDlg::OnBnClickedBtnFileBrow)
	ON_BN_CLICKED(IDC_BTN_NADIR_SET, &CBottomPanoChildDlg::OnBnClickedBtnNadirSet)
	ON_CBN_SELCHANGE(IDC_COMBO_NADIRPOS, &CBottomPanoChildDlg::OnCbnSelchangeComboNadirpos)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_SEAM1, IDC_RADIO_SEAM2, &CBottomPanoChildDlg::SetRadioSeamStatus)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_BLENDING1, IDC_RADIO_BLENDING2, &CBottomPanoChildDlg::SetRadioBlendingStatus)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_AGAINEFF1, IDC_RADIO_AGAINEFF2, &CBottomPanoChildDlg::SetRadioGainStatus)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_AGAINMIDE, &CBottomPanoChildDlg::OnCbnSelchangeComboAgainmide)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CLOSECOLORDLG, OnCloseColorDlg)
	ON_BN_CLICKED(IDC_BTN_GRAYSCALE_SET, &CBottomPanoChildDlg::OnBnClickedBtnGrayscaleSet)
END_MESSAGE_MAP()


// CBottomPanoChildDlg 메시지 처리기입니다.


void CBottomPanoChildDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc;
	GetParent()->GetClientRect(rc); // Docking pane Size

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (::IsWindow(m_ctlTopBar.m_hWnd))
	{
		CRect rcClient;
		m_ctlTopBar.GetClientRect(rcClient);
		rcClient.top = 5;
		rcClient.bottom = rcClient.top + rcClient.bottom;
		rcClient.right = rc.right;
		rcClient.left = rc.left;
		m_ctlTopBar.MoveWindow(&rcClient);
		m_ctlTopBar.Invalidate();
	}

}


BOOL CBottomPanoChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_sldNadirHeight.SetRange(1, 60);
	m_cboNadirPos.SetCurSel(1);

	m_sldBrightness.SetRange(0, 20);
	m_sldBrightness.SetPos(10);
	GetDlgItem(IDC_EDT_BRIGHTNESS)->SetWindowText(_T("0"));
	QVS_Engine::QVS_setBrightnessStep(0);

	m_sldContrast.SetRange(0, 10);
	m_sldContrast.SetPos(0);
	GetDlgItem(IDC_EDT_CONTRAST)->SetWindowText(_T("0"));
	QVS_Engine::QVS_setContrastStep(0);

	m_nAGainEffect = 0;

	m_sldDsAudio.SetRange(0, 50);
	m_sldDsAudio.SetLineSize(1);
	m_sldDsAudio.SetPos(50);
	m_oldAudioPos = 50;
	m_sldDsVideo.SetRange(0, 60);
	m_sldDsVideo.SetLineSize(1);
	m_sldDsVideo.SetPos(60);
	m_oldVudioPos = 60;


	SetFontObj();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CBottomPanoChildDlg::UpdateDialogItemValue()
{
	CString szTemp;
	//Seam
	int nSeam = QVS_Engine::QVS_GetSeamMode();
	m_nSeam = (nSeam== 2 ? 0 : 1) ;
	//Blending
	int nExposure = QVS_Engine::QVS_GetExposureMode();
	m_nBlending = (nExposure == 3 ? 0 : 1);

	if (!theApp.m_PrjAllCfg.m_bFirstUpdateBtmPano)
	{
		m_btnGainOnOff.SetImage(CGdipButton::STD_TYPE);
		m_btnCurves1.EnableWindow(FALSE);
		m_btnCurves1.EnableButton(FALSE);
		if (m_pCurveControlDlg)
			m_pCurveControlDlg->ShowWindow(SW_HIDE);

		int nPrevGainSel = m_cboAGainMode.GetCurSel();
		m_cboAGainMode.SetCurSel(0);

		// Gain Setting
		int nGainMode = QVS_Engine::QVS_GetGainMode();
		int nAutoGainMode = QVS_Engine::QVS_GetAutoGainMode();
		m_nAGainEffect = nAutoGainMode;

		theApp.m_ScreenManager.SetGainManCurveStatus(FALSE);
		theApp.m_ScreenManager.SetautoGainModeStatus(FALSE);

		//#17876
		nGainMode = nGainMode - 1;
		//if (QVS_Engine::QVS_getStereoMode()) {
		//	m_btnGainOnOff.Disable(&m_btnGainOnOff);
		//	nGainMode = -1;
		//}
		//printf("KHJ, QVS_Engine::QVS_getStereoMode()=%d, nGainMode=%d\n", QVS_Engine::QVS_getStereoMode(), nGainMode);
		if (nGainMode < 0)
			m_cboAGainMode.SetCurSel(0);
		else
			m_cboAGainMode.SetCurSel(nGainMode);

		if (nGainMode >= 0)
		{
			m_bGainActive = TRUE;
			m_btnGainOnOff.SetImage(CGdipButton::ALT_TYPE);
			
			EnableGainButton(m_bGainActive);
			//#17873
			CheckRadioButton(IDC_RADIO_AGAINEFF1, IDC_RADIO_AGAINEFF2, IDC_RADIO_AGAINEFF1);

			OnToggleCurveBtn();
		}
		else
		{
			m_bGainActive = FALSE;
			m_btnGainOnOff.SetImage(CGdipButton::STD_TYPE);
			EnableGainButton(m_bGainActive);
			OnToggleCurveBtn();
		}
		

		//Nadir Setting
		int nNadirMode = QVS_Engine::QVS_GetNadirMode();
		int nNadirPos = QVS_Engine::QVS_GetNadirPosition();
		int nNadirSize = QVS_Engine::QVS_GetNadirSize();

		if (nNadirMode == 1)
		{
			m_btnNadirOnOff.SetImage(CGdipButton::STD_TYPE);
			EnableNadirButton(FALSE);
		}
		else
		{
			m_btnNadirOnOff.SetImage(CGdipButton::ALT_TYPE);
			EnableNadirButton(TRUE);
		}

		m_cboNadirPos.SetCurSel(nNadirPos - 1);

		m_sldNadirHeight.SetPos(nNadirSize);
		szTemp.Format(_T("%d"), nNadirSize);
		GetDlgItem(IDC_EDT_NADIRHEIGHT)->SetWindowText(szTemp);

		szTemp = Char2CString(QVS_Engine::QVS_GetNadirPath());
		GetDlgItem(IDC_EDT_NADIRPATH)->SetWindowText(szTemp);

		//Delay Setting
		float Adelay = QVS_Engine::QVS_SetAudioBufferOffset(0) / 192000.0f;
		float Vdelay = QVS_Engine::QVS_SetVideoBufferOffset(0) / -30.0f;
		szTemp.Format(_T("%.2f"), Adelay);
		m_sldDsAudio.SetPos(50);
		m_oldAudioPos = 50;
		GetDlgItem(IDC_EDT_DS_AUDIO)->SetWindowText(szTemp);

		szTemp.Format(_T("%.2f"), Vdelay == 0.0f ? 0.0f : Vdelay);
		GetDlgItem(IDC_EDT_DS_VIDEO)->SetWindowText(szTemp);
	}

	UpdateData(FALSE);
	if (!theApp.m_PrjAllCfg.m_bFirstUpdateBtmPano)
		theApp.m_PrjAllCfg.m_bFirstUpdateBtmPano = TRUE;
}

void CBottomPanoChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		m_btnGainOnOff.LoadImages(IDB_TOGGLE_OFF, IDB_TOGGLE_ON, 0, 0, 0, 0, _T("PNG"));
		//m_btnGainOnOff.EnableToggle(TRUE);
		m_btnGainOnOff.SizeToContent();

		m_btnNadirOnOff.LoadImages(IDB_TOGGLE_OFF, IDB_TOGGLE_ON, 0, 0, 0, 0, _T("PNG"));
		//m_btnNadirOnOff.EnableToggle(TRUE);
		m_btnNadirOnOff.SizeToContent();

		m_btnGrayOnOff.LoadImages(IDB_TOGGLE_OFF, IDB_TOGGLE_ON, 0, 0, 0, 0, _T("PNG"));
		m_btnGrayOnOff.SizeToContent();

		//m_btnColor.LoadImages(IDB_BTN_COLOR_NORMAL, 0, IDB_BTN_COLOR_MOVER, IDB_BTN_COLOR_PRESSED, 0, 0, _T("PNG"));
		//m_btnColor.SizeToContent();

		//m_btnCurves1.LoadImages(IDB_BTN_CURVES_NORMAL, 0, IDB_BTN_CURVES_MOVER, IDB_BTN_CURVES_PRESSED, 0, 0, _T("PNG"));
		//m_btnCurves1.SizeToContent();

		//m_btnCurves2.LoadImages(IDB_BTN_CURVES_DISABLE, 0, IDB_BTN_CURVES_DISABLE, IDB_BTN_CURVES_DISABLE, 0, 0, _T("PNG"));
		//m_btnCurves2.SizeToContent();

		m_btnBrow.LoadImages(IDB_BTN_OPENFILE_NORMAL, 0, 0, 0, 0, 0, _T("PNG"));
		m_btnBrow.SizeToContent();

		OnToggleCurveBtn();
	}

	if (!bShow)
	{
		OnCloseColorDlg(1, 0);
		OnCloseColorDlg(2, 0);
	}

	if (bShow && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
	{
		UpdateDialogItemValue();
	}
	else
	{
		DeleteTempShapShot();
	}
}

void CBottomPanoChildDlg::OnBnClickedBtnCurves()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	/*
	if (m_bGainActive &&  m_cboAGainMode.GetCurSel() == 3)
	{
		DoSaveTempShapShot();
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(30);
		if (m_pCurveControlDlg == NULL)
		{
			m_pCurveControlDlg = new CCurvePaneDlg;
			m_pCurveControlDlg->Create(IDD_CURVEPANE_DIALOG, this);
			
			//초기 위치 잡기
			CRect rc, rcMain, rcCopy;
			m_pCurveControlDlg->GetWindowRect(rc);
			theApp.m_pMainWnd->GetWindowRect(rcMain);
			rcCopy = rc;

			rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
			rcCopy.bottom = rcCopy.top + rc.Height();
			rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
			rcCopy.right = rcCopy.left + rc.Width();

			m_pCurveControlDlg->MoveWindow(rcCopy);
			m_pCurveControlDlg->ShowWindow(SW_SHOW);
		}
	}
	*/
}


void CBottomPanoChildDlg::OnBnClickedBtnColorset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DoSaveTempShapShot();

	if (m_pColorSetlDlg == NULL)
	{
		m_pColorSetlDlg = new CColorSettingDlg;
		m_pColorSetlDlg->Create(IDD_COLORSET_DLG, this);

		//초기 위치 잡기
		CRect rc, rcMain, rcCopy;
		m_pColorSetlDlg->GetWindowRect(rc);
		theApp.m_pMainWnd->GetWindowRect(rcMain);
		rcCopy = rc;

		rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
		rcCopy.bottom = rcCopy.top + rc.Height();
		rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
		rcCopy.right = rcCopy.left + rc.Width();

		m_pColorSetlDlg->MoveWindow(rcCopy);
		m_pColorSetlDlg->ShowWindow(SW_SHOW);
	}
}


void CBottomPanoChildDlg::OnBnClickedBtnGain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bGainActive = !m_bGainActive;
	if (!m_bGainActive)
	{
		m_btnGainOnOff.SetImage(CGdipButton::STD_TYPE);
		m_btnCurves1.EnableWindow(FALSE);
		m_btnCurves1.EnableButton(FALSE);
		if (m_pCurveControlDlg)
			m_pCurveControlDlg->ShowWindow(SW_HIDE);

		int nPrevGainSel = m_cboAGainMode.GetCurSel();
		m_cboAGainMode.SetCurSel(0);
		
		//무조건 하면 안됨.
		//if (nPrevGainSel > 0)//#17876
			QVS_Engine::QVS_SetGainMode(0);

		//#17873
		CheckRadioButton(IDC_RADIO_AGAINEFF1, IDC_RADIO_AGAINEFF2, IDC_RADIO_AGAINEFF1);
		QVS_Engine::QVS_SetAutoGainMode(0);
	}
		
	else
	{
		m_btnGainOnOff.SetImage(CGdipButton::ALT_TYPE);
		if (m_cboAGainMode.GetCurSel() == 3)
		{
			m_btnCurves1.EnableWindow(TRUE);
			m_btnCurves1.EnableButton(TRUE);
		}
		OnCbnSelchangeComboAgainmide();//#17876

		//QVS_Engine::QVS_SetAutoGainMode(m_nAGainEffect);//#17873
	}

	
	EnableGainButton(m_bGainActive);
	OnToggleCurveBtn();
}


void CBottomPanoChildDlg::OnBnClickedBtnFileBrow()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	const TCHAR szFilters[] = _T("PNG(*.png)|*.png||");

	CFileDialog dlg(TRUE, _T("PNG"), _T(""), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, NULL, 0);
	//현재 Path저장
	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);

	if (dlg.DoModal() == IDOK)
	{
		CString szFullpath, szFileName, szTemp;
		szFullpath = dlg.GetPathName();
		szFileName = dlg.GetFileName();
		GetDlgItem(IDC_EDT_NADIRPATH)->SetWindowText(szFullpath);
		
		//Path변경.
		szFullpath.Replace(szFileName, _T(""));
		//SetCurrentDirectory(szFullpath);
		
		char* szPath = CString2Char(szFullpath + szFileName);
		QVS_Engine::QVS_SetNadirPath(szPath);
		QVS_Engine::QVS_UpdateNadirImage();
		free(szPath);
		//Path원복
		SetCurrentDirectory(Path);
	}
}

void CBottomPanoChildDlg::SetRadioSeamStatus(UINT value)
{

	UpdateData(TRUE);
	int nSeam = 2;	//Dist
	if (m_nSeam == 1)
		nSeam = 1;	//Image

	if (nSeam != QVS_Engine::QVS_GetSeamMode())
	{
		GetDlgItem(IDC_RADIO_SEAM1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SEAM2)->EnableWindow(FALSE);
		
		if (!theApp.m_ScreenManager.GetseamModeDone())
			theApp.m_ScreenManager.SetseamModeStatus(FALSE);

		QVS_Engine::QVS_SetSeamMode(nSeam);

		while (!theApp.m_ScreenManager.GetseamModeDone())
			((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(30);

		theApp.m_ScreenManager.SetseamModeStatus(FALSE);

		GetDlgItem(IDC_RADIO_SEAM1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SEAM2)->EnableWindow(TRUE);
	}
		
}

void CBottomPanoChildDlg::SetRadioBlendingStatus(UINT value)
{
	UpdateData(TRUE);
	int nBlending = 2;	//Multiband

	if (m_nBlending == 0)
		nBlending = 3; // Nomal

	QVS_Engine::QVS_SetExposureCompensationMode(nBlending);
}

void CBottomPanoChildDlg::SetRadioGainStatus(UINT value)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_RADIO_AGAINEFF1)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_AGAINEFF2)->EnableWindow(FALSE);

	QVS_Engine::QVS_SetAutoGainMode(m_nAGainEffect);

	while (!(theApp.m_ScreenManager.GetGainManCurveDone() || theApp.m_ScreenManager.GetGainCompensationDone() ||
		theApp.m_ScreenManager.GetautoGainModeDone()))
			((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);


	theApp.m_ScreenManager.SetGainManCurveStatus(FALSE);
	theApp.m_ScreenManager.SetautoGainModeStatus(FALSE);

	GetDlgItem(IDC_RADIO_AGAINEFF1)->EnableWindow(TRUE);
	//GetDlgItem(IDC_RADIO_AGAINEFF2)->EnableWindow(TRUE);
	if (QVS_Engine::QVS_getStereoMode())
		GetDlgItem(IDC_RADIO_AGAINEFF2)->EnableWindow(false);
	else
		GetDlgItem(IDC_RADIO_AGAINEFF2)->EnableWindow(TRUE);
	//printf("JW SetRadioGainStatus\n");
}

void CBottomPanoChildDlg::EnableGainButton(BOOL bEnable)
{
	GetDlgItem(IDC_COMBO_AGAINMIDE)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_AGAINEFF1)->EnableWindow(bEnable);
	if (QVS_Engine::QVS_getStereoMode())
		GetDlgItem(IDC_RADIO_AGAINEFF2)->EnableWindow(false);
	else
		GetDlgItem(IDC_RADIO_AGAINEFF2)->EnableWindow(bEnable);
}

void CBottomPanoChildDlg::EnableNadirButton(BOOL bEnable)
{
	GetDlgItem(IDC_COMBO_NADIRPOS)->EnableWindow(bEnable);
	GetDlgItem(IDC_SLD_NADIRHEIGHT)->EnableWindow(bEnable);
	m_btnBrow.EnableWindow(bEnable);
	m_btnBrow.EnableButton(bEnable);
}

void CBottomPanoChildDlg::OnBnClickedBtnNadirSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nNadirMode = QVS_Engine::QVS_GetNadirMode();
	if (nNadirMode == 1)	// Off -> On
		nNadirMode = 2;
	else					// On -> Off
		nNadirMode = 1;

	if (nNadirMode == 1)
	{
		m_btnNadirOnOff.SetImage(CGdipButton::STD_TYPE);
		EnableNadirButton(FALSE);
	}
	else
	{
		m_btnNadirOnOff.SetImage(CGdipButton::ALT_TYPE);
		EnableNadirButton(TRUE);
	}

	QVS_Engine::QVS_SetNadirCompensationMode(nNadirMode);
}

void CBottomPanoChildDlg::OnCbnSelchangeComboNadirpos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nPos = m_cboNadirPos.GetCurSel();
	int nSize = m_sldNadirHeight.GetPos();
	QVS_Engine::QVS_SetNadirCompensationParam(nPos + 1, nSize);
}

void CBottomPanoChildDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
	{
		CString szTemp;
		// Nadir Scroll일때.
		if (pScrollBar == (CScrollBar*)&m_sldNadirHeight)
		{
			
			int nPos = m_cboNadirPos.GetCurSel();
			int nSize = m_sldNadirHeight.GetPos();

			szTemp.Format(_T("%d"), nSize);
			GetDlgItem(IDC_EDT_NADIRHEIGHT)->SetWindowText(szTemp);
			QVS_Engine::QVS_SetNadirCompensationParam(nPos+1, nSize);
		}
		if (pScrollBar == (CScrollBar*)&m_sldDsAudio)
		{
			int nSetVal;
			int nPos = m_sldDsAudio.GetPos();
			if (m_oldAudioPos != nPos)
			{
				int nLoop = 0;
				float Adelay;
				if (m_oldAudioPos > nPos)
				{
					nLoop = m_oldAudioPos - nPos;
					nSetVal = -1;
				}
				else
				{
					nSetVal = 1;
					nLoop = nPos - m_oldAudioPos;
				}

				m_oldAudioPos = nPos;

				for (int i = 0; i < nLoop; i++)
					Adelay = QVS_Engine::QVS_SetAudioBufferOffset(nSetVal) / 192000.0f;
				szTemp.Format(_T("%.2f"), Adelay == 0.0f ? 0.0f : Adelay);
				GetDlgItem(IDC_EDT_DS_AUDIO)->SetWindowText(szTemp);
			}
		}
		if (pScrollBar == (CScrollBar*)&m_sldDsVideo)
		{
			int nSetVal;
			int nPos = m_sldDsVideo.GetPos();
			if (m_oldVudioPos != nPos)
			{
				int nLoop = 0;
				float Vdelay;
				if (m_oldVudioPos > nPos)
				{
					nLoop = m_oldVudioPos - nPos;
					nSetVal = -1;
				}
				else
				{
					nSetVal = 1;
					nLoop = nPos - m_oldVudioPos;
				}
					
				m_oldVudioPos = nPos;
				
				for (int i = 0; i < nLoop; i++)
					Vdelay = QVS_Engine::QVS_SetVideoBufferOffset(nSetVal) / -30.0f;
				szTemp.Format(_T("%.2f"), Vdelay == 0.0f ? 0.0f : Vdelay);
				GetDlgItem(IDC_EDT_DS_VIDEO)->SetWindowText(szTemp);
			}
		}
		if (pScrollBar == (CScrollBar*)&m_sldBrightness)
		{
			int nSize = m_sldBrightness.GetPos();
			nSize -= 10;

			QVS_Engine::QVS_setBrightnessStep(nSize);

			szTemp.Format(_T("%d"), nSize);
			GetDlgItem(IDC_EDT_BRIGHTNESS)->SetWindowText(szTemp);
		}
		if (pScrollBar == (CScrollBar*)&m_sldContrast)
		{
			int nSize = m_sldContrast.GetPos();

			QVS_Engine::QVS_setContrastStep(nSize);

			szTemp.Format(_T("%d"), nSize);
			GetDlgItem(IDC_EDT_CONTRAST)->SetWindowText(szTemp);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBottomPanoChildDlg::OnCbnSelchangeComboAgainmide()
{
	//Gain Brightness;Gain Color;Curve Color; 중 Curve Color만 사용


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nPos = m_cboAGainMode.GetCurSel();
	if (nPos != 3)
	{
		m_btnCurves1.EnableWindow(FALSE);
		m_btnCurves1.EnableButton(FALSE);
		if (m_pCurveControlDlg)
			m_pCurveControlDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		//m_btnCurves1.EnableWindow(TRUE);
		//m_btnCurves1.EnableButton(TRUE);
	}

	m_cboAGainMode.EnableWindow(FALSE);
	
	//QVS_Engine::QVS_SetGainMode(nPos);
	//QVS_Engine::QVS_SetGainMode(nPos+1);//#17876, Gain Brightness, Gain Color, Curve Color 순으로 3개 사용할 때
	QVS_Engine::QVS_SetGainMode(nPos + 3);//#171212, Curve Color만 사용할 때
	//printf("JW nPos = %d\n" , nPos);
	while (!(theApp.m_ScreenManager.GetGainManCurveDone() || theApp.m_ScreenManager.GetGainCompensationDone()||
		theApp.m_ScreenManager.GetautoGainModeDone()))
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);
	
	theApp.m_ScreenManager.SetGainCompensationStatus(FALSE);

	m_cboAGainMode.EnableWindow(TRUE);
	OnToggleCurveBtn();
}


void CBottomPanoChildDlg::DoSaveTempShapShot()
{
	//Temp Folder에 SnapShot을 저장한다.
	CFileFind	aFileFind;
	BOOL		bWorking;

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

	CString csPrefix(_T("")), csToken(_T(""));
	int nStart = 0, nEnd;
	while ((nEnd = szTempPath.Find('\\', nStart)) >= 0)
	{
		CString csToken = szTempPath.Mid(nStart, nEnd - nStart);
		CreateDirectory(csPrefix + csToken, NULL);

		csPrefix += csToken;
		csPrefix += _T("\\");
		nStart = nEnd + 1;
	}
	csToken = szTempPath.Mid(nStart);
	CreateDirectory(csPrefix + csToken, NULL);

	strFilePath.Format(_T("%s%s"), szTempPath, _T("snapshot_0.jpg"));

	bWorking = aFileFind.FindFile(strFilePath);

	if (!bWorking)
	{
		CString szFileName;
		CRect rcThumb = { 0, 0, 120, 80 };
		char*szShapShot = QVS_Engine::QVS_GetSnapshot_path();
		CString szOldPath = Char2CString(szShapShot);

		char* szPath = CString2Char(szTempPath);
		QVS_Engine::QVS_SetSnapshotPath(szPath);
		free(szPath);

		BOOL bCheckDeviceCon = FALSE;
		char temp[256] = { 0, };
		int nDeviceCnt = 0;
		
		for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
		{
			int nDevicIdx = QVS_Engine::QVS_getCaptureDeviceIndex(i);
			sprintf(temp, "%s", QVS_Engine::QVS_getCaptureDeviceName(i));
#ifdef _DEBUG
			TRACE(_T("SnapShot####  [%d]%s[Device idx: %d]\n"), i, CString(temp), nDevicIdx);
#endif
			if (nDevicIdx != -1)
			{
				nDeviceCnt++;			//실제 Device가 
				bCheckDeviceCon = TRUE;
			}
		}

		theApp.m_ScreenManager.SetSnapShotDoneCnt(nDeviceCnt);

		int result = QVS_Engine::QVS_SnapShot();

		if (result && bCheckDeviceCon)
		{
			while (!theApp.m_ScreenManager.GetSnapShotDone()) 
				((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);
			
			WCHAR Path[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, Path);

			//SetCurrentDirectory(szTempPath);
			for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
			{
				szFileName.Format(_T("snapshot_%d.jpg"), i);
				MakeThumbFrameImage(szFileName, szFileName, rcThumb, Gdiplus::ImageFormatJPEG);
			}
			theApp.m_ScreenManager.SetSnapShotStatus(FALSE);
			
			SetCurrentDirectory(Path);

		}
		char* szpPath = CString2Char(szOldPath);
		QVS_Engine::QVS_SetSnapshotPath(szpPath);
		free(szpPath);
	}
}

//--------------------------------------------------------------------------------------------
// 원본이미지를 특정크기로 바꾼다.
//--------------------------------------------------------------------------------------------
BOOL CBottomPanoChildDlg::MakeThumbFrameImage(LPCTSTR InSourceImageFileName, LPCTSTR InDestImageFileName,
	const CRect& InTagetRect, REFGUID InImageFormatToMake)
{
	CImage    TempSourceImage;
	CImage    TempDestImage;

	CRect    TempTargetRect = InTagetRect;
	//-------------------------------------------------
	// 1. 원본 이미지를 TempSourceImage에 로딩
	//-------------------------------------------------
	HRESULT hResult = TempSourceImage.Load(InSourceImageFileName);
	HBITMAP hBmp = TempSourceImage;
	if (hBmp)
	{
		CDC* pSourceDC = CDC::FromHandle(TempSourceImage.GetDC());    // 원본 이미지의 DC를 얻는다

		//-------------------------------------------------
		// 2. 파일로 만들 이미지 객체 TempDestImage 생성
		//-------------------------------------------------
		int BitPerPixel = pSourceDC->GetDeviceCaps(BITSPIXEL);
		TempDestImage.Create(TempTargetRect.Width(), TempTargetRect.Height(), BitPerPixel);
		CDC* pDestDC = CDC::FromHandle(TempDestImage.GetDC());

		if (!pSourceDC || !pDestDC)
		{
			return FALSE;
		}
		//---------------------------------------------------------------------
		// 3. 타겟 DC( 만들 이미지의 DC )에 원본 이미지 DC 의 내용을 를 쓴다 
		//--------------------------------------------------------------------
		pDestDC->SetStretchBltMode(COLORONCOLOR);
		pDestDC->StretchBlt(0, 0, TempTargetRect.Width(), TempTargetRect.Height(),
			pSourceDC, 0, 0, TempSourceImage.GetWidth(), TempSourceImage.GetHeight(), SRCCOPY);

		//--------------------------------------------------------
		// 4. 이미지 파일로 저장
		//-------------------------------------------------------
		TempDestImage.Save(InDestImageFileName, InImageFormatToMake);

		TempDestImage.ReleaseDC();
		TempSourceImage.ReleaseDC();
		return TRUE;
	}

	return FALSE;
}

void CBottomPanoChildDlg::DeleteTempShapShot()
{
	//Temp Folder에 SnapShot을 삭제한다.
	CFileFind	aFileFind;
	BOOL		bWorking;

	CString temp, szTempPath, strFilePath;
	// Temp폴더를 다른곳에?? 나중에 생각 해보자.
	//GetTempPath(_MAX_PATH, temp.GetBuffer(MAX_PATH));
	temp = theApp.GetAppDir();	//임시 폴더 변경...프로그램...디렉토리하위로..
	szTempPath.Format(_T("%s"), temp);
	szTempPath.Format(_T("%s"), szTempPath.Right(1));
	if (szTempPath.CompareNoCase(_T("\\")) != 0)
		strFilePath.Format(_T("%s%s"), temp, _T("\\_Q360_\\*.jpg"));
	else
		strFilePath.Format(_T("%s%s"), temp, _T("_Q360_\\*.jpg"));

	bWorking = aFileFind.FindFile(strFilePath);
	while (bWorking)
	{
		bWorking = aFileFind.FindNextFile();
		strFilePath = aFileFind.GetFilePath();

		if (!aFileFind.IsDirectory())
		{
			CString strFilePath = aFileFind.GetFilePath();

			if (strFilePath.GetLength() > 10)
				::DeleteFile((LPCTSTR)strFilePath);
		}
	}
}

BOOL CBottomPanoChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CBottomPanoChildDlg::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	CFont* ariaB12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria12);
	GetDlgItem(IDC_STATIC_12_1)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_2)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_3)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_4)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_5)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_6)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_7)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_8)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_9)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_10)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_12)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_13)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_14)->SetFont(aria12);

	GetDlgItem(IDC_STATIC_VIDEOSET)->SetFont(ariaB12);
	GetDlgItem(IDC_STATIC_GAINSET)->SetFont(ariaB12);
	GetDlgItem(IDC_STATIC_COLORSET)->SetFont(ariaB12);
	GetDlgItem(IDC_STATIC_FILTERSET)->SetFont(ariaB12);
	GetDlgItem(IDC_STATIC_NADIRSET)->SetFont(ariaB12);
	GetDlgItem(IDC_STATIC_DELAYSET)->SetFont(ariaB12);

	GetDlgItem(IDC_RADIO_SEAM1)->SetFont(aria12);
	GetDlgItem(IDC_RADIO_SEAM2)->SetFont(aria12);
	GetDlgItem(IDC_RADIO_BLENDING1)->SetFont(aria12);
	GetDlgItem(IDC_RADIO_BLENDING2)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_AGAINMIDE)->SetFont(aria12);
	GetDlgItem(IDC_RADIO_AGAINEFF1)->SetFont(aria12);
	GetDlgItem(IDC_RADIO_AGAINEFF2)->SetFont(aria12);
	GetDlgItem(IDC_EDT_BRIGHTNESS)->SetFont(aria12);
	GetDlgItem(IDC_EDT_CONTRAST)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_NADIRPOS)->SetFont(aria12);
	GetDlgItem(IDC_EDT_NADIRHEIGHT)->SetFont(aria12);
	GetDlgItem(IDC_EDT_NADIRPATH)->SetFont(aria12);
	GetDlgItem(IDC_EDT_DS_AUDIO)->SetFont(aria12);
	GetDlgItem(IDC_EDT_DS_VIDEO)->SetFont(aria12);
}

void CBottomPanoChildDlg::OnToggleCurveBtn()
{
	int nPos = m_cboAGainMode.GetCurSel();

	if (nPos == 3 && m_bGainActive == TRUE)
	{
		m_btnCurves1.ShowWindow(TRUE);
		m_btnCurves2.ShowWindow(FALSE);
	}
	else
	{
		m_btnCurves1.ShowWindow(FALSE);
		m_btnCurves2.ShowWindow(TRUE);
		if (m_pCurveControlDlg)
			m_pCurveControlDlg->ClickButtons(-1);
	}

}

void CBottomPanoChildDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnCloseColorDlg(1, 0);
	OnCloseColorDlg(2, 0);
}

LRESULT CBottomPanoChildDlg::OnCloseColorDlg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		if (m_pCurveControlDlg && m_pCurveControlDlg->GetSafeHwnd())
		{
			m_pCurveControlDlg->ShowWindow(SW_HIDE);
			m_pCurveControlDlg->DestroyWindow();
			delete m_pCurveControlDlg;
			m_pCurveControlDlg = NULL;
			DeleteTempShapShot();
		}
	}
	if (wParam == 2)
	{
		if (m_pColorSetlDlg && m_pColorSetlDlg->GetSafeHwnd())
		{
			m_pColorSetlDlg->ShowWindow(SW_HIDE);
			m_pColorSetlDlg->DestroyWindow();
			delete m_pColorSetlDlg;
			m_pColorSetlDlg = NULL;
			DeleteTempShapShot();
		}
	}

	return 0;
}



void CBottomPanoChildDlg::OnBnClickedBtnGrayscaleSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (setGrayscale == GRAYSCALE_ON)
	{
		setGrayscale = GRAYSCALE_OFF;
		QVS_Engine::QVS_setGrayscaleMode(setGrayscale);
		m_btnGrayOnOff.SetImage(CGdipButton::STD_TYPE);
	}

	else
	{
		setGrayscale = GRAYSCALE_ON;
		QVS_Engine::QVS_setGrayscaleMode(setGrayscale);
		m_btnGrayOnOff.SetImage(CGdipButton::ALT_TYPE);
	}
}
