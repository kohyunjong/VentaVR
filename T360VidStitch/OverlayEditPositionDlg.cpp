// OverlayEditPositionDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "OverlayEditPositionDlg.h"
#include "afxdialogex.h"


// COverlayEditPositionDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(COverlayEditPositionDlg, CDialogEx)

COverlayEditPositionDlg::COverlayEditPositionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COverlayEditPositionDlg::IDD, pParent)
{

}

COverlayEditPositionDlg::~COverlayEditPositionDlg()
{
}

void COverlayEditPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_EDITPOS_APPLY, m_btnApply);
}


BEGIN_MESSAGE_MAP(COverlayEditPositionDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_FOV, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposFov)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_YAW, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposYaw)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_ROLL, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposRoll)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_PITCH, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposPitch)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposDiffyaw)
	ON_EN_CHANGE(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, &COverlayEditPositionDlg::OnEnChangeEditOverlayEditposDiffpitch)
	ON_BN_CLICKED(IDC_BTN_EDITPOS_APPLY, &COverlayEditPositionDlg::OnBnClickedBtnEditposApply)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// COverlayEditPositionDlg �޽��� ó�����Դϴ�.
BOOL COverlayEditPositionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_btnApply.LoadImages(IDB_BTN_APPLY_S_NORMAL, 0, IDB_BTN_APPLY_S_MOVER, IDB_BTN_APPLY_S_PRESSED, 0, 0, _T("PNG"));
	m_btnApply.SizeToContent();

	((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH))->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL COverlayEditPositionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void COverlayEditPositionDlg::SetValues(int overlayid)
{
	if (overlayid < 0) //�ʱ�ȭ
	{
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->is_overlay_selected = false;

		id = -1;

		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_FOV, _T(""));
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_YAW, _T(""));
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_ROLL, _T(""));
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_PITCH, _T(""));

		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, _T(""));
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, _T(""));
	}
	else
	{
		id = overlayid;
		double fov = 0.0, yaw = 0.0, roll = 0.0, pitch = 0.0, diff_yaw = 0.0, diff_pitch = 0.0;

		//printf(">>>>> (id, fov, yaw, roll, pitch, diff_yaw, diff_pitch) = (%d, %lf, %lf, %lf, %lf, %lf, %lf)\n", id, fov, yaw, roll, pitch, diff_yaw, diff_pitch);

		CString temp;
		temp.Format(_T("%.2lf"), fov);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_FOV, temp);

		temp.Format(_T("%.2lf"), yaw);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_YAW, temp);

		temp.Format(_T("%.2lf"), roll);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_ROLL, temp);

		temp.Format(_T("%.2lf"), pitch);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_PITCH, temp);

		temp.Format(_T("%.2lf"), diff_yaw);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, temp);

		temp.Format(_T("%.2lf"), diff_pitch);
		SetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, temp);
	}
}

void COverlayEditPositionDlg::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_STATIC_EDITPOS)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_FOV)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_YAW)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_ROLL)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_PITCH)->SetFont(aria12);

	GetDlgItem(IDC_STATIC_EDITPOS2)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_DIFFYAW)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_EDITPOS_DIFFPITCH)->SetFont(aria12);
}

double COverlayEditPositionDlg::OnChangeStrToDouble(CString strValue, int id)
{
	for (int i = 0; i < strValue.GetLength(); i++)
	{
		if (!((48 <= strValue.GetAt(i) && strValue.GetAt(i) <= 57) || '.' == strValue.GetAt(i) || '-' == strValue.GetAt(i)))	//���� ,  '.', '-' ���� Ȯ��
		{
			//  0~ 9 , '.'  �̿� �ٸ����� �Է�
			if (strValue.GetLength() > 10)	// ���ڰ� �ѹ��� ���� ������ �ʱ�ȭ 
				strValue = _T("");
			else
				strValue.Delete(i, 1);

			SetDlgItemText(id, strValue);
			((CEdit*)GetDlgItem(id))->SetSel(-1, -1);
		}
	}
	return _wtof(strValue);
}

HBRUSH COverlayEditPositionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_EDITPOS:
	case IDC_STATIC_EDITPOS_FOV:
	case IDC_STATIC_EDITPOS_YAW:
	case IDC_STATIC_EDITPOS_ROLL:
	case IDC_STATIC_EDITPOS_PITCH:
	case IDC_STATIC_EDITPOS2:
	case IDC_STATIC_EDITPOS_DIFFYAW:
	case IDC_STATIC_EDITPOS_DIFFPITCH:
	{
		pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	break;
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


BOOL COverlayEditPositionDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposFov()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_FOV, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_FOV);		//���߿� ���ϰ��� FPS �ɹ������� ������
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposYaw()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_YAW, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_YAW);		//���߿� ���ϰ��� FPS �ɹ������� ������
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposRoll()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_ROLL, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_ROLL);		//���߿� ���ϰ��� FPS �ɹ������� ������
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposPitch()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_PITCH, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_PITCH);		//���߿� ���ϰ��� FPS �ɹ������� ������
}

void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposDiffyaw()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW);		//���߿� ���ϰ��� FPS �ɹ������� ������
}


void COverlayEditPositionDlg::OnEnChangeEditOverlayEditposDiffpitch()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString editValue;
	GetDlgItemText(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, editValue);
	OnChangeStrToDouble(editValue, IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH);		//���߿� ���ϰ��� FPS �ɹ������� ������
}

void COverlayEditPositionDlg::OnBnClickedBtnEditposApply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (id >= 0)
	{
		double fov = 0.0, yaw = 0.0, roll = 0.0, pitch = 0.0, diff_yaw = 0.0, diff_pitch = 0.0;

		CString temp;
		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_FOV, temp);
		fov = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_YAW, temp);
		yaw = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_ROLL, temp);
		roll = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_PITCH, temp);
		pitch = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW, temp);
		diff_yaw = _wtof(temp);

		GetDlgItemTextW(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH, temp);
		diff_pitch = _wtof(temp);

	}
}


void COverlayEditPositionDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow)
	{
		SetValues(-1);

		if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
		{
			((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW))->EnableWindow(TRUE);
			((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH))->EnableWindow(TRUE);
		}
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFYAW))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_OVERLAY_EDITPOS_DIFFPITCH))->EnableWindow(FALSE);
	}
}