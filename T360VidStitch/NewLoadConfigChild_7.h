#pragma once
#include "afxwin.h"
#include "NewLoadConfigChild_0.h"

// CNewLoadConfigChild_7 대화 상자입니다.

class CNewLoadConfigChild_7 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_7)

public:
	CNewLoadConfigChild_7(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewLoadConfigChild_7();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CNewLoadConfigChild_0* m_pNewLoadConfigChild_0;
	CComboBox m_cboNumOfCam7;
	CComboBox m_cboInputSize7;
	CComboBox m_cboOutputSize7;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void SetFontObj();
	void setCboData();
	void resetUI();

	afx_msg void OnCbnSelchangeComboNewloadNumberofcam7();
	afx_msg void OnCbnSelchangeComboNewloadInputsize7();
	afx_msg void OnCbnSelchangeComboNewloadOutputsize7();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
