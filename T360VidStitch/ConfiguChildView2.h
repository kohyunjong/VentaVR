#pragma once
#include "afxwin.h"
#include "ConfPanoramaChildOne.h"
#include "ConfPanoramaChildTwo.h"

// CConfiguChildView2 ��ȭ �����Դϴ�.

class CConfiguChildView2 : public CDialogEx
{
	DECLARE_DYNAMIC(CConfiguChildView2)

public:
	CConfiguChildView2(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfiguChildView2();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CXTPTaskPanel m_wndTaskPanel;
	CConfPanoramaChildOne m_dlgsubResolConf;
	CConfPanoramaChildTwo m_dlgsubRenderConf;

	CBitmapButton m_btnSave;
	CStatic m_ctrlFramePano;
	
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void SetValues();
};
