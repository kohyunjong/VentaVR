#pragma once
#include "afxwin.h"


// CRightOverlayListChildDlg ��ȭ �����Դϴ�.

class CRightOverlayListChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRightOverlayListChildDlg)

public:
	CRightOverlayListChildDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRightOverlayListChildDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RIGHTOVERLAYLISTCHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void UpdateRecentListContrlValue();

	CGdipButton m_btnBack;
	CListCtrl m_listOverlay;

	int setOverlayTemplateState(int idx, bool changePlayMode);
	void dlgUpdate();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnNMDblclkListOverlaylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnback();
	afx_msg void OnNMClickListOverlaylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListOverlaylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnOverlaypopupEdit();
	afx_msg void OnOverlaypopupShow();
	afx_msg void OnOverlaypopupHide();
	afx_msg void OnOverlaypopupClear();
};
