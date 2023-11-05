#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
// CConfOutputsChildThree 대화 상자입니다.

class CConfOutputsChildThree : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfOutputsChildThree)

public:
	CConfOutputsChildThree(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfOutputsChildThree();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW1_3_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_btnSave;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnConfig13Save();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetValues();
	void GetValue();
	void SetFontObj();
	CComboBox m_cboBmDevice;
	CComboBox m_cboBmDisplay;
};
