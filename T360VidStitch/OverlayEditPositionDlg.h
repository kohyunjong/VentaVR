#pragma once
#include "afxwin.h"


// COverlayEditPositionDlg ��ȭ �����Դϴ�.

class COverlayEditPositionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COverlayEditPositionDlg)

public:
	COverlayEditPositionDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COverlayEditPositionDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OVERLAY_EDITPOSITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
