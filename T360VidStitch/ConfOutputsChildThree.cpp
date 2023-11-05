// ConfOutputsChildThree.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfOutputsChildThree.h"
#include "afxdialogex.h"


// CConfOutputsChildThree ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CConfOutputsChildThree, CDialogEx)

CConfOutputsChildThree::CConfOutputsChildThree(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfOutputsChildThree::~CConfOutputsChildThree()
{
}

void CConfOutputsChildThree::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG1_3_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_CBO_CFOUT3_DEV, m_cboBmDevice);
	DDX_Control(pDX, IDC_CBO_CFOUT3_DP, m_cboBmDisplay);
}


BEGIN_MESSAGE_MAP(CConfOutputsChildThree, CTaskPanelDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG1_3_SAVE, &CConfOutputsChildThree::OnBnClickedBtnConfig13Save)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CConfOutputsChildThree �޽��� ó�����Դϴ�.


BOOL CConfOutputsChildThree::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

// 	m_btnSave.LoadImages(IDB_BTN_SAVE_NORMAL, 0, 0, IDB_BTN_SAVE_PRESSED, _T("PNG"));
// 	m_btnSave.SetToolTipText(_T("Show Source VIew"));
// 	m_btnSave.SizeToContent();

	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();
	SetFontObj();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CConfOutputsChildThree::OnBnClickedBtnConfig13Save()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->GetValue();
	((CConfiguChildView1*)(GetParent()->GetParent()))->m_dlgsubHDDConf.GetValue();
	((CConfiguChildView1*)(GetParent()->GetParent()))->m_dlgsubRTMPConf.GetValue();

	theApp.m_PrjAllCfg.SaveConfigValue();
}


HBRUSH CConfOutputsChildThree::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_12_38:
		case IDC_STATIC_12_39:
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
		switch (pWnd->GetDlgCtrlID())
		{
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


BOOL CConfOutputsChildThree::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	return TRUE;

}

void CConfOutputsChildThree::SetValues()
{
	m_cboBmDevice.ResetContent();
	m_cboBmDevice.AddString(_T("No device"));
	int i = 0;
	while (true) {
		i++;
		char displayName[256];
		char modelName[256];
		if (QVS_Engine::QVS_GetDeviceList(i, displayName, modelName) == false) {
			break;
		}

		m_cboBmDevice.AddString(CString(displayName));
	}
	m_cboBmDevice.SetCurSel(QVS_Engine::QVS_GetBmOutputDevice());

	m_cboBmDisplay.ResetContent();
	m_cboBmDisplay.AddString(_T("Default Display (3840x2160 30fps)"));
	m_cboBmDisplay.SetCurSel(QVS_Engine::QVS_GetBmOutputDisplay());
}

void CConfOutputsChildThree::GetValue()
{
	UpdateData(TRUE);
	theApp.m_PrjAllCfg.m_Configuration.bmoutput_device = m_cboBmDevice.GetCurSel();
	theApp.m_PrjAllCfg.m_Configuration.bmoutput_display = m_cboBmDisplay.GetCurSel();
}

void CConfOutputsChildThree::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_CBO_CFOUT3_DEV)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT3_DP)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_38)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_39)->SetFont(aria12);


}