#pragma once
#include "afxwin.h"
#include "NewLoadConfigChild_0.h"


// CNewLoadConfigChild_2 대화 상자입니다.

class CNewLoadConfigChild_2 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_2)

public:
	CNewLoadConfigChild_2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewLoadConfigChild_2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_2 };

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

	CComboBox m_cboNumOfCam2;
	CComboBox m_cboInputSize2;
	CComboBox m_cboOutputSize2;
	afx_msg void OnCbnSelchangeComboNewloadNumberofcam2();
	afx_msg void OnCbnSelchangeComboNewloadInputsize2();
	afx_msg void OnCbnSelchangeComboNewloadOutputsize2();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CNewLoadConfigChild_0* m_pNewLoadConfigChild_0;
};
