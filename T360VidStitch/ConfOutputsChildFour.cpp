// ConfOutputsChildFour.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfOutputsChildFour.h"
#include "afxdialogex.h"


// CConfOutputsChildFour 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConfOutputsChildFour, CTaskPanelDlg)

CConfOutputsChildFour::CConfOutputsChildFour(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfOutputsChildFour::~CConfOutputsChildFour()
{
}

void CConfOutputsChildFour::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_1_4_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_CONFIG1_4_BROW, m_btnBrowse);
	DDX_Control(pDX, IDC_CBO_CFOUT4_REPEAT, m_cboRepeat);
}


BEGIN_MESSAGE_MAP(CConfOutputsChildFour, CTaskPanelDlg)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_CONFIG1_4_BROW, &CConfOutputsChildFour::OnBnClickedBtnConfig14Brow)
	ON_BN_CLICKED(IDC_BTN_CONFIG_1_4_SAVE, &CConfOutputsChildFour::OnBnClickedBtnConfig14Save)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CConfOutputsChildFour 메시지 처리기입니다.


HBRUSH CConfOutputsChildFour::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_14_46:
	case IDC_STATIC_14_29:
	case IDC_STATIC_14_30:
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

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL CConfOutputsChildFour::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	return TRUE;
	//return CTaskPanelDlg::OnEraseBkgnd(pDC);
}

void CConfOutputsChildFour::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_CFOUT4_FILEPATH)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT4_SURL)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT4_REPEAT)->SetFont(aria12);

	GetDlgItem(IDC_STATIC_14_29)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_14_30)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_14_46)->SetFont(aria12);

}

BOOL CConfOutputsChildFour::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();

	m_btnBrowse.LoadBitmaps(IDBB_BTN_OPENFILE_NORMAL, 0, 0, IDBB_BTN_OPENFILE_DISABLE);
	m_btnBrowse.SizeToContent();

	m_cboRepeat.SetCurSel(0);

	SetFontObj();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CConfOutputsChildFour::OnBnClickedBtnConfig14Brow()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR szFilters[] = _T("MP4(*.MP4)|*.mp4||");
	CFileDialog dlg(TRUE, _T("mp4"), _T(""), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, NULL, 0);

	//현재 Path저장
	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);

	if (dlg.DoModal() == IDOK)
	{
		CString szFullpath = dlg.GetPathName();
		CString szFileName = dlg.GetFileName();
		GetDlgItem(IDC_CFOUT4_FILEPATH)->SetWindowText(szFullpath);

		//Path원복
		SetCurrentDirectory(Path);
	}
}


void CConfOutputsChildFour::OnBnClickedBtnConfig14Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vod Config save");
	CRightPanoraChildDlg* m_rightPanoDlg = (CRightPanoraChildDlg*)theApp.m_ScreenManager.m_pRightPanoramaChild;
	CString szTemp;

	GetDlgItemTextW(IDC_CFOUT4_FILEPATH, szTemp);
	m_rightPanoDlg->vod_filepath = CString2Char(szTemp);
	printf(">>>> filepath : %s", szTemp);

	GetDlgItemTextW(IDC_CFOUT4_SURL, szTemp);
	m_rightPanoDlg->vod_url = CString2Char(szTemp);
	printf(">>>> url : %s", szTemp);

	m_rightPanoDlg->vod_isRepeat = m_cboRepeat.GetCurSel();
	printf(">>>> repeat : %d\n", m_rightPanoDlg->vod_isRepeat);
}

void CConfOutputsChildFour::SetData()
{
	CRightPanoraChildDlg* m_rightPanoDlg = (CRightPanoraChildDlg*)theApp.m_ScreenManager.m_pRightPanoramaChild;
	CString szTemp;

	szTemp = Char2CString(m_rightPanoDlg->vod_filepath);
	SetDlgItemTextW(IDC_CFOUT4_FILEPATH, szTemp);

	szTemp = Char2CString(m_rightPanoDlg->vod_url);
	SetDlgItemTextW(IDC_CFOUT4_SURL, szTemp);

	m_cboRepeat.SetCurSel(m_rightPanoDlg->vod_isRepeat);
}

void CConfOutputsChildFour::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTaskPanelDlg::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		SetData();
	}
}
