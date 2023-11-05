#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
#include "afxwin.h"
// CConfOutputsChildTwo 대화 상자입니다.

class CConfOutputsChildTwo : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfOutputsChildTwo)

public:
	CConfOutputsChildTwo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfOutputsChildTwo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW1_2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
