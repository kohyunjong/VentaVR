// RightOverlayChildDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightOverlayChildDlg.h"
//#include "OverlayEditDepthDlg.h"
#include "afxdialogex.h"


// RightOverlayChildDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(RightOverlayChildDlg, CDialogEx)

RightOverlayChildDlg::RightOverlayChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(RightOverlayChildDlg::IDD, pParent)
{

}

RightOverlayChildDlg::~RightOverlayChildDlg()
{
}

void RightOverlayChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNADDOVERLAY, m_btnAddOverlay);
	DDX_Control(pDX, IDC_BTNDELETEOVERLAY, m_btnDelOverlay);
	DDX_Control(pDX, IDC_BTNEDITOVERLAY, m_btnEditOverlay);
	DDX_Control(pDX, IDC_BTNEDITOVERLAY_TEXT, m_btnEditOverlayText);
	DDX_Control(pDX, IDC_BTNBACK, m_btnBack);
	DDX_Control(pDX, IDC_BTN_NAME_EDIT, m_btnNameEdit);
	DDX_Control(pDX, IDC_BTN_NAME_CANCEL, m_btnNameCancel);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_BTN_NAME_SAVE, m_btnNameSave);
}


BEGIN_MESSAGE_MAP(RightOverlayChildDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTNADDOVERLAY, &RightOverlayChildDlg::OnBnClickedBtnaddoverlay)
	ON_BN_CLICKED(IDC_BTNDELETEOVERLAY, &RightOverlayChildDlg::OnBnClickedBtndeleteoverlay)
	ON_BN_CLICKED(IDC_BTNEDITOVERLAY, &RightOverlayChildDlg::OnBnClickedBtneditoverlay)
	ON_BN_CLICKED(IDC_BTNBACK, &RightOverlayChildDlg::OnBnClickedBtnback)
	ON_BN_CLICKED(IDC_BTNEDITOVERLAY_TEXT, &RightOverlayChildDlg::OnBnClickedBtnEditText)
	ON_BN_CLICKED(IDC_BTN_NAME_EDIT, &RightOverlayChildDlg::OnBnClickedBtnNameEdit)
	ON_BN_CLICKED(IDC_BTN_NAME_CANCEL, &RightOverlayChildDlg::OnBnClickedBtnNameCancel)
	ON_BN_CLICKED(IDC_BTN_NAME_SAVE, &RightOverlayChildDlg::OnBnClickedBtnNameSave)
	ON_EN_CHANGE(IDC_EDIT_NAME, &RightOverlayChildDlg::OnEnChangeEditName)
END_MESSAGE_MAP()


// RightOverlayChildDlg 메시지 처리기입니다.
BOOL RightOverlayChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect client, rc, btnRc_editText;
	GetWindowRect(client);
	m_btnEditOverlayText.GetWindowRect(btnRc_editText);

	m_dlgOverlayEditPos = new COverlayEditPositionDlg;
	m_dlgOverlayEditPos->Create(COverlayEditPositionDlg::IDD, this);
	m_dlgOverlayEditPos->GetWindowRect(rc);
	m_dlgOverlayEditPos->MoveWindow(0, btnRc_editText.bottom - 8, rc.Width(), rc.Height());
	m_dlgOverlayEditPos->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void RightOverlayChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		m_btnNameEdit.LoadImages(IDB_BTN_OVERLAY_EDIT_NORMAL, 0, IDB_BTN_OVERLAY_EDIT_MOVER, IDB_BTN_OVERLAY_EDIT_PRESSED, 0, 0, _T("PNG"));
		m_btnNameEdit.SizeToContent();

		m_btnNameSave.LoadImages(IDB_BTN_OVERLAY_SAVE_NORMAL, 0, IDB_BTN_OVERLAY_SAVE_MOVER, IDB_BTN_OVERLAY_SAVE_PRESSED, 0, 0, _T("PNG"));
		m_btnNameSave.SizeToContent();
		m_btnNameSave.ShowWindow(SW_HIDE);

		m_btnNameCancel.LoadImages(IDB_BTN_OVERLAY_CANCEL_NORMAL, 0, IDB_BTN_OVERLAY_CANCEL_MOVER, IDB_BTN_OVERLAY_CANCEL_PRESSED, 0, 0, _T("PNG"));
		m_btnNameCancel.SizeToContent();
		m_btnNameCancel.ShowWindow(SW_HIDE);

		m_btnAddOverlay.LoadImages(IDB_EXPORT_BTN_ADD_NORMAL, 0, IDB_EXPORT_BTN_ADD_MOVER, IDB_EXPORT_BTN_ADD_PRESSED, 0, 0, _T("PNG"));
		m_btnAddOverlay.SizeToContent();

		m_btnDelOverlay.LoadImages(IDB_EXPORT_BTN_DELETE_NORMAL, 0, IDB_EXPORT_BTN_DELETE_MOVER, IDB_EXPORT_BTN_DELETE_PRESSED, 0, 0, _T("PNG"));
		m_btnDelOverlay.SizeToContent();

		m_btnEditOverlay.LoadImages(IDB_EXPORT_BTN_EDITOFF_NORMAL, IDB_EXPORT_BTN_EDITON_NORMAL, IDB_EXPORT_BTN_EDITOFF_MOVER, IDB_EXPORT_BTN_EDITOFF_PRESSED, IDB_EXPORT_BTN_EDITON_MOVER, IDB_EXPORT_BTN_EDITON_PRESSED, _T("PNG"));
		m_btnEditOverlay.SizeToContent();

		m_btnEditOverlayText.LoadImages(IDB_EXPORT_BTN_TEXTEDIT_NORMAL, 0, IDB_EXPORT_BTN_TEXTEDIT_MOVER, IDB_EXPORT_BTN_TEXTEDIT_PRESSED, 0, 0, _T("PNG"));
		m_btnEditOverlayText.SizeToContent();
		m_btnEditOverlayText.ShowWindow(SW_HIDE);

		m_btnBack.LoadImages(IDB_EXPORT_BTN_BACK_NORMAL, 0, IDB_EXPORT_BTN_BACK_MOVER, IDB_EXPORT_BTN_BACK_PRESSED, 0, 0, _T("PNG"));
		m_btnBack.SizeToContent();

		setEditoverlayText(false);

		m_btnEditOverlay.SetImage(CGdipButton::STD_TYPE);
		m_btnAddOverlay.Enable(&m_btnAddOverlay);
		m_btnDelOverlay.Disable(&m_btnDelOverlay);

		CRect centerRect, clientRect;
		if ((CCenterView*)theApp.m_ScreenManager.m_pCenterView != NULL)
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->GetClientRect(&centerRect);
		this->GetClientRect(&clientRect);
		this->MoveWindow(0, 0, clientRect.Width(), centerRect.bottom + 50, SWP_NOMOVE);            // 크기는 알아서 맞게 지정

		dlgUpdate();
	}
	else
	{
		m_dlgOverlayEditPos->ShowWindow(SW_HIDE);
		m_btnEditOverlayText.ShowWindow(SW_HIDE);
		m_btnEditOverlayText.Disable(&m_btnEditOverlayText);

		if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isAddOverlayDlgShow)
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowAddOverlay(FALSE);

	}
}


void RightOverlayChildDlg::OnBnClickedBtnaddoverlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView))
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowAddOverlay(TRUE);
}


void RightOverlayChildDlg::OnBnClickedBtndeleteoverlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void RightOverlayChildDlg::OnBnClickedBtneditoverlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isAddOverlayDlgShow)
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowAddOverlay(FALSE);

	setEditoverlayText(false);

	if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isOverlayEdit)
	{
		m_btnAddOverlay.Enable(&m_btnAddOverlay);
		m_btnDelOverlay.Disable(&m_btnDelOverlay);
		m_btnEditOverlay.SetImage(CGdipButton::STD_TYPE);
		m_dlgOverlayEditPos->ShowWindow(SW_HIDE);
		m_btnEditOverlayText.ShowWindow(SW_HIDE);
		//pButton->SetActiveWindow(SW_HIDE)
	}
	else
	{
		m_btnAddOverlay.Disable(&m_btnAddOverlay);
		m_btnDelOverlay.Enable(&m_btnDelOverlay);
		m_btnEditOverlay.SetImage(CGdipButton::ALT_TYPE);
		m_dlgOverlayEditPos->ShowWindow(SW_SHOW);
		m_btnEditOverlayText.ShowWindow(SW_SHOW);
		m_btnEditOverlayText.Disable(&m_btnEditOverlayText);
	}
}
void RightOverlayChildDlg::setEditoverlay(bool isEdit)
{
	if (((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->isAddOverlayDlgShow)
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowAddOverlay(FALSE);

	if (isEdit)
	{
		m_btnAddOverlay.Enable(&m_btnAddOverlay);
		m_btnDelOverlay.Disable(&m_btnDelOverlay);
		m_btnEditOverlay.SetImage(CGdipButton::STD_TYPE);
		m_dlgOverlayEditPos->ShowWindow(SW_HIDE);
		m_btnEditOverlayText.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btnAddOverlay.Disable(&m_btnAddOverlay);
		m_btnDelOverlay.Enable(&m_btnDelOverlay);
		m_btnEditOverlay.SetImage(CGdipButton::ALT_TYPE);
		m_dlgOverlayEditPos->ShowWindow(SW_SHOW);
		m_btnEditOverlayText.ShowWindow(SW_SHOW);
		m_btnEditOverlayText.Disable(&m_btnEditOverlayText);
	}
}

void RightOverlayChildDlg::OnBnClickedBtnback()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

BOOL RightOverlayChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void RightOverlayChildDlg::OnBnClickedBtnEditText()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CCenterView* CCV = ((CCenterView*)theApp.m_ScreenManager.m_pCenterView);
	//if (CCV){
	//	CCV->ShowAddOverlay(TRUE);
	//	CCV->m_pdlgAddOverlay->GetTextOverlayInfo(CCV->mTargetTextOverlayID);
	//}

}

void RightOverlayChildDlg::OnBnClickedBtnNameEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_btnNameSave.ShowWindow(SW_SHOW);
	m_btnNameCancel.ShowWindow(SW_SHOW);
	m_btnNameEdit.ShowWindow(SW_HIDE);
	m_editName.EnableWindow(true);
}

void RightOverlayChildDlg::OnBnClickedBtnNameCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_btnNameEdit.ShowWindow(SW_SHOW);
	m_btnNameCancel.ShowWindow(SW_HIDE);
	m_btnNameSave.ShowWindow(SW_HIDE);
	m_editName.EnableWindow(false);
}


void RightOverlayChildDlg::OnBnClickedBtnNameSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	GetDlgItemText(IDC_EDIT_NAME, temp);

	if (temp.GetLength() == 0)
	{
		temp = _T("No Named");
		SetDlgItemText(IDC_EDIT_NAME, temp);
	}

	m_btnNameEdit.ShowWindow(SW_SHOW);
	m_btnNameSave.ShowWindow(SW_HIDE);
	m_btnNameCancel.ShowWindow(SW_HIDE);
	m_editName.EnableWindow(false);
}


void RightOverlayChildDlg::OnEnChangeEditName()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemTextW(IDC_EDIT_NAME, m_culName);

	if (strlen((const char*)(CStringA)m_culName) > 20)
	{
		//printf("bigheadk, test text limit!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		SetDlgItemTextW(IDC_EDIT_NAME, m_preName);
		m_editName.SetSel(-1);
	}
	else
	{
		m_preName = m_culName;
	}

	UpdateData();
}

void RightOverlayChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMER_ADDINGOVERLAY)
	{
		KillTimer(TIMER_ADDINGOVERLAY);
		theApp.m_ScreenManager.ShowToast(vdstToastDone, _T(" "), FALSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void RightOverlayChildDlg::setEditoverlayText(bool isEditText)
{
	if (isEditText)
		m_btnEditOverlayText.Enable(&m_btnEditOverlayText);
	else
		m_btnEditOverlayText.Disable(&m_btnEditOverlayText);
}

void RightOverlayChildDlg::dlgUpdate()
{
	CRect clientRect, btnRect;
	this->GetClientRect(&clientRect);
	
	m_btnBack.GetClientRect(&btnRect);
	m_btnBack.MoveWindow(8, clientRect.bottom - btnRect.Height(), btnRect.Width(), btnRect.Height(), SWP_NOSIZE);
}