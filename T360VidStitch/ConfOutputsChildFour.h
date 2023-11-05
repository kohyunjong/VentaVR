#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
#include "afxwin.h"


// CConfOutputsChildFour 대화 상자입니다.

class CConfOutputsChildFour : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfOutputsChildFour)

public:
	CConfOutputsChildFour(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfOutputsChildFour();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW1_4_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void SetFontObj();
	void SetData();

	CBitmapButton m_btnSave;
	CBitmapButton m_btnBrowse;
	CComboBox m_cboRepeat;

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnConfig14Brow();
	afx_msg void OnBnClickedBtnConfig14Save();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
