// ConfCalibrationChildThree.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfCalibrationChildThree.h"
#include "afxdialogex.h"
#include "FolderDlg.h"

// CConfCalibrationChildThree ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CConfCalibrationChildThree, CDialogEx)

CConfCalibrationChildThree::CConfCalibrationChildThree(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfCalibrationChildThree::~CConfCalibrationChildThree()
{
}

void CConfCalibrationChildThree::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_3_APPLY, m_btnSave);
	DDX_Control(pDX, IDC_BTN_CONFIG3_3_BROW, m_btnBrow);
}


BEGIN_MESSAGE_MAP(CConfCalibrationChildThree, CTaskPanelDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG_3_APPLY, &CConfCalibrationChildThree::OnBnClickedBtnConfig3Apply)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_CONFIG3_3_BROW, &CConfCalibrationChildThree::OnBnClickedBtnConfig33Brow)
END_MESSAGE_MAP()


// CConfCalibrationChildThree �޽��� ó�����Դϴ�.


BOOL CConfCalibrationChildThree::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.


	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();

	m_btnBrow.LoadBitmaps(IDBB_BTN_OPENFILE_NORMAL, 0, 0, IDBB_BTN_OPENFILE_DISABLE);
	m_btnBrow.SizeToContent();
	SetFontObj();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CConfCalibrationChildThree::OnBnClickedBtnConfig3Apply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString szPath;
	GetDlgItem(IDC_CALIB_SNSHOT_PATH)->GetWindowText(szPath);

	char* szpPath = CString2Char(szPath);

	if (szPath.GetLength() > 0)
	{
		QVS_Engine::QVS_SetSnapshotPath(szpPath);
		MessageBox(_T("Snapshot path set complite."), MB_OK, MB_ICONINFORMATION);
	}

	free(szpPath);
}


HBRUSH CConfCalibrationChildThree::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_12_47:
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


BOOL CConfCalibrationChildThree::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	return TRUE;
}

void CConfCalibrationChildThree::OnBnClickedBtnConfig33Brow()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);
	CString currentPath = _T("");
	LPCTSTR lpszTitle = _T("Select the root folder for the browse dialog:");
	UINT	uFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

	//m_txtPrjBasePath.GetWindowText(currentPath);
	//GetDlgItem(IDC_CALIB_SNSHOT_PATH)->GetWindowText(currentPath);
	currentPath = Path;

	CFolderDialog dlgRoot(lpszTitle, currentPath, this, uFlags);

	if (dlgRoot.DoModal() == IDOK)
	{
		//currentPath = dlgRoot.GetFolderPath();
		GetDlgItem(IDC_CALIB_SNSHOT_PATH)->SetWindowText(dlgRoot.GetFolderPath());
		UpdateData(FALSE);
	}
}

void CConfCalibrationChildThree::SetValues()
{
	GetDlgItem(IDC_CALIB_SNSHOT_PATH)->SetWindowText(Char2CString(QVS_Engine::QVS_GetSnapshot_path()));
}

void CConfCalibrationChildThree::GetValue()
{

}


void CConfCalibrationChildThree::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_CALIB_SNSHOT_PATH)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_47)->SetFont(aria12);

}