#pragma once
#include "afxwin.h"
#include "ConfPanoramaChildOne.h"
#include "ConfPanoramaChildTwo.h"

// CConfiguChildView2 대화 상자입니다.

class CConfiguChildView2 : public CDialogEx
{
	DECLARE_DYNAMIC(CConfiguChildView2)

public:
	CConfiguChildView2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfiguChildView2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
