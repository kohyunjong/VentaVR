#pragma once
#include "afxwin.h"
#include "NewLoadConfigChild_0.h"

// CNewLoadConfigChild_1 ��ȭ �����Դϴ�.

class CNewLoadConfigChild_1 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_1)

public:
	CNewLoadConfigChild_1(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNewLoadConfigChild_1();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_1 };

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

	CComboBox m_cboNumOfCam1;
	CComboBox m_cboInputSize1;
	CComboBox m_cboOutputSize1;
	afx_msg void OnCbnSelchangeComboNewloadNumberofcam1();
	afx_msg void OnCbnSelchangeComboNewloadInputsize1();
	afx_msg void OnCbnSelchangeComboNewloadOutputsize1();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CNewLoadConfigChild_0* m_pNewLoadConfigChild_0;
};
