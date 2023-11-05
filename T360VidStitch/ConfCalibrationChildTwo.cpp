// ConfCalibrationChildTwo.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfCalibrationChildTwo.h"
#include "afxdialogex.h"


// CConfCalibrationChildTwo ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CConfCalibrationChildTwo, CDialogEx)

CConfCalibrationChildTwo::CConfCalibrationChildTwo(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfCalibrationChildTwo::~CConfCalibrationChildTwo()
{
}

void CConfCalibrationChildTwo::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_2_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_BTN_CONFIG3_2_BROW, m_btnBrow);
}


BEGIN_MESSAGE_MAP(CConfCalibrationChildTwo, CTaskPanelDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG_2_APPLY, &CConfCalibrationChildTwo::OnBnClickedBtnConfig2Apply)
	ON_BN_CLICKED(IDC_BTN_CONFIG3_2_BROW, &CConfCalibrationChildTwo::OnBnClickedBtnConfig32Brow)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CConfCalibrationChildTwo �޽��� ó�����Դϴ�.


void CConfCalibrationChildTwo::OnBnClickedBtnConfig2Apply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString szFullpath;
	GetDlgItem(IDC_CALIB_FILEPATH)->GetWindowText(szFullpath);
	if (szFullpath.GetLength() > 0 && theApp.m_ScreenManager.parsingPTS(szFullpath) == QVS_SUCCESS)
	{
		szFullpath = _T("Import Calibration Template Path : ") + szFullpath;
		QVS_Engine::QVS_writeLog(CString2Char(szFullpath), szFullpath.GetLength()*sizeof(char));

		MessageBox(_T("PTS file apply success!"), MB_OK, MB_ICONEXCLAMATION);
		//ȭ���� �̵�
		theApp.m_ScreenManager.m_pLeftChild->OnBnClickedBtnpanorama();
	}
}


BOOL CConfCalibrationChildTwo::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_btnApply.LoadBitmaps(IDBB_BTN_APPLY_NORMAL, IDBB_BTN_APPLY_PRESSED, 0, IDBB_BTN_APPLY_DISABLE);
	m_btnApply.SizeToContent();

	m_btnBrow.LoadBitmaps(IDBB_BTN_OPENFILE_NORMAL, 0, 0, IDBB_BTN_OPENFILE_DISABLE);
	m_btnBrow.SizeToContent();
	SetFontObj();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CConfCalibrationChildTwo::OnBnClickedBtnConfig32Brow()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	const TCHAR szFilters[] = _T("PTGui file (*.pts)|*.pts||");

	CFileDialog dlg(TRUE, _T("pts"), _T("*.pts"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, NULL, 0);

	//���� Path����
	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);

	if (dlg.DoModal() == IDOK)
	{
		CString szFullpath = dlg.GetPathName();
		CString szFileName = dlg.GetFileName();
		GetDlgItem(IDC_CALIB_FILEPATH)->SetWindowText(szFullpath);

		//Path����
		SetCurrentDirectory(Path);
	}
}


HBRUSH CConfCalibrationChildTwo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_12_46:
	{
					   pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
					   pDC->SetBkMode(TRANSPARENT);
					   hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
		break;
	}
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
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


BOOL CConfCalibrationChildTwo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	return TRUE;
}

void CConfCalibrationChildTwo::SetValues()
{

}

void CConfCalibrationChildTwo::GetValue()
{

}

void CConfCalibrationChildTwo::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_CALIB_FILEPATH)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_46)->SetFont(aria12);

}

void CConfCalibrationChildTwo::clearValue()
{
	GetDlgItem(IDC_CALIB_FILEPATH)->SetWindowText(_T(""));
}