// RightSourceChildDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "RightSourceChildDlg.h"
#include "afxdialogex.h"


// CRightSourceChildDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRightSourceChildDlg, CDialogEx)

CRightSourceChildDlg::CRightSourceChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRightSourceChildDlg::IDD, pParent)
{

}

CRightSourceChildDlg::~CRightSourceChildDlg()
{
}

void CRightSourceChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BTNCALIBRATION2, m_btnCalibration);
	DDX_Control(pDX, IDC_BTNSNAPSHOT, m_btnSnapshot); 
}


BEGIN_MESSAGE_MAP(CRightSourceChildDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNSNAPSHOT, &CRightSourceChildDlg::OnBnClickedBtnsnapshot)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_BTNCALIBRATION2, &CRightSourceChildDlg::OnBnClickedBtncalibration2)
END_MESSAGE_MAP()


// CRightSourceChildDlg �޽��� ó�����Դϴ�.


void CRightSourceChildDlg::OnBnClickedBtnsnapshot()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
	{
		//���� ī�޶� ������ ���Ϸ� ����.
		m_btnSnapshot.SetImage(CGdipButton::ALT_TYPE);
		char temp[256] = { 0, };
		int nDeviceCnt = 0;

		for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
		{
			int nDevicIdx = QVS_Engine::QVS_getCaptureDeviceIndex(i);
			sprintf(temp, "%s", QVS_Engine::QVS_getCaptureDeviceName(i));
#ifdef _DEBUG
			TRACE(_T("SnapShot####  [%d]%s[Device idx: %d]\n"), i, CString(temp), nDevicIdx);
#endif
			if (nDevicIdx != -1)
				nDeviceCnt++;			//���� Device�� 
		}

		theApp.m_ScreenManager.SetSnapShotDoneCnt(nDeviceCnt);

		QVS_Engine::QVS_SnapShot();
		while (!theApp.m_ScreenManager.GetSnapShotDone()) 
			((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);

		theApp.m_ScreenManager.ShowToast(vdstToastDone, _T("Done"));

		SetTimer(TIMER_SNAPSHOTDONE, 2000, NULL);
	}
}


BOOL CRightSourceChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CRightSourceChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow) {
		//m_btnCalibration.LoadImages(IDB_EXPORT_BTN_CALIBRATION_NORMAL, 0, IDB_EXPORT_BTN_CALIBRATION_MOVER, IDB_EXPORT_BTN_CALIBRATION_PRESSED, _T("PNG"));
		//m_btnCalibration.SizeToContent();
		m_btnSnapshot.LoadImages(IDB_EXPORT_BTN_SNAPSHOT_NORMAL, IDB_EXPORT_BTN_SNAPSHOT_SELECTED, IDB_EXPORT_BTN_SNAPSHOT_MOVER, IDB_EXPORT_BTN_SNAPSHOT_PRESSED, IDB_EXPORT_BTN_SNAPSHOT_SELECTED, IDB_EXPORT_BTN_SNAPSHOT_SELECTED, _T("PNG"));
	}

}


BOOL CRightSourceChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CRightSourceChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == TIMER_SNAPSHOTDONE)
	{
		KillTimer(TIMER_SNAPSHOTDONE);
		m_btnSnapshot.SetImage(CGdipButton::STD_TYPE);
		theApp.m_ScreenManager.ShowToast(vdstToastDone, _T("Done"), FALSE);
		theApp.m_ScreenManager.SetSnapShotStatus(FALSE);
		m_btnSnapshot.EnableButton(TRUE);

		char snapshotPath[MAX_PATH*2];
		sprintf(snapshotPath, "Snapshot success path : %s", QVS_Engine::QVS_GetSnapshot_path());
		QVS_Engine::QVS_writeLog(snapshotPath, MAX_PATH*2);
	}

	CDialogEx::OnTimer(nIDEvent);
}

//void CRightSourceChildDlg::OnBnClickedBtncalibration2()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	//ToggleButton(vdstPanoramaView);
//	theApp.m_ScreenManager.ShowScreens(vdstRightCalibView);
//}
