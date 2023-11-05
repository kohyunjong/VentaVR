// FormView1.cpp : implementation file
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "T360VidStitchDlg.h"
#include "CenterView.h"
#include "TQClock.h"



const char* MSG_SUCCESS = "Quram Video360 Engine Linked Successfully!!";
const char* MSG_FAIL = "Quram Video360 Engine Link Fail!!";

unsigned char threeto8[8] =
{
	0, 0111 >> 1, 0222 >> 1, 0333 >> 1, 0444 >> 1, 0555 >> 1, 0666 >> 1, 0377
};

unsigned char twoto8[4] =
{
	0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] =
{
	0, 255
};

static int defaultOverride[13] =
{
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] =
{
	{ 0, 0, 0, 0 },
	{ 0x80, 0, 0, 0 },
	{ 0, 0x80, 0, 0 },
	{ 0x80, 0x80, 0, 0 },
	{ 0, 0, 0x80, 0 },
	{ 0x80, 0, 0x80, 0 },
	{ 0, 0x80, 0x80, 0 },
	{ 0xC0, 0xC0, 0xC0, 0 },

	{ 192, 220, 192, 0 },
	{ 166, 202, 240, 0 },
	{ 255, 251, 240, 0 },
	{ 160, 160, 164, 0 },

	{ 0x80, 0x80, 0x80, 0 },
	{ 0xFF, 0, 0, 0 },
	{ 0, 0xFF, 0, 0 },
	{ 0xFF, 0xFF, 0, 0 },
	{ 0, 0, 0xFF, 0 },
	{ 0xFF, 0, 0xFF, 0 },
	{ 0, 0xFF, 0xFF, 0 },
	{ 0xFF, 0xFF, 0xFF, 0 }
};
// CCenterView

IMPLEMENT_DYNCREATE(CCenterView, CFormView)

CCenterView::CCenterView()
	: CFormView(CCenterView::IDD)
{
	m_pDC = NULL;
	m_pOldPalette = NULL;
	m_play = FALSE;
	STITCHING_MODE = MODE_DRAW_SOURCE;
	m_hrc = NULL;
	m_isMLClicked = 0;
	m_isMRClicked = 0;
	m_isZoom = 0;
	m_DrawSize = 10;
	m_DrawManualMaskSize = 10;
	m_init = FALSE;
	m_bCapConfActive = FALSE;
	m_bFirstTime = TRUE;
	m_bMovingMainWnd = FALSE;
	m_pdlgEditInput = NULL;
	m_pdlgEditInputStereo = NULL;
	m_pdlgEditCameraParam = NULL;
	m_pdlgAddOverlay = NULL;
	m_pdlgLogViewer = NULL;
	m_pdlgManualRotateDlg = NULL;
	m_bControlDown = FALSE;
	fps = new RingBuf(60);
#if 0	// Update Therad사용 Code 실제 화면이 그려지지 않음..
	m_bfnUpdateContinue = FALSE;
	m_pfnUpdateThread = NULL;
	m_hfnUpdateThExitEvt = NULL;
#endif
}

CCenterView::~CCenterView()
{
	free(fps);
#ifdef _DEBUG
	TRACE(_T("~CCenterView Call \n"));
#endif
}

void CCenterView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCenterView, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	//ON_WM_KEYDOWN()
	//ON_WM_KEYUP()
	ON_MESSAGE(WM_CLOSEEDITINPUT, OnCloseEditInput)
	ON_MESSAGE(WM_CLOSEEDITINPUTSTEREO, OnCloseEditInputStereo)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()


// CCenterView diagnostics

#ifdef _DEBUG
void CCenterView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCenterView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCenterView message handlers
void CALLBACK callbackGLUpdate(HWND hWnd, UINT nID, UINT_PTR nEl, DWORD time)
{
	CCenterView* phWnd = ((CCenterView*)(theApp.m_ScreenManager.m_pCenterView));
	//if (!(phWnd->m_bMovingMainWnd))
	{
		if (phWnd)
		{
#if 0
			phWnd->PostMessage(WM_UPDATECENTERSCREEN, 0, 0);
#else
			//wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
			//((CT360VidStitchDlg*)theApp.m_pMainWnd)->UpdateFPS(0);
			//CString szFPS;

			char szFPS[1024] = { 0, };
			static int count = 0, MaxCnt = 0;;
			static DOUBLE OldTimer = TQClock();
			double diff = TQClock() - OldTimer;

			static int oldTick = TQClock();
			static float fTemp;
			if (!phWnd->isFullScreen)
			{
				if (diff > 30)
					MaxCnt++;

				phWnd->fps->push(diff);

				fTemp = 1000.0f / phWnd->fps->avg();
				//sprintf_s(szFPS, "diff %5.1f FPS:  %5.1f fps %5.1f ms", diff, fTemp, phWnd->fps->avg());
				sprintf_s(szFPS, "FPS:  %5.1f", fTemp);

				TRACE("MaxCnt %d, %s\n", MaxCnt, szFPS);

				((CT360VidStitchDlg*)theApp.m_pMainWnd)->m_strFPS = szFPS;

				if (count >= 60)
				{
					//pFrame->SetPaneText(0, szFPS);
					((CT360VidStitchDlg*)theApp.m_pMainWnd)->PostMessage(WM_UPDATESTRINGFPS, 0, 0);
					count = 0;
					MaxCnt = 0;
					fTemp = 0.0f;
				}

				count++;
			}
			OldTimer = TQClock();
			//wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);
			//GLResize();
			switch (phWnd->STITCHING_MODE)
			{
			case MODE_DRAW_SOURCE:
				//printf("callbackGLUpdate!!\n");
				phWnd->draw_sources();
				break;
			case MODE_DRAW_OUTPUT:
				//printf("callbackGLUpdate!!\n");
				phWnd->draw_output();
				break;
			case MODE_DRAW_INTERACTIVE:
				//printf("callbackGLUpdate!!\n");
				if (phWnd->interactive_animation_mode == ANIMATION_MODE_ROTATE)
				{
					if (phWnd->anim_timer > ANIMATION_TIMER)
					{
						QVS_Engine::QVS_SetPan(diff / 1000.0f * phWnd->animation_speed);
						QVS_Engine::QVS_MulPitch(0.9);
					}
					else
					{
						phWnd->anim_timer += diff;
					}
				}
				phWnd->draw_interactive();
				break;
			case MODE_DRAW_MANUAL_CALIB:
				//printf("callbackGLUpdate!!\n");
				phWnd->draw_manual_calib();
				break;
			}
			
#endif
			//wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
		}
	}
}

int CCenterView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	Init();

	//m_dlgEditInput = new CEditInputDlg();
	//m_dlgEditInput.Create(CEditInputDlg::IDD, this);

	//m_pdlgToastHdd = new CTransparentDlgType();
	m_dlgToastHdd.Create(IDD_TRANSPARENT_DLG, this);
	//Sleep(1);
	//m_pdlgToastRtmp = new CTransparentDlgType();
	m_dlgToastStream.Create(IDD_TRANSPARENT_DLG, this);
	//Sleep(1);
	//m_pdlgToastOutput = new CTransparentDlgType();
	m_dlgToastOutput.Create(IDD_TRANSPARENT_DLG, this);
	//Sleep(1);
	//m_pdlgToastDone = new CTransparentDlgType();
	m_dlgToastDone.Create(IDD_TRANSPARENT_DLG, this);

	m_dlgToastProgress.Create(IDD_TRANSPARENT_DLG, this);

	m_dlgToastWarping.Create(IDD_TRANSPARENT_DLG, this);

	m_dlgToastFail.Create(IDD_TRANSPARENT_DLG, this);
	Sleep(1);
	//m_pSourceEditRectWnd = new CSourceEditRect();
	m_SourceEditRectWnd.Create(IDD_SOURCEEDITRECT, this);

	return 0;
}

void CCenterView::OnDestroy()
{
	HGLRC   hrc = NULL;
#if 0	// Update Therad사용 Code 실제 화면이 그려지지 않음..
	KillfnUpdateThread();
#endif
	m_play = FALSE;

	if (!m_init)
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(60.0/1000.0);

	KillTimer(TIMER_STARTSCREENGL);
	((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(60.0 / 1000.0);
	//종료 구문 추가
/*	for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
	{
		QVS_Engine::QVS_destroyVideoCapture(i);
	}
	QVS_Engine::QVS_destroyAudioDevice();
	*/
	((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(60.0 / 1000.0);

	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL, NULL);

	if (hrc)
		::wglDeleteContext(hrc);

	if (m_pOldPalette)
		m_pDC->SelectPalette(m_pOldPalette, FALSE);

	if (m_pDC)
		delete m_pDC;

	m_cPalette.DeleteObject();

	CFormView::OnDestroy();
	m_dlgToastHdd.DestroyWindow();
	m_dlgToastStream.DestroyWindow();
	m_dlgToastOutput.DestroyWindow();
	m_dlgToastDone.DestroyWindow();
	m_dlgToastProgress.DestroyWindow();
	m_dlgToastWarping.DestroyWindow();
	m_dlgToastFail.DestroyWindow();
	
	if (m_pdlgEditInput)
	{
		m_pdlgEditInput->DestroyWindow();
		delete m_pdlgEditInput;
		m_pdlgEditInput = NULL;
	}

	if (m_pdlgEditInputStereo)
	{
		m_pdlgEditInputStereo->DestroyWindow();
		delete m_pdlgEditInputStereo;
		m_pdlgEditInputStereo = NULL;
	}

	if (m_pdlgEditCameraParam)
	{
		m_pdlgEditCameraParam->DestroyWindow();
		delete m_pdlgEditCameraParam;
		m_pdlgEditCameraParam = NULL;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CCenterView::GLResize()
{
	CRect rc;
	int cx, cy;
	GetClientRect(rc);
	cx = rc.Width();
	cy = rc.Height();
	
	theApp.m_PrjAllCfg.m_nScrWidth = cx;
	theApp.m_PrjAllCfg.m_nScrHeight = cy;

	if (cy > 0)
	{
		//if (cy == 0) cy = 1;

		glViewport(0, 0, cx, cy);

		if ((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
		{
			RedrawWindow();
		}

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)cx / cy, 3.0f, 7.0f);
		glMatrixMode(GL_MODELVIEW);
	}
}

void CCenterView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	GLResize();

	if (GetSafeHwnd())
		Arrange();

	MoveToast();

	if (isFullScreen)
	{
		SetCursorPos(theApp.m_PrjAllCfg.m_nScrWidth / 2, theApp.m_PrjAllCfg.m_nScrHeight / 2);
		isSetEvent = true;
	}

	QVS_Engine::QVS_onSize(cx, cy);
}


void CCenterView::OnPlay()
{
	while (m_bFirstTime)
	{
		//GLResize();
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(60.0 / 1000.0);
	}

	if (m_init == FALSE)
	{
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(60.0 / 1000.0); //처리가 완료되고 지연시간을 준다.
		m_init = TRUE;
	}

	m_play = m_play ? FALSE : TRUE;


	if (m_play)
		SetTimer(TIMER_STARTSCREENGL, 0, callbackGLUpdate);
	else
		KillTimer(TIMER_STARTSCREENGL);
#if 0	// Update Therad사용 Code 실제 화면이 그려지지 않음..
	DoStartUpdateTimer();
#endif
	//여기서 Config를 init하자..

}
/////////////////////////////////////////////////////////////////////////////
// CQuramVideo360SampleView drawing: ready for drawing source/output/interactive
void CCenterView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (!templateLoad)
	{
		WCHAR Path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, Path);

//		PLM #17129 : 프로젝트를 새로 생성한 경우라면 정합된 결과가 보이지 않아야함 문제
//		char* szPath = CString2Char(theApp.m_PrjAllCfg.m_szPTSFullPath);
// 		if (theApp.m_ScreenManager.m_iStartType == 0 &&
// 			parsingPTS(szPath))
// 			templateLoad = TRUE;
// 		else
			if (theApp.m_ScreenManager.m_iStartType == 1)
			{
				//Project가 있는 Path로 변경.
				//SetCurrentDirectory(theApp.m_PrjAllCfg.m_szProjectFullPath);

				CString szFullPath;
				char* szPath2 = CString2Char(theApp.m_PrjAllCfg.m_szProjectFullPath + _T("\\") + theApp.m_PrjAllCfg.m_szProjectName);
				if (loadParam(szPath2))
					templateLoad = TRUE;
				free(szPath2);
				SetCurrentDirectory(Path);

			}
//		PLM #17129 :
//		free(szPath);
	}

	if (m_hrc)
	{
		//wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);
		if (m_bFirstTime || m_play)
		{
			switch (STITCHING_MODE)
			{
			case MODE_DRAW_SOURCE:
				printf("OnDraw!!\n");
				draw_sources();
				break;
			case MODE_DRAW_OUTPUT:
				printf("OnDraw!!\n");
				draw_output();
				break;
			case MODE_DRAW_INTERACTIVE:
				printf("OnDraw!!\n");
				draw_interactive();
				break;
			case MODE_DRAW_MANUAL_CALIB:
				printf("OnDraw!!\n");
				draw_manual_calib();
				break;
			}
			m_bFirstTime = FALSE;
		}
		//wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}
}


/////////////////////////////////////////////////////////////////////////////
// GL helper functions


void CCenterView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMER_STARTSCREENGL && !m_bMovingMainWnd)
	{
// 		if (m_hrc && m_init)
// 		{
// 			//wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);
// 			//GLResize();
// 			switch (STITCHING_MODE)
// 			{
// 			case MODE_DRAW_SOURCE:
// 				draw_sources();
// 				break;
// 			case MODE_DRAW_OUTPUT:
// 				draw_output();
// 				break;
// 			case MODE_DRAW_INTERACTIVE:
// 				draw_interactive();
// 				break;
// 			}
// 			//wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
// 
// 			((CT360VidStitchDlg*)theApp.m_pMainWnd)->UpdateFPS();
// 
// 		}
	}

	CFormView::OnTimer(nIDEvent);

	// Eat spurious WM_TIMER messages
	MSG msg;
	while (::PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE));
}

/////////////////////////////////////////////////////////////////////////////
// GL helper functions
void CCenterView::Init()
{
	PIXELFORMATDESCRIPTOR pfd;
	int         n;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	CreateRGBPalette();

	m_hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	GetClientRect(&m_oldRect);

	initQVSEngine();
}

void CCenterView::initQVSEngine()
{
	printf("bigheadk, engine init!!\n");
	bool result = QVS_Engine::QVS_InitEngineManager();

	CString str;

	if (result)
	{
#ifdef _DEBUG
		TRACE("%s\n", MSG_SUCCESS);
#endif
		theApp.m_PrjAllCfg.m_bInitQVS_EngineDone = TRUE;
		
	}
	else
	{
		str = CString(MSG_FAIL);
#ifdef _DEBUG
		TRACE("ERROR : %s\n", MSG_FAIL);
#endif
		theApp.m_PrjAllCfg.m_bInitQVS_EngineDone = FALSE;
		//그냥 닫자.
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return;
	}

	printf("bigheadk init QVSEngine: (m_output_width, m_output_height) = (%d, %d) %d\n", theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight, theApp.m_PrjAllCfg.m_nStitchingMode);
	result = QVS_Engine::QVS_Create(theApp.m_PrjAllCfg.m_nMaxCamra, theApp.m_PrjAllCfg.m_nWidth, theApp.m_PrjAllCfg.m_nHeight, theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight, theApp.m_PrjAllCfg.m_nStitchingMode);
	QVS_Engine::QVS_onSize(theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight);

	((CConfiguView*)theApp.m_ScreenManager.m_pConfigView)->setValue();

	writeLog();

#if(LICENSE_MACADDRESS  != 0)
	if (result != QVS_SUCCESS)
	{
		theApp.m_PrjAllCfg.m_bInitQVS_EngineDone = FALSE;
		//그냥 닫자.
		MessageBox(_T("VRue360 : Invalid MacAddress."), MB_OK, MB_ICONINFORMATION);
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return;
	}
#endif

	theApp.m_ScreenManager.setCallbacks();

}

void CCenterView::draw_sources()
{
// 	CRect rc;
 	int err;
// 	GetClientRect(rc);
// 	try
// 	{
		//if (theApp.m_PrjAllCfg.m_nScrHeight > 0)
			err = QVS_Engine::QVS_Drawing(theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight);
		//else
		//	err = QVS_Engine::QVS_Drawing(theApp.m_PrjAllCfg.m_nWidth, theApp.m_PrjAllCfg.m_nHeight);


		SwapBuffers(wglGetCurrentDC());
// 	}
// 	catch (CException* ex)
// 	{
// 		ex->ReportError();
// 	}
}
void CCenterView::draw_manual_calib()
{
	int err;
	err = QVS_Engine::QVS_DrawingManualCalib(theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight);
	SwapBuffers(wglGetCurrentDC());
}
void CCenterView::draw_output()
{
// 	CRect rc;
 	int err;
// 
// 	GetClientRect(rc);
// 	try
// 	{
		//if (theApp.m_PrjAllCfg.m_nScrHeight > 0)
			err = QVS_Engine::QVS_Stitching(theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight, PANORAMA_RREVIEW_FULL);
		//else
		//	err = QVS_Engine::QVS_Stitching(theApp.m_PrjAllCfg.m_nWidth, theApp.m_PrjAllCfg.m_nHeight, PANORAMA_RREVIEW_FULL);
		SwapBuffers(wglGetCurrentDC());
// 	}
// 	catch (CException* ex)
// 	{
// 		ex->ReportError();
// 	}
}
void CCenterView::draw_interactive()
{

 	int err;

	int oldTick = GetTickCount();

	err = QVS_Engine::QVS_DrawingInteractive(theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight);

	int oldTick1 = GetTickCount();

	SwapBuffers(wglGetCurrentDC());

	TRACE("DI: %d, %d\n", oldTick1 - oldTick, GetTickCount() - oldTick1);

}
int CCenterView::parsingPTS(char* path)
{
	int result = QVS_Engine::QVS_ParsingPTS(path);
	if (result == 0) {
#ifdef _DEBUG
		TRACE("ERROR :App: Load PTS Fail!\n");
#endif
		AfxMessageBox(_T("Load PTS Fail!"), MB_OK, MB_ICONERROR);
	}
	else
	{
#ifdef _DEBUG
		TRACE("App: Load PTS Success!\n");
#endif
		printf("SampleApp: Load PTS Success!\n");
	}

	return result;
}

int CCenterView::loadParam(char* path)
{
	int result = QVS_Engine::QVS_LoadParam(path);

	if (result == 0) {
#ifdef _DEBUG
		TRACE("SampleApp: Load Configuration Fail!\n");
#endif
	}
	else
	{
#ifdef _DEBUG
		TRACE("SampleApp: Load Configuration Success!\n");
#endif
	}
	return result;
}


BOOL CCenterView::IsNeedSaveParam(char* path)
{
	BOOL bIsClose = FALSE;
	int result = QVS_Engine::QVS_IsNeedSave()|theApp.m_PrjAllCfg.isNeedSave();
	if (result == 0)	//	변경된 파일이 없음 -> 그대로 종료
		bIsClose = TRUE;
	if (result == 1)	// 변경된 파일이 존재
	{
		int ButtonRst = AfxMessageBox(_T("Do you want to save changes to the modified files?"), MB_YESNOCANCEL);
		if (ButtonRst == IDYES)			//	YES -> 저장 && 종료
		{
			saveParam(path);
			bIsClose = TRUE;
		}
		else if (ButtonRst == IDNO)		//	NO -> 종료
			bIsClose = TRUE;
		else if (ButtonRst == IDCANCEL)	// CANCEL -> 리턴
			bIsClose = FALSE;
	}
	return bIsClose;
}

int CCenterView::saveParam(char* path)
{
	int result = QVS_Engine::QVS_SetSavePath(path);
#ifdef _DEBUG
	if (result == 0) 
		TRACE("ERROR: QVS_SetSavePath: [%s]Fail!\n", path);
	else
		TRACE("QVS_SetSavePath: [%s]Success!\n", path);
#endif
	if (result != 0)
	{
		result = QVS_Engine::QVS_SaveParam(1);
#ifdef _DEBUG
		if (result == 0)
			TRACE("ERROR: QVS_SaveParam: [%s]Fail!\n", path);
		else
			TRACE("QVS_SaveParam: [%s]Success!\n", path);
#endif
	}
	return result;
}

BOOL CCenterView::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |          // support OpenGL
		PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ((pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0)
	{
		//MessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		//MessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}

	return TRUE;
}

unsigned char CCenterView::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
	unsigned char val;

	val = (unsigned char)(i >> shift);
	switch (nbits)
	{

	case 1:
		val &= 0x1;
		return oneto8[val];
	case 2:
		val &= 0x3;
		return twoto8[val];
	case 3:
		val &= 0x7;
		return threeto8[val];

	default:
		return 0;
	}
}

#pragma warning(disable : 4244)
void CCenterView::CreateRGBPalette()
{
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE *pPal;
	int n, i;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE)
	{
		n = 1 << pfd.cColorBits;
		pPal = (PLOGPALETTE) new char[sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY)];

		ASSERT(pPal != NULL);

		pPal->palVersion = 0x300;
		pPal->palNumEntries = n;
		for (i = 0; i<n; i++)
		{
			pPal->palPalEntry[i].peRed =
				ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
			pPal->palPalEntry[i].peGreen =
				ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
			pPal->palPalEntry[i].peBlue =
				ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
			pPal->palPalEntry[i].peFlags = 0;
		}

		/* fix up the palette to include the default GDI palette */
		if ((pfd.cColorBits == 8) &&
			(pfd.cRedBits == 3) && (pfd.cRedShift == 0) &&
			(pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
			(pfd.cBlueBits == 2) && (pfd.cBlueShift == 6)
			)
		{
			for (i = 1; i <= 12; i++)
				pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
		}

		m_cPalette.CreatePalette(pPal);
		delete[] pPal;

		m_pOldPalette = m_pDC->SelectPalette(&m_cPalette, FALSE);
		m_pDC->RealizePalette();
	}
}

void CCenterView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Arrange();
}

void CCenterView::ShowToastDlg(vdstToastType nType, CString szText, BOOL bShow)
{
	CTransparentDlgType* pToastDlg = NULL;

	switch (nType)
	{
		case vdstToastHdd:
		{
			pToastDlg = &m_dlgToastHdd;
		}
		break;
		case vdstToastStream:
		{
			pToastDlg = &m_dlgToastStream;
		}
		break;
		case vdstToastOutput:
		{
			pToastDlg = &m_dlgToastOutput;
		}
		break;
		case vdstToastDone:
		{
			 pToastDlg = &m_dlgToastDone;
		}
		break;
		case vdstToastProgress:
		{
			 pToastDlg = &m_dlgToastProgress;
		}
			break;
		case vdstToastWarping:
		{
			pToastDlg = &m_dlgToastWarping;
		}
			break;
		case vdstToastFail:
		{
			pToastDlg = &m_dlgToastFail;
		}
			break;
	}

	if (pToastDlg)
	{
		if (szText.GetLength() > 0)
		{
			pToastDlg->GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(szText);
			pToastDlg->ToastString(szText);
		}

		pToastDlg->ShowWindow(bShow ? SW_SHOW : SW_HIDE);

		if (bShow && (pToastDlg != &m_dlgToastDone) && (pToastDlg != &m_dlgToastWarping))
		{
			switch (nType){
			case vdstToastHdd:
					pToastDlg->DoStartATimer(vdstToastHdd);
				break;
			case vdstToastStream:
					pToastDlg->DoStartATimer(vdstToastStream);
				break;
			//case vdstToastOutput:
			//	if (output_time == 0)
			//		output_time = pToastDlg->DoStartATimer();
			//	else
			//		pToastDlg->DoStartATimer(output_time);
			//	break;
			case vdstToastProgress:
				pToastDlg->DoStartLoading();
				break;
			}
		}
	}

	MoveToast();
}

void CCenterView::MoveToast()
{
	int nVisivleCnt = 0;
	CRect  mainRc;
	CRect rc, rcCopy, rcBackup;

	GetWindowRect(mainRc);
	//여기 계산 재 필요..
	if (::IsWindow(m_dlgToastHdd.m_hWnd))
	{
		m_dlgToastHdd.GetWindowRect(rc);
		theApp.m_pMainWnd->ScreenToClient(rc);
		rcCopy = rc;

		rcCopy.top = mainRc.bottom - rc.Height() / 2;
		rcCopy.bottom = rcCopy.top + rc.Height();
		rcCopy.left = mainRc.left + (mainRc.Width() - rc.Width()) / 2;
		rcCopy.right = rcCopy.left + rc.Width();

		m_dlgToastHdd.MoveWindow(rcCopy);
		if (m_dlgToastHdd.IsWindowVisible()) 
			nVisivleCnt++;

		rcBackup = rcCopy;
	}

	if (::IsWindow(m_dlgToastStream.m_hWnd))
	{
		rcBackup.top = rcBackup.top - rc.Height()*nVisivleCnt;
		rcBackup.bottom = rcBackup.top + rc.Height();

		m_dlgToastStream.MoveWindow(rcBackup);

		if (m_dlgToastStream.IsWindowVisible())
			nVisivleCnt++;
	}

	if (::IsWindow(m_dlgToastOutput.m_hWnd))
	{
		rcBackup = rcCopy;

		rcBackup.top = rcBackup.top - rc.Height()*nVisivleCnt;
		rcBackup.bottom = rcBackup.top + rc.Height();

		m_dlgToastOutput.MoveWindow(rcBackup);

		if (m_dlgToastOutput.IsWindowVisible())
			nVisivleCnt++;
	}

	if (::IsWindow(m_dlgToastDone.m_hWnd))
	{
		rcBackup = rcCopy;

		rcBackup.top = rcBackup.top - rc.Height()*nVisivleCnt;
		rcBackup.bottom = rcBackup.top + rc.Height();

		m_dlgToastDone.MoveWindow(rcBackup);
	}

	if (::IsWindow(m_dlgToastWarping.m_hWnd))
	{
		rcBackup = rcCopy;

		rcBackup.top = rcBackup.top - rc.Height()*nVisivleCnt;
		rcBackup.bottom = rcBackup.top + rc.Height();

		m_dlgToastWarping.MoveWindow(rcBackup);
	}

	if (::IsWindow(m_dlgToastFail.m_hWnd))
	{
		rcBackup = rcCopy;

		rcBackup.top = rcBackup.top - rc.Height()*nVisivleCnt;
		rcBackup.bottom = rcBackup.top + rc.Height();

		m_dlgToastFail.MoveWindow(rcBackup);
	}

	if (::IsWindow(m_dlgToastProgress.m_hWnd))
	{
		int x, y, width, height;
		x = mainRc.left - 155;
		y = mainRc.top - 55;
		width = mainRc.right - mainRc.left + 310;
		height = mainRc.bottom - mainRc.top + 240;

		m_dlgToastProgress.SetWindowPos(&wndTop, x, y, width, height, NULL);
		m_dlgToastProgress.MoveTextPos(0, 0, width, height);
	}
}
void CCenterView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFormView::OnLButtonDblClk(nFlags, point);
}
void CCenterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//printf("OnLButtonDown %d || point (%d, %d)\n", STITCHING_MODE, point.x, point.y);
	switch (STITCHING_MODE)
	{
	case MODE_DRAW_SOURCE:
		if (m_init && m_bCapConfActive && m_isMLClicked == 0)
		{
			if (QVS_Engine::QVS_onDown(point.x, point.y) == 1)
			{
				m_OldPoint = point;
				m_isMLClicked = 1;
			}
		}
		break;
	case MODE_DRAW_OUTPUT:
		//printf("test LButton====> m_init(%d), isOverlayEdit(%d)\n", m_init, isOverlayEdit);
		if (m_init)
		{
			if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_EDIT_ROTATION)
			{
				m_isMLClicked = 1;
				m_OldPoint = point;
			}
			else if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_ADVANCE_UPDATE)
			{
				m_isMLClicked = 1;
				m_OldPoint = point;
				QVS_Engine::QVS_selectROI(point.x * QVS_Engine::QVS_GetOutputWidth() / (float)theApp.m_PrjAllCfg.m_nScrWidth,
					point.y * QVS_Engine::QVS_GetOutputHeight() / (float)theApp.m_PrjAllCfg.m_nScrHeight, m_isMLSelectClicked);

				((RightAdvanceUpdateChildDlg*)theApp.m_ScreenManager.m_pRightAdvanceUpdateChild)->isSelectAllCheck();
			}
		}
		break;
	case MODE_DRAW_MANUAL_CALIB:
		QVS_Engine::QVS_drawManualPosition(point.x, point.y, m_DrawManualMaskSize);
		if (m_init)
		{
			if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_MANUAL_CALIB_MASK)
			{
				if (point.x < theApp.m_PrjAllCfg.m_nScrWidth / 2) m_isLeft = 1;
				else m_isLeft = 0;

				m_isMLClicked = 1;
				//m_OldPoint = point;
				//int drawMode = 0;
				//if (m_isMLClicked)
				int	drawMode = 1;
				QVS_Engine::QVS_drawManualROI(point.x, point.y, theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight, m_DrawManualMaskSize, drawMode, m_isLeft);
				//QVS_Engine::QVS_clacManualMaskPoint2(m_isLeft, point.x, point.y);
			}
		}
		break;
	case MODE_DRAW_INTERACTIVE:
		m_isMLClicked = 1;
		m_OldPoint = point;
		SetCapture();
		break;
	}
	CFormView::OnLButtonDown(nFlags, point);
}

void CCenterView::OnRButtonDown(UINT nFlags, CPoint point)
{
	switch (STITCHING_MODE)
	{
	case MODE_DRAW_OUTPUT:
		if (m_init)
		{
			if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_EDIT_ROTATION || theApp.m_ScreenManager.m_pSubMode == SUBMODE_ADVANCE_UPDATE)
			{
				m_isMRClicked = 1;
				m_OldPoint = point;
			}
			else if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_REGIONAL_WARPING)
			{
				m_isMRClicked = 1;
				m_OldPoint = point;
			}

		}
		break;
	case MODE_DRAW_MANUAL_CALIB:
		QVS_Engine::QVS_drawManualPosition(point.x, point.y, m_DrawManualMaskSize);
		if (m_init)
		{
			if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_MANUAL_CALIB_MASK)
			{
				if (point.x < theApp.m_PrjAllCfg.m_nScrWidth / 2) m_isLeft = 1;
				else m_isLeft = 0;

				m_isMRClicked = 1;
				m_OldPoint = point;
				int drawMode = 0;
				QVS_Engine::QVS_drawManualROI(point.x, point.y, theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight, m_DrawManualMaskSize, drawMode, m_isLeft);
				//QVS_Engine::QVS_clacManualMaskPoint2(m_isLeft, point.x, point.y);
			}
		}
		break;
	}
	CFormView::OnRButtonDown(nFlags, point);
}

void CCenterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (STITCHING_MODE)
	{
	case MODE_DRAW_SOURCE:
		if (m_isMLClicked == 1){
			QVS_Engine::QVS_onUp(point.x, point.y);
			m_isMLClicked = 0;
		}
		break;
	case MODE_DRAW_OUTPUT:
		m_isMLSelectClicked = 0;
		if (m_isMLClicked == 1)
		{
			m_isMLClicked = 0;
			if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_ADVANCE_UPDATE)
			{
				QVS_Engine::QVS_drawROIFinish();
			}
		}	
		if (isOverlayEdit)
		{
			is_overlay_move = false;
		}
		break;
	case MODE_DRAW_MANUAL_CALIB:
		if (m_isMLClicked == 1)
		{
			m_isMLClicked = 0;
			if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_MANUAL_CALIB_MASK)
			{
				QVS_Engine::QVS_drawROIFinish();
			}
		}
		break;

	case MODE_DRAW_INTERACTIVE:
		if (m_isMLClicked == 1)
		{
			m_isMLClicked = 0;
			ReleaseCapture();
		}
		break;
	}
	CFormView::OnLButtonUp(nFlags, point);
}

void CCenterView::OnRButtonUp(UINT nFlags, CPoint point)
{
	switch (STITCHING_MODE)
	{
	case MODE_DRAW_OUTPUT:
		if (m_isMRClicked == 1)
		{
			m_isMRClicked = 0;
			if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_ADVANCE_UPDATE)
			{
				QVS_Engine::QVS_drawROIFinish();
			}
		}
		if (isOverlayEdit)
		{
			is_overlay_roll = false;
		}
		break;
	case MODE_DRAW_MANUAL_CALIB:
		if (m_isMRClicked == 1)
		{
			m_isMRClicked = 0;
			if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_MANUAL_CALIB_MASK)
			{
				QVS_Engine::QVS_drawROIFinish();
			}
		}
		break;
	}

	CFormView::OnRButtonUp(nFlags, point);
}

void CCenterView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (STITCHING_MODE)
	{
	case MODE_DRAW_SOURCE:
		if (m_isMLClicked == 1){
			int dx = m_OldPoint.x - point.x;
			int dy = m_OldPoint.y - point.y;

			m_OldPoint = point;

			QVS_Engine::QVS_onScroll(dx, dy);
		}
		break;
	case MODE_DRAW_OUTPUT:
		if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_EDIT_ROTATION)
		{
			if (m_isMLClicked == 1)
			{
				//if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
				{
					float x = point.x - m_OldPoint.x;
					float y = point.y - m_OldPoint.y;
					//printf("m_OldPoint(%d, %d), point(%d, %d)\n", m_OldPoint.x, m_OldPoint.y, point.x, point.y);
					if (x != 0.0f || y != 0.0f)
						QVS_Engine::QVS_moveOutput(x / (float)theApp.m_PrjAllCfg.m_nScrWidth,
						y / (float)theApp.m_PrjAllCfg.m_nScrHeight, 0, 0, 0);

					m_OldPoint = point;
				}
			}
			if (m_isMRClicked == 1)
			{
				int isRoll, isReverseX, isReverseY;
				isRoll = 1;
				float x = point.x - m_OldPoint.x;
				float y = point.y - m_OldPoint.y;
				if ((float)theApp.m_PrjAllCfg.m_nScrWidth / 2 > point.x)
					isReverseX = 0;
				else
					isReverseX = 1;

				if ((float)theApp.m_PrjAllCfg.m_nScrHeight / 2 > point.y)
					isReverseY = 0;
				else
					isReverseY = 1;

				if (x != 0.0f || y != 0.0f)
					QVS_Engine::QVS_moveOutput(x / (float)theApp.m_PrjAllCfg.m_nScrWidth,
					y / (float)theApp.m_PrjAllCfg.m_nScrHeight, isRoll, isReverseX, isReverseY);

				m_OldPoint = point;
			}
		}
		else if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_ADVANCE_UPDATE)
		{
			if ((m_isMLClicked || m_isMRClicked) && !m_isMLSelectClicked)
			{
				int drawMode = 0;
				float x = point.x;
				float y = point.y;
				if (m_isMLClicked)
					drawMode = 1;
				else
					drawMode = 0;
				QVS_Engine::QVS_drawROI(x * QVS_Engine::QVS_GetOutputWidth() / (float)theApp.m_PrjAllCfg.m_nScrWidth,
					y * QVS_Engine::QVS_GetOutputHeight() / (float)theApp.m_PrjAllCfg.m_nScrHeight, m_DrawSize, drawMode);
			}
		}
		else if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_REGIONAL_WARPING)
		{
			SetFocus();

			QVS_Engine::QVS_setCursor(point.x, point.y);

			if (m_isMRClicked)
			{
				int dx = m_OldPoint.x - point.x;
				int dy = m_OldPoint.y - point.y;

				m_OldPoint = point;

				QVS_Engine::QVS_Scroll(dx, dy, theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight);
			}
		}
		
		break;
	case MODE_DRAW_MANUAL_CALIB:
		if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_MANUAL_CALIB_MASK)
		{
			int isLeft = 0;
			if (point.x < theApp.m_PrjAllCfg.m_nScrWidth / 2) isLeft = 1;
			else isLeft = 0;
			QVS_Engine::QVS_drawManualPosition(point.x, point.y, m_DrawManualMaskSize);
			if ((m_isMLClicked || m_isMRClicked) && (m_isLeft == isLeft))
			{
				int drawMode = 0;
				if (m_isMLClicked)
					drawMode = 1;
				QVS_Engine::QVS_drawManualROI(point.x, point.y, theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight, m_DrawManualMaskSize, drawMode, isLeft);
				//QVS_Engine::QVS_clacManualMaskPoint2(isLeft, point.x, point.y);
			}
			else{
				QVS_Engine::QVS_InitDrawManualROI();
			}
		}
		break;
	case MODE_DRAW_INTERACTIVE:
		if (isFullScreen)
		{
			if (isSetEvent)
			{
				isSetEvent = false;
			}
			else {
				float x = m_OldPoint.x - point.x;
				float y = m_OldPoint.y - point.y;
				if (x != 0.0f)
				{
					QVS_Engine::QVS_SetPan(-x / 1000.f);
					anim_timer = 0;
				}
				if (y != 0.0f)
				{
					QVS_Engine::QVS_SetPitch(-y / 1000.f);
					anim_timer = 0;
				}

				SetCursorPos(theApp.m_PrjAllCfg.m_nScrWidth / 2, theApp.m_PrjAllCfg.m_nScrHeight / 2);
				isSetEvent = true;
			}
		}
		else if (m_isMLClicked == 1){
			if (theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
			{
				float x = m_OldPoint.x - point.x;
				float y = m_OldPoint.y - point.y;
				if (x != 0.0f)
				{
					QVS_Engine::QVS_SetPan(x / 1000.f);
					anim_timer = 0;
				}
				if (y != 0.0f)
				{
					QVS_Engine::QVS_SetPitch(y / 1000.f);
					anim_timer = 0;
				}

			}
		}
		m_OldPoint = point;
		break;
	}
	CFormView::OnMouseMove(nFlags, point);
}

#pragma warning(default : 4244)

BOOL CCenterView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
}


void CCenterView::Arrange()
{
	//이곳에서 PictureBox Control의 기본 크기 및 순서대로 위치에 놓는다..
	CRect rc, wrc, newrc;
	int posx = 0, posy = 0;

	GetClientRect(&rc);

	m_subWinWidth = rc.Width() / 3;
	m_subWinHeight = rc.Height() / 3;
	m_SourceEditRectWnd.SetSize(m_subWinWidth, m_subWinHeight);
	for (int i = 0; i < MAX_SOURCE; i++)
	{
		if (i != 0 && (i % 3) == 0)
		{
			posx = 0;
			posy = m_subWinHeight*(i / 3);
		}
		m_rcSourceRect[i] = CRect(posx, posy, posx + m_subWinWidth, posy + m_subWinHeight);

		posx = posx + m_subWinWidth;
	}
}

void CCenterView::onKeyUP(MSG* pMsg)
{
	CString szTemp;
	int nPos = ((CRightInteracChildDlg*)theApp.m_ScreenManager.m_pRightInteracChild)->m_sldSpeed.GetPos();

	switch (pMsg->wParam)
	{
	case 0x41:	// 'a'
		if (interactive_animation_mode == ANIMATION_MODE_NONE)
		{
			interactive_animation_mode = ANIMATION_MODE_ROTATE;
			animation_speed = 0.05;
			anim_timer = 0;

			((CRightInteracChildDlg*)theApp.m_ScreenManager.m_pRightInteracChild)->PlayState(interactive_animation_mode);
			printf("Start DrawInteractive Animation\n");
		}
		break;
	case 0x53:	// 's'
		interactive_animation_mode = ANIMATION_MODE_NONE;
		animation_speed = 0;
		anim_timer = 0;

		((CRightInteracChildDlg*)theApp.m_ScreenManager.m_pRightInteracChild)->PlayState(interactive_animation_mode);
		printf("Stop DrawInteractive Animation\n");
		break;
	case 0x42:	// 'b'
		if (interactive_animation_mode = ANIMATION_MODE_ROTATE)
			animation_speed = ((CRightInteracChildDlg*)theApp.m_ScreenManager.m_pRightInteracChild)->PlaySpeed(ANIMATION_SPEED_UP, animation_speed);
		break;
	case 0x56:	// 'v'
		if (interactive_animation_mode = ANIMATION_MODE_ROTATE)
			animation_speed = ((CRightInteracChildDlg*)theApp.m_ScreenManager.m_pRightInteracChild)->PlaySpeed(ANIMATION_SPEED_DOWN, animation_speed);
		break;
	}
}

//This function don't work sometimes...Why??
BOOL CCenterView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (STITCHING_MODE == MODE_DRAW_INTERACTIVE)
	{
#ifdef _DEBUG
		TRACE("OnMouseWheel %d\n", zDelta);
#endif
		int zPos = 1;
		int nLoop = abs(zDelta) / 60;
		zPos = (zDelta < 0) ? 1 : -1;
		for (int i = 0; i < nLoop; i++)
		{
			QVS_Engine::QVS_SetZoom(zPos);
			anim_timer = 0;
		}
	}
	else if (STITCHING_MODE == MODE_DRAW_OUTPUT)
	{
		if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_ADVANCE_UPDATE)
		{
			m_DrawSize += zDelta / 60;
			if (m_DrawSize < 10)
				m_DrawSize = 10;
			else if (m_DrawSize > 100)
				m_DrawSize = 100;
		}
		else if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_REGIONAL_WARPING)
		{
			if (zDelta < 0)
			{
				//QVS_Engine::QVS_ZoomIn(0.9, pt.x - 161, pt.y - 82);
				QVS_Engine::QVS_ZoomIn(0.9, QVS_Engine::QVS_getCursorX(), QVS_Engine::QVS_getCursorY());
			}
			else
			{
				//QVS_Engine::QVS_ZoomIn(1.1, pt.x - 161, pt.y - 82);
				QVS_Engine::QVS_ZoomIn(1.1, QVS_Engine::QVS_getCursorX(), QVS_Engine::QVS_getCursorY());
			}
		}
		else if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_EDIT_ROTATION)
		{
			float scale = 1.f;
			int zPos = 1;
			//int nLoop = abs(zDelta) / 60;
			zPos = (zDelta < 0) ? 1 : -1;

			if (zPos > 0)
			{
				scale = 1.01;
				QVS_Engine::QVS_ScaleOutput(scale, QVS_Engine::QVS_getCursorX(), QVS_Engine::QVS_getCursorY());
			}
			else if (zPos < 0)
			{
				scale = 0.99;
				QVS_Engine::QVS_ScaleOutput(scale, QVS_Engine::QVS_getCursorX(), QVS_Engine::QVS_getCursorY());
			}
		}
		
	}
	else if (STITCHING_MODE == MODE_DRAW_MANUAL_CALIB)
	{
		if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_MANUAL_CALIB_MASK)
		{
			m_DrawManualMaskSize += zDelta / 60;
			if (m_DrawManualMaskSize < 10)
				m_DrawManualMaskSize = 10;
			else if (m_DrawManualMaskSize > 100)
				m_DrawManualMaskSize = 100;
			QVS_Engine::QVS_drawManualPosition(-1, -1, m_DrawManualMaskSize);
		}
	}
	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}


LRESULT CCenterView::OnCloseEditInput(WPARAM wParam, LPARAM lParam)
{
	if (m_pdlgEditInput != NULL)
	{
		m_pdlgEditInput->ShowWindow(SW_HIDE);
		m_pdlgEditInput->DestroyWindow();
		delete m_pdlgEditInput;
		m_pdlgEditInput = NULL;
		if (m_bCapConfActive)
		{
			int nDeviceCnt = 0;
			char temp[256] = { 0, };
			for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
			{
				int nDevicIdx = QVS_Engine::QVS_getCaptureDeviceIndex(i);
				sprintf(temp, "%s", QVS_Engine::QVS_getCaptureDeviceName(i));
#ifdef _DEBUG
				TRACE(_T("SnapShot####  [%d]%s[Device idx: %d]\n"), i, CString(temp), nDevicIdx);
#endif
				if (nDevicIdx != -1)
				{
					nDeviceCnt++;			//실제 Device가 
				}
			}
			if (nDeviceCnt == 0)
				theApp.m_ScreenManager.m_pBottomSourceChild->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_CAPCONF, BN_CLICKED), 0L);
		}
	}
	return 0;
}

LRESULT CCenterView::OnCloseEditInputStereo(WPARAM wParam, LPARAM lParam)
{
	if (m_pdlgEditInputStereo != NULL)
	{
		m_pdlgEditInputStereo->ShowWindow(SW_HIDE);
		m_pdlgEditInputStereo->DestroyWindow();
		delete m_pdlgEditInputStereo;
		m_pdlgEditInputStereo = NULL;
		if (m_bCapConfActive)
		{
			int nDeviceCnt = 0;
			char temp[256] = { 0, };
			for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
			{
				int nDevicIdx = QVS_Engine::QVS_getCaptureDeviceIndex(i);
				sprintf(temp, "%s", QVS_Engine::QVS_getCaptureDeviceName(i));
#ifdef _DEBUG
				TRACE(_T("SnapShot####  [%d]%s[Device idx: %d]\n"), i, CString(temp), nDevicIdx);
#endif
				if (nDevicIdx != -1)
				{
					nDeviceCnt++;			//실제 Device가 
				}
			}
			if (nDeviceCnt == 0)
				theApp.m_ScreenManager.m_pBottomSourceChild->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_CAPCONF, BN_CLICKED), 0L);
		}
	}
	return 0;
}

void CCenterView::showEditInputDlg(BOOL bShow)
{
	if (theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo || theApp.m_PrjAllCfg.m_nStitchingMode == QVS_Engine::StitchingMode_Stereo_Side)
		ShowEditInputStereo(bShow);
	else
		ShowEditInput(bShow);
}

void CCenterView::ShowEditInput(BOOL bShow)
{
	if (bShow)
	{
		if (m_pdlgEditInput == NULL)
		{
			m_pdlgEditInput = new CEditInputDlg;
			m_pdlgEditInput->Create(CEditInputDlg::IDD, this);
			m_pdlgEditInput->ShowWindow(bShow);
			//초기 위치 잡기
			CRect rc, rcMain, rcCopy;
			m_pdlgEditInput->GetWindowRect(rc);
			theApp.m_pMainWnd->GetWindowRect(rcMain);
			rcCopy = rc;

			rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
			rcCopy.bottom = rcCopy.top + rc.Height();
			rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
			rcCopy.right = rcCopy.left + rc.Width();

			m_pdlgEditInput->MoveWindow(rcCopy);
		}
	}
	else
	{
		if (m_pdlgEditInput)
		{
			m_pdlgEditInput->ShowWindow(bShow);
			m_pdlgEditInput->DestroyWindow();
			delete m_pdlgEditInput;
			m_pdlgEditInput = NULL;
		}
	}
}

void CCenterView::ShowEditInputStereo(BOOL bShow)
{
	if (bShow)
	{
		if (m_pdlgEditInputStereo == NULL)
		{
			m_pdlgEditInputStereo = new CEditInputStereoDlg;
			m_pdlgEditInputStereo->Create(CEditInputStereoDlg::IDD, this);
			m_pdlgEditInputStereo->ShowWindow(bShow);
			//초기 위치 잡기
			CRect rc, rcMain, rcCopy;
			m_pdlgEditInputStereo->GetWindowRect(rc);
			theApp.m_pMainWnd->GetWindowRect(rcMain);
			rcCopy = rc;

			rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
			rcCopy.bottom = rcCopy.top + rc.Height();
			rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
			rcCopy.right = rcCopy.left + rc.Width();

			m_pdlgEditInputStereo->MoveWindow(rcCopy);
		}
	}
	else
	{
		if (m_pdlgEditInputStereo)
		{
			m_pdlgEditInputStereo->ShowWindow(bShow);
			m_pdlgEditInputStereo->DestroyWindow();
			delete m_pdlgEditInputStereo;
			m_pdlgEditInputStereo = NULL;
		}
	}
}

void CCenterView::ShowEditCameraParam(BOOL bShow)
{
	if (bShow)
	{
		if (m_pdlgEditCameraParam == NULL)
		{
			m_pdlgEditCameraParam = new EditCameraParamDlg;
			m_pdlgEditCameraParam->Create(EditCameraParamDlg::IDD, this);
			m_pdlgEditCameraParam->ShowWindow(bShow);
			//초기 위치 잡기
			CRect rc, rcMain, rcCopy;
			m_pdlgEditCameraParam->GetWindowRect(rc);
			theApp.m_pMainWnd->GetWindowRect(rcMain);
			rcCopy = rc;

			rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
			rcCopy.bottom = rcCopy.top + rc.Height();
			rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
			rcCopy.right = rcCopy.left + rc.Width();

			m_pdlgEditCameraParam->MoveWindow(rcCopy);
		}
	}
	else
	{
		if (m_pdlgEditCameraParam)
		{
			m_pdlgEditCameraParam->ShowWindow(bShow);
			m_pdlgEditCameraParam->DestroyWindow();
			delete m_pdlgEditCameraParam;
			m_pdlgEditCameraParam = NULL;
		}
	}
}


void CCenterView::ShowAddOverlay(BOOL bShow)
{
	//if (bShow)
	//{
	//	if (m_pdlgAddOverlay == NULL)
	//	{
	//		m_pdlgAddOverlay = new CAddOverlayParentDlg;
	//		m_pdlgAddOverlay->Create(CAddOverlayParentDlg::IDD, this);
	//		m_pdlgAddOverlay->ShowWindow(bShow);
	//		isAddOverlayDlgShow = true;
	//		//초기 위치 잡기
	//		CRect rc, rcMain, rcCopy;
	//		m_pdlgAddOverlay->GetWindowRect(rc);
	//		theApp.m_pMainWnd->GetWindowRect(rcMain);
	//		rcCopy = rc;

	//		rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
	//		rcCopy.bottom = rcCopy.top + rc.Height();
	//		rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
	//		rcCopy.right = rcCopy.left + rc.Width();

	//		m_pdlgAddOverlay->MoveWindow(rcCopy);
	//	}
	//	else if (m_pdlgAddOverlay)
	//	{
	//		m_pdlgAddOverlay->ShowWindow(bShow);
	//		m_pdlgAddOverlay->DestroyWindow();
	//		delete m_pdlgAddOverlay;
	//		m_pdlgAddOverlay = NULL;

	//		m_pdlgAddOverlay = new CAddOverlayParentDlg;
	//		m_pdlgAddOverlay->Create(CAddOverlayParentDlg::IDD, this);
	//		m_pdlgAddOverlay->ShowWindow(bShow);
	//		isAddOverlayDlgShow = true;
	//		//초기 위치 잡기
	//		CRect rc, rcMain, rcCopy;
	//		m_pdlgAddOverlay->GetWindowRect(rc);
	//		theApp.m_pMainWnd->GetWindowRect(rcMain);
	//		rcCopy = rc;

	//		rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
	//		rcCopy.bottom = rcCopy.top + rc.Height();
	//		rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
	//		rcCopy.right = rcCopy.left + rc.Width();

	//		m_pdlgAddOverlay->MoveWindow(rcCopy);
	//	}
	//}
	//else
	//{
	//	if (m_pdlgAddOverlay)
	//	{
	//		m_pdlgAddOverlay->ShowWindow(bShow);
	//		m_pdlgAddOverlay->DestroyWindow();
	//		delete m_pdlgAddOverlay;
	//		m_pdlgAddOverlay = NULL;
	//		isAddOverlayDlgShow = false;
	//	}
	//}
}

void CCenterView::ShowLogViewer(BOOL bShow)
{
	if (bShow)
	{
		isLogViewerDlgShow = true;

		if (m_pdlgLogViewer == NULL)
		{
			m_pdlgLogViewer = new CLogViewer;
			m_pdlgLogViewer->Create(CLogViewer::IDD, this);
			m_pdlgLogViewer->ShowWindow(bShow);
			//초기 위치 잡기
			CRect rc, rcMain, rcCopy;
			m_pdlgLogViewer->GetWindowRect(rc);
			theApp.m_pMainWnd->GetWindowRect(rcMain);
			rcCopy = rc;

			rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
			rcCopy.bottom = rcCopy.top + rc.Height();
			rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
			rcCopy.right = rcCopy.left + rc.Width();

			m_pdlgLogViewer->MoveWindow(rcCopy);
		}
		else if (m_pdlgLogViewer)
		{
			m_pdlgLogViewer->DestroyWindow();
			delete m_pdlgLogViewer;
			m_pdlgLogViewer = NULL;

			m_pdlgLogViewer = new CLogViewer;
			m_pdlgLogViewer->Create(CLogViewer::IDD, this);
			m_pdlgLogViewer->ShowWindow(bShow);
			//초기 위치 잡기
			CRect rc, rcMain, rcCopy;
			m_pdlgLogViewer->GetWindowRect(rc);
			theApp.m_pMainWnd->GetWindowRect(rcMain);
			rcCopy = rc;

			rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
			rcCopy.bottom = rcCopy.top + rc.Height();
			rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
			rcCopy.right = rcCopy.left + rc.Width();

			m_pdlgLogViewer->MoveWindow(rcCopy);
		}
	}
	else
	{
		if (m_pdlgLogViewer)
		{
			m_pdlgLogViewer->ShowWindow(bShow);
			m_pdlgLogViewer->DestroyWindow();
			delete m_pdlgLogViewer;
			m_pdlgLogViewer = NULL;

			isLogViewerDlgShow = false;
		}
	}
}

void CCenterView::ShowManualRotateDlg(BOOL bShow)
{
	if (bShow)
	{
		if (m_pdlgManualRotateDlg == NULL)
		{
			m_pdlgManualRotateDlg = new CManualRotateDlg;
			m_pdlgManualRotateDlg->Create(CManualRotateDlg::IDD, this);
			m_pdlgManualRotateDlg->ShowWindow(bShow);
			//초기 위치 잡기
			CRect rc, rcMain, rcCopy;
			m_pdlgManualRotateDlg->GetWindowRect(rc);
			theApp.m_pMainWnd->GetWindowRect(rcMain);
			rcCopy = rc;

			rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
			rcCopy.bottom = rcCopy.top + rc.Height();
			rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
			rcCopy.right = rcCopy.left + rc.Width();

			m_pdlgManualRotateDlg->MoveWindow(rcCopy);
		}
		else if (m_pdlgManualRotateDlg)
		{
			m_pdlgManualRotateDlg->ShowWindow(bShow);
			m_pdlgManualRotateDlg->DestroyWindow();
			delete m_pdlgManualRotateDlg;
			m_pdlgManualRotateDlg = NULL;

			m_pdlgManualRotateDlg = new CManualRotateDlg;
			m_pdlgManualRotateDlg->Create(CManualRotateDlg::IDD, this);
			m_pdlgManualRotateDlg->ShowWindow(bShow);
			//초기 위치 잡기
			CRect rc, rcMain, rcCopy;
			m_pdlgManualRotateDlg->GetWindowRect(rc);
			theApp.m_pMainWnd->GetWindowRect(rcMain);
			rcCopy = rc;

			rcCopy.top = rcMain.top + rcMain.Height() / 2 - rc.Height() / 2;
			rcCopy.bottom = rcCopy.top + rc.Height();
			rcCopy.left = rcMain.left + rcMain.Width() / 2 - rc.Width() / 2;
			rcCopy.right = rcCopy.left + rc.Width();

			m_pdlgManualRotateDlg->MoveWindow(rcCopy);
		}
	}
	else
	{
		if (m_pdlgManualRotateDlg)
		{
			m_pdlgManualRotateDlg->ShowWindow(bShow);
			m_pdlgManualRotateDlg->DestroyWindow();
			delete m_pdlgManualRotateDlg;
			m_pdlgManualRotateDlg = NULL;
		}
	}
}

void CCenterView::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	//CFormView::PostNcDestroy();
}

#if 0	// Update Therad사용 Code 실제 화면이 그려지지 않음..
void CCenterView::DoStartUpdateTimer()
{
	KillfnUpdateThread();
	//그담에 복사 시작.

	m_hfnUpdateThExitEvt = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bfnUpdateContinue = TRUE;
	m_pfnUpdateThread = AfxBeginThread(fnUpdateThreadProc, (LPVOID)this);
}

void CCenterView::KillfnUpdateThread()
{
	DWORD dwExitCode;

	if (m_pfnUpdateThread != NULL && GetExitCodeThread(m_pfnUpdateThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
	{
		m_bfnUpdateContinue = FALSE;

		WaitForSingleObject(m_hfnUpdateThExitEvt, INFINITE);
		CloseHandle(m_hfnUpdateThExitEvt);
		m_hfnUpdateThExitEvt = NULL;
		m_pfnUpdateThread = NULL;
	}
}

UINT CCenterView::fnUpdateThreadProc(LPVOID pParam)
{
	CCenterView* pInst = (CCenterView*)pParam;

	while (pInst->m_bfnUpdateContinue)
	{
		if (!pInst->m_bMovingMainWnd && pInst->m_hrc && pInst->m_init)
		{
			//wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);
			//GLResize();
			switch (pInst->STITCHING_MODE)
			{
			case MODE_DRAW_SOURCE:
				pInst->draw_sources();
				break;
			case MODE_DRAW_OUTPUT:
				pInst->draw_output();
				break;
			case MODE_DRAW_INTERACTIVE:
				pInst->draw_interactive();
				break;
			}
			//wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

			((CT360VidStitchDlg*)theApp.m_pMainWnd)->UpdateFPS();

		}
		Sleep(15);
	}

	SetEvent(pInst->m_hfnUpdateThExitEvt);
	return 0;
}
#endif

BOOL CCenterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// An OpenGL window must be created with the following flags and must not
	// include CS_PARENTDC for the class style. Refer to SetPixelFormat
	// documentation in the "Comments" section for further information.
	if (cs.style & CS_PARENTDC)
		cs.style |= ~CS_PARENTDC;	
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CFormView::PreCreateWindow(cs);
}

void CCenterView::setOutputSize(int w, int h)
{
	m_output_width = w;
	m_output_height = h;
}

void CCenterView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch (STITCHING_MODE)
	{
	case MODE_DRAW_OUTPUT:
		if (m_init)
		{
			if (theApp.m_ScreenManager.m_pSubMode == SUBMODE_REGIONAL_WARPING)
			{
				if (m_isZoom == 1)
				{
					m_isZoom = 0;
					//QVS_Engine::QVS_ZoomOut(point.x, point.y, theApp.m_PrjAllCfg.m_nScrWidth, theApp.m_PrjAllCfg.m_nScrHeight);
					QVS_Engine::QVS_ResetToFullView();
				}
				else
				{
					m_isZoom = 1;
					QVS_Engine::QVS_ResetToFullView();
					QVS_Engine::QVS_ZoomIn(3, point.x, point.y);
				}
			}
		}
		break;
	}

	CFormView::OnRButtonDblClk(nFlags, point);
}

void CCenterView::writeLog()
{
	int size = 64 * sizeof(char);
	char* LogBuf = new char[size];
	char* temp;

	sprintf(LogBuf, "==============================");
	QVS_Engine::QVS_writeLog(LogBuf, size);

	sprintf(LogBuf, "[Application Version]");
	QVS_Engine::QVS_writeLog(LogBuf, size);

	sprintf(LogBuf, ">>>> version : %d.%d.%d", QVS_VERSION_RELEASE, QVS_VERSION_MAJOR, QVS_VERSION_MINOR);
	QVS_Engine::QVS_writeLog(LogBuf, size);

	QVS_Engine::QVS_writeLog("", 1);

#if (LICENSE_PERSONAL_KEY)
	QVS_Engine::QVS_setPlanType(theApp.m_ScreenManager.plan_type);

	sprintf(LogBuf, "[Project Plan Type]");
	QVS_Engine::QVS_writeLog(LogBuf, size);

	if (QVS_Engine::QVS_getPlanType() == QVS_PLANTYPE_DEMO)
	{
		sprintf(LogBuf, ">>>> plan type : Demo");
	}
	else if (QVS_Engine::QVS_getPlanType() == QVS_PLANTYPE_4K)
	{
		sprintf(LogBuf, ">>>> plan type : Plan4K");
	}
	else if (QVS_Engine::QVS_getPlanType() == QVS_PLANTYPE_8K)
	{
		sprintf(LogBuf, ">>>> plan type : Plan8K");
	}
	QVS_Engine::QVS_writeLog(LogBuf, size);

	if (QVS_Engine::QVS_getPlanType() != QVS_PLANTYPE_DEMO)
	{
		CNewLoadView* m_pNewLoadView = (CNewLoadView*)theApp.m_ScreenManager.m_pNewLoadView;
		if (m_pNewLoadView != NULL)
		{
			sprintf(LogBuf, ">>>> plan keyID : %d", m_pNewLoadView->HL_key_id);
			QVS_Engine::QVS_writeLog(LogBuf, size);

			sprintf(LogBuf, ">>>> plan due-date : %4d-%02d-%02d", m_pNewLoadView->HL_key_year, m_pNewLoadView->HL_key_month, m_pNewLoadView->HL_key_day);
			QVS_Engine::QVS_writeLog(LogBuf, size);
		}
	}
	
	QVS_Engine::QVS_writeLog("", 1);
#endif

	sprintf(LogBuf, "[Project Setting Configuration]");
	QVS_Engine::QVS_writeLog(LogBuf, size);

	//project type
	switch (theApp.m_PrjAllCfg.m_nStitchingMode)
	{
	case (int)QVS_Engine::StitchingMode_Stereo:
		sprintf(LogBuf, ">>>> project type : Stereo Top/Bottom");
		QVS_Engine::QVS_writeLog(LogBuf, size);
		break;
	case (int)QVS_Engine::StitchingMode_Stereo_Side:
		sprintf(LogBuf, ">>>> project type : Stereo Left/Right");
		QVS_Engine::QVS_writeLog(LogBuf, size);
		break;
	case (int)QVS_Engine::StitchingMode_StitchedInput:
		sprintf(LogBuf, ">>>> project type : Extern Input");
		QVS_Engine::QVS_writeLog(LogBuf, size);
		break;
	case (int)QVS_Engine::StitchingMode_Wide:
		sprintf(LogBuf, ">>>> project type : Wide Panorama");
		QVS_Engine::QVS_writeLog(LogBuf, size);
		break;
	default:
		sprintf(LogBuf, ">>>> project type : Q360");
		QVS_Engine::QVS_writeLog(LogBuf, size);
		break;
	}

	temp = CString2Char(theApp.m_PrjAllCfg.m_szProjectName);
	sprintf(LogBuf, ">>>> project name : %s ", temp);
	QVS_Engine::QVS_writeLog(LogBuf, size);

	sprintf(LogBuf, ">>>> project input size : %d x %d", theApp.m_PrjAllCfg.m_nWidth, theApp.m_PrjAllCfg.m_nHeight);
	QVS_Engine::QVS_writeLog(LogBuf, size);

	if (theApp.m_PrjAllCfg.m_nStitchingMode == (int)QVS_Engine::StitchingMode_Stereo)
		sprintf(LogBuf, ">>>> project output size : %d x %d", theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight*2);
	else if (theApp.m_PrjAllCfg.m_nStitchingMode == (int)QVS_Engine::StitchingMode_Stereo_Side)
		sprintf(LogBuf, ">>>> project output size : %d x %d", theApp.m_PrjAllCfg.m_nDstWidth*2, theApp.m_PrjAllCfg.m_nDstHeight);
	else
		sprintf(LogBuf, ">>>> project output size : %d x %d", theApp.m_PrjAllCfg.m_nDstWidth, theApp.m_PrjAllCfg.m_nDstHeight);
	QVS_Engine::QVS_writeLog(LogBuf, size);

	sprintf(LogBuf, ">>>> project num of camera : %d", theApp.m_PrjAllCfg.m_nMaxCamra);
	QVS_Engine::QVS_writeLog(LogBuf, size);

	sprintf(LogBuf, "==============================");
	QVS_Engine::QVS_writeLog(LogBuf, size);

	if (LogBuf)
	{
		delete LogBuf;
		LogBuf = NULL;
	}
}