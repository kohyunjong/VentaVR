#pragma once
#include "locale.h"
#include "afxlinkctrl.h"

#include "NewLoadConfigChild.h"
#include "afxwin.h"
#include "afxcmn.h"

#define QVS_VERSION_RELEASE  2
#define QVS_VERSION_MAJOR	 4
#define QVS_VERSION_MINOR	 8
#define LOADBTNWIDTH 250
// CNewLoadView form view

class CNewLoadView : public CFormView
{
	DECLARE_DYNCREATE(CNewLoadView)

protected:
	CNewLoadView();           // protected constructor used by dynamic creation
	virtual ~CNewLoadView();
	friend class CT360VidStitchDlg;

public:
	enum { IDD = IDD_NEWLOADVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CGdipButton m_btnNew;
	CGdipButton m_btnLoad;
	CGdipButton m_btnHomeURL;

	CNewLoadConfigChild* m_pNewLoadConfigChild;
	
	CStatic m_staticRecentTitle;
	CListCtrl m_listRecentProject;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnNew();
	afx_msg void OnBnClickedBtnLoad();
	virtual void OnInitialUpdate();
	void LoadFormFile(CString szFullpath, CString szFileName);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNMDblclkListRecent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListRecent(NMHDR *pNMHDR, LRESULT *pResult);
	void SetFontObj();
	void UpdateRecentListContrlValue();
	void resetUI();

	BOOL ParseLoadValueFormConfiguFile(CString szFullpath, CString szFileName);
	virtual void PostNcDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Arrange();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	CMFCLinkCtrl m_releasenote_url;
	afx_msg void OnPaint();

	QLicense* mQLicense = NULL;
	int HL_key_id = -1;
	int HL_key_year = -1, HL_key_month = -1, HL_key_day = -1;
	CStatic m_ConfiguChildFrame;
	afx_msg void OnBnClickedBtnUrl();
	CStatic m_staticNewProjectTitle;
	CLinkCtrl m_linkContact;
	afx_msg void OnNMClickSyslinkContact(NMHDR *pNMHDR, LRESULT *pResult);
};


