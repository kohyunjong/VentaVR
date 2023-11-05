#pragma once
#include "afxwin.h"


// CEditInputDlg 대화 상자입니다.

class CEditInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditInputDlg)

public:
	CEditInputDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EDITINPUTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CComboBox m_cboInput0;
	CComboBox m_cboInput1;
	CComboBox m_cboInput2;
	CComboBox m_cboInput3;
	CComboBox m_cboInput4;
	CComboBox m_cboInput5;
	CComboBox m_cboInput6;
	CComboBox m_cboInput7;
	afx_msg void OnCbnSelchangeCboCinput1();
	afx_msg void OnCbnSelchangeCboCinput2();
	afx_msg void OnCbnSelchangeCboCinput3();
	afx_msg void OnCbnSelchangeCboCinput4();
	afx_msg void OnCbnSelchangeCboCinput5();
	afx_msg void OnCbnSelchangeCboCinput6();
	afx_msg void OnCbnSelchangeCboCinput7();
	afx_msg void OnCbnSelchangeCboCinput8();
	void updateEditAspectRatio();
	void updateCheckBoxState(int state = 0);
	int CheckStilImageSelect(int nIDX);
	void closeDlg();
	CGdipButton m_btnSave;
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool isNeedLoading = false;
	bool isInputCameraSelecting = false;
	bool isCFileDlgOpening = false;
	afx_msg void OnBnClickedBtnSmallReset0();
	afx_msg void OnBnClickedBtnSmallReset1();
	afx_msg void OnBnClickedBtnSmallReset2();
	afx_msg void OnBnClickedBtnSmallReset3();
	afx_msg void OnBnClickedBtnSmallReset4();
	afx_msg void OnBnClickedBtnSmallReset5();
	afx_msg void OnBnClickedBtnSmallReset6();
	afx_msg void OnBnClickedBtnSmallReset7();
	afx_msg void OnBnClickedCheckSysc();
	afx_msg void OnEnChangeEditInputAspectratioX0();
	afx_msg void OnEnChangeEditInputAspectratioY0();
	CGdipButton m_btnRatioReset0;
	CGdipButton m_btnRatioReset1;
	CGdipButton m_btnRatioReset2;
	CGdipButton m_btnRatioReset3;
	CGdipButton m_btnRatioReset4;
	CGdipButton m_btnRatioReset5;
	CGdipButton m_btnRatioReset6;
	CGdipButton m_btnRatioReset7;
};
