// ConfPanoramaChildTwo.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfPanoramaChildTwo.h"
#include "afxdialogex.h"


// CConfPanoramaChildTwo ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CConfPanoramaChildTwo, CTaskPanelDlg)

CConfPanoramaChildTwo::CConfPanoramaChildTwo(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfPanoramaChildTwo::~CConfPanoramaChildTwo()
{
}

void CConfPanoramaChildTwo::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_2_2_SAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CConfPanoramaChildTwo, CTaskPanelDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG_2_2_SAVE, &CConfPanoramaChildTwo::OnBnClickedBtnConfig22Save)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_CFPANO_2_EDITRENDER, &CConfPanoramaChildTwo::OnEnChangeCfpano2Editrender)
END_MESSAGE_MAP()


// CConfPanoramaChildTwo �޽��� ó�����Դϴ�.
BOOL CConfPanoramaChildTwo::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();

	//SetDlgItemTextW(IDC_CFPANO_2_EDITRENDER, _T("60.0"));

	SetFontObj();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CConfPanoramaChildTwo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CTaskPanelDlg::PreTranslateMessage(pMsg);
}


void CConfPanoramaChildTwo::OnBnClickedBtnConfig22Save()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString editValue;
	GetDlgItemText(IDC_CFPANO_2_EDITRENDER, editValue);

	double render_fps = _wtof(editValue);

	if (render_fps > 60.00){
		render_fps = 60.00;
	}
	else if (render_fps < 1.00){
		render_fps = 1.00;
	}

	editValue.Format(_T("%.2lf"), render_fps);
	SetDlgItemText(IDC_CFPANO_2_EDITRENDER, editValue);

	QVS_Engine::QVS_setRenderFPS(render_fps);
}


void CConfPanoramaChildTwo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTaskPanelDlg::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


HBRUSH CConfPanoramaChildTwo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_12_53:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	break;
	case IDC_CFPANO_2_EDITRENDER:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT);
		pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
		hbr = hbrNomalBack;
	}
	break;
	}
	
// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


BOOL CConfPanoramaChildTwo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}

void CConfPanoramaChildTwo::SetValues()
{
	double render_fps = QVS_Engine::QVS_getRenderFPS();
	if (render_fps < 1.0)
		render_fps = 1.0;
	else if (render_fps > 60.0)
		render_fps = 60.0;

	CString temp;
	temp.Format(_T("%.2lf"), render_fps);
	SetDlgItemText(IDC_CFPANO_2_EDITRENDER, temp);
}

void CConfPanoramaChildTwo::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	//GetDlgItem(IDC_CFPANO_1_HEIGHT)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_53)->SetFont(aria12);
	GetDlgItem(IDC_CFPANO_2_EDITRENDER)->SetFont(aria12);
}

double CConfPanoramaChildTwo::OnChangeStrToDouble(CString strValue)
{
	for (int i = 0; i < strValue.GetLength(); i++)
	{
		if (!((48 <= strValue.GetAt(i) && strValue.GetAt(i) <= 57) || '.' == strValue.GetAt(i)))	//���� ,  '.' ���� Ȯ��
		{
			//  0~ 9 , '.'  �̿� �ٸ����� �Է�
			if (strValue.GetLength() > 10)	// ���ڰ� �ѹ��� ���� ������ �ʱ�ȭ 
				strValue = _T("");
			else
				strValue.Delete(i, 1);

			SetDlgItemText(IDC_CFPANO_2_EDITRENDER, strValue);
			((CEdit*)GetDlgItem(IDC_CFPANO_2_EDITRENDER))->SetSel(-1, -1);
		}
	}
	return _wtof(strValue);
}

void CConfPanoramaChildTwo::OnEnChangeCfpano2Editrender()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CTaskPanelDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString editValue;
	GetDlgItemText(IDC_CFPANO_2_EDITRENDER, editValue);
	OnChangeStrToDouble(editValue);		//���߿� ���ϰ��� FPS �ɹ������� ������
}
