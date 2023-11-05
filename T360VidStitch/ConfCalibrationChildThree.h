#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
// CConfCalibrationChildThree ��ȭ �����Դϴ�.

class CConfCalibrationChildThree : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfCalibrationChildThree)

public:
	CConfCalibrationChildThree(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfCalibrationChildThree();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW3_3_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
