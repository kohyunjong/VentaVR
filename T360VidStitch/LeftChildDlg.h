#pragma once
#include "afxwin.h"


// CLeftChildDlg 대화 상자입니다.

class CLeftChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLeftChildDlg)

public:
	CLeftChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLeftChildDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LEFTCHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	
	afx_msg void OnBnClickedBtnpanorama();
	afx_msg void OnBnClickedBtninteractive();
	afx_msg void OnBnClickedBtnconfigu();
	afx_msg void OnBnClickedBtnsource();
	afx_msg void OnBnClickedBtnlogview();
	afx_msg void OnBnClickedBtnpreparemode();

	CGdipButton m_btnPanorama;
	CGdipButton m_btnInteractive;
	CGdipButton m_btnConfiguration;
	CGdipButton m_btnSource;
	CGdipButton m_btnLogView;
	CGdipButton m_btnConfiguration2;
	CGdipButton m_btnPrepareMode;

	void btnEnable(bool enable);
	void ToggleButton(int nType);
	afx_msg void OnBnClickedBtnstereoPair0();
	afx_msg void OnBnClickedBtnstereoPair1();
	afx_msg void OnBnClickedBtnstereoPair2();
	
	CGdipButton m_btnPair0;
	CGdipButton m_btnPair1;
	CGdipButton m_btnPair2;
	CGdipButton m_btnPair3;
	void setStereoPairUpdate();
	afx_msg void OnBnClickedBtnstereoPair3();
};
