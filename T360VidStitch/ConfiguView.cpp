// ConfiguView.cpp : implementation file
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfiguView.h"


// CConfiguView

IMPLEMENT_DYNCREATE(CConfiguView, CFormView)

CConfiguView::CConfiguView()
	: CFormView(CConfiguView::IDD)
{
	m_activeView = childOuputs;
	m_dlgChildOuputs = NULL;
	m_dlgChildPanorama = NULL;
	m_dlgChildCalibration = NULL;
}

CConfiguView::~CConfiguView()
{
	if (m_dlgChildOuputs)
		delete m_dlgChildOuputs;
	if (m_dlgChildPanorama)
		delete m_dlgChildPanorama;
	if (m_dlgChildCalibration)
		delete m_dlgChildCalibration;
}

void CConfiguView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICFRAME, m_ConfiguChildFrame);
	DDX_Control(pDX, IDC_BTN_CONFOUTPUT, m_btnOutput);
	DDX_Control(pDX, IDC_BTN_CONFPANORAMA, m_btnPanorama);
	DDX_Control(pDX, IDC_BTN_CONFCALRA, m_btnCalibtation);
	DDX_Control(pDX, IDC_STATIC_BTMTOPBAR, m_ctlTopBar);
}

BEGIN_MESSAGE_MAP(CConfiguView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_CONFOUTPUT, &CConfiguView::OnBnClickedBtnConfoutput)
	ON_BN_CLICKED(IDC_BTN_CONFPANORAMA, &CConfiguView::OnBnClickedBtnConfpanorama)
	ON_BN_CLICKED(IDC_BTN_CONFCALRA, &CConfiguView::OnBnClickedBtnConfcalra)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CConfiguView diagnostics

#ifdef _DEBUG
void CConfiguView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CConfiguView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CConfiguView message handlers
void CConfiguView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rc(0,0,cx,cy);
	CRect rc2(0, 0, cx, cy);
	if (::IsWindow(m_ConfiguChildFrame.m_hWnd))
	{
		GetParent()->GetClientRect(&rc);
		rc.top += 40;
		m_ConfiguChildFrame.MoveWindow(rc);
		m_ConfiguChildFrame.GetClientRect(rc);

		ShowScreen(m_activeView, rc);

		m_ctlTopBar.GetWindowRect(&rc2);
		ScreenToClient(&rc2);
	
		//rc2.SetRect(CPoint(rc2.TopLeft()), CPoint(rc.right - 50, rc2.bottom));
		rc2.SetRect(CPoint(rc2.TopLeft()), CPoint(rc.right - 3, rc2.bottom));
		m_ctlTopBar.MoveWindow(rc2);
	}

}
void CConfiguView::ShowScreen(vdstChildeView scr, CRect rc)
{
	switch (scr)
	{
	case CConfiguView::childOuputs:
		if (m_dlgChildOuputs != NULL)
		{
			m_activeView = childOuputs;
			m_dlgChildOuputs->ShowWindow(SW_SHOW);
			m_dlgChildOuputs->MoveWindow(rc);
		}
		break;
	case CConfiguView::childPanorama:
		if (m_dlgChildPanorama != NULL)
		{
			m_activeView = childPanorama;
			m_dlgChildPanorama->ShowWindow(SW_SHOW);
			m_dlgChildPanorama->MoveWindow(rc);
		}
		break;
	case CConfiguView::childCalibration:
		if (m_dlgChildCalibration != NULL)
		{
			m_activeView = childCalibration;
			m_dlgChildCalibration->ShowWindow(SW_SHOW);
			m_dlgChildCalibration->MoveWindow(rc);
		}
		break;
	}

	if (m_activeView != childOuputs && m_dlgChildOuputs)
		m_dlgChildOuputs->ShowWindow(SW_HIDE);
	if (m_activeView != childPanorama && m_dlgChildPanorama)
		m_dlgChildPanorama->ShowWindow(SW_HIDE);
	if (m_activeView != childCalibration && m_dlgChildCalibration)
		m_dlgChildCalibration->ShowWindow(SW_HIDE);

	ToggleButton(scr);
}

void CConfiguView::OnBnClickedBtnConfoutput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRect rc;
	m_ConfiguChildFrame.GetClientRect(rc);
	ShowScreen(childOuputs, rc);

	//ToggleButton(childOuputs);
}


void CConfiguView::OnBnClickedBtnConfpanorama()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRect rc;
	m_ConfiguChildFrame.GetClientRect(rc);
	ShowScreen(childPanorama, rc);

	//ToggleButton(childPanorama);
}


void CConfiguView::OnBnClickedBtnConfcalra()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRect rc;
	m_ConfiguChildFrame.GetClientRect(rc);
	ShowScreen(childCalibration, rc);

	//ToggleButton(childCalibration);
}

void CConfiguView::ToggleButton(int nType)
{
	switch (nType)
	{
	case childOuputs:
		m_btnOutput.SetImage(CGdipButton::ALT_TYPE);
		m_btnPanorama.SetImage(CGdipButton::STD_TYPE);
		m_btnCalibtation.SetImage(CGdipButton::STD_TYPE);

		m_dlgChildOuputs->setValue();
		break;
	case childPanorama:
		m_btnOutput.SetImage(CGdipButton::STD_TYPE);
		m_btnPanorama.SetImage(CGdipButton::ALT_TYPE);
		m_btnCalibtation.SetImage(CGdipButton::STD_TYPE);

		m_dlgChildPanorama->SetValues();
		break;
	case childCalibration:
		m_btnOutput.SetImage(CGdipButton::STD_TYPE);
		m_btnPanorama.SetImage(CGdipButton::STD_TYPE);
		m_btnCalibtation.SetImage(CGdipButton::ALT_TYPE);

		m_dlgChildCalibration->setValue();
		break;
	}
}


void CConfiguView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_dlgChildOuputs = new CConfiguChildView1;
	m_dlgChildOuputs->Create(CConfiguChildView1::IDD, GetDlgItem(IDC_STATICFRAME));
	m_dlgChildPanorama = new CConfiguChildView2;
	m_dlgChildPanorama->Create(CConfiguChildView2::IDD, GetDlgItem(IDC_STATICFRAME));
	m_dlgChildCalibration = new CConfiguChildView3;
	m_dlgChildCalibration->Create(CConfiguChildView3::IDD, GetDlgItem(IDC_STATICFRAME));	

	m_btnCalibtation.LoadImages(IDB_BTN_CONFIGTAB_CALIVRATION_NORMAL, IDB_BTN_CONFIGTAB_CALIVRATION_SELECTED, IDB_BTN_CONFIGTAB_CALIVRATION_MOVER, IDB_BTN_CONFIGTAB_CALIVRATION_PRESSED, IDB_BTN_CONFIGTAB_CALIVRATION_SELECTED, IDB_BTN_CONFIGTAB_CALIVRATION_SELECTED, _T("PNG"));
	m_btnCalibtation.SizeToContent();

	m_btnOutput.LoadImages(IDB_BTN_CONFIGTAB_OUTPUTS_NORMAL, IDB_BTN_CONFIGTAB_OUTPUTS_SELECTED, IDB_BTN_CONFIGTAB_OUTPUTS_MOVER, IDB_BTN_CONFIGTAB_OUTPUTS_PRESSED, IDB_BTN_CONFIGTAB_OUTPUTS_SELECTED, IDB_BTN_CONFIGTAB_OUTPUTS_SELECTED, _T("PNG"));
	m_btnOutput.SizeToContent();
	m_btnOutput.SetImage(CGdipButton::ALT_TYPE);

	m_btnPanorama.LoadImages(IDB_BTN_CONFIGTAB_PANORAMA_NORMAL, IDB_BTN_CONFIGTAB_PANORAMA_SELECTED, IDB_BTN_CONFIGTAB_PANORAMA_MOVER, IDB_BTN_CONFIGTAB_PANORAMA_PRESSED, IDB_BTN_CONFIGTAB_PANORAMA_SELECTED, IDB_BTN_CONFIGTAB_PANORAMA_SELECTED, _T("PNG"));
	m_btnPanorama.SizeToContent();
}


void CConfiguView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		switch (m_activeView)
		{
		case CConfiguView::childOuputs:
			m_dlgChildOuputs->setValue();
			m_dlgChildOuputs->OnShowWindow(bShow, nStatus);
			break;
		case CConfiguView::childPanorama:
			m_dlgChildPanorama->SetValues();
			m_dlgChildPanorama->OnShowWindow(bShow, nStatus);
			break;
		case CConfiguView::childCalibration:
			m_dlgChildCalibration->setValue();
			m_dlgChildCalibration->OnShowWindow(bShow, nStatus);
			break;
		}
	}
}



HBRUSH CConfiguView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATICFRAME:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	break;
	}
	return hbr;
}


BOOL CConfiguView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_BK);
	return TRUE;
}


void CConfiguView::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	//CFormView::PostNcDestroy();
}


void CConfiguView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_dlgChildOuputs)
	{
		m_dlgChildOuputs->DestroyWindow();
		delete m_dlgChildOuputs;
		m_dlgChildOuputs = NULL;
	}
	if (m_dlgChildPanorama)
	{
		m_dlgChildPanorama->DestroyWindow();
		delete m_dlgChildPanorama;
		m_dlgChildPanorama = NULL;
	}
	if (m_dlgChildCalibration)
	{
		m_dlgChildCalibration->DestroyWindow();
		delete m_dlgChildCalibration;
		m_dlgChildCalibration = NULL;
	}
}

void CConfiguView::clearValue(vdstChildeView scr)
{
	if (scr == childCalibration && m_dlgChildCalibration != NULL)
		m_dlgChildCalibration->clearValue();

	return;

	/*switch (scr)
	{
	case CConfiguView::childOuputs:
		if (m_dlgChildOuputs != NULL)
		{
		}
		break;
	case CConfiguView::childPanorama:
		if (m_dlgChildPanorama != NULL)
		{
		}
		break;
	case CConfiguView::childCalibration:
		if (m_dlgChildCalibration != NULL)
		{
			m_dlgChildCalibration->clearValue();
		}
		break;
	}*/
}

void CConfiguView::setValue()
{
	if (m_dlgChildPanorama) //Set first panorama size
		m_dlgChildPanorama->SetValues();


	if (m_dlgChildOuputs)
		m_dlgChildOuputs->setValue();
	if (m_dlgChildCalibration)
		m_dlgChildCalibration->setValue();
}