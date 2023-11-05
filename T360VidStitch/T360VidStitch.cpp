
// T360VidStitch.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "T360VidStitchDlg.h"
#include "VIRecentFileList.h"
#include <afxadv.h> //Has CRecentFileList class definition.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About


CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	m_delay = 0;
}

CAboutDlg::CAboutDlg(int delay) : CDialogEx(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_delay = delay;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if (m_delay != 0)
	{
		HRSRC hResource = FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SPLASH_PNG), TEXT("PNG"));
		if (!hResource)
		{
			AfxMessageBox(_T("fail"));
			PostMessage(WM_CLOSE);
			return FALSE;
		}

		DWORD imageSize = SizeofResource(AfxGetInstanceHandle(), hResource);
		HGLOBAL hGlobal = LoadResource(AfxGetInstanceHandle(), hResource);
		LPVOID pData = LockResource(hGlobal);

		HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
		LPVOID pBuffer = GlobalLock(hBuffer);

		CopyMemory(pBuffer, pData, imageSize);
		GlobalUnlock(hBuffer);

		IStream* pStream;
		HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);
		if (hr != S_OK)
		{
			AfxMessageBox(_T("fail"));
			PostMessage(WM_CLOSE);
			return FALSE;
		}

		Image imagePNG(pStream);

		pStream->Release();

		if (imagePNG.GetLastStatus() != Ok) return FALSE;

		Bitmap* pBitmap = static_cast<Bitmap*>(imagePNG.Clone());
		HBITMAP hBitmap = NULL;
		Status status = pBitmap->GetHBITMAP(Color(0, 0, 0), &hBitmap);
		if (status != Ok)
		{
			AfxMessageBox(_T("fail"));
			PostMessage(WM_CLOSE);
			return FALSE;
		}

		CDC* pDC = GetDC();
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		SelectObject(memDC.m_hDC, hBitmap);

		//	Graphics graphics(pDC->m_hDC);
		//	graphics.DrawImage(&imagePNG, 0,0, imagePNG.GetWidth(),imagePNG.GetHeight());

		m_nAboutWidth = imagePNG.GetWidth();
		m_nAboutHeight = imagePNG.GetHeight();

		this->ModifyStyle(WS_CAPTION, 0);
		this->ModifyStyleEx(0, WS_EX_LAYERED);

		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		CDC* dcDst = NULL;

		POINT ptDest = { 0, 0 };
		POINT ptSrc = { 0, 0 };

		SIZE szDest = { m_nAboutWidth, m_nAboutHeight };

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 250, AC_SRC_ALPHA };

		BOOL ret = UpdateLayeredWindow(dcDst, &ptDest, &szDest, &memDC, &ptSrc, RGB(255, 255, 255), &bf, ULW_ALPHA);

		if (ret == FALSE)
		{
			AfxMessageBox(_T("fail"));
			PostMessage(WM_CLOSE);
			return FALSE;
		}
		GlobalFree(hBuffer);
		DeleteObject(pBitmap);
		DeleteObject(hBitmap);
		memDC.DeleteDC();
		ReleaseDC(pDC);

		CenterWindow(CWnd::GetDesktopWindow());

		SetTimer(TIMER_STARTAPP, m_delay * 1000, NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	OnOK();
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpAboutBG);

	dc.BitBlt(0, 0, m_nAboutWidth, m_nAboutHeight, &cdc, 0, 0, SRCCOPY);
	cdc.DeleteDC();
}


void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMER_STARTAPP)
	{
		KillTimer(TIMER_STARTAPP);
		OnOK();
	}
	CDialogEx::OnTimer(nIDEvent);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CT360VidStitch
BEGIN_MESSAGE_MAP(CT360VidStitchApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CT360VidStitchApp construction

CT360VidStitchApp::CT360VidStitchApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_hMutex = NULL;				// 중복실행 방지를 위한 뮤텍스 핸들
	// TODO: add construction code here,

	// Place all significant initialization in InitInstance
}


// The one and only CT360VidStitch object

CT360VidStitchApp theApp;


BOOL PngLoadFromResource(CImage& img, LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	IStream* pStream = NULL;
	HRSRC hResource = ::FindResource(hInst, pName, pType); if (!hResource) return FALSE;
	DWORD nImageSize = ::SizeofResource(hInst, hResource); if (!nImageSize) return FALSE;

	HGLOBAL m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, nImageSize);
	BYTE* pBytes = (BYTE*)::LoadResource(hInst, hResource);
	if (m_hBuffer)
	{
		void* pSource = ::LockResource(::LoadResource(hInst, hResource));
		if (!pSource) return FALSE;
		void* pDest = ::GlobalLock(m_hBuffer);
		if (pDest)
		{
			CopyMemory(pDest, pSource, nImageSize);
			if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				img.Load(pStream); // 여기만 수정 하면
				pStream->Release();
			}
			::GlobalUnlock(m_hBuffer);
		}
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}
	if (img.IsNull()) return FALSE;
	return TRUE;
}

// CT360VidStitch initialization

BOOL CT360VidStitchApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	m_hMutex = ::CreateMutex(NULL, FALSE, _T("MUTEX_Q360"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		return FALSE;
	}

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
//#if _DEBUG
//	if (!AllocConsole())
//	{
//		AfxMessageBox(_T("Failed to create the console!"), MB_ICONEXCLAMATION);
//	}
//#endif

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("T360VidStitch"));
	LoadStdProfileSettings(20);

	CT360VidStitchDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
#ifdef _DEBUG
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
#endif
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CT360VidStitchApp::LoadStdProfileSettings(UINT nMaxMRU)
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList == NULL);
	static const TCHAR szFileSection[] = _T("Recent File List");
	static const TCHAR szFileEntry[] = _T("File%d");
	static const TCHAR szPreviewSection[] = _T("Settings");
	static const TCHAR szPreviewEntry[] = _T("PreviewPages");

	if (nMaxMRU != 0)
	{
		// create file MRU since nMaxMRU not zero

		// Here's the important part--overriding CRecentFileList
		m_pRecentFileList = new VIRecentFileList(0, szFileSection, szFileEntry, nMaxMRU);

		m_pRecentFileList->ReadList();
	}
	// 0 by default means not set
	m_nNumPreviewPages = GetProfileInt(szPreviewSection, szPreviewEntry, 0);
}  // LoadStdProfileSettings()

void CT360VidStitchApp::addToRecentFile(CString szPath)
{
	//m_pRecentFileList->Add(szPath);
	m_pRecentFileList->Add(szPath);
	m_pRecentFileList->WriteList();
}

void CT360VidStitchApp::updateRecentFile()
{
	m_pRecentFileList->WriteList();
}

CString CT360VidStitchApp::getRecentFile(int index)
{
	return (*m_pRecentFileList)[index];
}

int CT360VidStitchApp::GetRecentListSize()
{
	return m_pRecentFileList->GetSize();
}

void CT360VidStitchApp::RemoveRecentFile(int index)
{
	m_pRecentFileList->Remove(index);
}

CString CT360VidStitchApp::GetAppDir()
{
	// application directory exclude last root
	CString strResult = _T("");
	TCHAR szFileName[MAX_PATH] = { 0, };
	GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH);
	TCHAR* p = _tcsrchr(szFileName, _T('\\'));
	if (p)
		*p = 0;

	strResult.Format(_T("%s"), szFileName);
	return strResult;
}

int CT360VidStitchApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (m_hMutex)
		::CloseHandle(m_hMutex);
#ifdef _DEBUG
	TRACE(_T("CT360VidStitchApp::ExitInstance Call \n"));
#endif	

	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	return CWinApp::ExitInstance();
}




void CT360VidStitchApp::OnInitProjertConfig()
{
//		PLM #17129 :
//	m_PrjAllCfg.m_szPTSFullPath = "";
	m_PrjAllCfg.m_szProjectFullPath	=	"";
	m_PrjAllCfg.m_szProjectName		=	"";
	m_PrjAllCfg.m_nWidth			=	0;
	m_PrjAllCfg.m_nHeight			=   0;
	m_PrjAllCfg.m_nFov				=	0;
	m_PrjAllCfg.m_nMaxCamra			=	0;
	m_PrjAllCfg.m_bInitQVS_EngineDone = FALSE;

	m_PrjAllCfg.m_ModuleType = QVS_Engine::InputModuleType_Magewell;
}

BOOL CT360VidStitchApp::PreTranslateMessage(MSG* pMsg) {
	//CQuramVideo360SampleView   *pView = (CQuramVideo360SampleView *)GetActiveView();
	if (pMsg->message == WM_KEYUP || pMsg->message == WM_SYSKEYDOWN)
	{
		((CT360VidStitchDlg*)m_pMainWnd)->onKeyEvent(pMsg);
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

