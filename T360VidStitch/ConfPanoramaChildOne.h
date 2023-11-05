#pragma once
#include "afxwin.h"


// CConfPanoramaChildOne ��ȭ �����Դϴ�.

class CConfPanoramaChildOne : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfPanoramaChildOne)

public:
	CConfPanoramaChildOne(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfPanoramaChildOne();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW2_1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_btnSave;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedBtnConfig2Save();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void SetValues();
	void SetFontObj();
};
