// RightOverlayListChildDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightOverlayListChildDlg.h"
#include "afxdialogex.h"


// CRightOverlayListChildDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRightOverlayListChildDlg, CDialogEx)

CRightOverlayListChildDlg::CRightOverlayListChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRightOverlayListChildDlg::IDD, pParent)
{

}

CRightOverlayListChildDlg::~CRightOverlayListChildDlg()
{
}

void CRightOverlayListChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OVERLAYLIST, m_listOverlay);
	DDX_Control(pDX, IDC_BTNBACK, m_btnBack);

	m_btnBack.LoadImages(IDB_EXPORT_BTN_BACK_NORMAL, 0, IDB_EXPORT_BTN_BACK_MOVER, IDB_EXPORT_BTN_BACK_PRESSED, 0, 0, _T("PNG"));
	m_btnBack.SizeToContent();

	m_listOverlay.SetExtendedStyle(LVS_EX_GRIDLINES); //LVS_EX_GRIDLINES : 그리드
	m_listOverlay.InsertColumn(0, _T("Overlay List"), LVCFMT_CENTER, 129);
	m_listOverlay.SetBkColor(COLOR_LOAD_DIALOG_BK);
}


BEGIN_MESSAGE_MAP(CRightOverlayListChildDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BTNBACK, &CRightOverlayListChildDlg::OnBnClickedBtnback)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_OVERLAYLIST, &CRightOverlayListChildDlg::OnNMDblclkListOverlaylist)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OVERLAYLIST, &CRightOverlayListChildDlg::OnNMClickListOverlaylist)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_OVERLAYLIST, &CRightOverlayListChildDlg::OnNMRClickListOverlaylist)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OVERLAYPOPUP_EDIT, &CRightOverlayListChildDlg::OnOverlaypopupEdit)
	ON_COMMAND(ID_OVERLAYPOPUP_SHOW, &CRightOverlayListChildDlg::OnOverlaypopupShow)
	ON_COMMAND(ID_OVERLAYPOPUP_HIDE, &CRightOverlayListChildDlg::OnOverlaypopupHide)
	ON_COMMAND(ID_OVERLAYPOPUP_CLEAR, &CRightOverlayListChildDlg::OnOverlaypopupClear)
END_MESSAGE_MAP()


// CRightOverlayListChildDlg 메시지 처리기입니다.


void CRightOverlayListChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		CRect centerRect, clientRect;
		if ((CCenterView*)theApp.m_ScreenManager.m_pCenterView != NULL)
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->GetClientRect(&centerRect);
		this->GetClientRect(&clientRect);
		this->MoveWindow(0, 0, clientRect.Width(), centerRect.bottom + 50, SWP_NOMOVE);            // 크기는 알아서 맞게 지정

		dlgUpdate();

		theApp.m_ScreenManager.mCulTemplateNum = -1;
		UpdateRecentListContrlValue();
	}
}

void CRightOverlayListChildDlg::UpdateRecentListContrlValue()
{
	
}

void CRightOverlayListChildDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
}


void CRightOverlayListChildDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDCtl == IDC_LIST_OVERLAYLIST)
	{
		//CRect rc;
		//GetClientRect(&rc);

		lpMeasureItemStruct->itemHeight = 57;
	}
	
	CDialogEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CRightOverlayListChildDlg::OnNMDblclkListOverlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//printf("OverlayTemplate double click!!!!\n");
	
}


void CRightOverlayListChildDlg::OnBnClickedBtnback()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.m_ScreenManager.ShowScreens(vdstPanoramaView);
}


void CRightOverlayListChildDlg::OnNMClickListOverlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//printf("OverlayTemplate click!!!!\n");

}


void CRightOverlayListChildDlg::OnNMRClickListOverlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//printf("OverlayTemplate Rclick!!!!\n");
	
}


void CRightOverlayListChildDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMenu popup;
	CMenu* pMenu;

	popup.LoadMenuW(IDR_POPUP_OVERLAY);
	pMenu = popup.GetSubMenu(0);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CRightOverlayListChildDlg::OnOverlaypopupEdit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//printf("clicked Edit OverlayTemplate!!!\n");
	
}


void CRightOverlayListChildDlg::OnOverlaypopupShow()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//printf("clicked Show OverlayTemplate!!!\n");
	
}


void CRightOverlayListChildDlg::OnOverlaypopupHide()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//printf("clicked Hide OverlayTemplate!!!\n");
	
}

void CRightOverlayListChildDlg::OnOverlaypopupClear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//printf("clicked Clear OverlayTemplate!!!\n");
	
}

int CRightOverlayListChildDlg::setOverlayTemplateState(int idx, bool changePlayMode)
{
	return QVS_FALSE;
}

void CRightOverlayListChildDlg::dlgUpdate()
{
	CRect rc, btnRect, listRect, centerRect;
	this->GetClientRect(&rc);

	m_btnBack.GetClientRect(&btnRect);

	m_listOverlay.GetClientRect(&listRect);
	m_btnBack.MoveWindow(8, rc.bottom - btnRect.Height(), btnRect.Width(), btnRect.Height(), SWP_NOSIZE);
	m_listOverlay.MoveWindow(10, rc.top + 50, btnRect.Width(), rc.bottom - btnRect.Height() - 65, SWP_NOSIZE);
}

void CRightOverlayListChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMER_ADDINGOVERLAY)
	{
		KillTimer(TIMER_ADDINGOVERLAY);
		theApp.m_ScreenManager.ShowToast(vdstToastDone, _T(" "), FALSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}