#pragma once


// CRightSourceChildDlg ��ȭ �����Դϴ�.

class CRightSourceChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRightSourceChildDlg)

public:
	CRightSourceChildDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRightSourceChildDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RIGHTSOURCECHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
