#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// RightAdvanceAlignChildDlg 대화 상자입니다.

class RightAdvanceAlignChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightAdvanceAlignChildDlg)

public:
	RightAdvanceAlignChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~RightAdvanceAlignChildDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RIGHT_ADVANCE_ALIGN_CHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void SetFontObj();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnadvanceback();
	afx_msg void OnBnClickedBtnAdvanceAlignCalib();
	CGdipButton m_btnDoCalibration;
	CGdipButton m_btnBack;
	//step
	CSliderCtrl m_sldStep;
	CGdipButton	m_textStep;
	CEdit		m_editStep;

	void setEnableBtns(bool isEnable);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
