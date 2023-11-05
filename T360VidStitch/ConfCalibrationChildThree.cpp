// ConfCalibrationChildThree.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfCalibrationChildThree.h"
#include "afxdialogex.h"
#include "FolderDlg.h"

// CConfCalibrationChildThree 대화 상자입니다.

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


// CConfCalibrationChildThree 메시지 처리기입니다.


BOOL CConfCalibrationChildThree::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();

	m_btnBrow.LoadBitmaps(IDBB_BTN_OPENFILE_NORMAL, 0, 0, IDBB_BTN_OPENFILE_DISABLE);
	m_btnBrow.SizeToContent();
	SetFontObj();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CConfCalibrationChildThree::OnBnClickedBtnConfig3Apply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO:  여기서 DC의 특성을 변경합니다.
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
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	return TRUE;
}

void CConfCalibrationChildThree::OnBnClickedBtnConfig33Brow()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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