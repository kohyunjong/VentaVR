// ConfPanoramaChildOne.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfPanoramaChildOne.h"
#include "afxdialogex.h"


// CConfPanoramaChildOne 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConfPanoramaChildOne, CTaskPanelDlg)

CConfPanoramaChildOne::CConfPanoramaChildOne(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfPanoramaChildOne::~CConfPanoramaChildOne()
{
}

void CConfPanoramaChildOne::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_2_1_SAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CConfPanoramaChildOne, CTaskPanelDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG_2_1_SAVE, &CConfPanoramaChildOne::OnBnClickedBtnConfig2Save)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CConfPanoramaChildOne 메시지 처리기입니다.
BOOL CConfPanoramaChildOne::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();

	//((CComboBox*)GetDlgItem(IDC_CFPANO_1_HEIGHT))->SetCurSel(0);

	SetFontObj();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CConfPanoramaChildOne::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CTaskPanelDlg::PreTranslateMessage(pMsg);
}

void CConfPanoramaChildOne::OnBnClickedBtnConfig2Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nVal = ((CComboBox*)GetDlgItem(IDC_CFPANO_1_HEIGHT))->GetCurSel();
	if (nVal != -1)
		QVS_Engine::QVS_SetPanoramaSize(nVal);
}


void CConfPanoramaChildOne::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTaskPanelDlg::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


HBRUSH CConfPanoramaChildOne::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_12_52:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	break;
	case IDC_CFPANO_1_HEIGHT:
	{
		pDC->SetTextColor(COLOR_DIALOG_GRAY_TEXT); // @KEYWORD@THEME@
		pDC->SetBkColor(COLOR_DIALOG_GRAY_BK);
		hbr = hbrBack;
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


BOOL CConfPanoramaChildOne::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}

void CConfPanoramaChildOne::SetValues()
{
	CComboBox* m_cboResolution = (CComboBox*)GetDlgItem(IDC_CFPANO_1_HEIGHT);
	m_cboResolution->ResetContent();

	int wdt = theApp.m_PrjAllCfg.m_nDstWidth;
	int hgt = theApp.m_PrjAllCfg.m_nDstHeight;

	if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo)
		hgt *= 2;
	else if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
		wdt *= 2;

	CString sz_temp;
	sz_temp.Format(_T("(%dx%d)"), wdt, hgt);
	m_cboResolution->AddString(sz_temp);

	if (theApp.m_PrjAllCfg.m_nStitchingMode != QVS_Engine::StitchingMode_Wide)
	{
		hgt = wdt / 16 * 9;
		sz_temp.Format(_T("(%dx%d)"), wdt, hgt);
		m_cboResolution->AddString(sz_temp);
	}

	int n = QVS_Engine::QVS_GetPanoramaSize();
	((CComboBox*)GetDlgItem(IDC_CFPANO_1_HEIGHT))->SetCurSel(n);
}

void CConfPanoramaChildOne::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_CFPANO_1_HEIGHT)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_52)->SetFont(aria12);
}