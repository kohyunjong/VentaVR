#pragma once
#include "resource.h"
#include "TaskPanelDlg.h"
#include "afxwin.h"
// CConfOutputsChildOne 대화 상자입니다.

#define H264 0
#define HEVC 1

class CConfOutputsChildOne : public CTaskPanelDlg
{
	DECLARE_DYNAMIC(CConfOutputsChildOne)

public:
	CConfOutputsChildOne(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfOutputsChildOne();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW1_1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_btnSave;
	CBitmapButton m_btnOutputBrowser;
	CString GOP, VB, FPS;

	void SetValues();
	void GetValue();
	void SetFontObj();
	double OnChangeStrToDouble(CString strValue);

	CEdit m_editFPS;
	CEdit m_editVideoBit;
	CEdit m_editGOP;
	CComboBox m_cboCodec;
	CComboBox m_cboPreset;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnConfig11Save();
	afx_msg void OnBnClickedBtnConfig1Brow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCbnSelchangeCboCfoutCdec();
	afx_msg void OnEnChangeCfoutFps();
	afx_msg void OnEnChangeCfoutVb();
	afx_msg void OnEnChangeCfoutGop();
};
