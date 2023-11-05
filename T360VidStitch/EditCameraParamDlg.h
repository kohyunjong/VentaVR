#pragma once
#include "afxwin.h"


// EditCameraParamDlg ��ȭ �����Դϴ�.

class EditCameraParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditCameraParamDlg)

public:
	EditCameraParamDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~EditCameraParamDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EDITCAMERAPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
