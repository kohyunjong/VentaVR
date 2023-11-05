// NewLoadConfigChild_1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "NewLoadConfigChild_1.h"
#include "afxdialogex.h"


// CNewLoadConfigChild_1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewLoadConfigChild_1, CDialogEx)

CNewLoadConfigChild_1::CNewLoadConfigChild_1(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{
	m_pNewLoadConfigChild_0 = NULL;
}

CNewLoadConfigChild_1::~CNewLoadConfigChild_1()
{
	if (m_pNewLoadConfigChild_0 != NULL)
		delete m_pNewLoadConfigChild_0;
}

void CNewLoadConfigChild_1::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_NUMBEROFCAM1, m_cboNumOfCam1);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_INPUTSIZE1, m_cboInputSize1);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_OUTPUTSIZE1, m_cboOutputSize1);
}


BEGIN_MESSAGE_MAP(CNewLoadConfigChild_1, CTaskPanelDlg)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_NUMBEROFCAM1, &CNewLoadConfigChild_1::OnCbnSelchangeComboNewloadNumberofcam1)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_INPUTSIZE1, &CNewLoadConfigChild_1::OnCbnSelchangeComboNewloadInputsize1)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_OUTPUTSIZE1, &CNewLoadConfigChild_1::OnCbnSelchangeComboNewloadOutputsize1)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CNewLoadConfigChild_1 메시지 처리기입니다.
BOOL CNewLoadConfigChild_1::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (m_pNewLoadConfigChild_0 != NULL)
		delete m_pNewLoadConfigChild_0;

	m_pNewLoadConfigChild_0 = new CNewLoadConfigChild_0;
	m_pNewLoadConfigChild_0->Create(CNewLoadConfigChild_0::IDD, this);
	m_pNewLoadConfigChild_0->ShowWindow(SW_SHOW);
	
	resetUI();
	SetFontObj();

	CRect clientRect, rc;
	GetClientRect(clientRect);
	m_pNewLoadConfigChild_0->GetClientRect(rc);
	m_pNewLoadConfigChild_0->MoveWindow(0, clientRect.bottom, rc.Width(), rc.Height());
	this->MoveWindow(0, 0, clientRect.Width(), clientRect.Height() + rc.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH CNewLoadConfigChild_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	case IDC_COMBO_NEWLOAD_NUMBEROFCAM1:
	case IDC_COMBO_NEWLOAD_INPUTSIZE1:
	case IDC_COMBO_NEWLOAD_OUTPUTSIZE1:
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

BOOL CNewLoadConfigChild_1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CTaskPanelDlg::PreTranslateMessage(pMsg);
}

void CNewLoadConfigChild_1::SetFontObj()
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

	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM1)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE1)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_NEWLOAD_OUTPUTSIZE1)->SetFont(aria12);
}

void CNewLoadConfigChild_1::setCboData()
{
	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM1)->UpdateData(TRUE);
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE1)->UpdateData(TRUE);
	GetDlgItem(IDC_COMBO_NEWLOAD_OUTPUTSIZE1)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboNumOfCam(m_cboNumOfCam1.GetCurSel());
	theApp.m_PrjAllCfg.setCboInputSize(m_cboInputSize1.GetCurSel());
	theApp.m_PrjAllCfg.setCboOutputSize(m_cboOutputSize1.GetCurSel());
}

void CNewLoadConfigChild_1::resetUI()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM1))->SetCurSel(5);
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE1))->SetCurSel(2);
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_OUTPUTSIZE1))->SetCurSel(2);

	if (m_pNewLoadConfigChild_0)
		m_pNewLoadConfigChild_0->resetUI();
}

void CNewLoadConfigChild_1::OnCbnSelchangeComboNewloadNumberofcam1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM1)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboNumOfCam(m_cboNumOfCam1.GetCurSel());
}


void CNewLoadConfigChild_1::OnCbnSelchangeComboNewloadInputsize1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE1)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboInputSize(m_cboInputSize1.GetCurSel());
}


void CNewLoadConfigChild_1::OnCbnSelchangeComboNewloadOutputsize1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_OUTPUTSIZE1)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboOutputSize(m_cboOutputSize1.GetCurSel());
}


BOOL CNewLoadConfigChild_1::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, COLOR_DIALOG_BK);

	return TRUE;
	//return CTaskPanelDlg::OnEraseBkgnd(pDC);
}
