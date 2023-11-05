// NewLoadConfigChild_3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "NewLoadConfigChild_3.h"
#include "afxdialogex.h"


// CNewLoadConfigChild_3 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewLoadConfigChild_3, CDialogEx)

CNewLoadConfigChild_3::CNewLoadConfigChild_3(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{
	m_pNewLoadConfigChild_0 = NULL;
}

CNewLoadConfigChild_3::~CNewLoadConfigChild_3()
{
	if (m_pNewLoadConfigChild_0 != NULL)
		delete m_pNewLoadConfigChild_0;
}

void CNewLoadConfigChild_3::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_NUMBEROFCAM3, m_cboNumOfCam3);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_INPUTSIZE3, m_cboInputSize3);
}


BEGIN_MESSAGE_MAP(CNewLoadConfigChild_3, CTaskPanelDlg)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_NUMBEROFCAM3, &CNewLoadConfigChild_3::OnCbnSelchangeComboNewloadNumberofcam3)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_INPUTSIZE3, &CNewLoadConfigChild_3::OnCbnSelchangeComboNewloadInputsize3)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CNewLoadConfigChild_3 메시지 처리기입니다.
BOOL CNewLoadConfigChild_3::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemTextW(IDC_EDT_NEWLOAD_WIDTH3, _T("3840"));
	SetDlgItemTextW(IDC_EDT_NEWLOAD_HEIGHT3, _T("3840"));

	if (m_pNewLoadConfigChild_0 != NULL)
		delete m_pNewLoadConfigChild_0;

	m_pNewLoadConfigChild_0 = new CNewLoadConfigChild_0;
	m_pNewLoadConfigChild_0->Create(CNewLoadConfigChild_0::IDD, this);
	m_pNewLoadConfigChild_0->ShowWindow(SW_SHOW);
	
	CRect clientRect, rc;
	GetClientRect(clientRect);
	m_pNewLoadConfigChild_0->GetClientRect(rc);
	m_pNewLoadConfigChild_0->MoveWindow(0, clientRect.bottom, rc.Width(), rc.Height());
	this->MoveWindow(0, 0, clientRect.Width(), clientRect.Height() + rc.Height());

	resetUI();
	SetFontObj();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH CNewLoadConfigChild_3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_GRAY_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_NEWLOAD_3:
	case IDC_STATIC_NEWLOAD_4:
	case IDC_STATIC_NEWLOAD_5:
	case IDC_STATIC_NEWLOAD_6:
	case IDC_STATIC_NEWLOAD_7:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	break;
	case IDC_EDT_NEWLOAD_WIDTH3:
	case IDC_EDT_NEWLOAD_HEIGHT3:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT);
		pDC->SetBkColor(COLOR_DIALOG_GRAY_BK);
		hbr = hbrBack;
	}
	break;
	case IDC_COMBO_NEWLOAD_NUMBEROFCAM3:
	case IDC_COMBO_NEWLOAD_INPUTSIZE3:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT);
		pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
		hbr = hbrNomalBack;
	}
	break;
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

BOOL CNewLoadConfigChild_3::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CTaskPanelDlg::PreTranslateMessage(pMsg);
}

void CNewLoadConfigChild_3::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	CFont* aria11 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(TAria11);
	CFont* aria13 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria13);

	GetDlgItem(IDC_STATIC_NEWLOAD_3)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_NEWLOAD_4)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_NEWLOAD_5)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_NEWLOAD_6)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_NEWLOAD_7)->SetFont(aria12);

	GetDlgItem(IDC_EDT_NEWLOAD_WIDTH3)->SetFont(aria12);
	GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT3)->SetFont(aria12);

	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM3)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE3)->SetFont(aria12);
}

void CNewLoadConfigChild_3::setCboData()
{
	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM3)->UpdateData(TRUE);
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE3)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboNumOfCam(m_cboNumOfCam3.GetCurSel());
	theApp.m_PrjAllCfg.setCboInputSize(m_cboInputSize3.GetCurSel());
}

void CNewLoadConfigChild_3::resetUI()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM3))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE3))->SetCurSel(6);

	if (m_pNewLoadConfigChild_0)
		m_pNewLoadConfigChild_0->resetUI();
}

void CNewLoadConfigChild_3::OnCbnSelchangeComboNewloadNumberofcam3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM3)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboNumOfCam(m_cboNumOfCam3.GetCurSel());
}


void CNewLoadConfigChild_3::OnCbnSelchangeComboNewloadInputsize3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE3)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboInputSize(m_cboInputSize3.GetCurSel());
}


BOOL CNewLoadConfigChild_3::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, COLOR_DIALOG_BK);

	return TRUE;
	//return CTaskPanelDlg::OnEraseBkgnd(pDC);
}
