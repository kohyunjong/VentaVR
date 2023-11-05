#pragma once
#include "afxwin.h"


// CAddOverlayChild3 ��ȭ �����Դϴ�.

class CAddOverlayChild3 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddOverlayChild3)

public:
	CAddOverlayChild3(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAddOverlayChild3();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ADDOVERLAY_CHILD3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
