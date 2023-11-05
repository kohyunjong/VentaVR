#pragma once
#include "afxwin.h"
#include "NewLoadConfigChild_0.h"


// CNewLoadConfigChild_6 ��ȭ �����Դϴ�.

class CNewLoadConfigChild_6 : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CNewLoadConfigChild_6)

public:
	CNewLoadConfigChild_6(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNewLoadConfigChild_6();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD_6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void SetFontObj();
	void setCboData();
	void setEditData();
	void getEditData(int &nWidth, int &nHeight);
	void resetUI();
	void setWideMode(bool isSetting);

	bool isSettingWide = false;

	CComboBox m_cboNumOfCam6;
	CComboBox m_cboInputSize6;
	CEdit m_editOutputSizeW6;
	CEdit m_editOutputSizeH6;
	afx_msg void OnCbnSelchangeComboNewloadNumberofcam6();
	afx_msg void OnCbnSelchangeComboNewloadInputsize6();
	afx_msg void OnEnChangeEdtNewloadWidth6();
	afx_msg void OnEnChangeEdtNewloadHeight6();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CNewLoadConfigChild_0* m_pNewLoadConfigChild_0;
};
