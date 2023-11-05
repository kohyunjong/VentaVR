// PanoPreViewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "BottomSourceChildDlg.h"
#include "PanoPreViewDlg.h"
#include "afxdialogex.h"
#include "FormView1.h"

// CPanoPreViewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPanoPreViewDlg, CDialogEx)

CPanoPreViewDlg::CPanoPreViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPanoPreViewDlg::IDD, pParent)
{
	m_pPreView = NULL;
}

CPanoPreViewDlg::~CPanoPreViewDlg()
{
}

void CPanoPreViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPanoPreViewDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPanoPreViewDlg 메시지 처리기입니다.


BOOL CPanoPreViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CXTPSkinManager* pSkinManager = XTPSkinManager();

	CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + SUBSKIN);
	pSkinManager->SetWindowTheme(GetSafeHwnd(), pSchema);
	CMDTARGET_RELEASE(pSchema);

	CRect rc;
	GetClientRect(&rc);

	CCreateContext cc;
//임시로 막음 현재 Gl2개 사용 불가.
// 	CView* pView = (CView*)RUNTIME_CLASS(CFormView1)->CreateObject();
// 	ZeroMemory(&cc, sizeof(cc));
// 	pView->Create(nullptr, nullptr, WS_CHILD, rc, this, IDD_PANORAMAPREVIEW, &cc);
// 	pView->OnInitialUpdate();
// 	m_pPreView = pView;
// 	m_pPreView->ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPanoPreViewDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (m_pPreView)
	{
		if (bShow)
			m_pPreView->ShowWindow(SW_SHOW);
		else
			m_pPreView->ShowWindow(SW_HIDE);
	}
}


void CPanoPreViewDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (theApp.m_ScreenManager.m_pBottomSourceChild)
	{
		theApp.m_ScreenManager.m_pBottomSourceChild->m_btnPanoPrevie.SetImage(CGdipButton::STD_TYPE);
		theApp.m_ScreenManager.m_pBottomSourceChild->m_bisShowPanoPreview = !theApp.m_ScreenManager.m_pBottomSourceChild->m_bisShowPanoPreview;
	}
	theApp.m_ScreenManager.m_pBottomSourceChild->PostMessage(WM_CLOSECOLORDLG, 0, 0);
	CDialogEx::OnClose();
}


BOOL CPanoPreViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
