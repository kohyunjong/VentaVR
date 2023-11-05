// NewLoadConfigChild.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "NewLoadConfigChild.h"
#include "afxdialogex.h"

#define TASKPANEL_GROUP_UNEXPANDABLE 1
#define TASKPANEL_GROUP_STARTID IDD_NEWLOAD_CONFIG_CHILD_1
#define TASKPANEL_GROUP_ENDID IDD_NEWLOAD_CONFIG_CHILD_6

// CNewLoadConfigChild 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewLoadConfigChild, CDialogEx)

CNewLoadConfigChild::CNewLoadConfigChild(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewLoadConfigChild::IDD, pParent)
{
}

CNewLoadConfigChild::~CNewLoadConfigChild()
{
}

void CNewLoadConfigChild::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewLoadConfigChild, CDialogEx)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, &CNewLoadConfigChild::OnTaskPanelNotify)
END_MESSAGE_MAP()


// CNewLoadConfigChild 메시지 처리기입니다.
BOOL CNewLoadConfigChild::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CXTPClientRect rc(this);
	m_wndTaskPanel.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP, rc, this, 0);

	m_wndTaskPanel.SetTheme(xtpTaskPanelThemeOffice2000);
	//m_wndTaskPanel.SetBehaviour(XTPTaskPanelBehaviour::xtpTaskPanelBehaviourExplorer);
	//m_wndTaskPanel.EnableAnimation(FALSE);

	CXTPTaskPanelPaintManager* pPaintM = m_wndTaskPanel.GetPaintManager();
	XTP_TASKPANEL_GROUPCOLORS* pGColor = pPaintM->GetGroupColors(FALSE);
	pGColor->clrHead.clrLight.SetCustomValue(COLOR_DIALOG_BK);
	pGColor->clrHead.clrDark.SetCustomValue(COLOR_DIALOG_BK);

	LOGFONT lfont;
	::ZeroMemory(&lfont, sizeof(lfont));

	lfont.lfHeight = 16;
	lfont.lfWidth = 0; //auto width using height
	lfont.lfEscapement = 0; //rotate font, 0.1 degree unit in 360
	lfont.lfOrientation = 0; //rotate character
	lfont.lfWeight = FW_SEMIBOLD; //bold
	lfont.lfItalic = 0; //italic, boolean
	lfont.lfUnderline = 0; //underline, boolean
	lfont.lfStrikeOut = 0; //strke out line, boolean
	lfont.lfCharSet = ANSI_CHARSET; //ANSI_CHARSET : for windows, OEM_CHARSET : for dos, HANGEUL_CHARSET : for hangeul
	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfont.lfQuality = DEFAULT_QUALITY;
	lfont.lfPitchAndFamily = DEFAULT_PITCH;
	wcscpy(lfont.lfFaceName, L"Arial"); //font name

	CXTPTaskPanelGroup* pGroup = NULL;

	// 	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//
	CXTPTaskPanelGroupItem*  pItem;

	//
	/*pGroup = m_wndTaskPanel.AddGroup(IDD_NEWLOAD_CONFIG_CHILD_7);
	pGroup->SetCaption(_T("Kandao"));
	pGroup->SetItemFont(&lfont);

	m_pNewLoadConfigChild_7.Create(IDD_NEWLOAD_CONFIG_CHILD_7, &m_wndTaskPanel);
	m_pNewLoadConfigChild_7.SetFontObj();
	pItem = pGroup->AddControlItem(m_pNewLoadConfigChild_7);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	pItem->SetID(IDD_NEWLOAD_CONFIG_CHILD_7);
	m_pNewLoadConfigChild_7.SetItem(pItem);

	pGroup->SetExpanded(FALSE);*/

	//
	pGroup = m_wndTaskPanel.AddGroup(IDD_NEWLOAD_CONFIG_CHILD_6);
	pGroup->SetCaption(_T("Wide Panorama"));
	pGroup->SetItemFont(&lfont);

	m_pNewLoadConfigChild_6.Create(IDD_NEWLOAD_CONFIG_CHILD_6, &m_wndTaskPanel);
	m_pNewLoadConfigChild_6.SetFontObj();
	pItem = pGroup->AddControlItem(m_pNewLoadConfigChild_6);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	pItem->SetID(IDD_NEWLOAD_CONFIG_CHILD_6);
	m_pNewLoadConfigChild_6.SetItem(pItem);

	pGroup->SetExpanded(FALSE);

	//
	pGroup = m_wndTaskPanel.AddGroup(IDD_NEWLOAD_CONFIG_CHILD_5);
	pGroup->SetCaption(_T("4K Extern Input"));
	pGroup->SetItemFont(&lfont);

	m_pNewLoadConfigChild_5.Create(IDD_NEWLOAD_CONFIG_CHILD_5, &m_wndTaskPanel);
	m_pNewLoadConfigChild_5.SetFontObj();
	pItem = pGroup->AddControlItem(m_pNewLoadConfigChild_5);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	pItem->SetID(IDD_NEWLOAD_CONFIG_CHILD_5);
	m_pNewLoadConfigChild_5.SetItem(pItem);

	pGroup->SetExpanded(FALSE);

	//
	pGroup = m_wndTaskPanel.AddGroup(IDD_NEWLOAD_CONFIG_CHILD_4);
	pGroup->SetCaption(_T("Stereo Left/Right"));
	pGroup->SetItemFont(&lfont);

	m_pNewLoadConfigChild_4.Create(IDD_NEWLOAD_CONFIG_CHILD_4, &m_wndTaskPanel);
	m_pNewLoadConfigChild_4.SetFontObj();
	pItem = pGroup->AddControlItem(m_pNewLoadConfigChild_4);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	pItem->SetID(IDD_NEWLOAD_CONFIG_CHILD_4);
	m_pNewLoadConfigChild_4.SetItem(pItem);

	pGroup->SetExpanded(FALSE);

	//
	pGroup = m_wndTaskPanel.AddGroup(IDD_NEWLOAD_CONFIG_CHILD_3);
	pGroup->SetCaption(_T("Stereo Top/Bottom"));
	pGroup->SetItemFont(&lfont);

	m_pNewLoadConfigChild_3.Create(IDD_NEWLOAD_CONFIG_CHILD_3, &m_wndTaskPanel);
	m_pNewLoadConfigChild_3.SetFontObj();
	pItem = pGroup->AddControlItem(m_pNewLoadConfigChild_3);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	pItem->SetID(IDD_NEWLOAD_CONFIG_CHILD_3);
	m_pNewLoadConfigChild_3.SetItem(pItem);

	pGroup->SetExpanded(FALSE);

	//
#if(!LICENSE_AVM)
	pGroup = m_wndTaskPanel.AddGroup(IDD_NEWLOAD_CONFIG_CHILD_2);
	pGroup->SetCaption(_T("360 8K"));
	pGroup->SetItemFont(&lfont);

	m_pNewLoadConfigChild_2.Create(IDD_NEWLOAD_CONFIG_CHILD_2, &m_wndTaskPanel);
	m_pNewLoadConfigChild_2.SetFontObj();
	pItem = pGroup->AddControlItem(m_pNewLoadConfigChild_2);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	pItem->SetID(IDD_NEWLOAD_CONFIG_CHILD_2);
	m_pNewLoadConfigChild_2.SetItem(pItem);

	pGroup->SetExpanded(FALSE);
#endif
	//
	pGroup = m_wndTaskPanel.AddGroup(IDD_NEWLOAD_CONFIG_CHILD_1);
	pGroup->SetCaption(_T("360 4K"));
	pGroup->SetItemFont(&lfont);
	

	m_pNewLoadConfigChild_1.Create(IDD_NEWLOAD_CONFIG_CHILD_1, &m_wndTaskPanel);
	m_pNewLoadConfigChild_1.SetFontObj();
	pItem = pGroup->AddControlItem(m_pNewLoadConfigChild_1);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	pItem->SetID(IDD_NEWLOAD_CONFIG_CHILD_1);
	m_pNewLoadConfigChild_1.SetItem(pItem);

#if(0)
	pGroup->SetExpanded(TRUE);
	//pGroup->SetExpandable(FALSE);
	theApp.m_PrjAllCfg.setNewType((int)CProjAllConfig::NEWTYPE::Q360_4K);
	m_pNewLoadConfigChild_1.setCboData();
#else
	pGroup->SetExpanded(FALSE);
#endif

	//
	/*pGroup = m_wndTaskPanel.AddGroup(IDD_NEWLOAD_CONFIG_CHILD_0);
	pGroup->SetCaption(_T("Default"));
	pGroup->ShowCaption(FALSE);

	m_pNewLoadConfigChild_0.Create(IDD_NEWLOAD_CONFIG_CHILD_0, &m_wndTaskPanel);
	m_pNewLoadConfigChild_0.SetFontObj();
	pItem = pGroup->AddControlItem(m_pNewLoadConfigChild_0);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_pNewLoadConfigChild_0.SetItem(pItem);

	pGroup->SetExpanded(TRUE);
	pGroup->SetExpandable(FALSE);*/

	m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpanded(TRUE);
	//changeGroupUI(IDD_NEWLOAD_CONFIG_CHILD_1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CNewLoadConfigChild::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CNewLoadConfigChild::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_wndTaskPanel)
		m_wndTaskPanel.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
}

BOOL CNewLoadConfigChild::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//return CDialogEx::OnEraseBkgnd(pDC);

	//remove blinking
	return FALSE;
}

LRESULT CNewLoadConfigChild::OnTaskPanelNotify(WPARAM wParam, LPARAM lParam)
{
	CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
	
	switch (wParam)
	{
	case XTP_TPN_GROUPEXPANDED:
	{
		//printf("XTP_TPN_GROUPEXPANDED Event: pItem.Caption = %s, pItem.ID = %i\n", pItem->GetCaption(), pItem->GetIndex());
		int idx = pItem->GetID();
#if(1)
		changeGroupUI(idx);
#else
		//========== item move on top
		if (changeGroupUI(idx))
		{
			CXTPTaskPanelItems* pItems = pItem->GetParentItems();
			pItems->Move(pItem, 0);
		}
		//CXTPTaskPanelItems* pItems = m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1 + i)->GetItems();
#endif
	}
	break;
	default:
		break;
	}
	return 0;
}

bool CNewLoadConfigChild::changeGroupUI(int id)
{
	bool nRes = false;

	switch (id)
	{
	case IDD_NEWLOAD_CONFIG_CHILD_1:
		if (m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->IsExpanded())
		{
			theApp.m_PrjAllCfg.setNewType((int)CProjAllConfig::NEWTYPE::Q360_4K);

#if(TASKPANEL_GROUP_UNEXPANDABLE)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpandable(FALSE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpandable(TRUE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpandable(TRUE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpandable(TRUE);
#endif

#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpanded(FALSE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpanded(FALSE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpanded(FALSE);

			m_pNewLoadConfigChild_1.setCboData();
			nRes = true;
		}
		break;
#if(!LICENSE_AVM)
	case IDD_NEWLOAD_CONFIG_CHILD_2:
		if (m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->IsExpanded())
		{
			theApp.m_PrjAllCfg.setNewType((int)CProjAllConfig::NEWTYPE::Q360_8K);

#if(TASKPANEL_GROUP_UNEXPANDABLE)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpandable(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpandable(TRUE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpandable(TRUE);
#endif

			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpanded(FALSE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpanded(FALSE);

			m_pNewLoadConfigChild_2.setCboData();
			nRes = true;
		}
		break;
#endif
	case IDD_NEWLOAD_CONFIG_CHILD_3:
		if (m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->IsExpanded())
		{
			theApp.m_PrjAllCfg.setNewType((int)CProjAllConfig::NEWTYPE::Stereo_TB);

#if(TASKPANEL_GROUP_UNEXPANDABLE)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpandable(TRUE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpandable(TRUE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpandable(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpandable(TRUE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpandable(TRUE);
#endif

			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpanded(FALSE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpanded(FALSE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpanded(FALSE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpanded(FALSE);

			m_pNewLoadConfigChild_3.setCboData();
			nRes = true;
		}
		break;
	case IDD_NEWLOAD_CONFIG_CHILD_4:
		if (m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->IsExpanded())
		{
			theApp.m_PrjAllCfg.setNewType((int)CProjAllConfig::NEWTYPE::Stereo_LR);

#if(TASKPANEL_GROUP_UNEXPANDABLE)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpandable(TRUE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpandable(TRUE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpandable(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpandable(TRUE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpandable(TRUE);
#endif

			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpanded(FALSE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpanded(FALSE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpanded(FALSE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpanded(FALSE);

			m_pNewLoadConfigChild_4.setCboData();
			nRes = true;
		}
		break;
	case IDD_NEWLOAD_CONFIG_CHILD_5:
		if (m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->IsExpanded())
		{
			theApp.m_PrjAllCfg.setNewType((int)CProjAllConfig::NEWTYPE::Extern_Input);

#if(TASKPANEL_GROUP_UNEXPANDABLE)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpandable(TRUE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpandable(TRUE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpandable(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpandable(TRUE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpandable(TRUE);
#endif

			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpanded(FALSE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpanded(FALSE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpanded(FALSE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpanded(FALSE);

			m_pNewLoadConfigChild_5.setCboData();
			nRes = true;
		}
		break;
	case IDD_NEWLOAD_CONFIG_CHILD_6:
		if (m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->IsExpanded())
		{
			theApp.m_PrjAllCfg.setNewType((int)CProjAllConfig::NEWTYPE::Wide_Panorama);

#if(TASKPANEL_GROUP_UNEXPANDABLE)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpandable(TRUE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpandable(TRUE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpandable(FALSE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpandable(TRUE);
#endif

			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpanded(FALSE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpanded(FALSE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpanded(FALSE);
			//m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpanded(FALSE);

			m_pNewLoadConfigChild_6.setCboData();
			m_pNewLoadConfigChild_6.setEditData();
			nRes = true;
		}
		break;
	/*case IDD_NEWLOAD_CONFIG_CHILD_7:
		if (m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->IsExpanded())
		{
			theApp.m_PrjAllCfg.setNewType((int)CProjAllConfig::NEWTYPE::Q360_4K);

#if(TASKPANEL_GROUP_UNEXPANDABLE)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpandable(TRUE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpandable(TRUE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpandable(TRUE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_7)->SetExpandable(FALSE);
#endif

			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpanded(FALSE);
#if(!LICENSE_AVM)
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_2)->SetExpanded(FALSE);
#endif
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_3)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_4)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_5)->SetExpanded(FALSE);
			m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_6)->SetExpanded(FALSE);

			m_pNewLoadConfigChild_7.setCboData();
			nRes = true;
		}
		break;*/
	default:
		break;
	}

	m_pNewLoadConfigChild_6.setWideMode(theApp.m_PrjAllCfg.getNewType() == CProjAllConfig::NEWTYPE::Wide_Panorama);

	return nRes;
}

void CNewLoadConfigChild::resetUI()
{
	m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpanded(TRUE);
	changeGroupUI(IDD_NEWLOAD_CONFIG_CHILD_1);

	m_pNewLoadConfigChild_1.resetUI();
#if(!LICENSE_AVM)
	m_pNewLoadConfigChild_2.resetUI();
#endif
	m_pNewLoadConfigChild_3.resetUI();
	m_pNewLoadConfigChild_4.resetUI();
	m_pNewLoadConfigChild_5.resetUI();
	m_pNewLoadConfigChild_6.resetUI();
	//m_pNewLoadConfigChild_7.resetUI();
}

void CNewLoadConfigChild::setConfigNumOfCam(int type)
{
	int num = 0;

	switch ((CProjAllConfig::NEWTYPE)type)
	{
	case CProjAllConfig::NEWTYPE::Q360_4K:
	case CProjAllConfig::NEWTYPE::Q360_8K:
	case CProjAllConfig::NEWTYPE::Extern_Input:
	case CProjAllConfig::NEWTYPE::Wide_Panorama:
		num = theApp.m_PrjAllCfg.getCboNumOfCam() + 1;
		break;
	case CProjAllConfig::NEWTYPE::Stereo_TB:
	case CProjAllConfig::NEWTYPE::Stereo_LR:
		num = theApp.m_PrjAllCfg.getCboNumOfCam() + 1;
		num *= 2;
		break;
	case CProjAllConfig::NEWTYPE::None:
		num = 6;
		break;
	}

	theApp.m_PrjAllCfg.m_nMaxCamra = num;
}

void CNewLoadConfigChild::setConfigInputSize(int type)
{
	int nWidth, nHeight;

	switch ((CProjAllConfig::NEWTYPE)type)
	{
	case CProjAllConfig::NEWTYPE::Q360_4K:
	case CProjAllConfig::NEWTYPE::Stereo_TB:
	case CProjAllConfig::NEWTYPE::Stereo_LR:
	case CProjAllConfig::NEWTYPE::Wide_Panorama:
		getInputSize(theApp.m_PrjAllCfg.getCboInputSize(), nWidth, nHeight);
		break;
	case CProjAllConfig::NEWTYPE::Q360_8K:
		getInputSize(theApp.m_PrjAllCfg.getCboInputSize() + 2, nWidth, nHeight);
		break;
	case CProjAllConfig::NEWTYPE::Extern_Input:
		getInputSize(theApp.m_PrjAllCfg.getCboInputSize() + 8, nWidth, nHeight);
		break;
	case CProjAllConfig::NEWTYPE::None:
		nWidth = 1440;
		nHeight = 1080;
		break;
	}

	theApp.m_PrjAllCfg.m_nWidth = nWidth;
	theApp.m_PrjAllCfg.m_nHeight = nHeight;
}

void CNewLoadConfigChild::getInputSize(int type, int &width, int &height)
{
	switch (type)
	{
	case 0:
		width = 1280;
		height = 720;
		break;
	case 1:
		width = 1280;
		height = 960;
		break;
	case 2:
		width = 1440;
		height = 1080;
		break;
	case 3:
		width = 1920;
		height = 1080;
		break;
	case 4:
		width = 2048;
		height = 1080;
		break;
	case 5:
		width = 2048;
		height = 1556;
		break;
	case 6:
		width = 3840;
		height = 2160;
		break;
	case 7:
		width = 4096;
		height = 2160;
		break;
	case 8:
		width = 3840;
		height = 1920;
		break;
	case 9:
		width = 3840;
		height = 2160;
		break;
	}
}

void CNewLoadConfigChild::setConfigOutputSize(int type)
{
	int nWidth = 0, nHeight = 0;

	switch ((CProjAllConfig::NEWTYPE)type)
	{
	case CProjAllConfig::NEWTYPE::Q360_4K:
	case CProjAllConfig::NEWTYPE::Q360_8K:
		getOutputSize(theApp.m_PrjAllCfg.getCboOutputSize(), nWidth, nHeight);
		theApp.m_PrjAllCfg.m_nDstWidth = nWidth;
		theApp.m_PrjAllCfg.m_nDstHeight = nHeight;
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_Normal;
		break;
	case CProjAllConfig::NEWTYPE::Stereo_TB:
		getOutputSize(4, nWidth, nHeight);
		theApp.m_PrjAllCfg.m_nDstWidth = nWidth;
		theApp.m_PrjAllCfg.m_nDstHeight = nHeight;
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_Stereo;
		break;
	case CProjAllConfig::NEWTYPE::Stereo_LR:
		getOutputSize(theApp.m_PrjAllCfg.getCboOutputSize() + 5, nWidth, nHeight);
		theApp.m_PrjAllCfg.m_nDstWidth = nWidth;
		theApp.m_PrjAllCfg.m_nDstHeight = nHeight;
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_Stereo_Side;
		break;
	case CProjAllConfig::NEWTYPE::Extern_Input:
		getOutputSize(2, nWidth, nHeight);
		theApp.m_PrjAllCfg.m_nDstWidth = nWidth;
		theApp.m_PrjAllCfg.m_nDstHeight = nHeight;
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_StitchedInput;
		break;
	case CProjAllConfig::NEWTYPE::Wide_Panorama:
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_Wide;
		break;
	case CProjAllConfig::NEWTYPE::None:
		theApp.m_PrjAllCfg.m_nDstWidth = DST_4K_WIDTH;
		theApp.m_PrjAllCfg.m_nDstHeight = DST_4K_HEIGHT;
		theApp.m_PrjAllCfg.m_nStitchingMode = QVS_Engine::StitchingMode_Normal;
		break;
	}
}

void CNewLoadConfigChild::getOutputSize(int type, int &width, int &height)
{
	switch (type)
	{
	case 0:
		width = DST_1K_WIDTH;
		height = DST_1K_HEIGHT;
		break;
	case 1:
		width = DST_2K_WIDTH;
		height = DST_2K_HEIGHT;
		break;
	case 2:
		width = DST_4K_WIDTH;
		height = DST_4K_HEIGHT;
		break;
	case 3:
		width = DST_8K_WIDTH;
		height = DST_8K_HEIGHT;
		break; 
	case 4: //Stereo Top/Bottom output is 3840x3840
		width = DST_STEREO_4K_WIDTH;
		height = DST_STEREO_4K_HEIGHT;
		break;
	case 5: //Stereo Left/Right output is 3840x1920
		width = DST_2K_WIDTH;
		height = DST_4K_HEIGHT;
		break;
	case 6: //Stereo Left/Right output is 4096x2048
		width = DST_STEREO_SIDE_WIDTH;
		height = DST_STEREO_SIDE_HEIGHT;
		break;
	}
}