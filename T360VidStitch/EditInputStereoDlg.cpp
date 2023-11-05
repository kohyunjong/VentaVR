// EditInputStereoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "EditInputStereoDlg.h"
#include "afxdialogex.h"

#define EDT_SEL_DEVICE 0x01
// CEditInputStereoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditInputStereoDlg, CDialogEx)

CEditInputStereoDlg::CEditInputStereoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditInputStereoDlg::IDD, pParent)
{
}

CEditInputStereoDlg::~CEditInputStereoDlg()
{
}

void CEditInputStereoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnOK);
	DDX_Control(pDX, IDC_BTN_SMALL_RESET0, m_btnRatioReset0);
	DDX_Control(pDX, IDC_BTN_SMALL_RESET1, m_btnRatioReset1);
	DDX_Control(pDX, IDC_BTN_SMALL_RESET2, m_btnRatioReset2);
	DDX_Control(pDX, IDC_BTN_SMALL_RESET3, m_btnRatioReset3);
	DDX_Control(pDX, IDC_BTN_SMALL_RESET4, m_btnRatioReset4);
	DDX_Control(pDX, IDC_BTN_SMALL_RESET5, m_btnRatioReset5);
	DDX_Control(pDX, IDC_BTN_SMALL_RESET6, m_btnRatioReset6);
	DDX_Control(pDX, IDC_BTN_SMALL_RESET7, m_btnRatioReset7);
}


BEGIN_MESSAGE_MAP(CEditInputStereoDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CEditInputStereoDlg::OnBnClickedBtnSave)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_INPUT_ASPECTRATIO_X0, &CEditInputStereoDlg::OnEnChangeEditInputAspectratioX0)
	ON_EN_CHANGE(IDC_EDIT_INPUT_ASPECTRATIO_Y0, &CEditInputStereoDlg::OnEnChangeEditInputAspectratioY0)
	ON_BN_CLICKED(IDC_CHECK_SYSC, &CEditInputStereoDlg::OnBnClickedCheckSysc)
	ON_CBN_SELCHANGE(IDC_CBO_CINPUT1, &CEditInputStereoDlg::OnCbnSelchangeCboCinput1)
	ON_CBN_SELCHANGE(IDC_CBO_CINPUT2, &CEditInputStereoDlg::OnCbnSelchangeCboCinput2)
	ON_CBN_SELCHANGE(IDC_CBO_CINPUT3, &CEditInputStereoDlg::OnCbnSelchangeCboCinput3)
	ON_CBN_SELCHANGE(IDC_CBO_CINPUT4, &CEditInputStereoDlg::OnCbnSelchangeCboCinput4)
	ON_CBN_SELCHANGE(IDC_CBO_CINPUT5, &CEditInputStereoDlg::OnCbnSelchangeCboCinput5)
	ON_CBN_SELCHANGE(IDC_CBO_CINPUT6, &CEditInputStereoDlg::OnCbnSelchangeCboCinput6)
	ON_CBN_SELCHANGE(IDC_CBO_CINPUT7, &CEditInputStereoDlg::OnCbnSelchangeCboCinput7)
	ON_CBN_SELCHANGE(IDC_CBO_CINPUT8, &CEditInputStereoDlg::OnCbnSelchangeCboCinput8)
	ON_BN_CLICKED(IDC_BTN_SMALL_RESET0, &CEditInputStereoDlg::OnBnClickedBtnSmallReset0)
	ON_BN_CLICKED(IDC_BTN_SMALL_RESET1, &CEditInputStereoDlg::OnBnClickedBtnSmallReset1)
	ON_BN_CLICKED(IDC_BTN_SMALL_RESET2, &CEditInputStereoDlg::OnBnClickedBtnSmallReset2)
	ON_BN_CLICKED(IDC_BTN_SMALL_RESET3, &CEditInputStereoDlg::OnBnClickedBtnSmallReset3)
	ON_BN_CLICKED(IDC_BTN_SMALL_RESET4, &CEditInputStereoDlg::OnBnClickedBtnSmallReset4)
	ON_BN_CLICKED(IDC_BTN_SMALL_RESET5, &CEditInputStereoDlg::OnBnClickedBtnSmallReset5)
	ON_BN_CLICKED(IDC_BTN_SMALL_RESET6, &CEditInputStereoDlg::OnBnClickedBtnSmallReset6)
	ON_BN_CLICKED(IDC_BTN_SMALL_RESET7, &CEditInputStereoDlg::OnBnClickedBtnSmallReset7)
END_MESSAGE_MAP()


// CEditInputStereoDlg 메시지 처리기입니다.

BOOL CEditInputStereoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	m_btnOK.LoadImages(IDB_BTN_OK_NORMAL, 0, IDB_BTN_OK_PRESSED, IDB_BTN_OK_PRESSED, 0, 0, _T("PNG"));
	m_btnOK.SizeToContent();

	m_btnRatioReset0.LoadImages(IDB_BTN_EDITINPUT_RESET_NORMAL, 0, IDB_BTN_EDITINPUT_RESET_MOVER, IDB_BTN_EDITINPUT_RESET_PRESSED, 0, 0, _T("PNG"));
	m_btnRatioReset0.SizeToContent();

	m_btnRatioReset1.LoadImages(IDB_BTN_EDITINPUT_RESET_NORMAL, 0, IDB_BTN_EDITINPUT_RESET_MOVER, IDB_BTN_EDITINPUT_RESET_PRESSED, 0, 0, _T("PNG"));
	m_btnRatioReset1.SizeToContent();

	m_btnRatioReset2.LoadImages(IDB_BTN_EDITINPUT_RESET_NORMAL, 0, IDB_BTN_EDITINPUT_RESET_MOVER, IDB_BTN_EDITINPUT_RESET_PRESSED, 0, 0, _T("PNG"));
	m_btnRatioReset2.SizeToContent();

	m_btnRatioReset3.LoadImages(IDB_BTN_EDITINPUT_RESET_NORMAL, 0, IDB_BTN_EDITINPUT_RESET_MOVER, IDB_BTN_EDITINPUT_RESET_PRESSED, 0, 0, _T("PNG"));
	m_btnRatioReset3.SizeToContent();

	m_btnRatioReset4.LoadImages(IDB_BTN_EDITINPUT_RESET_NORMAL, 0, IDB_BTN_EDITINPUT_RESET_MOVER, IDB_BTN_EDITINPUT_RESET_PRESSED, 0, 0, _T("PNG"));
	m_btnRatioReset4.SizeToContent();

	m_btnRatioReset5.LoadImages(IDB_BTN_EDITINPUT_RESET_NORMAL, 0, IDB_BTN_EDITINPUT_RESET_MOVER, IDB_BTN_EDITINPUT_RESET_PRESSED, 0, 0, _T("PNG"));
	m_btnRatioReset5.SizeToContent();

	m_btnRatioReset6.LoadImages(IDB_BTN_EDITINPUT_RESET_NORMAL, 0, IDB_BTN_EDITINPUT_RESET_MOVER, IDB_BTN_EDITINPUT_RESET_PRESSED, 0, 0, _T("PNG"));
	m_btnRatioReset6.SizeToContent();

	m_btnRatioReset7.LoadImages(IDB_BTN_EDITINPUT_RESET_NORMAL, 0, IDB_BTN_EDITINPUT_RESET_MOVER, IDB_BTN_EDITINPUT_RESET_PRESSED, 0, 0, _T("PNG"));
	m_btnRatioReset7.SizeToContent();

	CheckDlgButton(IDC_CHECK_SYSC, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CEditInputStereoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		if (isCFileDlgOpening)
		{
			isCFileDlgOpening = false;
			return TRUE;
		}
		else
			OnClose();

	case VK_RETURN:
		return TRUE;

	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CEditInputStereoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		if (!((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->m_init)
			((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);

		CRect client;
		GetWindowRect(client);

		for (int i = 0; i < 8; i++)
		{
			CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_INPUT0 + i);
			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CBO_CINPUT1 + i);

			CStatic* pStaticVS = (CStatic*)GetDlgItem(IDC_STATIC_INPUT_VS0 + i);
			CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X0 + i);
			CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y0 + i);
			CButton* pBtnReset = (CButton*)GetDlgItem(IDC_BTN_SMALL_RESET0 + i);
			/*CSpinButtonCtrl* pSpinX = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_INPUT_ASPECT_X0 + i);
			CSpinButtonCtrl* pSpinY = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_INPUT_ASPECT_Y0 + i);*/

			if (i < theApp.m_PrjAllCfg.m_nMaxCamra)
			{
				pStatic->ShowWindow(SW_SHOW);
				pCombo->ShowWindow(SW_SHOW);

				pStaticVS->ShowWindow(SW_SHOW);
				pEditX->ShowWindow(SW_SHOW);
				pEditY->ShowWindow(SW_SHOW);
				/*pSpinX->ShowWindow(SW_SHOW);
				pSpinY->ShowWindow(SW_SHOW);*/

				pCombo->ResetContent();

				int max = 0;
				pCombo->AddString(_T("No Device"));
				for (int j = 0; j < QVS_Engine::QVS_getCaptureDeviceSize(); j++) //현재 Stile Image Device는 빼고 진행.
				{
					CString szTemp = CString(QVS_Engine::QVS_getCaptureDeviceName(j));
					pCombo->AddString(szTemp);

					int length = szTemp.GetLength();
					if (max < length)
						max = length;
				}
				pCombo->SetDroppedWidth(max * 5.5);

				//pCombo->AddString(_T("..."));
				int idx = QVS_Engine::QVS_getCaptureDeviceIndex(i);
				if (idx >= 0)
					pCombo->SetCurSel(idx + 1);
				else
					pCombo->SetCurSel(0);

				CString aspectX = _T("-1"), aspectY = _T("-1");
				if (idx >= 0 && idx < QVS_Engine::QVS_getCaptureDeviceSize())
				{
					int ratioX = QVS_Engine::QVS_getAspectX(i);
					int ratioY = QVS_Engine::QVS_getAspectY(i);

					aspectX.Format(_T("%d"), ratioX);
					aspectY.Format(_T("%d"), ratioY);

					if (ratioX < 0 || ratioY < 0)
					{
						pEditX->EnableWindow(FALSE);
						pEditY->EnableWindow(FALSE);

						aspectX = _T("-1");
						aspectY = _T("-1");
					}

					pBtnReset->ShowWindow(SW_SHOW);
				}

				pEditX->SetWindowTextW(aspectX);
				pEditY->SetWindowTextW(aspectY);

				//CheckStilImageSelect(IDC_CBO_CINPUT1 + i);
			}
			else
			{
				pStatic->ShowWindow(SW_HIDE);
				pCombo->ShowWindow(SW_HIDE);

				pBtnReset->ShowWindow(SW_HIDE);
				pStaticVS->ShowWindow(SW_HIDE);
				pEditX->ShowWindow(SW_HIDE);
				pEditY->ShowWindow(SW_HIDE);
				/*pSpinX->ShowWindow(SW_HIDE);
				pSpinY->ShowWindow(SW_HIDE);*/
			}
		}

		isInputCameraSelecting = true;
		((CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild)->CameraSelBtnState(isInputCameraSelecting);

		CButton* pChkBtn = (CButton*)GetDlgItem(IDC_CHECK_SYSC);
		pChkBtn->SetCheck(0);
		updateEditAspectRatio();
		updateCheckBoxState(QVS_Engine::QVS_getCaptureDeviceIndex(0));
	}
}

void CEditInputStereoDlg::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	closeDlg();
	CDialogEx::OnOK();
}


void CEditInputStereoDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	closeDlg();
	CDialogEx::OnClose();
}

void CEditInputStereoDlg::OnEnChangeEditInputAspectratioX0()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IsDlgButtonChecked(IDC_CHECK_SYSC))
	{
		CString aspectX;
		GetDlgItemTextW(IDC_EDIT_INPUT_ASPECTRATIO_X0, aspectX);

		for (int i = 1; i < theApp.m_PrjAllCfg.m_nMaxCamra; i++)
		{
			CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X0 + i);
			pEditX->SetWindowTextW(aspectX);
		}
	}
}


void CEditInputStereoDlg::OnEnChangeEditInputAspectratioY0()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IsDlgButtonChecked(IDC_CHECK_SYSC))
	{
		CString aspectY;
		GetDlgItemTextW(IDC_EDIT_INPUT_ASPECTRATIO_Y0, aspectY);

		for (int i = 1; i < theApp.m_PrjAllCfg.m_nMaxCamra; i++)
		{
			CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y0 + i);
			pEditY->SetWindowTextW(aspectY);
		}
	}
}


void CEditInputStereoDlg::OnBnClickedCheckSysc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	updateEditAspectRatio();
}

void CEditInputStereoDlg::OnCbnSelchangeCboCinput1()
{
	int nIDX = IDC_CBO_CINPUT1;
	
	isNeedLoading = true;
	if (CheckStilImageSelect(nIDX) == EDT_SEL_DEVICE)
	{
		updateCheckBoxState();
		;//추후 처리를 위해 놔두자..
	}
	else
		updateCheckBoxState(-1);
}

void CEditInputStereoDlg::OnCbnSelchangeCboCinput2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIDX = IDC_CBO_CINPUT2;

	isNeedLoading = true;
	if (CheckStilImageSelect(nIDX) == EDT_SEL_DEVICE)
	{
		;
	}
}

void CEditInputStereoDlg::OnCbnSelchangeCboCinput3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIDX = IDC_CBO_CINPUT3;

	isNeedLoading = true;
	if (CheckStilImageSelect(nIDX) == EDT_SEL_DEVICE)
	{
		;
	}
}

void CEditInputStereoDlg::OnCbnSelchangeCboCinput4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIDX = IDC_CBO_CINPUT4;

	isNeedLoading = true;
	if (CheckStilImageSelect(nIDX) == EDT_SEL_DEVICE)
	{
		;
	}
}

void CEditInputStereoDlg::OnCbnSelchangeCboCinput5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIDX = IDC_CBO_CINPUT5;

	isNeedLoading = true;
	if (CheckStilImageSelect(nIDX) == EDT_SEL_DEVICE)
	{
		;
	}
}

void CEditInputStereoDlg::OnCbnSelchangeCboCinput6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIDX = IDC_CBO_CINPUT6;

	isNeedLoading = true;
	if (CheckStilImageSelect(nIDX) == EDT_SEL_DEVICE)
	{
		;
	}
}

void CEditInputStereoDlg::OnCbnSelchangeCboCinput7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIDX = IDC_CBO_CINPUT7;

	isNeedLoading = true;
	if (CheckStilImageSelect(nIDX) == EDT_SEL_DEVICE)
	{
		;
	}
}

void CEditInputStereoDlg::OnCbnSelchangeCboCinput8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIDX = IDC_CBO_CINPUT8;

	isNeedLoading = true;
	if (CheckStilImageSelect(nIDX) == EDT_SEL_DEVICE)
	{
		;
	}
}

int CEditInputStereoDlg::CheckStilImageSelect(int nIDX)
{
	CString szText;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nIDX);
	pCombo->GetLBText(pCombo->GetCurSel(), szText);

	CButton* pBtnReset = (CButton*)GetDlgItem(nIDX + (IDC_BTN_SMALL_RESET0 - IDC_CBO_CINPUT1));
	CEdit* pEditX = (CEdit*)GetDlgItem(nIDX + (IDC_EDIT_INPUT_ASPECTRATIO_X0 - IDC_CBO_CINPUT1));
	CEdit* pEditY = (CEdit*)GetDlgItem(nIDX + (IDC_EDIT_INPUT_ASPECTRATIO_Y0 - IDC_CBO_CINPUT1));

	if (szText.CompareNoCase(_T("Snapshot")) == 0 && isNeedLoading)
	{
		pBtnReset->ShowWindow(SW_HIDE);
		pEditX->EnableWindow(FALSE);
		pEditY->EnableWindow(FALSE);
		pEditX->SetWindowTextW(_T("-1"));
		pEditY->SetWindowTextW(_T("-1"));

		const TCHAR szFilters[] = _T("JPG(*.jpg)|*.jpg||");

		CFileDialog dlg(TRUE, _T("jpg"), _T(""), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, NULL, 0);
		isCFileDlgOpening = true;

		//현재 Path저장
		WCHAR Path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, Path);

		if (dlg.DoModal() == IDOK)
		{
			CString szFullpath, szFileName, szTemp;
			char temp[256];
			szFullpath = dlg.GetPathName();
			szFileName = dlg.GetFileName();

			//Path변경.
			//			szFullpath.Replace(szFileName, _T(""));
			//			SetCurrentDirectory(szFullpath);

			char* szpFname = CString2Char(szFullpath);
			QVS_Engine::QVS_setInputImagePath(nIDX - IDC_CBO_CINPUT1, szpFname);
			QVS_Engine::QVS_setCaptureDeviceIndex(nIDX - IDC_CBO_CINPUT1, QVS_Engine::QVS_getCaptureDeviceSize() - 1);
			free(szpFname);
			//pCombo->SetCurSel(pCombo->GetCurSel() - 1);

			//Save Setting
			theApp.m_PrjAllCfg.m_nNeedSave = 1;
			//Path원복
			SetCurrentDirectory(Path);

			return -1;
		}
		else
		{
			int nNewIdx = QVS_Engine::QVS_getCaptureDeviceIndex(nIDX - IDC_CBO_CINPUT1);
			if (nNewIdx == -1)
				pCombo->SetCurSel(0);
		}
		return -2;
	}
	else if (szText.CompareNoCase(_T("Snapshot")) == 0)
	{
		pBtnReset->ShowWindow(SW_HIDE);
		pEditX->EnableWindow(FALSE);
		pEditY->EnableWindow(FALSE);
		pEditX->SetWindowTextW(_T("-1"));
		pEditY->SetWindowTextW(_T("-1"));

		QVS_Engine::QVS_setCaptureDeviceIndex(nIDX - IDC_CBO_CINPUT1, QVS_Engine::QVS_getCaptureDeviceSize() - 1);
		int nNewIdx = QVS_Engine::QVS_getCaptureDeviceIndex(nIDX - IDC_CBO_CINPUT1);
		if (nNewIdx == -1)
			pCombo->SetCurSel(0);
		return -3;
	}
	else if (szText.CompareNoCase(_T("No Device")) == 0)
	{
		pBtnReset->ShowWindow(SW_HIDE);
		pEditX->EnableWindow(FALSE);
		pEditY->EnableWindow(FALSE);
		pEditX->SetWindowTextW(_T("-1"));
		pEditY->SetWindowTextW(_T("-1"));

		int nNewIdx = QVS_Engine::QVS_getCaptureDeviceIndex(nIDX - IDC_CBO_CINPUT1);
		if (nNewIdx != -1)
			QVS_Engine::QVS_destroyVideoCapture(nIDX - IDC_CBO_CINPUT1);
		return -4;
	}
	else // 기본 디바이스 변경.
	{
		int nOldIdx = QVS_Engine::QVS_getCaptureDeviceIndex(nIDX - IDC_CBO_CINPUT1);

		QVS_Engine::QVS_setCaptureDeviceIndex(nIDX - IDC_CBO_CINPUT1, pCombo->GetCurSel() - 1);
		pBtnReset->ShowWindow(SW_SHOW);
		((CGdipButton*)pBtnReset)->SetImage(CGdipButton::STD_TYPE);
		for (int i = 0; i < 8; i++)
		{
			if (nIDX - IDC_CBO_CINPUT1 != i)
			{
				CComboBox* pComboOther = (CComboBox*)GetDlgItem(IDC_CBO_CINPUT1 + i);
				if (pComboOther->GetCurSel() == pCombo->GetCurSel())
				{
					CEdit* pOldEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X0 + i);
					CEdit* pOldEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y0 + i);

					if (nOldIdx == -1)
					{
						if (i == 0)
							updateCheckBoxState(-1);

						QVS_Engine::QVS_destroyVideoCapture(i);

						pOldEditX->EnableWindow(FALSE);
						pOldEditY->EnableWindow(FALSE);
						pOldEditX->SetWindowTextW(_T("-1"));
						pOldEditY->SetWindowTextW(_T("-1"));

						CButton* pOldBTNReset = (CButton*)GetDlgItem(IDC_BTN_SMALL_RESET0 + i);
						pOldBTNReset->ShowWindow(SW_HIDE);
					}
					else
					{
						QVS_Engine::QVS_setCaptureDeviceIndex(i, nOldIdx);
						//Save Setting
						theApp.m_PrjAllCfg.m_nNeedSave = 1;

						if (nOldIdx == QVS_Engine::QVS_getCaptureDeviceSize() - 1)
						{
							if (i == 0)
								updateCheckBoxState(-1);

							pOldEditX->EnableWindow(FALSE);
							pOldEditY->EnableWindow(FALSE);
							pOldEditX->SetWindowTextW(_T("-1"));
							pOldEditY->SetWindowTextW(_T("-1"));
						}
						else
						{
							int ratioX = QVS_Engine::QVS_getAspectX(i);
							int ratioY = QVS_Engine::QVS_getAspectY(i);

							CString oldAspectX, oldAspectY;
							oldAspectX.Format(_T("%d"), ratioX);
							oldAspectY.Format(_T("%d"), ratioY);

							if (ratioX < 0 || ratioY < 0)
							{
								pEditX->EnableWindow(FALSE);
								pEditY->EnableWindow(FALSE);

								oldAspectX = _T("-1");
								oldAspectY = _T("-1");
							}

							pOldEditX->SetWindowTextW(oldAspectX);
							pOldEditY->SetWindowTextW(oldAspectY);
						}
					}
					pComboOther->SetCurSel(nOldIdx + 1);
				}
			}
		}
		theApp.m_ScreenManager.m_pBottomSourceChild->UpdateDialogItemValue();

		int nAspectX = QVS_Engine::QVS_getAspectX(nIDX - IDC_CBO_CINPUT1);
		int nAspectY = QVS_Engine::QVS_getAspectY(nIDX - IDC_CBO_CINPUT1);
		if (nAspectX >= 0 || nAspectY >= 0)
		{
			pEditX->EnableWindow(TRUE);
			pEditY->EnableWindow(TRUE);

			CString aspectX, aspectY;
			aspectX.Format(_T("%d"), nAspectX);
			aspectY.Format(_T("%d"), nAspectY);

			pEditX->SetWindowTextW(aspectX);
			pEditY->SetWindowTextW(aspectY);
		}

		if (nAspectX < 0 || nAspectY < 0)
		{
			pEditX->EnableWindow(FALSE);
			pEditY->EnableWindow(FALSE);
			pEditX->SetWindowTextW(_T("-1"));
			pEditY->SetWindowTextW(_T("-1"));
		}
	}

	isNeedLoading = false;
	return EDT_SEL_DEVICE;
}

void CEditInputStereoDlg::updateCheckBoxState(int state)
{
	CButton* pChkBtn = (CButton*)GetDlgItem(IDC_CHECK_SYSC);

	if (state < 0 || state == QVS_Engine::QVS_getCaptureDeviceSize() - 1)
	{
		pChkBtn->EnableWindow(FALSE);
		pChkBtn->SetCheck(0);
	}
	else
	{
		pChkBtn->EnableWindow(TRUE);
	}
}

void CEditInputStereoDlg::updateEditAspectRatio()
{
	if (IsDlgButtonChecked(IDC_CHECK_SYSC))
	{
		CString aspectX, aspectY;
		GetDlgItemTextW(IDC_EDIT_INPUT_ASPECTRATIO_X0, aspectX);
		GetDlgItemTextW(IDC_EDIT_INPUT_ASPECTRATIO_Y0, aspectY);

		for (int i = 1; i < theApp.m_PrjAllCfg.m_nMaxCamra; i++)
		{
			int idx = QVS_Engine::QVS_getCaptureDeviceIndex(i);
			if (idx < 0 || idx == QVS_Engine::QVS_getCaptureDeviceSize())
				break; 

			CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X0 + i);
			CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y0 + i);

			pEditX->EnableWindow(FALSE);
			pEditY->EnableWindow(FALSE);

			pEditX->SetWindowTextW(aspectX);
			pEditY->SetWindowTextW(aspectY);
		}
	}
	else
	{
		for (int i = 0; i < theApp.m_PrjAllCfg.m_nMaxCamra; i++)
		{
			//CheckStilImageSelect(IDC_CBO_CINPUT1 + i);
#if(0)
			int idx = QVS_Engine::QVS_getCaptureDeviceIndex(i);
			if (idx < 0 || idx == QVS_Engine::QVS_getCaptureDeviceSize())
				break;

			CButton* pBtnReset = (CButton*)GetDlgItem(IDC_BTN_SMALL_RESET0 + i);
			CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X0 + i);
			CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y0 + i);

			{
				pBtnReset->ShowWindow(SW_SHOW);
				pEditX->EnableWindow(TRUE);
				pEditY->EnableWindow(TRUE);

				CString szTemp;
				szTemp.Format(_T("%d"), QVS_Engine::QVS_getAspectX(i));
				pEditX->SetWindowTextW(szTemp);
				szTemp.Format(_T("%d"), QVS_Engine::QVS_getAspectY(i));
				pEditY->SetWindowTextW(szTemp);
			}
#else
			CButton* pBtnReset = (CButton*)GetDlgItem(IDC_BTN_SMALL_RESET0 + i);
			CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X0 + i);
			CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y0 + i);

			int idx = QVS_Engine::QVS_getCaptureDeviceIndex(i);
			if (idx >= 0 && idx < QVS_Engine::QVS_getCaptureDeviceSize() - 1)
			{
				pBtnReset->ShowWindow(SW_SHOW);
				pEditX->EnableWindow(TRUE);
				pEditY->EnableWindow(TRUE);

				int ratioX = QVS_Engine::QVS_getAspectX(i);
				int ratioY = QVS_Engine::QVS_getAspectY(i);

				CString aspectX, aspectY;
				aspectX.Format(_T("%d"), ratioX);
				aspectY.Format(_T("%d"), ratioY);

				if (ratioX < 0 || ratioY < 0)
				{
					pEditX->EnableWindow(FALSE);
					pEditY->EnableWindow(FALSE);

					aspectX = _T("-1");
					aspectY = _T("-1");
				}

				pEditX->SetWindowTextW(aspectX);
				pEditY->SetWindowTextW(aspectY);
			}
			else
			{
				pBtnReset->ShowWindow(SW_HIDE);
				pEditX->EnableWindow(FALSE);
				pEditY->EnableWindow(FALSE);
				pEditX->SetWindowTextW(_T("-1"));
				pEditY->SetWindowTextW(_T("-1"));
			}
#endif
		}
	}
}

void CEditInputStereoDlg::closeDlg()
{
	((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->PostMessage(WM_CLOSEEDITINPUTSTEREO, 0, 0);

	isInputCameraSelecting = false;
	((CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild)->CameraSelBtnState(isInputCameraSelecting);
	((CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild)->isCameraSel = false;

	//apply aspect ratio
	CString aspectX, aspectY;
	for (int i = 0; i < theApp.m_PrjAllCfg.m_nMaxCamra; i++)
	{
		CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X0 + i);
		CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y0 + i);

		//if (pEditX->IsWindowEnabled());
		int nAspectX = -1, nAspectY = -1;
		GetDlgItemTextW(IDC_EDIT_INPUT_ASPECTRATIO_X0 + i, aspectX);
		GetDlgItemTextW(IDC_EDIT_INPUT_ASPECTRATIO_Y0 + i, aspectY);
		nAspectX = _ttoi(aspectX);
		nAspectY = _ttoi(aspectY);

		if (nAspectX >= 0)
			QVS_Engine::QVS_setAspectX(i, nAspectX);
		if (nAspectY >= 0)
			QVS_Engine::QVS_setAspectY(i, nAspectY);
	}
}

void CEditInputStereoDlg::OnBnClickedBtnSmallReset0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*IDC_BTN_SMALL_RESET0*/
	CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X0);
	CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y0);

	pEditX->SetWindowTextW(_T("16"));
	pEditY->SetWindowTextW(_T("9"));
	QVS_Engine::QVS_setAspectX(0, 16);
	QVS_Engine::QVS_setAspectY(0, 9);
}


void CEditInputStereoDlg::OnBnClickedBtnSmallReset1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X1);
	CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y1);

	pEditX->SetWindowTextW(_T("16"));
	pEditY->SetWindowTextW(_T("9"));
	QVS_Engine::QVS_setAspectX(1, 16);
	QVS_Engine::QVS_setAspectY(1, 9);
}


void CEditInputStereoDlg::OnBnClickedBtnSmallReset2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X2);
	CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y2);

	pEditX->SetWindowTextW(_T("16"));
	pEditY->SetWindowTextW(_T("9"));
	QVS_Engine::QVS_setAspectX(2, 16);
	QVS_Engine::QVS_setAspectY(2, 9);
}


void CEditInputStereoDlg::OnBnClickedBtnSmallReset3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X3);
	CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y3);

	pEditX->SetWindowTextW(_T("16"));
	pEditY->SetWindowTextW(_T("9"));
	QVS_Engine::QVS_setAspectX(3, 16);
	QVS_Engine::QVS_setAspectY(3, 9);
}


void CEditInputStereoDlg::OnBnClickedBtnSmallReset4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X4);
	CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y4);

	pEditX->SetWindowTextW(_T("16"));
	pEditY->SetWindowTextW(_T("9"));
	QVS_Engine::QVS_setAspectX(4, 16);
	QVS_Engine::QVS_setAspectY(4, 9);
}


void CEditInputStereoDlg::OnBnClickedBtnSmallReset5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X5);
	CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y5);

	pEditX->SetWindowTextW(_T("16"));
	pEditY->SetWindowTextW(_T("9"));
	QVS_Engine::QVS_setAspectX(5, 16);
	QVS_Engine::QVS_setAspectY(5, 9);
}


void CEditInputStereoDlg::OnBnClickedBtnSmallReset6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X6);
	CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y6);

	pEditX->SetWindowTextW(_T("16"));
	pEditY->SetWindowTextW(_T("9"));
	QVS_Engine::QVS_setAspectX(6, 16);
	QVS_Engine::QVS_setAspectY(6, 9);
}


void CEditInputStereoDlg::OnBnClickedBtnSmallReset7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit* pEditX = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_X7);
	CEdit* pEditY = (CEdit*)GetDlgItem(IDC_EDIT_INPUT_ASPECTRATIO_Y7);

	pEditX->SetWindowTextW(_T("16"));
	pEditY->SetWindowTextW(_T("9"));
	QVS_Engine::QVS_setAspectX(7, 16);
	QVS_Engine::QVS_setAspectY(7, 9);
}
