#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
// CConfCalibrationChildOne ��ȭ �����Դϴ�.

class CConfCalibrationChildOne : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfCalibrationChildOne)

public:
	CConfCalibrationChildOne(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfCalibrationChildOne();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW3_1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
