#pragma once

#include "TaskPanelDlg.h"
#include "afxwin.h"
// CNewLoadConfigChild_0 대화 상자입니다.

class CNewLoadConfigChild_0 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_0)

public:
	CNewLoadConfigChild_0(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewLoadConfigChild_0();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CBitmapButton m_btnBrowse;
	CBitmapButton m_btnSave;
	CEdit m_txtPrjBasePath;
	CEdit m_txtPrjName;

	void SetFontObj();
	void resetUI();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnConfigBrow();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	bool m_bPrjNameOverlap;

	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
