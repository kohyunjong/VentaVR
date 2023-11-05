// FormView2.cpp : implementation file
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "NewLoadView.h"
#include "locale.h"

#include "NewConfigDlg.h"

// CNewLoadView

IMPLEMENT_DYNCREATE(CNewLoadView, CFormView)

CNewLoadView::CNewLoadView()
	: CFormView(CNewLoadView::IDD)
{
	m_pNewLoadConfigChild = NULL;
}

CNewLoadView::~CNewLoadView()
{
	if (m_pNewLoadConfigChild)
		delete m_pNewLoadConfigChild;
}

void CNewLoadView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_NEW, m_btnNew);
	DDX_Control(pDX, IDC_BTN_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_STATIC_RECENTTITLE, m_staticRecentTitle);
	DDX_Control(pDX, IDC_LIST_RECENT, m_listRecentProject);
	DDX_Control(pDX, IDC_URL_RELEASENOTE, m_releasenote_url);
	DDX_Control(pDX, IDC_STATICFRAME_NEWLOAD, m_ConfiguChildFrame);
	DDX_Control(pDX, IDC_BTN_URL, m_btnHomeURL);
	DDX_Control(pDX, IDC_STATIC_NEWPROJECT, m_staticNewProjectTitle);
	DDX_Control(pDX, IDC_SYSLINK_CONTACT, m_linkContact);
}

BEGIN_MESSAGE_MAP(CNewLoadView, CFormView)
	ON_BN_CLICKED(IDC_BTN_NEW, &CNewLoadView::OnBnClickedBtnNew)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CNewLoadView::OnBnClickedBtnLoad)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RECENT, &CNewLoadView::OnNMDblclkListRecent)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_RECENT, &CNewLoadView::OnNMCustomdrawListRecent)
	ON_WM_TIMER()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_URL, &CNewLoadView::OnBnClickedBtnUrl)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_CONTACT, &CNewLoadView::OnNMClickSyslinkContact)
END_MESSAGE_MAP()


// CNewLoadView diagnostics

#ifdef _DEBUG
void CNewLoadView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNewLoadView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void CNewLoadView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CRect rc, rc1;
	CRect rcClient(0, 0, cx, cy);
	GetClientRect(rcClient);

	int titleWidth, titleHeight;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (::IsWindow(m_staticRecentTitle.m_hWnd))
	{
		m_staticRecentTitle.GetClientRect(&rc);
		titleWidth = rc.Width();
		titleHeight = rc.Height();

		//rc.left = rcClient.Width() / 2 + LOADRESENTSCREENGQP/*Right Pane Width Size*/;
		rc.left = (rcClient.Width() - LOADRESENTSCREENGQP - 200) * 2 / 3 - 20;
		rc.top = rcClient.Height() / 2 - LOADBTNWIDTH/*Bottom Pane Height Size*/;
		rc.right = rc.left + titleWidth;
		rc.bottom = rc.top + titleHeight;
		m_staticRecentTitle.MoveWindow(rc);

		//rc.left = rcClient.Width() / 2/*Right Pane Width Size*/;
		//rc.top = rcClient.Height() / 2/*Bottom Pane Height Size*/;
	}

	if (::IsWindow(m_btnLoad.m_hWnd))
	{
		//m_btnNew.GetClientRect(&rc1);

		//rc1.left = rc.left;
		//rc1.top = rc.top - LOADBTNWIDTH;		// NEWLOADSCREENGQP 은 버튼과 Recent 영역의 사이 공백
		//rc1.right = rc1.left + LOADBTNWIDTH;
		//rc1.bottom = rc1.top + LOADBTNWIDTH;
		//m_btnNew.MoveWindow(rc1);

		//m_btnLoad.m_hWnd;
		m_btnLoad.GetClientRect(&rc1);
		int nWidth = rc1.Width();
		int nHeight = rc1.Height();

		rc1.left = rc.left + LOADBTNWIDTH * 2 + NEWLOADSCREENGQP - nWidth;
		rc1.top = rc.top - 5;		// NEWLOADSCREENGQP 은 버튼과 Recent 영역의 사이 공백
		rc1.right = rc1.left + nWidth;
		rc1.bottom = rc1.top + nHeight;

		m_btnLoad.MoveWindow(rc1);
	}

	if (::IsWindow(m_listRecentProject.m_hWnd))
	{

		rc1.left = rc.left - 1;
		rc1.right = rc.left + LOADBTNWIDTH * 2 + NEWLOADSCREENGQP;
		rc1.top = rc.top + NEWLOADSCREENGQP - 20;
		//rc1.top = rc.top + 20;
		rc1.bottom = rc.top + LOADBTNWIDTH * 2 - NEWLOADSCREENGQP;
		//rc1.bottom = rc1.top + 130;
		m_listRecentProject.MoveWindow(rc1);

		//rc.top = rcClient.Height() / 2/*Bottom Pane Height Size*/;
	}

	if (::IsWindow(m_staticNewProjectTitle.m_hWnd))
	{
		int x = (rcClient.Width() - LOADRESENTSCREENGQP - 1100) / 3 + 10;
		int y = rc.top;
		CRect _rc = CRect(x, y, x + titleWidth, y + titleHeight);
		m_staticNewProjectTitle.MoveWindow(_rc);
	}

	if (::IsWindow(m_ConfiguChildFrame.m_hWnd))
	{
		int nWidth = rcClient.Width();
		int logoW = 600;
		int logoH = 150 / 2;
		int x = (nWidth - LOADRESENTSCREENGQP - 1100) / 3;
		int y = rc.top + NEWLOADSCREENGQP - 20;

		CRect _rc = CRect(x, y, x + logoW, rc1.bottom);

		m_ConfiguChildFrame.MoveWindow(_rc);
		m_pNewLoadConfigChild->MoveWindow(CRect(0, 0, _rc.Width(), _rc.Height()));
	}

	if (::IsWindow(m_btnHomeURL.m_hWnd))
	{
		m_btnHomeURL.GetClientRect(&rc1);
		int nHeight = rc1.Height();
		int nWidth = rc1.Width();

#if(!LICENSE_POWERED_BY_LG)
		rc1.left = NEWLOADSCREENGQP;
		rc1.right = NEWLOADSCREENGQP + nWidth;
		rc1.top = rcClient.bottom - nHeight - NEWLOADSCREENGQP;
		rc1.bottom = rcClient.bottom - NEWLOADSCREENGQP;
		m_btnHomeURL.MoveWindow(rc1);
#else
		rc1.left = NEWLOADSCREENGQP / 2;
		rc1.right = NEWLOADSCREENGQP / 2 + nWidth;
		rc1.top = rcClient.bottom - nHeight - NEWLOADSCREENGQP + 15;
		rc1.bottom = rc1.top + nHeight;
		m_btnHomeURL.MoveWindow(rc1);
		m_btnHomeURL.EnableWindow(FALSE);

		//width = 257, height = 23
		//rc1.left = rc1.right;
		rc1.left += 12;
		rc1.right = rc1.left + 300;
		rc1.top = rc1.bottom - 15;
		rc1.bottom = rc1.top + 25;
		m_linkContact.MoveWindow(rc1);
#endif
		Invalidate(FALSE);
	}
}

// CNewLoadView message handlers
void CNewLoadView::OnBnClickedBtnNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_btnLoad.SetImage(CGdipButton::STD_TYPE);
	//m_btnNew.SetImage(CGdipButton::ALT_TYPE);

#if LICENSE_PERSONAL_KEY
	if (mQLicense != NULL)
		theApp.m_ScreenManager.plan_type = mQLicense->checkKeyScope(HL_key_id, HL_key_year, HL_key_month, HL_key_day);
	else
	{
		AfxMessageBox(_T("License Error1 : We are opening file as demo."), MB_OK, MB_ICONEXCLAMATION);
		theApp.m_ScreenManager.plan_type = QVS_PLANTYPE_DEMO;
	}
#endif

	CNewConfigDlg NewDlg;
	if (NewDlg.DoModal() == IDOK)
	{
		m_btnLoad.Disable(&m_btnLoad);
		//m_btnNew.Disable(&m_btnNew);

		if (!theApp.m_PrjAllCfg.m_restart)
		{
			theApp.m_ScreenManager.m_iStartType = 0;
			theApp.m_ScreenManager.CreateAllScreen(vdstCenter);//다른 화면을 만든다.
			//임시 함수...
			//theApp.m_ScreenManager.parsingPTS(_T("0712.pts"));

			//프로젝트명을 
			theApp.addToRecentFile(theApp.m_PrjAllCfg.m_szProjectFullPath + _T("\\") + theApp.m_PrjAllCfg.m_szProjectName);
			theApp.m_ScreenManager.SetProjectName();
			theApp.m_ScreenManager.saveProject();

			//여기서 모든 화면의 Config들을 설정한다.
			//theApp.m_ScreenManager.initConfiguration(typeNew);
			//Project를 생성한다.
			//theApp.m_ScreenManager.ShowScreens(vdstSourceView);
			theApp.m_ScreenManager.PreScreens(vdstSourceView);
			SetTimer(TIMER_INTO_CENTERSCREEN, 100, NULL);
		}
		else
		{
			//프로젝트명을 

			CCenterView * pCenterView = (CCenterView *)(theApp.m_ScreenManager.m_pCenterView);
			pCenterView->initQVSEngine();

			if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone == FALSE)
				return;

			theApp.addToRecentFile(theApp.m_PrjAllCfg.m_szProjectFullPath + _T("\\") + theApp.m_PrjAllCfg.m_szProjectName);
			theApp.m_ScreenManager.saveProject();

			theApp.m_ScreenManager.m_iStartType = 0;
			theApp.m_ScreenManager.SetProjectName();

			theApp.m_ScreenManager.ShowScreens(vdstSourceView);

			SetTimer(TIMER_INTO_CENTERSCREEN, 100, NULL);
		}
	}
	/*else
		m_btnNew.SetImage(CGdipButton::STD_TYPE);*/
}


void CNewLoadView::OnBnClickedBtnLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_btnNew.SetImage(CGdipButton::STD_TYPE);
	m_btnLoad.SetImage(CGdipButton::ALT_TYPE);

	const TCHAR szFilters[] = _T("q360 project files (*.q360)|*.q360||");

	CFileDialog dlg(TRUE, _T("q360"), _T("*.q360"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, NULL, 0);

	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);


	if (dlg.DoModal() == IDOK)
	{
		CString szFullpath, szFileName, szTemp;

		SetCurrentDirectory(Path);// Path의 원복

		szFullpath = dlg.GetPathName();
		szFileName = dlg.GetFileName();

		szTemp = _T("\\") + szFileName;
		szFullpath.Replace(szTemp, _T(""));

		LoadFormFile(szFullpath, szFileName);

		if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone == TRUE)
			theApp.m_ScreenManager.isNewProject = false;
	}
	else
		m_btnLoad.SetImage(CGdipButton::STD_TYPE);

}

BOOL CNewLoadView::ParseLoadValueFormConfiguFile(CString szFullpath, CString szFileName)
{
	char szTemp[1024] = { 0, };
	char* szPath = CString2Char(szFullpath);
	char* szName = CString2Char(szFileName);
	sprintf_s(szTemp, "%s\\%s", szPath, szName);

	free(szPath); free(szName);

	FILE* fp = fopen(szTemp, "rt");
	if (fp == NULL)
	{
		AfxMessageBox(L"Load ERROR1 : q360 file open fail!");
		printf("Load ERROR: PTS file open fail!\n");
		return FALSE;
	}

	// Check Quram 360 Engine Version //
	int cmp = -1;
	int release_ver;
	int major_ver;
	int minor_ver;

	do
	{
		fgets(szTemp, MAX_PATH, fp);
		cmp = strcmp(szTemp, "# version:\n");
		if (cmp == 0)
			break;
	} while (!feof(fp));

	if (cmp != 0)
	{
		release_ver = 1;
		major_ver = 1;
		minor_ver = 1;
	}
	else
	{
		fscanf(fp, "v%d.%d.%d", &(release_ver), &(major_ver), &(minor_ver));
	}

	if ((release_ver < QVS_VERSION_RELEASE) ||
		(release_ver == QVS_VERSION_RELEASE && major_ver < QVS_VERSION_MAJOR) ||
		(release_ver == QVS_VERSION_RELEASE && major_ver == QVS_VERSION_MAJOR && minor_ver < QVS_VERSION_MINOR))
	{
		CString szTemp = _T("The version of this file is lower than the current version, which can cause problems opening the file.\nDo you still want to open this file?");
		int ButtonRst = AfxMessageBox(szTemp, MB_YESNO);
		if (ButtonRst == IDNO)
			return FALSE;
	}
	else if (release_ver > QVS_VERSION_RELEASE || major_ver > QVS_VERSION_MAJOR || minor_ver > QVS_VERSION_MINOR)
	{
		AfxMessageBox(L"The version of this file is higher than the current version.\nWe can't open this file.");
		return FALSE;
	}
	// Check Quram 360 Engine Version //

	// q360 파일을 처음부터 스캔하면서 카메라 개수, width, height 값을 찾음
	char temp_w[MAX_PATH];
	bool isMore;
	// status
	isMore = false;
	while (fscanf(fp, "%s", temp_w) != EOF)
	{
		if (strcmp(temp_w, "#-new") == 0)
		{
			isMore = true;
			break;
		}
	}
	while (isMore)
	{
		if (fscanf(fp, "%s", temp_w) == EOF)
		{
			AfxMessageBox(L"Load ERROR2 : q360 file open fail!");
			printf("Load ERROR: Not found configuration value!\n");
			fclose(fp);
			return FALSE;
		}
		switch (temp_w[0])
		{
		case 'n':
			// 카메라 개수 찾음
			sscanf(temp_w, "n %d", &(theApp.m_PrjAllCfg.m_nMaxCamra));
			break;
		case 'w':
			// 카메라 width 찾음
			sscanf(temp_w, "w %d", &(theApp.m_PrjAllCfg.m_nWidth));
			break;
		case 'h':
			// 카메라 height 찾음
			sscanf(temp_w, "h %d", &(theApp.m_PrjAllCfg.m_nHeight));
			//isMore = false;
			break;

		case 'x':
			//output width
			sscanf(temp_w, "x %d", &(theApp.m_PrjAllCfg.m_nDstWidth));
			break;
		case 'y':
			//output height
			sscanf(temp_w, "y %d", &(theApp.m_PrjAllCfg.m_nDstHeight));
			break;
		case 'm':
			//stitching mode
			sscanf(temp_w, "m %d", &(theApp.m_PrjAllCfg.m_nStitchingMode));
			isMore = false;
			break;
		default:
			break;
		}
		if (strcmp(temp_w, "#-input") == 0)
		{
			break;
		}
	}
	printf("JW ParseLoadValueFormConfiguFile: (theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight) = (%d, %d)\n", theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight);
	if (fp) fclose(fp);

#if LICENSE_PERSONAL_KEY
	if (theApp.m_PrjAllCfg.m_nDstWidth == 7680 && theApp.m_PrjAllCfg.m_nDstHeight == 3840 && theApp.m_ScreenManager.plan_type == QVS_PLANTYPE_4K)
	{
		AfxMessageBox(_T("It can't open the file as this project is 8K output.\nThis license can't open 8K project."), MB_OK, MB_ICONEXCLAMATION);
		return FALSE;
	}
#endif

	if (theApp.m_PrjAllCfg.m_nDstWidth == 0 || theApp.m_PrjAllCfg.m_nDstHeight == 0)
	{
		theApp.m_PrjAllCfg.m_nDstWidth = 3840;
		theApp.m_PrjAllCfg.m_nDstHeight = 1920;
		printf("JW configuration_btn_callback LoadConfig exception!!: (output_width, output_height) = (%d, %d)\n", theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight);
	}

	return TRUE;
}

void CNewLoadView::LoadFormFile(CString szFullpath, CString szFileName)
{
#if LICENSE_PERSONAL_KEY
	if (mQLicense != NULL)
		theApp.m_ScreenManager.plan_type = mQLicense->checkKeyScope(HL_key_id, HL_key_year, HL_key_month, HL_key_day);
	else
	{
		AfxMessageBox(_T("License Error2 : We are opening file as demo."), MB_OK, MB_ICONEXCLAMATION);
		theApp.m_ScreenManager.plan_type = QVS_PLANTYPE_DEMO;
	}
#endif

	if (ParseLoadValueFormConfiguFile(szFullpath, szFileName))
	{
		((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild)->btnEnable(false);

		m_btnLoad.Disable(&m_btnLoad);
		//m_btnNew.Disable(&m_btnNew);

		theApp.m_PrjAllCfg.m_szProjectFullPath = szFullpath;
		theApp.m_PrjAllCfg.m_szProjectName = szFileName;
		theApp.addToRecentFile(szFullpath + _T("\\") + szFileName);
		theApp.m_ScreenManager.isNewProject = false;

		if (!theApp.m_PrjAllCfg.m_restart)
		{
			theApp.m_ScreenManager.m_iStartType = 1;
			theApp.m_ScreenManager.CreateAllScreen(vdstCenter);//다른 화면을 만든다.
			theApp.m_ScreenManager.SetProjectName();

			//theApp.m_ScreenManager.ShowScreens(vdstSourceView);
			theApp.m_ScreenManager.PreScreens(vdstSourceView);
			SetTimer(TIMER_INTO_CENTERSCREEN, 100, NULL);
		}
		else
		{
			CCenterView * pCenterView = (CCenterView *)(theApp.m_ScreenManager.m_pCenterView);
			pCenterView->initQVSEngine();

			if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone == FALSE)
				return;

			theApp.m_ScreenManager.m_iStartType = 1;
			theApp.m_ScreenManager.SetProjectName();

			theApp.m_ScreenManager.ShowScreens(vdstSourceView);

			SetTimer(TIMER_INTO_CENTERSCREEN, 100, NULL);
		}

		resetUI();
	}
	else
	{
		//AfxMessageBox(L"Load ERROR: PTS file open fail!");
		m_btnLoad.Enable(&m_btnLoad);
		//m_btnNew.Enable(&m_btnNew);
		m_btnLoad.SetImage(CGdipButton::STD_TYPE);
	}

	//m_btnLoad.SetImage(CGdipButton::STD_TYPE);
}

void CNewLoadView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pNewLoadConfigChild = new CNewLoadConfigChild;
	m_pNewLoadConfigChild->Create(CNewLoadConfigChild::IDD, GetDlgItem(IDC_STATICFRAME_NEWLOAD));
	m_pNewLoadConfigChild->ShowWindow(SW_SHOW);

	//m_btnNew.LoadImages(IDB_FILE_BTN_NEW_NORMAL_PNG, IDB_FILE_BTN_NEW_SELECTED_PNG, IDB_FILE_BTN_NEW_MOVER_PNG, IDB_FILE_BTN_NEW_PRESSED_PNG, 0, 0, _T("PNG"));
	//m_btnNew.SizeToContent();
	//m_btnNew.EnableToggle(TRUE);

	m_btnLoad.LoadImages(IDB_FILE_BTN_LOADPROJECT_NORMAL, IDB_FILE_BTN_LOADPROJECT_SELECTED, IDB_FILE_BTN_LOADPROJECT_MOVER, IDB_FILE_BTN_LOADPROJECT_PRESSED, 0, 0, _T("PNG"));
	m_btnLoad.SizeToContent();
	//m_btnLoad.EnableToggle(TRUE);

	m_listRecentProject.SetExtendedStyle(LVS_EX_FULLROWSELECT); //LVS_EX_GRIDLINES : 그리드
	m_listRecentProject.InsertColumn(0, _T("FullPath"), LVCFMT_LEFT, 0);
	m_listRecentProject.InsertColumn(1, _T("Recently Opened Configurations"), LVCFMT_LEFT, 530);
	m_listRecentProject.SetBkColor(COLOR_LOAD_RECENTCONFI_BK);
	//m_listRecentProject.ShowScrollBar(SB_HORZ, FALSE);

#if(!LICENSE_POWERED_BY_LG)
	m_btnHomeURL.LoadImages(IDB_VRUE360_LOGO_SMALL, 0, 0, 0, 0, 0, _T("PNG"));
	m_linkContact.ShowWindow(SW_HIDE);
#else
	m_btnHomeURL.LoadImages(IDB_PNG_LOGO_LG, 0, IDB_PNG_LOGO_LG, IDB_PNG_LOGO_LG, 0, 0, _T("PNG"));
	m_linkContact.ShowWindow(SW_SHOW);
#endif
	m_btnHomeURL.SizeToContent();

	SetFontObj();

	if (mQLicense == NULL)
		mQLicense = new QLicense();
}


BOOL CNewLoadView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_LOAD_DIALOG_BK);
	return TRUE;
}


HBRUSH CNewLoadView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_LOAD_DIALOG_BK);
	// TODO:  여기서 DC의 특성을 변경합니다.

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT);
		pDC->SetBkColor(COLOR_LOAD_DIALOG_BK);
		hbr = hbrNomalBack;
	}

	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT);
		pDC->SetBkColor(COLOR_LOAD_DIALOG_BK);
		hbr = hbrNomalBack;
		//m_listRecentProject.ShowScrollBar(SB_HORZ, FALSE);
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CNewLoadView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// Hide 처리... 필요 없음.
	//	m_btnLoad.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	//	m_btnNew.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	//	m_listRecentProject.ShowWindow(bShow ? SW_SHOW : SW_HIDE);

	if (bShow)
	{
		//모든 새로 띄워주는 팝업창을 밝은 테마로 설정 // popup window thema //NOMALSKIN, SUBSKIN
		CXTPSkinManager* pSkinManager = XTPSkinManager();
		CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + NOMALSKIN);
		pSkinManager->SetAutoApplyNewWindows(FALSE);

		UpdateRecentListContrlValue();

		m_btnLoad.Enable(&m_btnLoad);
		//m_btnNew.Enable(&m_btnNew);

		SetFontObj();
	}
	else
		m_pNewLoadConfigChild->resetUI();
		//m_pNewLoadConfigChild->m_wndTaskPanel.FindGroup(IDD_NEWLOAD_CONFIG_CHILD_1)->SetExpanded(TRUE);

}

void CNewLoadView::UpdateRecentListContrlValue()
{
	m_listRecentProject.DeleteAllItems();
	for (int i = ID_FILE_MRU_FILE1; i <= ID_FILE_MRU_FILE16 + 4; i++)
	{
		CString szFile = theApp.getRecentFile(i - ID_FILE_MRU_FILE1);

		//여기서 파일의 존재 유무를 Check하여 팝업을 띄운다.
		CFileFind finder;
		BOOL bIsFile = TRUE;
		bIsFile = finder.FindFile(szFile);

		if (szFile.GetLength() > 0 && !bIsFile)
		{
			theApp.RemoveRecentFile(i - ID_FILE_MRU_FILE1);
			i--;
			continue;
		}

		if (szFile.GetLength() > 0)
		{
			CString szPath, szFileOnly;
			int nIndex = szFile.ReverseFind(_T('\\'));
			if (nIndex > 0) {

				int nStartID = m_listRecentProject.GetItemCount();
				m_listRecentProject.InsertItem(nStartID, szFile);

				szPath = szFile.Left(nIndex);
				szFileOnly = szFile.Mid(nIndex + 1);
				m_listRecentProject.SetItem(nStartID, 1, LVIF_TEXT, szFileOnly + _T("\n") + szFile, 0, NULL, NULL, NULL);
			}
		}
	}

	theApp.updateRecentFile();
	UpdateData(FALSE);
}

void CNewLoadView::resetUI()
{
	CBottomPanoChildDlg* m_pBottomPanoChild = (CBottomPanoChildDlg*)theApp.m_ScreenManager.m_pBottomPanoChild;
	if (m_pBottomPanoChild != NULL)
	{
		if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
		{
			m_pBottomPanoChild->m_sldBrightness.SetPos(10);
			m_pBottomPanoChild->GetDlgItem(IDC_EDT_BRIGHTNESS)->SetWindowText(_T("0"));
			m_pBottomPanoChild->m_sldBrightness.EnableWindow(FALSE);

			m_pBottomPanoChild->m_sldContrast.SetPos(0);
			m_pBottomPanoChild->GetDlgItem(IDC_EDT_CONTRAST)->SetWindowText(_T("0"));
			m_pBottomPanoChild->m_sldContrast.EnableWindow(FALSE);

			m_pBottomPanoChild->setGrayscale = GRAYSCALE_OFF;
			QVS_Engine::QVS_setGrayscaleMode(m_pBottomPanoChild->setGrayscale);
			m_pBottomPanoChild->m_btnGrayOnOff.SetImage(CGdipButton::STD_TYPE);
			m_pBottomPanoChild->m_btnGrayOnOff.EnableWindow(FALSE);
		}
		else
		{
			m_pBottomPanoChild->m_sldBrightness.EnableWindow(TRUE);
			m_pBottomPanoChild->m_sldContrast.EnableWindow(TRUE);
			m_pBottomPanoChild->m_btnGrayOnOff.EnableWindow(TRUE);
		}

		if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Wide)
			m_pBottomPanoChild->m_btnNadirOnOff.Disable(&m_pBottomPanoChild->m_btnNadirOnOff);
		else
			m_pBottomPanoChild->m_btnNadirOnOff.Enable(&m_pBottomPanoChild->m_btnNadirOnOff);
	}
}

void CNewLoadView::OnNMDblclkListRecent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMItemActivate->iItem != -1)
	{
		CString szText, szPath, szFile;
		szText = m_listRecentProject.GetItemText(pNMItemActivate->iItem, 0);

		CFileFind finder;
		BOOL bIsFile = TRUE;
		bIsFile = finder.FindFile(szText);

		if (!bIsFile)
		{
			CString szMsg;
			szMsg.Format(_T("\"%s\" File not found!"), m_listRecentProject.GetItemText(pNMItemActivate->iItem, 1));
			AfxMessageBox(szMsg, MB_OK, MB_ICONWARNING);
			UpdateRecentListContrlValue();
			return;
		}


		int nIndex = szText.ReverseFind(_T('\\'));
		if (nIndex > 0) {
			szPath = szText.Left(nIndex);
			szFile = szText.Mid(nIndex + 1);
			LoadFormFile(szPath, szFile);
		}
	}
	*pResult = 0;
}


void CNewLoadView::OnNMCustomdrawListRecent(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	LPNMCUSTOMDRAW pNMCD;
	pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	*pResult = 0;

	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	int iRow = (int)lplvcd->nmcd.dwItemSpec;

	switch (lplvcd->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			return;
		}

		// Modify item text and or background
		case CDDS_ITEMPREPAINT:
		{
			lplvcd->clrText = RGB(255, 255, 255);
			// If you want the sub items the same as the item,
			// set *pResult to CDRF_NEWFONT
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			return;
		}

		// Modify sub item text and/or background
		case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
		{

			lplvcd->clrTextBk = COLOR_LOAD_DIALOG_BK;
			*pResult = CDRF_DODEFAULT;
			return;
		}
	}
}


void CNewLoadView::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	//CFont* ariaB13 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria13);
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	GetDlgItem(IDC_STATIC_NEWPROJECT)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_RECENTTITLE)->SetFont(aria12);
	GetDlgItem(IDC_SYSLINK_CONTACT)->SetFont(aria12);
	//GetDlgItem(IDC_LIST_RECENT)->SetFont(ariaB13);
}

void CNewLoadView::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	//CFormView::PostNcDestroy();
}


void CNewLoadView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.v
	switch (nIDEvent)
	{
	case TIMER_INTO_CENTERSCREEN:
	{
		//if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->m_bFirstTime)
		if (!theApp.m_PrjAllCfg.isQVSEngineInit())
			break;
		KillTimer(TIMER_INTO_CENTERSCREEN);
		theApp.m_ScreenManager.ShowScreens(vdstSourceView);
		//m_btnNew.SetImage(CGdipButton::STD_TYPE);
		((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild)->btnEnable(true);
	}
		break;
	}

	CFormView::OnTimer(nIDEvent);
}


void CNewLoadView::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nIDCtl == IDC_LIST_RECENT)
	{
		lpMeasureItemStruct->itemHeight = 42;
	}

	CFormView::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CNewLoadView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nIDCtl == IDC_LIST_RECENT)
	{
		m_listRecentProject.ShowScrollBar(SB_HORZ, FALSE);
		CRect rc;
		GetClientRect(&rc);

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		CFont * pOldFont = NULL;
		CFont CulFont;

		pDC->SetTextColor(COLOR_DIALOG_TEXT);

		SetFontObj();

		lpDrawItemStruct->rcItem.right = rc.right;
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, COLOR_SELECTED_CMBBOX);

		CPen pen;
		pen.CreatePen(PS_SOLID, 1, COLOR_LOAD_RECENTCONFI_LINE);
		CPen* oldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(lpDrawItemStruct->rcItem.left - 1, lpDrawItemStruct->rcItem.top - 1);
		pDC->LineTo(lpDrawItemStruct->rcItem.right + 20, lpDrawItemStruct->rcItem.top - 1);
		pDC->MoveTo(lpDrawItemStruct->rcItem.left - 1, lpDrawItemStruct->rcItem.bottom - 1);
		pDC->LineTo(lpDrawItemStruct->rcItem.right + 20, lpDrawItemStruct->rcItem.bottom - 1);

		CString txt = m_listRecentProject.GetItemText(lpDrawItemStruct->itemID, 1);
		lpDrawItemStruct->rcItem.left = 3;
		lpDrawItemStruct->rcItem.top += 7;
		pDC->DrawText(txt, &lpDrawItemStruct->rcItem, DT_TOP);
	}

	CFormView::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CNewLoadView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.

	//Graphics gs(dc);
	//HINSTANCE g_hInst = AfxGetStaticModuleState()->m_hCurrentInstanceHandle;

	//HRSRC hResource = FindResource(g_hInst, MAKEINTRESOURCE(IDB_VRUE360_LOGO), TEXT("PNG"));
	//if (!hResource)	return;

	//DWORD imageSize = SizeofResource(g_hInst, hResource);
	//HGLOBAL hGlobal = LoadResource(g_hInst, hResource);
	//LPVOID pData = LockResource(hGlobal);

	//HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	//LPVOID pBuffer = GlobalLock(hBuffer);
	//CopyMemory(pBuffer, pData, imageSize);
	//GlobalUnlock(hBuffer);

	//IStream *pStream;
	//HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

	//Image logo(pStream);
	//pStream->Release();
	//if (logo.GetLastStatus() != Ok) return;

	//CRect rcClient(0, 0, 0, 0);
	//GetClientRect(rcClient);
	//dc.FillRect(&rcClient, &CBrush(COLOR_LOAD_DIALOG_BK));
	//int nWidth = rcClient.Width();
	//int nHeight= rcClient.Height();

	//int logoW = 600;
	//int logoH = 150 / 2;
	////int x = nWidth / 2 - LOADRESENTSCREENGQP - logoW;
	//int x = (nWidth - LOADRESENTSCREENGQP - 1100) / 3;
	//int y = nHeight / 2 - logoH;

	//gs.DrawImage(&logo, x, y);
}


void CNewLoadView::OnBnClickedBtnUrl()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#if(!LICENSE_POWERED_BY_LG)
	ShellExecute(NULL, _T("open"), _T("http://www.vrue360.com"), NULL, NULL, SW_SHOWNORMAL);
#else
#endif
}

void CNewLoadView::OnNMClickSyslinkContact(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CString str = _T("vrdevteam@lguplus.co.kr");

	if (!OpenClipboard())
	{
		AfxMessageBox(_T("Can not open Clipboard : Mail copy fail"));
		return;
	}
	EmptyClipboard();
	int cch = str.GetLength() * 2 + 2;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, cch);
	if (hMem)
	{
		LPSTR pClipData = (LPSTR)GlobalLock(hMem);
		pClipData[0] = 0;
		strncpy(pClipData, ATL::CW2AEX<1024>(str.GetBuffer(0)), cch);

		GlobalUnlock(hMem);
		if (SetClipboardData(CF_TEXT, hMem) == NULL)
		{
			AfxMessageBox(_T("Can not set Clipboard : Mail copy fail"));
			CloseClipboard();
			//GlobalFree(hMem); // 지역 스코프 내 자원이 아니면 해제하면 안 된다
			return;
		}
	}

	CloseClipboard();
	//GlobalFree(hMem);

	MessageBox(_T("Success : Mail copy done"), MB_OK, MB_ICONQUESTION);
}
