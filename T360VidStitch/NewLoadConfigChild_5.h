#pragma once
#include "afxwin.h"
#include "NewLoadConfigChild_0.h"


// CNewLoadConfigChild_5 ��ȭ �����Դϴ�.

class CNewLoadConfigChild_5 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_5)

public:
	CNewLoadConfigChild_5(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNewLoadConfigChild_5();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
