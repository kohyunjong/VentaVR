// ConfCalibrationChildOne.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfCalibrationChildOne.h"
#include "afxdialogex.h"


// CConfCalibrationChildOne 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConfCalibrationChildOne, CDialogEx)

CConfCalibrationChildOne::CConfCalibrationChildOne(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfCalibrationChildOne::~CConfCalibrationChildOne()
{
}

void CConfCalibrationChildOne::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_CREATE, m_btnCreate);
}


BEGIN_MESSAGE_MAP(CConfCalibrationChildOne, CTaskPanelDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG_CREATE, &CConfCalibrationChildOne::OnBnClickedBtnConfigCreate)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CConfCalibrationChildOne 메시지 처리기입니다.


void CConfCalibrationChildOne::OnBnClickedBtnConfigCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CConfCalibrationChildOne::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_btnCreate.LoadBitmaps(IDBB_BTN_CREATE_NORMAL, IDBB_BTN_CREATE_PRESSED, 0, IDBB_BTN_CREATE_DISABLE);
	m_btnCreate.SizeToContent();

	((CComboBox*)GetDlgItem(IDC_CBO_CALIB_PROJ))->SetCurSel(0);
	GetDlgItem(IDC_CALIB_HFOV)->SetWindowText(_T("120.0"));
	SetFontObj();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HBRUSH CConfCalibrationChildOne::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_12_44:
		{
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		}

		case IDC_STATIC_12_45:
		{
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		}
		break;
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT: // Edit box
		switch (pWnd->GetParent()->GetDlgCtrlID())
		{
		case IDC_CBO_CALIB_PROJ:
			pDC->SetTextColor(COLOR_DIALOG_GRAY_TEXT);
			pDC->SetBkColor(COLOR_DIALOG_GRAY_BK);
			hbr = hbrNomalBack;
			break;
		default:
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
			hbr = hbrNomalBack;
			break;
		}
		break;
	case CTLCOLOR_LISTBOX: // Listbox
		switch (pWnd->GetDlgCtrlID())
		{
		default:
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
			hbr = hbrNomalBack;
			break;
		}
		break;
	}
	return hbr;
}


BOOL CConfCalibrationChildOne::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	return TRUE;
}

void CConfCalibrationChildOne::SetValues()
{

}

void CConfCalibrationChildOne::GetValue()
{

}

void CConfCalibrationChildOne::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_CALIB_HFOV)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CALIB_PROJ)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_44)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_45)->SetFont(aria12);

}