#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
// CConfCalibrationChildTwo 대화 상자입니다.

class CConfCalibrationChildTwo : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfCalibrationChildTwo)

public:
	CConfCalibrationChildTwo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfCalibrationChildTwo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW3_2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_btnApply;
	CBitmapButton m_btnBrow;

	afx_msg void OnBnClickedBtnConfig2Apply();
	afx_msg void OnBnClickedBtnConfig32Brow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual BOOL OnInitDialog();
	void SetValues();
	void GetValue();
	void SetFontObj();
	void clearValue();
};
