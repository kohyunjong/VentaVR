#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
#include "afxwin.h"
// CConfOutputsChildTwo ��ȭ �����Դϴ�.

class CConfOutputsChildTwo : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfOutputsChildTwo)

public:
	CConfOutputsChildTwo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfOutputsChildTwo();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONFIGCHILDVIEW1_2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_btnSave;

	void SetValues();
	void GetValue();
	void SetFontObj();
	double OnChangeStrToDouble(CString strValue);

	CEdit m_editFPS;
	CEdit m_editVB;
	CEdit m_editGOP;
	CString GOP, FPS, VB;
	CComboBox m_cboCodec;
	CComboBox m_cboPreset;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnConfig12Save();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCbnSelchangeCboCfout2Cdec();
	afx_msg void OnEnChangeCfout2Fps();
	afx_msg void OnEnChangeCfout2Vb();
	afx_msg void OnEnChangeCfout2Gop();
};
