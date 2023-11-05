// ConfiguChildView1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfiguChildView1.h"
#include "afxdialogex.h"


// CConfiguChildView1 대화 상자입니다.
#define SWITCH_VOD_STREAMING 0

IMPLEMENT_DYNAMIC(CConfiguChildView1, CDialogEx)

CConfiguChildView1::CConfiguChildView1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfiguChildView1::IDD, pParent)
{

}

CConfiguChildView1::~CConfiguChildView1()
{
}

void CConfiguChildView1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfiguChildView1, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CConfiguChildView1 메시지 처리기입니다.


BOOL CConfiguChildView1::OnInitDialog()
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
// 	pGColor->clrClient.SetStandardValue(COLOR_DIALOG_BK);
// 	pGColor->clrClient.clrLight.SetCustomValue(COLOR_DIALOG_BK);
// 	pGColor->clrClient.clrDark.SetCustomValue(COLOR_DIALOG_BK);

	CXTPSkinManager* pSkinManager = XTPSkinManager();
	CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + SUBSKIN);
	pSkinManager->SetWindowTheme(GetSafeHwnd(), pSchema);
	CMDTARGET_RELEASE(pSchema);


	CXTPTaskPanelGroup* pGroup = NULL;

	// 	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("HDD Configuration"));

	m_dlgsubHDDConf.Create(IDD_CONFIGCHILDVIEW1_1_DLG, &m_wndTaskPanel);
	CXTPTaskPanelGroupItem* pItem = pGroup->AddControlItem(m_dlgsubHDDConf);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgsubHDDConf.SetItem(pItem);

	pGroup->SetExpanded(TRUE);
	pGroup->SetExpandable(FALSE);

	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("Streaming Configuration"));

	m_dlgsubRTMPConf.Create(IDD_CONFIGCHILDVIEW1_2_DLG, &m_wndTaskPanel);
	pItem = pGroup->AddControlItem(m_dlgsubRTMPConf);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgsubRTMPConf.SetItem(pItem);

	pGroup->SetExpanded(TRUE);
	pGroup->SetExpandable(FALSE);

#if SWITCH_VOD_STREAMING
	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("VOD Streaming Configuration"));

	m_dlgsubVodStreamConf.Create(IDD_CONFIGCHILDVIEW1_4_DLG, &m_wndTaskPanel);
	pItem = pGroup->AddControlItem(m_dlgsubVodStreamConf);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgsubVodStreamConf.SetItem(pItem);

	pGroup->SetExpanded(TRUE);
#endif 

	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("Black Magic Output Configuration"));

	m_dlgsubBlackMConf.Create(IDD_CONFIGCHILDVIEW1_3_DLG, &m_wndTaskPanel);
	pItem = pGroup->AddControlItem(m_dlgsubBlackMConf);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgsubBlackMConf.SetItem(pItem);

	pGroup->SetExpanded(TRUE);
	pGroup->SetExpandable(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CConfiguChildView1::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_wndTaskPanel)
		m_wndTaskPanel.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
}




void CConfiguChildView1::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		/*theApp.m_PrjAllCfg.m_HddOptions = QVS_Engine::QVS_GetHddOptions();
		theApp.m_PrjAllCfg.m_RtmpOption = QVS_Engine::QVS_GetRtmpOptions();
		m_dlgsubHDDConf.SetValues();
		m_dlgsubRTMPConf.SetValues();
		m_dlgsubBlackMConf.SetValues();*/
	}
}


BOOL CConfiguChildView1::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
}

BOOL CConfiguChildView1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE ) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CConfiguChildView1::setValue()
{
	m_dlgsubHDDConf.SetValues();
	m_dlgsubRTMPConf.SetValues();
	m_dlgsubBlackMConf.SetValues();
}