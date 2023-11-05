// NewLoadConfigChild_7.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "NewLoadConfigChild_7.h"
#include "afxdialogex.h"


// CNewLoadConfigChild_7 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewLoadConfigChild_7, CDialogEx)

CNewLoadConfigChild_7::CNewLoadConfigChild_7(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{
	m_pNewLoadConfigChild_0 = NULL;
}

CNewLoadConfigChild_7::~CNewLoadConfigChild_7()
{
	if (m_pNewLoadConfigChild_0 != NULL)
		delete m_pNewLoadConfigChild_0;
}

void CNewLoadConfigChild_7::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_NUMBEROFCAM7, m_cboNumOfCam7);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_INPUTSIZE7, m_cboInputSize7);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_OUTPUTSIZE7, m_cboOutputSize7);
}


BEGIN_MESSAGE_MAP(CNewLoadConfigChild_7, CTaskPanelDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_NUMBEROFCAM7, &CNewLoadConfigChild_7::OnCbnSelchangeComboNewloadNumberofcam7)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_INPUTSIZE7, &CNewLoadConfigChild_7::OnCbnSelchangeComboNewloadInputsize7)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_OUTPUTSIZE7, &CNewLoadConfigChild_7::OnCbnSelchangeComboNewloadOutputsize7)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CNewLoadConfigChild_7 메시지 처리기입니다.
BOOL CNewLoadConfigChild_7::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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

BOOL CNewLoadConfigChild_7::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CTaskPanelDlg::PreTranslateMessage(pMsg);
}

void CNewLoadConfigChild_7::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	CFont* aria11 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(TAria11);
	CFont* aria13 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria13);

	GetDlgItem(IDC_STATIC_NEWLOAD_3)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_NEWLOAD_4)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_NEWLOAD_5)->SetFont(aria12);

	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM7)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE7)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_NEWLOAD_OUTPUTSIZE7)->SetFont(aria12);
}

void CNewLoadConfigChild_7::setCboData()
{
	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM7)->UpdateData(TRUE);
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE7)->UpdateData(TRUE);
	GetDlgItem(IDC_COMBO_NEWLOAD_OUTPUTSIZE7)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboNumOfCam(m_cboNumOfCam7.GetCurSel());
	theApp.m_PrjAllCfg.setCboInputSize(m_cboInputSize7.GetCurSel());
	theApp.m_PrjAllCfg.setCboOutputSize(m_cboOutputSize7.GetCurSel());
}

void CNewLoadConfigChild_7::resetUI()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM7))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE7))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_OUTPUTSIZE7))->SetCurSel(0);

	if (m_pNewLoadConfigChild_0)
		m_pNewLoadConfigChild_0->resetUI();
}

void CNewLoadConfigChild_7::OnCbnSelchangeComboNewloadNumberofcam7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM7)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboNumOfCam(m_cboNumOfCam7.GetCurSel());
}


void CNewLoadConfigChild_7::OnCbnSelchangeComboNewloadInputsize7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE7)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboInputSize(m_cboInputSize7.GetCurSel());
}


void CNewLoadConfigChild_7::OnCbnSelchangeComboNewloadOutputsize7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_OUTPUTSIZE7)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboOutputSize(m_cboOutputSize7.GetCurSel());
}


HBRUSH CNewLoadConfigChild_7::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	break;
	case IDC_COMBO_NEWLOAD_NUMBEROFCAM7:
	case IDC_COMBO_NEWLOAD_INPUTSIZE7:
	case IDC_COMBO_NEWLOAD_OUTPUTSIZE7:
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


BOOL CNewLoadConfigChild_7::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, COLOR_DIALOG_BK);

	return TRUE;
	//return CTaskPanelDlg::OnEraseBkgnd(pDC);
}
