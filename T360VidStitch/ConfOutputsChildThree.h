#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
// CConfOutputsChildThree ��ȭ �����Դϴ�.

class CConfOutputsChildThree : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfOutputsChildThree)

public:
	CConfOutputsChildThree(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfOutputsChildThree();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW1_3_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
