#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightCalibChildDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CRightCalibChildDlg, CDialogEx)

CRightCalibChildDlg::CRightCalibChildDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CRightCalibChildDlg::IDD, pParent)
{

}

CRightCalibChildDlg::~CRightCalibChildDlg()
{
}

void CRightCalibChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNALIGN, m_btnAlign);
	DDX_Control(pDX, IDC_BTN_MANUAL_MASK, m_btnManualMask);
	DDX_Control(pDX, IDC_BTNDETAILCALIBRATION, m_btnDetailUpdate);
	DDX_Control(pDX, IDC_BTNADVANCE, m_btnAdvance);
	DDX_Control(pDX, IDC_BTNBACK, m_btnBack);
	DDX_Control(pDX, IDC_BTNUNDOCALIBRATION, m_btnUndo);
	DDX_Control(pDX, IDC_BTNREDOCALIBRATION, m_btnRedo);
	DDX_Control(pDX, IDC_BTNEDITOFF, m_btnEditOff);
	DDX_Control(pDX, IDC_BTNEDITON, m_btnEditOn);
	DDX_Control(pDX, IDC_BTNSTRAIGHTEN, m_btnStraighten);
	DDX_Control(pDX, IDC_BTNSMARTCALIBRATION, m_btnSmartCalibration);
	DDX_Control(pDX, IDC_BTN_MANUAL_ROTATE, m_btnManualRotate);
}

BEGIN_MESSAGE_MAP(CRightCalibChildDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNALIGN, &CRightCalibChildDlg::OnBnClickedBtnaligncalibration)
	ON_BN_CLICKED(IDC_BTN_MANUAL_MASK, &CRightCalibChildDlg::OnBnClickedBtnManualMask)
	ON_BN_CLICKED(IDC_BTNDETAILCALIBRATION, &CRightCalibChildDlg::OnBnClickedBtndetailcalibration)
	ON_BN_CLICKED(IDC_BTNADVANCE, &CRightCalibChildDlg::OnBnClickedBtnadvance)
	ON_BN_CLICKED(IDC_BTNBACK, &CRightCalibChildDlg::OnBnClickedBtnback)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTNEDITON, &CRightCalibChildDlg::OnBnClickedBtnediton)
	ON_BN_CLICKED(IDC_BTNEDITOFF, &CRightCalibChildDlg::OnBnClickedBtneditoff)
	ON_BN_CLICKED(IDC_BTNUNDOCALIBRATION, &CRightCalibChildDlg::OnBnClickedBtnundocalibration)
	ON_BN_CLICKED(IDC_BTNREDOCALIBRATION, &CRightCalibChildDlg::OnBnClickedBtnredocalibration)
	ON_BN_CLICKED(IDC_BTNSTRAIGHTEN, &CRightCalibChildDlg::OnBnClickedBtnstraighten)
	
	ON_BN_CLICKED(IDC_BTNSMARTCALIBRATION, &CRightCalibChildDlg::OnBnClickedBtnsmartcalibration)
	ON_BN_CLICKED(IDC_BTN_MANUAL_ROTATE, &CRightCalibChildDlg::OnBnClickedBtnManualRotate)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CRightCalibChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRightCalibChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	bool stereoMode = QVS_Engine::QVS_getStereoMode();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow) 
	{
		/*if (!stereoMode)
			m_btnAlign.LoadImages(IDB_EXPORT_BTN_ALIGN_NORMAL, 0, IDB_EXPORT_BTN_ALIGN_MOVER, IDB_EXPORT_BTN_ALIGN_PRESSED, 0, 0, _T("PNG"));
		else
			m_btnAlign.LoadImages(IDB_EXPORT_BTN_AUTOCROP_NORMAL, 0, IDB_EXPORT_BTN_AUTOCROP_MOVER, IDB_EXPORT_BTN_AUTOCROP_PRESSED, 0, 0, _T("PNG"));*/
		m_btnAlign.LoadImages(IDB_EXPORT_BTN_ALIGN_NORMAL, IDB_EXPORT_BTN_AUTOCROP_NORMAL, IDB_EXPORT_BTN_ALIGN_MOVER, IDB_EXPORT_BTN_ALIGN_PRESSED, IDB_EXPORT_BTN_AUTOCROP_MOVER, IDB_EXPORT_BTN_AUTOCROP_PRESSED, _T("PNG"));
		m_btnAlign.SizeToContent();
		
		m_btnBack.LoadImages(IDB_EXPORT_BTN_BACK_NORMAL, 0, IDB_EXPORT_BTN_BACK_MOVER, IDB_EXPORT_BTN_BACK_PRESSED, 0, 0, _T("PNG"));
		m_btnBack.SizeToContent();

		m_btnManualMask.LoadImages(IDB_EXPORT_BTN_MANUALMASK_NORMAL, 0, IDB_EXPORT_BTN_MANUALMASK_MOVER, IDB_EXPORT_BTN_MANUALMASK_PRESSED, 0, 0, _T("PNG"));
		m_btnManualMask.SizeToContent();

		m_btnDetailUpdate.LoadImages(IDB_EXPORT_BTN_DETAIL_NORMAL, 0, IDB_EXPORT_BTN_DETAIL_MOVER, IDB_EXPORT_BTN_DETAIL_PRESSED, 0, 0, _T("PNG"));
		m_btnDetailUpdate.SizeToContent();

		m_btnSmartCalibration.LoadImages(IDB_EXPORT_BTN_SMARTCALIB_NORMAL, 0, IDB_EXPORT_BTN_SMARTCALIB_MOVER, IDB_EXPORT_BTN_SMARTCALIB_PRESSED, 0, 0, _T("PNG"));
		m_btnSmartCalibration.SizeToContent();

		m_btnAdvance.LoadImages(IDB_EXPORT_BTN_ADVANCE_NORMAL, 0, IDB_EXPORT_BTN_ADVANCE_MOVER, IDB_EXPORT_BTN_ADVANCE_PRESSED, 0, 0, _T("PNG"));
		m_btnAdvance.SizeToContent();

		m_btnUndo.LoadImages(IDB_EXPORT_BTN_UNDO_NORMAL, 0, IDB_EXPORT_BTN_UNDO_MOVER, IDB_EXPORT_BTN_UNDO_PRESSED, 0, 0, _T("PNG"));
		m_btnUndo.SizeToContent();

		m_btnRedo.LoadImages(IDB_EXPORT_BTN_REDO_NORMAL, 0, IDB_EXPORT_BTN_REDO_MOVER, IDB_EXPORT_BTN_REDO_PRESSED, 0, 0, _T("PNG"));
		m_btnRedo.SizeToContent();

		m_btnEditOff.LoadImages(IDB_EXPORT_BTN_ROTATE_NORMAL, 0, IDB_EXPORT_BTN_ROTATE_MOVER, IDB_EXPORT_BTN_ROTATE_PRESSED, 0, 0, _T("PNG"));
		m_btnEditOff.SizeToContent();

		m_btnEditOn.LoadImages(IDB_EXPORT_BTN_ROTATE_SELECTED, 0, IDB_EXPORT_BTN_ROTATE_SELECTED_MOVER, IDB_EXPORT_BTN_ROTATE_SELECTED_PRESSED, 0, 0, _T("PNG"));
		m_btnEditOn.SizeToContent();
		m_btnEditOn.ShowWindow(SW_HIDE);

		m_btnStraighten.LoadImages(IDB_EXPORT_BTN_STRAIGHTEN_NORMAL, 0, IDB_EXPORT_BTN_STRAIGHTEN_MOVER, IDB_EXPORT_BTN_STRAIGHTEN_PRESSED, 0, 0, _T("PNG"));
		m_btnStraighten.SizeToContent();
		m_btnStraighten.ShowWindow(SW_HIDE);

		m_btnManualRotate.LoadImages(IDB_EXPORT_BTN_CAMERAPARAM_NORMAL, 0, IDB_EXPORT_BTN_CAMERAPARAM_MOVER, IDB_EXPORT_BTN_CAMERAPARAM_PRESSED, 0, 0, _T("PNG"));
		m_btnManualRotate.SizeToContent();
		m_btnManualRotate.ShowWindow(SW_HIDE);

		if (!stereoMode) 
		{
			m_btnManualMask.ShowWindow(SW_SHOW);
			m_btnDetailUpdate.ShowWindow(SW_SHOW);
			m_btnSmartCalibration.ShowWindow(SW_SHOW);
			m_btnAdvance.ShowWindow(SW_SHOW);
			m_btnUndo.ShowWindow(SW_SHOW);
			m_btnRedo.ShowWindow(SW_SHOW);
			m_btnEditOff.ShowWindow(SW_SHOW);
			m_btnManualRotate.ShowWindow(SW_HIDE);

			if (QVS_Engine::QVS_undoEnabled())
				m_btnUndo.EnableWindow(true);
			else
				m_btnUndo.EnableWindow(false);
			if (QVS_Engine::QVS_redoEnabled())
				m_btnRedo.EnableWindow(true);
			else
				m_btnRedo.EnableWindow(false);
		}
		else
		{
			m_btnManualMask.ShowWindow(SW_HIDE);
			m_btnDetailUpdate.ShowWindow(SW_HIDE);
			m_btnSmartCalibration.ShowWindow(SW_HIDE);
			m_btnAdvance.ShowWindow(SW_HIDE);
			m_btnUndo.ShowWindow(SW_HIDE);
			m_btnRedo.ShowWindow(SW_HIDE);
			m_btnEditOff.ShowWindow(SW_HIDE);
			m_btnManualRotate.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		CCenterView* m_pCenter = (CCenterView*)theApp.m_ScreenManager.m_pCenterView;
		if (m_pCenter != NULL)
			m_pCenter->ShowManualRotateDlg(FALSE);
	}
}

void CRightCalibChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//if (nIDEvent == TIMER_SNAPSHOTDONE)
	//{
	//	KillTimer(TIMER_SNAPSHOTDONE);
	//	theApp.m_ScreenManager.ShowToest(vdstToastDone, _T("Done"), FALSE);
	//	theApp.m_ScreenManager.SetSnapShotStatus(FALSE);
	//	m_btnSnapshot.EnableButton(TRUE);
	//}

	CDialogEx::OnTimer(nIDEvent);
}

void CRightCalibChildDlg::OnBnClickedBtnaligncalibration()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	bool stereoMode = QVS_Engine::QVS_getStereoMode();
	if (stereoMode)
		QVS_Engine::QVS_stereoStitching();
	else
		QVS_Engine::QVS_SetQuramCalibMode(QVS_Engine::CalibrationMode_MakeTemplate);
}

void CRightCalibChildDlg::refresh(bool state)
{
	printf("bigheadk, QVS_undoEnabled = %d\n", QVS_Engine::QVS_undoEnabled());
	if (!state)
	{
		m_btnUndo.Disable(&m_btnUndo);
		m_btnAdvance.Disable(&m_btnAdvance);
		m_btnDetailUpdate.Disable(&m_btnDetailUpdate);
			
		m_btnRedo.Disable(&m_btnRedo);

		m_btnAlign.Enable(&m_btnAlign);
		m_btnManualMask.Enable(&m_btnManualMask);
		m_btnBack.Enable(&m_btnBack);

		m_btnEditOff.Enable(&m_btnEditOff);
	}
	else
	{
		if (QVS_Engine::QVS_undoEnabled())
		{
			m_btnUndo.Enable(&m_btnUndo);
		}
		else
		{
			m_btnUndo.Disable(&m_btnUndo);
		}

		if (QVS_Engine::QVS_redoEnabled())
			m_btnRedo.Enable(&m_btnRedo);
		else
			m_btnRedo.Disable(&m_btnRedo);

		m_btnAdvance.Enable(&m_btnAdvance);
		m_btnDetailUpdate.Enable(&m_btnDetailUpdate);

		if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_EDIT_ROTATION){
			m_btnAlign.Disable(&m_btnAlign);
			m_btnDetailUpdate.Disable(&m_btnDetailUpdate);
			m_btnSmartCalibration.Disable(&m_btnSmartCalibration);
			m_btnAdvance.Disable(&m_btnAdvance);
			m_btnManualMask.Disable(&m_btnManualMask);
			m_btnUndo.Disable(&m_btnUndo);
			m_btnRedo.Disable(&m_btnRedo);
			m_btnBack.Disable(&m_btnBack);
		}
		else{
			m_btnAlign.Enable(&m_btnAlign);
			m_btnManualMask.Enable(&m_btnManualMask);
			m_btnSmartCalibration.Enable(&m_btnSmartCalibration);
			m_btnBack.Enable(&m_btnBack);
		}

		m_btnEditOff.Enable(&m_btnEditOff);
	}
	
#if LICENSE_PERSONAL_KEY
	if (QVS_Engine::QVS_getPlanType() == QVS_PLANTYPE_DEMO || !state || theApp.m_ScreenManager.m_pSubMode == SUBMODE_EDIT_ROTATION)
	{
		m_btnDetailUpdate.Disable(&m_btnDetailUpdate);
		m_btnDetailUpdate.EnableWindow(false);
		m_btnSmartCalibration.Disable(&m_btnSmartCalibration);
		m_btnSmartCalibration.EnableWindow(false);
		//m_btnAdvance.Disable(&m_btnAdvance);
		//m_btnAdvance.EnableWindow(false);
		//m_btnManualMask.Disable(&m_btnManualMask);
		//m_btnManualMask.EnableWindow(false);
	}
	else
	{
		m_btnDetailUpdate.Enable(&m_btnDetailUpdate);
		m_btnDetailUpdate.EnableWindow(true);
		m_btnSmartCalibration.Enable(&m_btnSmartCalibration);
		m_btnSmartCalibration.EnableWindow(true);
		//m_btnAdvance.Enable(&m_btnAdvance);
		//m_btnAdvance.EnableWindow(true);
		//m_btnManualMask.Enable(&m_btnManualMask);
		//m_btnManualMask.EnableWindow(true);
	}
#endif
}

void CRightCalibChildDlg::disableCalibrationBtns()
{
	m_btnAlign.Disable(&m_btnAlign);
	m_btnManualMask.Disable(&m_btnManualMask);
	m_btnDetailUpdate.Disable(&m_btnDetailUpdate);
	m_btnSmartCalibration.Disable(&m_btnSmartCalibration);
	m_btnAdvance.Disable(&m_btnAdvance);
	//m_btnBack.Disable(&m_btnBack);
	m_btnSnapshot.Disable(&m_btnSnapshot);
	m_btnCalibration.Disable(&m_btnCalibration);
	m_btnUndo.Disable(&m_btnUndo);
	m_btnRedo.Disable(&m_btnRedo);
	m_btnEditOff.Disable(&m_btnEditOff);
	m_btnEditOn.Disable(&m_btnEditOn);
	m_btnStraighten.Disable(&m_btnStraighten);
}

void CRightCalibChildDlg::OnBnClickedBtndetailcalibration()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int check = 0;
	QVS_Engine::QVS_CheckInitParam(check);
	if (!check){
		QVS_Engine::QVS_SetQuramCalibMode(QVS_Engine::CalibrationMode_Detail_All);
	}
	else{
		onCallback_calibration(QVS_MSG_CALIBRATION_NOT_RUN);
	}
}


void CRightCalibChildDlg::OnBnClickedBtnadvance()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_ScreenManager.ShowScreens(vdstRightAdvanceView);
}


void CRightCalibChildDlg::OnBnClickedBtnback()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCenterView* m_pCenter = (CCenterView*)theApp.m_ScreenManager.m_pCenterView;
	if (m_pCenter != NULL)
		m_pCenter->ShowManualRotateDlg(FALSE);

	theApp.m_ScreenManager.ShowScreens(vdstPanoramaView);
}

void CRightCalibChildDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CRightCalibChildDlg::OnBnClickedBtnundocalibration()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	QVS_Engine::QVS_undo();
	refresh();
}


void CRightCalibChildDlg::OnBnClickedBtnredocalibration()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	QVS_Engine::QVS_redo();
	refresh();
}

void CRightCalibChildDlg::OnBnClickedBtnediton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!QVS_Engine::QVS_getIsDoingAnimation())
	{
		m_btnEditOff.ShowWindow(SW_SHOW);
		m_btnEditOff.Enable(&m_btnEditOff);
		m_btnEditOn.ShowWindow(SW_HIDE);
		m_btnStraighten.ShowWindow(SW_HIDE);

		theApp.m_ScreenManager.setSubMode(SUBMODE_OFF);
		theApp.m_ScreenManager.updateSubMenu();
		QVS_Engine::QVS_editOff();
		QVS_Engine::QVS_saveOutput();
	}
}


void CRightCalibChildDlg::OnBnClickedBtneditoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int check = 0;
	QVS_Engine::QVS_CheckInitParam(check);
	if (!check){
		if (QVS_Engine::QVS_editOutput())
		{
			m_btnEditOn.ShowWindow(SW_SHOW);
			m_btnEditOn.Enable(&m_btnEditOn);
			m_btnEditOff.ShowWindow(SW_HIDE);
			m_btnStraighten.ShowWindow(SW_SHOW);
			m_btnStraighten.Enable(&m_btnStraighten);
			theApp.m_ScreenManager.setSubMode(SUBMODE_EDIT_ROTATION);
			theApp.m_ScreenManager.updateSubMenu();
		}

		refresh();
	}
	else{
		onCallback_calibration(QVS_MSG_CALIBRATION_NOT_RUN);
	}
	
}

void CRightCalibChildDlg::OnBnClickedBtnstraighten()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}

void CRightCalibChildDlg::OnBnClickedBtnManualMask()
{
	//printf("OnBnClickedBtnmanualcalibration button click \n");
	QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Manual_On);
	theApp.m_ScreenManager.ShowScreens(vdstRightManualCalibView);
	theApp.m_ScreenManager.setSubMode(SUBMODE_MANUAL_CALIB_MASK);
}

BOOL CRightCalibChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CRightCalibChildDlg::OnBnClickedBtnsmartcalibration()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CRightCalibChildDlg::OnBnClickedBtnManualRotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCenterView* m_pCenter = (CCenterView*)theApp.m_ScreenManager.m_pCenterView;
	if (m_pCenter != NULL)
	{
		m_pCenter->ShowManualRotateDlg(FALSE);
		m_pCenter->ShowManualRotateDlg(TRUE);
	}
}


void CRightCalibChildDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	bool stereoMode = QVS_Engine::QVS_getStereoMode();
	if (stereoMode)
		m_btnAlign.SetImage(CGdipButton::ALT_TYPE);
	else
		m_btnAlign.SetImage(CGdipButton::STD_TYPE);
}
