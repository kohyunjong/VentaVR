// ConfOutputsChildFour.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfOutputsChildFour.h"
#include "afxdialogex.h"


// CConfOutputsChildFour ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CConfOutputsChildFour, CTaskPanelDlg)

CConfOutputsChildFour::CConfOutputsChildFour(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfOutputsChildFour::~CConfOutputsChildFour()
{
}

void CConfOutputsChildFour::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_1_4_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_CONFIG1_4_BROW, m_btnBrowse);
	DDX_Control(pDX, IDC_CBO_CFOUT4_REPEAT, m_cboRepeat);
}


BEGIN_MESSAGE_MAP(CConfOutputsChildFour, CTaskPanelDlg)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_CONFIG1_4_BROW, &CConfOutputsChildFour::OnBnClickedBtnConfig14Brow)
	ON_BN_CLICKED(IDC_BTN_CONFIG_1_4_SAVE, &CConfOutputsChildFour::OnBnClickedBtnConfig14Save)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CConfOutputsChildFour �޽��� ó�����Դϴ�.


HBRUSH CConfOutputsChildFour::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_14_46:
	case IDC_STATIC_14_29:
	case IDC_STATIC_14_30:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
		break;
	}
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT: // Edit box
		switch (pWnd->GetDlgCtrlID())
		{
		default:
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
			hbr = hbrNomalBack;
			break;
		}
		break;
	case CTLCOLOR_LISTBOX: // Listbox
		switch (pWnd->GetDlgCtrlID())
		{
		default:
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
			hbr = hbrNomalBack;
			break;
		}
		break;
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


BOOL CConfOutputsChildFour::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	return TRUE;
	//return CTaskPanelDlg::OnEraseBkgnd(pDC);
}

void CConfOutputsChildFour::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_CFOUT4_FILEPATH)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT4_SURL)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT4_REPEAT)->SetFont(aria12);

	GetDlgItem(IDC_STATIC_14_29)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_14_30)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_14_46)->SetFont(aria12);

}

BOOL CConfOutputsChildFour::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();

	m_btnBrowse.LoadBitmaps(IDBB_BTN_OPENFILE_NORMAL, 0, 0, IDBB_BTN_OPENFILE_DISABLE);
	m_btnBrowse.SizeToContent();

	m_cboRepeat.SetCurSel(0);

	SetFontObj();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CConfOutputsChildFour::OnBnClickedBtnConfig14Brow()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	TCHAR szFilters[] = _T("MP4(*.MP4)|*.mp4||");
	CFileDialog dlg(TRUE, _T("mp4"), _T(""), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, NULL, 0);

	//���� Path����
	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);

	if (dlg.DoModal() == IDOK)
	{
		CString szFullpath = dlg.GetPathName();
		CString szFileName = dlg.GetFileName();
		GetDlgItem(IDC_CFOUT4_FILEPATH)->SetWindowText(szFullpath);

		//Path����
		SetCurrentDirectory(Path);
	}
}


void CConfOutputsChildFour::OnBnClickedBtnConfig14Save()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vod Config save");
	CRightPanoraChildDlg* m_rightPanoDlg = (CRightPanoraChildDlg*)theApp.m_ScreenManager.m_pRightPanoramaChild;
	CString szTemp;

	GetDlgItemTextW(IDC_CFOUT4_FILEPATH, szTemp);
	m_rightPanoDlg->vod_filepath = CString2Char(szTemp);
	printf(">>>> filepath : %s", szTemp);

	GetDlgItemTextW(IDC_CFOUT4_SURL, szTemp);
	m_rightPanoDlg->vod_url = CString2Char(szTemp);
	printf(">>>> url : %s", szTemp);

	m_rightPanoDlg->vod_isRepeat = m_cboRepeat.GetCurSel();
	printf(">>>> repeat : %d\n", m_rightPanoDlg->vod_isRepeat);
}

void CConfOutputsChildFour::SetData()
{
	CRightPanoraChildDlg* m_rightPanoDlg = (CRightPanoraChildDlg*)theApp.m_ScreenManager.m_pRightPanoramaChild;
	CString szTemp;

	szTemp = Char2CString(m_rightPanoDlg->vod_filepath);
	SetDlgItemTextW(IDC_CFOUT4_FILEPATH, szTemp);

	szTemp = Char2CString(m_rightPanoDlg->vod_url);
	SetDlgItemTextW(IDC_CFOUT4_SURL, szTemp);

	m_cboRepeat.SetCurSel(m_rightPanoDlg->vod_isRepeat);
}

void CConfOutputsChildFour::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTaskPanelDlg::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow)
	{
		SetData();
	}
}
