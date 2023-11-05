
// T360VidStitchDlg.h : header file
//

#pragma once

#include "CenterView.h"
#include "NewLoadView.h"
#include "PaneLeftDlg.h"
#include "PaneRightDlg.h"
#include "PaneBottomDlg.h"

#define IsCTRLPressed() ( 0x8000 ==(GetKeyState(VK_CONTROL) & 0x8000))


// CT360VidStitchDlg dialog
class CT360VidStitchDlg : public CDialogEx
{
	// Construction
public:
	CT360VidStitchDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_T360VIDSTITCH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CXTPDockingPaneManager	m_paneManager;
	CXTPDockingPane*		m_pActivePane;		//나중에 확인 하장.KBJ 161213

public:
	CPaneLeftDlg			m_dlgPaneLeft;
	CPaneRightDlg			m_dlgPaneRight;
	CPaneBottomDlg			m_dlgPaneBottom;

	CStatic m_wndCenterViewLabel;
	CStatic m_txtFps;

	CBitmapButton m_btnCurView;
	CBitmapButton m_btnViewClose;

	CStatic m_wndCenterViewFrame;

	CFont * GetFontObj(int index);

	bool isFullScreen = false;
private:
	CFont m_fontArial_10;	
	CFont m_fontArial_10R;	//우측정렬
	CFont m_fontArial_T11;
	CFont m_fontArial_11;
	CFont m_fontArial_12;
	CFont m_fontArial_B12;
	CFont m_fontArial_B13;

	int _style;
	CRect _originWindow;
	

	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);

	void UpdateFPS();
	void ToggleIcon(int nIndex);
	void EnableTitle(int state);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnviewclose();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CreateFontObj();
	void DestoryFontObj();
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	void Wait(double dwMillisecond);

	RingBuf* fps;
	CString m_strFPS;

	afx_msg LRESULT OnUpdateFPS(WPARAM wParam, LPARAM lParam);

	void onKeyEvent(MSG* pMsg);		// for Keyboard Event from T360VidStitch App
};