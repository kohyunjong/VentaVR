#pragma once
#include "afxwin.h"


// CLogViewer 대화 상자입니다.

class CLogViewer : public CDialogEx
{
	DECLARE_DYNAMIC(CLogViewer)

public:
	CLogViewer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogViewer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGVIEWERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	static UINT	TimerfnThreadProc(LPVOID pParam);

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_editLogViewer;
	CString m_culLog;
	CString m_preLog = _T("");
	bool isFirst;
	CGdipButton m_btnSaveFile;

	void SetFontObj();
	void LogUpdate();

	CWinThread* m_pfnTimerThread;
	HANDLE		m_hTimerfnThExitEvt;
	BOOL		m_bTimerfnContinue = FALSE;
	void		DoStartATimer();
	void		KillTimerfnThread();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnSavefile();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
