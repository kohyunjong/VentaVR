#pragma once
#include "afxwin.h"

#define LEFT_VIEW		01
#define RIGHT_VIEW		02

// CBottomManualChildDlg ��ȭ �����Դϴ�.

class CBottomManualChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBottomManualChildDlg)

public:
	CBottomManualChildDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBottomManualChildDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_BOTTOMMANUALCALIBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void OnShowBtn();
	void OnCulSelectedBtn();
	void NotSelectedBtn(int nType);

	CGdipButton m_btnL1, m_btnL2, m_btnL3, m_btnL4, m_btnL5, m_btnL6, m_btnL7, m_btnL8;
	CGdipButton m_btnR1, m_btnR2, m_btnR3, m_btnR4, m_btnR5, m_btnR6, m_btnR7, m_btnR8;
	afx_msg void OnBnClickedBtnLeft1();
	afx_msg void OnBnClickedBtnLeft2();
	afx_msg void OnBnClickedBtnLeft3();
	afx_msg void OnBnClickedBtnLeft4();
	afx_msg void OnBnClickedBtnLeft5();
	afx_msg void OnBnClickedBtnLeft6();
	afx_msg void OnBnClickedBtnLeft7();
	afx_msg void OnBnClickedBtnLeft8();
	afx_msg void OnBnClickedBtnRight1();
	afx_msg void OnBnClickedBtnRight2();
	afx_msg void OnBnClickedBtnRight3();
	afx_msg void OnBnClickedBtnRight4();
	afx_msg void OnBnClickedBtnRight5();
	afx_msg void OnBnClickedBtnRight6();
	afx_msg void OnBnClickedBtnRight7();
	afx_msg void OnBnClickedBtnRight8();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
