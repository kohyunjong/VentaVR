#pragma once
#include "afxwin.h"


// CConfPanoramaChildTwo ��ȭ �����Դϴ�.

class CConfPanoramaChildTwo : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfPanoramaChildTwo)

public:
	CConfPanoramaChildTwo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfPanoramaChildTwo();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW2_2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_btnSave;
	CString FPS;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedBtnConfig22Save();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void SetValues();
	void SetFontObj();
	double OnChangeStrToDouble(CString strValue);
	afx_msg void OnEnChangeCfpano2Editrender();
};
