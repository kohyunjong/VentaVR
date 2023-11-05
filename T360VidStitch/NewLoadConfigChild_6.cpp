// NewLoadConfigChild_6.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "NewLoadConfigChild_6.h"
#include "afxdialogex.h"


// CNewLoadConfigChild_6 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewLoadConfigChild_6, CDialogEx)

CNewLoadConfigChild_6::CNewLoadConfigChild_6(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{
	m_pNewLoadConfigChild_0 = NULL;
}

CNewLoadConfigChild_6::~CNewLoadConfigChild_6()
{
	if (m_pNewLoadConfigChild_0 != NULL)
		delete m_pNewLoadConfigChild_0;
}

void CNewLoadConfigChild_6::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_NUMBEROFCAM6, m_cboNumOfCam6);
	DDX_Control(pDX, IDC_COMBO_NEWLOAD_INPUTSIZE6, m_cboInputSize6);
	DDX_Control(pDX, IDC_EDT_NEWLOAD_WIDTH6, m_editOutputSizeW6);
	DDX_Control(pDX, IDC_EDT_NEWLOAD_HEIGHT6, m_editOutputSizeH6);
}


BEGIN_MESSAGE_MAP(CNewLoadConfigChild_6, CTaskPanelDlg)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_NUMBEROFCAM6, &CNewLoadConfigChild_6::OnCbnSelchangeComboNewloadNumberofcam6)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWLOAD_INPUTSIZE6, &CNewLoadConfigChild_6::OnCbnSelchangeComboNewloadInputsize6)
	ON_EN_CHANGE(IDC_EDT_NEWLOAD_WIDTH6, &CNewLoadConfigChild_6::OnEnChangeEdtNewloadWidth6)
	ON_EN_CHANGE(IDC_EDT_NEWLOAD_HEIGHT6, &CNewLoadConfigChild_6::OnEnChangeEdtNewloadHeight6)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CNewLoadConfigChild_6 메시지 처리기입니다.
BOOL CNewLoadConfigChild_6::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
#if(LICENSE_AVM)
	((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6))->SetWindowTextW(_T("3840"));
#else
	((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6))->SetWindowTextW(_T("7680"));
#endif
	((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6))->EnableWindow(FALSE);

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

BOOL CNewLoadConfigChild_6::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CTaskPanelDlg::PreTranslateMessage(pMsg);
}

HBRUSH CNewLoadConfigChild_6::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	case IDC_COMBO_NEWLOAD_NUMBEROFCAM6:
	case IDC_COMBO_NEWLOAD_INPUTSIZE6:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT);
		pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
		hbr = hbrNomalBack;
	}
	case IDC_EDT_NEWLOAD_WIDTH6:
	case IDC_EDT_NEWLOAD_HEIGHT6:
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

void CNewLoadConfigChild_6::SetFontObj()
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

	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM6)->SetFont(aria12);
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE6)->SetFont(aria12);

	GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6)->SetFont(aria12);
	GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT6)->SetFont(aria12);
}

void CNewLoadConfigChild_6::setCboData()
{
	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM6)->UpdateData(TRUE);
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE6)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboNumOfCam(m_cboNumOfCam6.GetCurSel());
	theApp.m_PrjAllCfg.setCboInputSize(m_cboInputSize6.GetCurSel());
}

void CNewLoadConfigChild_6::setEditData()
{
	GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6)->UpdateData(TRUE);
	GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT6)->UpdateData(TRUE);

	CString szTemp;
	((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6))->GetWindowTextW(szTemp);
	theApp.m_PrjAllCfg.m_nDstWidth = _ttoi(szTemp);
	((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT6))->GetWindowTextW(szTemp);
	theApp.m_PrjAllCfg.m_nDstHeight = _ttoi(szTemp);
}

void CNewLoadConfigChild_6::getEditData(int &nWidth, int &nHeight)
{
	GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6)->UpdateData(TRUE);
	GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT6)->UpdateData(TRUE);

	CString szTemp;
	((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6))->GetWindowTextW(szTemp);
	nWidth = _ttoi(szTemp);
	((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT6))->GetWindowTextW(szTemp);
	nHeight = _ttoi(szTemp);
}

void CNewLoadConfigChild_6::resetUI()
{
#if(LICENSE_AVM)
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM6))->SetCurSel(2);
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE6))->SetCurSel(3);

	((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT6))->SetWindowTextW(_T("640"));
#else
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM6))->SetCurSel(3);
	((CComboBox*)GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE6))->SetCurSel(6);

	((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT6))->SetWindowTextW(_T("2240"));
#endif

	if (m_pNewLoadConfigChild_0)
		m_pNewLoadConfigChild_0->resetUI();
}

void CNewLoadConfigChild_6::setWideMode(bool isSetting)
{
	isSettingWide = isSetting;
}

void CNewLoadConfigChild_6::OnCbnSelchangeComboNewloadNumberofcam6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_NUMBEROFCAM6)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboNumOfCam(m_cboNumOfCam6.GetCurSel());
}


void CNewLoadConfigChild_6::OnCbnSelchangeComboNewloadInputsize6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_NEWLOAD_INPUTSIZE6)->UpdateData(TRUE);
	theApp.m_PrjAllCfg.setCboInputSize(m_cboInputSize6.GetCurSel());
}


void CNewLoadConfigChild_6::OnEnChangeEdtNewloadWidth6()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTaskPanelDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6)->UpdateData(TRUE);

	if (isSettingWide)
	{
		CString szTemp;
		((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_WIDTH6))->GetWindowTextW(szTemp);
		theApp.m_PrjAllCfg.m_nDstWidth = _ttoi(szTemp);
	}
}


void CNewLoadConfigChild_6::OnEnChangeEdtNewloadHeight6()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTaskPanelDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT6)->UpdateData(TRUE);

	if (isSettingWide)
	{
		CString szTemp;
		((CEdit*)GetDlgItem(IDC_EDT_NEWLOAD_HEIGHT6))->GetWindowTextW(szTemp);
		theApp.m_PrjAllCfg.m_nDstHeight = _ttoi(szTemp);
	}
}


BOOL CNewLoadConfigChild_6::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, COLOR_DIALOG_BK);

	return TRUE;
}