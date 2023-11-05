#pragma once
#include "afxwin.h"
#include "NewLoadConfigChild_0.h"


// CNewLoadConfigChild_4 ��ȭ �����Դϴ�.

class CNewLoadConfigChild_4 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_4)

public:
	CNewLoadConfigChild_4(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNewLoadConfigChild_4();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_4 };

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

	CComboBox m_cboNumOfCam4;
	CComboBox m_cboInputSize4;
	CComboBox m_cboOutputSize4;
	afx_msg void OnCbnSelchangeComboNewloadNumberofcam4();
	afx_msg void OnCbnSelchangeComboNewloadInputsize4();
	afx_msg void OnCbnSelchangeComboNewloadOutputsize4();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CNewLoadConfigChild_0* m_pNewLoadConfigChild_0;
};
