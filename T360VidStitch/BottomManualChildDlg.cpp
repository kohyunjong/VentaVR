// BottomManualChildDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "BottomManualChildDlg.h"
#include "afxdialogex.h"


// CBottomManualChildDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBottomManualChildDlg, CDialogEx)

CBottomManualChildDlg::CBottomManualChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBottomManualChildDlg::IDD, pParent)
{

}

CBottomManualChildDlg::~CBottomManualChildDlg()
{
}

void CBottomManualChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_LEFT1, m_btnL1);
	DDX_Control(pDX, IDC_BTN_LEFT2, m_btnL2);
	DDX_Control(pDX, IDC_BTN_LEFT3, m_btnL3);
	DDX_Control(pDX, IDC_BTN_LEFT4, m_btnL4);
	DDX_Control(pDX, IDC_BTN_LEFT5, m_btnL5);
	DDX_Control(pDX, IDC_BTN_LEFT6, m_btnL6);
	DDX_Control(pDX, IDC_BTN_LEFT7, m_btnL7);
	DDX_Control(pDX, IDC_BTN_LEFT8, m_btnL8);
	DDX_Control(pDX, IDC_BTN_RIGHT1, m_btnR1);
	DDX_Control(pDX, IDC_BTN_RIGHT2, m_btnR2);
	DDX_Control(pDX, IDC_BTN_RIGHT3, m_btnR3);
	DDX_Control(pDX, IDC_BTN_RIGHT4, m_btnR4);
	DDX_Control(pDX, IDC_BTN_RIGHT5, m_btnR5);
	DDX_Control(pDX, IDC_BTN_RIGHT6, m_btnR6);
	DDX_Control(pDX, IDC_BTN_RIGHT7, m_btnR7);
	DDX_Control(pDX, IDC_BTN_RIGHT8, m_btnR8);
}


BEGIN_MESSAGE_MAP(CBottomManualChildDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_LEFT1, &CBottomManualChildDlg::OnBnClickedBtnLeft1)
	ON_BN_CLICKED(IDC_BTN_LEFT2, &CBottomManualChildDlg::OnBnClickedBtnLeft2)
	ON_BN_CLICKED(IDC_BTN_LEFT3, &CBottomManualChildDlg::OnBnClickedBtnLeft3)
	ON_BN_CLICKED(IDC_BTN_LEFT4, &CBottomManualChildDlg::OnBnClickedBtnLeft4)
	ON_BN_CLICKED(IDC_BTN_LEFT5, &CBottomManualChildDlg::OnBnClickedBtnLeft5)
	ON_BN_CLICKED(IDC_BTN_LEFT6, &CBottomManualChildDlg::OnBnClickedBtnLeft6)
	ON_BN_CLICKED(IDC_BTN_LEFT7, &CBottomManualChildDlg::OnBnClickedBtnLeft7)
	ON_BN_CLICKED(IDC_BTN_LEFT8, &CBottomManualChildDlg::OnBnClickedBtnLeft8)
	ON_BN_CLICKED(IDC_BTN_RIGHT1, &CBottomManualChildDlg::OnBnClickedBtnRight1)
	ON_BN_CLICKED(IDC_BTN_RIGHT2, &CBottomManualChildDlg::OnBnClickedBtnRight2)
	ON_BN_CLICKED(IDC_BTN_RIGHT3, &CBottomManualChildDlg::OnBnClickedBtnRight3)
	ON_BN_CLICKED(IDC_BTN_RIGHT4, &CBottomManualChildDlg::OnBnClickedBtnRight4)
	ON_BN_CLICKED(IDC_BTN_RIGHT5, &CBottomManualChildDlg::OnBnClickedBtnRight5)
	ON_BN_CLICKED(IDC_BTN_RIGHT6, &CBottomManualChildDlg::OnBnClickedBtnRight6)
	ON_BN_CLICKED(IDC_BTN_RIGHT7, &CBottomManualChildDlg::OnBnClickedBtnRight7)
	ON_BN_CLICKED(IDC_BTN_RIGHT8, &CBottomManualChildDlg::OnBnClickedBtnRight8)
END_MESSAGE_MAP()


// CBottomManualChildDlg 메시지 처리기입니다.


void CBottomManualChildDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rcClient(0, 0, cx, cy);
	CRect rc, rc1;
	int nWidth, nHeight;
	GetClientRect(rcClient);

	//LeftBTN 위치 조정
	if (::IsWindow(m_btnL1.m_hWnd))
	{
		m_btnL1.GetClientRect(&rc);
		nWidth = rc.Width();
		nHeight = rc.Height();

		rc.left = rcClient.left + 3;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnL1.MoveWindow(rc);
	}
	if (::IsWindow(m_btnL2.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnL2.MoveWindow(rc);
	}
	if (::IsWindow(m_btnL3.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnL3.MoveWindow(rc);
	}
	if (::IsWindow(m_btnL4.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnL4.MoveWindow(rc);
	}
	if (::IsWindow(m_btnL5.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnL5.MoveWindow(rc);
	}
	if (::IsWindow(m_btnL6.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnL6.MoveWindow(rc);
	}
	if (::IsWindow(m_btnL7.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnL7.MoveWindow(rc);
	}
	if (::IsWindow(m_btnL8.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnL8.MoveWindow(rc);
	}

	//RightBTN 위치 조정
	if (::IsWindow(m_btnR1.m_hWnd))
	{
		m_btnR1.GetClientRect(&rc);
		nWidth = rc.Width();
		nHeight = rc.Height();

		rc.left = rcClient.Width() / 2 - 75;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnR1.MoveWindow(rc);
	}
	if (::IsWindow(m_btnR2.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnR2.MoveWindow(rc);
	}
	if (::IsWindow(m_btnR3.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnR3.MoveWindow(rc);
	}
	if (::IsWindow(m_btnR4.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnR4.MoveWindow(rc);
	}
	if (::IsWindow(m_btnR5.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnR5.MoveWindow(rc);
	}
	if (::IsWindow(m_btnR6.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnR6.MoveWindow(rc);
	}
	if (::IsWindow(m_btnR7.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnR7.MoveWindow(rc);
	}
	if (::IsWindow(m_btnR8.m_hWnd))
	{
		rc.left += nWidth;
		rc.top = rc.top;
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;

		m_btnR8.MoveWindow(rc);
		Invalidate(FALSE);
	}
}


void CBottomManualChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		//왼쪽 버튼
		m_btnL1.LoadImages(IDB_BTN_BTMMANUAL_1_NORMAL, IDB_BTN_BTMMANUAL_1_SELECTED, IDB_BTN_BTMMANUAL_1_MOVER, IDB_BTN_BTMMANUAL_1_PRESSED, IDB_BTN_BTMMANUAL_1_SELECTED_MOVER, IDB_BTN_BTMMANUAL_1_SELECTED_PRESSED, _T("PNG"));
		m_btnL1.SizeToContent();
		m_btnL2.LoadImages(IDB_BTN_BTMMANUAL_2_NORMAL, IDB_BTN_BTMMANUAL_2_SELECTED, IDB_BTN_BTMMANUAL_2_MOVER, IDB_BTN_BTMMANUAL_2_PRESSED, IDB_BTN_BTMMANUAL_2_SELECTED_MOVER, IDB_BTN_BTMMANUAL_2_SELECTED_PRESSED, _T("PNG"));
		m_btnL2.SizeToContent();
		m_btnL3.LoadImages(IDB_BTN_BTMMANUAL_3_NORMAL, IDB_BTN_BTMMANUAL_3_SELECTED, IDB_BTN_BTMMANUAL_3_MOVER, IDB_BTN_BTMMANUAL_3_PRESSED, IDB_BTN_BTMMANUAL_3_SELECTED_MOVER, IDB_BTN_BTMMANUAL_3_SELECTED_PRESSED, _T("PNG"));
		m_btnL3.SizeToContent();
		m_btnL4.LoadImages(IDB_BTN_BTMMANUAL_4_NORMAL, IDB_BTN_BTMMANUAL_4_SELECTED, IDB_BTN_BTMMANUAL_4_MOVER, IDB_BTN_BTMMANUAL_4_PRESSED, IDB_BTN_BTMMANUAL_4_SELECTED_MOVER, IDB_BTN_BTMMANUAL_4_SELECTED_PRESSED, _T("PNG"));
		m_btnL4.SizeToContent();
		m_btnL5.LoadImages(IDB_BTN_BTMMANUAL_5_NORMAL, IDB_BTN_BTMMANUAL_5_SELECTED, IDB_BTN_BTMMANUAL_5_MOVER, IDB_BTN_BTMMANUAL_5_PRESSED, IDB_BTN_BTMMANUAL_5_SELECTED_MOVER, IDB_BTN_BTMMANUAL_5_SELECTED_PRESSED, _T("PNG"));
		m_btnL5.SizeToContent();
		m_btnL6.LoadImages(IDB_BTN_BTMMANUAL_6_NORMAL, IDB_BTN_BTMMANUAL_6_SELECTED, IDB_BTN_BTMMANUAL_6_MOVER, IDB_BTN_BTMMANUAL_6_PRESSED, IDB_BTN_BTMMANUAL_6_SELECTED_MOVER, IDB_BTN_BTMMANUAL_6_SELECTED_PRESSED, _T("PNG"));
		m_btnL6.SizeToContent();
		m_btnL7.LoadImages(IDB_BTN_BTMMANUAL_7_NORMAL, IDB_BTN_BTMMANUAL_7_SELECTED, IDB_BTN_BTMMANUAL_7_MOVER, IDB_BTN_BTMMANUAL_7_PRESSED, IDB_BTN_BTMMANUAL_7_SELECTED_MOVER, IDB_BTN_BTMMANUAL_7_SELECTED_PRESSED, _T("PNG"));
		m_btnL7.SizeToContent();
		m_btnL8.LoadImages(IDB_BTN_BTMMANUAL_8_NORMAL, IDB_BTN_BTMMANUAL_8_SELECTED, IDB_BTN_BTMMANUAL_8_MOVER, IDB_BTN_BTMMANUAL_8_PRESSED, IDB_BTN_BTMMANUAL_8_SELECTED_MOVER, IDB_BTN_BTMMANUAL_8_SELECTED_PRESSED, _T("PNG"));
		m_btnL8.SizeToContent();

		//오른쪽 버튼
		m_btnR1.LoadImages(IDB_BTN_BTMMANUAL_1_NORMAL, IDB_BTN_BTMMANUAL_1_SELECTED, IDB_BTN_BTMMANUAL_1_MOVER, IDB_BTN_BTMMANUAL_1_PRESSED, IDB_BTN_BTMMANUAL_1_SELECTED_MOVER, IDB_BTN_BTMMANUAL_1_SELECTED_PRESSED, _T("PNG"));
		m_btnR1.SizeToContent();
		m_btnR2.LoadImages(IDB_BTN_BTMMANUAL_2_NORMAL, IDB_BTN_BTMMANUAL_2_SELECTED, IDB_BTN_BTMMANUAL_2_MOVER, IDB_BTN_BTMMANUAL_2_PRESSED, IDB_BTN_BTMMANUAL_2_SELECTED_MOVER, IDB_BTN_BTMMANUAL_2_SELECTED_PRESSED, _T("PNG"));
		m_btnR2.SizeToContent();
		m_btnR3.LoadImages(IDB_BTN_BTMMANUAL_3_NORMAL, IDB_BTN_BTMMANUAL_3_SELECTED, IDB_BTN_BTMMANUAL_3_MOVER, IDB_BTN_BTMMANUAL_3_PRESSED, IDB_BTN_BTMMANUAL_3_SELECTED_MOVER, IDB_BTN_BTMMANUAL_3_SELECTED_PRESSED, _T("PNG"));
		m_btnR3.SizeToContent();
		m_btnR4.LoadImages(IDB_BTN_BTMMANUAL_4_NORMAL, IDB_BTN_BTMMANUAL_4_SELECTED, IDB_BTN_BTMMANUAL_4_MOVER, IDB_BTN_BTMMANUAL_4_PRESSED, IDB_BTN_BTMMANUAL_4_SELECTED_MOVER, IDB_BTN_BTMMANUAL_4_SELECTED_PRESSED, _T("PNG"));
		m_btnR4.SizeToContent();
		m_btnR5.LoadImages(IDB_BTN_BTMMANUAL_5_NORMAL, IDB_BTN_BTMMANUAL_5_SELECTED, IDB_BTN_BTMMANUAL_5_MOVER, IDB_BTN_BTMMANUAL_5_PRESSED, IDB_BTN_BTMMANUAL_5_SELECTED_MOVER, IDB_BTN_BTMMANUAL_5_SELECTED_PRESSED, _T("PNG"));
		m_btnR5.SizeToContent();
		m_btnR6.LoadImages(IDB_BTN_BTMMANUAL_6_NORMAL, IDB_BTN_BTMMANUAL_6_SELECTED, IDB_BTN_BTMMANUAL_6_MOVER, IDB_BTN_BTMMANUAL_6_PRESSED, IDB_BTN_BTMMANUAL_6_SELECTED_MOVER, IDB_BTN_BTMMANUAL_6_SELECTED_PRESSED, _T("PNG"));
		m_btnR6.SizeToContent();
		m_btnR7.LoadImages(IDB_BTN_BTMMANUAL_7_NORMAL, IDB_BTN_BTMMANUAL_7_SELECTED, IDB_BTN_BTMMANUAL_7_MOVER, IDB_BTN_BTMMANUAL_7_PRESSED, IDB_BTN_BTMMANUAL_7_SELECTED_MOVER, IDB_BTN_BTMMANUAL_7_SELECTED_PRESSED, _T("PNG"));
		m_btnR7.SizeToContent();
		m_btnR8.LoadImages(IDB_BTN_BTMMANUAL_8_NORMAL, IDB_BTN_BTMMANUAL_8_SELECTED, IDB_BTN_BTMMANUAL_8_MOVER, IDB_BTN_BTMMANUAL_8_PRESSED, IDB_BTN_BTMMANUAL_8_SELECTED_MOVER, IDB_BTN_BTMMANUAL_8_SELECTED_PRESSED, _T("PNG"));
		m_btnR8.SizeToContent();

		OnShowBtn();
		OnCulSelectedBtn();
	}
}

void CBottomManualChildDlg::OnShowBtn()
{
	int nCamera = QVS_Engine::QVS_GetNumInput() + 1;

	switch (nCamera)
	{
	case 1:
		m_btnL1.ShowWindow(SW_HIDE);
		m_btnR1.ShowWindow(SW_HIDE);
	case 2:
		m_btnL2.ShowWindow(SW_HIDE);
		m_btnR2.ShowWindow(SW_HIDE);
	case 3:
		m_btnL3.ShowWindow(SW_HIDE);
		m_btnR3.ShowWindow(SW_HIDE);
	case 4:
		m_btnL4.ShowWindow(SW_HIDE);
		m_btnR4.ShowWindow(SW_HIDE);
	case 5:
		m_btnL5.ShowWindow(SW_HIDE);
		m_btnR5.ShowWindow(SW_HIDE);
	case 6:
		m_btnL6.ShowWindow(SW_HIDE);
		m_btnR6.ShowWindow(SW_HIDE);
	case 7:
		m_btnL7.ShowWindow(SW_HIDE);
		m_btnR7.ShowWindow(SW_HIDE);
	case 8:
		m_btnL8.ShowWindow(SW_HIDE);
		m_btnR8.ShowWindow(SW_HIDE);
		break;
	}
}

void CBottomManualChildDlg::OnCulSelectedBtn()
{
	int LeftView = QVS_Engine::QVS_GetDrawMaskLeftIndex();
	int RightView = QVS_Engine::QVS_GetDrawMaskRightIndex();

	NotSelectedBtn(LEFT_VIEW);
	NotSelectedBtn(RIGHT_VIEW);

	CGdipButton* LeftBtn = (CGdipButton*)GetDlgItem(IDC_BTN_LEFT1 + LeftView);
	LeftBtn->SetImage(CGdipButton::ALT_TYPE);

	CGdipButton* RightBtn = (CGdipButton*)GetDlgItem(IDC_BTN_RIGHT1 + RightView);
	RightBtn->SetImage(CGdipButton::ALT_TYPE);
}

void CBottomManualChildDlg::OnBnClickedBtnLeft1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(LEFT_VIEW);
	m_btnL1.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskLeftIndex(0);
}


void CBottomManualChildDlg::OnBnClickedBtnLeft2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(LEFT_VIEW);
	m_btnL2.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskLeftIndex(1);
}


void CBottomManualChildDlg::OnBnClickedBtnLeft3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(LEFT_VIEW);
	m_btnL3.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskLeftIndex(2);
}


void CBottomManualChildDlg::OnBnClickedBtnLeft4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(LEFT_VIEW);
	m_btnL4.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskLeftIndex(3);
}


void CBottomManualChildDlg::OnBnClickedBtnLeft5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(LEFT_VIEW);
	m_btnL5.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskLeftIndex(4);
}


void CBottomManualChildDlg::OnBnClickedBtnLeft6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(LEFT_VIEW);
	m_btnL6.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskLeftIndex(5);
}


void CBottomManualChildDlg::OnBnClickedBtnLeft7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(LEFT_VIEW);
	m_btnL7.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskLeftIndex(6);
}


void CBottomManualChildDlg::OnBnClickedBtnLeft8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(LEFT_VIEW);
	m_btnL8.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskLeftIndex(7);
}


void CBottomManualChildDlg::OnBnClickedBtnRight1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(RIGHT_VIEW);
	m_btnR1.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskRightIndex(0);
}


void CBottomManualChildDlg::OnBnClickedBtnRight2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(RIGHT_VIEW);
	m_btnR2.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskRightIndex(1);
}


void CBottomManualChildDlg::OnBnClickedBtnRight3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(RIGHT_VIEW);
	m_btnR3.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskRightIndex(2);
}


void CBottomManualChildDlg::OnBnClickedBtnRight4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(RIGHT_VIEW);
	m_btnR4.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskRightIndex(3);
}


void CBottomManualChildDlg::OnBnClickedBtnRight5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(RIGHT_VIEW);
	m_btnR5.SetImage(CGdipButton::ALT_TYPE);
	
	QVS_Engine::QVS_SetDrawMaskRightIndex(4);
}


void CBottomManualChildDlg::OnBnClickedBtnRight6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(RIGHT_VIEW);
	m_btnR6.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskRightIndex(5);
}


void CBottomManualChildDlg::OnBnClickedBtnRight7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(RIGHT_VIEW);
	m_btnR7.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskRightIndex(6);
}


void CBottomManualChildDlg::OnBnClickedBtnRight8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NotSelectedBtn(RIGHT_VIEW);
	m_btnR8.SetImage(CGdipButton::ALT_TYPE);

	QVS_Engine::QVS_SetDrawMaskRightIndex(7);
}

void CBottomManualChildDlg::NotSelectedBtn(int nType)
{
	if (nType == LEFT_VIEW)
	{
		m_btnL1.SetImage(CGdipButton::STD_TYPE);
		m_btnL2.SetImage(CGdipButton::STD_TYPE);
		m_btnL3.SetImage(CGdipButton::STD_TYPE);
		m_btnL4.SetImage(CGdipButton::STD_TYPE);
		m_btnL5.SetImage(CGdipButton::STD_TYPE);
		m_btnL6.SetImage(CGdipButton::STD_TYPE);
		m_btnL7.SetImage(CGdipButton::STD_TYPE);
		m_btnL8.SetImage(CGdipButton::STD_TYPE);
	}

	if (nType == RIGHT_VIEW)
	{
		m_btnR1.SetImage(CGdipButton::STD_TYPE);
		m_btnR2.SetImage(CGdipButton::STD_TYPE);
		m_btnR3.SetImage(CGdipButton::STD_TYPE);
		m_btnR4.SetImage(CGdipButton::STD_TYPE);
		m_btnR5.SetImage(CGdipButton::STD_TYPE);
		m_btnR6.SetImage(CGdipButton::STD_TYPE);
		m_btnR7.SetImage(CGdipButton::STD_TYPE);
		m_btnR8.SetImage(CGdipButton::STD_TYPE);
	}
}