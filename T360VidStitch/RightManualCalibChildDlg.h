#pragma once
#include "afxwin.h"

class CRightManualCalibChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRightManualCalibChildDlg)

public:
	CRightManualCalibChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRightManualCalibChildDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_RIGHT_MANUAL_CALIB_CHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CGdipButton m_btnMaskSave;
	CGdipButton m_btnClear;
	CGdipButton m_btnBack;
	CGdipButton m_btnRightRotate;
	CGdipButton m_btnLeftRotate;

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnManualMask();
	afx_msg void OnBnClickedBtnManualMaskClear();
	afx_msg void OnBnClickedBtnManualBack();
	afx_msg void OnBnClickedBtnManualMaskSave();
	afx_msg void OnCbnSelchangeRightManualComboLeft();
	afx_msg void OnCbnSelchangeRightManualComboRight();
	CComboBox m_comboMaskLetfList;
	CComboBox m_comboMaskRightList;
	afx_msg void OnCbnSelchangeRightManualComboColor();
	CComboBox m_comboMaskColor;
	//CButton m_btnLeftRotate;
	//CButton m_btnRightRotate;
	afx_msg void OnBnClickedBtnManualMaskLeftRotate();
	afx_msg void OnBnClickedBtnManualMaskRightRotate();
};

