// RightAdvanceUpdateChildDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightAdvanceUpdateChildDlg.h"
#include "afxdialogex.h"


// RightAdvanceUpdateChildDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(RightAdvanceUpdateChildDlg, CDialogEx)

RightAdvanceUpdateChildDlg::RightAdvanceUpdateChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(RightAdvanceUpdateChildDlg::IDD, pParent)
{

}

RightAdvanceUpdateChildDlg::~RightAdvanceUpdateChildDlg()
{
}

void RightAdvanceUpdateChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_ADVANCE_UPDATE_ROI_CLEAR_ALL, m_btnRoiClearAll);
	DDX_Control(pDX, IDC_BTN_ADVANCE_UPDATE_SELECT_ALL, m_btnSelectAll);
	DDX_Control(pDX, IDC_BTN_ADVANCE_UPDATE_CALIB, m_btnDoCalibration);
	DDX_Control(pDX, IDC_BTN_ADVANCE_UPDATE_BACK, m_btnBack);
	DDX_Control(pDX, IDC_SLD_STEP, m_sldStep);
	DDX_Control(pDX, IDC_EDT_STEP, m_editStep);
	DDX_Control(pDX, IDC_STATIC_STEP, m_textStep);
}


BEGIN_MESSAGE_MAP(RightAdvanceUpdateChildDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_ADVANCE_UPDATE_ROI_CLEAR_ALL, &RightAdvanceUpdateChildDlg::OnBnClickedBtnAdvanceUpdateRoiClearAll)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_UPDATE_SELECT_ALL, &RightAdvanceUpdateChildDlg::OnBnClickedBtnAdvanceUpdateSelectAll)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_UPDATE_CALIB, &RightAdvanceUpdateChildDlg::OnBnClickedBtnAdvanceUpdateCalib)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_UPDATE_BACK, &RightAdvanceUpdateChildDlg::OnBnClickedBtnAdvanceUpdateBack)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


BOOL RightAdvanceUpdateChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void RightAdvanceUpdateChildDlg::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	CFont* ariaB12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria12);
	m_textStep.SetFont(aria12);
}

void RightAdvanceUpdateChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		m_btnRoiClearAll.LoadImages(IDB_EXPORT_BTN_CLEAR_NORMAL, 0, IDB_EXPORT_BTN_CLEAR_MOVER, IDB_EXPORT_BTN_CLEAR_PRESSED, 0, 0, _T("PNG"));
		m_btnRoiClearAll.SizeToContent();

		m_btnSelectAll.LoadImages(IDB_EXPORT_BTN_SELECTALL_NORMAL, IDB_EXPORT_BTN_SELECTALL_SELECTED, IDB_EXPORT_BTN_SELECTALL_MOVER, IDB_EXPORT_BTN_SELECTALL_PRESSED, IDB_EXPORT_BTN_SELECTALL_SELECTED_MOVER, IDB_EXPORT_BTN_SELECTALL_SELECTED_PRESSED, _T("PNG"));
		m_btnSelectAll.SizeToContent();

		m_btnDoCalibration.LoadImages(IDB_EXPORT_BTN_ADVANCECALIB_NORMAL, 0, IDB_EXPORT_BTN_ADVANCECALIB_MOVER, IDB_EXPORT_BTN_ADVANCECALIB_PRESSED, 0, 0, _T("PNG"));
		m_btnDoCalibration.SizeToContent();

		m_btnBack.LoadImages(IDB_EXPORT_BTN_BACK_NORMAL, 0, IDB_EXPORT_BTN_BACK_MOVER, IDB_EXPORT_BTN_BACK_PRESSED, 0, 0, _T("PNG"));
		m_btnBack.SizeToContent();

		m_sldStep.SetRange(1, 5);
		m_sldStep.SetPos(QVS_Engine::QVS_getStep(QVS_Engine::CalibrationMode_Advanced));
		m_sldStep.SetTicFreq(1);

		CString szTemp;
		int nPos = m_sldStep.GetPos();
		szTemp.Format(_T("%d"), nPos == 0 ? 0 : nPos);
		m_editStep.SetWindowTextW(szTemp);

		isSelectAllCheck();
	}
}

void RightAdvanceUpdateChildDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString szTemp;
	if (pScrollBar)
	{
		if (pScrollBar == (CScrollBar*)&m_sldStep)
		{
			int nPos = m_sldStep.GetPos();
			szTemp.Format(_T("%d"), nPos == 0 ? 0 : nPos);
			m_editStep.SetWindowTextW(szTemp);
			QVS_Engine::QVS_setStep(QVS_Engine::CalibrationMode_Advanced, nPos);
		}
	}
	else
	{
		// 
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void RightAdvanceUpdateChildDlg::OnBnClickedBtnAdvanceUpdateRoiClearAll()
{
	QVS_Engine::QVS_clearROI();
}


void RightAdvanceUpdateChildDlg::OnBnClickedBtnAdvanceUpdateSelectAll()
{
	QVS_Engine::QVS_selectAllROI();

	isSelectAllCheck();
}


void RightAdvanceUpdateChildDlg::OnBnClickedBtnAdvanceUpdateCalib()
{
	QVS_Engine::QVS_SetQuramCalibMode(QVS_Engine::CalibrationMode_Advanced);
}


void RightAdvanceUpdateChildDlg::OnBnClickedBtnAdvanceUpdateBack()
{
	QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_Off);
	QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Off);
	theApp.m_ScreenManager.setSubMode(SUBMODE_OFF);
	theApp.m_ScreenManager.ShowScreens(vdstRightAdvanceView);
}

void RightAdvanceUpdateChildDlg::setEnableBtns(bool isEnable)
{
	if (isEnable)
	{
		m_btnDoCalibration.Enable(&m_btnDoCalibration);
	}
	else
	{
		m_btnDoCalibration.Disable(&m_btnDoCalibration);
	}
}

BOOL RightAdvanceUpdateChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void RightAdvanceUpdateChildDlg::isSelectAllCheck()
{
	if (QVS_Engine::QVS_getSelectedImage() == QVS_Engine::QVS_GetNumInput())
		m_btnSelectAll.SetImage(CGdipButton::ALT_TYPE);
	else
		m_btnSelectAll.SetImage(CGdipButton::STD_TYPE);
}