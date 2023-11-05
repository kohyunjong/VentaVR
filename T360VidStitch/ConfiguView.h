#pragma once

#include "ConfiguChildView1.h"
#include "ConfiguChildView2.h"
#include "ConfiguChildView3.h"
// CConfiguView form view

class CConfiguView : public CFormView
{
	DECLARE_DYNCREATE(CConfiguView)

protected:
	CConfiguView();           // protected constructor used by dynamic creation
	virtual ~CConfiguView();
	friend class CT360VidStitchDlg;
public:
	enum { IDD = IDD_CONFIGURATIONVIEW_DLG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	enum vdstChildeView
	{
		childOuputs,
		childPanorama,
		childCalibration
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	vdstChildeView m_activeView;

	CConfiguChildView1* m_dlgChildOuputs;
	CConfiguChildView2* m_dlgChildPanorama;
	CConfiguChildView3* m_dlgChildCalibration;

	CGdipButton m_btnOutput;
	CGdipButton m_btnPanorama;
	CGdipButton m_btnCalibtation;
	CStatic m_ctlTopBar;
	CStatic m_ConfiguChildFrame;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnConfoutput();
	afx_msg void OnBnClickedBtnConfpanorama();
	afx_msg void OnBnClickedBtnConfcalra();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void OnInitialUpdate();
	virtual void PostNcDestroy();
	void ShowScreen(vdstChildeView scr, CRect rc);
	void ToggleButton(int nType);
	void clearValue(vdstChildeView scr);
	void setValue();
};


