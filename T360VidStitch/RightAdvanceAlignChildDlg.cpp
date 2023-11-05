// RightAdvanceAlignChildDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightAdvanceAlignChildDlg.h"
#include "afxdialogex.h"


// RightAdvanceAlignChildDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(RightAdvanceAlignChildDlg, CDialogEx)

RightAdvanceAlignChildDlg::RightAdvanceAlignChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(RightAdvanceAlignChildDlg::IDD, pParent)
{

}

RightAdvanceAlignChildDlg::~RightAdvanceAlignChildDlg()
{
}

void RightAdvanceAlignChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_ADVANCE_ALIGN_CALIB, m_btnDoCalibration);
	DDX_Control(pDX, IDC_BTN_ADVANCE_ALIGN_BACK, m_btnBack);
	DDX_Control(pDX, IDC_SLD_STEP, m_sldStep);
	DDX_Control(pDX, IDC_EDT_STEP, m_editStep);
	DDX_Control(pDX, IDC_STATIC_STEP, m_textStep);
}


BEGIN_MESSAGE_MAP(RightAdvanceAlignChildDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ADVANCE_ALIGN_BACK, &RightAdvanceAlignChildDlg::OnBnClickedBtnadvanceback)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_ALIGN_CALIB, &RightAdvanceAlignChildDlg::OnBnClickedBtnAdvanceAlignCalib)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL RightAdvanceAlignChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void RightAdvanceAlignChildDlg::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	CFont* ariaB12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria12);
	m_textStep.SetFont(aria12);
}

void RightAdvanceAlignChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		m_btnDoCalibration.LoadImages(IDB_EXPORT_BTN_ADVANCECALIB_NORMAL, 0, IDB_EXPORT_BTN_ADVANCECALIB_MOVER, IDB_EXPORT_BTN_ADVANCECALIB_PRESSED, 0, 0, _T("PNG"));
		m_btnDoCalibration.SizeToContent();

		m_btnBack.LoadImages(IDB_EXPORT_BTN_BACK_NORMAL, 0, IDB_EXPORT_BTN_BACK_MOVER, IDB_EXPORT_BTN_BACK_PRESSED, 0, 0, _T("PNG"));
		m_btnBack.SizeToContent();

		m_sldStep.SetRange(1, 5);
		m_sldStep.SetPos(QVS_Engine::QVS_getStep(QVS_Engine::CalibrationMode_MakeTemplate));
		m_sldStep.SetTicFreq(1);

		CString szTemp;
		int nPos = m_sldStep.GetPos();
		szTemp.Format(_T("%d"), nPos == 0 ? 0 : nPos);
		m_editStep.SetWindowTextW(szTemp);
	}
}


void RightAdvanceAlignChildDlg::OnBnClickedBtnadvanceback()
{
	theApp.m_ScreenManager.ShowScreens(vdstRightAdvanceView);
}


void RightAdvanceAlignChildDlg::OnBnClickedBtnAdvanceAlignCalib()
{
	QVS_Engine::QVS_SetQuramCalibMode(QVS_Engine::CalibrationMode_MakeTemplate);
}

void RightAdvanceAlignChildDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			QVS_Engine::QVS_setStep(QVS_Engine::CalibrationMode_MakeTemplate, nPos);
		}
	}
	else
	{
		// 
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void RightAdvanceAlignChildDlg::setEnableBtns(bool isEnable)
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

BOOL RightAdvanceAlignChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}