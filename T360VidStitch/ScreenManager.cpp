#include "stdafx.h"
#include "T360VidStitch.h"
#include "T360VidStitchDlg.h"
#include "ScreenManager.h"

//char sztempPath[1024] = { 0, };

char* CString2Char(CString szString)
{
	// Unicode CString -> char* 변환 과정 
	// CString -> wchar* -> char* 순서로 변환 되어야 함

	wchar_t *wchar_str;
	static char *char_str = NULL;
	int char_str_len;

	// 1. CString to wchar * conversion
	wchar_str = szString.GetBuffer(szString.GetLength());
	char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
	char_str = new char[char_str_len];
	memset(char_str, 0, char_str_len);
	// 2. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0, 0);

	return char_str;
}

CString Char2CString(char* szChar)
{
	// Unicode char* -> CString 변환 과정 
	// char* -> wchar* -> CString 순서로 변환 되어야 함

	int len;
	CString str;
	BSTR buf;

	// 1. char* to wchar * conversion
	len = MultiByteToWideChar(CP_ACP, 0, szChar, strlen(szChar), NULL, NULL);
	buf = SysAllocStringLen(NULL, len);
	MultiByteToWideChar(CP_ACP, 0, szChar, strlen(szChar), buf, len);

	// 2. wchar_t* to CString conversion 
	str.Format(_T("%s"), buf);

	return str;
}

CScreenManager::CScreenManager()
{
	m_pParent = NULL;

	m_pParentLeftPane = NULL;
	m_pParentRightPane = NULL;
	m_pParentBottomPane = NULL;

	m_pCenterView = NULL;
	m_pNewLoadView = NULL;
	m_pConfigView = NULL;

	//Left Pane
	m_pLeftChild = NULL;

	//Right Pane
	m_pRightSourceChild = NULL;
	m_pRightPanoramaChild = NULL;
	m_pRightInteracChild = NULL;
	m_pRightCalibChild = NULL;
	m_pRightOverlayChild = NULL;
	m_pRightOverlayListChild = NULL;
	m_pRightAdvanceChild = NULL;
	m_pRightAdvanceAlignChild = NULL;
	m_pRightAdvanceUpdateChild = NULL;
	m_pRightAdvanceManualChild = NULL;
	m_pRightManualCalibChild = NULL;

	//Bottom
	m_pBottomSourceChild = NULL;
	m_pBottomPanoChild = NULL;
	m_pBottomManualChild = NULL;

	m_ActiveWnd = vdstNewLoad;

	m_iStartType = -1;
	m_szCurrentPath = _T("");

	m_bSnapShotDone = FALSE;
	m_bGainManCurveDone = FALSE;
	m_bCalibrationDone = FALSE;
	m_bGainCompensationDone = FALSE;
	m_bseamModeDone = FALSE;
	m_bautoGainModeDone = FALSE;
	m_nEditOutputStatus = 0;	// 0 : nothing, -1: Fail, 1: Done
	m_nNCalibrationStatus = 0;	// 0 : nothing, -1: Fail, 1: Done
	m_pSubMode = SUBMODE_OFF;
}


CScreenManager::~CScreenManager()
{
	//View는 자동 삭제 된다.
	// Center View
	DeleteAllScreen();
#ifdef _DEBUG
	TRACE(_T("~CScreenManager Call \n"));
#endif
}

void CScreenManager::DeleteAllScreen()
{
	if (m_pNewLoadView && ::IsWindow(m_pNewLoadView->GetSafeHwnd()))
	{
		m_pNewLoadView->DestroyWindow();
		DeleteObject(m_pNewLoadView);
		//delete m_pNewLoadView;
		m_pNewLoadView = NULL;
	}

	DeleteScreens();
}

void CScreenManager::DeleteMainView()
{
	if (m_pCenterView && ::IsWindow(m_pCenterView->GetSafeHwnd()))
	{
		if (m_pRightPanoramaChild)
		{
			m_pRightPanoramaChild->CheckClosetoRecodeing();
			((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(1000);
		}
		m_pCenterView->DestroyWindow();
		DeleteObject(m_pCenterView);
		m_pCenterView = NULL;
	}
}

void CScreenManager::DeleteScreens()
{
	DeleteMainView();

	if (m_pConfigView && m_pConfigView->GetSafeHwnd())
	{
		m_pConfigView->DestroyWindow();
		DeleteObject(m_pConfigView);
		m_pConfigView = NULL;
	}

	//Left
	if (m_pLeftChild)
	{
		m_pLeftChild->DestroyWindow();
		delete m_pLeftChild;
		m_pLeftChild = NULL;
	}

	//Right
	if (m_pRightSourceChild)
	{
		m_pRightSourceChild->DestroyWindow();
		delete m_pRightSourceChild;
		m_pRightSourceChild = NULL;
	}

	if (m_pRightPanoramaChild)
	{
		m_pRightPanoramaChild->DestroyWindow();
		delete m_pRightPanoramaChild;
		m_pRightPanoramaChild = NULL;
	}

	if (m_pRightInteracChild)
	{
		m_pRightInteracChild->DestroyWindow();
		delete m_pRightInteracChild;
		m_pRightInteracChild = NULL;
	}

	if (m_pRightCalibChild)
	{
		m_pRightCalibChild->DestroyWindow();
		delete m_pRightCalibChild;
		m_pRightCalibChild = NULL;
	}

	if (m_pRightOverlayChild)
	{
		m_pRightOverlayChild->DestroyWindow();
		delete m_pRightOverlayChild;
		m_pRightOverlayChild = NULL;
	}

	if (m_pRightOverlayListChild)
	{
		m_pRightOverlayListChild->DestroyWindow();
		delete m_pRightOverlayListChild;
		m_pRightOverlayListChild = NULL;
	}

	if (m_pRightAdvanceChild)
	{
		m_pRightAdvanceChild->DestroyWindow();
		delete m_pRightAdvanceChild;
		m_pRightAdvanceChild = NULL;
	}

	if (m_pRightAdvanceAlignChild)
	{
		m_pRightAdvanceAlignChild->DestroyWindow();
		delete m_pRightAdvanceAlignChild;
		m_pRightAdvanceAlignChild = NULL;
	}

	if (m_pRightAdvanceUpdateChild)
	{
		m_pRightAdvanceUpdateChild->DestroyWindow();
		delete m_pRightAdvanceUpdateChild;
		m_pRightAdvanceUpdateChild = NULL;
	}

	if (m_pRightAdvanceManualChild)
	{
		m_pRightAdvanceManualChild->DestroyWindow();
		delete m_pRightAdvanceManualChild;
		m_pRightAdvanceManualChild = NULL;
	}

	if (m_pRightManualCalibChild)
	{
		m_pRightManualCalibChild->DestroyWindow();
		delete m_pRightManualCalibChild;
		m_pRightManualCalibChild = NULL;
	}
	//Bottom
	if (m_pBottomSourceChild)
	{
		m_pBottomSourceChild->DestroyWindow();
		delete m_pBottomSourceChild;
		m_pBottomSourceChild = NULL;
	}

	if (m_pBottomPanoChild)
	{
		m_pBottomPanoChild->DestroyWindow();
		delete m_pBottomPanoChild;
		m_pBottomPanoChild = NULL;
	}

	if (m_pBottomManualChild)
	{
		m_pBottomManualChild->DestroyWindow();
		delete m_pBottomManualChild;
		m_pBottomManualChild = NULL;
	}
}

void CScreenManager::CreateAllScreen(vdstScrrenPosition scrPos)
{

	switch (scrPos)
	{
	case vdstLeft:
		if (m_pParentLeftPane)
		{
			//Left Pane
			CreateScreens(vdstLeftControlView, m_pParentLeftPane);
		}
		break;
	case vdstRight:
		if (m_pParentRightPane)
		{
			//Right Pane
			CreateScreens(vdstRightSourceView, m_pParentRightPane);
			CreateScreens(vdstRightPanoramaView, m_pParentRightPane);
			CreateScreens(vdstRightInteracView, m_pParentRightPane);
			CreateScreens(vdstRightCalibView, m_pParentRightPane);
			CreateScreens(vdstRightOverlayView, m_pParentRightPane);
			CreateScreens(vdstRightOverlayListView, m_pParentRightPane);
			CreateScreens(vdstRightAdvanceView, m_pParentRightPane);
			CreateScreens(vdstRightAdvanceAlignView, m_pParentRightPane);
			CreateScreens(vdstRightAdvanceUpdateView, m_pParentRightPane);
			CreateScreens(vdstRightAdvanceManualView, m_pParentRightPane);
			CreateScreens(vdstRightManualCalibView, m_pParentRightPane);
		}
		break;
	case vdstBottom:
		if (m_pParentBottomPane)
		{
			//Bottom Pane
			CreateScreens(vdstBottomSourceView, m_pParentBottomPane);
			CreateScreens(vdstBottomPanoramaView, m_pParentBottomPane);
			CreateScreens(vdstBottomManualClibView, m_pParentBottomPane);
		}
		break;
	case vdstCenter:
		if (m_pParent)
		{
			//Center View
			CreateScreens(vdstScreenView, m_pParent);
			CreateScreens(vdstNewLoad, m_pParent);
			CreateScreens(vdstConfiguView, m_pParent);
		}
		break;
	}
}


void CScreenManager::CreateScreens(vdstScreenCenter screen, CWnd *pParent)
{
	CCreateContext cc;

	switch (screen)
	{
	case vdstNewLoad:
		if (m_pNewLoadView == NULL)
		{
			CView* pView = (CView*)RUNTIME_CLASS(CNewLoadView)->CreateObject();
			ZeroMemory(&cc, sizeof(cc));
			pView->Create(nullptr, nullptr, WS_CHILD, m_rcDlg, pParent, IDD_NEWLOADVIEW, &cc);
			pView->OnInitialUpdate();
			pView->ShowWindow(SW_HIDE);
			m_pNewLoadView = pView;
		}
		break;
	case vdstScreenView:
		if (m_pCenterView == NULL)
		{
#ifdef _DEBUG
			TRACE(_T("CreateScreens Center View\n"));
#endif
			m_pCenterView = (CView*)RUNTIME_CLASS(CCenterView)->CreateObject();
			ZeroMemory(&cc, sizeof(cc));

			printf("JW CreateScreens: (theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight)= (%d, %d)\n", theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight);
			//((CCenterView*)m_pCenterView)->setOutputSize(theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight);

			m_pCenterView->Create(nullptr, nullptr, WS_CHILD, m_rcDlg, pParent, IDD_CENTERSCREENVIEW, &cc);
			m_pCenterView->OnInitialUpdate();
			m_pCenterView->ShowWindow(SW_HIDE);
		}
		break;
	case vdstConfiguView:
		if (m_pConfigView == NULL)
		{
			CView* pView = (CView*)RUNTIME_CLASS(CConfiguView)->CreateObject();
			ZeroMemory(&cc, sizeof(cc));
			pView->Create(nullptr, nullptr, WS_CHILD, m_rcDlg, pParent, IDD_CONFIGURATIONVIEW_DLG, &cc);
			pView->OnInitialUpdate();
			pView->ShowWindow(SW_HIDE);
			m_pConfigView = pView;
		}
		break;
	case vdstLeftControlView:
		if (m_pLeftChild == NULL)
		{
			m_pLeftChild = new CLeftChildDlg;
			m_pLeftChild->Create(CLeftChildDlg::IDD, pParent);
		}
		break;
	case vdstRightSourceView:
		if (m_pRightSourceChild == NULL)
		{
			m_pRightSourceChild = new CRightSourceChildDlg;
			m_pRightSourceChild->Create(CRightSourceChildDlg::IDD, pParent);
		}
		break;
	case vdstRightPanoramaView:
		if (m_pRightPanoramaChild == NULL)
		{
			m_pRightPanoramaChild = new CRightPanoraChildDlg;
			m_pRightPanoramaChild->Create(CRightPanoraChildDlg::IDD, pParent);
		}
		break;
	case vdstRightInteracView:
		if (m_pRightInteracChild == NULL)
		{
			m_pRightInteracChild = new CRightInteracChildDlg;
			m_pRightInteracChild->Create(CRightInteracChildDlg::IDD, pParent);
		}
		break;
	case vdstRightCalibView:
		if (m_pRightCalibChild == NULL)
		{
			m_pRightCalibChild = new CRightCalibChildDlg;
			m_pRightCalibChild->Create(CRightCalibChildDlg::IDD, pParent);
		}
		break;
	case vdstRightOverlayView:
		if (m_pRightOverlayChild == NULL)
		{
			m_pRightOverlayChild = new RightOverlayChildDlg;
			m_pRightOverlayChild->Create(RightOverlayChildDlg::IDD, pParent);
		}
		break;
	case vdstRightOverlayListView:
		if (m_pRightOverlayListChild == NULL)
		{
			m_pRightOverlayListChild = new CRightOverlayListChildDlg;
			m_pRightOverlayListChild->Create(CRightOverlayListChildDlg::IDD, pParent);
		}
		break;
	case vdstRightAdvanceView:
		if (m_pRightAdvanceChild == NULL)
		{
			m_pRightAdvanceChild = new RightAdvanceChildDlg;
			m_pRightAdvanceChild->Create(RightAdvanceChildDlg::IDD, pParent);
		}
		break;
	case vdstRightAdvanceAlignView:
		if (m_pRightAdvanceAlignChild == NULL)
		{
			m_pRightAdvanceAlignChild = new RightAdvanceAlignChildDlg;
			m_pRightAdvanceAlignChild->Create(RightAdvanceAlignChildDlg::IDD, pParent);
		}
		break;
	case vdstRightAdvanceUpdateView:
		if (m_pRightAdvanceUpdateChild == NULL)
		{
			m_pRightAdvanceUpdateChild = new RightAdvanceUpdateChildDlg;
			m_pRightAdvanceUpdateChild->Create(RightAdvanceUpdateChildDlg::IDD, pParent);
		}
		break;
	case vdstRightAdvanceManualView:
		if (m_pRightAdvanceManualChild == NULL)
		{
			m_pRightAdvanceManualChild = new RightAdvanceManualChildDlg;
			m_pRightAdvanceManualChild->Create(RightAdvanceManualChildDlg::IDD, pParent);
		}
		break;
	case vdstRightManualCalibView:
		if (m_pRightManualCalibChild == NULL)
		{
			m_pRightManualCalibChild = new CRightManualCalibChildDlg;
			m_pRightManualCalibChild->Create(CRightManualCalibChildDlg::IDD, pParent);
		}
		break;
	case vdstBottomSourceView:
		if (m_pBottomSourceChild == NULL)
		{
			m_pBottomSourceChild = new CBottomSourceChildDlg;
			m_pBottomSourceChild->Create(CBottomSourceChildDlg::IDD, pParent);
		}
		break;
	case vdstBottomPanoramaView:
		if (m_pBottomPanoChild == NULL)
		{
			m_pBottomPanoChild = new CBottomPanoChildDlg;
			m_pBottomPanoChild->Create(CBottomPanoChildDlg::IDD, pParent);
		}
		break;
	case vdstBottomManualClibView:
		if (m_pBottomManualChild == NULL)
		{
			m_pBottomManualChild = new CBottomManualChildDlg;
			m_pBottomManualChild->Create(CBottomManualChildDlg::IDD, pParent);
		}
		break;
	case vdstBottomInteractiveView:
		//현재는 없음.
		break;
	default:
		break;
	}
}

void CScreenManager::PreScreens(vdstScreenCenter screen)
{
	((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_SOURCE);

	m_pCenterView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
	m_pCenterView->ShowWindow(SW_SHOW);
}

void CScreenManager::ShowScreens(vdstScreenCenter screen, bool isOnSize, bool isFullScreen)
{
	if (isOnSize)
		return;

	printf("ShowScreens screen=%d\n", screen);

	//먼저 Hide하자
	((CT360VidStitchDlg*)theApp.m_pMainWnd)->ToggleIcon(screen);
	//if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
	{
		if (screen != vdstSourceView && screen != vdstPanoramaView &&
			screen != vdstInteractiveView && m_pCenterView &&
			screen != vdstRightCalibView && screen != vdstRightAdvanceView && screen != vdstRightManualCalibView
			&& screen != vdstRightAdvanceAlignView && screen != vdstRightAdvanceUpdateView && screen != vdstRightAdvanceManualView)
			m_pCenterView->ShowWindow(SW_HIDE);

		if (screen != vdstSourceView && m_pCenterView && screen != vdstRightCalibView && screen != vdstRightAdvanceView && screen != vdstRightManualCalibView
			&& screen != vdstRightAdvanceAlignView && screen != vdstRightAdvanceUpdateView && screen != vdstRightAdvanceManualView)
		{
			//m_pSourceViewChild->ShowWindow(SW_HIDE);
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
		}
		if (screen != vdstInteractiveView && screen != vdstPanoramaView &&
			m_pCenterView && screen != vdstRightCalibView && screen != vdstRightAdvanceView && screen != vdstRightManualCalibView
			&& screen != vdstRightAdvanceAlignView && screen != vdstRightAdvanceUpdateView && screen != vdstRightAdvanceManualView)
		{
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);
		}

		if (screen != vdstConfiguView && m_pConfigView)
		{
			m_pConfigView->ShowWindow(SW_HIDE);
		}

		if (screen != vdstRightAdvanceUpdateView){

		}

		if (screen != vdstInteractiveView)
		{
			if (m_pRightInteracChild != NULL)
				m_pRightInteracChild->ShowWindow(SW_HIDE);

			if (m_pCenterView != NULL)
				((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->interactive_animation_mode = ANIMATION_MODE_NONE;
		}
	}

	if (screen != vdstNewLoad && m_pNewLoadView && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
	{
		m_pNewLoadView->ShowWindow(SW_HIDE);
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->EnableTitle(SW_SHOW);
	}
	if (m_pSubMode == SUBMODE_ADVANCE_UPDATE && !isOnSize)
	{
		if (screen != vdstRightAdvanceUpdateView){
			theApp.m_ScreenManager.setSubMode(SUBMODE_OFF);
			QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_Off);
			QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Off);
		}
	}

	if (m_pSubMode == SUBMODE_EDIT_ROTATION && !isOnSize)
	{
		if (m_pRightCalibChild != NULL)
		{
			m_pRightCalibChild->m_btnEditOff.ShowWindow(SW_SHOW);
			m_pRightCalibChild->m_btnEditOn.ShowWindow(SW_HIDE);
			m_pRightCalibChild->m_btnStraighten.ShowWindow(SW_HIDE);
			//m_pRightPanoramaChild->m_btnPanoSave.ShowWindow(SW_SHOW);
		}
		theApp.m_ScreenManager.setSubMode(SUBMODE_OFF);
		theApp.m_ScreenManager.updateSubMenu();
		QVS_Engine::QVS_saveOutput();
	}
	/*else if ((m_pSubMode == SUBMODE_ADVANCE_UPDATE) && !isOnSize)
	{
	QVS_Engine::QVS_setROIMode(QVS_Engine::ROIMaskMode_Off);
	QVS_Engine::QVS_setSelectMode(QVS_Engine::InputSelectMode_Off);
	theApp.m_ScreenManager.setSubMode(SUBMODE_OFF);
	}*/

	switch (screen)
	{
	case vdstNewLoad:
		if (m_pNewLoadView != NULL)
		{
			((CT360VidStitchDlg*)theApp.m_pMainWnd)->EnableTitle(SW_HIDE);
			m_ActiveWnd = vdstNewLoad;

			if (m_pLeftChild != NULL)
				m_pLeftChild->ShowWindow(SW_HIDE);
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayChild != NULL)
				m_pRightOverlayChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayListChild != NULL)
				m_pRightOverlayListChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
			if (m_pRightManualCalibChild != NULL)
				m_pRightManualCalibChild->ShowWindow(SW_HIDE);
			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomManualChild != NULL)
				m_pBottomManualChild->ShowWindow(SW_HIDE);

			m_pNewLoadView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
			m_pNewLoadView->ShowWindow(SW_SHOW);
			m_pNewLoadView->PostMessage(WM_SIZE, m_rcDlg.Width(), m_rcDlg.Height());

			if (m_pLeftChild != NULL)
				m_pLeftChild->ShowWindow(SW_HIDE);
		}
		break;
	case vdstSourceView:
		if (m_pCenterView != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			m_ActiveWnd = vdstSourceView;
			//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> w : %d, h : %d\n", theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight);
			if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isAddOverlayDlgShow)
				((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowAddOverlay(FALSE);

			if (((CCenterView*)m_pCenterView)->m_init && ((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();	//Stop을 시키고..

			((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_SOURCE, isFullScreen);

			m_pCenterView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
			m_pCenterView->ShowWindow(SW_SHOW);

			if (isFullScreen){
				if (m_pRightPanoramaChild->m_nFlgHddMode == recDoing)
					ShowToast(vdstToastHdd, _T("HDD Encoding…"), FALSE);
				if (m_pRightPanoramaChild->m_nFlgStreamMode == recDoing)
					ShowToast(vdstToastStream, _T("Streaming…"), FALSE);
				if (m_pRightPanoramaChild->m_nFlgOutputMode == recDoing)
					ShowToast(vdstToastOutput, _T("Output Doing…"), FALSE);
			}
			else{
				if (m_pRightPanoramaChild->m_nFlgHddMode == recDoing){
					setEnableConfiurationBtn(TRUE);
					ShowToast(vdstToastHdd, _T("HDD Encoding…"), TRUE);
				}
				if (m_pRightPanoramaChild->m_nFlgStreamMode == recDoing){
					setEnableConfiurationBtn(TRUE);
					ShowToast(vdstToastStream, _T("Streaming…"), TRUE);
				}
				if (m_pRightPanoramaChild->m_nFlgOutputMode == recDoing){
					setEnableConfiurationBtn(TRUE);
					ShowToast(vdstToastOutput, _T("Output Doing…"), TRUE);
				}
			}

			if (!isFullScreen)
			{
				if (m_pLeftChild != NULL)
					m_pLeftChild->ShowWindow(SW_SHOW);
				if (m_pRightSourceChild != NULL)
					m_pRightSourceChild->ShowWindow(SW_SHOW);
				if (m_pRightPanoramaChild != NULL)
					m_pRightPanoramaChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceChild != NULL)
					m_pRightAdvanceChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceAlignChild != NULL)
					m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceUpdateChild != NULL)
					m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceManualChild != NULL)
					m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
				if (m_pRightManualCalibChild != NULL)
					m_pRightManualCalibChild->ShowWindow(SW_HIDE);
				if (m_pRightCalibChild != NULL)
					m_pRightCalibChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayChild != NULL)
					m_pRightOverlayChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayListChild != NULL)
					m_pRightOverlayListChild->ShowWindow(SW_HIDE);

				if (m_pBottomPanoChild != NULL)
					m_pBottomPanoChild->ShowWindow(SW_HIDE);
				if (m_pBottomSourceChild != NULL)
				{
					CRect rc;
					m_pParentBottomPane->GetClientRect(rc);

					m_pBottomSourceChild->ShowWindow(SW_SHOW);
					m_pBottomSourceChild->MoveWindow(rc);            // 크기는 알아서 맞게 지정
					m_pBottomSourceChild->PostMessage(WM_SIZE, rc.Width(), rc.Height());          // 크기는 알아서 맞게 지정
				}
			}
			else
			{
				if (m_pLeftChild != NULL)
					m_pLeftChild->ShowWindow(SW_HIDE);
				if (m_pRightSourceChild != NULL)
					m_pRightSourceChild->ShowWindow(SW_HIDE);
				if (m_pRightCalibChild != NULL)
					m_pRightCalibChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayChild != NULL)
					m_pRightOverlayChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayListChild != NULL)
					m_pRightOverlayListChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceChild != NULL)
					m_pRightAdvanceChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceAlignChild != NULL)
					m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceUpdateChild != NULL)
					m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceManualChild != NULL)
					m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
				if (m_pRightManualCalibChild != NULL)
					m_pRightManualCalibChild->ShowWindow(SW_HIDE);
				if (m_pBottomSourceChild != NULL)
					m_pBottomSourceChild->ShowWindow(SW_HIDE);
			}

			if (m_pBottomManualChild != NULL)
				m_pBottomManualChild->ShowWindow(SW_HIDE);
			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->UpdateDialogItemValue();

			if (!((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();
		}
		break;
	case vdstInteractiveView:
		if (m_pCenterView != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			m_ActiveWnd = screen;

			if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isAddOverlayDlgShow)
				((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowAddOverlay(FALSE);

			if (((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();	//Stop을 시키고..

			if (screen == vdstInteractiveView)
				((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_INTERACTIVE, isFullScreen);
			else
			{
				((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_OUTPUT, isFullScreen);
			}


			m_pCenterView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
			m_pCenterView->ShowWindow(SW_SHOW);

			if (isFullScreen){
				if (m_pRightPanoramaChild->m_nFlgHddMode == recDoing)
					ShowToast(vdstToastHdd, _T("HDD Encoding…"), FALSE);
				if (m_pRightPanoramaChild->m_nFlgStreamMode == recDoing)
					ShowToast(vdstToastStream, _T("Streaming…"), FALSE);
				if (m_pRightPanoramaChild->m_nFlgOutputMode == recDoing)
					ShowToast(vdstToastOutput, _T("Output Doing…"), FALSE);
			}
			else{
				if (m_pRightPanoramaChild->m_nFlgHddMode == recDoing){
					setEnableConfiurationBtn(TRUE);
					ShowToast(vdstToastHdd, _T("HDD Encoding…"), TRUE);
				}
				if (m_pRightPanoramaChild->m_nFlgStreamMode == recDoing){
					setEnableConfiurationBtn(TRUE);
					ShowToast(vdstToastStream, _T("Streaming…"), TRUE);
				}
				if (m_pRightPanoramaChild->m_nFlgOutputMode == recDoing){
					setEnableConfiurationBtn(TRUE);
					ShowToast(vdstToastOutput, _T("Output Doing…"), TRUE);
				}
			}

			if (!((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();
			if (!isFullScreen)
			{
				if (m_pLeftChild != NULL)
					m_pLeftChild->ShowWindow(SW_SHOW);
				if (m_pRightPanoramaChild != NULL)
					m_pRightPanoramaChild->ShowWindow(SW_HIDE);
				if (m_pRightInteracChild != NULL)
					m_pRightInteracChild->ShowWindow(SW_SHOW);
				if (m_pRightCalibChild != NULL)
					m_pRightCalibChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayChild != NULL)
					m_pRightOverlayChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayListChild != NULL)
					m_pRightOverlayListChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceChild != NULL)
					m_pRightAdvanceChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceAlignChild != NULL)
					m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceUpdateChild != NULL)
					m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceManualChild != NULL)
					m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
				if (m_pRightManualCalibChild != NULL)
					m_pRightManualCalibChild->ShowWindow(SW_HIDE);

			}
			else
			{
				if (m_pLeftChild != NULL)
					m_pLeftChild->ShowWindow(SW_HIDE);
				if (m_pRightSourceChild != NULL)
					m_pRightSourceChild->ShowWindow(SW_HIDE);
				if (m_pRightCalibChild != NULL)
					m_pRightCalibChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayChild != NULL)
					m_pRightOverlayChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayListChild != NULL)
					m_pRightOverlayListChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceChild != NULL)
					m_pRightAdvanceChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceAlignChild != NULL)
					m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceUpdateChild != NULL)
					m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceManualChild != NULL)
					m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
				if (m_pRightManualCalibChild != NULL)
					m_pRightManualCalibChild->ShowWindow(SW_HIDE);
				if (m_pBottomSourceChild != NULL)
					m_pBottomSourceChild->ShowWindow(SW_HIDE);
			}

			if (m_pBottomManualChild != NULL)
				m_pBottomManualChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
			{
				CRect rc;
				m_pParentBottomPane->GetClientRect(rc);

				m_pBottomPanoChild->ShowWindow(SW_SHOW);
				m_pBottomPanoChild->MoveWindow(rc);            // 크기는 알아서 맞게 지정
				m_pBottomPanoChild->PostMessage(WM_SIZE, rc.Width(), rc.Height());          // 크기는 알아서 맞게 지정
			}
		}
		break;
	case vdstPanoramaView:
		if (m_pCenterView != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			m_ActiveWnd = screen;

			if (((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();	//Stop을 시키고..

			if (screen == vdstInteractiveView)
				((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_INTERACTIVE, isFullScreen);
			else
			{
				((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_OUTPUT, isFullScreen);
			}


			m_pCenterView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
			m_pCenterView->ShowWindow(SW_SHOW);

			if (isFullScreen){
				if (m_pRightPanoramaChild->m_nFlgHddMode == recDoing)
					ShowToast(vdstToastHdd, _T("HDD Encoding…"), FALSE);
				if (m_pRightPanoramaChild->m_nFlgStreamMode == recDoing)
					ShowToast(vdstToastStream, _T("Streaming…"), FALSE);
				if (m_pRightPanoramaChild->m_nFlgOutputMode == recDoing)
					ShowToast(vdstToastOutput, _T("Output Doing…"), FALSE);
			}
			else{
				if (m_pRightPanoramaChild->m_nFlgHddMode == recDoing){
					setEnableConfiurationBtn(TRUE);
					ShowToast(vdstToastHdd, _T("HDD Encoding…"), TRUE);
				}
				if (m_pRightPanoramaChild->m_nFlgStreamMode == recDoing){
					setEnableConfiurationBtn(TRUE);
					ShowToast(vdstToastStream, _T("Streaming…"), TRUE);
				}
				if (m_pRightPanoramaChild->m_nFlgOutputMode == recDoing){
					setEnableConfiurationBtn(TRUE);
					ShowToast(vdstToastOutput, _T("Output Doing…"), TRUE);
				}
			}

			if (!((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();
			if (!isFullScreen)
			{
				if (m_pLeftChild != NULL)
					m_pLeftChild->ShowWindow(SW_SHOW);
				if (m_pRightPanoramaChild != NULL)
					m_pRightPanoramaChild->ShowWindow(SW_SHOW);
				if (m_pRightCalibChild != NULL)
					m_pRightCalibChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayChild != NULL)
					m_pRightOverlayChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayListChild != NULL)
					m_pRightOverlayListChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceChild != NULL)
					m_pRightAdvanceChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceAlignChild != NULL)
					m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceUpdateChild != NULL)
					m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceManualChild != NULL)
					m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
				if (m_pRightManualCalibChild != NULL)
					m_pRightManualCalibChild->ShowWindow(SW_HIDE);

			}
			else
			{
				if (m_pLeftChild != NULL)
					m_pLeftChild->ShowWindow(SW_HIDE);
				if (m_pRightSourceChild != NULL)
					m_pRightSourceChild->ShowWindow(SW_HIDE);
				if (m_pRightCalibChild != NULL)
					m_pRightCalibChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayChild != NULL)
					m_pRightOverlayChild->ShowWindow(SW_HIDE);
				if (m_pRightOverlayListChild != NULL)
					m_pRightOverlayListChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceChild != NULL)
					m_pRightAdvanceChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceAlignChild != NULL)
					m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceUpdateChild != NULL)
					m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
				if (m_pRightAdvanceManualChild != NULL)
					m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
				if (m_pRightManualCalibChild != NULL)
					m_pRightManualCalibChild->ShowWindow(SW_HIDE);

				if (m_pBottomSourceChild != NULL)
					m_pBottomSourceChild->ShowWindow(SW_HIDE);
			}

			if (m_pBottomManualChild != NULL)
				m_pBottomManualChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
			{
				CRect rc;
				m_pParentBottomPane->GetClientRect(rc);

				m_pBottomPanoChild->ShowWindow(SW_SHOW);
				m_pBottomPanoChild->MoveWindow(rc);            // 크기는 알아서 맞게 지정
				m_pBottomPanoChild->PostMessage(WM_SIZE, rc.Width(), rc.Height());          // 크기는 알아서 맞게 지정
			}
		}
		break;
	case  vdstConfiguView:
		m_ActiveWnd = screen;

		if (m_pConfigView != NULL  && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			m_ActiveWnd = vdstConfiguView;
			m_pConfigView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
			m_pConfigView->ShowWindow(SW_SHOW);
			m_pConfigView->PostMessage(WM_SIZE, m_rcDlg.Width(), m_rcDlg.Height());

			if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isAddOverlayDlgShow)
				((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowAddOverlay(FALSE);

			//MoveWindow(m_rcDlg)
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
			if (m_pRightManualCalibChild != NULL)
				m_pRightManualCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayChild != NULL)
				m_pRightOverlayChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayListChild != NULL)
				m_pRightOverlayListChild->ShowWindow(SW_HIDE);

			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);
			if (m_pBottomManualChild != NULL)
				m_pBottomManualChild->ShowWindow(SW_HIDE);
		}
		break;
	case vdstRightCalibView:
		m_ActiveWnd = screen;
		m_pCenterView->MoveWindow(m_rcDlg);

		if (((CCenterView*)m_pCenterView)->STITCHING_MODE == MODE_DRAW_MANUAL_CALIB){
			((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_OUTPUT, isFullScreen);
			m_pCenterView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
			m_pCenterView->ShowWindow(SW_SHOW);
		}
		if (m_pRightCalibChild != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
			if (m_pRightManualCalibChild != NULL)
				m_pRightManualCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_SHOW);
			if (m_pRightOverlayChild != NULL)
				m_pRightOverlayChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayListChild != NULL)
				m_pRightOverlayListChild->ShowWindow(SW_HIDE);

			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);
			if (m_pBottomManualChild != NULL)
				m_pBottomManualChild->ShowWindow(SW_HIDE);

			theApp.m_ScreenManager.m_pRightCalibChild->refresh();
		}
		break;
	case vdstRightOverlayView:
		m_ActiveWnd = screen;
		m_pCenterView->MoveWindow(m_rcDlg);

		if (m_pCenterView != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			m_ActiveWnd = screen;

			if (((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();	//Stop을 시키고..

			if (screen == vdstInteractiveView)
				((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_INTERACTIVE, isFullScreen);
			else
			{
				((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_OUTPUT, isFullScreen);
			}


			m_pCenterView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
			m_pCenterView->ShowWindow(SW_SHOW);

			if (!((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();
		}

		if (m_pRightOverlayChild != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayChild != NULL)
				m_pRightOverlayChild->ShowWindow(SW_SHOW);
			if (m_pRightOverlayListChild != NULL)
				m_pRightOverlayListChild->ShowWindow(SW_HIDE);

			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);
		}
		break;
	case vdstRightOverlayListView:
		m_ActiveWnd = screen;
		m_pCenterView->MoveWindow(m_rcDlg);

		if (m_pCenterView != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			m_ActiveWnd = screen;

			if (((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();	//Stop을 시키고..

			if (screen == vdstInteractiveView)
				((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_INTERACTIVE, isFullScreen);
			else
			{
				((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_OUTPUT, isFullScreen);
			}


			m_pCenterView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
			m_pCenterView->ShowWindow(SW_SHOW);

			if (!((CCenterView*)m_pCenterView)->isPlay())
				((CCenterView*)m_pCenterView)->OnPlay();
		}

		if (m_pRightOverlayChild != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayChild != NULL)
			m_pRightOverlayChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayListChild != NULL)
				m_pRightOverlayListChild->ShowWindow(SW_SHOW);

			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);
		}
		break;
	case vdstRightAdvanceView:
		m_ActiveWnd = screen;
		m_pCenterView->MoveWindow(m_rcDlg);

		if (m_pRightAdvanceChild != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayChild != NULL)
				m_pRightOverlayChild->ShowWindow(SW_HIDE);
			if (m_pRightOverlayListChild != NULL)
				m_pRightOverlayListChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_SHOW);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
			if (m_pRightManualCalibChild != NULL)
				m_pRightManualCalibChild->ShowWindow(SW_HIDE);

			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);

			theApp.m_ScreenManager.m_pRightAdvanceChild->refresh();
		}
		break;
	case vdstRightManualCalibView:
		m_ActiveWnd = screen;
		m_pCenterView->MoveWindow(m_rcDlg);

		((CCenterView*)m_pCenterView)->SetViewMode(MODE_DRAW_MANUAL_CALIB, isFullScreen);
		m_pCenterView->MoveWindow(m_rcDlg);            // 크기는 알아서 맞게 지정
		m_pCenterView->ShowWindow(SW_SHOW);
		if (m_pRightManualCalibChild != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
			if (m_pRightManualCalibChild != NULL)
				m_pRightManualCalibChild->ShowWindow(SW_SHOW);

			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);
			
			if (m_pBottomManualChild != NULL)
			{
				CRect rc;
				m_pParentBottomPane->GetClientRect(rc);
				rc.left += 153;

				m_pBottomManualChild->ShowWindow(SW_SHOW);
				m_pBottomManualChild->MoveWindow(rc);            // 크기는 알아서 맞게 지정
				m_pBottomManualChild->PostMessage(WM_SIZE, rc.Width(), rc.Height());          // 크기는 알아서 맞게 지정
			}
		}
		break;
	case vdstRightAdvanceAlignView:
		m_ActiveWnd = screen;
		m_pCenterView->MoveWindow(m_rcDlg);

		if (m_pRightAdvanceAlignChild != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_SHOW);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
			if (m_pRightManualCalibChild != NULL)
				m_pRightManualCalibChild->ShowWindow(SW_HIDE);

			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);
		}
		break;
	case vdstRightAdvanceUpdateView:
		m_ActiveWnd = screen;
		m_pCenterView->MoveWindow(m_rcDlg);

		if (m_pRightAdvanceUpdateChild != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_SHOW);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_HIDE);
			if (m_pRightManualCalibChild != NULL)
				m_pRightManualCalibChild->ShowWindow(SW_HIDE);

			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);
		}
		break;
	case vdstRightAdvanceManualView:
		m_ActiveWnd = screen;
		m_pCenterView->MoveWindow(m_rcDlg);

		if (m_pRightAdvanceManualChild != NULL && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
		{
			if (m_pRightSourceChild != NULL)
				m_pRightSourceChild->ShowWindow(SW_HIDE);
			if (m_pRightPanoramaChild != NULL)
				m_pRightPanoramaChild->ShowWindow(SW_HIDE);
			if (m_pRightCalibChild != NULL)
				m_pRightCalibChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceChild != NULL)
				m_pRightAdvanceChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceAlignChild != NULL)
				m_pRightAdvanceAlignChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceUpdateChild != NULL)
				m_pRightAdvanceUpdateChild->ShowWindow(SW_HIDE);
			if (m_pRightAdvanceManualChild != NULL)
				m_pRightAdvanceManualChild->ShowWindow(SW_SHOW);
			if (m_pRightManualCalibChild != NULL)
				m_pRightManualCalibChild->ShowWindow(SW_HIDE);

			if (m_pBottomSourceChild != NULL)
				m_pBottomSourceChild->ShowWindow(SW_HIDE);
			if (m_pBottomPanoChild != NULL)
				m_pBottomPanoChild->ShowWindow(SW_HIDE);
		}
		break;
	}

	//if (isDoingUpdateWarping)
	//{
	//	setEnableCalibrationBtn(false);
	//}

}

void CScreenManager::ShowToast(vdstToastType nType, CString szText, BOOL bShow)
{

	switch (nType)
	{
	case  vdstToastHideAll:
		if (m_pCenterView != NULL)
		{
			((CCenterView*)m_pCenterView)->ShowToastDlg(vdstToastHdd, _T(""), FALSE);
			((CCenterView*)m_pCenterView)->ShowToastDlg(vdstToastStream, _T(""), FALSE);
			((CCenterView*)m_pCenterView)->ShowToastDlg(vdstToastOutput, _T(""), FALSE);

			m_pRightPanoramaChild->m_nFlgHddMode = recReady;
			m_pRightPanoramaChild->m_nFlgStreamMode = recReady;
			m_pRightPanoramaChild->m_nFlgOutputMode = recReady;

		}
		break;
	case vdstToastHdd:			//Hdd
	case vdstToastStream:		//Streaming
	case vdstToastOutput:		//Output
	case vdstToastDone:			//Done
	case vdstToastProgress:		//Progress
	case vdstToastWarping:
	case vdstToastFail:
		if (m_pCenterView != NULL)
		{
			((CCenterView*)m_pCenterView)->ShowToastDlg(nType, szText, bShow);
		}
		break;
	}
}

void CScreenManager::updateSubMenu()
{
	switch (m_pSubMode)
	{
	case SUBMODE_EDIT_ROTATION:
		if (m_pBottomSourceChild != NULL)
			m_pBottomSourceChild->ShowWindow(SW_HIDE);
		if (m_pBottomPanoChild != NULL)
			m_pBottomPanoChild->ShowWindow(SW_HIDE);

		m_pCenterView->MoveWindow(m_rcDlg);
		break;
	default:
		ShowScreens(m_ActiveWnd);
		break;
	}
}

void CScreenManager::MoveToast()
{
	switch (m_ActiveWnd)
	{
	case vdstNewLoad:
		break;
	case vdstSourceView:
	case vdstPanoramaView:
	case vdstRightCalibView:
	case vdstConfiguView:
	case vdstInteractiveView:
	default:
		if (m_pCenterView != NULL)
		{
			((CCenterView*)m_pCenterView)->MoveToast();
		}
		break;
	}
}

int CScreenManager::parsingPTS(CString path)
{
	int result = -1;

	if (m_pCenterView)
	{
		char* szpPath = CString2Char(path);
		result = ((CCenterView*)m_pCenterView)->parsingPTS(szpPath);
		free(szpPath);
	}

	return result;
}

int CScreenManager::loadProject()
{
	int result = -1;

	//::SetCurrentDirectory(theApp.m_PrjAllCfg.m_szProjectFullPath);

	if (m_pCenterView)
	{
		char* szpPath = CString2Char(theApp.m_PrjAllCfg.m_szProjectFullPath + _T("\\") + theApp.m_PrjAllCfg.m_szProjectName);
		result = ((CCenterView*)m_pCenterView)->loadParam(szpPath);
		free(szpPath);
	}

	return result;
}

int CScreenManager::saveProject()
{
	int result = -1;

	//SetCurrentDirectory(theApp.m_PrjAllCfg.m_szProjectFullPath);
	if (m_pCenterView)
	{
		char* szpPath = CString2Char(theApp.m_PrjAllCfg.m_szProjectFullPath + _T("\\") + theApp.m_PrjAllCfg.m_szProjectName);
		result = ((CCenterView*)m_pCenterView)->saveParam(szpPath);
		free(szpPath);
	}

	return result;
}

void CScreenManager::SetProjectName()
{
	//((CT360VidStitchDlg*)theApp.m_pMainWnd)->m_wndCenterViewLabel.SetWindowText(_T("Project Name: ") + theApp.m_PrjAllCfg.m_szProjectName);
	((CT360VidStitchDlg*)theApp.m_pMainWnd)->SetWindowText(theApp.m_PrjAllCfg.m_szProjectName);
}

void CScreenManager::setEnableConfiurationBtn(BOOL bShow)
{
	if (!bShow)
	{
		((CLeftChildDlg*)m_pLeftChild)->m_btnConfiguration.ShowWindow(SW_SHOW);
		((CLeftChildDlg*)m_pLeftChild)->m_btnConfiguration2.ShowWindow(SW_HIDE);
		((CLeftChildDlg*)m_pLeftChild)->m_btnConfiguration2.EnableButton(FALSE);
	}
	else
	{
		((CLeftChildDlg*)m_pLeftChild)->m_btnConfiguration.ShowWindow(SW_HIDE);
		((CLeftChildDlg*)m_pLeftChild)->m_btnConfiguration2.ShowWindow(SW_SHOW);
		((CLeftChildDlg*)m_pLeftChild)->m_btnConfiguration2.EnableButton(FALSE);
	}
}

void CScreenManager::setEnableCalibrationBtn(BOOL bShow)
{
	if (bShow)
	{
		theApp.m_ScreenManager.m_pRightCalibChild->refresh();
	}
	else
	{
		theApp.m_ScreenManager.m_pRightCalibChild->disableCalibrationBtns();
	}

	theApp.m_ScreenManager.m_pRightAdvanceAlignChild->setEnableBtns(bShow);
	theApp.m_ScreenManager.m_pRightAdvanceUpdateChild->setEnableBtns(bShow);
}

void CScreenManager::setCallbacks()
{
	QVS_Engine::QVS_setCallback_snapshot(onCallbackDone_snapshot);
	QVS_Engine::QVS_setCallback_doManualGainMode(onCallbackDone_doManualGainMode);
	//QVS_Engine::QVS_setCallback_quramCalibMode(onCallbackDone_quramCalibMode); //삭제
	QVS_Engine::QVS_setCallback_gainMode(onCallbackDone_gainMode);
	QVS_Engine::QVS_setCallback_seamMode(onCallbackDone_seamMode);
	QVS_Engine::QVS_setCallback_autoGainMode(onCallbackDone_autoGainMode);
	QVS_Engine::QVS_setCallback_editOutput(onCallback_editOutput);
	QVS_Engine::QVS_setCallback_calibration(onCallback_calibration);
	QVS_Engine::QVS_setCallback_doMakeWarpmap(onCallback_makeWarpmap);
	QVS_Engine::QVS_setCallback_history(onCallback_history);
	QVS_Engine::QVS_setCallback_audioError(onCallback_audioError);
	QVS_Engine::QVS_setCallback_regionWarping(onCallback_calibration);
	QVS_Engine::QVS_setCallback_stereoCalibration(onCallback_calibration);
	QVS_Engine::QVS_setCallback_load(onCallback_load);
}

void onCallbackDone_snapshot(int msg)
{
	printf("onCallbackDone snapshot: msg = 0x%x\n", msg);
	int size = sizeof("onCallbackDone snapshot: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallbackDone snapshot: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	delete(temp);
	switch (msg)
	{
	case QVS_MSG_SNAPSHOT_DONE:
		theApp.m_ScreenManager.m_nSnapShotDoneCnt--;
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_SNAPSHOT_DONE\n"));
#endif
		if (theApp.m_ScreenManager.m_nSnapShotDoneCnt == 0)
		{
			theApp.m_ScreenManager.SetSnapShotStatus(TRUE);
#ifdef _DEBUG
			TRACE(_T("Set QVS_MSG_SNAPSHOT_DONE flg\n"));
#endif
		}
		break;
	}
}

void onCallbackDone_doManualGainMode(int msg)
{
	printf("onCallbackDone ManualGainMode: msg = 0x%x\n", msg);
	int size = sizeof("onCallbackDone ManualGainMode: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallbackDone ManualGainMode: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	delete(temp);
	switch (msg)
	{
	case QVS_MSG_MANUAL_GAIN_MODE_DONE:
		theApp.m_ScreenManager.SetGainManCurveStatus(TRUE);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_MANUAL_GAIN_MODE_DONE\n"));
#endif
		break;
	}
}

void onCallbackDone_quramCalibMode(int msg)
{
	printf("onCallbackDone_quramCalibMode: msg = 0x%x\n", msg);
	int size = sizeof("onCallbackDone_quramCalibMode: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallbackDone_quramCalibMode: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	delete(temp);
	switch (msg)
	{
	case QVS_MSG_QURAM_CALIB_MODE_DONE:
	case QVS_MSG_CALIBRATION_FOV_WARNING:
		theApp.m_ScreenManager.SetCalibrationStatus(TRUE);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_QURAM_CALIB_MODE_DONE\n"));
#endif
		break;
	case QVS_MSG_CALIBRATION_DONE:
		theApp.m_ScreenManager.SetnewCalibrationStatus(1);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_CALIBRATION_DONE\n"));
#endif
		break;
	case QVS_MSG_CALIBRATION_FAIL:
	case QVS_MSG_CALIBRATION_FOV_FAIL:
		theApp.m_ScreenManager.SetnewCalibrationStatus(-1);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_CALIBRATION_FAIL\n"));
#endif
		break;
	}
}
void onCallbackDone_gainMode(int msg)
{
	printf("onCallbackDone_gainMode: msg = 0x%x\n", msg);
	int size = sizeof("onCallbackDone_gainMode: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallbackDone_gainMode: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	delete(temp);
	switch (msg)
	{
	case QVS_MSG_GAIN_DONE:
		theApp.m_ScreenManager.SetGainCompensationStatus(TRUE);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_GAIN_DONE\n"));
#endif
		break;
	}
}
void onCallbackDone_seamMode(int msg)
{
	printf("onCallbackDone_seamMode: msg = 0x%x\n", msg);
	int size = sizeof("onCallbackDone_seamMode: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallbackDone_seamMode: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	delete(temp);
	switch (msg)
	{
	case QVS_MSG_SEAM_DONE:
		theApp.m_ScreenManager.SetseamModeStatus(TRUE);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_SEAM_DONE\n"));
#endif
		break;
	}
}

void onCallbackDone_autoGainMode(int msg)
{
	printf("onCallbackDone_autoGainMode: msg = 0x%x\n", msg);
	int size = sizeof("onCallbackDone_autoGainMode: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallbackDone_autoGainMode: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	delete(temp);
	switch (msg)
	{
	case QVS_MSG_AUTO_GAIN_DONE:
		theApp.m_ScreenManager.SetautoGainModeStatus(TRUE);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_AUTO_GAIN_DONE\n"));
#endif
		break;
	}
}

void onCallback_editOutput(int msg)
{
	printf("onCallback_editOutput: msg = 0x%x\n", msg);
	switch (msg)
	{
	case QVS_MSG_EDIT_OUTPUT_DONE:
		theApp.m_ScreenManager.SetEditOutputStatus(1);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_EDIT_OUTPUT_DONE\n"));
#endif
		break;
	case QVS_MSG_EDIT_OUTPUT_FAIL:
		theApp.m_ScreenManager.SetEditOutputStatus(-1);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_EDIT_OUTPUT_FAIL\n"));
#endif
		break;
	}
}

void onCallback_calibration(int msg)
{
	printf("onCallback_calibration: msg = 0x%x\n", msg);
	int size = sizeof("onCallback_calibration: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallback_calibration: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	delete(temp);
	switch (msg)
	{
	case QVS_MSG_STRAIGHTEN_START:
	case QVS_MSG_UNDOREDO_START:
	case QVS_MSG_REGIONAL_WARPING_START:
	case QVS_MSG_CALIBRATION_START:
		theApp.m_ScreenManager.failCalibration = false;
		if (!theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = true;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), TRUE);
		}
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_QURAM_CALIB_MODE_DONE\n"));
#endif
		break;
	case QVS_MSG_UNDOREDO_DONE:
	case QVS_MSG_STRAIGHTEN_DONE:
		theApp.m_ScreenManager.failCalibration = false;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		break;
	case QVS_MSG_REGIONAL_WARPING_DONE:
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		((RightAdvanceManualChildDlg*)theApp.m_ScreenManager.m_pRightAdvanceManualChild)->refresh();
	case QVS_MSG_CALIBRATION_DONE:
		theApp.m_ScreenManager.failCalibration = false;
		//theApp.m_ScreenManager.isDoingProgToast = false;
		//theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_CALIBRATION_DONE\n"));
#endif
		break;
	case QVS_MSG_REGIONAL_WARPING_FAIL:
	case QVS_MSG_CALIBRATION_FAIL:
		theApp.m_ScreenManager.failCalibration = true;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		theApp.m_ScreenManager.ShowToast(vdstToastFail, _T("Calibration Fail"));
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_CALIBRATION_FAIL\n"));
#endif
		break;
	case QVS_MSG_CALIBRATION_FOV_FAIL:
	case QVS_MSG_CALIBRATION_FOV_WARNING:
		theApp.m_ScreenManager.failCalibration = true;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		theApp.m_ScreenManager.ShowToast(vdstToastFail, _T("Calibration FOV WARNING"));
		break;
	case QVS_MSG_CALIBRATION_TIMEOUT:
		theApp.m_ScreenManager.failCalibration = true;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		theApp.m_ScreenManager.ShowToast(vdstToastFail, _T("Calibration Time Out"));
		break;
	case QVS_MSG_CALIBRATION_NOT_RUN:
		theApp.m_ScreenManager.failCalibration = true;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		theApp.m_ScreenManager.ShowToast(vdstToastFail, _T("Calibration Align Not Run"));
#ifdef _DEBUG
		TRACE(_T("QVS_MSG_CALIBRATION_TIMEOUT\n"));
#endif
		break;
	case QVS_MSG_REGIONAL_WARPING_INVALID_POINT:
		theApp.m_ScreenManager.failCalibration = true;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		theApp.m_ScreenManager.ShowToast(vdstToastFail, _T("Regional Warping Invalid Point"));
		break;
	case QVS_MSG_REGIONAL_WARPING_INVALID_AREA:
		theApp.m_ScreenManager.failCalibration = true;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		theApp.m_ScreenManager.ShowToast(vdstToastFail, _T("Regional Warping Invalid Area"));
		break;
	case QVS_MSG_REGIONAL_WARPING_BIG_AREA:
		theApp.m_ScreenManager.failCalibration = true;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		theApp.m_ScreenManager.ShowToast(vdstToastFail, _T("Regional Warping Too Big Area"));
		break;
	case QVS_MSG_REGIONAL_WARPING_LONG_DISTANCE:
		theApp.m_ScreenManager.failCalibration = true;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		theApp.m_ScreenManager.ShowToast(vdstToastFail, _T("Regional Warping Too Long Distance"));
		break;
	}
}

void onCallback_makeWarpmap(int msg)
{
	printf("onCallbackDone_warpingMap: msg = 0x%x\n", msg);
	int size = sizeof("onCallbackDone_warpingMap: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallbackDone_warpingMap: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	delete(temp);
	switch (msg)
	{
	case QVS_MSG_MAKE_START_WARPMAP:
		// bigheadk 
		theApp.m_ScreenManager.isDoingUpdateWarping = true;
		if (!theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = true;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), TRUE);
		}
		break;
	case QVS_MSG_MAKE_DONE_WARPMAP:
		theApp.m_ScreenManager.isDoingUpdateWarping = false;
		if (theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.isDoingUpdateWarping = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}

		break;
	case QVS_MSG_LOADING_WARPMAP:
		if (!theApp.m_ScreenManager.isDoingProgToast && !QVS_Engine::QVS_getDoingLoad())
		{
			theApp.m_ScreenManager.isDoingProgToast = true;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), TRUE);
		}
		break;
	case QVS_MSG_LOADING_END_WARPMAP:
		if (theApp.m_ScreenManager.isDoingProgToast && !QVS_Engine::QVS_getDoingLoad())
		{
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}
		break;
	}
}

void onCallback_history(int msg)
{
	printf("onCallback_history: msg = 0x%x\n", msg);
	int size = sizeof("onCallback_history: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallback_history: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	switch (msg)
	{
	case QVS_MSG_HISTORY_ENQUEUE:
		theApp.m_ScreenManager.m_pRightCalibChild->refresh();
		//theApp.m_ScreenManager.SetautoGainModeStatus(TRUE);
		break;
	case QVS_MSG_HISTORY_UNDO:
		theApp.m_ScreenManager.m_pRightCalibChild->refresh();
		//theApp.m_ScreenManager.SetautoGainModeStatus(TRUE);
		break;
	case QVS_MSG_HISTORY_REDO:
		theApp.m_ScreenManager.m_pRightCalibChild->refresh();
		//theApp.m_ScreenManager.SetautoGainModeStatus(TRUE);
		break;
	}
}

void onCallback_audioError(int msg)
{
	printf("onCallback_audioError: msg = 0x%x\n", msg);
	int size = sizeof("onCallback_audioError: msg = 0x%x") * sizeof(char*)+sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallback_audioError: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	QVS_Engine::QVS_SetAudioDevice(-1);
	if (theApp.m_ScreenManager.m_pBottomSourceChild != NULL)
		theApp.m_ScreenManager.m_pBottomSourceChild->m_cbpAudioInput.SetCurSel(0);
}

void onCallback_load(int msg)
{
	printf("onCallback_load: msg = 0x%x\n", msg);
	int size = sizeof("onCallback_load: msg = 0x%x") * sizeof(char*) + sizeof(int);
	char* temp = new char[size];
	sprintf(temp, "onCallback_load: msg = 0x%x", msg);
	QVS_Engine::QVS_writeLog(temp, size);
	delete(temp);
	switch (msg)
	{
	case QVS_MSG_DONE_INPUT_LOAD:
		if (theApp.m_ScreenManager.isNewProject)
		{
			theApp.m_ScreenManager.isNewProject = false;

			CCenterView* mpCenterView = (CCenterView*)theApp.m_ScreenManager.m_pCenterView;
			if (mpCenterView != NULL)
				mpCenterView->showEditInputDlg(TRUE);

			CBottomSourceChildDlg* mpBottomSource = (CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild;
			if (mpBottomSource != NULL)
			{
				mpBottomSource->isCameraSel = true;
				mpBottomSource->m_btnCameraSel.SetImage(CGdipButton::ALT_TYPE);
			}
		}
		else
		{
			CBottomSourceChildDlg* mpBottomSource = (CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild;
			if (mpBottomSource != NULL)
				theApp.m_ScreenManager.m_pBottomSourceChild->UpdateDialogItemValue();
		}
		break;
	case QVS_MSG_DO_LOAD:
		if (!theApp.m_ScreenManager.isDoingProgToast)
		{
			theApp.m_ScreenManager.isDoingProgToast = true;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), TRUE);
		}

		if (!QVS_Engine::QVS_getDoingLoad())
			QVS_Engine::QVS_setDoingLoad(true);
		break;
	case QVS_MSG_DONE_LOAD:
		CCenterView* CCV = (CCenterView*)theApp.m_ScreenManager.m_pCenterView;

		if (theApp.m_ScreenManager.isDoingProgToast)
		{ 
			theApp.m_ScreenManager.isDoingProgToast = false;
			theApp.m_ScreenManager.ShowToast(vdstToastProgress, _T(" "), FALSE);
		}

		if (QVS_Engine::QVS_getDoingLoad())
			QVS_Engine::QVS_setDoingLoad(false);
		break;
	}
}