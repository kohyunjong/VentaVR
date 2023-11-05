// BottonSourceChildDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "BottomSourceChildDlg.h"
#include "afxdialogex.h"


// CBottomSourceChildDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CBottomSourceChildDlg, CDialogEx)

CBottomSourceChildDlg::CBottomSourceChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBottomSourceChildDlg::IDD, pParent)
{
	m_dlgPreviewDlg = NULL;
	m_bisShowPanoPreview = FALSE;
	m_bisCapConf = FALSE;
}

CBottomSourceChildDlg::~CBottomSourceChildDlg()
{
	OnClosePreviewDlg(0, 0);
}

void CBottomSourceChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BTMTOPBAR, m_ctlTopBar);
	DDX_Control(pDX, IDC_BTN_CAPCONF, m_btnCaptureConfig);
	DDX_Control(pDX, IDC_BTN_PREVIEPANO, m_btnPanoPrevie);
	DDX_Control(pDX, IDC_COMBO_AUDIOINPUT, m_cbpAudioInput);
	DDX_Control(pDX, IDC_COMBO_LENSTYPE, m_cbpLensType);
	DDX_Control(pDX, IDC_BTN_EDITINPUT, m_btnCameraSel);
	DDX_Control(pDX, IDC_BTNCIP_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_EDT_CIP_LENSTYPE, m_editLenstype);
}


BEGIN_MESSAGE_MAP(CBottomSourceChildDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_PREVIEPANO, &CBottomSourceChildDlg::OnBnClickedBtnPreviepano)
	ON_BN_CLICKED(IDC_BTN_CAPCONF, &CBottomSourceChildDlg::OnBnClickedBtnCapconf)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIOINPUT, &CBottomSourceChildDlg::OnCbnSelchangeComboAudioinput)
	ON_CBN_DROPDOWN(IDC_COMBO_AUDIOINPUT, &CBottomSourceChildDlg::OnCbnDropdownComboAudioinput)
	ON_BN_CLICKED(IDC_BTN_EDITINPUT, &CBottomSourceChildDlg::OnBnClickedBtnEditinput)
	ON_BN_CLICKED(IDC_BTNCIP_EDIT, &CBottomSourceChildDlg::OnBnClickedBtncipEdit)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CLOSECOLORDLG, OnClosePreviewDlg)
END_MESSAGE_MAP()


// CBottomSourceChildDlg �޽��� ó�����Դϴ�.


void CBottomSourceChildDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc;
	GetParent()->GetClientRect(rc); // Docking pane Size

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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


BOOL CBottomSourceChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_cbpAudioInput.SetCurSel(0);
	m_cbpLensType.SetCurSel(0);
	SetFontObj();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CBottomSourceChildDlg::UpdateDialogItemValue()
{
	printf("bigheadk, UpdateDialogItemValue\n");
	//����̽��� Init�ɶ����� ��ٸ���.
	if (!((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->m_init)
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);

	if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
	{
		CString szTemp;
		szTemp.Format(_T("%.1f"), QVS_Engine::QVS_GetCapureFPS());
		GetDlgItem(IDC_EDT_CIP_FPS)->SetWindowText(szTemp);

		szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_nWidth);
		GetDlgItem(IDC_EDT_CIP_WIDTH)->SetWindowText(szTemp);

		szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_nHeight);
		GetDlgItem(IDC_EDT_CIP_HEIGHT)->SetWindowText(szTemp);

		theApp.m_PrjAllCfg.m_nFov = QVS_Engine::QVS_getFOV();
		szTemp.Format(_T("%.1f"), theApp.m_PrjAllCfg.m_nFov);
		GetDlgItem(IDC_EDT_CIP_FOV)->SetWindowText(szTemp);

		//Audio Device..Setting.
		int nsize = QVS_Engine::QVS_getAudioCaptureDeviceSize();
		if (nsize > 0)
		{
			m_cbpAudioInput.ResetContent();
			m_cbpAudioInput.AddString(_T("No Device"));
			for (int j = 0; j < QVS_Engine::QVS_getAudioCaptureDeviceSize(); j++) {
				m_cbpAudioInput.AddString(CString(QVS_Engine::QVS_getAudioCaptureDeviceName(j)));
			}
			int idx = QVS_Engine::QVS_GetAudioDevice();
			if (idx == -1)
			{
				QVS_Engine::QVS_destroyAudioDevice();
				QVS_Engine::QVS_SetAudioDevice(-1);
				m_cbpAudioInput.SetCurSel(0);
			}
			else
				m_cbpAudioInput.SetCurSel(idx+1);
			m_cbpAudioInput.EnableWindow(TRUE);
		}
		else
			m_cbpAudioInput.SetCurSel(0);

		//Lens Type Setting.
		/*nsize = QVS_Engine::QVS_getLensTypeCount();
		if (nsize > 1)
		{
			m_cbpLensType.ResetContent();
			for (int j = 0; j < nsize; j++) {
				m_cbpLensType.AddString(CString(QVS_Engine::QVS_getLensTypeName(j)));
			}
			int idx = QVS_Engine::QVS_getLensType();
			m_cbpLensType.SetCurSel(idx);
			m_cbpLensType.EnableWindow(TRUE);
		}
		else
			m_cbpLensType.SetCurSel(0);*/

		SetDlgItemTextW(IDC_EDT_CIP_LENSTYPE, setLensType());
	}
	theApp.m_PrjAllCfg.m_bFirstUpdateBtmSource = TRUE;
}

CString CBottomSourceChildDlg::setLensType()
{
	int type = QVS_Engine::QVS_getLensType();
	CString szTemp = NULL;

	switch (type)
	{
	case RECTLINEAR_TO_EQUIRECT:
		szTemp = _T("Rectlinear");
		break;
	case CYLINDRICAL_PANORAMA_TO_EQUIRECT:
		szTemp = _T("Cylindrical Panorama");
		break;
	case CIRCULAR_FISH_TO_EQUIRECT:
		szTemp = _T("Circular Fisheye");
		break;
	case FULL_FRAME_FISHEYE_TO_EQUIRECT:
		szTemp = _T("FullFrame Fisheye");
		break;
	}

	return szTemp;
}

void CBottomSourceChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow)
	{

		m_btnEdit.LoadImages(IDB_BTN_EDIT_NORMAL, IDB_BTN_EDIT_SELECTED, IDB_BTN_EDIT_MOVER, IDB_BTN_EDIT_PRESSED, IDB_BTN_EDIT_SELECTED_MOVER, IDB_BTN_EDIT_SELECTED_PRESSED, _T("PNG"));
		m_btnEdit.SizeToContent();
		m_btnEdit.SetImage(CGdipButton::STD_TYPE);

		m_btnCameraSel.LoadImages(IDB_BTN_CAMERA_SEL_NORMAL, IDB_BTN_CAMERA_SEL_SELECTED, IDB_BTN_CAMERA_SEL_MOVER, IDB_BTN_CAMERA_SEL_PRESSED, IDB_BTN_CAMERA_SEL_SELECTED, IDB_BTN_CAMERA_SEL_SELECTED, _T("PNG"));
		m_btnCameraSel.SizeToContent();
		m_btnCameraSel.SetImage(CGdipButton::STD_TYPE);

		m_btnCaptureConfig.LoadImages(IDB_TOGGLE_OFF, IDB_TOGGLE_ON, 0, 0, 0, 0, _T("PNG"));
		m_btnCaptureConfig.SizeToContent();

		//m_btnPanoPrevie.LoadImages(IDB_TOGGLE_OFF, IDB_TOGGLE_ON, 0, 0, 0, 0, _T("PNG"));
		m_btnPanoPrevie.SizeToContent();

		if (m_bisShowPanoPreview)
		{
			OnClosePreviewDlg(1, 0);
		}

		/*if (theApp.m_ScreenManager.isNewProject)
		{
			theApp.m_ScreenManager.isNewProject = false;
			isCameraSel = true;

			if ((CCenterView*)theApp.m_ScreenManager.m_pCenterView)
				((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowEditInput(TRUE);

			m_btnCameraSel.SetImage(CGdipButton::ALT_TYPE);
		}*/

		if(!theApp.m_PrjAllCfg.m_bFirstUpdateBtmSource)
			UpdateDialogItemValue();
	}
	else if (!bShow)
	{
		OnClosePreviewDlg(0, 0);
		if (theApp.m_ScreenManager.m_pCenterView)
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->showEditInputDlg(FALSE);
		if (theApp.m_ScreenManager.m_pCenterView)
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowEditCameraParam(FALSE);

		if (m_bisCapConf)
		{
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->AcitveCapConf(FALSE);
			m_btnCaptureConfig.SetImage(CGdipButton::STD_TYPE);
			m_bisCapConf = FALSE;
		}

		isCParamEdit = false;
		isCameraSel = false;
	}
}

void CBottomSourceChildDlg::CameraSelBtnState(bool btnState)
{
	if (btnState)
		m_btnCameraSel.SetImage(CGdipButton::ALT_TYPE);
	else
		m_btnCameraSel.SetImage(CGdipButton::STD_TYPE);
}

void CBottomSourceChildDlg::OnBnClickedBtnPreviepano()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bisShowPanoPreview = !m_bisShowPanoPreview;
	if (m_bisShowPanoPreview)
	{
		OnClosePreviewDlg(1, 0);
		m_btnPanoPrevie.SetImage(CGdipButton::ALT_TYPE);
	}
	else
	{
		OnClosePreviewDlg(0, 0);
		
		m_btnPanoPrevie.SetImage(CGdipButton::STD_TYPE);
	}
}


void CBottomSourceChildDlg::OnBnClickedBtnCapconf()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nDeviceCnt = 0;
	char temp[256] = { 0, };
	for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
	{
		int nDevicIdx = QVS_Engine::QVS_getCaptureDeviceIndex(i);
		sprintf(temp, "%s", QVS_Engine::QVS_getCaptureDeviceName(i));
#ifdef _DEBUG
		TRACE(_T("SnapShot####  [%d]%s[Device idx: %d]\n"), i, CString(temp), nDevicIdx);
#endif
		if (nDevicIdx != -1)
		{
			nDeviceCnt++;			//���� Device�� 
		}
	}

	if (nDeviceCnt > 0)
	{
		m_bisCapConf = !m_bisCapConf;
		if (m_bisCapConf)
		{
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->AcitveCapConf(TRUE);
			m_btnCaptureConfig.SetImage(CGdipButton::ALT_TYPE);
			QVS_Engine::QVS_setEditInputMode(QVS_Engine::EditInputMode_On);
		}
		else
		{
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->AcitveCapConf(FALSE);
			m_btnCaptureConfig.SetImage(CGdipButton::STD_TYPE);
			QVS_Engine::QVS_setEditInputMode(QVS_Engine::EditInputMode_Off);
		}
	}
	else
	{
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->AcitveCapConf(FALSE);
		m_btnCaptureConfig.SetImage(CGdipButton::STD_TYPE);
	}
}


BOOL CBottomSourceChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CBottomSourceChildDlg::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	CFont* ariaB12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria12);
	GetDlgItem(IDC_STATIC_12_11)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_12)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_13)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_14)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_15)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_48)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_49)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_50)->SetFont(aria12);


	GetDlgItem(IDC_STATIC_B12_1)->SetFont(ariaB12);
	GetDlgItem(IDC_STATIC_B12_2)->SetFont(ariaB12);
	GetDlgItem(IDC_STATIC_B12_3)->SetFont(ariaB12);


	GetDlgItem(IDC_EDT_CIP_FPS)->SetFont(aria12);
	GetDlgItem(IDC_EDT_CIP_WIDTH)->SetFont(aria12);
	GetDlgItem(IDC_EDT_CIP_HEIGHT)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_AUDIOINPUT)->SetFont(aria12);
	GetDlgItem(IDC_EDT_CIP_FOV)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_LENSTYPE)->SetFont(aria12);
}

void CBottomSourceChildDlg::OnCbnSelchangeComboAudioinput()
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	QVS_Engine::QVS_destroyAudioDevice();
	if (m_cbpAudioInput.GetCurSel() != 0)
		QVS_Engine::QVS_SetAudioDevice(m_cbpAudioInput.GetCurSel()-1);
}

void CBottomSourceChildDlg::OnCbnDropdownComboAudioinput()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	QVS_Engine::QVS_AudioCaptureUpdate();
	//UpdateDialogItemValue();
	m_cbpAudioInput.ResetContent();
	m_cbpAudioInput.AddString(_T("No Device"));
	for (int j = 0; j < QVS_Engine::QVS_getAudioCaptureDeviceSize(); j++) {
		m_cbpAudioInput.AddString(CString(QVS_Engine::QVS_getAudioCaptureDeviceName(j)));
	}
	int idx = QVS_Engine::QVS_GetAudioDevice();
	m_cbpAudioInput.SetCurSel(idx + 1);
}

void CBottomSourceChildDlg::OnBnClickedBtnEditinput()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!isCParamEdit)
	{
		isCameraSel = true;

		if ((CCenterView*)theApp.m_ScreenManager.m_pCenterView)
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->showEditInputDlg(TRUE);

		m_btnCameraSel.SetImage(CGdipButton::ALT_TYPE);
	}
}


void CBottomSourceChildDlg::OnBnClickedBtncipEdit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//CString szTemp, szTemp_fov;
	//GetDlgItem(IDC_EDT_CIP_FPS)->GetWindowText(szTemp);
	//GetDlgItem(IDC_EDT_CIP_FOV)->GetWindowText(szTemp_fov);
	////QVS_Engine::QVS_SetInputConfiguration(_ttof(szTemp), theApp.m_PrjAllCfg.m_nWidth, theApp.m_PrjAllCfg.m_nHeight);
	//QVS_Engine::QVS_SetInputConfiguration(_ttof(szTemp), theApp.m_PrjAllCfg.m_nWidth, theApp.m_PrjAllCfg.m_nHeight, m_cbpLensType.GetCurSel(), _ttof(szTemp_fov));

	if (!isCameraSel)
	{
		isCParamEdit = true;

		m_btnEdit.SetImage(CGdipButton::ALT_TYPE);

		if ((CCenterView*)theApp.m_ScreenManager.m_pCenterView)
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowEditCameraParam(TRUE);
	}
}


void CBottomSourceChildDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	OnClosePreviewDlg(0, 0);
}


LRESULT CBottomSourceChildDlg::OnClosePreviewDlg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		if (m_dlgPreviewDlg)
		{
			m_dlgPreviewDlg->DestroyWindow();
			delete m_dlgPreviewDlg;
			m_dlgPreviewDlg = NULL;
		}
	}
	else
	{
		if (m_dlgPreviewDlg == NULL)
		{
			m_dlgPreviewDlg = new CPanoPreViewDlg;
			m_dlgPreviewDlg->Create(IDD_PANORAMAPREVIEW_DLG, this);
			m_dlgPreviewDlg->ShowWindow(SW_SHOW);

			CRect rc, rcMain, rcCopy;
			m_dlgPreviewDlg->GetWindowRect(rc);
			theApp.m_pMainWnd->GetWindowRect(rcMain);
			rcCopy = rc;

			rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
			rcCopy.bottom = rcCopy.top + rc.Height();
			rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
			rcCopy.right = rcCopy.left + rc.Width();

			m_dlgPreviewDlg->MoveWindow(rcCopy);
		}
	}

	return 0;
}
