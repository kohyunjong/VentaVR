#pragma once
#include "afxwin.h"


// CConfPanoramaChildTwo 대화 상자입니다.

class CConfPanoramaChildTwo : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfPanoramaChildTwo)

public:
	CConfPanoramaChildTwo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfPanoramaChildTwo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW2_2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
