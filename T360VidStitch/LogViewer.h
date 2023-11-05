#pragma once
#include "afxwin.h"


// CLogViewer ��ȭ �����Դϴ�.

class CLogViewer : public CDialogEx
{
	DECLARE_DYNAMIC(CLogViewer)

public:
	CLogViewer(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLogViewer();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LOGVIEWERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
