#pragma once
#include "afxwin.h"
#include "NewLoadConfigChild_0.h"


// CNewLoadConfigChild_5 대화 상자입니다.

class CNewLoadConfigChild_5 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_5)

public:
	CNewLoadConfigChild_5(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewLoadConfigChild_5();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void SetFontObj();
	void setCboData();
	void resetUI();

	CComboBox m_cboNumOfCam5;
	CComboBox m_cboInputSize5;
	CComboBox m_cboOutputSize5;
	afx_msg void OnCbnSelchangeComboNewloadNumberofcam5();
	afx_msg void OnCbnSelchangeComboNewloadInputsize5();
	afx_msg void OnCbnSelchangeComboNewloadOutputsize5();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CNewLoadConfigChild_0* m_pNewLoadConfigChild_0;
};
