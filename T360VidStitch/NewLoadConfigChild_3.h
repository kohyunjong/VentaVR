#pragma once
#include "afxwin.h"
#include "NewLoadConfigChild_0.h"


// CNewLoadConfigChild_3 대화 상자입니다.

class CNewLoadConfigChild_3 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_3)

public:
	CNewLoadConfigChild_3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewLoadConfigChild_3();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_3 };

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

	CComboBox m_cboNumOfCam3;
	CComboBox m_cboInputSize3;
	afx_msg void OnCbnSelchangeComboNewloadNumberofcam3();
	afx_msg void OnCbnSelchangeComboNewloadInputsize3();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CNewLoadConfigChild_0* m_pNewLoadConfigChild_0;
};
