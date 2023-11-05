#pragma once
#include "afxwin.h"
#include "NewLoadConfigChild_0.h"

// CNewLoadConfigChild_7 ��ȭ �����Դϴ�.

class CNewLoadConfigChild_7 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_7)

public:
	CNewLoadConfigChild_7(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNewLoadConfigChild_7();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
