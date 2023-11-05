#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
#include "afxwin.h"


// CConfOutputsChildFour ��ȭ �����Դϴ�.

class CConfOutputsChildFour : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfOutputsChildFour)

public:
	CConfOutputsChildFour(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfOutputsChildFour();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW1_4_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
