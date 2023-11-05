#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// RightAdvanceUpdateChildDlg 대화 상자입니다.

class RightAdvanceUpdateChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightAdvanceUpdateChildDlg)

public:
	RightAdvanceUpdateChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~RightAdvanceUpdateChildDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RIGHT_ADVANCE_UPDATE_CHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
