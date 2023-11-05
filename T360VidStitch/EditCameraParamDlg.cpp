// EditCameraParamDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "EditCameraParamDlg.h"
#include "afxdialogex.h"


// EditCameraParamDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(EditCameraParamDlg, CDialogEx)

EditCameraParamDlg::EditCameraParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(EditCameraParamDlg::IDD, pParent)
{

}

EditCameraParamDlg::~EditCameraParamDlg()
{
}

void EditCameraParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LENSTYPE, m_cbpLensType);
	DDX_Control(pDX, IDC_BTNCIP_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_EDT_CIP_FPS, m_editFPS);
	DDX_Control(pDX, IDC_EDT_CIP_FOV, m_editFOV);
}


BEGIN_MESSAGE_MAP(EditCameraParamDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDT_CIP_FOV, &EditCameraParamDlg::OnEnChangeEdtCipFov)
	ON_EN_CHANGE(IDC_EDT_CIP_FPS, &EditCameraParamDlg::OnEnChangeEdtCipFps)
	ON_BN_CLICKED(IDC_BTNCIP_SAVE, &EditCameraParamDlg::OnBnClickedBtncipSave)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// EditCameraParamDlg 메시지 처리기입니다.
void EditCameraParamDlg::UpdateDialogItemValue()
{
	printf("bigheadk, UpdateDialogItemValue\n");
	//디바이스가 Init될때까지 기다린다.
	if (!((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->m_init)
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);

	if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
	{
		CString szTemp;
		szTemp.Format(_T("%.1f"), QVS_Engine::QVS_GetCapureFPS());
		GetDlgItem(IDC_EDT_CIP_FPS)->SetWindowText(szTemp);

		szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_nWidth);
		GetDlgItem(IDC_EDT_CIP_WIDTH)->SetWindowText(szTemp);

		szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_nHeight);
		GetDlgItem(IDC_EDT_CIP_HEIGHT)->SetWindowText(szTemp);

		theApp.m_PrjAllCfg.m_nFov = QVS_Engine::QVS_getFOV();
		szTemp.Format(_T("%.1f"), theApp.m_PrjAllCfg.m_nFov);
		GetDlgItem(IDC_EDT_CIP_FOV)->SetWindowText(szTemp);

		//Lens Type Setting.
		int nsize = QVS_Engine::QVS_getLensTypeCount();
		if (nsize > 1)
		{
		m_cbpLensType.ResetContent();
		for (int j = 0; j < nsize; j++) {
		m_cbpLensType.AddString(CString(QVS_Engine::QVS_getLensTypeName(j)));
		}
		int idx = QVS_Engine::QVS_getLensType();
		m_cbpLensType.SetCurSel(idx);
		m_cbpLensType.EnableWindow(TRUE);
		}
		else
		m_cbpLensType.SetCurSel(0);
	}

	theApp.m_PrjAllCfg.m_bFirstUpdateBtmSource = TRUE;
}

void EditCameraParamDlg::OnEnChangeEdtCipFov()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	double fov;
	CString editValue;
	isErrorOfFOV = false;

	GetDlgItemText(IDC_EDT_CIP_FOV, editValue);
	fov = OnChangeStrToDouble(editValue, IDC_EDT_CIP_FOV);		//나중에 리턴값을 FPS 맴버변수에 넣을것

	fov = OnCheckRangeOfFOV(m_cbpLensType.GetCurSel(), fov);

	editValue.Format(_T("%.1f"), fov);
	FOV = editValue;

	UpdateData();

}

double EditCameraParamDlg::OnCheckRangeOfFOV(int CulSel, double fov)
{
	switch (CulSel)
	{
	case 0:							//Rectlinear
	case 1:							//Cylindrical Panorama
	case 3:							//FullFrame Fisheye
		if (fov < 60.0){
			isErrorOfFOV = true;
			fov = 60.0;
		}
		else if (fov > 180.0 ){
			isErrorOfFOV = true;
			fov = 180.0;
		}
		fovErrorMsg = _T("FOV : 60.0 ~ 180.0)\n");
		break;

	case 2:							//Circular Fisheye
		if (fov < 120.0){
			isErrorOfFOV = true;
			fov = 120.0;
		}
		else if (fov > 260.0){
			isErrorOfFOV = true;
			fov = 260.0;
		}
		fovErrorMsg = _T("FOV : 120.0 ~ 260.0)\n");
		break;
	}

	return fov;
}

void EditCameraParamDlg::OnEnChangeEdtCipFps()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	double fps;
	CString editValue;
	isErrorOfFPS = false;

	GetDlgItemText(IDC_EDT_CIP_FPS, editValue);
	fps = OnChangeStrToDouble(editValue, IDC_EDT_CIP_FPS);		//나중에 리턴값을 FPS 맴버변수에 넣을것

	if (fps > 60.00){
		isErrorOfFPS = true;
		fps = 60.00;
	}
	else if (fps < 12.00){
		isErrorOfFPS = true;
		fps = 12.00;
	}

	editValue.Format(_T("%.1f"), fps);
	FPS = editValue;

	UpdateData();
	
}

double EditCameraParamDlg::OnChangeStrToDouble(CString strValue, int id)
{
	for (int i = 0; i < strValue.GetLength(); i++)
	{
		if (!((48 <= strValue.GetAt(i) && strValue.GetAt(i) <= 57) || '.' == strValue.GetAt(i)))	//숫자 ,  '.' 인지 확인
		{
			//  0~ 9 , '.'  이외 다른문자 입력
			if (strValue.GetLength() > 10)	// 문자가 한번에 많이 들어오면 초기화 
				strValue = _T("");
			else
				strValue.Delete(i, 1);

			SetDlgItemText(id, strValue);
			//m_editFOV.SetSel(-1, -1);
		}
	}
	return _wtof(strValue);
}


void EditCameraParamDlg::OnBnClickedBtncipSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnEnChangeEdtCipFov();

	if (isErrorOfFOV || isErrorOfFPS)
	{
		AfxMessageBox(_T("Please check value of the FOV and FPS : \n") + fpsErrorMsg + fovErrorMsg);
		SetDlgItemText(IDC_EDT_CIP_FPS, FPS);
		SetDlgItemText(IDC_EDT_CIP_FOV, FOV);
	}
	else
	{
		CString szTemp, szTemp_fov;
		GetDlgItem(IDC_EDT_CIP_FPS)->GetWindowText(szTemp);
		GetDlgItem(IDC_EDT_CIP_FOV)->GetWindowText(szTemp_fov);
		//QVS_Engine::QVS_SetInputConfiguration(_ttof(szTemp), theApp.m_PrjAllCfg.m_nWidth, theApp.m_PrjAllCfg.m_nHeight);
		QVS_Engine::QVS_SetInputConfiguration(_ttof(szTemp), theApp.m_PrjAllCfg.m_nWidth, theApp.m_PrjAllCfg.m_nHeight, m_cbpLensType.GetCurSel(), _ttof(szTemp_fov));

		((CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild)->UpdateDialogItemValue();
		OnClose();
	}
}


void EditCameraParamDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	this->DestroyWindow();

	((CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild)->m_btnEdit.SetImage(CGdipButton::STD_TYPE);
	//delete ((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->m_pdlgEditCameraParam;
	((CCenterView*)theApp.m_ScreenManager.m_pCenterView)->m_pdlgEditCameraParam = NULL;

	((CBottomSourceChildDlg*)theApp.m_ScreenManager.m_pBottomSourceChild)->isCParamEdit = false;
	CDialogEx::OnClose();
}


void EditCameraParamDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		m_btnSave.LoadImages(IDB_BTN_SAVE_NORMAL, 0, IDB_BTN_SAVE_PRESSED, IDB_BTN_SAVE_PRESSED, 0, 0, _T("PNG"));
		m_btnSave.SizeToContent();

		UpdateDialogItemValue();
	}
}

BOOL EditCameraParamDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		OnClose();
	case VK_RETURN:
		return TRUE;

	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL EditCameraParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	this->SetIcon(hIcon, FALSE);

	m_editFPS.SetLimitText(5);
	m_editFOV.SetLimitText(5);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
