#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightManualCalibChildDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CRightManualCalibChildDlg, CDialogEx)

CRightManualCalibChildDlg::CRightManualCalibChildDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CRightManualCalibChildDlg::IDD, pParent)
{

}

CRightManualCalibChildDlg::~CRightManualCalibChildDlg()
{
}

void CRightManualCalibChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MANUAL_MASK_SAVE, m_btnMaskSave);
	DDX_Control(pDX, IDC_BTN_MANUAL_MASK_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_BTN_MANUAL_BACK, m_btnBack);
	DDX_Control(pDX, IDC_RIGHT_MANUAL_COMBO_LEFT, m_comboMaskLetfList);
	DDX_Control(pDX, IDC_RIGHT_MANUAL_COMBO_RIGHT, m_comboMaskRightList);
	DDX_Control(pDX, IDC_RIGHT_MANUAL_COMBO_COLOR, m_comboMaskColor);
	DDX_Control(pDX, IDC_BTN_MANUAL_MASK_LEFT_ROTATE, m_btnLeftRotate);
	DDX_Control(pDX, IDC_BTN_MANUAL_MASK_RIGHT_ROTATE, m_btnRightRotate);
}
BEGIN_MESSAGE_MAP(CRightManualCalibChildDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MANUAL_MASK_CLEAR, &CRightManualCalibChildDlg::OnBnClickedBtnManualMaskClear)
	ON_BN_CLICKED(IDC_BTN_MANUAL_BACK, &CRightManualCalibChildDlg::OnBnClickedBtnManualBack)
	ON_BN_CLICKED(IDC_BTN_MANUAL_MASK_SAVE, &CRightManualCalibChildDlg::OnBnClickedBtnManualMaskSave)
	ON_CBN_SELCHANGE(IDC_RIGHT_MANUAL_COMBO_LEFT, &CRightManualCalibChildDlg::OnCbnSelchangeRightManualComboLeft)
	ON_CBN_SELCHANGE(IDC_RIGHT_MANUAL_COMBO_RIGHT, &CRightManualCalibChildDlg::OnCbnSelchangeRightManualComboRight)
	ON_CBN_SELCHANGE(IDC_RIGHT_MANUAL_COMBO_COLOR, &CRightManualCalibChildDlg::OnCbnSelchangeRightManualComboColor)
	ON_BN_CLICKED(IDC_BTN_MANUAL_MASK_LEFT_ROTATE, &CRightManualCalibChildDlg::OnBnClickedBtnManualMaskLeftRotate)
	ON_BN_CLICKED(IDC_BTN_MANUAL_MASK_RIGHT_ROTATE, &CRightManualCalibChildDlg::OnBnClickedBtnManualMaskRightRotate)
END_MESSAGE_MAP()

BOOL CRightManualCalibChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	

	m_comboMaskColor.AddString(_T("Hide ( Red )"));
	m_comboMaskColor.AddString(_T("Visible ( Green )"));
	m_comboMaskColor.AddString(_T("Remove"));
	m_comboMaskColor.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRightManualCalibChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		m_btnClear.LoadImages(IDB_EXPORT_BTN_CLEAR_NORMAL, 0, IDB_EXPORT_BTN_CLEAR_MOVER, IDB_EXPORT_BTN_CLEAR_PRESSED, 0, 0, _T("PNG"));
		m_btnClear.SizeToContent();

		m_btnMaskSave.LoadImages(IDB_EXPORT_BTN_MASKSAVE_NORMAL, 0, IDB_EXPORT_BTN_MASKSAVE_MOVER, IDB_EXPORT_BTN_MASKSAVE_PRESSED, 0, 0, _T("PNG"));
		m_btnMaskSave.SizeToContent();

		m_btnBack.LoadImages(IDB_EXPORT_BTN_BACK_NORMAL, 0, IDB_EXPORT_BTN_BACK_MOVER, IDB_EXPORT_BTN_BACK_PRESSED, 0, 0, _T("PNG"));
		m_btnBack.SizeToContent();

		m_btnRightRotate.LoadImages(IDB_EXPORT_BTN_RIGHTROTATE_NORMAL, 0, IDB_EXPORT_BTN_RIGHTROTATE_MOVER, IDB_EXPORT_BTN_RIGHTROTATE_PRESSED, 0, 0, _T("PNG"));
		m_btnRightRotate.SizeToContent();

		m_btnLeftRotate.LoadImages(IDB_EXPORT_BTN_LEFTROTATE_NORMAL, 0, IDB_EXPORT_BTN_LEFTROTATE_MOVER, IDB_EXPORT_BTN_LEFTROTATE_PRESSED, 0, 0, _T("PNG"));
		m_btnLeftRotate.SizeToContent();

		int num_camera = QVS_Engine::QVS_GetNumCameara();
		m_comboMaskLetfList.ResetContent();
		m_comboMaskRightList.ResetContent();
		for (int i = 0; i < num_camera; i++){
			CString cstr;
			cstr.Format(_T("%d"), i);
			m_comboMaskLetfList.InsertString(i, cstr);
			m_comboMaskRightList.InsertString(i, cstr);
		}
		m_comboMaskLetfList.SetCurSel(QVS_Engine::QVS_GetDrawMaskLeftIndex());
		m_comboMaskRightList.SetCurSel(QVS_Engine::QVS_GetDrawMaskRightIndex());
	}
}

BOOL CRightManualCalibChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CRightManualCalibChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_MASKSAVEDONE)
	{
		KillTimer(TIMER_MASKSAVEDONE);
		theApp.m_ScreenManager.ShowToast(vdstToastDone, _T("Mask Save"), FALSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}
void CRightManualCalibChildDlg::OnBnClickedBtnManualMaskClear()
{
	QVS_Engine::QVS_clearManualMaskROI(false);
}
void CRightManualCalibChildDlg::OnBnClickedBtnManualBack()
{
	QVS_Engine::QVS_InitDrawManualROI();
	//QVS_Engine::QVS_clearManualMaskROI(true);
	theApp.m_ScreenManager.ShowScreens(vdstRightCalibView);
}

void CRightManualCalibChildDlg::OnBnClickedBtnManualMaskSave()
{
	/*for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
		QVS_Engine::QVS_saveMask(i);*/
	QVS_Engine::QVS_saveMask(-1);

	theApp.m_ScreenManager.ShowToast(vdstToastDone, _T("Mask Save"), TRUE);
	SetTimer(TIMER_MASKSAVEDONE, 2000, NULL);
}

void CRightManualCalibChildDlg::OnCbnSelchangeRightManualComboLeft()
{	
	int nSelect = m_comboMaskLetfList.GetCurSel();
	QVS_Engine::QVS_SetDrawMaskLeftIndex(nSelect);
}

void CRightManualCalibChildDlg::OnCbnSelchangeRightManualComboRight()
{
	int nSelect = m_comboMaskRightList.GetCurSel();
	QVS_Engine::QVS_SetDrawMaskRightIndex(nSelect);
}


void CRightManualCalibChildDlg::OnCbnSelchangeRightManualComboColor()
{
	int nSelect = m_comboMaskColor.GetCurSel();
	QVS_Engine::QVS_SetDrawMaskColor(nSelect);
}


void CRightManualCalibChildDlg::OnBnClickedBtnManualMaskLeftRotate()
{
	QVS_Engine::QVS_RotateManualMaskLeft(0);
}


void CRightManualCalibChildDlg::OnBnClickedBtnManualMaskRightRotate()
{
	QVS_Engine::QVS_RotateManualMaskRight(0);
}