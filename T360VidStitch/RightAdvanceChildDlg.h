#pragma once
#include "afxwin.h"


// RightAdvanceChildDlg ��ȭ �����Դϴ�.

class RightAdvanceChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightAdvanceChildDlg)

public:
	RightAdvanceChildDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~RightAdvanceChildDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RIGHT_ADVANCE_CHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
