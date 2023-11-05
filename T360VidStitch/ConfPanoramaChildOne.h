#pragma once
#include "afxwin.h"


// CConfPanoramaChildOne 대화 상자입니다.

class CConfPanoramaChildOne : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfPanoramaChildOne)

public:
	CConfPanoramaChildOne(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfPanoramaChildOne();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW2_1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
