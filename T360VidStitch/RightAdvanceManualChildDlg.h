#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// RightAdvanceManualChildDlg 대화 상자입니다.

class RightAdvanceManualChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightAdvanceManualChildDlg)

public:
	RightAdvanceManualChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~RightAdvanceManualChildDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RIGHT_ADVANCE_MANUAL_CHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CGdipButton m_btnDoWarp;
	CGdipButton m_btnEdit;
	CGdipButton m_btnDelete;
	CGdipButton m_btnUndo;
	CGdipButton m_btnRedo;
	CGdipButton m_btnBack;
	CGdipButton m_btnResetWarping;
	CGdipButton m_btnClearPoint;

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnAdvanceManualDoWarping();
	afx_msg void OnBnClickedBtnAdvanceManualEdit();
	afx_msg void OnBnClickedBtnAdvanceManualDelete();
	afx_msg void OnBnClickedBtnAdvanceManualUndo();
	afx_msg void OnBnClickedBtnAdvanceManualRedo();
	afx_msg void OnBnClickedBtnAdvanceManualBack();
	afx_msg void OnCbnSelchangeRightManualComboWarpingMode();
	afx_msg void OnCbnSelchangeRightManualComboAutoMode();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnAdvanceManualResetwarping();
	afx_msg void OnBnClickedBtnAdvanceManualClearpoint();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSliderCtrl m_sldSize;
	CEdit m_editSize;
	CSliderCtrl m_sldLength;
	CEdit m_editLength;
	CComboBox m_comboWarpingMode;
	CComboBox m_comboAutoMode;

	void refresh(bool state = true);
};
