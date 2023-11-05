#pragma once
#include "afxwin.h"


// EditCameraParamDlg 대화 상자입니다.

class EditCameraParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditCameraParamDlg)

public:
	EditCameraParamDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~EditCameraParamDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EDITCAMERAPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void UpdateDialogItemValue();
	double OnChangeStrToDouble(CString strValue, int id);
	double OnCheckRangeOfFOV(int CulSel, double fov);

	bool isErrorOfFOV = false, isErrorOfFPS = false;
	CString fpsErrorMsg = _T("(FPS : 12.0 ~ 60.0, "), fovErrorMsg = _T("");

	CComboBox m_cbpLensType;
	afx_msg void OnEnChangeEdtCipFov();
	afx_msg void OnEnChangeEdtCipFps();
	afx_msg void OnBnClickedBtncipSave();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CGdipButton m_btnSave;
	CString FPS, FOV;
	CEdit m_editFPS;
	CEdit m_editFOV;
	virtual BOOL OnInitDialog();
};
