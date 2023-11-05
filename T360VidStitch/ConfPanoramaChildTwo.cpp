// ConfPanoramaChildTwo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfPanoramaChildTwo.h"
#include "afxdialogex.h"


// CConfPanoramaChildTwo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConfPanoramaChildTwo, CTaskPanelDlg)

CConfPanoramaChildTwo::CConfPanoramaChildTwo(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfPanoramaChildTwo::~CConfPanoramaChildTwo()
{
}

void CConfPanoramaChildTwo::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_2_2_SAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CConfPanoramaChildTwo, CTaskPanelDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG_2_2_SAVE, &CConfPanoramaChildTwo::OnBnClickedBtnConfig22Save)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_CFPANO_2_EDITRENDER, &CConfPanoramaChildTwo::OnEnChangeCfpano2Editrender)
END_MESSAGE_MAP()


// CConfPanoramaChildTwo 메시지 처리기입니다.
BOOL CConfPanoramaChildTwo::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();

	//SetDlgItemTextW(IDC_CFPANO_2_EDITRENDER, _T("60.0"));

	SetFontObj();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CConfPanoramaChildTwo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CTaskPanelDlg::PreTranslateMessage(pMsg);
}


void CConfPanoramaChildTwo::OnBnClickedBtnConfig22Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString editValue;
	GetDlgItemText(IDC_CFPANO_2_EDITRENDER, editValue);

	double render_fps = _wtof(editValue);

	if (render_fps > 60.00){
		render_fps = 60.00;
	}
	else if (render_fps < 1.00){
		render_fps = 1.00;
	}

	editValue.Format(_T("%.2lf"), render_fps);
	SetDlgItemText(IDC_CFPANO_2_EDITRENDER, editValue);

	QVS_Engine::QVS_setRenderFPS(render_fps);
}


void CConfPanoramaChildTwo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTaskPanelDlg::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


HBRUSH CConfPanoramaChildTwo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_12_53:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	break;
	case IDC_CFPANO_2_EDITRENDER:
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


BOOL CConfPanoramaChildTwo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}

void CConfPanoramaChildTwo::SetValues()
{
	double render_fps = QVS_Engine::QVS_getRenderFPS();
	if (render_fps < 1.0)
		render_fps = 1.0;
	else if (render_fps > 60.0)
		render_fps = 60.0;

	CString temp;
	temp.Format(_T("%.2lf"), render_fps);
	SetDlgItemText(IDC_CFPANO_2_EDITRENDER, temp);
}

void CConfPanoramaChildTwo::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	//GetDlgItem(IDC_CFPANO_1_HEIGHT)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_53)->SetFont(aria12);
	GetDlgItem(IDC_CFPANO_2_EDITRENDER)->SetFont(aria12);
}

double CConfPanoramaChildTwo::OnChangeStrToDouble(CString strValue)
{
	for (int i = 0; i < strValue.GetLength(); i++)
	{
		if (!((48 <= strValue.GetAt(i) && strValue.GetAt(i) <= 57) || '.' == strValue.GetAt(i)))	//숫자 ,  '.' 인지 확인
		{
			//  0~ 9 , '.'  이외 다른문자 입력
			if (strValue.GetLength() > 10)	// 문자가 한번에 많이 들어오면 초기화 
				strValue = _T("");
			else
				strValue.Delete(i, 1);

			SetDlgItemText(IDC_CFPANO_2_EDITRENDER, strValue);
			((CEdit*)GetDlgItem(IDC_CFPANO_2_EDITRENDER))->SetSel(-1, -1);
		}
	}
	return _wtof(strValue);
}

void CConfPanoramaChildTwo::OnEnChangeCfpano2Editrender()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTaskPanelDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString editValue;
	GetDlgItemText(IDC_CFPANO_2_EDITRENDER, editValue);
	OnChangeStrToDouble(editValue);		//나중에 리턴값을 FPS 맴버변수에 넣을것
}
