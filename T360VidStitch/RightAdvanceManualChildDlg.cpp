// RightAdvanceManualChildDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightAdvanceManualChildDlg.h"
#include "afxdialogex.h"


// RightAdvanceManualChildDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(RightAdvanceManualChildDlg, CDialogEx)

RightAdvanceManualChildDlg::RightAdvanceManualChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(RightAdvanceManualChildDlg::IDD, pParent)
{

}

RightAdvanceManualChildDlg::~RightAdvanceManualChildDlg()
{
}

void RightAdvanceManualChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_ADVANCE_DOMANUAL, m_btnDoWarp);
	DDX_Control(pDX, IDC_BTN_ADVANCE_MANUAL_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_ADVANCE_MANUAL_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BTN_ADVANCE_MANUAL_BACK, m_btnBack);
	DDX_Control(pDX, IDC_SLIDER_WARPING_SIZE, m_sldSize);
	DDX_Control(pDX, IDC_EDT_WARPING_SIZE, m_editSize);
	DDX_Control(pDX, IDC_SLIDER_WARPING_LENGTH, m_sldLength);
	DDX_Control(pDX, IDC_EDT_WARPING_LENGTH, m_editLength);
	DDX_Control(pDX, IDC_RIGHT_MANUAL_COMBO_WARPING_MODE, m_comboWarpingMode);
	DDX_Control(pDX, IDC_RIGHT_MANUAL_COMBO_AUTO_MODE, m_comboAutoMode);
	DDX_Control(pDX, IDC_BTN_ADVANCE_MANUAL_UNDO, m_btnUndo);
	DDX_Control(pDX, IDC_BTN_ADVANCE_MANUAL_REDO, m_btnRedo);
	DDX_Control(pDX, IDC_BTN_ADVANCE_MANUAL_RESETWARPING, m_btnResetWarping);
	DDX_Control(pDX, IDC_BTN_ADVANCE_MANUAL_CLEARPOINT, m_btnClearPoint);
}


BEGIN_MESSAGE_MAP(RightAdvanceManualChildDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_ADVANCE_DOMANUAL, &RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualDoWarping)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_MANUAL_BACK, &RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualBack)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_RIGHT_MANUAL_COMBO_WARPING_MODE, &RightAdvanceManualChildDlg::OnCbnSelchangeRightManualComboWarpingMode)
	ON_CBN_SELCHANGE(IDC_RIGHT_MANUAL_COMBO_AUTO_MODE, &RightAdvanceManualChildDlg::OnCbnSelchangeRightManualComboAutoMode)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_MANUAL_EDIT, &RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualEdit)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_MANUAL_DELETE, &RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualDelete)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_MANUAL_UNDO, &RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualUndo)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_MANUAL_REDO, &RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualRedo)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_MANUAL_RESETWARPING, &RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualResetwarping)
	ON_BN_CLICKED(IDC_BTN_ADVANCE_MANUAL_CLEARPOINT, &RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualClearpoint)
END_MESSAGE_MAP()


BOOL RightAdvanceManualChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
void RightAdvanceManualChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		m_btnDoWarp.LoadImages(IDB_EXPORT_BTN_WARPING_NORMAL, 0, IDB_EXPORT_BTN_WARPING_MOVER, IDB_EXPORT_BTN_WARPING_PRESSED, 0, 0, _T("PNG"));
		m_btnDoWarp.SizeToContent();

		m_btnResetWarping.LoadImages(IDB_EXPORT_BTN_WARPING_RESETWARPINT_NORMAL, 0, IDB_EXPORT_BTN_WARPING_RESETWARPINT_MOVER, IDB_EXPORT_BTN_WARPING_RESETWARPINT_PRESSED, 0, 0, _T("PNG"));
		m_btnResetWarping.SizeToContent();

		m_btnClearPoint.LoadImages(IDB_EXPORT_BTN_WARPING_CLEARPOINT_NORMAL, 0, IDB_EXPORT_BTN_WARPING_CLEARPOINT_MOVER, IDB_EXPORT_BTN_WARPING_CLEARPOINT_PRESSED, 0, 0, _T("PNG"));
		m_btnClearPoint.SizeToContent();

		m_btnEdit.LoadImages(IDB_EXPORT_BTN_EDITOFF_NORMAL, IDB_EXPORT_BTN_EDITON_NORMAL, IDB_EXPORT_BTN_EDITOFF_MOVER, IDB_EXPORT_BTN_EDITOFF_PRESSED, IDB_EXPORT_BTN_EDITON_MOVER, IDB_EXPORT_BTN_EDITON_PRESSED, _T("PNG"));
		m_btnEdit.SizeToContent();
		m_btnEdit.ShowWindow(SW_HIDE);

		m_btnDelete.LoadImages(IDB_EXPORT_BTN_DELETE_NORMAL, 0, IDB_EXPORT_BTN_DELETE_MOVER, IDB_EXPORT_BTN_DELETE_PRESSED, 0, 0, _T("PNG"));
		m_btnDelete.SizeToContent();
		m_btnDelete.ShowWindow(SW_HIDE);

		m_btnUndo.LoadImages(IDB_EXPORT_BTN_UNDO_NORMAL, 0, IDB_EXPORT_BTN_UNDO_MOVER, IDB_EXPORT_BTN_UNDO_PRESSED, 0, 0, _T("PNG"));
		m_btnUndo.SizeToContent();
		m_btnUndo.ShowWindow(SW_HIDE);

		m_btnRedo.LoadImages(IDB_EXPORT_BTN_REDO_NORMAL, 0, IDB_EXPORT_BTN_REDO_MOVER, IDB_EXPORT_BTN_REDO_PRESSED, 0, 0, _T("PNG"));
		m_btnRedo.SizeToContent();
		m_btnRedo.ShowWindow(SW_HIDE);

		m_btnBack.LoadImages(IDB_EXPORT_BTN_BACK_NORMAL, 0, IDB_EXPORT_BTN_BACK_MOVER, IDB_EXPORT_BTN_BACK_PRESSED, 0, 0, _T("PNG"));
		m_btnBack.SizeToContent();

		refresh();
	}

	if (!bShow)
	{
		theApp.m_ScreenManager.setSubMode(SUBMODE_OFF);
	}
}

void RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualDoWarping()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	refresh();
}

void RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualBack()
{

}

BOOL RightAdvanceManualChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void RightAdvanceManualChildDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void RightAdvanceManualChildDlg::OnCbnSelchangeRightManualComboWarpingMode()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}


void RightAdvanceManualChildDlg::OnCbnSelchangeRightManualComboAutoMode()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
}


void RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualEdit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
}


void RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	refresh();
}


void RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualUndo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//QVS_Engine::QVS_undoRegionalWarping();
	refresh();
}


void RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualRedo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//QVS_Engine::QVS_redoRegionalWarping();
	refresh();
}

void RightAdvanceManualChildDlg::refresh(bool state)
{
	//if (QVS_Engine::QVS_getRegionalWarpingGroupCurrentIndex() > 0) {
	//	m_btnUndo.EnableWindow(TRUE);
	//	m_btnUndo.SetImage(CGdipButton::STD_TYPE);
	//}
	//else {
	//	m_btnUndo.EnableWindow(FALSE);
	//	m_btnUndo.SetImage(CGdipButton::DIS_TYPE);
	//}

	//if (QVS_Engine::QVS_getRegionalWarpingGroupCurrentIndex() < QVS_Engine::QVS_getRegionalWarpingGroupSize() - 1) {
	//	m_btnRedo.EnableWindow(TRUE);
	//	m_btnRedo.SetImage(CGdipButton::STD_TYPE);
	//}
	//else {
	//	m_btnRedo.EnableWindow(FALSE);
	//	m_btnRedo.SetImage(CGdipButton::DIS_TYPE);
	//}


}

void RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualResetwarping()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	refresh();
}


void RightAdvanceManualChildDlg::OnBnClickedBtnAdvanceManualClearpoint()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	refresh();
}
