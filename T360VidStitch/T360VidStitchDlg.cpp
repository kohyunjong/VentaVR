
// T360VidStitchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "T360VidStitchDlg.h"
#include "afxdialogex.h"

#include "TQClock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CT360VidStitchDlg dialog



CT360VidStitchDlg::CT360VidStitchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CT360VidStitchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);
	XTPSkinManager()->LoadSkin(theApp.GetAppDir() + NOMALSKIN, _T("ExtraLargeNoir.ini"));

	fps = new RingBuf(60);
}

void CT360VidStitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_MAINFRAME, m_wndCenterViewLabel);
	DDX_Control(pDX, IDC_MAIN_CHILD, m_wndCenterViewFrame);
	DDX_Control(pDX, IDC_BTNCURRENTTYPE, m_btnCurView);
	DDX_Control(pDX, IDC_TEXT_FPS, m_txtFps);
	DDX_Control(pDX, IDC_BTNVIEWCLOSE, m_btnViewClose);
}

BEGIN_MESSAGE_MAP(CT360VidStitchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_MESSAGE(WM_UPDATESTRINGFPS, OnUpdateFPS)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNVIEWCLOSE, &CT360VidStitchDlg::OnBnClickedBtnviewclose)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_MOVING()
END_MESSAGE_MAP()


// CT360VidStitchDlg message handlers


LRESULT CT360VidStitchDlg::OnUpdateFPS(WPARAM wParam, LPARAM lParam)
{
	CString szText;
	m_txtFps.GetWindowText(szText);

	if (szText.CompareNoCase(m_strFPS))
		GetDlgItem(IDC_TEXT_FPS)->SetWindowText(m_strFPS);
	return TRUE;
}

BOOL CT360VidStitchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_FILESAVE, _T("File &Save"));
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//CenterView Create //느려진다. 이유는??
	theApp.m_ScreenManager.SetParent(&m_wndCenterViewFrame);
	theApp.m_ScreenManager.CreateScreens(vdstNewLoad, &m_wndCenterViewFrame);	// 최초 NewLoad만...

	CRect rcClient;
	m_wndCenterViewFrame.GetClientRect(rcClient);
	theApp.m_ScreenManager.SetRCDlg(rcClient);


	VERIFY(m_paneManager.InstallDockingPanes(this));

	m_paneManager.SetTheme(xtpPaneThemeDefault);

	m_paneManager.SetAlphaDockingContext(TRUE);
	m_paneManager.LockSplitters(TRUE);

	// Create docking panes.

	CXTPDockingPane* pwndPaneLeft = m_paneManager.CreatePane(IDR_PANE_LEFT, CRect(0, 0, 150, 150), xtpPaneDockLeft);
	CXTPDockingPane* pwndPaneRight = m_paneManager.CreatePane(IDR_PANE_RIGHT, CRect(0, 0, 150, 150), xtpPaneDockRight);
	CXTPDockingPane* pwndPaneBottom = m_paneManager.CreatePane(IDR_PANE_BOTTOM, CRect(0, 0, 200, 180), xtpPaneDockBottom);
	//CXTPDockingPane* pwndPaneTop = m_paneManager.CreatePane(IDR_PANE_TOP, CRect(0, 0, 20, 30), xtpPaneDockTop);

	//pwndPaneTop->SetOptions(xtpPaneNoCaption | xtpPaneNoFloatable | xtpPaneNoCloseable | xtpPaneNoHideable);
	pwndPaneLeft->SetOptions(xtpPaneNoCaption | xtpPaneNoFloatable | xtpPaneNoCloseable | xtpPaneNoHideable);
	pwndPaneRight->SetOptions(xtpPaneNoCaption | xtpPaneNoFloatable | xtpPaneNoCloseable | xtpPaneNoHideable);
	pwndPaneBottom->SetOptions(xtpPaneNoCaption | xtpPaneNoFloatable | xtpPaneNoCloseable | xtpPaneNoHideable);

	CXTPDockingPaneLayout layout(&m_paneManager);
	if (layout.Load(_T("NormalLayout")))
	{
		m_paneManager.SetLayout(&layout);
	}

	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);
	theApp.m_ScreenManager.m_szCurrentPath = Path;		//Current Path의 저장..


	m_btnViewClose.LoadBitmaps(IDBB_BTN_CLOSE_NOR, IDBB_BTN_CLOSE_PRESSED, 0, 0);
	m_btnViewClose.SizeToContent();

	m_btnCurView.LoadBitmaps(IDBB_BTN_WINICON_NONE, 0, 0, 0);
	m_btnCurView.SizeToContent();

	CreateFontObj();
	m_wndCenterViewLabel.SetFont(&m_fontArial_12);
	m_wndCenterViewFrame.SetFont(&m_fontArial_10R);
	// ConfiguView... 화면 객체 생성 Center View와 같이 생성 하면 시간이 많이 걸림..
	theApp.m_ScreenManager.CreateScreens(vdstConfiguView, &m_wndCenterViewFrame);

	CAboutDlg* pdlg = new CAboutDlg(3);
	pdlg->DoModal();
	delete pdlg;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CT360VidStitchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == IDM_FILESAVE)
	{

		if (theApp.m_ScreenManager.getScreenMode() != vdstNewLoad && !theApp.m_ScreenManager.isDoingProgToast)
		{
			QVS_Engine::QVS_SaveParam();
			MessageBox(_T("VRue360 : Save this project."), MB_OK, MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("VRue360 : Fail to save."), MB_OK, MB_ICONINFORMATION);
		}
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CT360VidStitchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

LRESULT CT360VidStitchDlg::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_PANE_LEFT:
				if (!::IsWindow(m_dlgPaneLeft.m_hWnd))
				{
					m_dlgPaneLeft.Create(IDD_PANELEFTVIEW, this);
				}
				pPane->Attach(&m_dlgPaneLeft);
				theApp.m_ScreenManager.SetLeftParent(&m_dlgPaneLeft);
				theApp.m_ScreenManager.CreateAllScreen(vdstLeft);
				break;

			case IDR_PANE_RIGHT:
				if (!::IsWindow(m_dlgPaneRight.m_hWnd))
				{
					m_dlgPaneRight.Create(IDD_PANERIGHTVIEW, this);
				}
				pPane->Attach(&m_dlgPaneRight);
				theApp.m_ScreenManager.SetRightParent(&m_dlgPaneRight);
				theApp.m_ScreenManager.CreateAllScreen(vdstRight);
				break;

			case IDR_PANE_BOTTOM:
				if (!::IsWindow(m_dlgPaneBottom.m_hWnd))
				{
					m_dlgPaneBottom.Create(IDD_PANEBOTTOMVIEW, this);
				}
				pPane->Attach(&m_dlgPaneBottom);
				theApp.m_ScreenManager.SetBottomParent(&m_dlgPaneBottom);
				theApp.m_ScreenManager.CreateAllScreen(vdstBottom);
				break;
			case IDR_PANE_TOP:
				break;
			}
		}

		return TRUE; // handled
	}

	return FALSE;
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CT360VidStitchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CT360VidStitchDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rcClient(0, 0, cx, cy);
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, 0, 0, &rcClient);
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rcClient, &rcClient);

	printf("bigheadk, OnSize!!\n");
	if (!isFullScreen)
	{
		if (::IsWindow(m_wndCenterViewLabel.m_hWnd))
		{
			CRect rc;
			m_wndCenterViewLabel.GetWindowRect(&rc);
			ScreenToClient(&rc);

			int x = rcClient.left;
			int y = 30;
			int cx = 15;//rcClient.Width();
			int cy = rc.Height();

			rc.SetRect(CPoint(x, y), CPoint(x + cx, y + 15));
			m_btnCurView.MoveWindow(rc);
			m_btnCurView.ShowWindow(SW_SHOW);

			x = rc.right + 7;		// Icon 영역과 Project Name 역역 간격 7pix
			cx = (rcClient.Width() - 24) / 9 * 8;
			rc.SetRect(CPoint(x, y), CPoint(x + cx, y + cy));
			m_wndCenterViewLabel.MoveWindow(rc);
			m_wndCenterViewLabel.ShowWindow(SW_SHOW);

			x = rc.right + 80;
			cx = (rcClient.Width() - 24) / 9 - 7 - 80;
			rc.SetRect(CPoint(x, y), CPoint(x + cx - 11, y + cy));		// txtFps  Right 위치 - 11 Pix
			m_txtFps.MoveWindow(rc);
			m_txtFps.ShowWindow(SW_SHOW);

			x = rc.right + 11;											// txtFps 줄어든 범위만큼 증가 
			cx = 10;
			rc.SetRect(CPoint(x, y + 3), CPoint(x + cx, y + 3 + cx));
			m_btnViewClose.MoveWindow(rc);
			m_btnViewClose.ShowWindow(SW_SHOW);

			rcClient.top += y + cy + 5;
		}

		if (::IsWindow(m_wndCenterViewFrame.m_hWnd))
		{
			m_wndCenterViewFrame.MoveWindow(rcClient);
			//m_wndCenterViewFrame.Invalidate();

			m_wndCenterViewFrame.GetClientRect(rcClient);
			theApp.m_ScreenManager.SetRCDlg(rcClient);
			theApp.m_ScreenManager.ShowScreens(theApp.m_ScreenManager.GetAcitveScreen(), true);
			theApp.m_ScreenManager.updateSubMenu();
		}

		if ((CCenterView*)theApp.m_ScreenManager.m_pCenterView != NULL)
		{
			CRect center_rc;
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->GetClientRect(center_rc);
			if ((theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side) && theApp.m_ScreenManager.m_ActiveWnd != vdstNewLoad)
			{
				if ((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild != NULL)
				{
					CRect left_rc;
					CLeftChildDlg* m_pLeft = (CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild;
					m_pLeft->GetClientRect(left_rc);
					m_pLeft->MoveWindow(0, 0, left_rc.Width(), center_rc.bottom + 50, SWP_NOMOVE);            // 크기는 알아서 맞게 지정

					m_pLeft->setStereoPairUpdate();
				}
			}

			if (theApp.m_ScreenManager.m_ActiveWnd == vdstRightOverlayListView)
			{
				if ((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild != NULL)
				{
					CRect right_rc;
					CRightOverlayListChildDlg* m_pVIew = (CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild;
					m_pVIew->GetClientRect(right_rc);
					m_pVIew->MoveWindow(0, 0, right_rc.Width(), center_rc.bottom + 50, SWP_NOMOVE);            // 크기는 알아서 맞게 지정

					m_pVIew->dlgUpdate();
				}
			}

			if (theApp.m_ScreenManager.m_ActiveWnd == vdstRightOverlayView)
			{
				if ((RightOverlayChildDlg*)theApp.m_ScreenManager.m_pRightOverlayChild != NULL)
				{
					CRect right_rc, btnRect;
					RightOverlayChildDlg* m_pVIew = (RightOverlayChildDlg*)theApp.m_ScreenManager.m_pRightOverlayChild;
					m_pVIew->GetClientRect(right_rc);
					m_pVIew->MoveWindow(0, 0, right_rc.Width(), center_rc.bottom + 50, SWP_NOMOVE);            // 크기는 알아서 맞게 지정
					
					m_pVIew->dlgUpdate();
				}
			}
		}
	}
	else
	{
		CRect MyRect;

		CWnd::GetDesktopWindow()->GetWindowRect(MyRect);

		printf("bigheadk, %d, %d, %d, %d\n", MyRect.top, MyRect.left, MyRect.right, MyRect.bottom);

		if (::IsWindow(m_wndCenterViewLabel.m_hWnd))
		{
			CRect rc;
			m_wndCenterViewLabel.GetWindowRect(&rc);
			ScreenToClient(&rc);

			int x = rcClient.left;
			int y = rcClient.top;
			int cx = 15;//rcClient.Width();
			int cy = rc.Height();

			rc.SetRect(CPoint(x, y), CPoint(x + cx, y + 15));
			m_btnCurView.MoveWindow(rc);
			m_btnCurView.ShowWindow(SW_HIDE);

			x = rc.right + 7;		// Icon 영역과 Project Name 역역 간격 7pix
			cx = (rcClient.Width() - 24) / 9 * 8;
			rc.SetRect(CPoint(x, y), CPoint(x + cx, y + cy));
			m_wndCenterViewLabel.MoveWindow(rc);
			m_wndCenterViewLabel.ShowWindow(SW_HIDE);

			x = rc.right + 80;
			cx = (rcClient.Width() - 24) / 9 - 7 - 80;
			rc.SetRect(CPoint(x, y), CPoint(x + cx - 11, y + cy));		// txtFps  Right 위치 - 11 Pix
			m_txtFps.MoveWindow(rc);
			m_txtFps.ShowWindow(SW_HIDE);

			x = rc.right + 11;											// txtFps 줄어든 범위만큼 증가 
			cx = 9;
			rc.SetRect(CPoint(x, y + 3), CPoint(x + cx, y + 3 + 9));
			m_btnViewClose.MoveWindow(rc);
			m_btnViewClose.ShowWindow(SW_HIDE);

			rcClient.top += cy + 5;
		}

		if (::IsWindow(m_wndCenterViewFrame.m_hWnd))
		{
			m_wndCenterViewFrame.MoveWindow(MyRect);
			//m_wndCenterViewFrame.Invalidate();

			m_wndCenterViewFrame.GetClientRect(rcClient);
			theApp.m_ScreenManager.SetRCDlg(MyRect);
			theApp.m_ScreenManager.ShowScreens(theApp.m_ScreenManager.GetAcitveScreen(), false, true);
		}
	}
}


void CT360VidStitchDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (lpwndpos->cx <= MIN_APP_WIDTH) lpwndpos->cx = MIN_APP_WIDTH;
	if (lpwndpos->cy <= MIN_APP_HEIGHT) lpwndpos->cy = MIN_APP_HEIGHT;
}


void CT360VidStitchDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = MIN_APP_WIDTH;
	lpMMI->ptMinTrackSize.y = MIN_APP_HEIGHT;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CT360VidStitchDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMER_STARTAPP)
	{
		KillTimer(TIMER_STARTAPP);
	}

	if (nIDEvent == TIMER_ADDINGOVERLAY)
	{
		KillTimer(TIMER_ADDINGOVERLAY);
		theApp.m_ScreenManager.ShowToast(vdstToastDone, _T(" "), FALSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CT360VidStitchDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	DestoryFontObj();
	theApp.m_ScreenManager.DeleteAllScreen();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	free(fps);
}


void CT360VidStitchDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);
	if(theApp.m_ScreenManager.m_pCenterView)
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->m_bMovingMainWnd = FALSE;	//GL갱신 관련
	if (theApp.m_ScreenManager.GetAcitveScreen() != vdstNewLoad)
	{
		theApp.m_ScreenManager.MoveToast();
	}
}

void CT360VidStitchDlg::UpdateFPS()
{
	CString szFPS;
	static DOUBLE OldTimer = 0;
	static int oldTick = TQClock();
	//double diff = abs(GetTickCount() - OldTimer);
	//fps->push(diff);

	int TickGap = 0, nCurentTick = TQClock();
	TickGap = nCurentTick - oldTick;

	//m_strFPS.Format(_T("FPS:  %5.1f fps %5.1f ms"), 1000.0f / fps->avg(), fps->avg());
	m_strFPS.Format(_T("FPS:  %5.1f"), 1000.0f / fps->avg());

	OldTimer = TQClock();
	if (TickGap >= 300)
	{
		PostMessage(WM_UPDATESTRINGFPS, 0, 0);
		oldTick = nCurentTick;
	}
}

HBRUSH CT360VidStitchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

// 	switch (pWnd->GetDlgCtrlID())
// 	{
// 	case IDC_MAIN_CHILD:
// 	{
// 		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
// 		pDC->SetBkMode(TRANSPARENT);
// 		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
// 	}
// 	break;
// 	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CT360VidStitchDlg::OnBnClickedBtnviewclose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (theApp.m_ScreenManager.m_iStartType == -1)		 
		return; 

	vdstScreenCenter centerMode = theApp.m_ScreenManager.getScreenMode();
	
	CCenterView * pCenterView = (CCenterView *)(theApp.m_ScreenManager.m_pCenterView);
	if (pCenterView)
	{
		if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isOverlayEdit)
			((RightOverlayChildDlg*)theApp.m_ScreenManager.m_pRightOverlayChild)->OnBnClickedBtneditoverlay();

		if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isAddOverlayDlgShow)
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowAddOverlay(FALSE);

		char* szpPath = CString2Char(theApp.m_PrjAllCfg.m_szProjectFullPath + _T("\\") + theApp.m_PrjAllCfg.m_szProjectName);
		BOOL bIsClose = ((CCenterView*)pCenterView)->IsNeedSaveParam(szpPath);
		free(szpPath);
		// 리턴 값이 TURE 일때 종료 
		if (!bIsClose)
			return;

		((CConfiguView*)theApp.m_ScreenManager.m_pConfigView)->clearValue(CConfiguView::childCalibration);
		theApp.m_ScreenManager.plan_type = QVS_PLANTYPE_DEMO;
		pCenterView->ShowLogViewer(FALSE);
		theApp.m_ScreenManager.ShowToast(vdstToastHideAll);
		QVS_Engine::QVS_clearLog();
		QVS_Engine::QVS_destroy();
		
		//theApp.m_ScreenManager.saveProject();
		theApp.OnInitProjertConfig();

		pCenterView->templateLoad = FALSE;
		pCenterView->m_init = FALSE;
		pCenterView->m_play = FALSE;

		theApp.m_PrjAllCfg.m_bInitQVS_EngineDone = false;
		theApp.m_PrjAllCfg.m_bFirstUpdateBtmSource = false;
		theApp.m_PrjAllCfg.m_bFirstUpdateBtmPano = false;
		theApp.m_PrjAllCfg.m_restart = true;

		theApp.m_ScreenManager.ShowScreens(vdstNewLoad);
		//theApp.m_ScreenManager.DeleteMainView();
		theApp.m_ScreenManager.SetProjectName();
		theApp.m_ScreenManager.m_iStartType = -1;

		//UI 초기화
		CConfiguView* pConfiguView = (CConfiguView*)theApp.m_ScreenManager.m_pConfigView;
		if (pConfiguView)
			pConfiguView->m_activeView = CConfiguView::childOuputs;
		CBottomPanoChildDlg* pBottomPanoChild = (CBottomPanoChildDlg*)theApp.m_ScreenManager.m_pBottomPanoChild;
		if (pBottomPanoChild)
		{
			pBottomPanoChild->m_sldBrightness.SetPos(10);
			pBottomPanoChild->GetDlgItem(IDC_EDT_BRIGHTNESS)->SetWindowText(_T("0"));
			pBottomPanoChild->m_sldContrast.SetPos(0);
			pBottomPanoChild->GetDlgItem(IDC_EDT_CONTRAST)->SetWindowText(_T("0"));

			pBottomPanoChild->setGrayscale = GRAYSCALE_ON;
			QVS_Engine::QVS_setGrayscaleMode(pBottomPanoChild->setGrayscale);
			pBottomPanoChild->m_btnGrayOnOff.SetImage(CGdipButton::STD_TYPE);
		}
	}
}

void CT360VidStitchDlg::ToggleIcon(int Index)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch (Index)
	{
	/*case vdstRightManualCalibView:
	case vdstRightAdvanceManualView:
	case vdstRightAdvanceUpdateView:
	case vdstRightAdvanceAlignView:
	case vdstRightOverlayView:
	case vdstRightAdvanceView:
	case vdstRightCalibView:
	case vdstPanoramaView: m_btnCurView.LoadBitmaps(IDBB_BTN_WINICON_PANO, NULL, NULL, NULL);
		break;
	case vdstSourceView: m_btnCurView.LoadBitmaps(IDBB_BTN_WINICON_SOURCES, 0, 0, 0);
		break;
	case vdstInteractiveView: m_btnCurView.LoadBitmaps(IDBB_BTN_WINICON_INTERACT, 0, 0, 0);
		break;
	case vdstConfiguView: m_btnCurView.LoadBitmaps(IDBB_BTN_WINICON_CONFIG, 0, 0, 0);
		break;*/
	default: m_btnCurView.LoadBitmaps(IDBB_BTN_WINICON_NONE, 0, 0, 0);
		break;
	}
	m_btnCurView.Invalidate(FALSE);
}

void CT360VidStitchDlg::EnableTitle(int nState)
{
	m_wndCenterViewLabel.ShowWindow(nState);
	m_txtFps.ShowWindow(nState);
	m_btnViewClose.ShowWindow(nState);
}

void CT360VidStitchDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//printf("======> OnClose() start!!\n");
	vdstScreenCenter centerMode = theApp.m_ScreenManager.getScreenMode();

	CCenterView * pCenterView = (CCenterView *)(theApp.m_ScreenManager.m_pCenterView);
	if (pCenterView)
	{
		//CCenterView * pCenterView = (CCenterView *)RUNTIME_CLASS(CCenterView)->CreateObject();
		char* szpPath = CString2Char(theApp.m_PrjAllCfg.m_szProjectFullPath + _T("\\") + theApp.m_PrjAllCfg.m_szProjectName);
		BOOL bIsClose = ((CCenterView*)pCenterView)->IsNeedSaveParam(szpPath);
		free(szpPath);
		// 리턴 값이 TURE 일때 종료 

		if (!bIsClose)
			return;
		else{
			//QVS_Engine::QVS_saveLogFile();

			theApp.m_ScreenManager.ShowToast(vdstToastHideAll);
			pCenterView->ShowLogViewer(FALSE);
			QVS_Engine::QVS_clearLog();
			QVS_Engine::QVS_destroy();
			//==========================================
			theApp.OnInitProjertConfig();
			pCenterView->templateLoad = FALSE;
			pCenterView->m_init = FALSE;
			theApp.m_PrjAllCfg.m_bInitQVS_EngineDone = false;
			theApp.m_PrjAllCfg.m_bFirstUpdateBtmSource = false;
			theApp.m_PrjAllCfg.m_bFirstUpdateBtmPano = false;
			theApp.m_PrjAllCfg.m_restart = true;
			theApp.m_ScreenManager.ShowScreens(vdstNewLoad);
			theApp.m_ScreenManager.SetProjectName();
			theApp.m_ScreenManager.m_iStartType = -1;
			//=========================================
		}
	}
	CDialogEx::OnClose();
	//printf("======> OnClose() end!!\n");
}


void CT360VidStitchDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	static bool once = true;

	// 창이 보이는 시점이 프로그램 실행후 최초인지 검사하는 구문
	if (bShow && !IsWindowVisible() && once)
	{
		once = false;
		POINT min, max;
		min.x = 0;
		min.y = 0;
		max.x = MIN_APP_WIDTH;
		max.y = MIN_APP_HEIGHT;

		WINDOWPLACEMENT lwp;
		lwp.length = sizeof(WINDOWPLACEMENT);
		lwp.flags = SW_SHOWMINIMIZED | WPF_SETMINPOSITION;
		lwp.showCmd = SW_SHOW;
		lwp.ptMinPosition = min;
		lwp.ptMaxPosition = max;
		lwp.rcNormalPosition = CRect(min, max);
		SetWindowPlacement(&lwp);
	}
}

void CT360VidStitchDlg::onKeyEvent(MSG* pMsg)
{
	vdstScreenCenter centerMode = theApp.m_ScreenManager.getScreenMode();

	if (centerMode != vdstNewLoad && centerMode != vdstRightOverlayView)
	{

		//Preparemode ShortKey
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == VK_F9 /*'F9'*/)
		{
			printf("CTRL + key F9\n");
			((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild)->OnBnClickedBtnpreparemode();
		}

		//Stereo Pair ShortKey
		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && pMsg->wParam == 0x31 /*'1'*/)
		{
			printf("SHIFT + key 1\n");
			if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
				((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild)->OnBnClickedBtnstereoPair0();
		}

		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && pMsg->wParam == 0x32 /*'2'*/)
		{
			printf("SHIFT + key 2\n");
			if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
				((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild)->OnBnClickedBtnstereoPair1();
		}

		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && pMsg->wParam == 0x33 /*'3'*/)
		{
			printf("SHIFT + key 3\n");
			if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
				((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild)->OnBnClickedBtnstereoPair2();
		}

		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && pMsg->wParam == 0x34 /*'4'*/)
		{
			printf("SHIFT + key 4\n");
			if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
				((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild)->OnBnClickedBtnstereoPair3();
		}

		//Overlay ShortKey
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x31 /*'1'*/)
		{
			printf("CTRL + key 1\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(0, true);
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x32 /*'2'*/)
		{
			printf("CTRL + key 2\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(1, true);
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x33 /*'3'*/)
		{
			printf("CTRL + key 3\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(2, true);
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x34 /*'4'*/)
		{
			printf("CTRL + key 4\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(3, true);
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x35 /*'5'*/)
		{
			printf("CTRL + key 5\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(4, true);
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x36 /*'6'*/)
		{
			printf("CTRL + key 6\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(5, true);
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x37 /*'7'*/)
		{
			printf("CTRL + key 7\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(6, true);
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x38 /*'8'*/)
		{
			printf("CTRL + key 8\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(7, true);
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x39 /*'9'*/)
		{
			printf("CTRL + key 9\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(8, true);
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x30 /*'0'*/)
		{
			printf("CTRL + key 0\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(9, true);
		}

		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x31 /*'1'*/)
		{
			printf("ALT + key 1\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(10, true);
		}
		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x32 /*'2'*/)
		{
			printf("ALT + key 2\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(11, true);
		}
		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x33 /*'3'*/)
		{
			printf("ALT + key 3\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(12, true);
		}
		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x34 /*'4'*/)
		{
			printf("ALT + key 4\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(13, true);
		}
		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x35 /*'5'*/)
		{
			printf("ALT + key 5\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(14, true);
		}
		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x36 /*'6'*/)
		{
			printf("ALT + key 6\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(15, true);
		}
		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x37 /*'7'*/)
		{
			printf("ALT + key 7\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(16, true);
		}
		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x38 /*'8'*/)
		{
			printf("ALT + key 8\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(17, true);
		}
		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x39 /*'9'*/)
		{
			printf("ALT + key 9\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(18, true);
		}
		if (pMsg->message == WM_SYSKEYDOWN /*ALT key*/ && pMsg->wParam == 0x30 /*'0'*/)
		{
			printf("ALT + key 0\n");
			((CRightOverlayListChildDlg*)theApp.m_ScreenManager.m_pRightOverlayListChild)->setOverlayTemplateState(19, true);
		}
	}

	if (!QVS_Engine::QVS_getIsDoingAnimation())
	{
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x5A /*'z'*/ && (GetAsyncKeyState(VK_SHIFT) & 0x8000) == FALSE && pMsg->message != WM_SYSKEYDOWN /*ALT key*/)
		{
			if (centerMode != vdstSourceView && centerMode != vdstConfiguView && centerMode != vdstRightManualCalibView && centerMode != vdstRightOverlayView && 
				QVS_Engine::QVS_undoEnabled() && theApp.m_ScreenManager.getSubMode() != SUBMODE_EDIT_ROTATION && !theApp.m_ScreenManager.isDoingProgToast)
			{
				printf("CTRL + Z\n");
				QVS_Engine::QVS_undo();
			}
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && pMsg->wParam == 0x5A /*'z'*/ && pMsg->message != WM_SYSKEYDOWN /*ALT key*/)
		{
			if (centerMode != vdstSourceView && centerMode != vdstConfiguView && centerMode != vdstRightManualCalibView && centerMode != vdstRightOverlayView &&
				QVS_Engine::QVS_redoEnabled() && theApp.m_ScreenManager.getSubMode() != SUBMODE_EDIT_ROTATION && !theApp.m_ScreenManager.isDoingProgToast)
			{
				printf("CTRL + SHIFT + Z\n");
				QVS_Engine::QVS_redo();
			}
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x59 /*'y'*/ && (GetAsyncKeyState(VK_SHIFT) & 0x8000) == FALSE && pMsg->message != WM_SYSKEYDOWN /*ALT key*/)
		{
			if (centerMode != vdstSourceView && centerMode != vdstConfiguView && centerMode != vdstRightManualCalibView && centerMode != vdstRightOverlayView &&
				QVS_Engine::QVS_redoEnabled() && theApp.m_ScreenManager.getSubMode() != SUBMODE_EDIT_ROTATION && !theApp.m_ScreenManager.isDoingProgToast)
			{
				printf("CTRL + Y\n");
				QVS_Engine::QVS_redo();
			}
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && pMsg->wParam == 0x53 /*'s'*/)
		{
			if (centerMode != vdstNewLoad && !theApp.m_ScreenManager.isDoingProgToast && !((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isOverlayEdit)
			{
				printf("CTRL + S\n");
				QVS_Engine::QVS_SaveParam();
				MessageBox(_T("VRue360 : Save this project."), MB_OK, MB_ICONINFORMATION);
			}
		}
	}

	if (centerMode == vdstSourceView || centerMode == vdstPanoramaView || centerMode == vdstInteractiveView)
	{
		if (pMsg->wParam == VK_RETURN && pMsg->message == WM_SYSKEYDOWN /*ALT key*/ &&
			!(((CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild)->isCParamEdit) &&
			!(((CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild)->isCameraSel))
		{
			if (!isFullScreen)
			{
				isFullScreen = true;
				ShowCursor(FALSE);
				//preMenu = GetMenu();

				ModifyStyle(WS_CAPTION, NULL);   // 타이틀바 없애기
				SetMenu(NULL);       // 메뉴바 없애기

				int windowWidth = GetSystemMetrics(SM_CXSCREEN);
				int windowHeight = GetSystemMetrics(SM_CYSCREEN);
				_style = ::GetWindowLong(this->m_hWnd, GWL_STYLE);
				::GetWindowRect(this->m_hWnd, &_originWindow);

				int style = ::GetWindowLong(this->m_hWnd, GWL_STYLE);
				style &= ~(WS_DLGFRAME | WS_THICKFRAME);
				::SetWindowLong(this->m_hWnd, GWL_STYLE, style);
				
				//::SetWindowLong(this->m_hWnd, GWL_STYLE, NULL);
				SetWindowPos(&wndTopMost, 0, 0, windowWidth, windowHeight, NULL);
				ShowWindow(SW_MAXIMIZE);
			}
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			if (isFullScreen)
			{
				isFullScreen = false;
				::SetWindowLong(this->m_hWnd, GWL_STYLE, _style);
				//GetParent()
				SetWindowPos(&wndNoTopMost, _originWindow.left, _originWindow.top,
					_originWindow.right - _originWindow.left, _originWindow.bottom - _originWindow.top, NULL);

				CMenu newMenu;
				newMenu.LoadMenu(IDR_MAINFRAME);

				//HMENU menu = LoadMenu();
				ASSERT(newMenu);
				SetMenu(&newMenu);
				//m_hMenuDefault = newMenu.GetSafeHmenu();

				ModifyStyle(WS_DLGFRAME, WS_CAPTION, SWP_FRAMECHANGED);

				ShowCursor(TRUE);

				//pView->isFullScreen = false;
			}
		}
	}
	if (centerMode == vdstInteractiveView)
	{
		((CCenterView*)(theApp.m_ScreenManager.m_pCenterView))->onKeyUP(pMsg);
	}
}


BOOL CT360VidStitchDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

CFont * CT360VidStitchDlg::GetFontObj(int index)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFont * pFont = NULL;

	switch (index)
	{
		case Aria10: pFont = &m_fontArial_10;
			break;
		case TAria11: pFont = &m_fontArial_T11;
			break;
		case Aria11: pFont = &m_fontArial_11;
			break;
		case Aria12: pFont = &m_fontArial_12;
			break;
		case BAria12: pFont = &m_fontArial_B12;
			break;
		case BAria13: pFont = &m_fontArial_B13;
			break;
	}

	return pFont;
}

void CT360VidStitchDlg::CreateFontObj()
{
	/////////////////////////////////////////////////////////////////

	m_fontArial_10.CreateFont(		  // 10pix , Arial
		13,                           // 문자 폭
	 	0,                           // 문자 높이
	 	0,                            // 문자 기울기
	 	0,                            // 문자 방향
	 	FW_NORMAL,                    // 문자 굵기
	 	FALSE,                        // 기울기
	 	FALSE,                        // 밑줄
	 	FALSE,                        // 취소선
	 	DEFAULT_CHARSET,			  // 문자셋
	 	OUT_DEFAULT_PRECIS,			  // 출력 정확도
	 	CLIP_DEFAULT_PRECIS,		  // 클리핑 정확도
	 	DEFAULT_QUALITY,			  // 출력의 질
	 	DEFAULT_PITCH,				  // 자간
	 	_T("Arial") );				  // 폰트

	m_fontArial_10R.CreateFont(		  // 10pix , Arial , 우측정렬
		13,                           // 문자 폭
		0,                            // 문자 높이
		0,                            // 문자 기울기
		TA_RIGHT,					  // 문자 방향
		FW_NORMAL,                    // 문자 굵기
		FALSE,                        // 기울기
		FALSE,                        // 밑줄
		FALSE,                        // 취소선
		DEFAULT_CHARSET,			  // 문자셋
		OUT_DEFAULT_PRECIS,			  // 출력 정확도
		CLIP_DEFAULT_PRECIS,		  // 클리핑 정확도
		DEFAULT_QUALITY,			  // 출력의 질
		DEFAULT_PITCH,				  // 자간
		_T("Arial"));				  // 폰트

	m_fontArial_T11.CreateFont(		  // 11pix , Arial
		14,                           // 문자 폭
		0,                            // 문자 높이
		0,                            // 문자 기울기
		0,                            // 문자 방향
		FW_THIN,                    // 문자 굵기
		FALSE,                        // 기울기
		FALSE,                        // 밑줄
		FALSE,                        // 취소선
		DEFAULT_CHARSET,			  // 문자셋
		OUT_DEFAULT_PRECIS,			  // 출력 정확도
		CLIP_DEFAULT_PRECIS,		  // 클리핑 정확도
		DEFAULT_QUALITY,			  // 출력의 질
		DEFAULT_PITCH,				  // 자간
		_T("Arial"));				  // 폰트

	m_fontArial_11.CreateFont(		  // 11pix , Arial
		14,                           // 문자 폭
		0,                            // 문자 높이
		0,                            // 문자 기울기
		0,                            // 문자 방향
		FW_NORMAL,                    // 문자 굵기
		FALSE,                        // 기울기
		FALSE,                        // 밑줄
		FALSE,                        // 취소선
		DEFAULT_CHARSET,			  // 문자셋
		OUT_DEFAULT_PRECIS,			  // 출력 정확도
		CLIP_DEFAULT_PRECIS,		  // 클리핑 정확도
		DEFAULT_QUALITY,			  // 출력의 질
		DEFAULT_PITCH,				  // 자간
		_T("Arial"));				  // 폰트

	m_fontArial_12.CreateFont(		  // 12pix , Arial
		16,                           // 문자 폭
		0,                            // 문자 높이
		0,                            // 문자 기울기
		0,                            // 문자 방향
		FW_NORMAL,                    // 문자 굵기
		FALSE,                        // 기울기
		FALSE,                        // 밑줄
		FALSE,                        // 취소선
		DEFAULT_CHARSET,			  // 문자셋
		OUT_DEFAULT_PRECIS,			  // 출력 정확도
		CLIP_DEFAULT_PRECIS,		  // 클리핑 정확도
		DEFAULT_QUALITY,			  // 출력의 질
		DEFAULT_PITCH,				  // 자간
		_T("Arial"));				  // 폰트

	m_fontArial_B12.CreateFont(		  // 12pix , Arial
		16,                           // 문자 폭
		0,                            // 문자 높이
		0,                            // 문자 기울기
		0,                            // 문자 방향
		FW_BOLD,                      // 문자 굵기
		FALSE,                        // 기울기
		FALSE,                        // 밑줄
		FALSE,                        // 취소선
		DEFAULT_CHARSET,			  // 문자셋
		OUT_DEFAULT_PRECIS,			  // 출력 정확도
		CLIP_DEFAULT_PRECIS,		  // 클리핑 정확도
		DEFAULT_QUALITY,			  // 출력의 질
		DEFAULT_PITCH,				  // 자간
		_T("Arial"));				  // 폰트

	m_fontArial_B13.CreateFont(		  // 13pix , Arial
		17,                           // 문자 폭
		0,                            // 문자 높이
		0,                            // 문자 기울기
		0,                            // 문자 방향
		FW_BOLD,                    // 문자 굵기
		FALSE,                        // 기울기
		FALSE,                        // 밑줄
		FALSE,                        // 취소선
		DEFAULT_CHARSET,			  // 문자셋
		OUT_DEFAULT_PRECIS,			  // 출력 정확도
		CLIP_DEFAULT_PRECIS,		  // 클리핑 정확도
		DEFAULT_QUALITY,			  // 출력의 질
		DEFAULT_PITCH,				  // 자간
		_T("Arial"));


	/*

	12px   9pt   0.75em    75%      
	13px   10pt  0.813em   81.3%    
	14px   11pt  0.875em   87.5%    
	16px   12pt  1em       100%     
	17px   13pt  1.063em   106.3%   

	19px   14pt  1.1em	   110%
	20px   15pt  1.25em    125%
	21px   16pt  1.3em	   130%
	23px   17pt  1.45em    145%
	24px   18pt  1.5em	   150%
	25px   19pt  1.55em    155%
	27px   20pt  1.7em	   170%

	*/
}

void CT360VidStitchDlg::DestoryFontObj()
{
	m_fontArial_10.DeleteObject();
	m_fontArial_10R.DeleteObject();
	m_fontArial_11.DeleteObject();
	m_fontArial_12.DeleteObject();
	m_fontArial_B12.DeleteObject();
	m_fontArial_B13.DeleteObject();
}

void CT360VidStitchDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);
#ifdef _DEBUG
	TRACE("OnMoving\n");
#endif
	if (theApp.m_ScreenManager.m_pCenterView)
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->m_bMovingMainWnd = TRUE; //GL갱신 관련
}


void CT360VidStitchDlg::Wait(double dwMillisecond)
{
	MSG msg;
	double dwStart, newEnd;
	newEnd = dwStart = TQClock();

	while (newEnd - dwStart < dwMillisecond)
	{
		newEnd = TQClock();
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}