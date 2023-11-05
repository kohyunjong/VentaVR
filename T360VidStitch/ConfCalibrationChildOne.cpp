// ConfCalibrationChildOne.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfCalibrationChildOne.h"
#include "afxdialogex.h"


// CConfCalibrationChildOne ��ȭ �����Դϴ�.

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


// CConfCalibrationChildOne �޽��� ó�����Դϴ�.


void CConfCalibrationChildOne::OnBnClickedBtnConfigCreate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CConfCalibrationChildOne::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_btnCreate.LoadBitmaps(IDBB_BTN_CREATE_NORMAL, IDBB_BTN_CREATE_PRESSED, 0, IDBB_BTN_CREATE_DISABLE);
	m_btnCreate.SizeToContent();

	((CComboBox*)GetDlgItem(IDC_CBO_CALIB_PROJ))->SetCurSel(0);
	GetDlgItem(IDC_CALIB_HFOV)->SetWindowText(_T("120.0"));
	SetFontObj();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


HBRUSH CConfCalibrationChildOne::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
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
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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