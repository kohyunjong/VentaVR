#pragma once
#include "afxwin.h"


// CAddOverlayChild3 대화 상자입니다.

class CAddOverlayChild3 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddOverlayChild3)

public:
	CAddOverlayChild3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddOverlayChild3();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADDOVERLAY_CHILD3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	bool isEnableDone = false;
	bool isCheckedCrop = false;
	float fps;

	CComboBox m_cboCam;
	CComboBox m_cboFPS;
	CEdit m_editFPS;
	CEdit m_editLeft;
	CEdit m_editTop;
	CEdit m_editRight;
	CEdit m_editBottom;
	CEdit m_editWidth;
	CEdit m_editHeight;

	afx_msg void OnCbnSelchangeCboOverlayCam();
	afx_msg void OnBnClickedCheckCrop();
	afx_msg void OnEnChangeEcOverlayWdt();
	afx_msg void OnEnChangeEcOverlayHgt();
	afx_msg void OnEnChangeEcOverlayFps();

	double OnChangeStrToDouble(CString strValue);
};
