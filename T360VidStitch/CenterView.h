#pragma once
#include "TransparentDlgType.h"
#include "SourceEditRect.h"
#include "EditInputDlg.h"
#include "EditInputStereoDlg.h"
#include "EditCameraParamDlg.h"
#include "AddOverlayParentDlg.h"
#include "LogViewer.h"
#include "ManualRotateDlg.h"
// CCenterView form view
#define ANIMATION_MODE_NONE		0x00
#define ANIMATION_MODE_ROTATE	0x01

#define ANIMATION_SPEED_UP		0x02
#define ANIMATION_SPEED_DOWN	0x03

#define ANIMATION_TIMER			3000 

// FPS 측정을 위한 struct (최근 입력된 k개의 FPS의 평균으로 FPS 계산)
// push : FPS 입력
// avg : 평균 FPS 출력
/* 선언 부분 */

struct RingBuf {
	float* v;
	int cur, last, K;
	RingBuf(int k) :cur(0), last(0), K(k)
	{
		v = new float[K];
	}

	void push(float x)
	{
		v[cur] = x; 
		cur = (cur + 1) % K; 
		last++; 
		last = last > K ? K : last;
	}

	float avg()
	{
		float sum = 0; 
		for (int i = 0; i < last; i++) 
			sum += v[i]; 
		return sum / (float)last;
	}
};

class CCenterView : public CFormView
{
	DECLARE_DYNCREATE(CCenterView)

protected:
	CCenterView();           // protected constructor used by dynamic creation
	virtual ~CCenterView();
	friend class CT360VidStitchDlg;
public:
	enum { IDD = IDD_CENTERSCREENVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL		m_bMovingMainWnd;
	CPalette    m_cPalette;
	CPalette    *m_pOldPalette;
	CRect       m_oldRect;
	float       m_fRadius;
	CClientDC   *m_pDC;


	char* PTS_PATH = "0712.pts";//pts file for sample image
	char* Q360_PATH = "161208.q360";//q360 file for sample image

	bool templateLoad = FALSE;
	BOOL m_play;
	bool m_init = FALSE;

	bool isSetEvent = false;
	bool isOverlayEdit = false;
	bool isAddOverlayDlgShow = false;
	bool isLogViewerDlgShow = false;

	int overlay_edit_mode;

	int overlay_id = 0;
	bool is_overlay_modifi = false;
	bool is_overlay_selected = false;
	bool is_overlay_roll = false;
	bool is_overlay_move = false;

	int lastX;
	int lastY;

	/*Quram Video360 API Example end*/


	// Operations
	int STITCHING_MODE;

	//Output Size
	int m_output_width = 0, m_output_height = 0;
	bool isFullScreen = false;

	int interactive_animation_mode = ANIMATION_MODE_NONE;
	float animation_speed = 0;
	double anim_timer = 0;

public:
	void Init();
	void CreateRGBPalette(void);
	BOOL bSetupPixelFormat(void);
	BOOL isPlay() { return m_play; }
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);


	/*Quram Video360 API example*/
	void initQVSEngine();
	void GLResize();
	void draw_output();
	void draw_sources();
	void draw_manual_calib();
	void draw_interactive();
	int parsingPTS(char* path);
	int loadParam(char* path);
	int saveParam(char* path);
	BOOL IsNeedSaveParam(char* path);
	/*Quram Video360 API example end*/

	void onKeyUP(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	void OnPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetViewMode(int nMode, bool isFull = false) { STITCHING_MODE = nMode; isFullScreen = isFull; }
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
	void setOutputSize(int w, int h);

	//Toast Dialog
	CTransparentDlgType m_dlgToastHdd;
	CTransparentDlgType m_dlgToastStream;
	CTransparentDlgType m_dlgToastOutput;
	CTransparentDlgType m_dlgToastDone;
	CTransparentDlgType m_dlgToastProgress;
	CTransparentDlgType m_dlgToastWarping;
	CTransparentDlgType m_dlgToastFail;

	//OpenGL DC
	HGLRC       m_hrc;

	//Mouse Event
	int m_isLeft; //1 : left, 0 : right
	int m_isMLSelectClicked;
	int m_isMLClicked;
	int m_isMRClicked;
	int m_isZoom;
	int m_DrawSize;
	int m_DrawManualMaskSize;
	CPoint m_OldPoint;

	//Source Edit
	CRect m_rcSourceRect[MAX_SOURCE];
	int m_subWinWidth;
	int m_subWinHeight;
	int m_selSubViewIDX;
	BOOL m_bCapConfActive;
	CPoint	m_SourceEditRectPoint;
	CSourceEditRect m_SourceEditRectWnd;
	int m_bFirstTime;
	bool m_bControlDown;

	CEditInputDlg* m_pdlgEditInput;
	CEditInputStereoDlg* m_pdlgEditInputStereo;
	EditCameraParamDlg* m_pdlgEditCameraParam;
	CAddOverlayParentDlg* m_pdlgAddOverlay;
	CLogViewer* m_pdlgLogViewer;
	CManualRotateDlg* m_pdlgManualRotateDlg;

	int mTargetTextOverlayID = -1;
	int mOverlayID = 0;

	void AcitveCapConf(BOOL bVal) { m_bCapConfActive = bVal; }
	void ShowToastDlg(vdstToastType nType, CString szText, BOOL bShow = TRUE);
	void MoveToast();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void Arrange();
	void showEditInputDlg(BOOL bShow);
	void ShowEditInput(BOOL bShow);
	void ShowEditInputStereo(BOOL bShow);
	void ShowEditCameraParam(BOOL bShow);
	void ShowAddOverlay(BOOL bShow);
	void ShowLogViewer(BOOL bShow);
	void ShowManualRotateDlg(BOOL bShow);
	virtual void PostNcDestroy();
	afx_msg LRESULT OnCloseEditInput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloseEditInputStereo(WPARAM wParam, LPARAM lParam);
	RingBuf* fps;

//	void CalcToastWndPos(CTransparentDlgType* pTDlg, int nWndCnt = 0);
#if 0	// Update Therad사용 Code 실제 화면이 그려지지 않음..
// Screen update thread...
	CWinThread* m_pfnUpdateThread;
	HANDLE		m_hfnUpdateThExitEvt;
	BOOL		m_bfnUpdateContinue;
	void		DoStartUpdateTimer();
	void		KillfnUpdateThread();
protected:
	static UINT	fnUpdateThreadProc(LPVOID pParam);
#endif
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	void writeLog();
};



extern unsigned char threeto8[8];
extern unsigned char twoto8[4];
extern unsigned char oneto8[2];
