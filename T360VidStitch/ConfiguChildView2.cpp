// ConfiguChildView2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfiguChildView2.h"
#include "afxdialogex.h"


// CConfiguChildView2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConfiguChildView2, CDialogEx)

CConfiguChildView2::CConfiguChildView2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfiguChildView2::IDD, pParent)
{

}

CConfiguChildView2::~CConfiguChildView2()
{
}

void CConfiguChildView2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfiguChildView2, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CConfiguChildView2 메시지 처리기입니다.
BOOL CConfiguChildView2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CXTPClientRect rc(this);
	m_wndTaskPanel.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP, rc, this, 0);

	m_wndTaskPanel.SetTheme(xtpTaskPanelThemeOffice2000);
	//m_wndTaskPanel.NavigateItems(TRUE);
	m_wndTaskPanel.EnableAnimation(FALSE);

	CXTPTaskPanelPaintManager* pPaintM = m_wndTaskPanel.GetPaintManager();
	XTP_TASKPANEL_GROUPCOLORS* pGColor = pPaintM->GetGroupColors(FALSE);
	pGColor->clrHead.clrLight.SetCustomValue(COLOR_DIALOG_BK);
	pGColor->clrHead.clrDark.SetCustomValue(COLOR_DIALOG_BK);

	CXTPSkinManager* pSkinManager = XTPSkinManager();
	CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + SUBSKIN);
	pSkinManager->SetWindowTheme(GetSafeHwnd(), pSchema);
	CMDTARGET_RELEASE(pSchema);

	CXTPTaskPanelGroup* pGroup = NULL;

	// 	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	pGroup = m_wndTaskPanel.AddGroup(IDD_CONFIGCHILDVIEW2_1_DLG);
	pGroup->SetCaption(_T("Resolution"));

	m_dlgsubResolConf.Create(IDD_CONFIGCHILDVIEW2_1_DLG, &m_wndTaskPanel);
	CXTPTaskPanelGroupItem* pItem = pGroup->AddControlItem(m_dlgsubResolConf);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgsubResolConf.SetItem(pItem);

	pGroup->SetExpanded(TRUE);
	pGroup->SetExpandable(FALSE);

	pGroup = m_wndTaskPanel.AddGroup(IDD_CONFIGCHILDVIEW2_2_DLG);
	pGroup->SetCaption(_T("Rendering"));

	m_dlgsubRenderConf.Create(IDD_CONFIGCHILDVIEW2_2_DLG, &m_wndTaskPanel);
	pItem = pGroup->AddControlItem(m_dlgsubRenderConf);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgsubRenderConf.SetItem(pItem);

	pGroup->SetExpanded(TRUE);
	pGroup->SetExpandable(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CConfiguChildView2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CConfiguChildView2::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		//SetValues();
	}
}

BOOL CConfiguChildView2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}

void CConfiguChildView2::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_wndTaskPanel)
		m_wndTaskPanel.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
}


void CConfiguChildView2::SetValues()
{
	m_dlgsubResolConf.SetValues();
	m_dlgsubRenderConf.SetValues();
}
