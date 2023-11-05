// PanoPreViewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "BottomSourceChildDlg.h"
#include "PanoPreViewDlg.h"
#include "afxdialogex.h"
#include "FormView1.h"

// CPanoPreViewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPanoPreViewDlg, CDialogEx)

CPanoPreViewDlg::CPanoPreViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPanoPreViewDlg::IDD, pParent)
{
	m_pPreView = NULL;
}

CPanoPreViewDlg::~CPanoPreViewDlg()
{
}

void CPanoPreViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPanoPreViewDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPanoPreViewDlg �޽��� ó�����Դϴ�.


BOOL CPanoPreViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CXTPSkinManager* pSkinManager = XTPSkinManager();

	CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + SUBSKIN);
	pSkinManager->SetWindowTheme(GetSafeHwnd(), pSchema);
	CMDTARGET_RELEASE(pSchema);

	CRect rc;
	GetClientRect(&rc);

	CCreateContext cc;
//�ӽ÷� ���� ���� Gl2�� ��� �Ұ�.
// 	CView* pView = (CView*)RUNTIME_CLASS(CFormView1)->CreateObject();
// 	ZeroMemory(&cc, sizeof(cc));
// 	pView->Create(nullptr, nullptr, WS_CHILD, rc, this, IDD_PANORAMAPREVIEW, &cc);
// 	pView->OnInitialUpdate();
// 	m_pPreView = pView;
// 	m_pPreView->ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPanoPreViewDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (m_pPreView)
	{
		if (bShow)
			m_pPreView->ShowWindow(SW_SHOW);
		else
			m_pPreView->ShowWindow(SW_HIDE);
	}
}


void CPanoPreViewDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (theApp.m_ScreenManager.m_pBottomSourceChild)
	{
		theApp.m_ScreenManager.m_pBottomSourceChild->m_btnPanoPrevie.SetImage(CGdipButton::STD_TYPE);
		theApp.m_ScreenManager.m_pBottomSourceChild->m_bisShowPanoPreview = !theApp.m_ScreenManager.m_pBottomSourceChild->m_bisShowPanoPreview;
	}
	theApp.m_ScreenManager.m_pBottomSourceChild->PostMessage(WM_CLOSECOLORDLG, 0, 0);
	CDialogEx::OnClose();
}


BOOL CPanoPreViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
