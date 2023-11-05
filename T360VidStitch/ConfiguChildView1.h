#pragma once
#include "ConfOutputsChildOne.h"
#include "ConfOutputsChildTwo.h"
#include "ConfOutputsChildThree.h"
#include "ConfOutputsChildFour.h"
// CConfiguChildView1 ��ȭ �����Դϴ�.

class CConfiguChildView1 : public CDialogEx
{
	DECLARE_DYNAMIC(CConfiguChildView1)

public:
	CConfiguChildView1(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfiguChildView1();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CXTPTaskPanel m_wndTaskPanel;
	CConfOutputsChildOne m_dlgsubHDDConf;
	CConfOutputsChildTwo m_dlgsubRTMPConf;
	CConfOutputsChildThree m_dlgsubBlackMConf;
	CConfOutputsChildFour m_dlgsubVodStreamConf;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void setValue();
};
