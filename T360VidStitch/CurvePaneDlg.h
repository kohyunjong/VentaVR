
// CurvePaneDlg.h : ��� ����
//

#pragma once
#include "CustCurveCtrl.h"
#include "afxwin.h"

// CCurvePaneDlg ��ȭ ����
class CCurvePaneDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CCurvePaneDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CURVEPANE_DIALOG };

	CustCurveCtrl m_covCtrl;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	
public:
	
	CComboBox m_comboChannelList;
	afx_msg void OnBnClickedButtonSave();
	int m_iComboChannel;
	afx_msg void OnCbnSelchangeComboChannel();
	CEdit m_editInput;
	CEdit m_editOutput;

	double	m_Input;
	double	m_Output;

	POINT m_pointRed_1;
	POINT m_pointRed_2;
	POINT m_pointRed_3;

	POINT m_pointGreen_1;
	POINT m_pointGreen_2;
	POINT m_pointGreen_3;

	POINT m_pointBlue_1;
	POINT m_pointBlue_2;
	POINT m_pointBlue_3;

	int m_nActiveIdx;

	afx_msg void OnEnChangeEditInput();
	afx_msg void OnEnChangeEditOutput();
	void UpdateIOEditCtrl(int Index);

	CGdipButton m_btnSave;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void LoadButtonImage(BOOL bShow);
	CGdipButtonExt m_btnThumb1;
	CGdipButtonExt m_btnThumb2;
	CGdipButtonExt m_btnThumb3;
	CGdipButtonExt m_btnThumb4;
	CGdipButtonExt m_btnThumb5;
	CGdipButtonExt m_btnThumb6;
	CGdipButtonExt m_btnThumb7;
	CGdipButtonExt m_btnThumb8;

	int m_nOldBtnIdx;
	BOOL m_bCompareButton;

	void ClickButtons(int i);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonImage1();
	afx_msg void OnBnClickedButtonImage2();
	afx_msg void OnBnClickedButtonImage3();
	afx_msg void OnBnClickedButtonImage4();
	afx_msg void OnBnClickedButtonImage5();
	afx_msg void OnBnClickedButtonImage6();
	afx_msg void OnBnClickedButtonImage7();
	afx_msg void OnBnClickedButtonImage8();
	double OnChangeStrToDouble(CString strValue , int nIdx);	// �Է¹��� ���ڿ� , Input/Output Ȯ���ϴ� IDX  ( 0 : Input , 1 : Output )
	void SetControlValue();
	void GetControlValue(int index);
	void GetControlValueAll();
	void InitCurveGraph();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
