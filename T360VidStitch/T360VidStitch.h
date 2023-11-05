
// SubFormView.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "ScreenManager.h"
#include "ProjAllConfig.h"
#include "T360VidStitchDlg.h"
// CT360VidStitch:
// See T360VidStitch.cpp for the implementation of this class
//

class CT360VidStitchApp : /*public CLogWriter,*/ public CWinApp
{
public:
	CT360VidStitchApp();
private:
	HANDLE m_hMutex;				// 중복실행 방지를 위한 뮤텍스 핸들

	int _style;
	CRect _originWindow;
// Overrides
public:
	virtual BOOL InitInstance();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
// Implementation

	DECLARE_MESSAGE_MAP()

public:
	ULONG_PTR m_gdiplusToken;
	CScreenManager m_ScreenManager;
	CProjAllConfig m_PrjAllCfg;

	CString GetAppDir();
	void OnLogRequest(int nLevel, char* szName, char* szMsg);
	void OnInitProjertConfig();

	virtual int ExitInstance();
	void addToRecentFile(CString szPath);
	CString getRecentFile(int index);
	void RemoveRecentFile(int index);
	void updateRecentFile();
	int GetRecentListSize();
	void LoadStdProfileSettings(UINT nMaxMRU);

};


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	CAboutDlg(int delay);

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
private:
	CBitmap m_bmpAboutBG; // About 배경
	int m_nAboutWidth;
	int m_nAboutHeight;
	int m_delay;

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

extern CT360VidStitchApp theApp;

BOOL PngLoadFromResource(CImage& img, LPCTSTR pName, LPCTSTR pType, HMODULE hInst=NULL);