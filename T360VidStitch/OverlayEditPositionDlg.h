#pragma once
#include "afxwin.h"


// COverlayEditPositionDlg 대화 상자입니다.

class COverlayEditPositionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COverlayEditPositionDlg)

public:
	COverlayEditPositionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COverlayEditPositionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OVERLAY_EDITPOSITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CGdipButton m_btnApply;
	int id;

	void SetValues(int overlayid);
	void SetFontObj();
	double OnChangeStrToDouble(CString strValue, int id);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnChangeEditOverlayEditposFov();
	afx_msg void OnEnChangeEditOverlayEditposYaw();
	afx_msg void OnEnChangeEditOverlayEditposRoll();
	afx_msg void OnEnChangeEditOverlayEditposPitch();
	afx_msg void OnEnChangeEditOverlayEditposDiffyaw();
	afx_msg void OnEnChangeEditOverlayEditposDiffpitch();
	afx_msg void OnBnClickedBtnEditposApply();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
