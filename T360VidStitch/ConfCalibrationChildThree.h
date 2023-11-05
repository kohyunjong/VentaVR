#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
// CConfCalibrationChildThree 대화 상자입니다.

class CConfCalibrationChildThree : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfCalibrationChildThree)

public:
	CConfCalibrationChildThree(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfCalibrationChildThree();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW3_3_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_btnSave;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnConfig3Apply();
	CBitmapButton m_btnBrow;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetValues();
	afx_msg void OnBnClickedBtnConfig33Brow();
	void GetValue();
	void SetFontObj();
};
