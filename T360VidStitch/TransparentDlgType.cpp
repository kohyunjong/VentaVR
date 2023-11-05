// TransparentDlgType2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "CenterView.h"
#include "TransparentDlgType.h"
#include "afxdialogex.h"
#include "CGdiPlusBitmap.h"

// CTransparentDlgType ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTransparentDlgType, CDialogEx)

CTransparentDlgType::CTransparentDlgType(CWnd* pParent /*=NULL*/)
: CDialog((UINT)0, pParent)
{
	m_bTimerfnContinue = FALSE;
	m_pfnTimerThread = NULL;
	m_hTimerfnThExitEvt = NULL;

	m_bLoadingfnContinue = FALSE;
	m_pLoadingfnThread = NULL;
	m_hLoadingfnThExitEvt = NULL;
	frame = 0;
}

CTransparentDlgType::~CTransparentDlgType()
{
}

void CTransparentDlgType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_szText1);
}


BEGIN_MESSAGE_MAP(CTransparentDlgType, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTransparentDlgType �޽��� ó�����Դϴ�.


BOOL CTransparentDlgType::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rc;
	GetClientRect(&rc);
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pbmpOld = MemDC.SelectObject(&m_BGBitmap);
	pDC->BitBlt(0, 0, rc.right, rc.bottom, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pbmpOld);

	if (m_bLoadingfnContinue)
		pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);

	ReleaseDC(pDC);
	return TRUE;
}

void CTransparentDlgType::MoveTextPos(int x, int y, int width, int height)
{
	m_szText1.SetWindowPos(NULL, x, y, width, height, NULL);
}


BOOL CTransparentDlgType::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	SetLayer pSetLayer = (SetLayer)GetProcAddress(hUser32, "SetLayeredWindowAttributes");

	if (pSetLayer == NULL)
		return FALSE;

	CGdiPlusBitmapResource nPng;
	nPng.Load(IDB_TRANSBGTYPE2, _T("PNG"));
	HBITMAP hBitmap = NULL;
	nPng.m_pBitmap->GetHBITMAP(COLOR_LOAD_DIALOG_BK, &hBitmap);
	m_BGBitmap.Attach(hBitmap);

	int alphaValue = 190; //���� ���� 0 ~ 255 ( 0�� �������� )
	SetWindowLong(this->m_hWnd, GWL_EXSTYLE,
		GetWindowLong(this->m_hWnd, GWL_EXSTYLE) | 0x80000);
	pSetLayer(this->m_hWnd, 0, alphaValue, LWA_ALPHA);

	SetFontObj();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTransparentDlgType::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	dc.SetTextColor(RGB(255, 255, 255));
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
}


void CTransparentDlgType::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (!bShow){
		if (m_bTimerfnContinue)
			KillTimerfnThread();
		else if (m_bLoadingfnContinue)
			KillLoadingfnThread();
	}
	if (bShow && theApp.m_ScreenManager.failCalibration){
		SetTimer(TIMER_CALIBRATIONFAIL, 2000, NULL);
	}
}

//Timer ����
void CTransparentDlgType::DoStartATimer(int nType)
{
	//KillTimerfnThread();
	//�״㿡 ���� ����.

	type = nType;

	m_hTimerfnThExitEvt = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bTimerfnContinue = TRUE;
	m_pfnTimerThread = AfxBeginThread(TimerfnThreadProc, (LPVOID)this);
}


void CTransparentDlgType::KillTimerfnThread()
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
		m_szText1.SetWindowTextW(_T(""));
	}
}

UINT CTransparentDlgType::TimerfnThreadProc(LPVOID pParam)
{
	//Sleep(5000);
	CTransparentDlgType* pInst = (CTransparentDlgType*)pParam;

	CString szText1 = _T("");
	//szText1 = pInst->m_strText;

	double bitrate;
	int secs;
	float fps;
	bool isFirst = true;

	while (pInst->m_bTimerfnContinue)
	{
		//pInst->m_tend = CTime::GetCurrentTime();
		//CTimeSpan elapsedTime = pInst->m_tend - pInst->m_tstart;

		//CString szText = elapsedTime.Format(_T("%H:%M:%S"));

		if (isFirst)
			Sleep(6000);

		int culSec = secs % 60;
		int culMin = (secs / 60) % 60;
		int culHour = secs / 3600;

		CString H, M, S, F;

		CString timer;
		H.Format(_T("%02d"), culHour);
		M.Format(_T("%02d"), culMin);
		S.Format(_T("%02d"), culSec);
		F.Format(_T("%.02f"), fps);
		timer = H + _T(":") + M + _T(":") + S + _T("  fps:") + F;
		if (!pInst->m_bTimerfnContinue)
			break;

		pInst->m_szText1.GetWindowText(szText1);
		//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %d\n", secs);
		if (secs > 0 && fps > 1 && !isFirst)
			pInst->m_szText1.SetWindowText(pInst->m_strText + _T("  ") + timer);
		else if (szText1 == pInst->m_strText + _T("  Please wait") || szText1 == pInst->m_strText + _T("  Not working"))
			pInst->m_szText1.SetWindowText(pInst->m_strText + _T("  Not working"));
		else
		{
			isFirst = false;
			pInst->m_szText1.SetWindowText(pInst->m_strText + _T("  Please wait"));
			Sleep(2000);
		}
		pInst->m_szText1.Invalidate();

		Sleep(1000);
	}

	SetEvent(pInst->m_hTimerfnThExitEvt);
	return 0;
}

//Loading... ����
void CTransparentDlgType::DoStartLoading()
{
	m_hLoadingfnThExitEvt = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bLoadingfnContinue = TRUE;
	m_pLoadingfnThread = AfxBeginThread(LoadingfnThreadProc, (LPVOID)this);
}

void CTransparentDlgType::KillLoadingfnThread()
{
	DWORD dwExitCode;

	if (m_pLoadingfnThread != NULL && GetExitCodeThread(m_pLoadingfnThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
	{
		m_bLoadingfnContinue = FALSE;

		TerminateThread(m_pLoadingfnThread, 0);
		//WaitForSingleObject(m_hLoadingfnThExitEvt, INFINITE);
		CloseHandle(m_hLoadingfnThExitEvt);
		m_hLoadingfnThExitEvt = NULL;
		m_pLoadingfnThread = NULL;
		frame = 0;
		m_szText1.SetWindowTextW(_T(""));
	}
}

UINT CTransparentDlgType::LoadingfnThreadProc(LPVOID pParam)
{
	CTransparentDlgType* pInst = (CTransparentDlgType*)pParam;

	CString szText1 = _T("");
	//pInst->m_szText1.GetWindowTextW(szText1);

	while (pInst->m_bLoadingfnContinue){
		switch (pInst->frame % 4)
		{
		case 0:
			szText1 = (_T(""));
			break;
		case 1:
			szText1 = (_T("."));
			break;
		case 2:
			szText1 = (_T(". ."));
			break;
		case 3:
			szText1 = (_T(". . ."));
			break;
		}
		if (!pInst->m_bLoadingfnContinue)
			break;

		pInst->m_szText1.SetWindowText(szText1);
		pInst->m_szText1.SetFontSize(100);
		pInst->m_szText1.Invalidate();

		pInst->frame++;
		Sleep(300);
	}

	SetEvent(pInst->m_hLoadingfnThExitEvt);
	return 0;
}

void CTransparentDlgType::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	m_szText1.SetFont(aria12);

}

void CTransparentDlgType::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_pfnTimerThread != NULL)
		KillTimerfnThread();
	if (m_pLoadingfnThread != NULL)
		KillLoadingfnThread();
}


HBRUSH CTransparentDlgType::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrNomalBack = CreateSolidBrush(RGB(28, 26, 26));

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT && !m_bLoadingfnContinue)
		{
			//pDC->SetTextColor(RGB());
			pDC->SetBkColor(RGB(28, 26, 26));
			hbr = hbrNomalBack;
		}
	}
	}
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

BOOL CTransparentDlgType::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CTransparentDlgType::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == TIMER_CALIBRATIONFAIL)
	{
		KillTimer(TIMER_CALIBRATIONFAIL);
		theApp.m_ScreenManager.failCalibration = false;
		theApp.m_ScreenManager.ShowToast(vdstToastFail, _T(" "), FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CTransparentDlgType::ToastString(CString str)
{
	m_strText = str;
}