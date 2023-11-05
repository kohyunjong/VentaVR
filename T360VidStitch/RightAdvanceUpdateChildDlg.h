#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// RightAdvanceUpdateChildDlg ��ȭ �����Դϴ�.

class RightAdvanceUpdateChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightAdvanceUpdateChildDlg)

public:
	RightAdvanceUpdateChildDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~RightAdvanceUpdateChildDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RIGHT_ADVANCE_UPDATE_CHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CGdipButton m_btnRoiClearAll;
	CGdipButton m_btnSelectAll;
	CGdipButton m_btnDoCalibration;
	CGdipButton m_btnBack;
	CSliderCtrl m_sldStep;
	CEdit		m_editStep;
	CGdipButton m_textStep;
	afx_msg void OnBnClickedBtnAdvanceUpdateRoiClearAll();
	afx_msg void OnBnClickedBtnAdvanceUpdateSelectAll();
	afx_msg void OnBnClickedBtnAdvanceUpdateCalib();
	
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void SetFontObj();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnAdvanceUpdateBack();

	void setEnableBtns(bool isEnable);
	void isSelectAllCheck();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
