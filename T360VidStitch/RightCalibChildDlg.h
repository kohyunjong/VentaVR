#pragma once
#include "afxwin.h"
class CRightCalibChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRightCalibChildDlg)

public:
	CRightCalibChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRightCalibChildDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_RIGHTCALIBCHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CGdipButton m_btnAlign;
	CGdipButton m_btnManualMask;
	CGdipButton m_btnDetailUpdate;
	CGdipButton m_btnAdvance;
	CGdipButton m_btnBack;
	CGdipButton m_btnSnapshot;
	CGdipButton m_btnCalibration;
	CGdipButton m_btnUndo; 
	CGdipButton m_btnRedo;
	CGdipButton m_btnEditOff;
	CGdipButton m_btnEditOn;
	CGdipButton m_btnStraighten;
	CGdipButton m_btnSmartCalibration;
	CGdipButton m_btnManualRotate;

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedBtnaligncalibration();
	afx_msg void OnBnClickedBtnfastcalibration();
	afx_msg void OnBnClickedBtndetailcalibration();
	afx_msg void OnBnClickedBtnadvance();
	afx_msg void OnBnClickedBtnback();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnundocalibration();
	afx_msg void OnBnClickedBtnredocalibration();
	afx_msg void OnBnClickedBtnediton();
	afx_msg void OnBnClickedBtneditoff();
	afx_msg void OnBnClickedBtnstraighten();
	afx_msg void OnBnClickedBtnMask();
	
	afx_msg void OnBnClickedBtnManualMask();

	void refresh(bool state = true);
	void disableCalibrationBtns();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnsmartcalibration();
	afx_msg void OnBnClickedBtnManualRotate();
	afx_msg void OnPaint();
};

