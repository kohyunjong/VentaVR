// FormView1.cpp : implementation file
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "FormView1.h"
#include "CenterView.h"



#define GL_TEST

// CFormView1
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

IMPLEMENT_DYNCREATE(CFormView1, CFormView)

CFormView1::CFormView1()
	: CFormView(CFormView1::IDD)
{
	m_pDC = NULL;
	m_pOldPalette = NULL;
	STITCHING_MODE = MODE_DRAW_SOURCE;
	m_play = TRUE;
	m_hrc = NULL;
}

CFormView1::~CFormView1()
{
}

void CFormView1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormView1, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CFormView1 diagnostics

#ifdef _DEBUG
void CFormView1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormView1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormView1 message handlers
int CFormView1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	Init();
	return 0;
}
void CFormView1::OnDraw(CDC* /*pDC*/)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (m_play && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
	{
		draw_output();
	}
}
/////////////////////////////////////////////////////////////////////////////
// GL helper functions
void CFormView1::Init()
{
	PIXELFORMATDESCRIPTOR pfd;
	int         n;
	

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;
#ifndef GL_TEST
	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	CreateRGBPalette();
#endif
	m_hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);
	wglMakeCurrent(NULL, NULL);
	GetClientRect(&m_oldRect);
}

void CFormView1::GLRenderSceneTest()
{
	glLoadIdentity();	//행렬을 초기화
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//현재 버퍼를 비워줌
	glColor3f(1.0f,1.0f,1.0f);	// 버텍스의 색깔을 흰색으로 셋팅
	glEnable(GL_DEPTH_TEST);	// 깊이 테스트 On

	gluLookAt(0.0f, 0.0f, 1000.0f, 0.0f, 0.f, 0.0f, 0.0f, 1.0f, 0.0f);
	//카메라의 위치는 (0,0,1000), 카메라가 바라보는곳 (0, 10, 0), 카메라 up vector (0, 1, 0);

	glBegin(GL_QUADS);
	glVertex3f(-50, -50, 0);
	glVertex3f(50, -50, 0);
	glVertex3f(50, 50, 0);
	glVertex3f(-50, 50, 0);
	glEnd();

	glFlush();;
}


void CFormView1::draw_output()
{
	CRect rc;
	if (m_hrc)
	{
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);
		GetClientRect(rc);
		GLResize(rc.right, rc.bottom);
#ifdef GL_TEST
		GLRenderSceneTest();
#else
		int err;

		if (rc.Height() > 0)
			err = QVS_Engine::QVS_Stitching(rc.Width(), rc.Height(), PANORAMA_RREVIEW_FULL);
		else
			err = QVS_Engine::QVS_Stitching(theApp.m_PrjAllCfg.m_nWidth, theApp.m_PrjAllCfg.m_nHeight, PANORAMA_RREVIEW_FULL);
#endif
		SwapBuffers(m_pDC->GetSafeHdc());

		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

}

BOOL CFormView1::bSetupPixelFormat()
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
		MessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}

	return TRUE;
}

unsigned char CFormView1::ComponentFromIndex(int i, UINT nbits, UINT shift)
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
void CFormView1::CreateRGBPalette()
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

void CFormView1::GLResize(int cx, int cy)
{
#ifndef GL_TEST
	if (cy == 0) cy = 1;

	glViewport(0, 0, cx, cy);

	if ((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
		RedrawWindow();

	m_oldRect.right = cx;
	m_oldRect.bottom = cy;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLdouble)cx / cy, 3.0f, 7.0f);
	glMatrixMode(GL_MODELVIEW);
#else
	GLfloat fAspect;

	if (cy == 0)
		cy = 1;

	glViewport(0, 0, cx, cy);				//함수를 사용하여, 모니터 전체 화면을 뷰포트 영역으로 잡아 준다.

	fAspect = (GLfloat)cx / (GLfloat)cy;

	glMatrixMode(GL_PROJECTION);			// 원근 투영 임을 선언
	glLoadIdentity();

	gluPerspective(60.0f, fAspect, 1.0f, 10000.0f);		//관측 공간을 설정

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif
}


void CFormView1::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (cy > 0)
	{
		VERIFY(wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc));
		GLResize(cx, cy);
		VERIFY(wglMakeCurrent(NULL, NULL));
	}

}
void CFormView1::OnTimer(UINT_PTR nIDEvent)
{
	if (IsWindowEnabled() && m_play && theApp.m_PrjAllCfg.m_bInitQVS_EngineDone)
	{
		draw_output();
	}
	CFormView::OnTimer(nIDEvent);

	// Eat spurious WM_TIMER messages
	//MSG msg;
	//while (::PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE));

}
void CFormView1::OnDestroy()
{
	HGLRC   hrc = NULL;

	m_play = FALSE;

	//KillTimer(TIMER_STARTSCREENGL);


	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL,  NULL);

	if (hrc)
		::wglDeleteContext(hrc);

	if (m_pOldPalette)
		m_pDC->SelectPalette(m_pOldPalette, FALSE);

	if (m_pDC)
		delete m_pDC;

	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
void CFormView1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}


void CFormView1::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
// 	if (bShow)
// 		SetTimer(TIMER_STARTSCREENGL, 60, NULL);
// 	else
// 		KillTimer(TIMER_STARTSCREENGL);
}
