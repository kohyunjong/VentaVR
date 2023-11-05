#pragma once

#include "CenterView.h"
#include "NewLoadView.h"
#include "ConfiguView.h"

#include "LeftChildDlg.h"
#include "RightSourceChildDlg.h"
#include "RightPanoraChildDlg.h"
#include "RightInteracChildDlg.h"
#include "RightCalibChildDlg.h"
#include "RightOverlayChildDlg.h"
#include "RightOverlayListChildDlg.h"
#include "RightAdvanceChildDlg.h"
#include "RightAdvanceAlignChildDlg.h"
#include "RightAdvanceUpdateChildDlg.h"
#include "RightAdvanceManualChildDlg.h"
#include "RightManualCalibChildDlg.h"
#include "BottomSourceChildDlg.h"
#include "BottomPanoChildDlg.h"
#include "BottomManualChildDlg.h"

class CScreenManager
{
public:
	CScreenManager();
	~CScreenManager();

	CWnd * m_pParent;
	
	CWnd * m_pParentLeftPane;
	CWnd * m_pParentRightPane;
	CWnd * m_pParentBottomPane;

	int m_iStartType; // -1 : None/  0 : Default PTS Load/  1 : Project Load  
	CString m_szCurrentPath;
	CRect m_rcDlg;
	CRect m_rcBtmDlg;
	
	
	// Center View
	CView* m_pCenterView;
	CView* m_pNewLoadView;
	CView* m_pConfigView;	

	//Left Pane
	CLeftChildDlg*			m_pLeftChild;
	//Right Pane
	CRightSourceChildDlg*		m_pRightSourceChild;
	CRightPanoraChildDlg*		m_pRightPanoramaChild;
	CRightInteracChildDlg*		m_pRightInteracChild;
	CRightCalibChildDlg*		m_pRightCalibChild;
	RightOverlayChildDlg*		m_pRightOverlayChild;
	CRightOverlayListChildDlg*	m_pRightOverlayListChild;
	RightAdvanceChildDlg*		m_pRightAdvanceChild;
	RightAdvanceAlignChildDlg*	m_pRightAdvanceAlignChild;
	RightAdvanceUpdateChildDlg*	m_pRightAdvanceUpdateChild;
	RightAdvanceManualChildDlg*	m_pRightAdvanceManualChild;
	CRightManualCalibChildDlg*	m_pRightManualCalibChild;
	//Bottom Pane
	CBottomSourceChildDlg*	m_pBottomSourceChild;
	CBottomPanoChildDlg*	m_pBottomPanoChild;
	CBottomManualChildDlg*	m_pBottomManualChild;

	vdstScreenCenter m_ActiveWnd;
	void SetParent(CWnd *pParent) { m_pParent = pParent; }
	void SetLeftParent(CWnd *pParent) { m_pParentLeftPane = pParent; }
	void SetRightParent(CWnd *pParent) { m_pParentRightPane = pParent; }
	void SetBottomParent(CWnd *pParent) { m_pParentBottomPane = pParent; }
	vdstScreenCenter getScreenMode() { return m_ActiveWnd; }

	void SetRCDlg(CRect rcDlg) { m_rcDlg = rcDlg; }
	CRect GetRCDlg() { return m_rcDlg; }

	void CreateAllScreen(vdstScrrenPosition scrPos);
	void CreateScreens(vdstScreenCenter screen, CWnd *pParent);
	void DeleteScreens();
	void DeleteAllScreen();
	void DeleteMainView();
	void ShowScreens(vdstScreenCenter screen, bool isOnSize = false, bool isFullScreen = false);
	void PreScreens(vdstScreenCenter screen);
	void ShowToast(vdstToastType nType, CString szText = _T(""), BOOL bShow = TRUE);
	void MoveToast();

	void updateSubMenu();


	int parsingPTS(CString path);
	int loadProject();
	int saveProject();
	void SetProjectName();
	void setEnableConfiurationBtn(BOOL bShow);

	void setEnableCalibrationBtn(BOOL bShow);

	vdstScreenCenter GetAcitveScreen() { return m_ActiveWnd; }
	void setCallbacks();

	BOOL m_bSnapShotDone;
	int  m_nSnapShotDoneCnt;
	void SetSnapShotDoneCnt(int nVal) { m_nSnapShotDoneCnt = nVal; m_bSnapShotDone = FALSE; }
	void SetSnapShotStatus(BOOL bVal) { m_bSnapShotDone = bVal; }
	BOOL GetSnapShotDone() { return m_bSnapShotDone;	}

	BOOL m_bGainManCurveDone;
	void SetGainManCurveStatus(BOOL bVal) { m_bGainManCurveDone = bVal; }
	BOOL GetGainManCurveDone() { return m_bGainManCurveDone; }

	BOOL m_bCalibrationDone;
	void SetCalibrationStatus(BOOL bVal) { m_bCalibrationDone = bVal; }
	BOOL GetCalibrationDone() { return m_bCalibrationDone; }

	BOOL m_bGainCompensationDone;
	void SetGainCompensationStatus(BOOL bVal) { m_bGainCompensationDone = bVal; }
	BOOL GetGainCompensationDone() { return m_bGainCompensationDone; }

	BOOL m_bseamModeDone;
	void SetseamModeStatus(BOOL bVal) { m_bseamModeDone = bVal; }
	BOOL GetseamModeDone() { return m_bseamModeDone; }

	BOOL m_bautoGainModeDone;
	void SetautoGainModeStatus(BOOL bVal) { m_bautoGainModeDone = bVal; }
	BOOL GetautoGainModeDone() { return m_bautoGainModeDone; }

	int m_nEditOutputStatus;		// 0 : nothing, -1: Fail, 1: Done
	void SetEditOutputStatus(int nVal) { m_nEditOutputStatus = nVal; }
	int GetEditOutputStatus() { return m_nEditOutputStatus;  }

	int m_nNCalibrationStatus;		// 0 : nothing, -1: Fail, 1: Done
	void SetnewCalibrationStatus(int nVal){ m_nNCalibrationStatus = nVal; }
	int GetnewCalibrationStatus() { return m_nNCalibrationStatus; }

	//170411
	int m_pSubMode;
	void setSubMode(int mode){ m_pSubMode = mode; }
	int getSubMode() { return m_pSubMode; }

	bool isDoingUpdateWarping = false;
	bool failCalibration = false;
	bool isDoingProgToast = false;
	bool isNewProject;

	CString m_TextOverlay;
	CString m_TextOverlayFont;
	FontStyle m_TextOverlayFontStyle;
	Color m_TextOverlayTextColor;
	Color m_TextOverlayTextBgColor;

	int plan_type = QVS_PLANTYPE_DEMO;

	bool updateOverlayTemplateShow = FALSE;
	int mCulTemplateNum = -1;
};

void onCallbackDone_snapshot(int msg);
void onCallbackDone_doManualGainMode(int msg);
void onCallbackDone_quramCalibMode(int msg);
void onCallbackDone_gainMode(int msg);
void onCallbackDone_seamMode(int msg);
void onCallbackDone_autoGainMode(int msg);
void onCallback_editOutput(int msg);
void onCallback_calibration(int msg);
void onCallback_makeWarpmap(int msg);
void onCallback_history(int msg);
void onCallback_audioError(int msg);
void onCallback_vodStreamingFinished();
void onCallback_load(int msg);

char* CString2Char(CString szString);
CString Char2CString(char* szChar);