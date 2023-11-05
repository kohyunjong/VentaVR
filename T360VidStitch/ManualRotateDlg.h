#pragma once
#include "afxwin.h"


// CManualRotateDlg 대화 상자입니다.

class CManualRotateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualRotateDlg)

public:
	CManualRotateDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualRotateDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_ROTATE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CGdipButton m_btnOK;

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnSave();

	afx_msg void OnEnChangeEditMrotateYaw0();
	afx_msg void OnEnChangeEditMrotateYaw1();
	afx_msg void OnEnChangeEditMrotateYaw2();
	afx_msg void OnEnChangeEditMrotateYaw3();
	afx_msg void OnEnChangeEditMrotateYaw4();
	afx_msg void OnEnChangeEditMrotateYaw5();
	afx_msg void OnEnChangeEditMrotateYaw6();
	afx_msg void OnEnChangeEditMrotateYaw7();

	afx_msg void OnEnChangeEditMrotatePitch0();
	afx_msg void OnEnChangeEditMrotatePitch1();
	afx_msg void OnEnChangeEditMrotatePitch2();
	afx_msg void OnEnChangeEditMrotatePitch3();
	afx_msg void OnEnChangeEditMrotatePitch4();
	afx_msg void OnEnChangeEditMrotatePitch5();
	afx_msg void OnEnChangeEditMrotatePitch6();
	afx_msg void OnEnChangeEditMrotatePitch7();

	afx_msg void OnEnChangeEditMrotateRoll0();
	afx_msg void OnEnChangeEditMrotateRoll1();
	afx_msg void OnEnChangeEditMrotateRoll2();
	afx_msg void OnEnChangeEditMrotateRoll3();
	afx_msg void OnEnChangeEditMrotateRoll4();
	afx_msg void OnEnChangeEditMrotateRoll5();
	afx_msg void OnEnChangeEditMrotateRoll6();
	afx_msg void OnEnChangeEditMrotateRoll7();

	void subChangeEdit(int IDC);
	void SetFontObj();
};
