#pragma once
#include "afxwin.h"


// RightAdvanceChildDlg 대화 상자입니다.

class RightAdvanceChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightAdvanceChildDlg)

public:
	RightAdvanceChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~RightAdvanceChildDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RIGHT_ADVANCE_CHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CGdipButton m_btnAdvanceAlign;
	CGdipButton m_btnAdvanceUpdate;
	CGdipButton m_btnAdvanceManual;
	CGdipButton m_btnBack;
	afx_msg void OnBnClickedBtnAdvanceAlign();
	afx_msg void OnBnClickedBtnAdvanceUpdate();
	afx_msg void OnBnClickedBtnAdvanceManual();
	afx_msg void OnBnClickedBtnAdvanceBack();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void refresh();
};
