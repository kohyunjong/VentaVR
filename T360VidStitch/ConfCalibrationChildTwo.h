#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
// CConfCalibrationChildTwo ��ȭ �����Դϴ�.

class CConfCalibrationChildTwo : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfCalibrationChildTwo)

public:
	CConfCalibrationChildTwo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfCalibrationChildTwo();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW3_2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
