// RightInteracChildDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightInteracChildDlg.h"
#include "afxdialogex.h"


// CRightInteracChildDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRightInteracChildDlg, CDialogEx)

CRightInteracChildDlg::CRightInteracChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRightInteracChildDlg::IDD, pParent)
{

}

CRightInteracChildDlg::~CRightInteracChildDlg()
{
}

void CRightInteracChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNPLAY, m_btnPlay);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_sldSpeed);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_szSpeed);
	DDX_Control(pDX, IDC_EDT_SPEED, m_txtSpeed);
}


BEGIN_MESSAGE_MAP(CRightInteracChildDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTNPLAY, &CRightInteracChildDlg::OnBnClickedBtnplay)
END_MESSAGE_MAP()


// CRightInteracChildDlg 메시지 처리기입니다.


void CRightInteracChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow) {
		m_btnPlay.LoadImages(IDB_EXPORT_BTN_PLAY_NORMAL, IDB_EXPORT_BTN_PLAY_SELECTED, IDB_EXPORT_BTN_PLAY_MOVER, IDB_EXPORT_BTN_PLAY_PRESSED, IDB_EXPORT_BTN_PLAY_SELECTED_MOVER, IDB_EXPORT_BTN_PLAY_SELECTED_PRESSED, _T("PNG"));
		m_btnPlay.SizeToContent();
		//m_btnPlay.SetImage(CGdipButton::STD_TYPE);

		m_sldSpeed.SetRange(1, 10);
		m_sldSpeed.SetTicFreq(1);

		CString szTemp;
		int nPos = m_sldSpeed.GetPos();
		szTemp.Format(_T("%d"), nPos == 0 ? 0 : nPos);
		m_txtSpeed.SetWindowTextW(szTemp);

		/*((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->interactive_animation_mode = ANIMATION_MODE_NONE;
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->animation_speed = 0;
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->anim_timer = 0;*/

		//m_sldSpeed.ShowWindow(SW_HIDE);
		//m_szSpeed.ShowWindow(SW_HIDE);
		//m_txtSpeed.ShowWindow(SW_HIDE);

		PlayState(((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->interactive_animation_mode);
	}
}

void CRightInteracChildDlg::PlayState(int mode)
{
	int nPos = m_sldSpeed.GetPos();

	if (mode == ANIMATION_MODE_ROTATE)
	{
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->animation_speed = 0.05 + 0.02 * (nPos - 1);
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->anim_timer = 0;

		m_sldSpeed.ShowWindow(SW_SHOW);
		m_szSpeed.ShowWindow(SW_SHOW);
		m_txtSpeed.ShowWindow(SW_SHOW);

		m_btnPlay.SetImage(CGdipButton::ALT_TYPE);
		printf("Start DrawInteractive Animation\n");
	}
	else if (mode == ANIMATION_MODE_NONE)
	{
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->animation_speed = 0;
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->anim_timer = 0;

		m_sldSpeed.ShowWindow(SW_HIDE);
		m_szSpeed.ShowWindow(SW_HIDE);
		m_txtSpeed.ShowWindow(SW_HIDE);

		m_btnPlay.SetImage(CGdipButton::STD_TYPE);
		printf("Stop DrawInteractive Animation\n");
	}
}

void CRightInteracChildDlg::OnBnClickedBtnplay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->interactive_animation_mode == ANIMATION_MODE_NONE)
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->interactive_animation_mode = ANIMATION_MODE_ROTATE;
	else if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->interactive_animation_mode == ANIMATION_MODE_ROTATE)
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->interactive_animation_mode = ANIMATION_MODE_NONE;

	PlayState(((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->interactive_animation_mode);
}

void CRightInteracChildDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString szTemp;
	if (pScrollBar)
	{
		if (pScrollBar == (CScrollBar*)&m_sldSpeed)
		{
			int nPos = m_sldSpeed.GetPos();
			szTemp.Format(_T("%d"), nPos == 0 ? 0 : nPos);
			m_txtSpeed.SetWindowTextW(szTemp);

			if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->interactive_animation_mode == ANIMATION_MODE_ROTATE)
			{
				((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->animation_speed = 0.05 + 0.02 * (nPos - 1);
			}
		}
	}
	else
	{
		// 
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

float CRightInteracChildDlg::PlaySpeed(int mode, float speed)
{
	CString szTemp;
	int nPos = m_sldSpeed.GetPos();

	if (mode == ANIMATION_SPEED_UP)
	{
		speed += 0.02;
		nPos += 1;

		szTemp.Format(_T("%d"), nPos == 0 ? 0 : nPos);
		m_txtSpeed.SetWindowTextW(szTemp);
		m_sldSpeed.SetPos(nPos);

		if (nPos > 10)
		{
			speed = 0.23;

			m_txtSpeed.SetWindowTextW(_T("10"));
			m_sldSpeed.SetPos(10);
		}

		return speed;
	}

	else if (mode == ANIMATION_SPEED_DOWN)
	{
		speed -= 0.02;
		nPos -= 1;

		szTemp.Format(_T("%d"), nPos == 0 ? 0 : nPos);
		m_txtSpeed.SetWindowTextW(szTemp);
		m_sldSpeed.SetPos(nPos);

		if (nPos < 1)
		{
			speed = 0.05;

			m_txtSpeed.SetWindowTextW(_T("1"));
			m_sldSpeed.SetPos(1);
		}

		return speed;
	}
}

BOOL CRightInteracChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}