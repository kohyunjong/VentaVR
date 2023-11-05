#pragma once
#include "ConfCalibrationChildOne.h"
#include "ConfCalibrationChildTwo.h"
#include "ConfCalibrationChildThree.h"

// CConfiguChildView3 대화 상자입니다.

class CConfiguChildView3 : public CDialogEx
{
	DECLARE_DYNAMIC(CConfiguChildView3)

public:
	CConfiguChildView3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfiguChildView3();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIGCHILDVIEW3_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CXTPTaskPanel m_wndTaskPanel;
	CConfCalibrationChildOne m_dlgsubNewCalConf;
	CConfCalibrationChildTwo m_dlgsubImportCalConf;
	CConfCalibrationChildThree m_dlgsubSnapshotDSConf;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void clearValue();
	void setValue();
};
