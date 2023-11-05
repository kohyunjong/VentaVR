// ConfiguChildView3.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfiguChildView3.h"
#include "afxdialogex.h"


// CConfiguChildView3 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CConfiguChildView3, CDialogEx)

CConfiguChildView3::CConfiguChildView3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfiguChildView3::IDD, pParent)
{

}

CConfiguChildView3::~CConfiguChildView3()
{
}

void CConfiguChildView3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfiguChildView3, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CConfiguChildView3 �޽��� ó�����Դϴ�.


BOOL CConfiguChildView3::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CXTPClientRect rc(this);
	m_wndTaskPanel.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP, rc, this, 0);

	m_wndTaskPanel.SetTheme(xtpTaskPanelThemeOffice2000);
	m_wndTaskPanel.NavigateItems(TRUE);

	// TaskPanel�� Title Caption ������ ����.. KBJ...
	CXTPTaskPanelPaintManager* pPaintM = m_wndTaskPanel.GetPaintManager();
	XTP_TASKPANEL_GROUPCOLORS* pGColor = pPaintM->GetGroupColors(FALSE);
	pGColor->clrHead.clrLight.SetCustomValue(COLOR_DIALOG_BK);
	pGColor->clrHead.clrDark.SetCustomValue(COLOR_DIALOG_BK);
	pGColor->clrClient.SetStandardValue(COLOR_DIALOG_BK);
	pGColor->clrClient.clrLight.SetCustomValue(COLOR_DIALOG_BK);
	pGColor->clrClient.clrDark.SetCustomValue(COLOR_DIALOG_BK);

	CXTPTaskPanelGroup* pGroup = NULL;

	// 	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//pGroup = m_wndTaskPanel.AddGroup(0);
	//pGroup->SetCaption(_T("New Calibration"));

	//m_dlgsubNewCalConf.Create(IDD_CONFIGCHILDVIEW3_1_DLG, &m_wndTaskPanel);
	//CXTPTaskPanelGroupItem* pItem = pGroup->AddControlItem(m_dlgsubNewCalConf);
	//pItem->GetMargins().SetRect(0, 0, 0, 0);
	//m_dlgsubNewCalConf.SetItem(pItem);
	//pGroup->SetExpanded(TRUE);

	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("Import Calibration"));

	m_dlgsubImportCalConf.Create(IDD_CONFIGCHILDVIEW3_2_DLG, &m_wndTaskPanel);
	CXTPTaskPanelGroupItem* pItem = pGroup->AddControlItem(m_dlgsubImportCalConf);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgsubImportCalConf.SetItem(pItem);

	pGroup->SetExpanded(TRUE);
	pGroup->SetExpandable(FALSE);

	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("Snapshots Directory Settings"));

	m_dlgsubSnapshotDSConf.Create(IDD_CONFIGCHILDVIEW3_3_DLG, &m_wndTaskPanel);
	pItem = pGroup->AddControlItem(m_dlgsubSnapshotDSConf);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgsubSnapshotDSConf.SetItem(pItem);

	pGroup->SetExpanded(TRUE);
	pGroup->SetExpandable(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CConfiguChildView3::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_wndTaskPanel)
		m_wndTaskPanel.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
}


void CConfiguChildView3::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow)
	{
		/*m_dlgsubNewCalConf.SetValues();
		m_dlgsubImportCalConf.SetValues();
		m_dlgsubSnapshotDSConf.SetValues();*/
	}
}


BOOL CConfiguChildView3::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CConfiguChildView3::clearValue()
{
	//m_dlgsubNewCalConf.clearValue();
	m_dlgsubImportCalConf.clearValue();
	//m_dlgsubSnapshotDSConf.clearValue();
}

void CConfiguChildView3::setValue()
{
	m_dlgsubNewCalConf.SetValues();
	m_dlgsubImportCalConf.SetValues();
	m_dlgsubSnapshotDSConf.SetValues();
}