// ManualRotateDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ManualRotateDlg.h"
#include "afxdialogex.h"


// CManualRotateDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CManualRotateDlg, CDialogEx)

CManualRotateDlg::CManualRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualRotateDlg::IDD, pParent)
{

}

CManualRotateDlg::~CManualRotateDlg()
{
}

void CManualRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnOK);
}


BEGIN_MESSAGE_MAP(CManualRotateDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CManualRotateDlg::OnBnClickedBtnSave)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_YAW0, &CManualRotateDlg::OnEnChangeEditMrotateYaw0)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_YAW1, &CManualRotateDlg::OnEnChangeEditMrotateYaw1)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_YAW2, &CManualRotateDlg::OnEnChangeEditMrotateYaw2)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_YAW3, &CManualRotateDlg::OnEnChangeEditMrotateYaw3)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_YAW4, &CManualRotateDlg::OnEnChangeEditMrotateYaw4)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_YAW5, &CManualRotateDlg::OnEnChangeEditMrotateYaw5)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_YAW6, &CManualRotateDlg::OnEnChangeEditMrotateYaw6)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_YAW7, &CManualRotateDlg::OnEnChangeEditMrotateYaw7)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_PITCH0, &CManualRotateDlg::OnEnChangeEditMrotatePitch0)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_PITCH1, &CManualRotateDlg::OnEnChangeEditMrotatePitch1)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_PITCH2, &CManualRotateDlg::OnEnChangeEditMrotatePitch2)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_PITCH3, &CManualRotateDlg::OnEnChangeEditMrotatePitch3)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_PITCH4, &CManualRotateDlg::OnEnChangeEditMrotatePitch4)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_PITCH5, &CManualRotateDlg::OnEnChangeEditMrotatePitch5)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_PITCH6, &CManualRotateDlg::OnEnChangeEditMrotatePitch6)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_PITCH7, &CManualRotateDlg::OnEnChangeEditMrotatePitch7)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_ROLL0, &CManualRotateDlg::OnEnChangeEditMrotateRoll0)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_ROLL1, &CManualRotateDlg::OnEnChangeEditMrotateRoll1)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_ROLL2, &CManualRotateDlg::OnEnChangeEditMrotateRoll2)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_ROLL3, &CManualRotateDlg::OnEnChangeEditMrotateRoll3)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_ROLL4, &CManualRotateDlg::OnEnChangeEditMrotateRoll4)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_ROLL5, &CManualRotateDlg::OnEnChangeEditMrotateRoll5)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_ROLL6, &CManualRotateDlg::OnEnChangeEditMrotateRoll6)
	ON_EN_CHANGE(IDC_EDIT_MROTATE_ROLL7, &CManualRotateDlg::OnEnChangeEditMrotateRoll7)
END_MESSAGE_MAP()


// CManualRotateDlg �޽��� ó�����Դϴ�.
BOOL CManualRotateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	m_btnOK.LoadImages(IDB_BTN_OK_NORMAL, 0, IDB_BTN_OK_PRESSED, IDB_BTN_OK_PRESSED, 0, 0, _T("PNG"));
	m_btnOK.SizeToContent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CManualRotateDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_RETURN)
	{
		OnBnClickedBtnSave();
		return TRUE;
	}

	if (pMsg->wParam == VK_ESCAPE)
	{
		if ((CCenterView*)theApp.m_ScreenManager.m_pCenterView != NULL)
			((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowManualRotateDlg(FALSE);
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualRotateDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow)
	{
		int camNum = theApp.m_PrjAllCfg.m_nMaxCamra;
		for (int i = 0; i < 8; i++)
		{
			CStatic* pGroup = (CStatic*)GetDlgItem(IDC_STATIC_MROTATE_INPUT0 + i);

			CStatic* pStaticYaw = (CStatic*)GetDlgItem(IDC_STATIC_MROTATE_YAW0 + i);
			CStatic* pStaticPitch = (CStatic*)GetDlgItem(IDC_STATIC_MROTATE_PITCH0 + i);
			CStatic* pStaticRoll = (CStatic*)GetDlgItem(IDC_STATIC_MROTATE_ROLL0 + i);

			CEdit* pEditYaw = (CEdit*)GetDlgItem(IDC_EDIT_MROTATE_YAW0 + i);
			CEdit* pEditPitch = (CEdit*)GetDlgItem(IDC_EDIT_MROTATE_PITCH0 + i);
			CEdit* pEditRoll = (CEdit*)GetDlgItem(IDC_EDIT_MROTATE_ROLL0 + i);

			if (i < camNum)
			{
				pGroup->ShowWindow(SW_SHOW);

				pStaticYaw->ShowWindow(SW_SHOW);
				pStaticPitch->ShowWindow(SW_SHOW);
				pStaticRoll->ShowWindow(SW_SHOW);
				
				pEditYaw->ShowWindow(SW_SHOW);
				pEditPitch->ShowWindow(SW_SHOW);
				pEditRoll->ShowWindow(SW_SHOW);

				float yaw, pitch, roll;
				QVS_Engine::QVS_getImageParam(i, yaw, pitch, roll);

				CString szTemp;
				szTemp.Format(_T("%lf"), yaw);
				pEditYaw->SetWindowTextW(szTemp);
				szTemp.Format(_T("%lf"), pitch);
				pEditPitch->SetWindowTextW(szTemp);
				szTemp.Format(_T("%lf"), roll);
				pEditRoll->SetWindowTextW(szTemp);
			}
			else
			{
				pGroup->ShowWindow(SW_HIDE);

				pStaticYaw->ShowWindow(SW_HIDE);
				pStaticPitch->ShowWindow(SW_HIDE);
				pStaticRoll->ShowWindow(SW_HIDE);

				pEditYaw->ShowWindow(SW_HIDE);
				pEditPitch->ShowWindow(SW_HIDE);
				pEditRoll->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CManualRotateDlg::OnBnClickedBtnSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	for (int i = 0; i < theApp.m_PrjAllCfg.m_nMaxCamra; i++)
	{
		CEdit* pEditYaw = (CEdit*)GetDlgItem(IDC_EDIT_MROTATE_YAW0 + i);
		CEdit* pEditPitch = (CEdit*)GetDlgItem(IDC_EDIT_MROTATE_PITCH0 + i);
		CEdit* pEditRoll = (CEdit*)GetDlgItem(IDC_EDIT_MROTATE_ROLL0 + i);

		float yaw, pitch, roll;
		CString szTemp;
		pEditYaw->GetWindowTextW(szTemp);
		yaw = _ttof(szTemp);
		pEditPitch->GetWindowTextW(szTemp);
		pitch = _ttof(szTemp);
		pEditRoll->GetWindowTextW(szTemp);
		roll = _ttof(szTemp);

		QVS_Engine::QVS_setImageParam(i, yaw, pitch, roll);	
	}

	if ((CCenterView*)theApp.m_ScreenManager.m_pCenterView != NULL)
		((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->ShowManualRotateDlg(FALSE);
}

void CManualRotateDlg::OnEnChangeEditMrotateYaw0()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_YAW0;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateYaw1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_YAW1;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateYaw2()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_YAW2;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateYaw3()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_YAW3;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateYaw4()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_YAW4;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateYaw5()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_YAW5;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateYaw6()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_YAW6;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateYaw7()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_YAW7;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotatePitch0()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_PITCH0;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotatePitch1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_PITCH1;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotatePitch2()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_PITCH2;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotatePitch3()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_PITCH3;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotatePitch4()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_PITCH4;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotatePitch5()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_PITCH5;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotatePitch6()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_PITCH6;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotatePitch7()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_PITCH7;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateRoll0()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_ROLL0;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateRoll1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_ROLL1;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateRoll2()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_ROLL2;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateRoll3()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_ROLL3;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateRoll4()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_ROLL4;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateRoll5()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_ROLL5;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateRoll6()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_ROLL6;
	subChangeEdit(IDC);
}


void CManualRotateDlg::OnEnChangeEditMrotateRoll7()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int IDC = IDC_EDIT_MROTATE_ROLL7;
	subChangeEdit(IDC);
}

void CManualRotateDlg::subChangeEdit(int IDC)
{
	double yaw;
	CString editValue;
	GetDlgItemTextW(IDC, editValue);

	int cursor;
	int size = strlen((const char*)(CStringA)editValue);
	bool isChange = false;
	for (int i = 0; i < size; i++)
	{
		if (!((48 <= editValue.GetAt(i) && editValue.GetAt(i) <= 57) || editValue.GetAt(i) == '.' || editValue.GetAt(i) == '-'))
		{
			editValue.Delete(i, 1);
			size--;
			cursor = i;
			i--;
			isChange = true;
		}
	}

	if (isChange)
	{
		SetDlgItemTextW(IDC, editValue);
		((CEdit*)GetDlgItem(IDC))->SetSel(cursor, cursor);
	}

	((CEdit*)GetDlgItem(IDC))->UpdateData();
}

void CManualRotateDlg::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);
	CFont* ariaB12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(BAria12);
	GetDlgItem(IDC_STATIC_MROTATE_INPUT0)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_INPUT1)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_INPUT2)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_INPUT3)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_INPUT4)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_INPUT5)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_INPUT6)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_INPUT7)->SetFont(aria12);

	GetDlgItem(IDC_STATIC_MROTATE_YAW0)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_YAW1)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_YAW2)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_YAW3)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_YAW4)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_YAW5)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_YAW6)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_YAW7)->SetFont(aria12);

	GetDlgItem(IDC_STATIC_MROTATE_PITCH0)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_PITCH1)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_PITCH2)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_PITCH3)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_PITCH4)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_PITCH5)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_PITCH6)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_PITCH7)->SetFont(aria12);

	GetDlgItem(IDC_STATIC_MROTATE_ROLL0)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_ROLL1)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_ROLL2)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_ROLL3)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_ROLL4)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_ROLL5)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_ROLL6)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_MROTATE_ROLL7)->SetFont(aria12);
}