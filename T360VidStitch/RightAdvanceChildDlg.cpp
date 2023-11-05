#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightAdvanceChildDlg.h"
#include "afxdialogex.h"


// RightAdvanceChildDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(RightAdvanceChildDlg, CDialogEx)

RightAdvanceChildDlg::RightAdvanceChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(RightAdvanceChildDlg::IDD, pParent)
{

}

RightAdvanceChildDlg::~RightAdvanceChildDlg()
{
}

void RightAdvanceChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_ADVANCE_ALIGN, m_btnAdvanceAlign);
	DDX_Control(pDX, IDC_BTN_ADVANCE_UPDATE, m_btnAdvanceUpdate);
	DDX_Control(pDX, IDC_BTN_ADVANCE_MANUAL, m_btnAdvanceManual);
	DDX_Control(pDX, IDC_BTN_ADVANCE_BACK, m_btnBack);
}


BEGIN_MESSAGE_MAP(RightAdvanceChildDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ADVANCE_ALIGN, &RightAdvanceChildDlg::OnBnClickedBtnAdvanceAlign)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_UPDATE, &RightAdvanceChildDlg::OnBnClickedBtnAdvanceUpdate)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_MANUAL, &RightAdvanceChildDlg::OnBnClickedBtnAdvanceManual)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_BACK, &RightAdvanceChildDlg::OnBnClickedBtnAdvanceBack)
END_MESSAGE_MAP()

BOOL RightAdvanceChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void RightAdvanceChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
}

void RightAdvanceChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		m_btnAdvanceAlign.LoadImages(IDB_EXPORT_BTN_ALIGNMODE_NORMAL, 0, IDB_EXPORT_BTN_ALIGNMODE_MOVER, IDB_EXPORT_BTN_ALIGNMODE_PRESSED, 0, 0, _T("PNG"));
		m_btnAdvanceAlign.SizeToContent();

		m_btnAdvanceUpdate.LoadImages(IDB_EXPORT_BTN_UPDATEMODE_NORMAL, 0, IDB_EXPORT_BTN_UPDATEMODE_MOVER, IDB_EXPORT_BTN_UPDATEMODE_PRESSED, 0, 0, _T("PNG"));
		m_btnAdvanceUpdate.SizeToContent();

		m_btnAdvanceManual.LoadImages(IDB_EXPORT_BTN_MANUALMODE_NORMAL, 0, IDB_EXPORT_BTN_MANUALMODE_MOVER, IDB_EXPORT_BTN_MANUALMODE_PRESSED, 0, 0, _T("PNG"));
		m_btnAdvanceManual.SizeToContent();
		//m_btnAdvanceManual.ShowWindow(SW_HIDE);

		m_btnBack.LoadImages(IDB_EXPORT_BTN_BACK_NORMAL, 0, IDB_EXPORT_BTN_BACK_MOVER, IDB_EXPORT_BTN_BACK_PRESSED, 0, 0, _T("PNG"));
		m_btnBack.SizeToContent();
	}
}


void RightAdvanceChildDlg::OnBnClickedBtnAdvanceAlign()
{
	theApp.m_ScreenManager.ShowScreens(vdstRightAdvanceAlignView);
}


void RightAdvanceChildDlg::OnBnClickedBtnAdvanceUpdate()
{
	int check = 0;
	QVS_Engine::QVS_CheckInitParam(check);
	if (!check){
		theApp.m_ScreenManager.setSubMode(SUBMODE_ADVANCE_UPDATE);
		QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_On);
		QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_On);
		theApp.m_ScreenManager.ShowScreens(vdstRightAdvanceUpdateView);
	}
	else{
		onCallback_calibration(QVS_MSG_CALIBRATION_NOT_RUN);
	}
}


void RightAdvanceChildDlg::OnBnClickedBtnAdvanceManual()
{
	//theApp.m_ScreenManager.ShowScreens(vdstRightAdvanceManualView);

	int check = 0;
	QVS_Engine::QVS_CheckInitParam(check);
	if (!check){
		theApp.m_ScreenManager.setSubMode(SUBMODE_REGIONAL_WARPING);
		theApp.m_ScreenManager.ShowScreens(vdstRightAdvanceManualView);
	}
	else{
		onCallback_calibration(QVS_MSG_CALIBRATION_NOT_RUN);
	}
}


void RightAdvanceChildDlg::OnBnClickedBtnAdvanceBack()
{
	theApp.m_ScreenManager.ShowScreens(vdstRightCalibView);
}

BOOL RightAdvanceChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void RightAdvanceChildDlg::refresh()
{
#if LICENSE_PERSONAL_KEY
	if (QVS_Engine::QVS_getPlanType() == QVS_PLANTYPE_DEMO)
	{
		m_btnAdvanceAlign.Disable(&m_btnAdvanceAlign);
		m_btnAdvanceAlign.EnableWindow(false);
		m_btnAdvanceUpdate.Disable(&m_btnAdvanceUpdate);
		m_btnAdvanceUpdate.EnableWindow(false);
	}
	else
	{
		m_btnAdvanceAlign.Enable(&m_btnAdvanceAlign);
		m_btnAdvanceAlign.EnableWindow(true);
		m_btnAdvanceUpdate.Enable(&m_btnAdvanceUpdate);
		m_btnAdvanceUpdate.EnableWindow(true);
	}
#endif
}