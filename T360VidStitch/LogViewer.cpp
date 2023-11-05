// LogViewer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "LogViewer.h"
#include "afxdialogex.h"


// CLogViewer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogViewer, CDialogEx)

CLogViewer::CLogViewer(CWnd* pParent /*=NULL*/)
: CDialogEx(CLogViewer::IDD, pParent)
{

}

CLogViewer::~CLogViewer()
{
}

void CLogViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_editLogViewer);
	DDX_Control(pDX, ID_BTN_SAVEFILE, m_btnSaveFile);
}


BEGIN_MESSAGE_MAP(CLogViewer, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(ID_BTN_SAVEFILE, &CLogViewer::OnBnClickedBtnSavefile)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CLogViewer 메시지 처리기입니다.
BOOL CLogViewer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLogViewer::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_EDIT_LOG)->SetFont(aria12);

}

HBRUSH CLogViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrDeepBack = CreateSolidBrush(RGB(0, 0, 0));
	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_EDIT_LOG:
		{
			pDC->SetTextColor(RGB(255, 255, 255)); // @KEYWORD@THEME@
			pDC->SetBkColor(RGB(0, 0, 0));
			hbr = hbrDeepBack;
		}
			break;
	}

	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT: // Edit box
		switch (pWnd->GetDlgCtrlID())
		{
		default:
			pDC->SetTextColor(RGB(255, 255, 255)); // @KEYWORD@THEME@
			pDC->SetBkColor(RGB(0, 0, 0));
			hbr = hbrDeepBack;
			break;
		}
		break;
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CLogViewer::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rt;
	int margin = 12;

	GetClientRect(rt);
	int left = rt.left + margin;
	int top = rt.top + margin;
	int width = rt.right - rt.left - margin * 2;
	int height = rt.bottom - rt.top - 58;
	m_editLogViewer.MoveWindow(left, top, width, height);

	left = rt.right - margin;
	top = rt.bottom - margin;
	m_btnSaveFile.GetWindowRect(rt);
	width = rt.right - rt.left;
	height = rt.bottom - rt.top;
	left -= width;
	top -= height;
	m_btnSaveFile.MoveWindow(left, top, width, height);
}

void CLogViewer::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		CXTPSkinManager* pSkinManager = XTPSkinManager();
		CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + SUBSKIN);
		pSkinManager->SetAutoApplyNewWindows(FALSE);

		CRect rt;
		int margin = 12;

		GetClientRect(rt);
		int left = rt.left + margin;
		int top = rt.top + margin;
		int width = rt.right - rt.left - margin * 2;
		int height = rt.bottom - rt.top - 58;
		m_editLogViewer.MoveWindow(left, top, width, height);

		left = rt.right - margin;
		top = rt.bottom - margin;
		m_btnSaveFile.GetWindowRect(rt);
		width = rt.right - rt.left;
		height = rt.bottom - rt.top;
		left -= width;
		top -= height;
		m_btnSaveFile.MoveWindow(left, top, width, height);

		m_btnSaveFile.LoadImages(IDB_BTN_SAVE_D_NORMAL, 0, IDB_BTN_SAVE_D_PRESSED, IDB_BTN_SAVE_D_PRESSED, 0, 0, _T("PNG"));
		m_btnSaveFile.SizeToContent();

		isFirst = true;
		SetFontObj();
		DoStartATimer();
	}
	if (!bShow)
	{
		this->DestroyWindow();
	}
}

void CLogViewer::LogUpdate()
{
	char* temp = NULL;
	temp = QVS_Engine::QVS_viewLog(isFirst);
	isFirst = false;
	
	if (temp != NULL)
	{
		m_culLog += Char2CString(temp);

		if (m_culLog != m_preLog)
		{
			m_editLogViewer.ReplaceSel(Char2CString(temp));
			m_preLog = m_culLog;
		}
	}
}

void CLogViewer::OnBnClickedBtnSavefile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	QVS_Engine::QVS_saveLogFile();
}

void CLogViewer::DoStartATimer()
{
	//KillTimerfnThread();
	//그담에 복사 시작.

	m_hTimerfnThExitEvt = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bTimerfnContinue = TRUE;
	m_pfnTimerThread = AfxBeginThread(TimerfnThreadProc, (LPVOID)this);
}

void CLogViewer::KillTimerfnThread()
{
	DWORD dwExitCode;

	if (m_pfnTimerThread != NULL && GetExitCodeThread(m_pfnTimerThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
	{
		m_bTimerfnContinue = FALSE;

		//WaitForSingleObject(m_hTimerfnThExitEvt, INFINITE);
		TerminateThread(m_pfnTimerThread, 0);
		CloseHandle(m_hTimerfnThExitEvt);
		m_hTimerfnThExitEvt = NULL;
		m_pfnTimerThread = NULL;
	}
}

UINT CLogViewer::TimerfnThreadProc(LPVOID pParam)
{
	CLogViewer* pInst = (CLogViewer*)pParam;

	CString szText1 = _T("");

	while (pInst->m_bTimerfnContinue)
	{
		if (pInst->m_editLogViewer.GetLineCount() > MAX_LOGVECTOR + 2)
		{
			int end = pInst->m_culLog.Find(_T("\r\n")) + 1;
			pInst->m_culLog.Delete(0, end);
		}

		pInst->LogUpdate();
		Sleep(100);
	}

	SetEvent(pInst->m_hTimerfnThExitEvt);
	return 0;
}

void CLogViewer::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bTimerfnContinue)
		KillTimerfnThread();
	this->DestroyWindow();

	CDialogEx::OnClose();
}


void CLogViewer::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bTimerfnContinue)
		KillTimerfnThread();

	((CLeftChildDlg*)theApp.m_ScreenManager.m_pLeftChild)->m_btnLogView.SetImage(CGdipButton::STD_TYPE);
	((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isLogViewerDlgShow = false;
}

BOOL CLogViewer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_ESCAPE)
	{
		OnDestroy();
		OnClose();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}