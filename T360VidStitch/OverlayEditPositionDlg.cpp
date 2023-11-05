// OverlayEditPositionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "OverlayEditPositionDlg.h"
#include "afxdialogex.h"


// COverlayEditPositionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COverlayEditPositionDlg, CDialogEx)

COverlayEditPositionDlg::COverlayEditPositionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COverlayEditPositionDlg::IDD, pParent)
{

}

COverlayEditPositionDlg::~COverlayEditPositionDlg()
{
}

void COverlayEditPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_EDITPOS_APPLY, m_btnApply);
}


BEGIN_MESSAGE_MAP(COverlayEditPositionDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_FOV, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposFov)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_YAW, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposYaw)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_ROLL, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposRoll)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_PITCH, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposPitch)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposDiffyaw)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposDiffpitch)
	ON_BN_CLICKED(IDC_BTN_EDITPOS_APPLY, &COverlayEditPositionDlg::OnBnClickedBtnEditposApply)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// COverlayEditPositionDlg 메시지 처리기입니다.
BOOL COverlayEditPositionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_btnApply.LoadImages(IDB_BTN_APPLY_S_NORMAL, 0, IDB_BTN_APPLY_S_MOVER, IDB_BTN_APPLY_S_PRESSED, 0, 0, _T("PNG"));
	m_btnApply.SizeToContent();

	((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH))->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL COverlayEditPositionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void COverlayEditPositionDlg::SetValues(int overlayid)
{
	if (overlayid < 0) //초기화
	{
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->is_overlay_selected = false;

		id = -1;

		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_FOV, _T(""));
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_YAW, _T(""));
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_ROLL, _T(""));
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_PITCH, _T(""));

		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, _T(""));
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, _T(""));
	}
	else
	{
		id = overlayid;
		double fov = 0.0, yaw = 0.0, roll = 0.0, pitch = 0.0, diff_yaw = 0.0, diff_pitch = 0.0;

		//printf(">>>>> (id, fov, yaw, roll, pitch, diff_yaw, diff_pitch) = (%d, %lf, %lf, %lf, %lf, %lf, %lf)\n", id, fov, yaw, roll, pitch, diff_yaw, diff_pitch);

		CString temp;
		temp.Format(_T("%.2lf"), fov);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_FOV, temp);

		temp.Format(_T("%.2lf"), yaw);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_YAW, temp);

		temp.Format(_T("%.2lf"), roll);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_ROLL, temp);

		temp.Format(_T("%.2lf"), pitch);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_PITCH, temp);

		temp.Format(_T("%.2lf"), diff_yaw);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, temp);

		temp.Format(_T("%.2lf"), diff_pitch);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, temp);
	}
}

void COverlayEditPositionDlg::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_STATIC_EDITPOS)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_FOV)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_YAW)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_ROLL)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_PITCH)->SetFont(aria12);

	GetDlgItem(IDC_STATIC_EDITPOS2)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_DIFFYAW)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_DIFFPITCH)->SetFont(aria12);
}

double COverlayEditPositionDlg::OnChangeStrToDouble(CString strValue, int id)
{
	for (int i = 0; i < strValue.GetLength(); i++)
	{
		if (!((48 <= strValue.GetAt(i) && strValue.GetAt(i) <= 57) || '.' == strValue.GetAt(i) || '-' == strValue.GetAt(i)))	//숫자 ,  '.', '-' 인지 확인
		{
			//  0~ 9 , '.'  이외 다른문자 입력
			if (strValue.GetLength() > 10)	// 문자가 한번에 많이 들어오면 초기화 
				strValue = _T("");
			else
				strValue.Delete(i, 1);

			SetDlgItemText(id, strValue);
			((CEdit*)GetDlgItem(id))->SetSel(-1, -1);
		}
	}
	return _wtof(strValue);
}

HBRUSH COverlayEditPositionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_EDITPOS:
	case IDC_STATIC_EDITPOS_FOV:
	case IDC_STATIC_EDITPOS_YAW:
	case IDC_STATIC_EDITPOS_ROLL:
	case IDC_STATIC_EDITPOS_PITCH:
	case IDC_STATIC_EDITPOS2:
	case IDC_STATIC_EDITPOS_DIFFYAW:
	case IDC_STATIC_EDITPOS_DIFFPITCH:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	break;
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL COverlayEditPositionDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposFov()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_FOV, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_FOV);		//나중에 리턴값을 FPS 맴버변수에 넣을것
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposYaw()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_YAW, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_YAW);		//나중에 리턴값을 FPS 맴버변수에 넣을것
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposRoll()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_ROLL, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_ROLL);		//나중에 리턴값을 FPS 맴버변수에 넣을것
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposPitch()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_PITCH, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_PITCH);		//나중에 리턴값을 FPS 맴버변수에 넣을것
}

void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposDiffyaw()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW);		//나중에 리턴값을 FPS 맴버변수에 넣을것
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposDiffpitch()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH);		//나중에 리턴값을 FPS 맴버변수에 넣을것
}

void COverlayEditPositionDlg::OnBnClickedBtnEditposApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (id >= 0)
	{
		double fov = 0.0, yaw = 0.0, roll = 0.0, pitch = 0.0, diff_yaw = 0.0, diff_pitch = 0.0;

		CString temp;
		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_FOV, temp);
		fov = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_YAW, temp);
		yaw = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_ROLL, temp);
		roll = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_PITCH, temp);
		pitch = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, temp);
		diff_yaw = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, temp);
		diff_pitch = _wtof(temp);

	}
}


void COverlayEditPositionDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		SetValues(-1);

		if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
		{
			((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW))->EnableWindow(TRUE);
			((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH))->EnableWindow(TRUE);
		}
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH))->EnableWindow(FALSE);
	}
}