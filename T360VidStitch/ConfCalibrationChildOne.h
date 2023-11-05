#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
// CConfCalibrationChildOne 대화 상자입니다.

class CConfCalibrationChildOne : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfCalibrationChildOne)

public:
	CConfCalibrationChildOne(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfCalibrationChildOne();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW3_1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_btnCreate;
	afx_msg void OnBnClickedBtnConfigCreate();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetValues();
	void GetValue();
	void SetFontObj();
};
