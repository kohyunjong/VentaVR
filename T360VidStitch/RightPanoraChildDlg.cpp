// RightPanoraChildDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "T360VidStitchDlg.h"
#include "RightPanoraChildDlg.h"
#include "afxdialogex.h"
#include "FolderDlg.h"


// RightPanoraChildDlg 대화 상자입니다.
#define SWITCH_VOD_STREAMING 0

IMPLEMENT_DYNAMIC(CRightPanoraChildDlg, CDialogEx)

CRightPanoraChildDlg::CRightPanoraChildDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRightPanoraChildDlg::IDD, pParent)
{
	m_nFlgHddMode = recReady;
	m_nFlgStreamMode = recReady;
	m_nFlgOutputMode = recReady;
	m_nFlgClibration = recReady;
	m_nFlgVODStreamMode = recReady;
	m_bFirstUpdate = FALSE;
}

CRightPanoraChildDlg::~CRightPanoraChildDlg()
{
}

void CRightPanoraChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNCALIBRATION, m_btnCailbration);
	DDX_Control(pDX, IDC_BTNHDD, m_btnHdd);
	DDX_Control(pDX, IDC_BTNRTMP, m_btnStream);
	DDX_Control(pDX, IDC_BTNOUTPUT, m_btnOutput);
	DDX_Control(pDX, IDC_BTNHDD2, m_btnHdd2);
	DDX_Control(pDX, IDC_BTNRTMP2, m_btnStream2);
	DDX_Control(pDX, IDC_BTNOUTPUT2, m_btnOutput2);
	//DDX_Control(pDX, IDC_BTNEDITDISABLE, m_btnEditDisable);
	//DDX_Control(pDX, IDC_BTNEDITOFF, m_btnEditOff);
	//DDX_Control(pDX, IDC_BTNEDITON, m_btnEditOn);
	//DDX_Control(pDX, IDC_BTNSTRAIGHTEN, m_btnStraighten);
	DDX_Control(pDX, IDC_BTNPANOSAVE, m_btnPanoSave);
	DDX_Control(pDX, IDC_BTNPANORAMA, m_btnOverlay);
	DDX_Control(pDX, IDC_BTNVOD, m_btnVOD);
	DDX_Control(pDX, IDC_BTNVOD2, m_btnVOD2);
	//DDX_Control(pDX, IDC_BTNSTEREO, m_btnStereo);
}


BEGIN_MESSAGE_MAP(CRightPanoraChildDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNCALIBRATION, &CRightPanoraChildDlg::OnBnClickedBtncalibration)
	ON_BN_CLICKED(IDC_BTNOUTPUT, &CRightPanoraChildDlg::OnBnClickedBtnoutput)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_BTNEDITON, &CRightPanoraChildDlg::OnBnClickedBtnediton)
	//ON_BN_CLICKED(IDC_BTNEDITOFF, &CRightPanoraChildDlg::OnBnClickedBtneditoff)
	//ON_BN_CLICKED(IDC_BTNSTRAIGHTEN, &CRightPanoraChildDlg::OnBnClickedBtnstraighten)
	ON_BN_CLICKED(IDC_BTNPANOSAVE, &CRightPanoraChildDlg::OnBnClickedBtnpanosave)
	ON_BN_CLICKED(IDC_BTNPANORAMA, &CRightPanoraChildDlg::OnBnClickedBtnpanorama)
	ON_BN_CLICKED(IDC_BTNVOD, &CRightPanoraChildDlg::OnBnClickedBtnvod)
END_MESSAGE_MAP()


// RightPanoraChildDlg 메시지 처리기입니다.

BOOL CRightPanoraChildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRightPanoraChildDlg::OnBnClickedBtncalibration()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//theApp.m_ScreenManager.ShowToast(vdstToastHideAll); // Hide all

	/*if (theApp.m_ScreenManager.GetAcitveScreen() != vdstPanoramaView)
	{
		theApp.m_ScreenManager.m_pLeftChild->OnBnClickedBtnpanorama();
		if (!m_bFirstUpdate)
		{
			theApp.m_ScreenManager.SetseamModeStatus(FALSE);
			theApp.m_ScreenManager.SetGainCompensationStatus(FALSE);
			m_nFlgClibration = recDoing;
			KillTimer(TIMER_FUPDATEPANO_RIGHTCHECK);
			SetTimer(TIMER_FIRSTUPDATEPANO_RIGHT, 100, NULL);

			while (!m_bFirstUpdate)
				((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(30);
		}
	}

	if (m_bFirstUpdate == FALSE)
		return;

	if (m_nFlgClibration != recReady)
		return;

	theApp.m_ScreenManager.SetseamModeStatus(FALSE);

	m_nFlgClibration = recDoing;

	int result = QVS_Engine::QVS_SetQuramCalibMode(0);
	if (result == QVS_SUCCESS)
	{
		KillTimer(TIMER_CHECKCALIBRATION);
		SetTimer(TIMER_CHECKCALIBRATION, 100, NULL);
	}*/
	theApp.m_ScreenManager.ShowScreens(vdstRightCalibView);
}

void CRightPanoraChildDlg::OnBnClickedBtnoutput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//if (theApp.m_PrjAllCfg.m_ModuleType == QVS_Engine::InputModuleType_Blackmagic)
	{
		if (m_nFlgOutputMode == recReady)
		{
			if (QVS_Engine::QVS_StartOutput() == QVS_SUCCESS)
			{
				m_nFlgOutputMode = recDoing;
				theApp.m_ScreenManager.ShowToast(vdstToastOutput, _T("Output Doing…"));

				theApp.m_ScreenManager.setEnableConfiurationBtn(TRUE);
				m_btnOutput2.ShowWindow(SW_SHOW);
				m_btnOutput.ShowWindow(SW_HIDE);

				SetTimer(TIMER_OUTPUTSTARTWAIT, 5000, NULL);
			}
		}
		else if (m_nFlgOutputMode == recDoing)
		{
			if (QVS_Engine::QVS_FinishOutput() == QVS_SUCCESS) // QVS_FinishStreaming API 호출 반환값이 1 일 때 (성공)
			{
				m_btnOutput2.ShowWindow(SW_SHOW);
				m_btnOutput.ShowWindow(SW_HIDE);
				m_nFlgOutputMode = recFinish;

				theApp.m_ScreenManager.ShowToast(vdstToastOutput, _T(""), FALSE);

				SetTimer(TIMER_OUTPUTENDWAIT, 5000, NULL);
			}
		}
	}
}


void CRightPanoraChildDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (theApp.m_ScreenManager.GetAcitveScreen() == vdstPanoramaView && !m_bFirstUpdate)
	{
		theApp.m_ScreenManager.SetseamModeStatus(FALSE);
		theApp.m_ScreenManager.SetGainCompensationStatus(FALSE);
		m_nFlgClibration = recDoing;
		SetTimer(TIMER_FIRSTUPDATEPANO_RIGHT, 100, NULL);
	}
	else
	{
		m_nFlgClibration = recReady;
	}


	if (bShow)
	{
		m_btnCailbration.LoadImages(IDB_EXPORT_BTN_CALIBRATION_NORMAL, 0, IDB_EXPORT_BTN_CALIBRATION_MOVER, IDB_EXPORT_BTN_CALIBRATION_PRESSED, 0, 0, _T("PNG"));
		m_btnCailbration.SizeToContent();

		m_btnHdd.LoadImages(IDB_EXPORT_BTN_HDD_NORMAL, IDB_EXPORT_BTN_HDD_ENCORDING, IDB_EXPORT_BTN_HDD_MOVER, IDB_EXPORT_BTN_HDD_PRESSED, IDB_EXPORT_BTN_HDD_ENCORDING_MOVER, IDB_EXPORT_BTN_HDD_ENCORDING_PRESSED, _T("PNG"));
		m_btnHdd.SizeToContent();

		m_btnOutput.LoadImages(IDB_EXPORT_BTN_OUTPUT_NORMAL, IDB_EXPORT_BTN_OUTPUT_STREAMING, IDB_EXPORT_BTN_OUTPUT_MOVER, IDB_EXPORT_BTN_OUTPUT_PRESSED, IDB_EXPORT_BTN_OUTPUT_STREAMING_MOVER, IDB_EXPORT_BTN_OUTPUT_STREAMING_PRESSED, _T("PNG"));
		m_btnOutput.SizeToContent();

		m_btnStream.LoadImages(IDB_EXPORT_BTN_RTMP_NORMAL, IDB_EXPORT_BTN_RTMP_STREAMING, IDB_EXPORT_BTN_RTMP_MOVER, IDB_EXPORT_BTN_RTMP_PRESSED, IDB_EXPORT_BTN_RTMP_STREAMING_MOVER, IDB_EXPORT_BTN_RTMP_STREAMING_PRESSED, _T("PNG"));
		m_btnStream.SizeToContent();

		m_btnVOD.LoadImages(IDB_EXPORT_BTN_VODSTREAM_NORMAL, IDB_EXPORT_BTN_VODSTREAM_STREAMING, IDB_EXPORT_BTN_VODSTREAM_MOVER, IDB_EXPORT_BTN_VODSTREAM_PRESSED, IDB_EXPORT_BTN_VODSTREAM_STREAMING_MOVER, IDB_EXPORT_BTN_VODSTREAM_STREAMING_PRESSED, _T("PNG"));
		m_btnVOD.SizeToContent();
		m_btnVOD.ShowWindow(SW_HIDE);

		////////////////////////////////////////////////////////////////////////////////////
		m_btnHdd2.LoadImages(IDB_EXPORT_BTN_HDD_DISABLE, IDB_EXPORT_BTN_HDD_DISABLE, IDB_EXPORT_BTN_HDD_DISABLE, IDB_EXPORT_BTN_HDD_DISABLE, 0, 0, _T("PNG"));
		m_btnHdd2.SizeToContent();
		m_btnHdd2.ShowWindow(SW_HIDE);

		m_btnOutput2.LoadImages(IDB_EXPORT_BTN_OUTPUT_DISABLE, IDB_EXPORT_BTN_OUTPUT_DISABLE, IDB_EXPORT_BTN_OUTPUT_DISABLE, IDB_EXPORT_BTN_OUTPUT_DISABLE, 0, 0, _T("PNG"));
		m_btnOutput2.SizeToContent();
		m_btnOutput2.ShowWindow(SW_HIDE);

		m_btnStream2.LoadImages(IDB_EXPORT_BTN_RTMP_DISABLE, 0, 0, 0, 0, 0, _T("PNG"));
		m_btnStream2.SizeToContent();
		m_btnStream2.ShowWindow(SW_HIDE);

		m_btnVOD2.LoadImages(IDB_EXPORT_BTN_VODSTREAM_DISABLE, 0, 0, 0, 0, 0, _T("PNG"));
		m_btnVOD2.SizeToContent();
		m_btnVOD2.ShowWindow(SW_HIDE);

		/*m_btnEditDisable.LoadImages(IDB_EXPORT_BTN_EDITOFF_DISABLE, 0, 0, 0, _T("PNG"));
		m_btnEditDisable.SizeToContent();
		m_btnEditOff.ShowWindow(SW_HIDE);*/

		/*
		m_btnEditOff.LoadImages(IDB_EXPORT_BTN_EDITOFF_NORMAL, 0, IDB_EXPORT_BTN_EDITOFF_MOVER, IDB_EXPORT_BTN_EDITOFF_PRESSED, 0, 0, _T("PNG"));
		m_btnEditOff.SizeToContent();
		//m_btnEditOff.ShowWindow(SW_HIDE);

		m_btnEditOn.LoadImages(IDB_EXPORT_BTN_EDITON_NORMAL, 0, IDB_EXPORT_BTN_EDITON_MOVER, IDB_EXPORT_BTN_EDITON_PRESSED, 0, 0, _T("PNG"));
		m_btnEditOn.SizeToContent();
		m_btnEditOn.ShowWindow(SW_HIDE);

		m_btnStraighten.LoadImages(IDB_EXPORT_BTN_STRAIGHTEN_NORMAL, 0, IDB_EXPORT_BTN_STRAIGHTEN_MOVER, IDB_EXPORT_BTN_STRAIGHTEN_PRESSED, 0, 0, _T("PNG"));
		m_btnStraighten.SizeToContent();
		m_btnStraighten.ShowWindow(SW_HIDE);
		*/

		m_btnPanoSave.LoadImages(IDB_EXPORT_BTN_PANOSAVE_NORMAL, 0, IDB_EXPORT_BTN_PANOSAVE_MOVER, IDB_EXPORT_BTN_PANOSAVE_PRESSED, 0, 0, _T("PNG"));
		m_btnPanoSave.SizeToContent();
		m_btnPanoSave.ShowWindow(SW_SHOW);

		m_btnOverlay.LoadImages(IDB_EXPORT_BTN_OVERLAY_NORMAL, 0, IDB_EXPORT_BTN_OVERLAY_MOVER, IDB_EXPORT_BTN_OVERLAY_PRESSED, 0, 0, _T("PNG"));
		m_btnOverlay.SizeToContent();
		//m_btnOverlay.ShowWindow(SW_HIDE);
		
		if (QVS_Engine::QVS_GetNumInput() == 0)
		{
			m_btnCailbration.Disable(&m_btnCailbration);
			m_btnHdd.Disable(&m_btnHdd);
			m_btnOutput.Disable(&m_btnOutput);
			m_btnStream.Disable(&m_btnStream);
			//m_btnHdd2.Disable(&m_btnHdd2);
			//m_btnOutput2.Disable(&m_btnOutput2);
			//m_btnStream2.Disable(&m_btnStream2);
			//m_btnEditOff.Disable(&m_btnEditOff);
			m_btnPanoSave.Disable(&m_btnPanoSave);
			m_btnOverlay.Disable(&m_btnOverlay);
		}
		else
		{
			m_btnCailbration.Enable(&m_btnCailbration);
			m_btnHdd.Enable(&m_btnHdd);
			m_btnOutput.Enable(&m_btnOutput);
			m_btnStream.Enable(&m_btnStream);
			//m_btnHdd2.Enable(&m_btnHdd2);
			//m_btnOutput2.Enable(&m_btnOutput2);
			//m_btnStream2.Enable(&m_btnStream2);
			//m_btnEditOff.Enable(&m_btnEditOff);
			m_btnPanoSave.Enable(&m_btnPanoSave);
			m_btnOverlay.Enable(&m_btnOverlay);

			if (m_nFlgHddMode == recDoing)
				m_btnHdd.SetImage(CGdipButton::ALT_TYPE);
			else
				m_btnHdd.SetImage(CGdipButton::STD_TYPE);

			if (m_nFlgOutputMode == recDoing)
				m_btnOutput.SetImage(CGdipButton::ALT_TYPE);
			else
				m_btnOutput.SetImage(CGdipButton::STD_TYPE);

			if (m_nFlgStreamMode == recDoing)
				m_btnStream.SetImage(CGdipButton::ALT_TYPE);
			else
				m_btnStream.SetImage(CGdipButton::STD_TYPE);

			if (m_nFlgVODStreamMode == recDoing)
				m_btnVOD.SetImage(CGdipButton::ALT_TYPE);
			else
				m_btnVOD.SetImage(CGdipButton::STD_TYPE);
		}

#if SWITCH_VOD_STREAMING
		m_btnVOD.ShowWindow(SW_SHOW);
#endif
	}
}


BOOL CRightPanoraChildDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CRightPanoraChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TIMER_FIRSTUPDATEPANO_RIGHT:
	{
		if (theApp.m_ScreenManager.GetseamModeDone() && theApp.m_ScreenManager.GetGainCompensationDone())
		{
			KillTimer(TIMER_FIRSTUPDATEPANO_RIGHT);
			SetTimer(TIMER_FUPDATEPANO_RIGHTCHECK, 5000, NULL);
		}
	}
	break;
	case TIMER_FUPDATEPANO_RIGHTCHECK:
	{
		KillTimer(TIMER_FUPDATEPANO_RIGHTCHECK);
		
		if(theApp.m_ScreenManager.GetAcitveScreen() == vdstPanoramaView)
			m_bFirstUpdate = TRUE;

		m_nFlgClibration = recReady;
	}
	break;

	case TIMER_CHECKCALIBRATION:
	{
		CString szDoneText = _T("Calibration Fail.");

		if (theApp.m_ScreenManager.GetnewCalibrationStatus() == 0
			|| (theApp.m_ScreenManager.GetnewCalibrationStatus() == 1 && !theApp.m_ScreenManager.GetseamModeDone()) //Calibration Fail의 경우 seamMode가 변경 되지 않는다.
 			)
			break;

		KillTimer(TIMER_CHECKCALIBRATION);
		
		if (theApp.m_ScreenManager.GetnewCalibrationStatus() == 1)
			szDoneText = _T("Calibration Done.");

		SetTimer(TIMER_CALIBRATIONDONE, 1000, NULL);
	}
	break;

	case TIMER_CALIBRATIONDONE:
	{
		static int nCnt = 0;
		CString szDoneText = _T("Calibration Fail.");

		nCnt++;

		if (theApp.m_ScreenManager.GetnewCalibrationStatus() == 1)
			szDoneText = _T("Calibration Done.");
		else if (theApp.m_ScreenManager.GetnewCalibrationStatus() == -1 && nCnt < 3)
			nCnt = 3;

		if (nCnt % 5 == 3)
		{
			m_nFlgClibration = recFinish;
			theApp.m_ScreenManager.ShowToast(vdstToastDone, szDoneText);
		}
		else if (nCnt % 5 == 0 && m_nFlgClibration == recFinish)
		{
			theApp.m_ScreenManager.ShowToast(vdstToastDone, szDoneText, FALSE);

			theApp.m_ScreenManager.SetseamModeStatus(FALSE);

			theApp.m_ScreenManager.SetnewCalibrationStatus(0);
			m_btnCailbration.EnableButton(TRUE);
			m_btnCailbration.EnableWindow(TRUE);
			nCnt = 0;
			m_nFlgClibration = recReady;
			KillTimer(TIMER_CALIBRATIONDONE);
		}

	}
	break;

	case TIMER_STARTENCODINGWAIT:
	{
		KillTimer(TIMER_STARTENCODINGWAIT);
		m_btnHdd.SetImage(CGdipButton::ALT_TYPE);
		m_btnHdd.ShowWindow(SW_SHOW);
		m_btnHdd.SetFocus();
		m_btnHdd2.ShowWindow(SW_HIDE);
	}
	break;

	case TIMER_STREAMSTARTWAIT:
	{
		KillTimer(TIMER_STREAMSTARTWAIT);
		m_btnStream.SetImage(CGdipButton::ALT_TYPE);
		m_btnStream.ShowWindow(SW_SHOW);
		m_btnStream2.ShowWindow(SW_HIDE);
		m_btnStream.SetFocus();
	}
	break;

	case TIMER_OUTPUTSTARTWAIT:
	{
		KillTimer(TIMER_OUTPUTSTARTWAIT);
		m_btnOutput.SetImage(CGdipButton::ALT_TYPE);
		m_btnOutput.ShowWindow(SW_SHOW);
		m_btnOutput.SetFocus();
		m_btnOutput2.ShowWindow(SW_HIDE);
	}
	break;

	case TIMER_VODSTARTWAIT:
	{
		KillTimer(TIMER_VODSTARTWAIT);
		m_btnVOD.SetImage(CGdipButton::ALT_TYPE);
		m_btnVOD.ShowWindow(SW_SHOW);
		m_btnVOD.SetFocus();
		m_btnVOD2.ShowWindow(SW_HIDE);
	}
		break;

	case TIMER_ENDENCODINGWAIT:
	{
		if (QVS_Engine::QVS_IsEncoding() != QVS_MSG_NO_ENCODING)
			break;

		KillTimer(TIMER_ENDENCODINGWAIT);
		if (m_nFlgHddMode == recFinish)
		{
			m_nFlgHddMode = recReady;
			m_btnHdd.SetImage(CGdipButton::STD_TYPE);
			m_btnHdd.ShowWindow(SW_SHOW);
			m_btnHdd.SetFocus();
			m_btnHdd2.ShowWindow(SW_HIDE);
		}
	}
	break;

	case TIMER_STREAMENDWAIT:
	{
		KillTimer(TIMER_STREAMENDWAIT);

		m_nFlgStreamMode = recReady;
		m_btnStream.SetImage(CGdipButton::STD_TYPE);
		m_btnStream.ShowWindow(SW_SHOW);
		m_btnStream2.ShowWindow(SW_HIDE);
		m_btnStream.SetFocus();
	}
	break;

	case  TIMER_OUTPUTENDWAIT:
	{
		KillTimer(TIMER_OUTPUTENDWAIT);

		m_nFlgOutputMode = recReady;
		m_btnOutput.SetImage(CGdipButton::STD_TYPE);
		m_btnOutput.ShowWindow(SW_SHOW);
		m_btnOutput.SetFocus();
		m_btnOutput2.ShowWindow(SW_HIDE);

	}
	break;

	case  TIMER_VODENDWAIT:
	{
	    KillTimer(TIMER_VODENDWAIT);

		m_nFlgVODStreamMode = recReady;
		m_btnVOD.SetImage(CGdipButton::STD_TYPE);
		m_btnVOD.ShowWindow(SW_SHOW);
		m_btnVOD.SetFocus();
		m_btnVOD2.ShowWindow(SW_HIDE);

	}
		break;
	}

	if (nIDEvent == TIMER_ADDINGOVERLAY)
	{
		KillTimer(TIMER_ADDINGOVERLAY);
		theApp.m_ScreenManager.ShowToast(vdstToastDone, _T(" "), FALSE);
	}

	if (nIDEvent == TIMER_OUTPUTENDWAIT || nIDEvent == TIMER_STREAMENDWAIT || nIDEvent == TIMER_ENDENCODINGWAIT)
	{
		if (m_nFlgHddMode == 0 && m_nFlgStreamMode == 0 && m_nFlgOutputMode == 0 && m_nFlgVODStreamMode == 0)
			theApp.m_ScreenManager.setEnableConfiurationBtn(FALSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CRightPanoraChildDlg::CheckClosetoRecodeing()
{
	if (m_nFlgOutputMode == recDoing)
		QVS_Engine::QVS_FinishOutput();
}

/*
void CRightPanoraChildDlg::OnBnClickedBtnediton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_btnEditOff.ShowWindow(SW_SHOW);
	m_btnEditOn.ShowWindow(SW_HIDE);
	m_btnStraighten.ShowWindow(SW_HIDE);
	m_btnPanoSave.ShowWindow(SW_SHOW);
	theApp.m_ScreenManager.setSubMode(SUBMODE_OFF);
	theApp.m_ScreenManager.updateSubMenu();
	QVS_Engine::QVS_saveOutput();
}


void CRightPanoraChildDlg::OnBnClickedBtneditoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (QVS_Engine::QVS_editOutput())
	{
		m_btnEditOn.ShowWindow(SW_SHOW);
		m_btnEditOff.ShowWindow(SW_HIDE);
		m_btnStraighten.ShowWindow(SW_SHOW);
		m_btnPanoSave.ShowWindow(SW_HIDE);
		theApp.m_ScreenManager.setSubMode(SUBMODE_EDIT_ROTATION);
		theApp.m_ScreenManager.updateSubMenu();
	}
}

void CRightPanoraChildDlg::OnBnClickedBtnstraighten()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	QVS_Engine::QVS_straightenOutput();
}
*/


void CRightPanoraChildDlg::OnBnClickedBtnpanosave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTime cTime = CTime::GetCurrentTime();
	CString time;
	time.Format(_T("%04d-%02d-%02d__%02d-%02d-%02d.png"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	const TCHAR szFilters[] = _T("png image files (*.png)|*.png||");
	CFileDialog dlg(FALSE, _T("png"), time, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters, NULL, 0);

	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);

	char* path;
	CString FilePath;

	if (dlg.DoModal() == IDOK)
	{
		FilePath = dlg.GetFolderPath() + _T("\\") + dlg.GetFileName();
		path = CString2Char(FilePath);

		QVS_Engine::QVS_savePanorama(path);
	}
}


void CRightPanoraChildDlg::OnBnClickedBtnpanorama()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}

void CRightPanoraChildDlg::OnBnClickedBtnvod()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CRightPanoraChildDlg::writeLog(int type)
{
	char* temp = new char[512];
	int size;

	switch (type)
	{
	case WriteOutputLogType::HDD_START:
		size = sizeof("========== HDD Encoding START") * sizeof(char);
		sprintf(temp, "========== HDD Encoding START");
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("output : 3840x2160") * sizeof(char);
		if (QVS_Engine::QVS_GetPanoramaSize())
			sprintf(temp, "output : %dx%d", theApp.m_PrjAllCfg.m_Configuration.hdd_width, theApp.m_PrjAllCfg.m_Configuration.hdd_width / 16 * 9);
		else
			sprintf(temp, "output : %dx%d", theApp.m_PrjAllCfg.m_Configuration.hdd_width, theApp.m_PrjAllCfg.m_Configuration.hdd_height);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("output directory : ") * sizeof(char) + sizeof(theApp.m_PrjAllCfg.m_Configuration.hdd_path) * sizeof(char);
		sprintf(temp, "output directory : %s", theApp.m_PrjAllCfg.m_Configuration.hdd_path);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("bitrate : 100000 Kbits/s") * sizeof(char);
		sprintf(temp, "bitrate : %d Kbits/s", theApp.m_PrjAllCfg.m_Configuration.hdd_bitrate);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("GOP : 120") * sizeof(char);
		sprintf(temp, "GOP : %d", theApp.m_PrjAllCfg.m_Configuration.hdd_gop);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("audio bitrate : 128 Kbits/s") * sizeof(char);
		sprintf(temp, "audio bitrate : %d Kbits/s", theApp.m_PrjAllCfg.m_Configuration.hdd_audio_bitrate == 0? 128:192);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("color space : YUV444") * sizeof(char);
		sprintf(temp, "color space : %s", theApp.m_PrjAllCfg.m_Configuration.hdd_color_space == 0? "YUV444":"NV12");
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("preset : Low Latency High Perfomance") * sizeof(char);
		sprintf(temp, "preset : %s", theApp.m_PrjAllCfg.m_Configuration.hdd_preset == 0? "High Quality" :
									 theApp.m_PrjAllCfg.m_Configuration.hdd_preset == 1? "High Performance" :
									 theApp.m_PrjAllCfg.m_Configuration.hdd_preset == 2? "Low Latency High Performance" : "Low Latency High Quality");
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("FPS : 60") * sizeof(char);
		sprintf(temp, "FPS : %d", theApp.m_PrjAllCfg.m_Configuration.hdd_fps);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("codec : H.264") * sizeof(char);
		sprintf(temp, "codec : %s", theApp.m_PrjAllCfg.m_Configuration.hdd_codec == 0? "H.264" : "HEVC");
		QVS_Engine::QVS_writeLog(temp, size);
		break;
	case WriteOutputLogType::HDD_END:
		size = sizeof("========== HDD Encoding END") * sizeof(char);
		sprintf(temp, "========== HDD Encoding END");
		QVS_Engine::QVS_writeLog(temp, size);
		break;
	case WriteOutputLogType::STREAMING_START:
		size = sizeof("========== Streaming START") * sizeof(char);
		sprintf(temp, "========== Streaming START");
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("output : 3840x2160") * sizeof(char);
		if (QVS_Engine::QVS_GetPanoramaSize())
			sprintf(temp, "output : %dx%d", theApp.m_PrjAllCfg.m_Configuration.rtmp_width, theApp.m_PrjAllCfg.m_Configuration.rtmp_width / 16 * 9);
		else
			sprintf(temp, "output : %dx%d", theApp.m_PrjAllCfg.m_Configuration.rtmp_width, theApp.m_PrjAllCfg.m_Configuration.rtmp_height);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("stream URL : ") * sizeof(char) + sizeof(theApp.m_PrjAllCfg.m_Configuration.rtmp_path) * sizeof(char);
		sprintf(temp, "stream URL : %s", theApp.m_PrjAllCfg.m_Configuration.rtmp_path);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("bitrate : 100000 Kbits/s") * sizeof(char);
		sprintf(temp, "bitrate : %d Kbits/s", theApp.m_PrjAllCfg.m_Configuration.rtmp_bitrate);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("GOP : 120") * sizeof(char);
		sprintf(temp, "GOP : %d", theApp.m_PrjAllCfg.m_Configuration.rtmp_gop);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("audio bitrate : 128 Kbits/s") * sizeof(char);
		sprintf(temp, "audio bitrate : %d Kbits/s", theApp.m_PrjAllCfg.m_Configuration.rtmp_audio_bitrate == 0 ? 128 : 192);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("color space : YUV444") * sizeof(char);
		sprintf(temp, "color space : %s", theApp.m_PrjAllCfg.m_Configuration.rtmp_color_space == 0 ? "YUV444" : "NV12");
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("preset : Low Latency High Perfomance") * sizeof(char);
		sprintf(temp, "preset : %s", theApp.m_PrjAllCfg.m_Configuration.rtmp_preset == 0 ? "High Quality" :
									 theApp.m_PrjAllCfg.m_Configuration.rtmp_preset == 1 ? "High Performance" :
									 theApp.m_PrjAllCfg.m_Configuration.rtmp_preset == 2 ? "Low Latency High Performance" : "Low Latency High Quality");
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("FPS : 60") * sizeof(char);
		sprintf(temp, "FPS : %d", theApp.m_PrjAllCfg.m_Configuration.rtmp_fps);
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("codec : H.264") * sizeof(char);
		sprintf(temp, "codec : %s", theApp.m_PrjAllCfg.m_Configuration.rtmp_codec == 0 ? "H.264" : "HEVC");
		QVS_Engine::QVS_writeLog(temp, size);

		size = sizeof("repeat : Repeat") * sizeof(char);
		sprintf(temp, "repeat : %s", theApp.m_PrjAllCfg.m_Configuration.rtmp_repeat == 0 ? "Once" : "Repeat");
		QVS_Engine::QVS_writeLog(temp, size);
		break;
	case WriteOutputLogType::STREAMING_END:
		size = sizeof("========== Streaming END") * sizeof(char);
		sprintf(temp, "========== Streaming END");
		QVS_Engine::QVS_writeLog(temp, size);
		break;
	}

	delete(temp);
}