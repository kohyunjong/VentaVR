#pragma once


// CRightSourceChildDlg 대화 상자입니다.

class CRightSourceChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRightSourceChildDlg)

public:
	CRightSourceChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRightSourceChildDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RIGHTSOURCECHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnsnapshot();
	CGdipButton m_btnSnapshot;
	CGdipButton m_btnCalibration;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtncalibration2();
};
